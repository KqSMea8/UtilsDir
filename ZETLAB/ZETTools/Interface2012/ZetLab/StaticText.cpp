//------------------------------------------------------------------------------
// StaticText.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\ZetLab\StaticText.h"
//------------------------------------------------------------------------------
CStaticText::CStaticText(CCustomContainer *owner, CRect rect) :
				CCustomElement(owner)
{
	m_pDText = NULL;
	if (m_pOwner)
	{
#ifdef _GDIPLUS
		m_pDText = new CDStaticText(rect, m_pOwner->GetPointerToGraphics(),
			m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime() );
#else
		m_pDText = new CDStaticText(rect,
			m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime() );
#endif
		m_pDClass = m_pDText;
	}
}
//------------------------------------------------------------------------------
CStaticText::~CStaticText()
{
	if (m_pDText)
		delete m_pDText;
}
//------------------------------------------------------------------------------
void CStaticText::SaveParameters(SimpleXML *pSimpleXML)
{
    CCustomElement::SaveParameters(pSimpleXML);
}
//------------------------------------------------------------------------------
void CStaticText::LoadParameters(SimpleXML *pSimpleXML)
{
    CCustomElement::LoadParameters(pSimpleXML);
}
//------------------------------------------------------------------------------