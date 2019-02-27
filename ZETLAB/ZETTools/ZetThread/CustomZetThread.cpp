//------------------------------------------------------------------------------
//	���� CustomZetThread.cpp
//	������ �� 21.08.2012
//	����������, ��� "����", �������� �.�.
//------------------------------------------------------------------------------
#include "stdafx.h"
#include <ZetThread\CustomZetThread.h>
//------------------------------------------------------------------------------
CCustomZetThread::CCustomZetThread()	// ����������� ������
{
	zParam0.TimeOut = 3000;
	zParam0.TimeOutSend = 500;
	zParam0.ztId = 0;
	zParam0.NameThread.Empty();
	zParam0.FlagJob = true;
	zParam0.FlagStop = false;
	zParam0.FlagViewStop = true;
	zParam0.statusEx = notStart;
	zParam0.hThread = NULL;
	zParam0.hWndParent = 0;
	zParam0.TimeSleep = 20;
	zParam0.Function = NULL;
	zParam0.pParamFunction = NULL;
	zParam0.pParamNext = NULL;

}
//------------------------------------------------------------------------------
bool CCustomZetThread::Create(
	CString nameThread,			// ��� ������
	HWND hwnd,					// hwnd �������� (��� ����� ���� ���������)
	FunctionThread function,	// ����������� � ������ ��������� ���������� �������
	PVOID paramFunction )		// ��������� �� � ��������
{	// function ����� ���� NULL 
	zParam0.NameThread = nameThread;
	zParam0.hWndParent = hwnd;
	zParam0.Function = function;
	zParam0.pParamFunction = paramFunction;
	return zParam0.Function != NULL;
}
//------------------------------------------------------------------------------
CCustomZetThread::~CCustomZetThread()	// ���������� ������
{// ����� ������ ����������� � ������������ �������-�����������!!!
// ����� �������� ��������, ����� ��������� ������� ������� ����������
// ��� ������������� ������. � ���� ������ ������������������ ��������
// ����� ���� ���������. � ������ ���������� ���������� ������� ���������
// ��������� ������, �.�. ������������ ���� zParam0.FlagJob � �����
// ���������� ��������� � ��������. ��� ���� ���������� ����������
// ����������, ��� ������������ ����� ������. ����� � ��� ����� �����������
// (�� �������� ����� FlagJob ��� �� �����), � ���������� ��������� �
// ��������� �������! ����� � ����!
// �������: � ����������� ��������� ������� ���������� �������� �������
// ������� ������ SynchroFinishThread.
	if (zParam0.hThread)
		CloseHandle(zParam0.hThread);
}
//------------------------------------------------------------------------------
bool CCustomZetThread::CreateZetThread(
	_Thread function)	// ��������� ���������� ������� ������
{//	�������� ����������������� ������
	bool ret;
	zParam0.hThread = (HANDLE)_beginthreadex(NULL, 0, (PTHREAD_START)function,
									&zParam0, CREATE_SUSPENDED, &zParam0.ztId);
	if (zParam0.hThread)
		ret = true;
	else
		ret = false;
	return ret;
}
//------------------------------------------------------------------------------
void CCustomZetThread::SynchroFinishThread()
{	// ���������� ���������� ������ ������
	if ( EndAndWait() < 0 )
	{// �������, ����������� � ������ ������� ��� ���� ������
		#pragma warning(suppress: 6258)
		TerminateThread(zParam0.hThread, -1);			// ������ �����
		WaitForSingleObject(zParam0.hThread, zParam0.TimeOut);	// �����������
	}													// ���� ���������
}
//------------------------------------------------------------------------------
long CCustomZetThread::_MyLastError()
{
	long ret = GetLastError();
	if (ret > 0)
		ret = -ret;
	return ret;
}
//------------------------------------------------------------------------------
// ������ ������:
//	0 - �����������;
//	1 - ����������;
//	2 - ��������;
// <0 - ��� ������ - �������� � �������; 
long CCustomZetThread::GetStatus()
{
	long ret;
	switch ( WaitForSingleObject(zParam0.hThread, 0) )
	{
	case WAIT_TIMEOUT:	// ����� ��������
		ret = (long)zParam0.FlagViewStop;
		break;

	case WAIT_OBJECT_0:	// ����� ��� �������� ������
		ret = 2;
		break;

	default:			// WAIT_FAILED ������ ��� ������ ������
		ret = _MyLastError();		// ��� ��� �����������
	}
	return ret;
}
//------------------------------------------------------------------------------
long CCustomZetThread::_End(
	bool flagWait)	// ��� �������: ����������� - false, ���������� - true
{//  ��������� ����� ���������� ������ ������ (�� ��� ������)
//		���� ��������:
//  1 - ����� ��� ����������;
//  0 - ����� �������� (��� ������� ��������)
// -1 - ����� �� ���������� � ������� FTimeOut ���� (�����?)
// <0 - ��� ������ - �������� � �������� ��� � �������;
//----------------------------------
	long ret;
	// �������� ��������� ������
	switch ( WaitForSingleObject(zParam0.hThread, 0) )
	{
	case WAIT_TIMEOUT:	// ����� �� ��������
		zParam0.FlagJob = false;	// ������� ��������� �����
		// �������� ����� ����� ��� ��� (�� �������)
		if (zParam0.FlagViewStop)
		// ����� �����, ��� ���� ��������� ����� �� ��� ����������
			while ( ResumeThread(zParam0.hThread) > 1 );
		// ���� ����� ��������, �� ������ ������ �� ����
		ret = 0;
		// ���� ������� ����������
		if (flagWait)
		{	// ��� ���������
			switch ( WaitForSingleObject(zParam0.hThread, zParam0.TimeOut) )
			{
			case WAIT_OBJECT_0: //	����� ����������
				//ret = 0;		// ����� ����������, ������� ���������
				break;
			case WAIT_TIMEOUT:	// ����� �� ���������� � ������� FTimeOut ���
				ret = -1;		// ������ ����� �����
				break;
			default:			// WAIT_FAILED ������ ������
				ret = _MyLastError();	// ��� ��� �����������
			}
		}
		break;	// case WAIT_TIMEOUT:	// ����� �� ��������

	case WAIT_OBJECT_0:	// ������ ������ ��� ���������
		ret = 1;
		break;

	default:			// WAIT_FAILED ������ ��� ������ ������
		ret = _MyLastError();	// ��� ��� �����������
	}
	return ret;
}
//------------------------------------------------------------------------------
long CCustomZetThread::_Stop(
	bool flagWait)	// ��� �������: ����������� - false, ���������� - true
{//		��������� ����� ��������� ������ ������ (�� ��� ������)
//		���� ��������:
//  1 - ����� �����
//	0 - ����� ���������� (��� �������� ������� �����������)
// -1 - ����� ��� �������� ���� ������
// -2 - ����� �� ����������� � ������� FTimeOut ����
// <0 - ��� ������ - �������� � �������� ��� � �������;
//----------------------------------
	long ret;
	// �������� ��������� ������
	switch ( WaitForSingleObject(zParam0.hThread, 0) )
	{
	case WAIT_TIMEOUT:	// ����� ��������
		if (!zParam0.FlagViewStop)
		{// ����� �� �����
			zParam0.FlagStop = true;	// ������� ���������� �����
			ret = 0;
			if (flagWait)	// ���� ������� ���������� ��� ���������
			{
				for(DWORD n=0; n<zParam0.TimeOut/20; ++n)
				{// ��� ����� ����� �����������, �������� ����� ������ 20 ����
					Sleep(20);
					if (zParam0.FlagViewStop)
						break;
				}
				if (!zParam0.FlagViewStop)
					ret = -2;
			}
		}
		else
			ret = 1;
		break;
	case WAIT_OBJECT_0:	// ������ ������ ���������
		ret = -1;
		break;
	default:			// WAIT_FAILED ������ ������
		ret = _MyLastError();	// ��� ��� �����������
	}
	return ret;
}
//------------------------------------------------------------------------------
// ����������� (��� �������� ����������) ������� ������ "�����"
//--------------------
long CCustomZetThread::End()
{
	return _End(false);
}
//------------------------------------------------------------------------------
// ���������� (� ��������� ����������) ������� ������ "�����"
//---------------------------
long CCustomZetThread::EndAndWait()
{
	return _End(true);
}
//------------------------------------------------------------------------------
long CCustomZetThread::Start()
{//	������ ������ (���� �� �����)
//		���� ��������:
//	0 - ����� �������
// -1 - ����� ��� �������� ���� ������
// <0 - ��� ������ - �������� � �������;
	long ret;
	switch( WaitForSingleObject(zParam0.hThread, 0) )
	{
	case WAIT_OBJECT_0:	// ������ ������ ���������
		ret = -1;
		break;
	case WAIT_TIMEOUT:	// ����� �� ��������, �������� ���
		zParam0.FlagStop = false;
		if (zParam0.FlagViewStop)
		{//������ c ������ �������� ���������
			while (true)
			{
				if (ResumeThread(zParam0.hThread) <= 1)
					break;
			}
		}
		ret = 0;
		break;
	default:			//  WAIT_FAILED ������ ������
		ret = _MyLastError();	// ��� ��� �����������
	}
	return ret;
}
//------------------------------------------------------------------------------
long CCustomZetThread::Stop()			// ����������� ������� ������ "����"
{
	return _Stop(false);
}
//------------------------------------------------------------------------------
long CCustomZetThread::StopAndWait()	// ���������� ������� ������ "����"
{
	return _Stop(true);
}
//------------------------------------------------------------------------------