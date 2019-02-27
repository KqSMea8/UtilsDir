//------------------------------------------------------------------------------
// ButtonHMME.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include <Interface2012\ZetLab\ButtonHMME.h>
#include <Dialog_ZET\Dialog_ZET.h>
//------------------------------------------------------------------------------
CButtonHMME::CButtonHMME(
	CCustomContainer *owner, CRect rect, TypeButtonHMME codeButton)
	: CCustomElement(owner)
{
	CString name(L"");
	m_pDButton = NULL;
	m_hCursor = LoadCursorW(NULL, IDC_HAND);
	if (m_pOwner)
	{
#ifdef _ACTIVEX_ZIS
		name.Empty();
#else
		name = L"$(ZetToolsDir)Interface2012\\Textures\\ButtonHMME\\Button";
#endif
		switch(codeButton)
		{
		case tbHMME_Help :
			name += L"Help";
			m_command = VK_F1;
			break;
		case tbHMME_Min :
			name += L"Min";
			m_command = SC_MINIMIZE;
			break;
		case tbHMME_Max :
			name += L"Max";
			m_command = SC_MAXIMIZE;
			break;
		case tbHMME_End :
			name += L"End";
			m_command = SC_CLOSE;
			break;
		default: name.Empty();
		}
#ifdef _GDIPLUS
		m_pDButton = new CDButtonHMME(rect, m_pOwner->GetPointerToGraphics(),
			m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime() );
#else
		m_pDButton = new CDButtonHMME(rect,	m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime(), codeButton);
#endif
		m_pDButton->SetImageFileName(name);
		m_pDClass = m_pDButton;
	}
}
//------------------------------------------------------------------------------
void CButtonHMME::Clear (void)
{
	m_pDButton->Clear();
}
//------------------------------------------------------------------------------
CButtonHMME::~CButtonHMME()
{
	if (m_pDButton)
	{
		delete m_pDButton;
		m_pDButton = nullptr;
	}
}
//------------------------------------------------------------------------------
void CButtonHMME::Resize(CRect rect)
{
	m_pDButton->Resize(rect);
}
//------------------------------------------------------------------------------
void CButtonHMME::OnLButtonClk(UINT nFlags, CPoint point)
{
	m_pDButton->OnLButtonClk(nFlags, point);
	if (m_pOwner)
	{
		if (m_command != VK_F1)
			m_pOwner->MessageToParent(WM_SYSCOMMAND, m_command, 0);
		else{
#ifdef _ACTIVEX2012
#else
			((CDialog_ZET*)m_pOwner->GetParent())->OnHelp();
#endif
        }
	}
}