//------------------------------------------------------------------------------
// Regulator.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\ZetLab\Regulator.h"
//------------------------------------------------------------------------------
CRegulator::CRegulator(CCustomContainer *owner, CRect rect, bool variant, bool need_scale, float start_angle) :
CCustomRegulator(owner)
{
	if (m_pOwner)
	{
#ifdef _GDIPLUS 
		m_pDRegulator = new CDRegulator(rect, m_pOwner->GetPointerToGraphics(),
			m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime() );
#else
		m_pDRegulator = new CDRegulator(rect, m_pOwner->GetPointerToIsChange(),
			m_pOwner->GetPointerToTime(), variant, need_scale, start_angle );
#endif
		m_pDClass = m_pDRegulator;
		m_variant = variant;
	}
}
//------------------------------------------------------------------------------
CRegulator::~CRegulator()
{
	if (m_pDRegulator)
		delete m_pDRegulator;
}
//------------------------------------------------------------------------------
void CRegulator::Clear(void)
{
	m_pDRegulator->Clear();
}
//------------------------------------------------------------------------------
void CRegulator::OnMouseLeave()
{
	CCustomElement::OnMouseLeave();
}
//------------------------------------------------------------------------------
void CRegulator::OnLButtonClk(UINT nFlags, CPoint point)
{
	CCustomRegulator::OnLButtonClk(nFlags, point);
}
//------------------------------------------------------------------------------
void CRegulator::OnLButtonUp(UINT nFlags, CPoint point)
{
	CCustomRegulator::OnLButtonUp(nFlags, point);
}
//------------------------------------------------------------------------------
void CRegulator::SaveParameters(SimpleXML *pSimpleXML)
{
    CCustomElement::SaveParameters(pSimpleXML);
}
//------------------------------------------------------------------------------
void CRegulator::SetScale (int quan)
{
    m_pDRegulator->SetScale(quan);
}
//------------------------------------------------------------------------------
int CRegulator::GetScale (void)
{
    return m_pDRegulator->GetScale();
}
//------------------------------------------------------------------------------