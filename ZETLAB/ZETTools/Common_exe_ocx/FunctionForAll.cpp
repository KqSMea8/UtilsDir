//------------------------------------------------------------------------------
// FunctionForAll.cpp
//------------------------------------------------------------------------------
#include "StdAfx.h"
#include "FunctionForAll.h"
//------------------------------------------------------------------------------
// ѕосылка Zet-панели сообщени€ об открытии/закрытии окна
//
void SendOpen(const HWND hWndPanel, const HWND hWndWin, const bool bOpen)
{
	if (hWndPanel)
		::PostMessage(hWndPanel, WM_SEND_CHILD_HWND, (WPARAM)hWndWin, (LPARAM)bOpen);
}
//------------------------------------------------------------------------------
// ‘ункци€, реализующа€ ограничение минимальных размеров окна.
// ѕредназначена дл€ использовани€ в обработчике сообщени€ WM_SIZING.
// pRect - новые размеры окна;
// pRectOld - прежние размеры окна;
// cx_min - минимальна€ ширинв окна;
// cy_min - миинимальна€ высота окна.
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
	*pRectOld = *pRect; // запоминаю последнее положение
}
//------------------------------------------------------------------------------