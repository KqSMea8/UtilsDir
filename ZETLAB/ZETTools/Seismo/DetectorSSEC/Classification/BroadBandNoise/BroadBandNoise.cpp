//------------------------------------------------------------------------------
#include "stdafx.h"
#include "BroadBandNoise.h"
//------------------------------------------------------------------------------
namespace zet {
namespace algorithm {
namespace dsp {
namespace seismo {
//------------------------------------------------------------------------------
CBroadBandNoise::CBroadBandNoise() : CCustomFalseEvent()
{	// задание значений указателей из базового класса
	m_bOriginalSignal = true;

	m_type_Event_Algr = tea_ActualEvent;
	m_pConfig = &m_config.zet_header;

	long r = ztOpenStructure(m_pConfig,
		ZETLAB_SSM_CFG_ALGR_BROADBANDNOISE_TYPE, ZETLAB_SSM_CFG_ALGR_BROADBANDNOISE_SIZE);

	// структура результата см. базовый класс

	for (int i = 0; i < 3; ++i)
	{
		m_pFiltrHighPass[i] = new CIirFiltrInternalDouble();
		m_pFiltrBandPass[i] = new CIirFiltrBandDouble();
	}
}
//------------------------------------------------------------------------------
CBroadBandNoise::CBroadBandNoise(const CBroadBandNoise & val) :
					CCustomFalseEvent(val)
{
	Equal(val);
}
//------------------------------------------------------------------------------
CBroadBandNoise::~CBroadBandNoise()
{
	for (int i = 0; i < 3; ++i)
	{
		if (m_pFiltrHighPass[i])
		{
			delete m_pFiltrHighPass[i];
			m_pFiltrHighPass[i] = nullptr;
		}
		if (m_pFiltrBandPass[i])
		{
			delete m_pFiltrBandPass[i];
			m_pFiltrBandPass[i] = nullptr;
		}
	}
}
//------------------------------------------------------------------------------
void CBroadBandNoise::operator = (const CBroadBandNoise &val)
{
	*((CCustomFalseEvent*)this) = *((CCustomFalseEvent*)&val);
	Equal(val);
}
//------------------------------------------------------------------------------
void CBroadBandNoise::Equal(const CBroadBandNoise & val)
{
	m_cfgClass = val.m_cfgClass;
	m_config = val.m_config;

	m_saDouble = val.m_saDouble;
	for (int i = 0; i < 4; ++i)
		m_saXYZE[i] = val.m_saXYZE[i];

	for (int i = 0; i < 3; ++i)
	{
		if (val.m_pFiltrHighPass[i] != nullptr)
		{// в val фильтр есть
			if (m_pFiltrHighPass[i] == nullptr)
				m_pFiltrHighPass[i] = new CIirFiltrInternalDouble();
			m_pFiltrHighPass[i]->Create(&m_freqADCdouble);
			m_pFiltrHighPass[i]->InitFiltr(tfHighPass, (double)m_config.freqBoundary);
		}
		else
		{// в val фильтра нет
			if (m_pFiltrHighPass[i] != nullptr)
			{
				delete m_pFiltrHighPass[i];
				m_pFiltrHighPass[i] = nullptr;
			}
		}

		if (val.m_pFiltrBandPass[i] != nullptr)
		{// в val фильтр есть
			if (m_pFiltrBandPass[i] == nullptr)
				m_pFiltrBandPass[i] = new CIirFiltrBandDouble();
			m_pFiltrBandPass[i]->Create(&m_freqADCdouble);
			m_pFiltrBandPass[i]->InitFiltr(tfBandPass,
				(double)m_config.freqStart, (double)m_config.freqBoundary);
		}
		else
		{// в val фильтра нет
			if (m_pFiltrBandPass[i] != nullptr)
			{
				delete m_pFiltrBandPass[i];
				m_pFiltrBandPass[i] = nullptr;
			}
		}
	}
}
//------------------------------------------------------------------------------
long CBroadBandNoise::ReStart()
{
	bool ret(true);
	for (int i = 0; i < 3; ++i)
	{
		if (m_pFiltrHighPass[i] != nullptr)
			ret &= m_pFiltrHighPass[i]->ReStart();
		if (m_pFiltrBandPass[i] != nullptr)
			ret &= m_pFiltrBandPass[i]->ReStart();
	}
	return ret ? ErrorCodes::NoError : ErrorCodes::FatalError;
}
//------------------------------------------------------------------------------
long CBroadBandNoise::SetConfigFromProxy(const DATE timeSRV)
{// копирование cfg-параметров из Proxy в структуру класса
	m_config = m_cfgClass;
	m_result.valueThreshold = m_config.threshold;
	for (int i = 0; i < 3; ++i)
	{
		m_pFiltrHighPass[i]->Create(&m_freqADCdouble);
		m_pFiltrHighPass[i]->InitFiltr(tfHighPass, (double)m_config.freqBoundary);

		m_pFiltrBandPass[i]->Create(&m_freqADCdouble);
		m_pFiltrBandPass[i]->InitFiltr(tfBandPass,
			(double)m_config.freqStart, (double)m_config.freqBoundary);
	}
	return ztSetStructureTimeLocal(m_pConfig, timeSRV);
}
//------------------------------------------------------------------------------
long CBroadBandNoise::SetConfig(ZetHeader* pParam, const DATE timeSRV)
{	// проверка пришедшей структуры
	long ret = CCustomAlgorithm::SetConfig(pParam, timeSRV);
	if (ret == NoError)
	{	// копирование этой структуры в Proxy с целью проверки полей структуры
		m_cfgClass = *((Ssm_cfg_Algr_BroadBandNoise*)pParam);
		// копирование параметрров из Proxy в структуру класса
		ret = SetConfigFromProxy(timeSRV);
	}
	return ret;
}
//------------------------------------------------------------------------------
// —игнал алгоритма - это отношение RMS сигнала в высокочастотной области
// к RMS сигнала в рабочей области. ѕорог не адаптивный.
bool CBroadBandNoise::CalcSignalAlgoritm(
	const float *pDataX, const float *pDataY, const float *pDataZ,
	const int size, float & signal)
{
	float sigHigh(0.f), sigLow(0.f);
	// высокочастотна€ область
	ippsConvert_32f64f(pDataX, m_saDouble.pData, size);
	m_pFiltrHighPass[0]->Filtration_I(m_saDouble.pData, size);
	ippsConvert_64f32f(m_saDouble.pData, m_saXYZE[0].pData, size);
	ippsSqr_32f_I(m_saXYZE[0].pData, size);

	ippsConvert_32f64f(pDataY, m_saDouble.pData, size);
	m_pFiltrHighPass[1]->Filtration_I(m_saDouble.pData, size);
	ippsConvert_64f32f(m_saDouble.pData, m_saXYZE[1].pData, size);
	ippsSqr_32f_I(m_saXYZE[1].pData, size);

	ippsConvert_32f64f(pDataZ, m_saDouble.pData, size);
	m_pFiltrHighPass[2]->Filtration_I(m_saDouble.pData, size);
	ippsConvert_64f32f(m_saDouble.pData, m_saXYZE[2].pData, size);
	ippsSqr_32f_I(m_saXYZE[2].pData, size);
	
	ippsAdd_32f(m_saXYZE[0].pData, m_saXYZE[1].pData, m_saXYZE[3].pData, size);
	ippsAdd_32f_I(m_saXYZE[2].pData, m_saXYZE[3].pData, size);
	ippsMean_32f(m_saXYZE[3].pData, size, &sigHigh, ippAlgHintFast);

	// низкочастотна€ область
	ippsConvert_32f64f(pDataX, m_saDouble.pData, size);
	m_pFiltrBandPass[0]->Filtration_I(m_saDouble.pData, size);
	ippsConvert_64f32f(m_saDouble.pData, m_saXYZE[0].pData, size);
	ippsSqr_32f_I(m_saXYZE[0].pData, size);

	ippsConvert_32f64f(pDataY, m_saDouble.pData, size);
	m_pFiltrBandPass[1]->Filtration_I(m_saDouble.pData, size);
	ippsConvert_64f32f(m_saDouble.pData, m_saXYZE[1].pData, size);
	ippsSqr_32f_I(m_saXYZE[1].pData, size);

	ippsConvert_32f64f(pDataZ, m_saDouble.pData, size);
	m_pFiltrBandPass[2]->Filtration_I(m_saDouble.pData, size);
	ippsConvert_64f32f(m_saDouble.pData, m_saXYZE[2].pData, size);
	ippsSqr_32f_I(m_saXYZE[2].pData, size);

	ippsAdd_32f(m_saXYZE[0].pData, m_saXYZE[1].pData, m_saXYZE[3].pData, size);
	ippsAdd_32f_I(m_saXYZE[2].pData, m_saXYZE[3].pData, size);
	ippsMean_32f(m_saXYZE[3].pData, size, &sigLow, ippAlgHintFast);

	bool ret = sigLow > 0.f;
	signal = ret ? (sigLow / sigHigh) : 0.f;
	return ret;
}
//------------------------------------------------------------------------------
long CBroadBandNoise::FalseEvent(
	const float *pDataX,
	const float *pDataY,
	const float *pDataZ,
	const DWORD number)
{
	m_result.result = 0;
	m_result.value = 0.f;
	m_result.valueSNR = 0;
	m_result.valueThreshold = m_config.threshold;

	int size = (int)number;
	long ret = ((size > 0) &&
		(pDataX != nullptr) && (pDataY != nullptr) && (pDataZ != nullptr)) ?
		ErrorCodes::NoError : ErrorCodes::Err_NULL_Pointer;
	if (ret == ErrorCodes::NoError)
	{
		if (m_saDouble.ReSize(number) &&
			m_saXYZE[0].ReSize(number) && m_saXYZE[1].ReSize(number) &&
			m_saXYZE[2].ReSize(number) && m_saXYZE[3].ReSize(number))
		{
			float signalPre(0.), signalEvent(0.f);
			bool flag = CalcSignalAlgoritm(
							pDataX, pDataY, pDataZ,
							m_sizePreHistory, signalPre) &&
						CalcSignalAlgoritm(
							pDataX + m_sizePreHistory, pDataY + m_sizePreHistory,
							pDataZ + m_sizePreHistory,
							size - m_sizePreHistory, signalEvent);
			
			m_result.result = (signalEvent > m_config.threshold) ? 1 : 0;
			m_result.value = signalEvent;
			m_result.valueSNR = (signalPre > 0.f) ? (signalEvent / signalPre) : 0.f;

			ret = flag ? ErrorCodes::NoError : ErrorCodes::Err_Calc;
		}
		else
			ret = ErrorCodes::Err_Create_Array;
	}
	return ret;
}
//------------------------------------------------------------------------------
}
}
}
}
//------------------------------------------------------------------------------