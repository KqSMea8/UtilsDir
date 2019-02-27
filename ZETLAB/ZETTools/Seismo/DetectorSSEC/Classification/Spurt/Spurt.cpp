#include "stdafx.h"
#include "Spurt.h"
//------------------------------------------------------------------------------------------
namespace zet {
namespace algorithm {
namespace dsp {
namespace seismo {
//------------------------------------------------------------------------------------------
CSpurt::CSpurt() : CCustomFalseEvent()
{
	m_pConfig = &m_config.zet_header;
	m_pDiagns = &m_diagns.zet_header;

	// задание конфигурационных параметров значениями по умолчанию
	OpenStructure(m_pConfig, ZETLAB_SSM_CFG_ALGR_SPURT_TYPE, ZETLAB_SSM_CFG_ALGR_SPURT_SIZE);
	CloseStructure(m_pConfig, true);

	// структура диагностических данных
	OpenStructure(m_pDiagns, ZETLAB_SSM_DIA_ALGR_SPURT_TYPE, ZETLAB_SSM_DIA_ALGR_SPURT_SIZE);

	// структура результата см. базовый класс
}
//--------------------------------------------------------------
CSpurt::CSpurt(const CSpurt & val) : CCustomFalseEvent(val)
{
	m_cfgClass = val.m_cfgClass;
	m_config = val.m_config;
	m_diagns = val.m_diagns;
}
//--------------------------------------------------------------
CSpurt::~CSpurt()
{}
//--------------------------------------------------------------
void CSpurt::operator = (const CSpurt & val)
{
	*((CCustomFalseEvent*)this) = *((CCustomFalseEvent*)&val);

	m_cfgClass = val.m_cfgClass;
	m_config = val.m_config;
	m_diagns = val.m_diagns;
}
//--------------------------------------------------------------
ErrorCode CSpurt::SetConfigFromProxy()
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
ErrorCode CSpurt::SetConfig(ZetHeader* pParam)
{	// проверка пришедшей структуры
	ErrorCode ret = CCustomAlgorithm::SetConfig(pParam);
	if (ret == NoError)
	{	// копирование этой структуры в Proxy с целью проверки полей структуры
		try
		{
			m_cfgClass = *((Ssm_cfg_Algr_Spurt*)pParam);
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
ErrorCode CSpurt::FalseEvent(const float *pDataX,
	const float *pDataY, const float *pDataZ, const DWORD number)
{// ВОЗМОЖНО ИСПОЛЬЗОВАНИЕ АЛГОРИТМА В ОДНОКАНАЛЬНОМ РЕЖИМЕ
	m_diagns.peakX = m_diagns.peakY = m_diagns.peakZ = 0.f;
	m_diagns.spurtX = m_diagns.spurtY = m_diagns.spurtZ = 0.f;

	m_result.result = 0;
	m_result.value = 0.f;

	float ratioX(0.f), ratioY(0.f), ratioZ(0.f);
	int size = (int)number;
	ErrorCode ret = ((size > 0) && (size > m_sizePreHistory) && (m_sizePreHistory > 0)) ?
		ErrorCodes::NoError : ErrorCodes::FatalError;
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
			{	// надо, чтобы функция всегда вызывалась
				CalcOneComponent(pDataX, size,
					m_diagns.peakX, m_diagns.spurtX, ratioX);
				maxRatio = std::max(maxRatio, ratioX);
				bFalseEvent |= ratioX > m_config.thresholdRatio;
			}
			if (by)
			{
				CalcOneComponent(pDataY, size,
					m_diagns.peakY, m_diagns.spurtY, ratioY);
				maxRatio = std::max(maxRatio, ratioY);
				bFalseEvent |= ratioY > m_config.thresholdRatio;
			}
			if (bz)
			{
				CalcOneComponent(pDataZ, size,
					m_diagns.peakZ, m_diagns.spurtZ, ratioZ);
				maxRatio = std::max(maxRatio, ratioZ);
				bFalseEvent |= ratioZ > m_config.thresholdRatio;
			}
			m_result.value = maxRatio;
			//m_result.value = m_result.valueThreshold *
			//	maxRatio / m_config.thresholdRatio;
			m_result.result = bFalseEvent ? 1 : 0;
		}
		else
			ret = ErrorCodes::Err_NULL_Pointer;
	}
	return ret;
}//------------------------------------------------------------------------------
// Ничего не проверяю, уже всё проверено
void CSpurt::CalcOneComponent(
	const float *pData, const int size,
	float &peakPreHistory, float &maxAll, float &ratio)
{
	float mi(0.f), ma(0.f);
	if (ippsMinMax_32f(pData, m_sizePreHistory, &mi, &ma) == ippStsNoErr)
	{
		if (ma > mi)
		{
			peakPreHistory = ma - mi;
			maxAll = 0.f;
			float val1 = pData[m_sizePreHistory - 1];
			for (int i = m_sizePreHistory; i < size; ++i)
			{
				float val2 = pData[i];
				float val = fabsf(val2 - val1);
				if (maxAll < val)
					maxAll = val;
				val1 = val2;
			}
			ratio = maxAll / peakPreHistory;
		}
		else
			peakPreHistory = maxAll = ratio = 0.f;
	}
	else
		ratio = 0.f;
}
//------------------------------------------------------------------------------
}//namespace seismo
}//namespace dsp
}//namespace algorithm
}//namespace zet
//------------------------------------------------------------------------------