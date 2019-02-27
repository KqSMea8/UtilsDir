//------------------------------------------------------------------------------
#include "stdafx.h"
#include "ConstValXYZ.h"
//------------------------------------------------------------------------------
namespace zet {
namespace algorithm {
namespace dsp {
namespace seismo {
//------------------------------------------------------------------------------
CConstValXYZ::CConstValXYZ() : CCustomFalseEvent()
{	// задание значений указателей из базового класса
	m_bOriginalSignal = true;

	m_type_Event_Algr = tea_FalseEvent;
	m_pConfig = &m_config.zet_header;
	m_pDiagns = &m_diagns.zet_header;

	OpenStructure(m_pConfig,
		ZETLAB_SSM_CFG_ALGR_CONSTVALXYZ_TYPE, ZETLAB_SSM_CFG_ALGR_CONSTVALXYZ_SIZE);
	CloseStructure(m_pConfig, true);

	OpenStructure(m_pDiagns,
		ZETLAB_SSM_DIA_ALGR_CONSTVALXYZ_TYPE, ZETLAB_SSM_DIA_ALGR_CONSTVALXYZ_SIZE);

	// структура результата см. базовый класс
}
//------------------------------------------------------------------------------
CConstValXYZ::CConstValXYZ(const CConstValXYZ & val) : CCustomFalseEvent(val)
{
	m_cfgClass = val.m_cfgClass;
	m_config = val.m_config;
	m_diagns = val.m_diagns;
}
//------------------------------------------------------------------------------
void CConstValXYZ::operator = (const CConstValXYZ & val)
{
	*((CCustomFalseEvent*)this) = *((CCustomFalseEvent*)&val);

	m_cfgClass = val.m_cfgClass;
	m_config = val.m_config;
	m_diagns = val.m_diagns;
}
//------------------------------------------------------------------------------
ErrorCode CConstValXYZ::SetConfigFromProxy()
{// копирование cgf-параметров из Proxy в структуру класса
	ErrorCode ret(NoError);
	try
	{
		m_config = m_cfgClass;
		m_result.valueThreshold = m_config.maxValue;
		CloseStructure(m_pConfig, true);
	}
	catch (ZetException&)
	{
		ret = Err_Copy_Structure;
	}
	return ret;
}
//------------------------------------------------------------------------------
ErrorCode CConstValXYZ::SetConfig(ZetHeader* pParam)
{	// проверка пришедшей структуры
	ErrorCode ret = CCustomAlgorithm::SetConfig(pParam);
	if (ret == NoError)
	{	// копирование этой структуры в Proxy с целью проверки полей структуры
		try
		{
			m_cfgClass = *((Ssm_cfg_Algr_ConstValXYZ*)pParam);
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
//------------------------------------------------------------------------------
bool CConstValXYZ::CalcSignalAlgoritm(
	const float *pDataX, const float *pDataY, const float *pDataZ,
	const int size, float & signal)
{
	bool ret =
		((ippsMean_32f(pDataX, size, &m_diagns.meanX, ippAlgHintFast)) == ippStsNoErr) &&
		((ippsMean_32f(pDataY, size, &m_diagns.meanY, ippAlgHintFast)) == ippStsNoErr) &&
		((ippsMean_32f(pDataZ, size, &m_diagns.meanZ, ippAlgHintFast)) == ippStsNoErr);
	if (ret)
	{
		signal = std::max(std::max(
			fabsf(m_diagns.meanX), fabsf(m_diagns.meanY)),
			fabsf(m_diagns.meanZ));
	}
	return ret;
}
//------------------------------------------------------------------------------
ErrorCode CConstValXYZ::FalseEvent(const float *pDataX, const float *pDataY,
		const float *pDataZ, const DWORD number)
{
	m_diagns.meanX = m_diagns.meanY = m_diagns.meanZ = 0.f;
	m_result.value = 0.f;
	m_result.result = 0;
	m_result.valueSNR = 0;

	int size = (int)number;
	ErrorCode ret = ((size > 0) &&
		(pDataX != nullptr) && (pDataY != nullptr) && (pDataZ != nullptr)) ?
		ErrorCodes::NoError : ErrorCodes::Err_NULL_Pointer;
	if (ret == ErrorCodes::NoError)
	{
		float sigPre(0.f), sigEvn(0.f);
		ret = CalcSignalAlgoritm(pDataX, pDataY, pDataZ, m_sizePreHistory, sigPre) &&
			CalcSignalAlgoritm(
			pDataX + m_sizePreHistory,
			pDataY + m_sizePreHistory,
			pDataZ + m_sizePreHistory,
			size - m_sizePreHistory, sigEvn);
		if (ret)
		{
			m_result.value = sigEvn;
			m_result.result = (sigEvn > m_config.maxValue) ? 1 : 0;			 
			m_result.valueSNR = (sigPre > 0.f) ? (sigEvn / sigPre) : 0.f;
		}
		else
			ret = ErrorCodes::Err_Calc;
	}
	return ret;
}
//------------------------------------------------------------------------------
}
}
}
}
//------------------------------------------------------------------------------