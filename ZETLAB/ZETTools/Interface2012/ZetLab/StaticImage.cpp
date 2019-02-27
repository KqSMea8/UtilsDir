//------------------------------------------------------------------------------
// StaticImage.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\ZetLab\StaticImage.h"
//------------------------------------------------------------------------------
CStaticImage::CStaticImage(CCustomContainer *owner, CRect rect) :
				CCustomElement(owner)
{
	m_pDImage = NULL;
	if (m_pOwner)
	{
#ifdef _GDIPLUS
		m_pDImage = new CDStaticImage(rect, m_pOwner->GetPointerToGraphics(),
			m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime() );
#else
		m_pDImage = new CDStaticImage(rect,
			m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime() );
#endif
		m_pDClass = m_pDImage;
	}
}
//------------------------------------------------------------------------------
CStaticImage::~CStaticImage()
{
	if (m_pDImage)
		delete m_pDImage;
}
//------------------------------------------------------------------------------