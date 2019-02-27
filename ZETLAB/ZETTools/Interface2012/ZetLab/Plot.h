//------------------------------------------------------------------------------
// Plot.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Custom\CustomPlot.h"
#include "C:\ZETTools\Interface2012\Draw\DPlot.h"
//------------------------------------------------------------------------------


struct Plot : public PlotParam
{
    Plot()
    {
        m_axisX.paScaleType = stLinear;
        m_axisX.paBegin = -1000;
        m_axisX.paEnd = 1000;
        m_axisX.paGridType = gtHorizontal;
        m_axisX.paMaxCurrent = 100;
        m_axisX.paMinCurrent = 0;
        m_axisX.paFitType = ftCenter;

        m_axisY.paScaleType = stLinear;
        m_axisY.paBegin = -100;
        m_axisY.paEnd = 100;
        m_axisY.paGridType = gtVertical;
        m_axisY.paMaxCurrent = 10;
        m_axisY.paMinCurrent = -10;
        m_axisY.paFitType = ftCenter;

        m_startValue = 0;
        m_endValue = 100;
        m_firstPoint = 0;
        m_lastPoint = m_firstPoint + MAX_QNT_OF_POINTS; //с какой и по какую точку рисовать
        m_step = 0.1;
        m_qntPoints = (m_endValue - m_startValue)/m_step;
    }
};

class CPlot : public CCustomPlot
{
public:
    CPlot(CCustomContainer *owner, CRect rect);
    virtual ~CPlot();

private:
    CDPlot *m_pDPlot;
    Plot m_param;
protected:

public:
    void SetAxis(double minX, double maxX, double minY, double maxY);
    void SetBounds(double minX, double maxX, double minY, double maxY);
    void SetShift(double ShiftX, double ShiftY);
    void SetScale(double ScaleX, double ScaleY);
    void SetScaleType(ScaleType scaleTypeX, ScaleType scaleTypeY);
    void SetData(std::vector<double> DataArrayX, std::vector<double> DataArrayY, int ArraySize, int Quality);
    void SetData(std::vector<double> DataArrayY, int ArraySize, int Quality);
    int SetSpace(double minX, double maxX, double step);
    double SetSpace(double minX, double maxX, long Qnt);
    void SetStep(double stepX);
    void SetBorders(double border);
    void SetFit(FitType fitTypeX, FitType fitTypeY);
    void Shift(zRect rectType);
    void Scale(zRect rectType);
    //
    virtual void OnLButtonDown(UINT nFlags, CPoint point);
    virtual void SaveParameters(SimpleXML *pSimpleXML);
};
//------------------------------------------------------------------------------