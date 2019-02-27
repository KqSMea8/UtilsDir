//------------------------------------------------------------------------------
// MainParam.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\ZetLab\MainData.h"
#include "C:\ZETTools\Interface2012\Custom\Custom_ZETi12.h"
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
CMainData::CMainData(CCustomMain *owner, CRect rect)
	: CCustomContainer(owner)
{
	m_pDData = NULL;
	m_rect = rect;
	if (m_pOwner)
	{
		m_pDData = new CDMainData(rect, m_pOwner->GetPointerToGraphics(),
			m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime());
		m_pDClass = m_pDData;
		CString str = m_pOwner->GetFolderPNG() + L"\\Main\\Main";
		m_pDData->SetImageFileName(str);
	}	
}
//------------------------------------------------------------------------------
#else
CMainData::CMainData(CCustomMain *owner, CRect rect)
	: CCustomContainer(owner)
{
	m_pDData = NULL;
	m_rect = rect;
	if (m_pOwner)
	{
		m_pDData = new CDMainData(rect, m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime());
		m_pDClass = m_pDData;
	}	
}
#endif
//------------------------------------------------------------------------------
void CMainData::Clear (void)
{
	m_pDData->Clear();

}
//------------------------------------------------------------------------------
CMainData::~CMainData()
{
	if (m_pDData)
		delete m_pDData;
}
//------------------------------------------------------------------------------
void CMainData::OnDraw()
{
	m_pDData->OnDraw();
	CCustomContainer::OnDraw();
}
//------------------------------------------------------------------------------
void CMainData::SaveParameters(SimpleXML *pSimpleXML)
{
    m_pDData->SaveParameters(pSimpleXML);
    CCustomContainer::SaveParameters(pSimpleXML);
}
//------------------------------------------------------------------------------
void CMainData::LoadParameters(SimpleXML *pSimpleXML)
{
    m_pDData->LoadParameters(pSimpleXML);
    CCustomContainer::LoadParameters(pSimpleXML);
}
//------------------------------------------------------------------------------
void CMainData::OnLButtonDown( UINT nFlags, CPoint point )
{
	if (!BelongsMenu) m_pOwner->CloseMenu();
	CloseMenu();
	CCustomContainer::OnLButtonDown(nFlags, point);
	MessageToParent(WM_NCLBUTTONDOWN, HTCAPTION, 0);
}
