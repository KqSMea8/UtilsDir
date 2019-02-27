//------------------------------------------------------------------------------
// DOscillGrid.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include "math.h"
#include <Interface2012\Draw\CustomDraw.h>
#include <Interface2012\Custom\FunctionForAll.h>
#include <Interface2012\Draw\FunctionsForGraph.h>
#include <string>


struct ParamOscillPlot : public ParamCustomDraw {
    CRect m_rect;
    std::vector<double> pVertexs;
	std::vector<double> pBacklightVertexs;
	COLORREF plotColor;
	GLfloat plotWidth;
	GLint plotTransparency;

    ParamOscillPlot()
	{
		plotColor = RGB(1, 203, 247);
		plotWidth = 2.0;
		plotTransparency = 255;
    }

	void Clear()
	{
		if (!pVertexs.empty())
			pVertexs.clear();
	}
};

class CDOscillPlot : public CCustomDraw{
public:
    CDOscillPlot(CRect rect, bool *pFlagIsChange, double *pTimeAnimation);
    virtual ~CDOscillPlot();
    ParamOscillPlot m_param;

public:
    //получить массив точек для отрисовки графика
    void SetPlotVertex(const std::vector<double> &vertex);
    //прорисовка графика
    void DrawPlot();
	//
	void DrawBacklight();
    //прорисовка всего элемента
    virtual void OnDraw();
	//установка цвета графика
	void SetPlotColor(COLORREF color);
	//установка толщины графика
	void SetPlotWidth(float width);
	//установка прозрачности графика
	void SetPlotTransparency(float transparency);

	void Resize(CRect rect);
};