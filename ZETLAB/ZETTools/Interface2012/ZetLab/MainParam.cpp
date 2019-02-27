//------------------------------------------------------------------------------
// MainParam.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\ZetLab\MainParam.h"
#include "C:\ZETTools\Interface2012\Custom\Custom_ZETi12.h"
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
CMainParam::CMainParam(CCustomContainer *owner, CRect rect)
	: CCustomContainer(owner)
{
	m_pDParam = NULL;
	m_rect = rect;
	if (m_pOwner)
	{
		m_pDParam = new CDMainParam(rect, m_pOwner->GetPointerToGraphics(),
			m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime());
		m_pDClass = m_pDParam;
		CString str = m_pOwner->GetFolderPNG() + L"\\Main\\Main";
		m_pDParam->SetImageFileName(str);
	}
}
//------------------------------------------------------------------------------
#else
CMainParam::CMainParam(CCustomContainer *owner, CRect rect, int height)
	: CCustomContainer(owner)
{
	m_pDParam = NULL;
	m_rect = rect;
	m_addRect = 0;
	if (m_pOwner)
	{
		m_pDParam = new CDMainParam(rect, m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime(), height);
		//m_pDData->SetWidthHMME(0);
		//m_pDMain->SetSizeIcon(0);
		//m_pDMain->SetWidthBorder(3);
		//m_pDData->SetMainWindow(false);
		//m_pDData->SetWindowParam(false);
		m_pDClass = m_pDParam;
		//m_pOwner->SetParamRect(rect);
	}	
}
#endif
//------------------------------------------------------------------------------
void CMainParam::Clear (void)
{
	m_pDParam->Clear();
}
//------------------------------------------------------------------------------
CMainParam::~CMainParam()
{
	if (m_pDParam)
		delete m_pDParam;
}
//------------------------------------------------------------------------------
void CMainParam::OnDraw()
{
	m_pDParam->OnDraw();
	CCustomContainer::OnDraw();
}
//------------------------------------------------------------------------------
void CMainParam::SaveParameters(SimpleXML *pSimpleXML)
{
    m_pDParam->SaveParameters(pSimpleXML);
    CCustomContainer::SaveParameters(pSimpleXML);
}
//------------------------------------------------------------------------------
void CMainParam::LoadParameters(SimpleXML *pSimpleXML)
{
    m_pDParam->LoadParameters(pSimpleXML);
    CCustomContainer::LoadParameters(pSimpleXML);
}

void CMainParam::OnLButtonDown( UINT nFlags, CPoint point )
{
	if (!BelongsMenu) m_pOwner->CloseMenu();
		CloseMenu();
	CCustomContainer::OnLButtonDown(nFlags, point);
	MessageToParent(WM_NCLBUTTONDOWN, HTCAPTION, 0);
}

void CMainParam::SetParamAddRect(CRect rect)
{
	m_addRect = rect;
	m_pOwner->SetParamAddRect(m_addRect);
}
