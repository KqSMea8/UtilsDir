//------------------------------------------------------------------------------
//	���� CustomThreadSRV.cpp
//	������ �� 28.08.2012
//	����������, ��� "����", �������� �.�.
//------------------------------------------------------------------------------
#include "StdAfx.h"
#include <ZetThread\CustomThreadSRV.h>
//------------------------------------------------------------------------------
CCustomThreadSRV::CCustomThreadSRV() : CCustomZetThread()
{	// ��������� �� ���������
//	FNumberChannelsMax = 271;	// 128 - ����, 271 - ������� �����
	parSRV0.pSRV = NULL;
	parSRV0.NumberChannels = 0;
	parSRV0.Time = 0.;
	parSRV0.TimeInterval = 0.1;
	parSRV0.hEventSynchro = NULL;
	parSRV0.hEventModify = NULL;
}
//------------------------------------------------------------------------------
bool CCustomThreadSRV::Create(
	CString nameThread,			// ��� ������
	HWND hwnd,					// HWND ������������� ����
	CDSRV *psrv,				// ��������� �� ������
	int numberChannels,			// ���-�� �������
	FunctionThread function,	// ����� ��������� �������
	PVOID paramFunction )		// ��������� ��������� �������
{
	bool ret = psrv &&	// ����� ������� ������ ����
		CCustomZetThread::Create(nameThread, hwnd, function, paramFunction);
	if (ret)
	{
		zParam0.pParamNext = &parSRV0;
		parSRV0.pSRV = psrv;
		parSRV0.NumberChannels = numberChannels;
		// ������ ������� � ����������� � � ���������� (���������) ����������
		parSRV0.hEventSynchro = CreateEventW(NULL, FALSE, TRUE, NULL);
		// ������ ������� � ������ ������� � � ���������� (���������) ����������
		parSRV0.hEventModify = CreateEventW(NULL, TRUE, TRUE, NULL);
		ret =(parSRV0.hEventSynchro != NULL) && (parSRV0.hEventModify != NULL);
	}
	return  ret;
}
//------------------------------------------------------------------------------
CCustomThreadSRV::~CCustomThreadSRV()
{
	CloseHandle(parSRV0.hEventSynchro);
	parSRV0.hEventSynchro = NULL;		// �����������
	CloseHandle(parSRV0.hEventModify);
	parSRV0.hEventModify = NULL;
}
//------------------------------------------------------------------------------
bool CCustomThreadSRV::TestSynchro()
{
	bool ret;
	if (parSRV0.hEventSynchro)
	{
		if ( WaitForSingleObject(parSRV0.hEventSynchro,
								zParam0.TimeOutSend) == WAIT_OBJECT_0 )
			ret = true;
		else// if ( kod == WAIT_TIMEOUT )
			ret = false;
	}
	else
		ret = false;
	return ret;
}
//------------------------------------------------------------------------------
double CCustomThreadSRV::SetCurTime(long chan, long fast)
{// ������� �������� ������� �������, �������� 0,1 ��� ��� 1 ��� ��� 10 ���
	long ch(0);
	double dt(0.);
	switch(fast)
	{
	case 0: dt = 10.; break;
	case 1: dt = 1.; break;
	case 2: dt = 0.1; break;
	default : dt = 10.;
	}
	if ( (chan >= 0) && (chan < parSRV0.pSRV->QuanChan() ) )
		ch = chan;
	else
		ch = 0;
	parSRV0.Time =
			int( parSRV0.pSRV->CurrentTime(chan) * dt + 0.5 ) / dt;
	return parSRV0.Time;
}
//------------------------------------------------------------------------------
void CCustomThreadSRV::StartSRV(long chan, long fast)
{// ������ ������ � �������� ������� ������� ����������, �������� fast
	SetNewTime(chan, fast);
	Start();
}
//------------------------------------------------------------------------------