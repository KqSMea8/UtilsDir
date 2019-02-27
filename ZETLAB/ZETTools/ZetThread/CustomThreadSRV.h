//------------------------------------------------------------------------------
//	���� CustomThreadSRV.h
//	������ �� 28.08.2012
//	����������, ��� "����", �������� �.�.
//------------------------------------------------------------------------------
//		������� ����� ��� �������, ��������� ������ ��� ������ ������ �
// ������� � �� ���������. ��������� ������ CCustomZetThread.
//		����� �� ������� �������, � �������� ���������� ����� ���������� �
// �������, ������� ��������� � ������� ������.
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include "ATLComTime.h"
#include <Constants.h>
#include <ZetThread\CustomZetThread.h>
#include <ZETView_basic_class\CDSRV.h>
#include <chaninfo.h>
//------------------------------------------------------------------------------
struct ZetThreadParamSRV0
{// ��������� �������. ��� �������� �� �������� � ����� ���������� � ���������.
	CDSRV *pSRV;			// ��������� �� ��������� ������ �������
	int NumberChannels;		// ���-�� ������������ �������
	HANDLE hEventSynchro;	// ���������������� ������� ����������� �����������
	HANDLE hEventModify;	// ���������������� ������� ��� Modify SRV
	PVOID pParamNext;		// ����� ��������� ���������� ��������� ������
	double Time;			// ��������� ����� ������� (�� 0)
	double TimeInterval;	// �������� ������������� ������� �������
};
//------------------------------------------------------------------------------
class CCustomThreadSRV : public CCustomZetThread
{// ����������� ������� � �������-����������� ����� ����������������
private:

protected:
//	unsigned int FNumberChannelsMax;
	ZetThreadParamSRV0 parSRV0;
	double SetCurTime(long chan, long fast);
	bool TestSynchro();

public:
	CCustomThreadSRV();
	virtual bool Create( CString nameThread, HWND hwnd, CDSRV *psrv,
		int numberChannels,
		FunctionThread function, PVOID paramFunction = NULL );
	virtual ~CCustomThreadSRV(void);

	// ������ �������
	int GetNumberChannels() { return parSRV0.NumberChannels; }
	int GetNumberChannelsMax() { return MAXCHAN; }
	
	CDSRV* GetSRV() { return parSRV0.pSRV; }
	void SetSRV(CDSRV *val) { parSRV0.pSRV = val; }

	double GetTime() { return parSRV0.Time; }
	virtual void SetTime(double val) { parSRV0.Time = val; }

	double GetTimeInterval() { return parSRV0.TimeInterval; }
	virtual void SetTimeInterval(double val) { parSRV0.TimeInterval = val; }

	DATE GetTimeInDATE() { return
		parSRV0.pSRV->GetTimeSRVasDATE(parSRV0.Time); }

	DATE GetStartTimeInDATE() { return
		parSRV0.pSRV->GetTimeSRVasDATE(0); }

	// ������ ������
	virtual float* GetDataPointer(int index) {return NULL;}
	void StartSRV(long chan, long fast);
	virtual void SetNewTime(long chan, long fast) {
		parSRV0.Time = parSRV0.TimeInterval + SetCurTime(chan, fast); }
	virtual void ZeroTime() { parSRV0.Time = 0; }
	
	// �������, ����������� ������ ���������� ����������
	// �� ������ ���������� ����������� ������ ��� �����������
	// �������� ������� ����� ������ ���� ���
	void SetSynchro() {
		if (parSRV0.hEventSynchro) SetEvent(parSRV0.hEventSynchro); }

	void StopThreadForModifySRV() {
		if (parSRV0.hEventSynchro) ResetEvent(parSRV0.hEventSynchro); }
	void GoOnThreadAfterModifySRV() {
		if (parSRV0.hEventSynchro) SetEvent(parSRV0.hEventSynchro); }
};
//------------------------------------------------------------------------------