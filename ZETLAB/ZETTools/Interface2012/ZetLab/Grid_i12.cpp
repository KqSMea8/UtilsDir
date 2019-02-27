//------------------------------------------------------------------------------
// Grid.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\ZetLab\Grid_i12.h"
//------------------------------------------------------------------------------
CGrid_i12::CGrid_i12(CCustomContainer *owner, CRect rect) :
CCustomGrid(owner)
{
    if (m_pOwner)
    {
        m_pDGrid = new CDGrid(rect, m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime() );
        m_pDClass = m_pDGrid;
        unsigned int andMask=1, xorMask=0;
        m_hCursor = CreateCursor(nullptr, 0, 0, 1, 1, &andMask, &xorMask);
    }
}
//------------------------------------------------------------------------------
CGrid_i12::~CGrid_i12()
{
    if (m_pDGrid)
        delete m_pDGrid;
}
//------------------------------------------------------------------------------
void CGrid_i12::SetMinMax(double MinX, double MaxX, double MinY, double MaxY){
    m_pDGrid->SetMinMaxCurrent(MinX, MaxX, MinY, MaxY);
}
//------------------------------------------------------------------------------
void CGrid_i12::SetBorders(double leftBorder, double rightBorder, double topBorder, double bottomBorder)
{
    m_pDGrid->SetBorder(leftBorder, rightBorder, topBorder, bottomBorder);
}
//------------------------------------------------------------------------------
void CGrid_i12::SetScaleType(ScaleType scaleTypeX, ScaleType scaleTypeY){

}
//------------------------------------------------------------------------------
void CGrid_i12::SetScale(double scaleX, double scaleY){
    m_pDGrid->SetScale(scaleX, scaleY);
}
//------------------------------------------------------------------------------
double CGrid_i12::GetScale(GridType gridType)
{
    return m_pDGrid->GetScale(gridType);
}
//------------------------------------------------------------------------------
void CGrid_i12::SetShift(double shiftX, double shiftY){
    m_pDGrid->SetShift(shiftX, shiftY);
}
//------------------------------------------------------------------------------
double CGrid_i12::GetShift(GridType gridType){
    return m_pDGrid->GetShift(gridType);
}
//------------------------------------------------------------------------------
void CGrid_i12::SetStep(double stepX, double stepY){
    m_pDGrid->SetStep(stepX, stepY);
}
//------------------------------------------------------------------------------
double CGrid_i12::GetStep(GridType gridType)
{
    return m_pDGrid->GetStep(gridType);
}
//------------------------------------------------------------------------------
void CGrid_i12::SetAxis(double minX, double maxX, double minY, double maxY){
    m_pDGrid->SetAxis(minX, maxX, minY, maxY);
}
//------------------------------------------------------------------------------
TwoDouble CGrid_i12::GetAxis(GridType gridType)
{
    return m_pDGrid->GetAxis(gridType);
}
//------------------------------------------------------------------------------
void CGrid_i12::SetAxisTitle(CString titleX, CString titleY)
{
    m_pDGrid->SetAxisTitle(titleX, titleY);
}
//------------------------------------------------------------------------------
void CGrid_i12::OnLButtonDown(UINT nFlags, CPoint point){
    m_pDGrid->OnLButtonDown(nFlags, point);
}
//------------------------------------------------------------------------------
void CGrid_i12::SetScaleShiftRect(CRect leftShiftRect, CRect rightShiftRect,
    CRect bottomShiftRect, CRect topShiftRect, CRect leftScaleRect,
    CRect rightScaleRect, CRect topScaleRect, CRect bottomScaleRect,
    CRect centerAutoScaleRect)
{
    m_pDGrid->SetRectAll(leftShiftRect, rightShiftRect,
        bottomShiftRect, topShiftRect, leftScaleRect,
        rightScaleRect, topScaleRect, bottomScaleRect, centerAutoScaleRect);
}
//------------------------------------------------------------------------------
void CGrid_i12::SetFit(FitType fitTypeX, FitType fitTypeY){
    m_pDGrid->SetFit(fitTypeX, fitTypeY);
}
//------------------------------------------------------------------------------
void CGrid_i12::SetBounds(double minX, double maxX, double minY, double maxY)
{
    m_pDGrid->SetBounds(minX, maxX, minY, maxY);
}
//------------------------------------------------------------------------------
void CGrid_i12::OnMouseMove(UINT nFlags, CPoint point){
    SetClassLongW(GetParent()->m_hWnd, GCL_HCURSOR, (LONG)m_hCursor);    
    CCustomElement::OnMouseMove(nFlags, point);
}
void CGrid_i12::SaveParameters(SimpleXML *pSimpleXML)
{
    CCustomElement::SaveParameters(pSimpleXML);
}
//------------------------------------------------------------------------------
void CGrid_i12::Shift(zRect rectType){
    m_pDGrid->Shift(rectType);
}
//------------------------------------------------------------------------------
void CGrid_i12::Scale(zRect rectType){
    m_pDGrid->Scale(rectType);
}
//------------------------------------------------------------------------------