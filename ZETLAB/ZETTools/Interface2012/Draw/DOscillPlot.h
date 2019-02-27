//------------------------------------------------------------------------------
// DOscillGrid.h : ���� ���������
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
    //�������� ������ ����� ��� ��������� �������
    void SetPlotVertex(const std::vector<double> &vertex);
    //���������� �������
    void DrawPlot();
	//
	void DrawBacklight();
    //���������� ����� ��������
    virtual void OnDraw();
	//��������� ����� �������
	void SetPlotColor(COLORREF color);
	//��������� ������� �������
	void SetPlotWidth(float width);
	//��������� ������������ �������
	void SetPlotTransparency(float transparency);

	void Resize(CRect rect);
};