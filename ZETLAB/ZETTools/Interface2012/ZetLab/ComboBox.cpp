//------------------------------------------------------------------------------
// ComboBox.cpp : файл реализации
// ћаксимальное число элементов, использующих меню, равно 48, т.е.
// (0x8000 - 0x5000) / 0x100

//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\ZetLab\ComboBox.h"
//------------------------------------------------------------------------------
CZComboBox::CZComboBox(CCustomContainer *owner, CRect rect, CRect ownerRect) :
			CCustomItem(owner)
{// –егистраци€ меню (т.е. определение m_StartItemID) в конструкторе CCustomItem
	m_pDComboBox = NULL;
	if (m_pOwner)
	{
#ifdef _GDIPLUS 
		m_pDComboBox = new CDComboBox(rect, m_pOwner->GetPointerToGraphics(),
			m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime() );
#else
		m_pDComboBox = new CDComboBox(rect,
			m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime() );
		m_pDComboBox->SetParent(owner);
#endif
		m_pDClass = m_pDComboBox;
		m_pDComboBox->SetStartItemID(m_StartItemID);
		m_pDComboBox->SetSpaceSize(ownerRect);
		m_pPress = false;
		m_rect = rect;
	}
}
//------------------------------------------------------------------------------
void CZComboBox::Clear (void)
{
	m_pDComboBox->Clear();
}
//------------------------------------------------------------------------------
CZComboBox::~CZComboBox()
{
	if (m_pDComboBox)
		delete m_pDComboBox;
}
//------------------------------------------------------------------------------
void CZComboBox::OnPickItem(long index)
{// оператор выбрал строку меню с индексом index
	//CCustomItem::OnPickItem(index);

	
	if (index != m_pDComboBox->_pNum && index != m_pDComboBox->_pNum + 1)
	{
		if (m_pDComboBox->GetIndexCur() != index)
		{
			m_pDComboBox->SetIndexCur(index);
			NewLong(index);
		}
			m_pDComboBox->SetMenuButtonsState(false);
			m_pOwner->CloseMenu();
	}
	else if (index == m_pDComboBox->_pNum)
	{
		m_pDComboBox->ComboboxUpButton();
	}
	else if (index == m_pDComboBox->_pNum + 1)
	{
		m_pDComboBox->ComboboxDownButton();
	}
}
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
void CZComboBox::OnLButtonClk(UINT nFlags, CPoint point)
{// отображение меню
	CPoint p;
	CCustomItem::OnLButtonClk(nFlags, point);
	p.x = m_pDComboBox->GetRect().left;
	p.y = m_pDComboBox->GetRect().bottom;
	ClientToScreen(GetParent()->m_hWnd, &p);

	m_pDComboBox->GetMenu()->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON,
		p.x, p.y, GetParent() );
}
//------------------------------------------------------------------------------
#else
void CZComboBox::OnLButtonClk(UINT nFlags, CPoint point)
{// отображение меню
	//if (BelongsToCB == false && m_pOwner->OpenCombobox == true) m_pOwner->CloseCombobox(); //закрываем открытые комбобоксы, если такие есть
	
	if (!m_Press)
	{
		if (m_pOwner->OpenMenu)
			m_pOwner->CloseMenu();
		m_pOwner->SetParamAddRect(CRect(m_pDComboBox->GetButRect().left,
										m_pDComboBox->GetButRect().bottom,
										m_pDComboBox->GetButRect().right,
										m_pDComboBox->GetButRect().bottom + m_pDComboBox->GetHeightCB()));
		m_pDComboBox->SetMenuButtonsState(true);
		m_pOwner->OpenMenu = true;
		m_Press = true;
	}
	else
	{
		m_Press = false;
		m_pOwner->CloseMenu();
		m_pOwner->SetParamAddRect(CRect(0,0,0,0));
	}
	
}
//------------------------------------------------------------------------------
#endif
void CZComboBox::OnMouseWheel(UINT nFlags, CPoint point, short delta)
{
	long indOld = m_pDComboBox->GetIndexCur();
	CCustomItem::OnMouseWheel(nFlags, point, delta);
	if ( indOld != m_pDComboBox->GetIndexCur() )
		NewLong( m_pDComboBox->GetIndexCur() );
}
