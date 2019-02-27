//------------------------------------------------------------------------------
// Panel.cpp : файл реализации
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\ZetLab\Panel.h"
//------------------------------------------------------------------------------
CPanel::CPanel(CCustomContainer *owner, CRect rect) :
				CCustomContainer(owner)
{
	m_pDPanel = NULL;
	if (m_pOwner)
	{
#ifdef _GDIPLUS
		m_pDPanel = new CDPanel(rect, m_pOwner->GetPointerToGraphics(),
			m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime() );
#else
		m_pDPanel = new CDPanel(rect, m_pOwner->GetPointerToIsChange(), 
			m_pOwner->GetPointerToTime() );
#endif
		m_pDClass = m_pDPanel;
	}
}
//------------------------------------------------------------------------------
CPanel::~CPanel()
{
	if (m_pDPanel)
		delete m_pDPanel;
}
//------------------------------------------------------------------------------
void CPanel::OnDraw()
{
	m_pDClass->OnDraw();
	CCustomContainer::OnDraw();
}
//------------------------------------------------------------------------------
void CPanel::Clear()
{
	m_pDPanel->Clear();
}
//------------------------------------------------------------------------------
void CPanel::SaveParameters(SimpleXML *pSimpleXML)
{
    m_pDPanel->SaveParameters(pSimpleXML);
    CCustomContainer::SaveParameters(pSimpleXML);
}
//------------------------------------------------------------------------------
void CPanel::LoadParameters(SimpleXML *pSimpleXML)
{
    m_pDPanel->LoadParameters(pSimpleXML);
    CCustomContainer::LoadParameters(pSimpleXML);
}

