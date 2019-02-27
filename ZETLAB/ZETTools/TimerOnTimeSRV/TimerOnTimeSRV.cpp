//------------------------------------------------------------------------------
//	Файл TimerOnTimeSRV.cpp
//------------------------------------------------------------------------------
#include "StdAfx.h"
#include <TimerOnTimeSRV\TimerOnTimeSRV.h>
//------------------------------------------------------------------------------
CTimerOnTimeSRV::CTimerOnTimeSRV()
	: m_bInit(false)
	, m_bOneOff(true)
	, m_bRunnung(false)
	, m_id(1)
	, m_interval(1)
	, m_timeActivation(0.)
	, m_psrv(NULL)
	, m_elapse(NULL)
	, m_hWnd(0)
{}
//------------------------------------------------------------------------------
bool CTimerOnTimeSRV::Create(UINT_PTR timerID, UINT elapse,
	HWND hWnd, CDSRV *psrv)
{
	m_bInit = (hWnd != NULL) && (psrv != NULL);
	m_id = timerID;
	m_hWnd = hWnd;
	m_psrv = psrv;
	m_elapse = elapse;
	return m_bInit;
}
//------------------------------------------------------------------------------
void CTimerOnTimeSRV::KillTimer()
{
	::KillTimer(m_hWnd, m_id);
	m_bRunnung = false;
}
//------------------------------------------------------------------------------
UINT_PTR CTimerOnTimeSRV::SetTimer(double interval_sec, bool bOneOff)
{
	UINT_PTR ret(0);
	if (m_bInit && !m_bRunnung)
	{
		m_bOneOff = bOneOff;
		m_interval = interval_sec;
		m_timeActivation = m_psrv->WorkingTime() + m_interval;
		ret = ::SetTimer(m_hWnd, m_id, m_elapse, NULL);		
		m_bRunnung = ret == m_id;
	}
	return ret;
}
//------------------------------------------------------------------------------
bool CTimerOnTimeSRV::TestTimerEvent()
{
	bool ret = m_bInit && m_bRunnung;
	if (ret)
	{
		double timeSRV = m_psrv->WorkingTime();
		ret = timeSRV > m_timeActivation;
		if (ret)
		{// следующее срабатывание -------------------------
			if (m_bOneOff)
				KillTimer();
			else
				m_timeActivation = timeSRV + m_interval;	// += нельзя
		}
	}
	return ret;
}
//------------------------------------------------------------------------------