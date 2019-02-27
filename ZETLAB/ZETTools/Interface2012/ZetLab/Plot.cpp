//------------------------------------------------------------------------------
// Plot.cpp : файл реализации
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\ZetLab\Plot.h"
//------------------------------------------------------------------------------
CPlot::CPlot(CCustomContainer *owner, CRect rect) :
CCustomPlot(owner)
{
    if (m_pOwner)
    {
        m_pDPlot = new CDPlot(rect, m_pOwner->GetPointerToIsChange(),
            m_pOwner->GetPointerToTime() );
        m_pDClass = m_pDPlot;
    }
}
//------------------------------------------------------------------------------
CPlot::~CPlot()
{
    if (m_pDPlot)
        delete m_pDPlot;
};
//------------------------------------------------------------------------------
void CPlot::OnLButtonDown(UINT nFlags, CPoint point){
    m_pDPlot->OnLButtonDown(nFlags, point);
}
//------------------------------------------------------------------------------
void CPlot::SetBorders(double border){
    m_pDPlot->SetBorder(border);
}
//------------------------------------------------------------------------------
void CPlot::SetShift(double ShiftX, double ShiftY){
    
}
//------------------------------------------------------------------------------
void CPlot::SetScale(double ScaleX, double ScaleY){
    
}
//------------------------------------------------------------------------------
void CPlot::SetScaleType(ScaleType scaleTypeX, ScaleType scaleTypeY){
    m_pDPlot->SetScaleType(scaleTypeX, scaleTypeY);
}
//------------------------------------------------------------------------------
void CPlot::SetData(std::vector<double> DataArrayX, std::vector<double> DataArrayY, int ArraySize, int Quality){
    //m_pDPlot->SetData(DataArrayX, DataArrayY, ArraySize, Quality);
}
//------------------------------------------------------------------------------
void CPlot::SetData(std::vector<double> DataArrayY, int ArraySize, int Quality){
    //m_pDPlot->SetData(DataArrayY, ArraySize, Quality);
}
//------------------------------------------------------------------------------
void CPlot::SetStep(double stepX){
    //m_pDPlot->SetStep(stepX);
}
void CPlot::SetAxis(double minX, double maxX, double minY, double maxY)
{
    //m_pDPlot->SetAxis(minX, maxX, minY, maxY);
}
//------------------------------------------------------------------------------
void CPlot::SetBounds(double minX, double maxX, double minY, double maxY)
{
    //m_pDPlot->SetBounds(minX, maxX, minY, maxY);
}
//------------------------------------------------------------------------------
int CPlot::SetSpace(double minX, double maxX, double step)
{
    return m_pDPlot->SetSpace(minX, maxX, step);
}
//------------------------------------------------------------------------------
double CPlot::SetSpace(double minX, double maxX, long Qnt)
{
    return 0.0;
}
//------------------------------------------------------------------------------
void CPlot::SetFit(FitType fitTypeX, FitType fitTypeY)
{
    //m_pDPlot->SetFit(fitTypeX, fitTypeY);
}
//------------------------------------------------------------------------------
void CPlot::Shift(zRect rectType){
    //m_pDPlot->Shift(rectType);
}
//------------------------------------------------------------------------------
void CPlot::Scale(zRect rectType){
    //m_pDPlot->Scale(rectType);
}
//------------------------------------------------------------------------------
void CPlot::SaveParameters(SimpleXML *pSimpleXML)
{
    CCustomElement::SaveParameters(pSimpleXML);
}