#pragma once
#include "C:\ZETTools\Interface2012\Draw\CustomDraw.h"
#include "C:\ZETTools\Interface2012\Draw\Selector\Selector.h"


struct ParamSelector : public ParamCustomDraw
{// ��������� ���������� ����������� 
	// � ������������ ��������� - ����� ���������, ����� - ������� ���������

	short currentDigit;
	Color selectorColor;			//���� ���������
	Color textColor;				//���� ������
	Color selectedDigitColor;		//���� ������ ����������� �����
	float maxValue;				//������������ ��������
	float minValue;				//����������� ��������
	float currentValue;			//������� ��������
	int countVisibleDigits;		//���������� ������� ������
	int countVisibleDivDigits;	//���������� ������� ������

	ParamSelector() : ParamCustomDraw()
	{
		Size = sizeof(ParamSelector);
	}
};

class DSelector : public CCustomDraw, public Selector
{
public:

	DSelector(CRect rect, PVOID ppGraphics,
		bool *pFlagIsChange, double *pTimeAnimation);
	virtual ~DSelector();

	// ����������� �������, ���������������� � ������
	virtual void OnDraw();
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnMouseWhell(UINT nflags, CPoint point, short delta);
	virtual void OnChar(UINT nChar, LPARAM nFlags);
	virtual void KillFocus();

// 	virtual void OnMouseEnter();
// 	virtual void OnMouseLeave();

private:

	ParamSelector m_param; // ��������� ���� ���������� �����������

	GraphicsPath *m_backLightGp;
	PathGradientBrush *pPgb;
	void initLight();

	bool m_bEvent;

//������������, ����� �������� ������� ������� ��� ��������� �����
// 	Color activeSelectorColor;
// 	Color inactiveSelectorColor;
// 
// 	Color activeTextColor;
// 	Color inactiveTextColor;
// 
// 	Color Darker(long color, byte procent);

public: //GetSetMethods------------------------------------------------------------------

	bool SetStruct(PVOID add);	// ����������� ���������� �� add � ���������
	bool GetStruct(PVOID add);	// ����������� ���������� �� ��������� � add

	//void SetRect(const int &x, const int &y, const int &w, const int &h);
	void SetSelectorColor(const Color &selectorColor);
	void SetTextColor(const Color &textColor);
	void SetSelectedDigitColor(const Color &selectedDigitColor);
	void SetMaxValue(const float &maxVal);
	void SetMinValue(const float &minVal);
	void SetCurrentValue(const float &currVal);
	void SetCountVisibleDigits(const int &numDig);
	void SetCountVisibleDivDigits(const int &numDig);
	void SetCurrentDigit(const short &currDigit);


	// ������� ������
	bool GetNeedEvent();

};