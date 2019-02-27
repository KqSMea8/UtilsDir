//------------------------------------------------------------------------------
// DStaticImage.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\Draw\DStaticImage.h"
//------------------------------------------------------------------------------
int g_nStaticImage = 0;		// счётчик статических картинок
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
CDStaticImage::CDStaticImage(CRect rect, PVOID ppGraphics,
				bool *pFlagIsChange, double *pTimeAnimation) :
				CCustomDraw(rect, ppGraphics, pFlagIsChange, pTimeAnimation)
#else
CDStaticImage::CDStaticImage(CRect rect,
	bool *pFlagIsChange, double *pTimeAnimation) :
	CCustomDraw(rect, pFlagIsChange, pTimeAnimation)
#endif
{
	m_pParamDraw = &m_param;	// обеспечение ОБЩИХ параметров
	m_param.nElement = ++g_nStaticImage;
	SetNameWithNumber(L"StaticImage");
	//TRACE("CREATE CDStaticImage %s\n", (CStringA)m_pParamDraw->Name);
	// переменные класса
	m_pImage = NULL;
}
//------------------------------------------------------------------------------
CDStaticImage::~CDStaticImage()
{
	//TRACE("DELETE CDStaticImage %s\n", (CStringA)m_pParamDraw->Name);
#ifdef _GDIPLUS
	DeleteGdiPlusObject(m_pImage);
#endif
}
//------------------------------------------------------------------------------
void CDStaticImage::SetImageFileName(CString str)
{
	if ( !str.IsEmpty() && (m_param.fileName != str) )
	{
		m_param.fileName = str;
#ifdef _GDIPLUS
		LoadImageFromFile(&m_pImage, str);
#else
		m_pImage = LoadGLTextures(str, PicSize, 0);
#endif
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDStaticImage::SetCoord(float val)
{
	if ( m_param.CoordZ != val)
	{
		m_param.CoordZ = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
void CDStaticImage::OnDraw()
{// отображение
	if (m_param.bVisible)
	{
		Graphics *pGraphics = *((Graphics**)m_ppGraphics);
		pGraphics->DrawImage(m_pImage,
				m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height() );
	}
}
#else
void CDStaticImage::OnDraw()
{// отображение
	if (m_param.bVisible)
	{
		glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT);
		glPushMatrix();
		glLoadIdentity();
		glColor3f(1.0f,1.0f,1.0f);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_pImage);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 1);
		glVertex3f(m_rect.left, m_rect.top, m_param.CoordZ);
		glTexCoord2f(0, 0);
		glVertex3f(m_rect.left, m_rect.bottom, m_param.CoordZ);
		glTexCoord2f(1, 0);
		glVertex3f(m_rect.right, m_rect.bottom, m_param.CoordZ);
		glTexCoord2f(1, 1);
		glVertex3f(m_rect.right, m_rect.top, m_param.CoordZ);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_COLOR_MATERIAL);
		glPopMatrix();
		glPopAttrib();
		glFinish();
	}
}
#endif
//------------------------------------------------------------------------------