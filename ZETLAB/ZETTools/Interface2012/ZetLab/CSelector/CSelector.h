#pragma once
#include "C:\ZETTools\Interface2012\Custom\CustomSlider.h"
#include "C:\ZETTools\Interface2012\Draw\Selector\DSelector.h"
//------------------------------------------------------------------------------


class CSelector : public CCustomSlider
{
public:
	CSelector(CCustomContainer *owner, CRect rect);
	virtual ~CSelector();

 	virtual void OnMouseMove(UINT nFlags, CPoint point);
 	virtual void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual void OnMouseWhell(UINT nflags, CPoint point, short delta);
	virtual void OnChar(UINT nChar, LPARAM nFlags);

private:

	HCURSOR m_crHand;
	HCURSOR m_crArrow;

	DSelector *m_pDSelector;

	CWnd *m_pCWnd;
	CEdit *m_pCEdit;

	int RunModalInput(CPoint point);

public: //GetSetMethods------------------------------------------------------------------
	

	void SetSelectorColor(const Color &selectorColor);
	void SetTextColor(const Color &textColor);
	void SetSelectedDigitColor(const Color &selectedDigitColor);
	void SetMaxValue(const float &maxVal);
	void SetMinValue(const float &minVal);
	void SetCurrentValue(const float &currVal);
	void SetCountVisibleDigits(const int &numDig);
	void SetCountVisibleDivDigits(const int &numDig);
	void SetCurrentDigit(const short &currDigit);


	Color GetSelectorColor();
	Color GetTextColor();
	Color GetSelectedDigitColor();
	float GetMaxValue();
	float GetMinValue();
	float GetCurrentValue();
	int GetCountVisibleDigits();
	int GetCountVisibleDivDigits();
	short GetCurrentDigit();

};