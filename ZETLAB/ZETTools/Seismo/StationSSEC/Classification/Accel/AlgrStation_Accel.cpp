//------------------------------------------------------------------------------
#include "stdafx.h"
#include "AlgrStation_Accel.h"
//------------------------------------------------------------------------------
namespace zet {
namespace algorithm {
namespace dsp {
namespace seismo {
//------------------------------------------------------------------------------
CFalseEventStationAccel::CFalseEventStationAccel() : CCustomFalseEventStation()
{	// задание значений указателей из базового класса
	m_type_Event_Algr = tea_FalseEvent;
	m_pConfig = &m_config.zet_header;

	long ret = ztOpenStructure(m_pConfig,
		ZETLAB_SSM_CFG_ALGRSTATION_ACCEL_TYPE, ZETLAB_SSM_CFG_ALGRSTATION_ACCEL_SIZE);

	// структура результата см. базовый класс
}
//------------------------------------------------------------------------------
CFalseEventStationAccel::CFalseEventStationAccel(const CFalseEventStationAccel & val)
	: CCustomFalseEventStation(val)
{
	m_config = val.m_config;
	m_cfgClass = val.m_cfgClass;
	m_pConfig = &m_config.zet_header;
}
//------------------------------------------------------------------------------
void CFalseEventStationAccel::operator = (const CFalseEventStationAccel &val)
{
	*((CCustomFalseEventStation*)this) = *((CCustomFalseEventStation*)&val);
	m_config = val.m_config;
	m_cfgClass = val.m_cfgClass;
	m_pConfig = &m_config.zet_header;
}
//------------------------------------------------------------------------------
long CFalseEventStationAccel::SetConfigFromProxy(const DATE timeSRV)
{// копирование cfg-параметров из Proxy в структуру класса
	m_config = m_cfgClass;
	m_result.valueThreshold = m_config.thresholdRatio;
	return ztSetStructureTimeLocal(m_pConfig, timeSRV);
}
//------------------------------------------------------------------------------
long CFalseEventStationAccel::SetConfig(ZetHeader* pParam, const DATE timeSRV)
{	// проверка пришедшей структуры
	long ret = CCustomAlgorithm::SetConfig(pParam);
	if (ret == NoError)
	{	// копирование этой структуры в Proxy с целью проверки полей структуры
		m_cfgClass = *((Ssm_cfg_AlgrStation_Accel*)pParam);
		// копирование параметрров из Proxy в структуру класса
		ret = SetConfigFromProxy(timeSRV);
	}
	return ret;
}
//------------------------------------------------------------------------------
long CFalseEventStationAccel::Calculation(
	std::list<Ssm_str_EventParam> & listEvent)
{
	m_result.result = 0;
	//m_result.value = 0.f;	см. CalcRatios(m_result.value)

	m_vectData.clear();
	for (auto & it : listEvent)
		m_vectData.push_back(it.peakAccelA);

	long ret = CalcRatios();
	m_result.result = (m_result.value >= m_config.thresholdRatio) ? 1 : 0;

	return ret;
}
//------------------------------------------------------------------------------
}
}
}
}
//------------------------------------------------------------------------------