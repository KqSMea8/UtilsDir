//------------------------------------------------------------------------------
// DPlot.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include "math.h"
#include "C:\ZETTools\Interface2012\Draw\CustomDraw.h"
#include "C:\ZETTools\Interface2012\Draw\FunctionsForGraph.h"

#define ZDEPTH 6

struct ParamPlot : public ParamCustomDraw {
    CRect m_rect;
    CRect m_rectRightShift, m_rectLeftShift, m_rectTopShift, m_rectBottomShift, m_rectPlot,
         m_rectRightScale, m_rectLeftScale, m_rectBottomScale, m_rectTopScale, m_centerAutoScaleRect;
    GLfloat m_plotColor[3], m_plotBorderColor[3];
    double m_dataArrayX[MAX_QNT_OF_POINTS], m_dataArrayY[MAX_QNT_OF_POINTS];
    double m_workArrayX[MAX_QNT_OF_POINTS], m_workArrayYmax[MAX_QNT_OF_POINTS], m_workArrayYmin[MAX_QNT_OF_POINTS];
    double m_maxCurrentX, m_minCurrentX, m_maxCurrentY, m_minCurrentY,
        m_shiftX, m_shiftY, m_border, m_step, m_scaleX, m_scaleY,
        m_leftBound, m_rightBound, m_topBound, m_bottomBound,
        m_startValue, m_endValue;
    int m_qntPoints;
    int m_begin, m_end;
    ScaleType m_scaleTypeX, m_scaleTypeY;
    Settings m_settings;
    FitType m_fitTypeX, m_fitTypeY;
    PlotType m_plotType;

    ParamPlot(){
        Size = sizeof(ParamPlot);
        m_plotColor[0] = 1;
        m_plotColor[1] = 203;
        m_plotColor[2] = 247;
        m_plotBorderColor[0] = 80;
        m_plotBorderColor[1] = 80;
        m_plotBorderColor[2] = 80;
        m_maxCurrentX = 100;
        m_minCurrentX = 0;
        m_maxCurrentY = 100;
        m_minCurrentY = 0;
        m_startValue = 0;
        m_endValue = 100;
        m_leftBound = -200;
        m_rightBound = 200;
        m_topBound = 200;
        m_bottomBound = -200;
        m_step = 10;
        m_scaleX = 2;
        m_scaleY = 2;
        m_settings = sAuto;
        m_fitTypeX = ftLeft;
        m_fitTypeY = ftCenter;
        m_plotType = ptOscill;
        m_begin = 0;
        m_end = m_begin + MAX_QNT_OF_POINTS;
		memset(m_dataArrayX, 0.0, sizeof(m_dataArrayX));
		memset(m_dataArrayY, 0.0, sizeof(m_dataArrayY));
		memset(m_workArrayX, 0.0, sizeof(m_workArrayX));
		memset(m_workArrayYmax, 0.0, sizeof(m_workArrayYmax));
		memset(m_workArrayYmin, 0.0, sizeof(m_workArrayYmin));
		m_shiftX = m_shiftY = 0.0;
		m_border = 0.0;
		m_qntPoints = 0;
		memset(&m_scaleTypeX, 0, sizeof(m_scaleTypeX)); 
		memset(&m_scaleTypeY, 0, sizeof(m_scaleTypeY)); 
    }
};

class CDPlot : public CCustomDraw{
public:
    CDPlot(CRect rect, bool *pFlagIsChange, double *pTimeAnimation);
    virtual ~CDPlot(); 

protected:

private:
    ParamPlot m_param;
    bool m_bFlag;
    zRect IsMouseInRect(CPoint point);
public:
    void Shift(zRect val);
    void Scale(zRect val);
    void SetRectAll(CRect leftShiftRect, CRect rightShiftRect,
        CRect bottomShiftRect, CRect topShiftRect, CRect leftScaleRect,
        CRect rightScaleRect, CRect topScaleRect, CRect bottomScaleRect,
        CRect centerAutoScaleRect);
    void DrawPlot(double *DataArrayX, double *DataArrayY);
    void DrawSpectr(double *DataArrayX, double *DataArrayY);
    void SetData(double *DataArrayX, double *DataArrayY, int Quality);
    void SetData(double *DataArrayY, int Quality);
    int SetSpace(double minX, double maxX, double step);
    void SetBorder(double border);
    void SetScaleType(ScaleType scaleTypeX, ScaleType scaleTypeY);
    void SetShift(double shiftX, double shiftY);
    void SetScale(double scaleX, double scaleY);
    void SetStep(double step);
    void SetFit(FitType fitTypeX, FitType fitTypeY);
    void SetPlotType(PlotType plotType);
    double FromWorldToScreen(double val, TypePosition Type);
    void SetAxis(double minX, double maxX, double minY, double maxY);
    void SetBounds(double minX, double maxX, double minY, double maxY);
    void SieveArray(double *pArrayX, double *pArrayY, int arraySize);
    void CreateOutputSpectrArray(double *pInputX, double *pInputY, double *pOutput, double *pOutputY, int Begin, int End);

    // виртуальные функции, переопределяемые в классе
    virtual void OnDraw();
    virtual void SaveParameters(SimpleXML *pSimpleXML);
    virtual void LoadParameters(SimpleXML *pSimpleXML);
    virtual void OnLButtonDown(UINT nFlags, CPoint point);
    /*virtual void OnMouseMove(UINT nFlags, CPoint point);
    virtual void OnMouseEnter();
    virtual void OnMouseLeave();*/
};