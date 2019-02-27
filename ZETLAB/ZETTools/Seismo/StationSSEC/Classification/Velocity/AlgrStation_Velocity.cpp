//------------------------------------------------------------------------------
#include "stdafx.h"
#include "AlgrStation_Velocity.h"
//------------------------------------------------------------------------------
namespace zet {
namespace algorithm {
namespace dsp {
namespace seismo {
//------------------------------------------------------------------------------
CFalseEventStationVelocity::CFalseEventStationVelocity() : CCustomFalseEventStation()
{	// ������� �������� ���������� �� �������� ������
	m_type_Event_Algr = tea_FalseEvent;
	m_pConfig = &m_config.zet_header;

	long ret = ztOpenStructure(m_pConfig,
		ZETLAB_SSM_CFG_ALGRSTATION_VELOCITY_TYPE, ZETLAB_SSM_CFG_ALGRSTATION_VELOCITY_SIZE);

	// ��������� ���������� ��. ������� �����
}
//------------------------------------------------------------------------------
CFalseEventStationVelocity::CFalseEventStationVelocity(const CFalseEventStationVelocity & val)
	: CCustomFalseEventStation(val)
{
	m_config = val.m_config;
	m_cfgClass = val.m_cfgClass;
	m_pConfig = &m_config.zet_header;
}
//------------------------------------------------------------------------------
void CFalseEventStationVelocity::operator = (const CFalseEventStationVelocity &val)
{
	*((CCustomFalseEventStation*)this) = *((CCustomFalseEventStation*)&val);
	m_config = val.m_config;
	m_cfgClass = val.m_cfgClass;
	m_pConfig = &m_config.zet_header;
}
//------------------------------------------------------------------------------
long CFalseEventStationVelocity::SetConfigFromProxy(const DATE timeSRV)
{// ����������� cfg-���������� �� Proxy � ��������� ������
	m_config = m_cfgClass;
	m_result.valueThreshold = m_config.thresholdRatio;
	return ztSetStructureTimeLocal(m_pConfig, timeSRV);
}
//------------------------------------------------------------------------------
long CFalseEventStationVelocity::SetConfig(ZetHeader* pParam, const DATE timeSRV)
{	// �������� ��������� ���������
	long ret = CCustomAlgorithm::SetConfig(pParam);
	if (ret == NoError)
	{	// ����������� ���� ��������� � Proxy � ����� �������� ����� ���������
		m_cfgClass = *((Ssm_cfg_AlgrStation_Velocity*)pParam);
		// ����������� ����������� �� Proxy � ��������� ������
		ret = SetConfigFromProxy(timeSRV);
	}
	return ret;
}
//------------------------------------------------------------------------------
long CFalseEventStationVelocity::Calculation(
	std::list<Ssm_str_EventParam> & listEvent)
{
	m_result.result = 0;
	//m_result.value = 0.f;	��. CalcRatios(m_result.value)
	m_result.valueSNR = 0.f;

	m_vectData.clear();
	for (auto & it : listEvent)
		m_vectData.push_back(it.peakVelocityA);

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