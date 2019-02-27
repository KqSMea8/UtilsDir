//------------------------------------------------------------------------------
#include "stdafx.h"
#include <Seismo/DetectorSSEC/Classification/Correlation/FalseEventCorrelation.h>

namespace zet {
namespace algorithm {
namespace dsp {
namespace seismo {
//------------------------------------------------------------------------------
CFalseEventCorrelation::CFalseEventCorrelation() : CCustomFalseEvent()
{
	m_bOriginalSignal = false;

	m_type_Event_Algr = tea_ActualEvent;
	m_pConfig = &m_config.zet_header;

	long r = ztOpenStructure(m_pConfig,
		ZETLAB_SSM_CFG_ALGR_CORRELATION_TYPE, ZETLAB_SSM_CFG_ALGR_CORRELATION_SIZE);

	// структура результата см. базовый класс
}
//------------------------------------------------------------------------------
CFalseEventCorrelation::CFalseEventCorrelation(
	const CFalseEventCorrelation &val) : CCustomFalseEvent(val)
{
	m_cfgClass = val.m_cfgClass;
	m_config = val.m_config;
}
//------------------------------------------------------------------------------
CFalseEventCorrelation::~CFalseEventCorrelation()
{}
//------------------------------------------------------------------------------
void CFalseEventCorrelation::operator = (const CFalseEventCorrelation &val)
{
	*((CCustomFalseEvent*)this) = *((CCustomFalseEvent*)&val);
	m_cfgClass = val.m_cfgClass;
	m_config = val.m_config;
}
//------------------------------------------------------------------------------
bool CFalseEventCorrelation::Correlation(
	const float* data1, const float* data2, float &corr, const int size)
{	
	float mean1(0.f), mean2(0.f);
	float sum12(0.f), sum11(0.f), sum22(0.f);

		// центрирование исходных массивов
	bool ret =
		(ippsMean_32f(data1, size, &mean1, ippAlgHintNone) == ippStsNoErr) &&
		(ippsSubC_32f(data1, mean1, m_sa1.pData, size) == ippStsNoErr) &&
		(ippsMean_32f(data2, size, &mean2, ippAlgHintNone) == ippStsNoErr) &&
		(ippsSubC_32f(data2, mean2, m_sa2.pData, size) == ippStsNoErr) &&

		// Расчёты
		(ippsMul_32f(m_sa1.pData, m_sa2.pData, m_sa3.pData, size) == ippStsNoErr) &&
		(ippsSum_32f(m_sa3.pData, size, &sum12, ippAlgHintNone) == ippStsNoErr) &&

		(ippsSqr_32f_I(m_sa1.pData, size) == ippStsNoErr) &&
		(ippsSum_32f(m_sa1.pData, size, &sum11, ippAlgHintNone) == ippStsNoErr) &&
		(ippsSqr_32f_I(m_sa2.pData, size) == ippStsNoErr) &&
		(ippsSum_32f(m_sa2.pData, size, &sum22, ippAlgHintNone) == ippStsNoErr);

	if (ret)
	{
		ret = (sum11 != 0.f) && (sum22 != 0.f);
		if (ret)
		{
			corr = fabsf(sum12 / sqrt(sum11 * sum22));
			corr = (corr > 0.f) ? ((1.f - corr) / corr) : 0.f;
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
long CFalseEventCorrelation::SetConfigFromProxy(const DATE timeSRV)
{// копирование cfg-параметров из Proxy в структуру класса
	m_config = m_cfgClass;
	m_result.valueThreshold = m_config.threshold;

	return ztSetStructureTimeLocal(m_pConfig, timeSRV);
}
//------------------------------------------------------------------------------
long CFalseEventCorrelation::SetConfig(ZetHeader* pParam, const DATE timeSRV)
{	// проверка пришедшей структуры
	long ret = CCustomAlgorithm::SetConfig(pParam, timeSRV);
	if (ret == NoError)
	{	// копирование этой структуры в Proxy с целью проверки полей структуры
		m_cfgClass = *((Ssm_cfg_Algr_Correlation*)pParam);
		// копирование параметрров из Proxy в структуру класса
		ret = SetConfigFromProxy(timeSRV);
	}
	return ret;
}
//------------------------------------------------------------------------------
bool CFalseEventCorrelation::Calculation(const float *pDataX, const float *pDataY,
	const float *pDataZ, const int size, float & corr)
{
	corr = 0.f;
	float c01(0.f), c12(0.f), c20(0.f);
	bool ret =
		Correlation(pDataX, pDataY, c01, size) &&
		Correlation(pDataY, pDataZ, c12, size) &&
		Correlation(pDataZ, pDataX, c20, size);
	corr = min(c01, min(c12, c20));
//	corr = std::max(c01, std::max(c12, c20));
	return ret;
}
//------------------------------------------------------------------------------
long CFalseEventCorrelation::FalseEvent(
	const float *pDataX, const float *pDataY, const float *pDataZ,
	const DWORD number)
{
	long ret = ErrorCodes::NoError;
	m_result.result = 0;
	m_result.value = 0.f;
	m_result.valueSNR = 0;
	int size = (int)number;
	bool flag = (size > 0) && (pDataX != nullptr) && (pDataY != nullptr) && (pDataZ != nullptr)
		&& m_sa1.ReSize(number) && m_sa2.ReSize(number) && m_sa3.ReSize(number);
	if (flag)
	{
		float corrPre(0.f), corrEvn(0.f);
		flag = Calculation(pDataX, pDataY, pDataZ, m_sizePreHistory, corrPre) &&
				Calculation(
						pDataX + m_sizePreHistory,
						pDataY + m_sizePreHistory,
						pDataZ + m_sizePreHistory,
						(int)number - m_sizePreHistory, corrEvn);
		if (flag)
		{
			m_result.value = corrEvn;
			m_result.result = (corrEvn > m_config.threshold) ? 1 : 0;
			m_result.valueSNR = corrEvn / corrPre;
		}
	}
	else
		ret = ErrorCodes::Err_NULL_Pointer;
	return ret;
}
//------------------------------------------------------------------------------
}//namespace seismo
}//namespace dsp
}//namespace algorithm
}//namespace zet
//------------------------------------------------------------------------------