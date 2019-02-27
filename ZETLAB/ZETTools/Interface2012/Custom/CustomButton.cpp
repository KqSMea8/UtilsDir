//------------------------------------------------------------------------------
// CustomButton.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\Custom\CustomButton.h"
#include "C:\ZETTools\Interface2012\Custom\Custom_ZETi12.h"
//------------------------------------------------------------------------------
CCustomButton::CCustomButton(CCustomContainer *owner) : CCustomElement(owner)
{
	m_FunctionEvent = NULL;
	m_hCursor = LoadCursorW(NULL, IDC_HAND);
}
//------------------------------------------------------------------------------
CCustomButton::~CCustomButton()
{
}
//------------------------------------------------------------------------------
void CCustomButton::OnLButtonClk(UINT nFlags, CPoint point)
{
	CCustomElement::OnLButtonClk(nFlags, point);

	CWnd *pParent = GetParent();
	if (m_pDClass->GetVisible() && m_pDClass->GetEnable() &&
		m_FunctionEvent && pParent)
		(((CCustom_ZETi12*)pParent)->*m_FunctionEvent)();
	if (GetButtonType() == RadioButton) m_pOwner->ResetRadioButtons(m_pDClass->GetName());
}

void CCustomButton::OnLButtonUp( UINT nFlags, CPoint point )
{
	if (BelongsMenu) m_pOwner->MessageToParent(IDM_PICK_ITEM, GetButtonID(), 0);
	if (!BelongsMenu && m_pOwner->OpenMenu == true)
			m_pOwner->CloseMenu();
	//CCustom_ZETi12::OnNewMenuItems();  
	CCustomAll::OnLButtonUp(nFlags, point);

}
//------------------------------------------------------------------------------