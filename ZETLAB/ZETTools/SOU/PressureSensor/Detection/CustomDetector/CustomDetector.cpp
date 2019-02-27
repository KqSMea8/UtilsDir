//----------------------------------------------------------------------------------------------------------------------
//										������� ����� ���������� ��� �� ��������
//----------------------------------------------------------------------------------------------------------------------
#include <SOU\PressureSensor\Detection\CustomDetector\CustomDetector.h>
//----------------------------------------------------------------------------------------------------------------------
CCustomDetectorPressure::CCustomDetectorPressure()
{
}
//----------------------------------------------------------------------------------------------------------------------
bool CCustomDetectorPressure::Create(Lds_cfg_Detector_Pressure_ns::Proxy& proxy)
{
	m_cfg = proxy;
	m_sizePreHistory = (int)floor(m_cfg.timePreHistory * m_cfg.freqADC);
	m_counterInitMax = 22000;	// ����� �������������� � �����������
	m_threshold = m_cfg.thresholdForSTA_LTA;

	ZeroData();
	
	return true;
}
//------------------------------------------------------------------------------
void CCustomDetectorPressure::ZeroData()
{
	m_bInit = false;
	m_bEvent = false;
	m_bInterval = false;
	m_counterForInit = 0;
}
//------------------------------------------------------------------------------
TypeEventYesNo CCustomDetectorPressure::SetTypeEvent(bool bEvent)
{// ����������� �������� ��������� �������
	if (m_bEvent && bEvent)
	{// ������� �����������
		m_typeEvent = ted_Continue;
	}
	else if (!m_bEvent && bEvent)
	{// ������� ��������
		m_typeEvent = ted_Start;
	}
	else if (m_bEvent && !bEvent)
	{// ������� �����������
		m_typeEvent = ted_End;
	}
	else//if (!m_bEvent && !bEvent)
	{// ��� �������
		m_typeEvent = ted_No;
	}
	m_bEvent = bEvent;
	return m_typeEvent;
}
//------------------------------------------------------------------------------
// ����������� ����������� �������� ������� ������, �������������� ��������� �
// ����� ������ �������� ������ � ��������� ��������� � ������. �����������
// ������������� ������� ��������� (��� ��������������).
bool CCustomDetectorPressure::Calculation(const float* pData, const int sizePage)
{
	bool ret = (pData != nullptr) && (0 < sizePage);
	if (ret)
	{
		if (!m_bInit)
		{
			m_counterForInit += sizePage;
			m_bInit = m_counterForInit >= m_counterInitMax;
		}
	}
	return ret;
}
//------------------------------------------------------------------------------