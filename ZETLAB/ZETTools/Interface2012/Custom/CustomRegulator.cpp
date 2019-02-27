//------------------------------------------------------------------------------
// CustomRegulator.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\Custom\CustomRegulator.h"
//------------------------------------------------------------------------------
CCustomRegulator::CCustomRegulator(CCustomContainer *owner) : CCustomElement(owner)
{
	m_FunctionEvent = NULL;
	m_hCursor = LoadCursorW(NULL, IDC_HAND);
}
//------------------------------------------------------------------------------
CCustomRegulator::~CCustomRegulator()
{
}
//------------------------------------------------------------------------------
void CCustomRegulator::NewDouble(double newVal)
{
	CWnd *pParent = GetParent();
	if (m_FunctionEvent && pParent)
		(((CCustom_ZETi12*)pParent)->*m_FunctionEvent)(newVal);
}
//------------------------------------------------------------------------------