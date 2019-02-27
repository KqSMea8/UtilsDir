//------------------------------------------------------------------------------
#include "stdafx.h"
#include "Dialog_ZET\DataTime.h"
//------------------------------------------------------------------------------
// �� ��������� ������ � ������ � ��� = 0, �.�. ����� ���������� = 0,1 ���
CDataTime::CDataTime()
	: m_bCodeMode(true)
	, m_bCalcNumberMode(false)
	, m_bZetMode(false)
	, m_bInDependentMode(false)
	, m_codeFast(0)
	, m_mode10sec(m10s_every_1sec)
	, m_timeAverage(1.)
	, m_zdt(0.1)
	, m_zTimeRead(0.)
	, reserved3(0)
{}
//------------------------------------------------------------------------------
CDataTime::CDataTime(const CDataTime & val)
{
	*this = val;
}
//------------------------------------------------------------------------------
CDataTime & CDataTime::operator = (const CDataTime & val)
{
	m_bCodeMode = val.m_bCodeMode;
	m_bCalcNumberMode = val.m_bCalcNumberMode;
	m_bZetMode = val.m_bZetMode;
	m_bInDependentMode = val.m_bInDependentMode;
	m_codeFast = val.m_codeFast;
	m_mode10sec = val.m_mode10sec;
	m_timeAverage = val.m_timeAverage;
	m_zdt = val.m_zdt;
	m_zTimeRead = val.m_zTimeRead;

	return *this;
}
//------------------------------------------------------------------------------
// C����� ������� ���-�� ����������� ��������:
// true - �� m_timeAverage, false - �� m_zdt
bool CDataTime::GetCalcNumberMode()
{
	return m_bCalcNumberMode;
}
//------------------------------------------------------------------------------
void CDataTime::SetCalcNumberMode(bool bCalcByAver)
{
	m_bCalcNumberMode = bCalcByAver;
}
//------------------------------------------------------------------------------
long CDataTime::TestFast(const long val)
{	// ���������� ����������� 0 <= val <= 2
	long ret;
	if (val < 0)
		ret = 0;
	else if (val > 2)
		ret = 2;
	else
		ret = val;
	return ret;
}
//------------------------------------------------------------------------------
/* ������� ���������� ������� � ������ User.
//------------------------------------------------------------------------------
	��� ��������:
	1. ����� ������ timeInterval (indexMax = 0).
� ���� ������ timeInterval ������ ���� ������ 1 ���. �.�. ���� ����� �����
����������, ������� ��� ��������� �������� � 1 ���., �.�.
		timeInterval * indexMax = 1.
���� ����� ��������� ��� ������������� � ��� ������������ �����.

	2. ����� ��� ���������. ��� ��� ������, ����� timeInterval �� ������ 1
���. � ���� ������ ����������, ����� �� ��������� ����� ���������� ���������
��������, ������ ������ ����� ���., �.�. ������ ����������� ���������:
		timeInterval * indexMax = m_time0_delta.
��������: ���������� ������� 0,128 ���, indexMax = 125, ����� m_time0_delta = 16.
*/
bool CDataTime::SetUserParamMode(
	double timeInterval,
	long indexMax,
	double timeAverage)
{
	bool ret = timeInterval > 0.;
	if (ret)
	{
		m_bInDependentMode = false;
		m_bCodeMode = false;
		m_bZetMode = false;
		m_zdt = timeInterval;
		m_timeAverage = timeAverage;
		if (indexMax == 0)
		{
			uint32_t nanosec = m_zdt.GetNanoseconds();
			if (nanosec > 0)
			{
				div_t dvt = div(1000000000, nanosec);
				ret = dvt.rem == 0;
			}
		}
		else
		{
			CZetTimeSpan zts(m_zdt);
			zts *= indexMax;
			ret = 0 ==  m_zdt.GetNanoseconds();
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
// ������� ���� ������� ����������.
// ��� ������� ����� 7000 ������� �������� ������� ������������� � ��
void CDataTime::SetAverageCodeMode(
	const long val,			// ��������� ��� ����������
	const double freqADC)	// ������� ������������� ������ (��������� ��� 7000)
{
	m_bCodeMode = true;
	m_bZetMode = false;
	m_bInDependentMode = false;
	// ���������� ����������� 0 <= val <= 2
	m_codeFast = TestFast(val);
	if ((freqADC == 0.) || (freqADC >= 50.))
	{// ����������� ����������
		switch (m_codeFast)
		{
		case 0:
			m_timeAverage = 0.1;
			m_zdt = CZetTimeSpan((int64_t)0, (uint32_t)100000000);
			break;
		case 1:
			m_timeAverage = 1.;
			m_zdt = 1;
			break;
		case 2:
			m_timeAverage = 10.;
			m_zdt = (int)m_mode10sec;
			//	break;
		}
	}
	else   // �������� ��� 7000
	{
		switch (m_codeFast)
		{
		case 0:
			m_timeAverage = 0.1;
			m_zdt = CZetTimeSpan((int64_t)0, (uint32_t)100000000);
			if ((double)(m_zdt * freqADC) >= 1.)
				break;
		case 1:
			m_timeAverage = 1.;
			m_codeFast = 1;
			// �� ������, ����� �� ���� break ������
			m_zdt = 1;
			if (freqADC >= 1.)
				break;
		case 2:
			m_timeAverage = 10.;
			m_codeFast = 2;
			// �� ������, ����� �� ���� break ������
			if (m_mode10sec == m10s_every_1sec)
			{
				m_zdt = 1;
				if (freqADC < 1.)
				{
					m_mode10sec = m10s_every_10sec;
					m_zdt = 10;
				}
			}
			else
			{//m_mode10sec == m10s_every_10sec
				m_zdt = 10;
			}
			//	break;
		}
	}
}
//------------------------------------------------------------------------------
// ������� ������� ������� ���������� ������.
// ������ ������ ������ ����� ������.
void CDataTime::SetStartTime(const double timeCur)		// ������� �����
{// ����� c������ ����� ���� �������������
	if (m_codeFast == 0)
	{
		m_zTimeRead = timeCur;
		m_zTimeRead.RoundSecondsDown();
	}
	else if (m_codeFast == 1)
	{
		m_zTimeRead = timeCur;
		m_zTimeRead += CZetTimeSpan(0, 100000000);	// + 0.1 ���
	}
	else if (m_codeFast == 2)
	{
		double time = floor(timeCur);
		time /= 10;
		time = floor(time);
		time *= 10;
		m_zTimeRead = timeCur;
	}

// 	double time = floor(timeCur);
// 	if (m_bCodeMode)
// 	{
// 		if (m_codeFast == 2)
// 		{
// 			time /= 10;
// 			time = floor(time);
// 			time *= 10;
// 		}
// 	}
// 	m_zTimeRead = time;
}
//------------------------------------------------------------------------------
// ���������� ������� ���������� ������.
// ��� �� ����� m_dt (��������: 0,1 ���) ������ �� ������ ����������
void CDataTime::IncrementTime()
{
	m_zTimeRead += m_zdt;
}
//------------------------------------------------------------------------------
// ������� ��������� � ������ Zet
void CDataTime::SetZetMode(CZetTimeSpan & zts)
{
	m_bInDependentMode = false;
	m_bCodeMode = false;
	m_bZetMode = true;
	
	m_zdt = zts;
	m_timeAverage = zts;
}
//------------------------------------------------------------------------------
// ������� ���������� � ������, ����� �������� � ����� ���������� �� �������
void CDataTime::SetInDependentMode(const CZetTimeSpan & zTimeInterval, const double timeAver)
{
	m_bInDependentMode = true;
	m_bCodeMode = false;
	m_bZetMode = false;

	m_zdt = zTimeInterval;
	m_timeAverage = timeAver;
}
//------------------------------------------------------------------------------
// ������� �������� ������ ������ ��������
void CDataTime::SetTimeRead(const CZetTimeSpan & zTime)
{
	m_zTimeRead = zTime;
}
//------------------------------------------------------------------------------