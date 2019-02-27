#pragma once
#include "C:\ZETTools\Interface2012\Draw\CustomDraw.h"
#include "C:\ZETTools\Interface2012\Draw\Selector\Selector.h"


struct ParamSelector : public ParamCustomDraw
{// Структура параметров отображения 
	// В родительской структуре - общие параметры, здесь - частные параметры

	short currentDigit;
	Color selectorColor;			//цвет селектора
	Color textColor;				//цвет текста
	Color selectedDigitColor;		//цвет вокруг выделенного числа
	float maxValue;				//максимальное значение
	float minValue;				//минимальное значение
	float currentValue;			//текущее значение
	int countVisibleDigits;		//количество видимых знаков
	int countVisibleDivDigits;	//количество дробных знаков

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

	// виртуальные функции, переопределяемые в классе
	virtual void OnDraw();
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnMouseWhell(UINT nflags, CPoint point, short delta);
	virtual void OnChar(UINT nChar, LPARAM nFlags);
	virtual void KillFocus();

// 	virtual void OnMouseEnter();
// 	virtual void OnMouseLeave();

private:

	ParamSelector m_param; // структура всех параметров отображения

	GraphicsPath *m_backLightGp;
	PathGradientBrush *pPgb;
	void initLight();

	bool m_bEvent;

//раскоментить, чтобы селектор изменял яркость при наведении мышью
// 	Color activeSelectorColor;
// 	Color inactiveSelectorColor;
// 
// 	Color activeTextColor;
// 	Color inactiveTextColor;
// 
// 	Color Darker(long color, byte procent);

public: //GetSetMethods------------------------------------------------------------------

	bool SetStruct(PVOID add);	// копирование параметров из add в структуру
	bool GetStruct(PVOID add);	// копирование параметров из структуры в add

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


	// функции класса
	bool GetNeedEvent();

};