//------------------------------------------------------------------------------
// CustomSlider.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\Custom\CustomSlider.h"
//------------------------------------------------------------------------------
CCustomSlider::CCustomSlider(CCustomContainer *owner) : CCustomElement(owner)
{
	m_FunctionEvent = NULL;
	m_hCursor = LoadCursorW(NULL, IDC_HAND);
}
//------------------------------------------------------------------------------
CCustomSlider::~CCustomSlider()
{
}
//------------------------------------------------------------------------------
void CCustomSlider::NewDouble(double newVal)
{
	CWnd *pParent = GetParent();
	if (m_FunctionEvent && pParent)
		(((CCustom_ZETi12*)pParent)->*m_FunctionEvent)(newVal);
}
//------------------------------------------------------------------------------