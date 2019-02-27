//------------------------------------------------------------------------------
// CustomItem.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\Custom\CustomItem.h"
//------------------------------------------------------------------------------
CCustomItem::CCustomItem(CCustomContainer *owner) : CCustomElement(owner)
{
	m_FunctionEvent = NULL;
	m_hCursor = LoadCursorW(NULL, IDC_HAND);
	CWnd *pParent = GetParent();
	if (pParent)
		((CCustom_ZETi12*)pParent)->RegistrationMenu(this);
}
//------------------------------------------------------------------------------
CCustomItem::~CCustomItem()
{
}
//------------------------------------------------------------------------------
void CCustomItem::NewLong(long val)
{
	CWnd *pParent = GetParent();
	if (m_pDClass->GetVisible() && m_pDClass->GetEnable() &&
		m_FunctionEvent && pParent)
			(((CCustom_ZETi12*)pParent)->*m_FunctionEvent)(val);
}
//------------------------------------------------------------------------------