//------------------------------------------------------------------------------
// CustomGDI_ZET.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include <Interface2012\Custom\CustomAll.h>
#include <Dialog_ZET\Dialog_ZET.h>
//------------------------------------------------------------------------------
CCustomAll::CCustomAll()
	: m_IndexTabFocus(-1)
	, m_bToolTipEnabled(false)
	, m_bFlagClick(false)
	, m_hCursor(NULL)
	, m_butFixation(false)
	, OpenMenu(false)
	, BelongsMenu(false)
	, Removal(false)
	, m_Press(false)
{

}
//------------------------------------------------------------------------------
CCustomAll::~CCustomAll()
{
	if (m_hCursor)
		DestroyCursor(m_hCursor);
}
//------------------------------------------------------------------------------
BOOL CCustomAll::AddToolTip(CString str)
{
	BOOL ret;
	CWnd *pParent = GetParent();
	if (pParent)
		ret = ((CDialog_ZET*)pParent)->m_ptooltip->AddTool(pParent, str,
			&m_pDClass->GetRect(), 1);
	else
		ret = 0;
	m_bToolTipEnabled = ret != 0;
	return ret;
}
//------------------------------------------------------------------------------
BOOL CCustomAll::AddToolTip(UINT nIDS_STRING)
{
	BOOL ret;
	CWnd *pParent = GetParent();
	if (pParent)
		ret = ((CDialog_ZET*)pParent)->m_ptooltip->AddTool(pParent, nIDS_STRING,
			&m_pDClass->GetRect(), 1);
	else
		ret = 0;
	m_bToolTipEnabled = ret != 0;
	return ret;
}
//------------------------------------------------------------------------------
void CCustomAll::UpdateToolTipText(CString str)
{
	CWnd *pParent = GetParent();
	if (pParent)
		((CDialog_ZET*)pParent)->m_ptooltip->UpdateTipText(str, pParent, 1);
}
//------------------------------------------------------------------------------
void CCustomAll::UpdateToolTipText(UINT nIDS_STRING)
{
	CWnd *pParent = GetParent();
	if (pParent)
		((CDialog_ZET*)pParent)->m_ptooltip->
					UpdateTipText(nIDS_STRING, pParent, 1);
}
//------------------------------------------------------------------------------
void CCustomAll::SetCursor(LPCWSTR pCursorName)
{
	if (m_hCursor)
		DestroyCursor(m_hCursor);
	m_hCursor = LoadCursorW(NULL, pCursorName);
}
//------------------------------------------------------------------------------
void CCustomAll::SetCursorFromFile(LPCWSTR pCursorName)
{
	if (m_hCursor)
		DestroyCursor(m_hCursor);
	m_hCursor = LoadCursorFromFile(GetFolderPNG() + L"Cursor\\" + pCursorName);
}
//------------------------------------------------------------------------------
CWnd* CCustomAll::GetParent()
{
	CWnd *pParent;
	if (m_pCOwner)
		pParent = m_pCOwner->GetParent();
	else
		pParent = NULL;
	return pParent;
}
//------------------------------------------------------------------------------
CString CCustomAll::GetFolderPNG()
{
	CString ret(L"");
	if (m_pCOwner)
		ret = m_pCOwner->GetFolderPNG();
	else
		ret.Empty();
	return ret;
}
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
Gdiplus::Graphics** CCustomAll::GetPointerToGraphics()
{
	Gdiplus::Graphics **pret;
	if (m_pCOwner)
		pret = m_pCOwner->GetPointerToGraphics();
	else
		pret = NULL;
	return pret;
}
#endif
//------------------------------------------------------------------------------
double CCustomAll::GetTime()
{
	double ret(0.0);
	if (m_pCOwner)
		ret = m_pCOwner->GetTime();
	else
		ret = 0.;
	return ret;
}
//------------------------------------------------------------------------------
double* CCustomAll::GetPointerToTime()
{
	double *pret(nullptr);
	if (m_pCOwner)
		pret = m_pCOwner->GetPointerToTime();
	else
		pret = NULL;
	return pret;
}
//------------------------------------------------------------------------------
bool* CCustomAll::GetPointerToIsChange()
{
	bool *pret;
	if (m_pCOwner)
		pret = m_pCOwner->GetPointerToIsChange();
	else
		pret = NULL;
	return pret;
}
//------------------------------------------------------------------------------
void CCustomAll::OnMouseMove(UINT nFlags, CPoint point)
{	
	CWnd *pParent = GetParent();
	if (m_bToolTipEnabled && m_pDClass->GetVisible() && pParent)
	{
		MSG msg;
		msg.hwnd = pParent->m_hWnd;
		msg.message = WM_MOUSEMOVE;
		msg.wParam = (WPARAM)nFlags;
		msg.lParam = MAKELPARAM(point.x, point.y);
		msg.time = 0;
		msg.pt = (POINT)point;
		((CDialog_ZET*)pParent)->m_ptooltip->RelayEvent(&msg);
	}
	m_pDClass->OnMouseMove(nFlags, point);
}
//------------------------------------------------------------------------------
void CCustomAll::OnMouseLeave()
{
	m_bFlagClick = false;
	m_pDClass->OnMouseLeave();
}
//------------------------------------------------------------------------------
void CCustomAll::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bFlagClick)
	{
		if ( (abs(m_PointClick.x - point.x) < 2) && (abs(m_PointClick.y - point.y) < 2) )
		{
			OnLButtonClk(nFlags, point);
			//TRACE("CUSTOM CLICK %s\n", (CStringA)GetName());
		}
		m_bFlagClick = false;
	}
	m_pDClass->OnLButtonUp(nFlags, point);	
}
//------------------------------------------------------------------------------
void CCustomAll::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bFlagClick = true;
	m_PointClick = point;
	m_pDClass->OnLButtonDown(nFlags, point);
}
//------------------------------------------------------------------------------
void CCustomAll::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (m_bFlagClick)
	{
		if ((abs(m_PointClick.x - point.x) < 2) && (abs(m_PointClick.y - point.y) < 2))
		{
			//OnRButtonClk(nFlags, point);
			//TRACE("CUSTOM CLICK %s\n", (CStringA)GetName());
		}
		m_bFlagClick = false;
	}
	m_pDClass->OnRButtonUp(nFlags, point);
}
//------------------------------------------------------------------------------
void CCustomAll::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_bFlagClick = true;
	m_PointClick = point;
	m_pDClass->OnRButtonDown(nFlags, point);
}
//------------------------------------------------------------------------------