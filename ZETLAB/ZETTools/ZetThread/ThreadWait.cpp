//------------------------------------------------------------------------------
//	������ ThreadWait.cpp
//	����������, ��� "����", �������� �.�.
//------------------------------------------------------------------------------
#include "StdAfx.h"
#include "ThreadWait.h"
//------------------------------------------------------------------------------
DWORD WINAPI Calc_Thread_Wait(PVOID pParam)
{// ������� �������� ������
	long ret(0);
	// ��������� ����� ��������� ����� ���������� ������ (��. �����-��������)
	ZetThreadParam0 * par0 = (ZetThreadParam0*)pParam;
	ParamThreadWait * pptw = (ParamThreadWait*)par0->pParamNext;
	HANDLE * pHandle = pptw->pHandle;

	if ((pHandle[0] == NULL) || (pHandle[1] == NULL))
		return -1;

	while (par0->FlagJob)
	{
		switch (WaitForMultipleObjects(2, pHandle, FALSE, INFINITE))
		{
		case WAIT_OBJECT_0:
			ret = (*par0->Function)(par0->pParamFunction);
			::PostMessage(par0->hWndParent, WM_END_WAIT, (WPARAM)ret, 0);
			break;

		case WAIT_OBJECT_0 + 1:
			par0->FlagJob = false;
			break;
		}
	}// while ( par0->FlagJob )
	return 0;
}
//------------------------------------------------------------------------------
CThreadWait::CThreadWait() : CCustomZetThread()
, m_bObjectInternal(true)
{
	ZeroMemory(&m_ptw, sizeof(m_ptw));
	zParam0.pParamNext = &m_ptw;
}
//------------------------------------------------------------------------------
CThreadWait::~CThreadWait()
{// ���������� ������� ��������� ���������� �����, � ����� ������� �������,
// ����� �������� ��������, ����� ����� ��������� � ��� �������� ��������
	SetEvent(m_ptw.pHandle[1]);
	Sleep(20);
	SynchroFinishThread();

	if (m_bObjectInternal)
		CloseHandle(m_ptw.pHandle[0]);
	
	CloseHandle(m_ptw.pHandle[1]);
}
//------------------------------------------------------------------------------
bool CThreadWait::Create( CString nameThread, HWND hwnd,
						FunctionThread function, PVOID paramFunction )
{
	bool ret = function &&	// ����� ������� ������ ���� �����!
		CCustomZetThread::Create(nameThread, hwnd, function, paramFunction);
	if (ret)
	{
		m_ptw.pHandle[0] = CreateEventW(	// ������ ��������� ������� ��� ����������� ������
			NULL,	// ������ �� ���������
			FALSE,	// � ������ �������
			FALSE,	// � ������� ���������
			NULL);	// ��� �����

		m_ptw.pHandle[1] = CreateEventW(	// ������ ��������� ������� ��� ���������� ������
			NULL,	// ������ �� ���������
			TRUE,	// � ������ �������
			FALSE,	// � ������� ���������
			NULL);	// ��� �����

		ret = (m_ptw.pHandle[0] != NULL) && (m_ptw.pHandle[1] != NULL);
	}
	if (ret)
		ret = CreateZetThread(&Calc_Thread_Wait);	// ������ ���������������� �����
	return ret;
}
//------------------------------------------------------------------------------
long CThreadWait::End()
{// ����������� ������� ������ "�����"
	SetEvent(m_ptw.pHandle[1]);
	return CCustomZetThread::End();
}
//------------------------------------------------------------------------------
long CThreadWait::EndAndWait()
{// ���������� ������� ������ "�����"
	SetEvent(m_ptw.pHandle[1]);
	zParam0.FlagStop = false;
	zParam0.FlagViewStop = true;
	return CCustomZetThread::EndAndWait();
}
//------------------------------------------------------------------------------
long CThreadWait::Stop()
{// ����������� ������� ������ "����"
	zParam0.FlagStop = false;
	zParam0.FlagViewStop = true;
	return CCustomZetThread::Stop();
}
//------------------------------------------------------------------------------
long CThreadWait::StopAndWait()
{// ���������� ������� ������ "����"
	return Stop();
}
//------------------------------------------------------------------------------
void CThreadWait::SetExternalSynchroObject(HANDLE handle)
{
	if (GetStatus() == 0)
		EndAndWait();

	CloseHandle(m_ptw.pHandle[0]);
	m_bObjectInternal = false;
	m_ptw.pHandle[0] = handle;
}
//------------------------------------------------------------------------------
void CThreadWait::SetSynchro()
{
	if (m_bObjectInternal)
		SetEvent(m_ptw.pHandle[0]);
}
//------------------------------------------------------------------------------