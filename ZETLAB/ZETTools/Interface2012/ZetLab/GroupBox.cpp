//------------------------------------------------------------------------------
// GroupBox.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\ZetLab\GroupBox.h"
//------------------------------------------------------------------------------
CGroupBox::CGroupBox(CCustomContainer *owner, CRect rect) :
				CCustomContainer(owner)
{
	m_pDGroupBox = NULL;
	if (m_pOwner)
	{
#ifdef _GDIPLUS
		m_pDGroupBox = new CDGroupBox(rect, m_pOwner->GetPointerToGraphics(),
			m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime() );
#else
		m_pDGroupBox = new CDGroupBox(rect,
			m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime() );
#endif
		m_pDClass = m_pDGroupBox;
	}
}
//------------------------------------------------------------------------------
CGroupBox::~CGroupBox()
{
	if (m_pDGroupBox)
		delete m_pDGroupBox;
}
//------------------------------------------------------------------------------
void CGroupBox::OnDraw()
{
	m_pDGroupBox->OnDraw();
	CCustomContainer::OnDraw();
}
//------------------------------------------------------------------------------
void CGroupBox::SaveParameters(SimpleXML *pSimpleXML)
{
    m_pDGroupBox->SaveParameters(pSimpleXML);
    CCustomContainer::SaveParameters(pSimpleXML);
}
//------------------------------------------------------------------------------
void CGroupBox::LoadParameters(SimpleXML *pSimpleXML)
{
    m_pDGroupBox->LoadParameters(pSimpleXML);
    CCustomContainer::LoadParameters(pSimpleXML);
}
//------------------------------------------------------------------------------
void CGroupBox::OnLButtonDown( UINT nFlags, CPoint point )
{
	m_pOwner->CloseMenu();
	CCustomContainer::OnLButtonDown(nFlags, point);
	MessageToParent(WM_NCLBUTTONDOWN, HTCAPTION, 0);
}