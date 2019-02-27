//------------------------------------------------------------------------------
// Grid.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Custom\CustomGrid.h"
#include "C:\ZETTools\Interface2012\Draw\DGrid.h"
//#include "CustomGrid.h"
//#include "DGrid.h"
//------------------------------------------------------------------------------
class CGrid_i12 : public CCustomGrid
{
public:
    CGrid_i12(CCustomContainer *owner, CRect rect);
    virtual ~CGrid_i12();

private:
    CDGrid *m_pDGrid;
protected:

public:
    void SetMinMax(double MinX, double MaxX, double MinY, double MaxY);
    void SetBorders(double leftBorder, double rightBorder, double topBorder, double bottomBorder);
    void SetScaleType(ScaleType scaleTypeX, ScaleType scaleTypeY);

    void SetStep(double stepX, double stepY);
    double GetStep(GridType gridType);

    void SetShift(double shiftX, double shiftY);
    double GetShift(GridType gridType);

    void SetScale(double scaleX, double scaleY);
    double GetScale(GridType gridType);

    void SetScaleShiftRect(CRect leftShiftRect, CRect rightShiftRect,
        CRect bottomShiftRect, CRect topShiftRect, CRect leftScaleRect,
        CRect rightScaleRect, CRect topScaleRect, CRect bottomScaleRect,
        CRect centerAutoScaleRect);

    void SetAxis(double minX, double maxX, double minY, double maxY);
    TwoDouble GetAxis(GridType gridType);

    void SetBounds(double minX, double maxX, double minY, double maxY);
    void SetFit(FitType fitTypeX, FitType fitTypeY);
    void SetAxisTitle(CString titleX, CString titleY);
    void Shift(zRect rectType);
    void Scale(zRect rectType);

    // виртуальные функции, переопределяемые в классе
    virtual void OnLButtonDown(UINT nFlags, CPoint point);
    virtual void SaveParameters(SimpleXML *pSimpleXML);
    virtual void OnMouseMove(UINT nFlags, CPoint point);

};
//------------------------------------------------------------------------------