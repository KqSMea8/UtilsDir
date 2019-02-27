#include "stdafx.h"
#include <ListCtrlToolTip/ListCtrlToolTip.h>

BEGIN_MESSAGE_MAP(CListCtrlToolTip, CListCtrl)
	ON_NOTIFY_EX(TTN_NEEDTEXTA, 0, OnToolNeedText)
	ON_NOTIFY_EX(TTN_NEEDTEXTW, 0, OnToolNeedText)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

CListCtrlToolTip::CListCtrlToolTip()
	: m_LastToolTipCol(-1)
	, m_LastToolTipRow(-1)
{
}

CListCtrlToolTip::~CListCtrlToolTip()
{
}

void CListCtrlToolTip::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	CListCtrl::PreSubclassWindow();

	// Focus retangle is not painted properly without double-buffering
#if (_WIN32_WINNT >= 0x501)
	SetExtendedStyle(LVS_EX_DOUBLEBUFFER | GetExtendedStyle());
#endif
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_HEADERDRAGDROP);
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_GRIDLINES);
	
	// Disable the CToolTipCtrl of CListCtrl so it won't disturb our own tooltip-ctrl
	GetToolTips()->Activate(FALSE);

	// Enable our own tooltip-ctrl and make it show tooltip even if not having focus
	VERIFY(m_OwnToolTipCtrl.Create(this, TTS_ALWAYSTIP));
	m_OwnToolTipCtrl.Activate(TRUE);
}


BOOL CListCtrlToolTip::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_OwnToolTipCtrl.m_hWnd)
		m_OwnToolTipCtrl.RelayEvent(pMsg);
	return CListCtrl::PreTranslateMessage(pMsg);
}

void CListCtrlToolTip::OnMouseMove(UINT nFlags, CPoint point)
{
	CPoint pt(GetMessagePos());
	ScreenToClient(&pt);

	// Find the subitem
	LVHITTESTINFO hitinfo = { 0 };
	hitinfo.flags = nFlags;
	hitinfo.pt = pt;
	SubItemHitTest(&hitinfo);

	if (m_LastToolTipCol != hitinfo.iSubItem || m_LastToolTipRow != hitinfo.iItem)
	{
		// Mouse moved over a new cell
		m_LastToolTipCol = hitinfo.iSubItem;
		m_LastToolTipRow = hitinfo.iItem;

		// Remove the old tooltip (if available)
		if (m_OwnToolTipCtrl.GetToolCount() > 0)
		{
			m_OwnToolTipCtrl.DelTool(this);
			m_OwnToolTipCtrl.Activate(FALSE);
		}

		// Add the new tooltip (if available)
		if (m_LastToolTipRow != -1)
		{
			// Not using CToolTipCtrl::AddTool() because it redirects the messages to CListCtrl parent
			TOOLINFO ti = { 0 };
			ti.cbSize = sizeof(TOOLINFO);
			ti.uFlags = TTF_IDISHWND;	// Indicate that uId is handle to a control
			ti.uId = (UINT_PTR)m_hWnd;	// Handle to the control
			ti.hwnd = m_hWnd;			// Handle to window to receive the tooltip-messages
			ti.hinst = AfxGetInstanceHandle();
			ti.lpszText = LPSTR_TEXTCALLBACK;
			m_OwnToolTipCtrl.SendMessage(TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);
			m_OwnToolTipCtrl.Activate(TRUE);
		}
	}

	CListCtrl::OnMouseMove(nFlags, point);
}

BOOL CListCtrlToolTip::OnToolNeedText(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
{
	CPoint pt(GetMessagePos());
	ScreenToClient(&pt);

	int nRow, nCol;
	CellHitTest(pt, nRow, nCol);

	CString tooltip = GetToolTipText(nRow, nCol);
	if (tooltip.IsEmpty())
		return FALSE;

	// Non-unicode applications can receive requests for tooltip-text in unicode
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
#ifndef _UNICODE
	if (pNMHDR->code == TTN_NEEDTEXTA)
		lstrcpyn(pTTTA->szText, static_cast<LPCTSTR>(tooltip), sizeof(pTTTA->szText));
	else
		_mbstowcs(pTTTW->szText, static_cast<LPCTSTR>(tooltip), sizeof(pTTTW->szText) / sizeof(WCHAR));
#else
	if (pNMHDR->code == TTN_NEEDTEXTA)
		_wcstombsz(pTTTA->szText, static_cast<LPCTSTR>(tooltip), sizeof(pTTTA->szText));
	else
		LPTSTR Str = lstrcpyn(pTTTW->szText, static_cast<LPCTSTR>(tooltip), sizeof(pTTTW->szText) / sizeof(WCHAR));
#endif
	// If wanting to display a tooltip which is longer than 80 characters,
	// then one must allocate the needed text-buffer instead of using szText,
	// and point the TOOLTIPTEXT::lpszText to this text-buffer.
	// When doing this, then one is required to release this text-buffer again
	return TRUE;
}

void CListCtrlToolTip::CellHitTest(const CPoint& pt, int& nRow, int& nCol) const
{
	nRow = -1;
	nCol = -1;

	LVHITTESTINFO lvhti = { 0 };
	lvhti.pt = pt;
	nRow = ListView_SubItemHitTest(m_hWnd, &lvhti);	// SubItemHitTest is non-const
	nCol = lvhti.iSubItem;
	if (!(lvhti.flags & LVHT_ONITEMLABEL))
		nRow = -1;
}

CString CListCtrlToolTip::GetToolTipText(int nRow, int nCol)
{
	if (nRow != -1 && nCol != -1)
		return GetItemText(nRow, nCol);	// Cell-ToolTip
	else
		return CString("");
}