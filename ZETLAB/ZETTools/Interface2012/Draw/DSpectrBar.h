//------------------------------------------------------------------------------
// DSpectrBar.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include "math.h"
#include <Interface2012\Draw\CustomDraw.h>
#include <Interface2012\Custom\FunctionForAll.h>
#include <Interface2012\Draw\FunctionsForGraph.h>
#include <string>


struct ParamSpectrBar : public ParamCustomDraw {
    CRect m_rect;
    std::vector<double> pVertexs;
    std::vector<double> pOutlineVertexs;

    ParamSpectrBar(){

    }
};

class CDSpectrBar : public CCustomDraw{
public:
    CDSpectrBar(CRect rect, bool *pFlagIsChange, double *pTimeAnimation);
    virtual ~CDSpectrBar();
    ParamSpectrBar m_param;

protected:

private:

public:
    //получить массив точек для отрисовки столбиков
    void SetBarVertex(const std::vector<double> &vertex);
    //получить массив точек для отрисовки каемки
    void SetOutlineVertex(const std::vector<double> &vertex);
    //отрисовка графика
    void DrawPlot();

	void Resize(CRect rect);
	virtual void Clear (void);
    //отрисовка всего элемента
    virtual void OnDraw();

};