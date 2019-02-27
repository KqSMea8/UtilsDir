//------------------------------------------------------------------------------
// TrendPlot.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Custom\CustomPlot.h"
#include "C:\ZETTools\Interface2012\Draw\DOscillPlot.h"
//------------------------------------------------------------------------------

struct TrendPlot : public PlotParam 
{
    TrendPlot()
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
        m_axisY.paMinCurrentValue = 0;
        m_axisY.paMaxCurrentValue = 0;

        m_startValue = 0;
        m_endValue = 100;
        m_firstPoint = 0;
        m_lastPoint = m_firstPoint + MAX_QNT_OF_POINTS; //с какой и по какую точку рисовать
        m_step = 0.1;
        m_qntPoints = (m_endValue - m_startValue)/m_step;
    }
};

class CTrendPlot : public CCustomPlot
{
public:
    CTrendPlot(CCustomContainer *owner, ScaleType hScaleType, ScaleType vScaleType, CRect rect);
    virtual ~CTrendPlot();
    virtual void Clear (void);
private:
    CDOscillPlot *m_pDTrendPlot;
    TrendPlot m_param;

protected:

public:
    //заполнение массива для отрисовки
    void FillPlotArray();

    void SieveArray(const std::vector<SYSTEMTIME> &pArrayX, const std::vector<double> &pArrayY);

    double ConvertFromDate(SYSTEMTIME val);

    //установить видимые границы области
    void SetAxis(SYSTEMTIME minX, SYSTEMTIME maxX, double minY, double maxY);
    //
    void SetData(std::vector<double> &DataArrayY, int Quality);

    //сдвиг
    void Shift(zRect rectType);
    //масштабирование
    void Scale(zRect rectType);
    //сохранение параметров отображения в XML
    virtual void SaveParameters(SimpleXML *pSimpleXML);

};
//------------------------------------------------------------------------------