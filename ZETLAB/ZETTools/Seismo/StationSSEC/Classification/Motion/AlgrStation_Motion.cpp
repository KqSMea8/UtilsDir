//------------------------------------------------------------------------------
#include "stdafx.h"
#include "AlgrStation_Motion.h"
//------------------------------------------------------------------------------
namespace zet {
namespace algorithm {
namespace dsp {
namespace seismo {
//------------------------------------------------------------------------------
CFalseEventStationMotion::CFalseEventStationMotion() : CCustomFalseEventStation()
{	// ������� �������� ���������� �� �������� ������
	m_type_Event_Algr = tea_FalseEvent;
	m_pConfig = &m_config.zet_header;

	long ret = ztOpenStructure(m_pConfig,
		ZETLAB_SSM_CFG_ALGRSTATION_MOTION_TYPE, ZETLAB_SSM_CFG_ALGRSTATION_MOTION_SIZE);

	// ��������� ���������� ��. ������� �����
}
//------------------------------------------------------------------------------
CFalseEventStationMotion::CFalseEventStationMotion(const CFalseEventStationMotion & val)
	: CCustomFalseEventStation(val)
{
	m_config = val.m_config;
	m_cfgClass = val.m_cfgClass;
	m_pConfig = &m_config.zet_header;
}
//------------------------------------------------------------------------------
void CFalseEventStationMotion::operator = (const CFalseEventStationMotion &val)
{
	*((CCustomFalseEventStation*)this) = *((CCustomFalseEventStation*)&val);
	m_config = val.m_config;
	m_cfgClass = val.m_cfgClass;
	m_pConfig = &m_config.zet_header;
}
//------------------------------------------------------------------------------
long CFalseEventStationMotion::SetConfigFromProxy(const DATE timeSRV)
{// ����������� cgf-���������� �� Proxy � ��������� ������
	m_config = m_cfgClass;
	m_result.valueThreshold = m_config.thresholdRatio;
	return ztSetStructureTimeLocal(m_pConfig, timeSRV);
}
//------------------------------------------------------------------------------
long CFalseEventStationMotion::SetConfig(ZetHeader* pParam, const DATE timeSRV)
{	// �������� ��������� ���������
	long ret = CCustomAlgorithm::SetConfig(pParam);
	if (ret == NoError)
	{	// ����������� ���� ��������� � Proxy � ����� �������� ����� ���������
		m_cfgClass = *((Ssm_cfg_AlgrStation_Motion*)pParam);
		// ����������� ����������� �� Proxy � ��������� ������
		ret = SetConfigFromProxy(timeSRV);
	}
	return ret;
}
//------------------------------------------------------------------------------
long CFalseEventStationMotion::Calculation(
	std::list<Ssm_str_EventParam> & listEvent)
{
	m_result.result = 0;
	//m_result.value = 0.f;	��. CalcRatios(m_result.value)
	m_result.valueSNR = 0.f;

	m_vectData.clear();
	for (auto & it : listEvent)
		m_vectData.push_back(it.peakMotionA);

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