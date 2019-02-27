//------------------------------------------------------------------------------
// DButtonHMME.h : файл заголовка (Draw Graph)
//------------------------------------------------------------------------------
// Базовый класс отображения графиков.
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Draw\CustomDraw.h"
//------------------------------------------------------------------------------
struct ParamCustomGraph : public ParamCustomDraw
{// Структура параметров отображения графиков
	// В родительской структуре - общие параметры, здесь - частные параметры
	int iBorderHeight;
	COLORREF clrBorderGradientColorStart;
	COLORREF clrBorderGradientColorEnd;
	ParamCustomGraph()
	{
		Size = sizeof(ParamCustomGraph);
		iBorderHeight = 3;
		clrBorderGradientColorStart = RGB(90, 90, 90);
		clrBorderGradientColorEnd = RGB(180, 180, 180);
	}
};
//------------------------------------------------------------------------------
class CDCustomGraph : public CCustomDraw
{
public:
	CDCustomGraph(CRect rect, PVOID ppGraphics,
		bool *pFlagIsChange, double *pTimeAnimation);
	virtual ~CDCustomGraph();

private:
	ParamCustomGraph m_param;// структура всех параметров отображения

protected:

public:
	// функции параметров отображения
	int GetBorderHeight() { return m_param.iBorderHeight; }
	void SetBorderHeight(int iValue);

	COLORREF GetBorderGradientColorStart() { return m_param.clrBorderGradientColorStart; }
	void SetBorderGradientColorStart(COLORREF clrValue);

	COLORREF GetBorderGradientColorEnd() { return m_param.clrBorderGradientColorEnd; }
	void SetBorderGradientColorEnd(COLORREF clrValue);
};
//------------------------------------------------------------------------------