//------------------------------------------------------------------------------
//	���� CustomZetThread.h
//	������ �� 21.08.2012
//	����������, ��� "����", �������� �.�.
//------------------------------------------------------------------------------
//		������� ����� ���� �������, ��������� ������. 
//   ����� �� ������� �������, � �������� ���������� ����� ���������� � �������
// ��� �������-����������� (����������, ���������, ���������� ������ ������).
//	���� �������� ������� ������:
//	 0 - �������� ������� ���������
//	 1 - �������� ������� ���� ��������� �� ������ �������
//	-1 - ����� �������� ���� ������ �� ������ �������
//	-2 - ����� �������� ���������� �������� ������� ��������� �������������
//		 �������� TimeOut, ������� �� ��������� ����� 3 ���.
//	-3 - �� ������ ������� ��� ���������� � ������
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include "TypeStatusThread.h"
#include <process.h>
//------------------------------------------------------------------------------
typedef DWORD (__stdcall *_Thread) (void *);
typedef unsigned int (__stdcall *PTHREAD_START) (void *);
typedef long (*FunctionThread)(void *);
//------------------------------------------------------------------------------
#include <DefineWM.h>
//------------------------------------------------------------------------------
//   ���������, ���������� ��� ��������� ������. ��������� �� �� - ���
// �������� ��������� ���������� �������.
//   ��������� �������� ����� ��������� � ��������� �� ��������� �������
// ����������, ������� ������������ � �������-�����������.
struct ZetThreadParam0
{
	bool FlagJob;				// ���� ������������� ������ ������
	bool FlagStop;				// ���� ������������� ��������� ������
	bool FlagViewStop;			// ���� ��������� ������ (1-����� ��� 0-���)
	TypeStatusThread statusEx;		// ����������� ������ ������
	HANDLE hThread;				// ���������� ������
	HWND hWndParent;			// ���������� ����������� ����
	CString NameThread;			// ��� ������
	unsigned int ztId;			// ������������� ������
	DWORD TimeOut;				// ������������ ����� ���������� ������� � ������
	DWORD TimeOutSend;			// ������������ ����� �������� ���������� SendMessage
	DWORD TimeSleep;			// ����� �������� ������
	FunctionThread Function;	// ����� ��������� ���������� �������
	PVOID pParamFunction;		// ����� ���������� ��������� ���������� �������
	PVOID pParamNext;			// ����� ��������� ������� ���������� ������
};
//------------------------------------------------------------------------------
class CCustomZetThread
{
private:
	long _MyLastError();
	long _End(bool flagWait);
	long _Stop(bool flagWait);

protected:
	ZetThreadParam0 zParam0;	// ��������� ��������� ���������� �������
	bool CreateZetThread(_Thread function);	// �������� ����������������� ������
	void SynchroFinishThread();	// ���������� ���������� ������ ������

public:
	CCustomZetThread();
	virtual ~CCustomZetThread();
	virtual bool Create( CString nameThread, HWND hwnd,
						FunctionThread function, PVOID paramFunction = NULL );

	// ������ �������
	unsigned int GetId() { return zParam0.ztId; }
	CString GetNameThread()	{ return zParam0.NameThread; }
	HWND GetHWNDParent() { return zParam0.hWndParent; }
	TypeStatusThread GetStatusThread() { return zParam0.statusEx; }

	void SetFunction(FunctionThread function) { zParam0.Function = function; }

	long GetStatus();
	bool IsError() { return GetStatus() < 0; }
	bool IsRunning() { return GetStatus() == 0; }
	bool IsStopped() { return GetStatus() == 1; }
	bool IsCompleted() { return GetStatus() == 2; }
	
	DWORD GetTimeOut() { return zParam0.TimeOut; }
	void SetTimeOut(DWORD val) { zParam0.TimeOut = val; }
	
	DWORD GetTimeOutSend() { return zParam0.TimeOutSend; }
	void SetTimeOutSend(DWORD val) { zParam0.TimeOutSend = val; }
	
	DWORD GetTimeSleep() { return zParam0.TimeSleep; }
	void SetTimeSleep(DWORD val) { zParam0.TimeSleep = val; }	

	// ������ ������ ----------------------------
	virtual long End();				// ����������� ������� ������ "�����"
	virtual long EndAndWait();		// ���������� ������� ������ "�����"
	long Start();			// ������ ������
	virtual long Stop();			// ����������� ������� ������ "����"
	virtual long StopAndWait();		// ���������� ������� ������ "����"
};
//------------------------------------------------------------------------------