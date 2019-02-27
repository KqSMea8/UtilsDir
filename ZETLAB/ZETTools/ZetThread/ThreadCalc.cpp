//------------------------------------------------------------------------------
//	������ ThreadCalc.cpp
//	������ 02.08.2012
//	����������, ��� "����", �������� �.�.
//------------------------------------------------------------------------------
#include "StdAfx.h"
#include "ThreadCalc.h"
//------------------------------------------------------------------------------
DWORD WINAPI Calc_Thread(PVOID pParam)
{// ������� �������� ������ �������
	// ��������� ����� ��������� ����� ���������� ������ (��. �����-��������)
	ZetThreadParam0 *par0 = (ZetThreadParam0*)pParam;
	
	while ( par0->FlagJob )
	{// ���� ������ -----------------------------------------------------
		par0->FlagViewStop = false;
		// ���-�� ����� (��������, ...)
		if (par0->Function)
		{// ���� ������� ��������� ������ ������
			long kod = (*par0->Function)(par0->pParamFunction);
			if ( kod >= 0 )
			{// ������� � ���������� ��������� ������ ���� �� ���� ������
				if ( par0->hWndParent )
					::PostMessageW( par0->hWndParent, WM_END_CALC,
							(WPARAM)kod, 0 );		// ��� ��������
			}
			else
			{// ������� ������� ������
				if ( par0->hWndParent )
					::PostMessageW(par0->hWndParent, WM_ERROR_THREAD,
							(WPARAM)kod,		// ��� ������
							(LPARAM)(LPCWSTR)par0->NameThread);
			}			
		}
		// ������ ������������ ����� ----------------------------------
		par0->FlagViewStop = true;
		SuspendThread(par0->hThread);	// ��������� ������� � ����� while
	}// while ( par0->FlagJob )
	return 0;
}
//------------------------------------------------------------------------------
CThreadCalc::CThreadCalc() : CCustomZetThread()
{
}
//------------------------------------------------------------------------------
CThreadCalc::~CThreadCalc()
{// ���������� ������� ��������� ���������� �����, � ����� ������� �������,
// ����� �������� ��������, ����� ����� ��������� � ��� �������� ��������
	SynchroFinishThread();
}
//------------------------------------------------------------------------------
bool CThreadCalc::Create( CString nameThread, HWND hwnd,
						FunctionThread function, PVOID paramFunction )
{
	bool ret = function &&	// ����� ������� ������ ���� �����!
		CCustomZetThread::Create(nameThread, hwnd, function, paramFunction);
	if (ret)
		ret = CreateZetThread(&Calc_Thread);	// ������ ���������������� �����
	return ret;
}
//------------------------------------------------------------------------------