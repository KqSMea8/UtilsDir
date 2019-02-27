///------------------------------------------------------------------------------
// Button.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\ZetLab\Button.h"
IMPLEMENT_DYNAMIC(CZButton, CObject)
//------------------------------------------------------------------------------
CZButton::CZButton(CCustomContainer *owner, CRect rect) : CCustomButton(owner)
{
	m_pDButton = nullptr;
	m_pDClass = nullptr;
	if (m_pOwner)
	{		
#ifdef _GDIPLUS
		CString name;
		name = m_pOwner->GetFolderPNG() + L"Button\\Button";
		m_pDButton = new CDButton(rect, m_pOwner->GetPointerToGraphics(),
			m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime() );
		m_pDButton->SetImageFileName(name);
#else
		m_pDButton = new CDButton(rect,	m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime() );
#endif		
		m_pDClass = m_pDButton;
	}	
}
//------------------------------------------------------------------------------
CZButton::CZButton(CCustomContainer *owner, CRect rect, CString ImageFile, int NumberTexture) : CCustomButton(owner)
{
	m_pDButton = nullptr;
	m_pDClass = nullptr;
	if (m_pOwner)
	{
#ifdef _GDIPLUS
		CString name;
		name = m_pOwner->GetFolderPNG() + L"Button\\Button";
		m_pDButton = new CDButton(rect, m_pOwner->GetPointerToGraphics(),
			m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime());
		m_pDButton->SetImageFileName(name);
#else
		m_pDButton = new CDButton(rect, m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime(), ImageFile, NumberTexture);
#endif		
		m_pDClass = m_pDButton;
	}
}
//------------------------------------------------------------------------------
void CZButton::Clear (void)
{
	m_pDButton->Clear();	
}
//------------------------------------------------------------------------------
CZButton::~CZButton()
{
	if (m_pDButton)
		delete m_pDButton;
}
//------------------------------------------------------------------------------
void CZButton::SetFixation()
{
	if (m_pDButton->GetButtonType() == RadioButton) m_pDButton->SetButtonState(GetElementState());	
	if (GetBelongsMenu()) m_pDButton->DSetBelongsMenu(true);
}
//------------------------------------------------------------------------------