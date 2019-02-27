//------------------------------------------------------------------------------
// IndicatorLevel.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "LevelIndicator.h"
//------------------------------------------------------------------------------
CLevelIndicator::CLevelIndicator(CCustomContainer *owner, CRect rect, bool rotate, bool setdiod) :
CCustomIndicator(owner)
{
	m_pDLevelIndicator = NULL;
	m_hCursor = LoadCursorW(NULL, IDC_HAND);
	if (m_pOwner)
	{
#ifdef _GDIPLUS
		m_pDLevelIndicator = new m_pDLevelIndicator(rect, m_pOwner->GetPointerToGraphics(),
			m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime() );
#else
		m_pDLevelIndicator = new CDLevelIndicator(rect, m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime(), rotate, setdiod );
#endif
		m_pDClass = m_pDLevelIndicator;
	}
}
//------------------------------------------------------------------------------
void CLevelIndicator::Clear (void)
{
	m_pDLevelIndicator->Clear();
}
//------------------------------------------------------------------------------
CLevelIndicator::~CLevelIndicator()
{
	if (m_pDLevelIndicator)
		delete m_pDLevelIndicator;
}
//------------------------------------------------------------------------------
void CLevelIndicator::OnLButtonClk(UINT nFlags, CPoint point)
{
	m_pDLevelIndicator->OnLButtonClk(nFlags, point);
	CCustomElement::OnLButtonClk(nFlags, point);
}
//------------------------------------------------------------------------------
void CLevelIndicator::SaveParameters(SimpleXML *pSimpleXML)
{
    CCustomElement::SaveParameters(pSimpleXML);
}
//------------------------------------------------------------------------------