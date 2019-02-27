//------------------------------------------------------------------------------
// OscillPlot.cpp : файл реализации
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "TrendPlot.h"
//------------------------------------------------------------------------------

CTrendPlot::CTrendPlot(CCustomContainer *owner, ScaleType hScaleType, ScaleType vScaleType, CRect rect) :
CCustomPlot(owner)
{
    if (m_pOwner)
    {
        m_pPlotParam = &m_param;
        m_param.m_rect = rect;
        m_param.m_axisX.paScaleType = hScaleType;
        m_param.m_axisY.paScaleType = vScaleType;

        m_pDTrendPlot = new CDOscillPlot(rect, m_pOwner->GetPointerToIsChange(),m_pOwner->GetPointerToTime() );
        m_pDClass = m_pDTrendPlot;
    }
}
//------------------------------------------------------------------------------
void CTrendPlot::Clear (void)
{
	m_pDTrendPlot->Clear();
}
//------------------------------------------------------------------------------
CTrendPlot::~CTrendPlot()
{
	m_param.Clear();
    if (m_pDTrendPlot)
        delete m_pDTrendPlot;
};
//------------------------------------------------------------------------------
void CTrendPlot::FillPlotArray()
{
    int Counter, Length;
    if(!m_param.pVertexs.empty())
        m_param.pVertexs.clear();
    SieveArray(m_param.m_axisX.paTimeDataArray, m_param.m_axisY.paDataArray);
    if(m_param.m_axisY.paMinCurrentValue > m_param.m_axisY.paMaxCurrent)
    {
        m_param.pVertexs.push_back(m_param.m_rect.left);
        m_param.pVertexs.push_back(m_param.m_rect.top+2);
        m_param.pVertexs.push_back(5);
        m_param.pVertexs.push_back(m_param.m_rect.right);
        m_param.pVertexs.push_back(m_param.m_rect.top+2);
        m_param.pVertexs.push_back(5);
    }

    if(m_param.m_axisY.paMaxCurrentValue < m_param.m_axisY.paMinCurrent)
    {
        m_param.pVertexs.push_back(m_param.m_rect.left);
        m_param.pVertexs.push_back(m_param.m_rect.bottom-2);
        m_param.pVertexs.push_back(5);
        m_param.pVertexs.push_back(m_param.m_rect.right);
        m_param.pVertexs.push_back(m_param.m_rect.bottom-2);
        m_param.pVertexs.push_back(5);
    }

    if(m_param.m_axisY.paMinCurrentValue < m_param.m_axisY.paMaxCurrent &&
        m_param.m_axisY.paMaxCurrentValue > m_param.m_axisY.paMinCurrent){
            if(m_param.m_axisY.paDrawArray.size()>m_param.m_axisX.paDrawArray.size())
                Length = m_param.m_axisX.paDrawArray.size();
            else 
                Length = m_param.m_axisY.paDrawArray.size();

            for(Counter=0; Counter<Length; Counter++)
            {
                m_param.pVertexs.push_back(m_param.m_rect.left + m_param.m_axisX.paDrawArray[Counter]);
                m_param.pVertexs.push_back(m_param.m_rect.bottom - FromWorldToScreen(m_param.m_axisY.paDrawArray[Counter],
                    m_param.m_axisY.paMinCurrent,
                    m_param.m_axisY.paMaxCurrent, gtVertical,  m_param.m_axisY.paScaleType,
                    m_param.m_rect));
                m_param.pVertexs.push_back(5);
            }
    }
    m_pDTrendPlot->SetPlotVertex(m_param.pVertexs);
}
//------------------------------------------------------------------------------
void CTrendPlot::SieveArray(const std::vector<SYSTEMTIME> &pArrayX, const std::vector<double> &pArrayY)
{
    DATE a;
    COleDateTime b;
    double current, next, max, min, counterValue, temp;
    int Counter, Counter1=0, PointOfStart;
    long Length;
    bool flag=false;
    CTime start, end, firstVal, t_min, t_max;
    CTimeSpan delta, dx;
    if(pArrayY.empty() || pArrayX.empty()){
        return;
    }

    m_param.m_axisX.paDrawArray.clear();
    m_param.m_axisY.paDrawArray.clear();

    if(m_param.m_axisY.paDataArray.size()>m_param.m_axisX.paTimeDataArray.size())
        Length = m_param.m_axisX.paTimeDataArray.size();
    else 
        Length = m_param.m_axisY.paDataArray.size();

    m_param.m_axisX.paMinCurrent = 0;

    if(m_param.m_stepDate.tiID == Year)
    {
        PointOfStart = (m_param.m_axisX.paMinCurrentTime.wYear - pArrayX[0].wYear)/m_param.m_stepDate.tiData + 1;
        (m_param.m_axisX.paMinCurrentTime.wYear - pArrayX[PointOfStart].wYear);
        m_param.m_axisX.paMaxCurrent = m_param.m_axisX.paMaxCurrentTime.wMonth
            + m_param.m_axisX.paMaxCurrentTime.wYear*12 - m_param.m_axisX.paMinCurrentTime.wMonth
            - m_param.m_axisX.paMinCurrentTime.wYear*12 + m_param.m_axisX.paMaxCurrentTime.wDay/30 - m_param.m_axisX.paMinCurrentTime.wDay/30;
    }

    if(m_param.m_stepDate.tiID == Month)
    {
        PointOfStart = ((m_param.m_axisX.paMinCurrentTime.wYear - pArrayX[0].wYear)*12
            - (m_param.m_axisX.paMinCurrentTime.wMonth - pArrayX[0].wMonth))/m_param.m_stepDate.tiData + 1;
        m_param.m_axisX.paMaxCurrent = m_param.m_axisX.paMaxCurrentTime.wMonth
            + m_param.m_axisX.paMaxCurrentTime.wYear*12 - m_param.m_axisX.paMinCurrentTime.wMonth
            - m_param.m_axisX.paMinCurrentTime.wYear*12 + m_param.m_axisX.paMaxCurrentTime.wDay/30 - m_param.m_axisX.paMinCurrentTime.wDay/30;
    }

    start = CTime(m_param.m_axisX.paMinCurrentTime);
    end = CTime(m_param.m_axisX.paMaxCurrentTime);
    firstVal = CTime(pArrayX[0]);
    delta = firstVal - start;
    dx = end - start;

    if(m_param.m_stepDate.tiID == Day)
    {
        PointOfStart = delta.GetDays()/m_param.m_stepDate.tiData + 1;
        m_param.m_axisX.paMaxCurrent = dx.GetTotalHours()/24.0;
    }

    if(m_param.m_stepDate.tiID == Hour)
    {
        PointOfStart = delta.GetTotalHours()/m_param.m_stepDate.tiData + 1;
        m_param.m_axisX.paMaxCurrent = dx.GetTotalMinutes()/60.0;
    }

    if(m_param.m_stepDate.tiID == Minute)
    {
        PointOfStart = delta.GetTotalMinutes()/m_param.m_stepDate.tiData + 1;
        m_param.m_axisX.paMaxCurrent = dx.GetTotalSeconds()/60.0;
    }

    if(m_param.m_stepDate.tiID == Second)
    {
        PointOfStart = delta.GetTotalSeconds()/m_param.m_stepDate.tiData + 1;
        m_param.m_axisX.paMaxCurrent = dx.GetTotalSeconds();
    }

    if(m_param.m_stepDate.tiID == Millisecond)
    {
        PointOfStart = (delta.GetTotalSeconds()*1000 + pArrayX[0].wMilliseconds
            - m_param.m_axisX.paMinCurrentTime.wMilliseconds)/m_param.m_stepDate.tiData;
        m_param.m_axisX.paMaxCurrent = dx.GetTotalSeconds()*1000 + m_param.m_axisX.paMaxCurrentTime.wMilliseconds
            - m_param.m_axisX.paMinCurrentTime.wMilliseconds;
    }

    PointOfStart = abs(PointOfStart);

    if(start < firstVal)
    {
        PointOfStart = 0;
    }

    if(PointOfStart > Length-1)
        PointOfStart = Length-1;

    m_param.m_firstPoint = PointOfStart;

    current = next = FromWorldToScreen(ConvertFromDate(pArrayX[PointOfStart]), m_param.m_axisX.paMinCurrent, 
        m_param.m_axisX.paMaxCurrent, gtHorizontal, m_param.m_axisX.paScaleType, m_param.m_rect);

    m_param.m_axisY.paMinCurrentValue = pArrayY[0];
    m_param.m_axisY.paMaxCurrentValue = pArrayY[0];

    for(Counter=PointOfStart; Counter<Length-1; Counter++)
    {
        if(_finite(pArrayY[Counter]) == 0)
            continue;
        if(pArrayY[Counter] < m_param.m_axisY.paMinCurrentValue)
            m_param.m_axisY.paMinCurrentValue = pArrayY[Counter];

        if(pArrayY[Counter] > m_param.m_axisY.paMaxCurrentValue)
            m_param.m_axisY.paMaxCurrentValue = pArrayY[Counter];

        counterValue = next;
        next = FromWorldToScreen(ConvertFromDate(pArrayX[Counter+1]), m_param.m_axisX.paMinCurrent, 
            m_param.m_axisX.paMaxCurrent, gtHorizontal, m_param.m_axisX.paScaleType, m_param.m_rect);
        if((next - current) > 1.0)
        {
            if(flag)
            {
                m_param.m_axisX.paDrawArray.push_back(temp);
                m_param.m_axisX.paDrawArray.push_back(temp);
                m_param.m_axisY.paDrawArray.push_back(min);
                m_param.m_axisY.paDrawArray.push_back(max);
                flag = false;
                current = next;
                Counter1++;
                continue;
            }
            else
            {
                m_param.m_axisX.paDrawArray.push_back(counterValue);
                m_param.m_axisY.paDrawArray.push_back(pArrayY[Counter]);
                Counter1++;
            }
        }
        else
        {
            if(!flag)
            {
                if(pArrayY[Counter] > pArrayY[Counter+1]){
                    max = pArrayY[Counter];
                    min = pArrayY[Counter+1];
                }
                else{
                    min = pArrayY[Counter];
                    max = pArrayY[Counter+1];
                }
                temp = counterValue;
                flag = true;
                continue;
            }
            else
            {
                if(pArrayY[Counter+1] > max)
                    max = pArrayY[Counter+1];
                if(pArrayY[Counter+1] < min)
                    min = pArrayY[Counter+1];
            }
        }
    }
    m_param.m_axisX.paDrawArray.push_back(FromWorldToScreen(ConvertFromDate(pArrayX[Length-1]), m_param.m_axisX.paMinCurrent, 
        m_param.m_axisX.paMaxCurrent, gtHorizontal, m_param.m_axisX.paScaleType, m_param.m_rect));
    m_param.m_axisY.paDrawArray.push_back(pArrayY[Length-1]);
}
//------------------------------------------------------------------------------
void CTrendPlot::SetData(std::vector<double> &DataArrayY, int Quality)
{
    CCustomPlot::SetData(DataArrayY, Quality);
    FillPlotArray();
}
//------------------------------------------------------------------------------
void CTrendPlot::SaveParameters(SimpleXML *pSimpleXML)
{
    CCustomElement::SaveParameters(pSimpleXML);
}
//------------------------------------------------------------------------------
double CTrendPlot::ConvertFromDate(SYSTEMTIME val){
    CTimeSpan delta;

    if(m_param.m_stepDate.tiID == Year)
    {
        return (val.wMonth + val.wYear*12 - m_param.m_axisX.paMinCurrentTime.wMonth
            - m_param.m_axisX.paMinCurrentTime.wYear*12 + val.wDay/30
            - m_param.m_axisX.paMinCurrentTime.wDay/30);
    }

    if(m_param.m_stepDate.tiID == Month)
    {
        return (val.wMonth + val.wYear*12 - m_param.m_axisX.paMinCurrentTime.wMonth
            - m_param.m_axisX.paMinCurrentTime.wYear*12 + val.wDay/30
            - m_param.m_axisX.paMinCurrentTime.wDay/30);
    }

    delta = CTime(val) - CTime(m_param.m_axisX.paMinCurrentTime);

    if(m_param.m_stepDate.tiID == Day)
    {
        return delta.GetTotalHours()/24.0;
    }

    if(m_param.m_stepDate.tiID == Hour)
    {
        return delta.GetTotalMinutes()/60.0;
    }

    if(m_param.m_stepDate.tiID == Minute)
    {
        return delta.GetTotalSeconds()/60.0;
    }

    if(m_param.m_stepDate.tiID == Second)
    {
        return delta.GetTotalSeconds();
    }

    if(m_param.m_stepDate.tiID == Millisecond)
    {
        return delta.GetTotalSeconds()*1000
            + val.wMilliseconds - m_param.m_axisX.paMinCurrentTime.wMilliseconds;
    }

    return -1.0;
}