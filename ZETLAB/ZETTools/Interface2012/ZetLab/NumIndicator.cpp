//------------------------------------------------------------------------------
// IndicatorLevel.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\ZetLab\NumIndicator.h"
//------------------------------------------------------------------------------
CNumIndicator::CNumIndicator(CCustomContainer *owner, CRect rect) :
CCustomIndicator(owner)
{
	m_pDNumIndicator = NULL;
	m_hCursor = LoadCursorW(NULL, IDC_HAND);
	if (m_pOwner)
	{
#ifdef _GDIPLUS
		m_pDNumIndicator = new CDNumIndicator(rect, m_pOwner->GetPointerToGraphics(),
			m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime() );
#else
		m_pDNumIndicator = new CDNumIndicator(rect,
			m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime() );
#endif
		m_pDClass = m_pDNumIndicator;
	}
}
//------------------------------------------------------------------------------
CNumIndicator::~CNumIndicator()
{
	if (m_pDNumIndicator)
		delete m_pDNumIndicator;
}
//------------------------------------------------------------------------------
void CNumIndicator::OnLButtonClk(UINT nFlags, CPoint point)
{
	m_pDNumIndicator->OnLButtonClk(nFlags, point);
	CCustomElement::OnLButtonClk(nFlags, point);
}
//------------------------------------------------------------------------------
void CNumIndicator::SaveParameters(SimpleXML *pSimpleXML)
{
     CCustomElement::SaveParameters(pSimpleXML);
}
//------------------------------------------------------------------------------
void CNumIndicator::LoadParameters(SimpleXML *pSimpleXML)
{
    CCustomElement::LoadParameters(pSimpleXML);
}