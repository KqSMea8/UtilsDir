#include "stdafx.h"
#include "DSelector.h"


//------------------------------------------------------------------------------
int g_nSelector = 0;		// счётчик стрелочных индикаторов
//------------------------------------------------------------------------------
DSelector::DSelector(CRect rect, PVOID ppGraphics, bool *pFlagIsChange, double *pTimeAnimation) : CCustomDraw(rect, ppGraphics, pFlagIsChange, pTimeAnimation)
{

	Selector::SetRect(rect.left, rect.top, rect.Width(), rect.Height());
	Selector::SetCurrentValue(Selector::GetCurrentValue());
	//Selector::SetSelectedDigitColor(Color(0xff, 0, 0, 0));

// 	activeSelectorColor = Selector::GetSelectorColor();
// 	inactiveSelectorColor = Darker(activeSelectorColor.ToCOLORREF(), 10);
// 	Selector::SetSelectorColor(inactiveSelectorColor);
// 
// 	activeTextColor = Selector::GetTextColor();
// 	inactiveTextColor = Darker(activeTextColor.ToCOLORREF(), 10);
// 	Selector::SetTextColor(inactiveTextColor);

	m_backLightGp = NULL;
	pPgb = NULL;

	initLight();

	m_pParamDraw = &m_param;	// обеспечение ОБЩИХ параметров
	// изменение значений общих параметров, заданных по умолчанию
	m_param.nElement = ++g_nSelector;
	SetNameWithNumber(L"Selector");

	m_param.bMouseChange = true;
	m_bEvent = false;
}


DSelector::~DSelector()
{
	if(m_backLightGp != NULL)
	{
		DeleteObject(m_backLightGp);
		delete m_backLightGp;
	}

	if(pPgb != NULL)
	{
		DeleteObject(pPgb);
		delete pPgb;
	}
}


void DSelector::OnDraw()
{
	Graphics *pGr = *(Graphics**)m_ppGraphics;

	if(Selector::GetBackColor() == NULL)
	{
		Selector::SetBackColor(Color(0, 255, 255, 255));
	}

	if(Selector::GetRect().Width != m_rect.Width() || Selector::GetRect().Height != m_rect.Height())
	{
		Selector::SetRect(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height());

		initLight();
	}
	
	//Катя сказала, что не надо
// 	if(m_param.bMouseCome)
// 		pPgb->SetCenterColor(Selector::GetSelectedDigitColor());
// 	else
// 		pPgb->SetCenterColor(Color(0,0,0));	// небольшой эффект вдавленности
	
	if(m_param.bMouseCome)
		pGr->FillPath(pPgb, m_backLightGp);

	Selector::DrawObject(*pGr);
}


void DSelector::initLight()
{
	if(m_backLightGp == NULL)
		m_backLightGp = new GraphicsPath();
	else
		m_backLightGp->Reset();

	if(pPgb != NULL)
		delete pPgb;

	CRect rc(m_rect.left - 3, m_rect.top - 2, m_rect.right + 3, m_rect.bottom + 3);
	RoundRectInGraphicsPath(m_backLightGp, &rc, 4);

	pPgb = new PathGradientBrush(m_backLightGp);

	pPgb->SetCenterColor(Selector::GetSelectedDigitColor());

	Color colors[] = {Color(0, 180, 180, 180)};
	int count = 1;
	pPgb->SetSurroundColors(colors, &count);
	pPgb->SetFocusScales(0.8f, 0.65f);
}


// Color DSelector::Darker(long color, byte procent)
// {
// 	return 
// 	Color(
// 		max((color & 255) - (color & 255) * procent / 100, 0), 
// 		max(((color >> 8) & 255) - ((color >> 8) & 255) * procent / 100, 0), 
// 		max(((color >> 16) & 255) - ((color >> 16) & 255) * procent / 100, 0)
// 		);
// }


bool DSelector::SetStruct(PVOID add)
{
	bool ret = CCustomDraw::SetStruct(add);

	if(!ret)
		return false;

	SetSelectorColor(m_param.selectorColor);
	SetTextColor(m_param.textColor);
	SetSelectedDigitColor(m_param.selectedDigitColor);
	SetMaxValue(m_param.maxValue);
	SetMinValue(m_param.minValue);
	SetCurrentValue(m_param.currentValue);
	SetCountVisibleDigits(m_param.countVisibleDigits);
	SetCountVisibleDivDigits(m_param.countVisibleDivDigits);
	SetCurrentDigit(m_param.currentDigit);
}

bool DSelector::GetStruct(PVOID add)
{
	m_param.currentDigit = Selector::GetCurrentDigit();
	m_param.selectorColor = Selector::GetSelectorColor();	
	m_param.textColor = Selector::GetTextColor();		
	m_param.selectedDigitColor = Selector::GetSelectedDigitColor();			
	m_param.maxValue = Selector::GetMaxValue();			
	m_param.minValue = Selector::GetMinValue();			
	m_param.currentValue = Selector::GetCurrentValue();		
	m_param.countVisibleDigits = Selector::GetCountVisibleDigits();	
	m_param.countVisibleDivDigits = Selector::GetCountVisibleDivDigits();

	return CCustomDraw::SetStruct(add);
}


void DSelector::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_param.bMouseCome)
	{
		//CCustomDraw::GetFocus();

		short oldCurrDigit = Selector::GetCurrentDigit();

		Selector::OnLButtonDown(point);

		if(oldCurrDigit != Selector::GetCurrentDigit())
			CCustomDraw::IsChange();
	}
}


void DSelector::OnMouseWhell(UINT nflags, CPoint point, short delta)
{
	if(m_param.bMouseCome && m_param.bFocus)
	{
		float oldCurrVal = Selector::GetCurrentValue();

		Selector::OnMouseWheel(delta, point);

		if(oldCurrVal != Selector::GetCurrentValue())
		{
			CCustomDraw::IsChange();
			m_bEvent = true;
		}
	}
}

void DSelector::OnChar(UINT nChar, LPARAM nFlags)
{
	if(m_param.bFocus)
	{
		float oldCurrVal = Selector::GetCurrentValue();

		Selector::OnKeyUp(nChar);

		if(oldCurrVal != Selector::GetCurrentValue())
		{
			CCustomDraw::IsChange();
			m_bEvent = true;
		}
	}
}





void DSelector::SetSelectorColor(const Color &selectorColor)
{
	if(/*activeSelectorColor.GetValue() */ selectorColor.GetValue() != Selector::GetSelectorColor().GetValue())
	{
// 		activeSelectorColor = selectorColor;
// 		inactiveSelectorColor = Darker(activeSelectorColor.ToCOLORREF(), 10);
// 
// 		if(m_param.bFocus)
// 			Selector::SetSelectorColor(activeSelectorColor);
// 		else
// 			Selector::SetSelectorColor(inactiveSelectorColor);

		Selector::SetSelectorColor(selectorColor);

		CCustomDraw::IsChange();
	}
}


void DSelector::SetTextColor(const Color &textColor)
{
	if(/*activeTextColor.GetValue()*/textColor.GetValue() != Selector::GetTextColor().GetValue())
	{
// 		activeTextColor = textColor;
// 		inactiveTextColor = Darker(activeTextColor.ToCOLORREF(), 10);
// 
// 		if(m_param.bFocus)
// 			Selector::SetTextColor(activeTextColor);
// 		else
// 			Selector::SetTextColor(inactiveTextColor);

		Selector::SetTextColor(textColor);

		CCustomDraw::IsChange();
	}
}


void DSelector::SetSelectedDigitColor(const Color &selectedDigitColor)
{
	if(selectedDigitColor.GetValue() != Selector::GetSelectedDigitColor().GetValue())
	{
		Selector::SetSelectedDigitColor(selectedDigitColor);
		CCustomDraw::IsChange();
	}
}


void DSelector::SetMaxValue(const float &maxVal)
{
	if(maxVal != Selector::GetMaxValue())
	{
		Selector::SetMaxValue(maxVal);
		CCustomDraw::IsChange();
	}
}

void DSelector::SetMinValue(const float &minVal)
{
	if(minVal != Selector::GetMinValue())
	{
		Selector::SetMinValue(minVal);
		CCustomDraw::IsChange();
	}
}

void DSelector::SetCurrentValue(const float &currVal)
{
	if(currVal != Selector::GetCurrentValue())
	{
		Selector::SetCurrentValue(currVal);
		CCustomDraw::IsChange();
	}
}

void DSelector::SetCountVisibleDigits(const int &numDig)
{
	if(numDig != Selector::GetCountVisibleDigits())
	{
		Selector::SetCountVisibleDigits(numDig);
		CCustomDraw::IsChange();
	}
}

void DSelector::SetCountVisibleDivDigits(const int &numDig)
{
	if(numDig != Selector::GetCountVisibleDivDigits())
	{
		Selector::SetCountVisibleDivDigits(numDig);
		CCustomDraw::IsChange();
	}
}

void DSelector::SetCurrentDigit(const short &currDigit)
{
	if(currDigit != Selector::GetCurrentDigit())
	{
		Selector::SetCurrentDigit(currDigit);
		CCustomDraw::IsChange();
	}
}


void DSelector::KillFocus()
{
	CCustomDraw::KillFocus();

	SetCurrentDigit((short)-1);
}



bool DSelector::GetNeedEvent()
{
	bool ret = m_bEvent;
	m_bEvent = false;
	return ret;
}


// void DSelector::OnMouseEnter()
// {
// 	Selector::SetSelectorColor(activeSelectorColor);
// 	Selector::SetTextColor(activeTextColor);
// 
// 	CCustomDraw::OnMouseEnter();
// }

// void DSelector::OnMouseLeave()
// {
// 	Selector::SetSelectorColor(inactiveSelectorColor);
// 	Selector::SetTextColor(inactiveTextColor);
// 
// 	CCustomDraw::OnMouseLeave();
// }
