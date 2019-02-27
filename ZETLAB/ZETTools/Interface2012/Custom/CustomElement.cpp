//------------------------------------------------------------------------------
// CustomGDI_ZET.cpp : ���� ����������
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include <Interface2012\Custom\CustomElement.h>
#include <Interface2012\Custom\Dialog_ZETi12.h>
//------------------------------------------------------------------------------
CCustomElement::CCustomElement(CCustomContainer *owner) : CCustomAll()
{
	m_pCOwner = owner;
	m_pOwner = owner;
	if (m_pOwner)
	{
		m_pOwner->AddClass(this);
		m_hCursor = m_pOwner->GetHCursor();
	}
	else
		m_hCursor = 0;
}
//------------------------------------------------------------------------------
CCustomElement::~CCustomElement()
{
}
//------------------------------------------------------------------------------
void CCustomElement::RegistrationFocus()
{
	if (m_IndexTabFocus >= 0)
	{
		CWnd *pParent = GetParent();
#ifdef _ACTIVEX2012
#else
		if (pParent)
			((CDialog_ZETi12*)pParent)->RegistrationFocus(this);
#endif
	}
}
//------------------------------------------------------------------------------
void CCustomElement::OnMouseMove(UINT nFlags, CPoint point)
{// ���� ������. ���� ���������� ��� � �������.
	CCustomAll *pTemp;
	CCustomAll::OnMouseMove(nFlags, point);		// ����������� ToolTip
	CWnd *pParent = GetParent();
	if ( !m_pDClass->GetMouseStatus() )
	{// ������ ����� �� ������
		if (m_pOwner)
		{
			pTemp = m_pOwner->WhoOwnerMouse();	// ���������� ����
			if (pTemp)
				pTemp->OnMouseLeave();
		}
		m_pDClass->OnMouseEnter();				// ������ ���� ���
		if (pParent)
		{
#ifdef _ACTIVEX2012
#else
			((CDialog_ZETi12*)pParent)->RegistrationOwnerMouse(this);
#endif
			if ( m_pDClass->GetVisible() && m_pDClass->GetEnable() )
			{
				SetClassLongW(pParent->m_hWnd, GCL_HCURSOR, (LONG)m_hCursor);
				//TRACE("GET MOUSE %s\n", (CStringA)m_pDClass->GetName());
			}
		}
	}
//	else	// � ������ ���� ���� � ����, ������ ������ �� ����
}
//------------------------------------------------------------------------------
void CCustomElement::OnLButtonClk(UINT nFlags, CPoint point)
{
	if (!BelongsMenu)
		m_pOwner->CloseMenu();
	TRACE("REGISTR CLICK %s\n", (CStringA)m_pDClass->GetName());
	RegistrationFocus();
	CCustomAll::OnLButtonClk(nFlags, point);
}
//------------------------------------------------------------------------------
void CCustomElement::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CCustomAll::OnLButtonDblClk(nFlags, point);
	RegistrationFocus();
}
//------------------------------------------------------------------------------
void CCustomElement::OnLButtonUp( UINT nFlags, CPoint point )
{ 
	CCustomAll::OnLButtonUp(nFlags, point);
}

//------------------------------------------------------------------------------