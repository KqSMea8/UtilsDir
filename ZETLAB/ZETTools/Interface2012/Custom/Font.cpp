//------------------------------------------------------------------------------
// Font.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include <Interface2012\Custom\Font.h>
//------------------------------------------------------------------------------
CZFont::CZFont()
{
	Create();
}
//------------------------------------------------------------------------------
CZFont::CZFont(ParamFont paramFont)
{
	m_param = paramFont;
	Create();
}
//------------------------------------------------------------------------------
CZFont::CZFont(CString name, float size, Gdiplus::FontStyle style,
	COLORREF color)
{
	m_param.Size = size;	// размер в UintPoint = 1/72 дюйма
	m_param.Name = name;
	m_param.Color = color;
	m_param.Style = style;
	Create();
}
//------------------------------------------------------------------------------
CZFont::~CZFont()
{
	Delete();
}
//------------------------------------------------------------------------------
void CZFont::Create()
{
	m_pFont = new Gdiplus::Font(m_param.Name, m_param.Size, m_param.Style);
}
//------------------------------------------------------------------------------
void CZFont::Delete()
{
	DeleteGdiPlusObject(m_pFont);
}
//------------------------------------------------------------------------------
void CZFont::SetName(CString newVal)
{
	if (m_param.Name != newVal)
	{
		Delete();
		m_param.Name = newVal;
		Create();
	}
}
//------------------------------------------------------------------------------
void CZFont::SetSize(float newVal)
{
	if (m_param.Size != newVal)
	{
		Delete();
		m_param.Size = newVal;
		Create();
	}
}
//------------------------------------------------------------------------------
void CZFont::SetStyle(Gdiplus::FontStyle newVal)
{
	if (m_param.Style != newVal)
	{
		Delete();
		m_param.Style = newVal;
		Create();
	}
}
//------------------------------------------------------------------------------