#include "stdafx.h"
#include "UnipolarSignal.h"
//------------------------------------------------------------------------------------------
namespace zet {
namespace algorithm {
namespace dsp {
namespace seismo {
//------------------------------------------------------------------------------------------
	CUnipolarSignal::CUnipolarSignal()
	: CCustomFalseEvent()
{
	m_pConfig = &m_config.zet_header;
	m_pDiagns = &m_diagns.zet_header;

	// задание конфигурационных параметров значениями по умолчанию
	OpenStructure(m_pConfig, ZETLAB_SSM_CFG_ALGR_UNIPOLARSIGNAL_TYPE, ZETLAB_SSM_CFG_ALGR_UNIPOLARSIGNAL_SIZE);
	CloseStructure(m_pConfig, true);

	// структура диагностических данных
	OpenStructure(m_pDiagns, ZETLAB_SSM_DIA_ALGR_UNIPOLARSIGNAL_TYPE, ZETLAB_SSM_DIA_ALGR_UNIPOLARSIGNAL_SIZE);

	// структура результата см. базовый класс
}
//--------------------------------------------------------------
CUnipolarSignal::CUnipolarSignal(
		const CUnipolarSignal & val) : CCustomFalseEvent(val)
{
	m_saHist = val.m_saHist;

	m_cfgClass = val.m_cfgClass;
	m_config = val.m_config;
	m_diagns = val.m_diagns;
}
//--------------------------------------------------------------
CUnipolarSignal::~CUnipolarSignal()
{}
//--------------------------------------------------------------
void CUnipolarSignal::operator = (const CUnipolarSignal & val)
{
	*((CCustomFalseEvent*)this) = *((CCustomFalseEvent*)&val);
	m_saHist = val.m_saHist;

	m_cfgClass = val.m_cfgClass;
	m_config = val.m_config;
	m_diagns = val.m_diagns;
}
//--------------------------------------------------------------
ErrorCode CUnipolarSignal::SetConfigFromProxy()
{// копирование cgf-параметров из Proxy в структуру класса
	ErrorCode ret(NoError);
	try
	{
		m_config = m_cfgClass;
		CloseStructure(m_pConfig, true);
	}
	catch (ZetException&)
	{
		ret = Err_Copy_Structure;
	}
	return ret;
}
//--------------------------------------------------------------
ErrorCode CUnipolarSignal::SetConfig(ZetHeader* pParam)
{	// проверка пришедшей структуры
	ErrorCode ret = CCustomAlgorithm::SetConfig(pParam);
	if (ret == NoError)
	{	// копирование этой структуры в Proxy с целью проверки полей структуры
		try
		{
			m_cfgClass = *((Ssm_cfg_Algr_UnipolarSignal*)pParam);
		}
		catch (ZetException&)
		{
			ret = ErrorCodes::InvalidParams;
		}
		// копирование параметрров из Proxy в структуру класса
		SetConfigFromProxy();
	}
	return ret;
}
//--------------------------------------------------------------
ErrorCode CUnipolarSignal::FalseEvent(const float *pDataX,
	const float *pDataY, const float *pDataZ, const DWORD number)
{// ВОЗМОЖНО ИСПОЛЬЗОВАНИЕ АЛГОРИТМА В ОДНОКАНАЛЬНОМ РЕЖИМЕ
	m_diagns.meanX = m_diagns.meanY = m_diagns.meanZ = 0.f;
	m_diagns.deltaMeanX = m_diagns.deltaMeanY = m_diagns.deltaMeanZ = 0.f;
	m_diagns.valueDownX = m_diagns.valueDownY = m_diagns.valueDownZ = 0.f;
	m_diagns.valueUpX = m_diagns.valueUpY = m_diagns.valueUpZ = 0.f;
	m_diagns.ratioX = m_diagns.ratioY = m_diagns.ratioZ = 0.f;

	m_result.result = 0;
	m_result.value = 0.f;

	float val(0.f);
	int size = (int)number;
	ErrorCode ret = (size > 0) ? ErrorCodes::NoError : ErrorCodes::FatalError;
	if (ret == 0)
	{
		float maxRatio(0.f);
		bool bFalseEvent(false);
		bool bx = pDataX != nullptr;
		bool by = pDataY != nullptr;
		bool bz = pDataZ != nullptr;
		if (bx || by || bz)
		{
			if (bx)
			{
				if (CalcOneComponent(pDataX, size,
					m_diagns.meanX, m_diagns.deltaMeanX,
					m_diagns.valueDownX, m_diagns.valueUpX,
					m_diagns.ratioX))
				{// надо, чтобы функция всегда вызывалась
					maxRatio = std::max(maxRatio, m_diagns.ratioX);
					bFalseEvent |= m_diagns.ratioX > m_config.thresholdRatio;
				}
			}
			if (by)
			{
				if (CalcOneComponent(pDataY, size,
					m_diagns.meanY, m_diagns.deltaMeanY,
					m_diagns.valueDownY, m_diagns.valueUpY,
					m_diagns.ratioY))
				{
					maxRatio = std::max(maxRatio, m_diagns.ratioY);
					bFalseEvent |= m_diagns.ratioY > m_config.thresholdRatio;
				}
			}
			if (bz)
			{
				if (CalcOneComponent(pDataZ, size,
					m_diagns.meanZ, m_diagns.deltaMeanZ,
					m_diagns.valueDownZ, m_diagns.valueUpZ,
					m_diagns.ratioZ))
				{
					maxRatio = std::max(maxRatio, m_diagns.ratioZ);
					bFalseEvent |= m_diagns.ratioZ > m_config.thresholdRatio;
				}
			}
			// Получили величину maxRatio, которая всегда >= 1.
			// Эту величину надо преобразовать с помощью. некоторой формуты в сигнал алгоритма:
			// value = k * maxRatio + y,
			// k & y находятся из следующих условий:
			// value(r) = 1;
			// value(thresholdRatio) = 3
			// где r - значение maxRatio в отсутствии однополярного сигнала,
			// чаще - это величина около 1,2
			//float r = 1.2f;
			//float k = 2.f / (m_config.thresholdRatio - r);
			//float y = (m_config.thresholdRatio - 3 * r) / (m_config.thresholdRatio - r);
			//m_result.value = k * maxRatio + y;
			//m_result.value = m_result.valueThreshold *
			//	maxRatio / m_config.thresholdRatio;

			m_result.value = maxRatio;
			m_result.result = bFalseEvent ? 1 : 0;
		}
		else
			ret = ErrorCodes::Err_NULL_Pointer;
	}
	return ret;
}//------------------------------------------------------------------------------
// Ничего не проверяю, уже всё проверено
bool CUnipolarSignal::CalcOneComponent(
	const float *pData, const int size,
	float &mean, float &deltaMean,
	float &valueDoun, float &valueUp, float &ratio)
{
	std::vector<int> histogram;		// гистограмма
	float valItem0Histogram(0.f);	// величина середины 0-ого интервала гистограммы
	float widthItemHistogram(0.f);	// ширина интервалов гистограммы
	bool ret(true);
	// строю гистограмму ----------------------------------
	ret = Histogram(pData, size, histogram,
		valItem0Histogram, widthItemHistogram);
	if (ret)
	{
		int maxPos(0);
		int maxValue(0);
		int sizeHist = histogram.size();	// обязательно!!!!
		ret = (sizeHist > 1) &&
			(ippsMaxIndx_32s(histogram.data(), sizeHist, &maxValue, &maxPos) == ippStsNoErr);
		if (ret)
		{
			mean = valItem0Histogram + maxPos * widthItemHistogram;
			deltaMean = 0.5f * widthItemHistogram;
			valueDoun = maxPos * widthItemHistogram;
			valueUp = (sizeHist - 1 - maxPos) * widthItemHistogram;
			ratio = CalcRatio2(valueDoun, valueUp);
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
}//namespace seismo
}//namespace dsp
}//namespace algorithm
}//namespace zet
//------------------------------------------------------------------------------