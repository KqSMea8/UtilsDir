//------------------------------------------------------------------------------
// MainCheckBox.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\ZetLab\CheckBox.h"
#include "C:\ZETTools\Interface2012\Custom\Custom_ZETi12.h"
//------------------------------------------------------------------------------
CZCheckBox::CZCheckBox(CCustomContainer *owner, CRect rect) : CCustomButton(owner)

{
	CString name;
	m_pDCheckBox = NULL;
	if (m_pOwner)
	{
		name = m_pOwner->GetFolderPNG() + L"\\CheckBox\\CheckBox";
		m_pDCheckBox = new CDButton(rect, m_pOwner->GetPointerToGraphics(),
			m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime() );
		m_pDCheckBox->SetImageFileName(name);
		m_pDCheckBox->SetCheckBox(true);
		m_pDClass = m_pDCheckBox;
	}
}
//------------------------------------------------------------------------------
CZCheckBox::~CZCheckBox()
{
	if (m_pDCheckBox)
		delete m_pDCheckBox;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------