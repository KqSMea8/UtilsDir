//------------------------------------------------------------------------------
#include "stdafx.h"
#include "AmplRatio.h"
//------------------------------------------------------------------------------
namespace zet {
namespace algorithm {
namespace dsp {
namespace seismo {
//------------------------------------------------------------------------------
CAmplRatio::CAmplRatio() : CCustomFalseEvent()
{	// задание значений указателей из базового класса
	m_bOriginalSignal = false;

	m_type_Event_Algr = tea_FalseEvent;
	m_pConfig = &m_config.zet_header;

	long ret = ztOpenStructure(m_pConfig,
		ZETLAB_SSM_CFG_ALGR_AMPLRATIO_TYPE, ZETLAB_SSM_CFG_ALGR_AMPLRATIO_SIZE);

	// структура результата см. базовый класс
}
//------------------------------------------------------------------------------
CAmplRatio::CAmplRatio(const CAmplRatio & val) : CCustomFalseEvent(val)
{
	m_saTmp = val.m_saTmp;

	m_cfgClass = val.m_cfgClass;
	m_config = val.m_config;
}
//------------------------------------------------------------------------------
void CAmplRatio::operator = (const CAmplRatio &val)
{
	*((CCustomFalseEvent*)this) = *((CCustomFalseEvent*)&val);
	m_saTmp = val.m_saTmp;

	m_cfgClass = val.m_cfgClass;
	m_config = val.m_config;
}
//------------------------------------------------------------------------------
long CAmplRatio::SetConfigFromProxy(const DATE timeSRV)
{// копирование cgf-параметров из Proxy в структуру класса
	m_config = m_cfgClass;
	m_result.valueThreshold = m_config.thresholdAmpl;

	return ztSetStructureTimeLocal(m_pConfig, timeSRV);
}
//------------------------------------------------------------------------------
long CAmplRatio::SetConfig(ZetHeader* pParam, const DATE timeSRV)
{	// проверка пришедшей структуры
	long ret = CCustomAlgorithm::SetConfig(pParam, timeSRV);
	if (ret == NoError)
	{	// копирование этой структуры в Proxy с целью проверки полей структуры
		m_cfgClass = *((Ssm_cfg_Algr_AmplRatio*)pParam);
		// копирование параметрров из Proxy в структуру класса
		ret = SetConfigFromProxy(timeSRV);
	}
	return ret;
}
//------------------------------------------------------------------------------
long CAmplRatio::FalseEvent(
	const float *pDataX,
	const float *pDataY,
	const float *pDataZ,
	const DWORD number)
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
		if (m_saTmp.ReSize(number))
		{
			float peakX(0.f), peakY(0.f), peakZ(0.f);

			// –асчеты по предыстории
			GetPeakValue_32f(pDataX, m_sizePreHistory, peakX);
			GetPeakValue_32f(pDataY, m_sizePreHistory, peakY);
			GetPeakValue_32f(pDataZ, m_sizePreHistory, peakZ);
			float ratioAmplPre = CalcMaxRatio(peakX, peakY, peakZ);

			// –асчеты по событию
			int num = size - m_sizePreHistory;
			GetPeakValue_32f(pDataX + m_sizePreHistory, num, peakX);
			GetPeakValue_32f(pDataY + m_sizePreHistory, num, peakY);
			GetPeakValue_32f(pDataZ + m_sizePreHistory, num, peakZ);
			float valueRatioAmpl = CalcMaxRatio(peakX, peakY, peakZ);
			
			// –езультаты
			m_result.result = (valueRatioAmpl > m_config.thresholdAmpl) ? 1 : 0;
			m_result.value = valueRatioAmpl;
			m_result.valueSNR = valueRatioAmpl / ratioAmplPre;			
		}
		else
			ret = ErrorCodes::Err_Create_Array;
	}
	return ret;
}
//------------------------------------------------------------------------------
float CAmplRatio::CalcMaxRatio(
	const float val1, const float val2, const float val3)
{
	return std::max(std::max(
		CalcRatio2(val1, val2),
		CalcRatio2(val1, val3)),
		CalcRatio2(val2, val3));
}
//------------------------------------------------------------------------------
}
}
}
}
//------------------------------------------------------------------------------