//------------------------------------------------------------------------------
// SimpleSlider.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\ZetLab\Slider.h"
//------------------------------------------------------------------------------
CSlider::CSlider(CCustomContainer *owner, CRect rect) :
CCustomSlider(owner)
{
	if (m_pOwner)
	{
		m_pDSlider = new CDSlider(rect, m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime() );
		m_pDClass = m_pDSlider;
	}
}
//------------------------------------------------------------------------------
CSlider::~CSlider()
{
	if (m_pDSlider)
		delete m_pDSlider;
}
//------------------------------------------------------------------------------
void CSlider::SetPositionValue(float val)
{
	float oldVal = m_pDSlider->GetPositionValue();
	m_pDSlider->SetPositionValue(val);
	if ( oldVal != m_pDSlider->GetPositionValue() )
		NewDouble( m_pDSlider->GetPositionValue() );
}
//------------------------------------------------------------------------------
void CSlider::OnMouseLeave()
{
	CCustomElement::OnMouseLeave();
	if ( m_pDSlider->GetNeedEvent() )
		NewDouble( m_pDSlider->GetPositionValue() );
}
//------------------------------------------------------------------------------
void CSlider::OnLButtonClk(UINT nFlags, CPoint point)
{
	CCustomSlider::OnLButtonClk(nFlags, point);
	if ( m_pDSlider->GetNeedEvent() )
		NewDouble( m_pDSlider->GetPositionValue() );

}
//------------------------------------------------------------------------------
void CSlider::OnLButtonUp(UINT nFlags, CPoint point)
{
	CCustomSlider::OnLButtonUp(nFlags, point);
	if ( m_pDSlider->GetNeedEvent() )
		NewDouble( m_pDSlider->GetPositionValue() );
}
//------------------------------------------------------------------------------
void CSlider::SaveParameters(SimpleXML *pSimpleXML)
{
    CCustomElement::SaveParameters(pSimpleXML);
}
//------------------------------------------------------------------------------
void CSlider::LoadParameters(SimpleXML *pSimpleXML)
{
    CCustomElement::LoadParameters(pSimpleXML);
}
//------------------------------------------------------------------------------