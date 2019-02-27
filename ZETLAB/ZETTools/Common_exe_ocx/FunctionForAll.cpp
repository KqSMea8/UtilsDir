//------------------------------------------------------------------------------
// FunctionForAll.cpp
//------------------------------------------------------------------------------
#include "StdAfx.h"
#include "FunctionForAll.h"
//------------------------------------------------------------------------------
// ������� Zet-������ ��������� �� ��������/�������� ����
//
void SendOpen(const HWND hWndPanel, const HWND hWndWin, const bool bOpen)
{
	if (hWndPanel)
		::PostMessage(hWndPanel, WM_SEND_CHILD_HWND, (WPARAM)hWndWin, (LPARAM)bOpen);
}
//------------------------------------------------------------------------------
// �������, ����������� ����������� ����������� �������� ����.
// ������������� ��� ������������� � ����������� ��������� WM_SIZING.
// pRect - ����� ������� ����;
// pRectOld - ������� ������� ����;
// cx_min - ����������� ������ ����;
// cy_min - ������������ ������ ����.
//
void _OnSizing(LPRECT pRect, LPRECT pRectOld, const long cx_min, const long cy_min)
{
	if ((pRect->right - pRect->left) < cx_min)
	{
		pRect->left = pRectOld->left;
		pRect->right = pRectOld->right;
	}
	if ((pRect->bottom - pRect->top) < cy_min)
	{
		pRect->top = pRectOld->top;
		pRect->bottom = pRectOld->bottom;
	}
	*pRectOld = *pRect; // ��������� ��������� ���������
}
//------------------------------------------------------------------------------