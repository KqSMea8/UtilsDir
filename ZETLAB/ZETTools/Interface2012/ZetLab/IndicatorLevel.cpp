//------------------------------------------------------------------------------
// IndicatorLevel.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\ZetLab\IndicatorLevel.h"
//------------------------------------------------------------------------------
CIndicatorLevel::CIndicatorLevel(CCustomContainer *owner, CRect rect) :
			CCustomIndicator(owner)
{
	m_pDIndicator = NULL;
	if (m_pOwner)
	{
		m_pDIndicator = new CDIndicatorLevel(rect, m_pOwner->GetPointerToGraphics(),
			m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime() );
		m_pDClass = m_pDIndicator;
		m_pDIndicator->SetFileNameTriangle(m_pOwner->GetFolderPNG() +
											L"\\IndicatorLevel\\Triangle");
	}
}
//------------------------------------------------------------------------------
CIndicatorLevel::~CIndicatorLevel()
{
	if (m_pDIndicator)
		delete m_pDIndicator;
}
//------------------------------------------------------------------------------
void CIndicatorLevel::OnLButtonClk(UINT nFlags, CPoint point)
{
	m_pDIndicator->OnLButtonClk(nFlags, point);
	CCustomElement::OnLButtonClk(nFlags, point);
}
//------------------------------------------------------------------------------