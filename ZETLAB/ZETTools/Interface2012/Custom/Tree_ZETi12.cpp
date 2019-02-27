//------------------------------------------------------------------------------
// Tree_ZETi12.cpp : файл реализации
//
//------------------------------------------------------------------------------
//подключение стандартных заголовков
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\Custom\Tree_ZETi12.h"
#include "C:\ZETTools\Interface2012\Custom\CustomItem.h"
#include "C:\ZETTools\Interface2012\Custom\Dialog_ZETi12.h"
//------------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//------------------------------------------------------------------------------
// конструктор диалогового окна CDialog_ZETi12
CTree_ZETi12::CTree_ZETi12(
	UINT nIDTemplate,			// идентификатор программы
	CWnd* pMainWnd)				// указатель на главное окно программы,
	: CCustom_ZETi12(nIDTemplate, NULL)
{
	
	m_pMainWnd = (CCustom_ZETi12*)pMainWnd;
}
//------------------------------------------------------------------------------
CTree_ZETi12::~CTree_ZETi12()
{

}
//------------------------------------------------------------------------------
void CTree_ZETi12::Init(
	CString sFullFolderPNG,
	double *pTimeDraw)
{
	m_pTimeDraw = pTimeDraw;
	m_sFullFolderPNG = sFullFolderPNG;
}
//------------------------------------------------------------------------------
void CTree_ZETi12::DoDataExchange(CDataExchange* pDX)
{
	CCustom_ZETi12::DoDataExchange(pDX);
}
//------------------------------------------------------------------------------
void CTree_ZETi12::OnCancel()
{
	CDialog_ZETi12 *pParent = (CDialog_ZETi12*)m_pMainWnd;
}
//------------------------------------------------------------------------------
void CTree_ZETi12::OnLChannel(CString str)
{
	CDialog_ZETi12 *pParent = (CDialog_ZETi12*)m_pMainWnd;
	if (str != pParent->m_pAutoChanSelector->GetCaption()&& (str != L""))
	{	
		pParent->m_pAutoChanSelector->SetCaption(str);
		//CDialog_ZETi12::m_button = false;
		&CCustom_ZETi12::OnNotifyEvent_ChanPanel;
	}
}
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CTree_ZETi12, CCustom_ZETi12)
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()
//------------------------------------------------------------------------------
BOOL CTree_ZETi12::OnInitDialog()
{
	CCustom_ZETi12::OnInitDialog();
	long NewLong = DS_SETFONT | DS_FIXEDSYS | WS_POPUP | WS_SYSMENU;
	SetWindowLongW(m_hWnd, GWL_STYLE, NewLong);

	return TRUE;
}
//------------------------------------------------------------------------------
void CTree_ZETi12::OnDestroy()
{
	CCustom_ZETi12::OnDestroy();
}
//------------------------------------------------------------------------------
void CTree_ZETi12::OnSetFocus(CWnd* pOldWnd)
{// окно параметров получило фокус, главное окно также выводим на первый план
	if (m_pMainWnd && m_pMainWnd->m_hWnd)
		m_pMainWnd->SetWindowPos(&CWnd::wndTop,
		0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE |
		SWP_NOACTIVATE | SWP_SHOWWINDOW);
}
//------------------------------------------------------------------------------
