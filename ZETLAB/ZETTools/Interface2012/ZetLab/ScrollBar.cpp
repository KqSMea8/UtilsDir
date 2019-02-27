//------------------------------------------------------------------------------
// ZScrollBar.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\ZetLab\ScrollBar.h"
//------------------------------------------------------------------------------
CZScrollBar::CZScrollBar(CCustomContainer *owner, CRect rect) :
CCustomSlider(owner)
{
	if (m_pOwner)
	{
		m_pDScrollBar = new CDScrollBar(rect, m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime() );
		m_pDClass = m_pDScrollBar;
	}
}
//------------------------------------------------------------------------------
CZScrollBar::~CZScrollBar()
{
	if (m_pDScrollBar)
		delete m_pDScrollBar;
}
//------------------------------------------------------------------------------
void CZScrollBar::Clear (void)
{
	m_pDScrollBar->Clear();
}
//------------------------------------------------------------------------------
void CZScrollBar::SetPositionValue(float val)
{
	float oldVal = m_pDScrollBar->GetPositionValue();
	m_pDScrollBar->SetPositionValue(val);
	if ( oldVal != m_pDScrollBar->GetPositionValue() )
		NewDouble( m_pDScrollBar->GetPositionValue() );
}
//------------------------------------------------------------------------------
void CZScrollBar::OnMouseLeave()
{
	CCustomElement::OnMouseLeave();
	if ( m_pDScrollBar->GetNeedEvent() )
		NewDouble( m_pDScrollBar->GetPositionValue() );
}
//------------------------------------------------------------------------------
void CZScrollBar::OnLButtonClk(UINT nFlags, CPoint point)
{
	CCustomSlider::OnLButtonClk(nFlags, point);
	if ( m_pDScrollBar->GetNeedEvent() )
		NewDouble( m_pDScrollBar->GetPositionValue() );

}
//------------------------------------------------------------------------------
void CZScrollBar::OnLButtonUp(UINT nFlags, CPoint point)
{
	CCustomSlider::OnLButtonUp(nFlags, point);
	if ( m_pDScrollBar->GetNeedEvent() )
		NewDouble( m_pDScrollBar->GetPositionValue() );
}
//------------------------------------------------------------------------------
void CZScrollBar::SaveParameters(SimpleXML *pSimpleXML)
{
	CCustomElement::SaveParameters(pSimpleXML);
}
//------------------------------------------------------------------------------
void CZScrollBar::LoadParameters(SimpleXML *pSimpleXML)
{
	CCustomElement::LoadParameters(pSimpleXML);
}
//------------------------------------------------------------------------------