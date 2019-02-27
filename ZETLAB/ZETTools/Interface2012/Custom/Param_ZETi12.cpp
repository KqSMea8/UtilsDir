﻿//------------------------------------------------------------------------------
// Param_ZETi12.cpp : файл реализации
//
//------------------------------------------------------------------------------
//подключение стандартных заголовков
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\Custom\Param_ZETi12.h"
#include "C:\ZETTools\Interface2012\Custom\CustomItem.h"
#include "C:\ZETTools\Interface2012\Custom\Dialog_ZETi12.h"
//------------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//------------------------------------------------------------------------------
// конструктор диалогового окна CDialog_ZETi12
CParam_ZETi12::CParam_ZETi12(
	UINT nIDTemplate,			// идентификатор программы
	CWnd* pMainWnd)				// указатель на главное окно программы,
	: CCustom_ZETi12(nIDTemplate, NULL)
{
	m_pMainContainer = NULL;
	m_pMainWnd = (CCustom_ZETi12*)pMainWnd;
}
//------------------------------------------------------------------------------
CParam_ZETi12::~CParam_ZETi12()
{
	if (m_pMainContainer)
		delete m_pMainContainer;
}
//------------------------------------------------------------------------------
void CParam_ZETi12::Init(
	CString sFullFolderPNG,
	double *pTimeDraw)
{
	m_pTimeDraw = pTimeDraw;
	m_sFullFolderPNG = sFullFolderPNG;
}
//------------------------------------------------------------------------------
void CParam_ZETi12::DoDataExchange(CDataExchange* pDX)
{
	CCustom_ZETi12::DoDataExchange(pDX);
}
//------------------------------------------------------------------------------
void CParam_ZETi12::OnCancel()
{
	CDialog_ZETi12 *pParent = (CDialog_ZETi12*)m_pMainWnd;
	if (pParent)
	{
		if (pParent->m_pAutoSliderParam)
			pParent->m_pAutoSliderParam->SetPositionValue(0);
	}
}
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CParam_ZETi12, CCustom_ZETi12)
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()
//------------------------------------------------------------------------------
BOOL CParam_ZETi12::OnInitDialog()
{
	CCustom_ZETi12::OnInitDialog();
	long NewLong = DS_SETFONT | DS_FIXEDSYS | WS_POPUP | WS_SYSMENU;
	SetWindowLongW(m_hWnd, GWL_STYLE, NewLong);
	
	return TRUE;
}
//------------------------------------------------------------------------------
void CParam_ZETi12::OnDestroy()
{
	if (m_pMainContainer)
		delete m_pMainContainer;
	CCustom_ZETi12::OnDestroy();
}
//------------------------------------------------------------------------------
void CParam_ZETi12::OnSetFocus(CWnd* pOldWnd)
{// окно параметров получило фокус, главное окно также выводим на первый план
	if (m_pMainWnd && m_pMainWnd->m_hWnd)
		m_pMainWnd->SetWindowPos(&CWnd::wndTop,
					0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE |
					SWP_NOACTIVATE | SWP_SHOWWINDOW);
}
//------------------------------------------------------------------------------
void CParam_ZETi12::OnTimerDraw()
{
	if (m_pMainContainer)
	{
#ifdef _GDIPLUS
		m_pMainContainer->OnTimer();
<<<<<<< .mine
#else
		wglMakeCurrent((pDC_param->GetSafeHdc()), hrc_param);
		m_pMainContainer->OnTimer();
		m_pMainContainer->OnDraw();
#endif
	}
=======
#else 
		wglMakeCurrent((pDC_param->GetSafeHdc()), hrc_param);
		m_pMainContainer->OnTimer();
		m_pMainContainer->OnDraw();
#endif
	}
>>>>>>> .r7469
}
//------------------------------------------------------------------------------