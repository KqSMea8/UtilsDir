#include "stdafx.h"
#include "CSelector.h"
//#include "ModalInput.h"


CSelector::CSelector(CCustomContainer *owner, CRect rect) : CCustomSlider(owner)
{
	m_pDSelector = NULL;

	if (m_pOwner)
	{
		m_pDSelector = new DSelector(rect, m_pOwner->GetPointerToGraphics(),
			m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime() );

		if(m_pDSelector != NULL)
			m_pDClass = m_pDSelector;
	}

	ASSERT(m_pDSelector);

	m_crHand = LoadCursor(NULL, IDC_HAND);
	m_crArrow = LoadCursor(NULL, IDC_ARROW);
}


CSelector::~CSelector()
{
	if(m_pDSelector != NULL)
		delete m_pDSelector;
}


void CSelector::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_pDSelector->GetEnable())
		if(m_pDSelector->GetFocusStatus())
		{	
			short pos = m_pDSelector->GetPosFromPoint(point);
			if(pos != m_pDSelector->GetCurrentDigit())
				m_pDSelector->SetCurrentDigit(pos);

			if(pos > -1)
			{
				if(GetCursor() != m_crHand)
					SetClassLong(GetParent()->m_hWnd, GCL_HCURSOR, (long)m_crHand);
			}
			else
				SetClassLong(GetParent()->m_hWnd, GCL_HCURSOR, (long)m_crArrow);
		}
		else 
			if(GetCursor() != m_crHand)
				SetClassLong(GetParent()->m_hWnd, GCL_HCURSOR, (long)m_crHand);

	CCustomElement::OnMouseMove(nFlags, point);
}



void CSelector::OnMouseWhell(UINT nflags, CPoint point, short delta)
{
	m_pDSelector->OnMouseWhell(nflags, point, delta);
	
	if ( m_pDSelector->GetNeedEvent() )
		NewValue( GetCurrentValue() );
}



void CSelector::OnChar(UINT nChar, LPARAM nFlags)
{
	m_pDSelector->OnChar(nChar, nFlags);

	if ( m_pDSelector->GetNeedEvent() )
		NewValue( GetCurrentValue() );
}



void CSelector::OnLButtonDblClk(UINT nFlags, CPoint point)
{
// 	if(m_pDSelector->GetEnable())
// 	{
// 		RunModalInput(point);
// 	}
}


int CSelector::RunModalInput(CPoint point)
{
//Вариант1
// 	GetWindowRect(GetParent()->m_hWnd, &MyRect);
// 
// 	CRect wndRect(100, 100, 200, 200);
// 	CRect editRect(3,2,52,12);
// 
// 	m_pCWnd = new CWnd();
// 	m_pCWnd->Create(NULL, _T(""),  WS_BORDER | WS_SYSMENU, wndRect, GetParent(), 1001); 
// 
// 	m_pCEdit = new CEdit();
// 	m_pCEdit->Create(ES_CENTER | ES_AUTOHSCROLL, editRect, m_pCWnd, 1002);
// 
// 	m_pCWnd->ShowWindow(TRUE);



//Вариант2
// 	INT_PTR rt;
// 	CString stroka;
// 	CString frm;
// 	RECT MyRect;
// 	ModalInput *mydig;

// 	mydig = new ModalInput(GetParent());
// 	mydig->Create(ModalInput::IDD, GetParent());

// 	frm.Format(_T("%%%d.%df"), m_pDSelector->GetCountVisibleDigits(), m_pDSelector->GetCountVisibleDivDigits());
// 	stroka.Format(frm, m_pDSelector->GetCurrentValue());
// 	mydig->m_TextOut = stroka;
// 	mydig->m_point.x = point.x + MyRect.left;
// 	mydig->m_point.y = point.y + MyRect.top;
// 	rt = mydig->DoModal();
// 	if(rt == IDOK)
// 	{
// 		m_pDSelector->SetCurrentValue(_ttof(mydig->m_TextOut));
// 	}

	return 0;
}



void CSelector::SetSelectorColor(const Color &selectorColor)
{
	m_pDSelector->SetSelectorColor(selectorColor);
}

void CSelector::SetTextColor(const Color &textColor)
{
	m_pDSelector->SetTextColor(textColor);
}

void CSelector::SetSelectedDigitColor(const Color &selectedDigitColor)
{
	m_pDSelector->SetSelectedDigitColor(selectedDigitColor);
}

void CSelector::SetMaxValue(const float &maxVal)
{
	m_pDSelector->SetMaxValue(maxVal);
}

void CSelector::SetMinValue(const float &minVal)
{
	m_pDSelector->SetMinValue(minVal);
}

void CSelector::SetCurrentValue(const float &currVal)
{
	m_pDSelector->SetCurrentValue(currVal);
}

void CSelector::SetCountVisibleDigits(const int &numDig)
{
	m_pDSelector->SetCountVisibleDigits(numDig);
}

void CSelector::SetCountVisibleDivDigits(const int &numDig)
{
	m_pDSelector->SetCountVisibleDivDigits(numDig);
}

void CSelector::SetCurrentDigit(const short &currDigit)
{
	m_pDSelector->SetCurrentDigit(currDigit);
}




Color CSelector::GetSelectorColor()
{
	return m_pDSelector->GetSelectorColor();
}

Color CSelector::GetTextColor()
{
	return m_pDSelector->GetTextColor();
}

Color CSelector::GetSelectedDigitColor()
{
	return m_pDSelector->GetSelectedDigitColor();
}

float CSelector::GetMaxValue()
{
	return m_pDSelector->GetMaxValue();
}

float CSelector::GetMinValue()
{
	return m_pDSelector->GetMinValue();
}

float CSelector::GetCurrentValue()
{
	return m_pDSelector->GetCurrentValue();
}

int CSelector::GetCountVisibleDigits()
{
	return m_pDSelector->GetCountVisibleDigits();
}

int CSelector::GetCountVisibleDivDigits()
{
	return m_pDSelector->GetCountVisibleDivDigits();
}

short CSelector::GetCurrentDigit()
{
	return m_pDSelector->GetCurrentDigit();
}
