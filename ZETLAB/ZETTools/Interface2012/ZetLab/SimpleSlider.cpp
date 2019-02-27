//------------------------------------------------------------------------------
// SimpleSlider.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\ZetLab\SimpleSlider.h"
//------------------------------------------------------------------------------
CSimpleSlider::CSimpleSlider(CCustomContainer *owner, CRect rect) :
				CCustomSlider(owner)
{
	if (m_pOwner)
	{
#ifdef _GDIPLUS 
		m_pDSlider = new CDSimpleSlider(rect, m_pOwner->GetPointerToGraphics(),
			m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime() );
#else
		m_pDSlider = new CDSimpleSlider(rect, m_pOwner->GetPointerToIsChange(),
			m_pOwner->GetPointerToTime() );
#endif
		m_pDClass = m_pDSlider;
	}
}
//------------------------------------------------------------------------------
CSimpleSlider::~CSimpleSlider()
{
	if (m_pDSlider)
		delete m_pDSlider;
}
//------------------------------------------------------------------------------
void CSimpleSlider::SetPositionValue(BOOL val)
{
	BOOL oldVal = m_pDSlider->GetPositionValue();
	m_pDSlider->SetPositionValue(val);
	if ( oldVal != m_pDSlider->GetPositionValue() )
		NewDouble( m_pDSlider->GetPositionValue() );
}
//------------------------------------------------------------------------------
void CSimpleSlider::OnMouseLeave()
{
	CCustomElement::OnMouseLeave();
	if ( m_pDSlider->GetNeedEvent() )
		NewDouble( m_pDSlider->GetPositionValue() );
}
//------------------------------------------------------------------------------
void CSimpleSlider::OnLButtonClk(UINT nFlags, CPoint point)
{
	CCustomSlider::OnLButtonClk(nFlags, point);
	if ( m_pDSlider->GetNeedEvent() )
		NewDouble( m_pDSlider->GetPositionValue() );

}
//------------------------------------------------------------------------------
void CSimpleSlider::OnLButtonUp(UINT nFlags, CPoint point)
{
	CCustomSlider::OnLButtonUp(nFlags, point);
	if ( m_pDSlider->GetNeedEvent() )
		NewDouble( m_pDSlider->GetPositionValue() );
}
//------------------------------------------------------------------------------
void CSimpleSlider::SaveParameters(SimpleXML *pSimpleXML)
{
    CCustomElement::SaveParameters(pSimpleXML);
}
//------------------------------------------------------------------------------
void CSimpleSlider::LoadParameters(SimpleXML *pSimpleXML)
{
    CCustomElement::LoadParameters(pSimpleXML);
}
//------------------------------------------------------------------------------
void CSimpleSlider::Clear(void)
{
	m_pDSlider->Clear();
}
//------------------------------------------------------------------------------