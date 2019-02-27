//------------------------------------------------------------------------------
// Grid.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\ZetLab\Grid.h"
//------------------------------------------------------------------------------
CGrid::CGrid(CCustomContainer *owner, CRect rect) :
CCustomGrid(owner)
{
    if (m_pOwner)
    {
        m_pDGrid = new CDGrid(rect, m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime() );
        m_pDClass = m_pDGrid;
        CString str = m_pOwner->GetFolderPNG() + L"\\Grid\\";
    }
}
//------------------------------------------------------------------------------
CGrid::~CGrid()
{
    if (m_pDGrid)
        delete m_pDGrid;
}

void CGrid::SetParam(double MinX, double MaxX, double MinY, double MaxY, double border, ScaleType scaleTypeX, ScaleType scaleTypeY){
    m_pDGrid->SetParam(MinX, MaxX, MinY, MaxY, border, scaleTypeX, scaleTypeY);
}

void CGrid::SetMinMax(double MinX, double MaxX, double MinY, double MaxY){
    m_pDGrid->SetMinMaxCurrent(MinX, MaxX, MinY, MaxY);
}

void CGrid::SetBorders(double border){
    m_pDGrid->SetBorder(border);
}

void CGrid::OnLButtonDown(UINT nFlags, CPoint point){
    m_pDGrid->OnLButtonDown(nFlags, point);
}

double CGrid::GetShift(bool Axis){
    return m_pDGrid->GetShift(Axis);
}