//------------------------------------------------------------------------------
// NumSelector.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\ZetLab\NumSelector.h"
#include <math.h>
//------------------------------------------------------------------------------
CNumSelector::CNumSelector(CCustomContainer *owner, CRect rect) :
CCustomRegulator(owner)
{
	if (m_pOwner)
	{
#ifdef _GDIPLUS 
		m_pDNumSelector = new CDNumSelector(rect, m_pOwner->GetPointerToGraphics(),
			m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime() );
#else
		m_pDNumSelector = new CDNumSelector(rect, m_pOwner->GetPointerToIsChange(),
			m_pOwner->GetPointerToTime() );
#endif
		m_pDClass = m_pDNumSelector;
	}
}
//------------------------------------------------------------------------------
CNumSelector::~CNumSelector()
{
	if (m_pDNumSelector)
		delete m_pDNumSelector;
}
//------------------------------------------------------------------------------
void CNumSelector::OnMouseLeave()
{
	CCustomElement::OnMouseLeave();
}
