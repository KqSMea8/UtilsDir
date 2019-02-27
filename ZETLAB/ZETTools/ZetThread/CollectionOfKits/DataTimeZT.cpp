//------------------------------------------------------------------------------
#include "stdafx.h"
#include <mathZet.h>
#include "DataTimeZT.h"
//------------------------------------------------------------------------------
CDataTimeZT::CDataTimeZT()
	: m_zts_dtBuf (1i64, 0U)	// 1 ���

	, m_zts_dtRead (1i64, 0U)	// 1 ���
	, m_zts_dt�alc (1i64, 0U)	// 1 ���
	, m_zts_dtAver (1i64, 0U)	// 1 ���

	, m_zts_read (0i64, 0U)		// 0 ���
	, m_zts_calc (1i64, 0U)		// 1 ���
	, m_zts_aver (1i64, 0U)		// 1 ���
{
}
//------------------------------------------------------------------------------
CDataTimeZT::~CDataTimeZT()
{
}
//------------------------------------------------------------------------------
CDataTimeZT::CDataTimeZT(const CDataTimeZT & val)
{
	*this = val;
}
//------------------------------------------------------------------------------
CDataTimeZT & CDataTimeZT::operator = (const CDataTimeZT & val)
{
	m_zts_dtBuf = val.m_zts_dtBuf;

	m_zts_dtRead = val.m_zts_dtRead;
	m_zts_dt�alc = val.m_zts_dt�alc;
	m_zts_dtAver = val.m_zts_dtAver;

	m_zts_read = val.m_zts_read;
	m_zts_calc = val.m_zts_calc;
	m_zts_aver = val.m_zts_aver;

	return *this;
}
//------------------------------------------------------------------------------
// �������� ������� ��� ������� ������� ������
CZetTimeSpan CDataTimeZT::GetIntervalBuffer()
{
	return m_zts_dtBuf;
}

void CDataTimeZT::SetIntervalBuffer(const CZetTimeSpan & ztInterval)
{
	if ((CZetTimeSpan)ztInterval > CZetTimeSpan(0i64, 0U))
		m_zts_dtBuf = ztInterval;
}
//------------------------------------------------------------------------------
// ������ ������ ������
CZetTimeSpan & CDataTimeZT::GetIntervalRead()
{
	return m_zts_dtRead;
}

void CDataTimeZT::SetIntervalRead(const CZetTimeSpan & ztInterval)
{
	if ((CZetTimeSpan)ztInterval > CZetTimeSpan(0i64, 0U))
		m_zts_dtRead = ztInterval;
}

void CDataTimeZT::SetIntervalRead(const double dInterval)
{
	if (dInterval > 0.)
		m_zts_dtRead = dInterval;
}
//------------------------------------------------------------------------------
// ������ ������� ������������� �����������
CZetTimeSpan & CDataTimeZT::GetIntervalCalc()
{
	return m_zts_dt�alc;
}

void CDataTimeZT::SetIntervalCalc(const CZetTimeSpan & ztInterval, const bool bRound)
{
	if ((CZetTimeSpan)ztInterval > CZetTimeSpan(0i64, 0U))
		m_zts_dt�alc = ztInterval;
	
	if (bRound)
	{
		m_zts_dt�alc.RoundSeconds();
		if (m_zts_dt�alc < m_zts_dtRead)
			m_zts_dt�alc = m_zts_dtRead;
	}
}

void CDataTimeZT::SetIntervalCalc(const double dInterval, const bool bRound)
{
	if (dInterval > 0.)
		m_zts_dt�alc = dInterval;

	if (bRound)
	{
		m_zts_dt�alc.RoundSeconds();
		if (m_zts_dt�alc < m_zts_dtRead)
			m_zts_dt�alc = m_zts_dtRead;
	}
}
//------------------------------------------------------------------------------
// ������ ���������� ������������� �����������
CZetTimeSpan & CDataTimeZT::GetIntervalAver()
{
	return m_zts_dtAver;
}

void CDataTimeZT::SetIntervalAver(const CZetTimeSpan & ztInterval)
{
	if ((CZetTimeSpan)ztInterval > CZetTimeSpan(0i64, 0U))
		m_zts_dtAver = ztInterval;
}

void CDataTimeZT::SetIntervalAver(const double dInterval)
{
	if (dInterval > 0.)
		m_zts_dtAver = dInterval;
}
//------------------------------------------------------------------------------
// ����� ������ ������
CZetTimeSpan & CDataTimeZT::GetTimeRead()
{
	return m_zts_read;
}

void CDataTimeZT::SetTimeRead(const CZetTimeSpan & zTime)
{
	m_zts_read = zTime;
}
//------------------------------------------------------------------------------
// ����� ������� ������������� �����������
CZetTimeSpan & CDataTimeZT::GetTimeCalc()
{
	return m_zts_calc;
}

void CDataTimeZT::SetTimeCalc(const CZetTimeSpan & zTime)
{
	m_zts_calc = zTime;
}
//------------------------------------------------------------------------------
// ����� ���������� ������������� �����������
CZetTimeSpan & CDataTimeZT::GetTimeAver()
{
	return m_zts_aver;
}

void CDataTimeZT::SetTimeAver(const CZetTimeSpan & zTime)
{
	m_zts_aver = zTime;
}
//------------------------------------------------------------------------------





//------------------------------------------------------------------------------
void CDataTimeZT::SetTimeReadStart(const double timeCur)
{
	m_zts_read = timeCur;
	m_zts_read.RoundSecondsUp();

	if ((0i64 == m_zts_dtRead.GetSeconds()) && (0 < m_zts_dtRead.GetNanoseconds()))
		m_zts_read += m_zts_dtRead;
}
//------------------------------------------------------------------------------
void CDataTimeZT::IncrementTime()
{
	m_zts_read += m_zts_dtRead;
}
//------------------------------------------------------------------------------
// ��������� ������� ���������� ������
void CDataTimeZT::IncrementTimeAver()
{
	m_zts_aver += m_zts_dtAver;
}
//------------------------------------------------------------------------------
double CDataTimeZT::GetTime()
{
	return (double)m_zts_read;
}
//------------------------------------------------------------------------------
// ��������� ������� ������� ������������� �����������
void CDataTimeZT::IncrementTimeCalc()
{
	m_zts_calc += m_zts_dt�alc;
}
//------------------------------------------------------------------------------
// �������� ����������� ������� ������� ������������� �����������
bool CDataTimeZT::IsTimeCalc()
{
	return m_zts_read >= m_zts_calc;
}
//------------------------------------------------------------------------------
// �������� ���������� ������� ���������� ������
bool CDataTimeZT::IsTimeAver()
{
	return m_zts_read >= m_zts_aver;
}
//------------------------------------------------------------------------------
// ����� ������ ������ ��� ������������� �������
// zts_dt - ������� ������ � ������� ������ ������ �������
// (������������� ��������)
void CDataTimeZT::ReStartSRV(const CZetTimeSpan & zts_dt)
{
	m_zts_read -= zts_dt;
}
//------------------------------------------------------------------------------
// ��������� ������ �������� ����� ������, ������� � ���������� �
// ������������ � ��������� ������������� ������ ���������� ��������.
// ������� ������, ������� � ���������� ������ ���� ������.
// timeCur - ������� ����� �������;
// timeForInit - ����� �������������.
// ��������������� ��������� ��������:
// TimeRead = RoundingByRules(timeCur - timeForInit);
// TimeCalc = TimeRead + timeForInit;
// TimeAver = RoundingByRules(timeCur + IntervalTimeAver)

void CDataTimeZT::SetFirstTimesByRules(
	const double timeCur,		// ������� ����� �������
	const double timeForInit)	// ����� �������������
{	// ����� ������� ������ ������ ---------------------------------------------
	bool bNotOk(true);
	double tRead = timeCur - timeForInit;
	double dtAver = m_zts_dtAver;

	if (m_zts_dtAver > CZetTimeSpan(1, 0))
	{	// �������� m_zts_aver �� ��������� ��������� ���������� ������.
		// ���� m_zts_aver ������ 1 ��� � ������ ������, m_zts_aver ������ � ������
		// ���� m_zts_aver ������ 1 ��� � ������ ����, m_zts_aver ������ � ������
		if ((RoundToInt(86400 / dtAver) * dtAver == 86400.))
		{
			bNotOk = false;
			m_zts_read = floor(tRead / dtAver) * dtAver;
		}
	}
	if (bNotOk)
	{
		m_zts_read = tRead;
		m_zts_read.RoundSecondsDown();
	}
	CZetTimeSpan m_zts_read_round(m_zts_read);
	// ���� �������� ������� ������ ������ 1 ���
	if ((0i64 == m_zts_dtRead.GetSeconds()) && (0 < m_zts_dtRead.GetNanoseconds()))
			m_zts_read += m_zts_dtRead;

	// ������ ����� ������� ���������� ������ ---------------------------------
	int m = RoundToInt(timeForInit / dtAver);
	double t = m * dtAver;
	CZetTimeSpan zts(t);
	if (timeForInit > t)
		zts += m_zts_dtAver;
	m_zts_aver = m_zts_read_round + zts + m_zts_dtAver;

	// ����� ������� �������
	zts = (double)m_zts_read + timeForInit;
	zts.RoundSeconds();
	SetTimeCalc(zts);
}
//------------------------------------------------------------------------------