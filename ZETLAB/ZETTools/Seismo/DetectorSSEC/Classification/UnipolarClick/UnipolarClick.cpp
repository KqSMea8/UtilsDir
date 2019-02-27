#include "stdafx.h"
#include "UnipolarClick.h"
//------------------------------------------------------------------------------------------
namespace zet {
namespace algorithm {
namespace dsp {
namespace seismo {
//------------------------------------------------------------------------------------------
CUnipolarClick::CUnipolarClick() : CCustomFalseEvent()
{
	m_bOriginalSignal = true;

	m_type_Event_Algr = tea_FalseEvent;
	m_pConfig = &m_config.zet_header;

	// задание конфигурационных параметров значениями по умолчанию
	long r = ztOpenStructure(m_pConfig, ZETLAB_SSM_CFG_ALGR_UNIPOLARCLICK_TYPE, ZETLAB_SSM_CFG_ALGR_UNIPOLARCLICK_SIZE);

	for (int i = 0; i < 3; ++i)
		m_pFiltrDiff[i] = new CIirFiltrSimpleDouble();
}
//------------------------------------------------------------------------------------------
CUnipolarClick::CUnipolarClick(const CUnipolarClick & val) : CCustomFalseEvent(val)
{
	Equal(val);
}
//------------------------------------------------------------------------------------------
CUnipolarClick::~CUnipolarClick()
{
	for (int i = 0; i < 3; ++i)
	{
		if (m_pFiltrDiff[i] != nullptr)
		{
			delete m_pFiltrDiff[i];
			m_pFiltrDiff[i] = nullptr;
		}
	}
}
//------------------------------------------------------------------------------------------
void CUnipolarClick::operator = (const CUnipolarClick & val)
{
	*((CCustomFalseEvent*)this) = *((CCustomFalseEvent*)&val);
	Equal(val);
}
//------------------------------------------------------------------------------------------
void CUnipolarClick::Equal(const CUnipolarClick & val)
{
	m_cfgClass = val.m_cfgClass;
	m_config = val.m_config;

	m_saDouble = val.m_saDouble;
	for (int i = 0; i < 3; ++i)
		m_saXYZE[i] = val.m_saXYZE[i];

	for (int i = 0; i < 3; ++i)
	{
		if (val.m_pFiltrDiff[i] != nullptr)
		{// в val фильтр есть
			if (m_pFiltrDiff[i] == nullptr)
				m_pFiltrDiff[i] = new CIirFiltrSimpleDouble();
			m_pFiltrDiff[i]->Create(&m_freqADCdouble);
			m_pFiltrDiff[i]->InitFiltr(tfDiff1);
		}
		else
		{// в val фильтра нет
			if (m_pFiltrDiff[i] != nullptr)
			{
				delete m_pFiltrDiff[i];
				m_pFiltrDiff[i] = nullptr;
			}
		}
	}
}
//------------------------------------------------------------------------------------------
void CUnipolarClick::SetFreqADC(double *pFreqADC)
{
	for (int i = 0; i < 3; ++i)
	{
		m_pFiltrDiff[i]->Create(pFreqADC);
		m_pFiltrDiff[i]->InitFiltr(tfDiff1);
	}
}
//------------------------------------------------------------------------------------------
long CUnipolarClick::ReStart()
{
	bool ret(true);
	for (int i = 0; i < 3; ++i)
	{
		ret &= m_pFiltrDiff[i]->ReStart();
	}
	return ret ? ErrorCodes::NoError : ErrorCodes::FatalError;
}
//------------------------------------------------------------------------------------------
long CUnipolarClick::SetConfigFromProxy(const DATE timeSRV)
{// копирование cgf-параметров из Proxy в структуру класса
	m_config = m_cfgClass;
	m_result.valueThreshold = m_config.thresholdRatio;
	SetFreqADC(&m_freqADCdouble);

	return ztSetStructureTimeLocal(m_pConfig, timeSRV);
}
//------------------------------------------------------------------------------------------
long CUnipolarClick::SetConfig(ZetHeader* pParam, const DATE timeSRV)
{	// проверка пришедшей структуры
	long ret = CCustomAlgorithm::SetConfig(pParam, timeSRV);
	if (ret == NoError)
	{	// копирование этой структуры в Proxy с целью проверки полей структуры
		m_cfgClass = *((Ssm_cfg_Algr_UnipolarClick*)pParam);
		// копирование параметрров из Proxy в структуру класса
		ret = SetConfigFromProxy(timeSRV);
	}
	return ret;
}
//------------------------------------------------------------------------------------------
long CUnipolarClick::FalseEvent(const float *pDataX,
	const float *pDataY, const float *pDataZ, const DWORD number)
{
	m_result.value = 0.f;
	m_result.result = 0;
	m_result.valueSNR = 0;

	int size = (int)number;
	long ret = ((size > 0) &&
		(pDataX != nullptr) && (pDataY != nullptr) && (pDataZ != nullptr)) ?
		ErrorCodes::NoError : ErrorCodes::Err_NULL_Pointer;
	if (ret == ErrorCodes::NoError)
	{
		float signalPre(0.f), signal(0.f);
		bool bEvent(false);

		if (// Расчёты по предыстории
			CalcSignalAlgoritm(pDataX, pDataY, pDataZ, m_sizePreHistory, signalPre, bEvent) &&
			// Расчёты по событию
			CalcSignalAlgoritm(
				pDataX + m_sizePreHistory, pDataY + m_sizePreHistory,
				pDataZ + m_sizePreHistory, (int)number - m_sizePreHistory,
				signal, bEvent) )
		{
			m_result.result = bEvent ? 1 : 0;
			m_result.value = signal;
			m_result.valueSNR = signal / signalPre;
		}
		else
			ret = ErrorCodes::Err_Create_Array;
	}
	return ret;
}
//------------------------------------------------------------------------------------------
// перед вызовом этой функции выполняется проверка указателей и размера
bool CUnipolarClick::CalcSignalAlgoritm(
	const float *pDataX, const float *pDataY, const float *pDataZ,
	const int size, float & signal, bool & bFalseEvent)
{
	bool ret = m_saDouble.ReSize(size);
	signal = 0.f;
	bFalseEvent = false;
	if (ret)
	{
		float peakValue(0.f), ratio(0.f);
		if (m_saXYZE[0].ReSize(size))
		{
			if ((ippsCopy_32f(pDataX, m_saXYZE[0].pData, size) == ippStsNoErr) &&
				CalcOneComponent(m_saXYZE[0].pData, size,
				peakValue, ratio,
				m_pFiltrDiff[0]))
			{// надо, чтобы функция всегда вызывалась
				signal = ratio;
				bFalseEvent |= ratio > m_config.thresholdRatio;
			}
		}
		if (m_saXYZE[1].ReSize(size))
		{
			if ((ippsCopy_32f(pDataY, m_saXYZE[1].pData, size) == ippStsNoErr) &&
				CalcOneComponent(m_saXYZE[1].pData, size,
				peakValue, ratio,
				m_pFiltrDiff[1]))
			{// надо, чтобы функция всегда вызывалась
				signal = std::max(signal, ratio);
				bFalseEvent |= ratio > m_config.thresholdRatio;
			}
		}
		if (m_saXYZE[2].ReSize(size))
		{
			if ((ippsCopy_32f(pDataZ, m_saXYZE[2].pData, size) == ippStsNoErr) &&
				CalcOneComponent(m_saXYZE[2].pData, size,
				peakValue, ratio,
				m_pFiltrDiff[2]))
			{// надо, чтобы функция всегда вызывалась
				signal = std::max(signal, ratio);
				bFalseEvent |= ratio > m_config.thresholdRatio;
			}
		}
	}
	return ret;
}
//------------------------------------------------------------------------------------------
bool CUnipolarClick::CalcOneComponent(float *pData, const int size, float &peakPreHistory,
	float &ratio, CIirFiltrSimpleDouble* pFiltr)
{
	bool ret = false;
	if (pData != nullptr)
	{
		ret = 
			(ippsConvert_32f64f(pData, m_saDouble.pData, size) == ippStsNoErr) &&
			pFiltr->Filtration_I(m_saDouble.pData, size) &&
			(ippsConvert_64f32f(m_saDouble.pData, pData, size) == ippStsNoErr);

		if (ret)
		{
			float fMin(0.f), fMax(0.f);
			ippsMinMax_32f(pData + 2, size - 2, &fMin, &fMax);
			ratio = CalcRatio2(fabsf(fMax), fabsf(fMin));
			
			/*float peakCoeff = peakPreHistory * m_config.coeffPeak;
			std::vector<float> minValues, maxValues;
			float maxE(peakCoeff), minE(peakCoeff);
			for (int i = 2; i < size; ++i)
			{
				if (pData[i] > peakCoeff)
				{
					maxValues.push_back(pData[i]);
				}
				else if (pData[i] < -peakCoeff)
				{
					minValues.push_back(pData[i]);
				}
			}
			if (!maxValues.empty())
			{
				float *pointer = maxValues.data();
				int len = maxValues.size();
				ippsSqr_32f_I(pointer, len);
				ippsMean_32f(pointer, len, &maxE, ippAlgHintNone);
				maxE = sqrt(maxE);
			}
			if (!minValues.empty())
			{
				float *pointer = minValues.data();
				int len = minValues.size();
				ippsSqr_32f_I(pointer, len);
				ippsMean_32f(pointer, len, &minE, ippAlgHintNone);
				minE = sqrt(minE);
			}
			ratio = CalcRatio2(maxE, minE);*/
		}
	}
	return ret;
}
//------------------------------------------------------------------------------------------
}//namespace seismo
}//namespace dsp
}//namespace algorithm
}//namespace zet
//------------------------------------------------------------------------------------------