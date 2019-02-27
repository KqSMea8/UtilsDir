//------------------------------------------------------------------------------
// Grid.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Custom\CustomGrid.h"
#include "C:\ZETTools\Interface2012\Draw\DGrid.h"
//------------------------------------------------------------------------------
class CGrid : public CCustomGrid
{
public:
    CGrid(CCustomContainer *owner, CRect rect);
    virtual ~CGrid();

private:
    CDGrid *m_pDGrid;
protected:

public:
    virtual void PassParent();
    void SetMinMax(double MinX, double MaxX, double MinY, double MaxY);
    void SetBorders(double border);
    void SetParam(double MinX, double MaxX, double MinY, double MaxY, double border, ScaleType scaleTypeX, ScaleType scaleTypeY);
    double GetShift(bool Axis);
    // виртуальные функции, переопределяемые в классе
    virtual void OnLButtonDown(UINT nFlags, CPoint point);

};
//------------------------------------------------------------------------------