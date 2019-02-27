//------------------------------------------------------------------------------
// CustomDraw.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include <Interface2012\Draw\CustomDraw.h>
//------------------------------------------------------------------------------
CCustomDraw::CCustomDraw(
	CRect rect,
#ifdef _GDIPLUS
	PVOID ppGraphics,		//	Graphics **ppGraphics,
#endif
	bool *pFlagIsChange,
	double *pTimeAnimation)
{
	m_rect = rect;
	m_pbIsChange = pFlagIsChange;
	m_pTimeAnimation = pTimeAnimation;
#ifdef _GDIPLUS
	m_ppGraphics = ppGraphics;
#endif
	m_pParamDraw = NULL;
}
//------------------------------------------------------------------------------
CCustomDraw::~CCustomDraw()
{
}
//------------------------------------------------------------------------------
CPoint CCustomDraw::GetClientPoint(CPoint point)
{
	return point - m_rect.TopLeft();
}
//------------------------------------------------------------------------------
void CCustomDraw::SetNameWithNumber(CString str)
{
	m_pParamDraw->Name.Format(str + L" %d", m_pParamDraw->nElement);
}
//------------------------------------------------------------------------------
void CCustomDraw::IsChange()
{
	if (m_pbIsChange)
		*m_pbIsChange = true;
}
//------------------------------------------------------------------------------
double CCustomDraw::GetTime()
{
	double ret(0.0);
	if (m_pTimeAnimation)
		ret = *m_pTimeAnimation;
	else
		ret = 0.;
	return ret;
}
//------------------------------------------------------------------------------
bool CCustomDraw::SetStruct(PVOID add)
{// копирование параметров из add в структуру
	bool ret;
	if (add)
		ret = 0 ==
			memcpy_s(m_pParamDraw, m_pParamDraw->Size, add, m_pParamDraw->Size);
	else
		ret = false;
	return ret;
}
//------------------------------------------------------------------------------
bool CCustomDraw::GetStruct(PVOID add)
{// копирование параметров из структуры в add
	bool ret;
	if (add)
		ret = 0 ==
			memcpy_s(add, m_pParamDraw->Size, m_pParamDraw, m_pParamDraw->Size);
	else
		ret = false;
	return ret;
}
//------------------------------------------------------------------------------
void CCustomDraw::SetVisible(bool val)
{
	if (m_pParamDraw->bVisible != val)
	{
		m_pParamDraw->bVisible = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CCustomDraw::SetEnable(bool val)
{
	if (m_pParamDraw->bEnable != val)
	{
		m_pParamDraw->bEnable = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CCustomDraw::GetFocus()
{
	m_pParamDraw->bFocus = true;
	IsChange();
}
//------------------------------------------------------------------------------
void CCustomDraw::KillFocus()
{
	m_pParamDraw->bFocus = false;
	IsChange();
}
//------------------------------------------------------------------------------
void CCustomDraw::OnMouseEnter()
{
	m_pParamDraw->bMouseCome = true;
	//TRACE("ENTER MOUSE %s\n", (CStringA)m_pParamDraw->Name);
	if (m_pParamDraw->bMouseChange &&
		m_pParamDraw->bVisible && m_pParamDraw->bEnable)
		IsChange();
}
//------------------------------------------------------------------------------
void CCustomDraw::OnMouseLeave()
{
	m_pParamDraw->bMouseCome = false;
	//TRACE("LEAVE MOUSE %s\n", (CStringA)m_pParamDraw->Name);
	if (m_pParamDraw->bMouseChange &&
		m_pParamDraw->bVisible && m_pParamDraw->bEnable)
		 IsChange();
}
//------------------------------------------------------------------------------
