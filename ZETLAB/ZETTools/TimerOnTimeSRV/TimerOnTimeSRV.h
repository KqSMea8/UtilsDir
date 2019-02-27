//------------------------------------------------------------------------------
//	���� TimerOnTimeSRV.h
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include "float.h"
#include <ZETView_basic_class\CDSRV.h>
#include <ZETView_basic_class\zetbasic.h>
//------------------------------------------------------------------------------
class CTimerOnTimeSRV
{
private:
	bool m_bInit;
	bool m_bOneOff;			// ����������� ������������
	bool m_bRunnung;		// ������ �������
	CDSRV *m_psrv;
	HWND m_hWnd;
	UINT m_elapse;
	double m_timeActivation;	// SRV-����� ������������ �������

protected:
public:
	UINT_PTR m_id;			// ID �������
	double m_interval;		// �������� � ��� SRV-�������

	CTimerOnTimeSRV();
	virtual ~CTimerOnTimeSRV() {}

	bool Create(UINT_PTR timerID, UINT elapse, HWND hWnd, CDSRV *psrv);
	void KillTimer();
	UINT_PTR SetTimer(double interval_sec, bool bOneOff = false);
	bool TestTimerEvent();
};
//------------------------------------------------------------------------------