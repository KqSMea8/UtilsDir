//------------------------------------------------------------------------------
// CustomPlot.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include <Interface2012\Custom\CustomPlot.h>
//------------------------------------------------------------------------------

CCustomPlot::CCustomPlot(CCustomContainer *owner) : CCustomElement(owner)
	, m_pPlotParam(nullptr)
{
    //m_FunctionEvent = NULL;
    m_hCursor = LoadCursorW(NULL, IDC_CROSS/*IDC_HAND*/);
}
//------------------------------------------------------------------------------
CCustomPlot::~CCustomPlot()
{
	m_pPlotParam->Clear();
}
//------------------------------------------------------------------------------
double CCustomPlot::FromWorldToScreen(double val, double MinCurrent, double MaxCurrent, GridType gridType, ScaleType scaleType, const CRect &rect)
{
    double Length(0.0);
	if (gridType == gtHorizontal) Length = rect.right - rect.left;
	else Length = rect.bottom - rect.top;

    //if(scaleType==stLinear)
	return (val - MinCurrent) / (MaxCurrent - MinCurrent) * Length;
}
//------------------------------------------------------------------------------
int CCustomPlot::SetSpace(double pointOfStart, double pointOfEnd, double step, bool Invertation)
{
    int i;
	std::vector<double> tmp;
	tmp.resize(m_pPlotParam->m_qntPoints);

    if(step==0)
    {
        return -1;
    }
    m_pPlotParam->m_step = step;
    
    m_pPlotParam->m_startValue = pointOfStart;
    m_pPlotParam->m_endValue = pointOfEnd;
    m_pPlotParam->m_qntPoints = (pointOfEnd-pointOfStart)/step+1;
    if(m_pPlotParam->m_qntPoints>MAX_QNT_OF_POINTS)
        m_pPlotParam->m_qntPoints = MAX_QNT_OF_POINTS;

	tmp.resize(m_pPlotParam->m_qntPoints);
    m_pPlotParam->m_axisX.paDataArray.resize(m_pPlotParam->m_qntPoints);
    m_pPlotParam->m_axisY.paDataArray.resize(m_pPlotParam->m_qntPoints);

	tmp[0] = pointOfStart;
	for (i = 1; i < m_pPlotParam->m_qntPoints; i++){
		pointOfStart = pointOfStart + m_pPlotParam->m_step;
		tmp[i] = pointOfStart;
	}
    /*m_pPlotParam->m_axisX.paDataArray[0] = pointOfStart;
    for(i=1; i < m_pPlotParam->m_qntPoints; i++){
        pointOfStart = pointOfStart + m_pPlotParam->m_step;
        m_pPlotParam->m_axisX.paDataArray[i] = pointOfStart;
    }*/

	if(!Invertation)
	{
		if(!m_pPlotParam->m_axisX.paDataArray.empty())
			m_pPlotParam->m_axisX.paDataArray.clear();
		m_pPlotParam->m_axisX.paDataArray = tmp;
	}

	else
	{
		if (!m_pPlotParam->m_axisY.paDataArray.empty())
			m_pPlotParam->m_axisY.paDataArray.clear();
		m_pPlotParam->m_axisY.paDataArray = tmp;
	}
    return m_pPlotParam->m_qntPoints;
}
//------------------------------------------------------------------------------
double CCustomPlot::SetSpace(double pointOfStart, double pointOfEnd, long Qnt, bool Invertation)
{

	if (pointOfStart == pointOfEnd)
		return m_pPlotParam->m_step;

    int i;
	std::vector<double> tmp;
	m_pPlotParam->m_qntPoints = Qnt;
	tmp.resize(m_pPlotParam->m_qntPoints);

    if(m_pPlotParam->m_qntPoints>MAX_QNT_OF_POINTS)
        m_pPlotParam->m_qntPoints = MAX_QNT_OF_POINTS;

    m_pPlotParam->m_axisX.paDataArray.resize(m_pPlotParam->m_qntPoints);
    m_pPlotParam->m_axisY.paDataArray.resize(m_pPlotParam->m_qntPoints);

	if (pointOfStart > pointOfEnd)
	{
		m_pPlotParam->m_startValue = pointOfEnd;
		m_pPlotParam->m_endValue = pointOfStart;
	}
	else{
		m_pPlotParam->m_startValue = pointOfStart;
		m_pPlotParam->m_endValue = pointOfEnd;
	}
	m_pPlotParam->m_step = (m_pPlotParam->m_endValue - m_pPlotParam->m_startValue) / (m_pPlotParam->m_qntPoints - 1);
	tmp[0] = m_pPlotParam->m_startValue;
    for(i=1; i < m_pPlotParam->m_qntPoints; i++){
		tmp[i] = m_pPlotParam->m_startValue + m_pPlotParam->m_step*i;
    }

	if (!Invertation)
	{
		if (!m_pPlotParam->m_axisX.paDataArray.empty())
			m_pPlotParam->m_axisX.paDataArray.clear();
		m_pPlotParam->m_axisX.paDataArray = tmp;
	}

	else
	{
		if (!m_pPlotParam->m_axisY.paDataArray.empty())
			m_pPlotParam->m_axisY.paDataArray.clear();
		m_pPlotParam->m_axisY.paDataArray = tmp;
	}
    return m_pPlotParam->m_step;
}
//------------------------------------------------------------------------------
void CCustomPlot::SetData(std::vector<double> &DataArrayX, std::vector<double> &DataArrayY,
    int Quality)
{
    m_pPlotParam->m_axisX.paDataArray = DataArrayX;
    m_pPlotParam->m_axisY.paDataArray = DataArrayY;
    m_pPlotParam->m_qntPoints = DataArrayX.size();
}
//------------------------------------------------------------------------------
void CCustomPlot::SetData(std::vector<double> &DataArrayY, int Quality, bool Invertation)
{
	if (!Invertation)
	{
		m_pPlotParam->m_axisY.paDataArray.clear();
		m_pPlotParam->m_axisY.paDataArray = DataArrayY;
	}

	else
	{
		m_pPlotParam->m_axisX.paDataArray.clear();
		m_pPlotParam->m_axisX.paDataArray = DataArrayY;
	}
}
//------------------------------------------------------------------------------
void CCustomPlot::SetDataFloat(std::vector<float> &DataArrayY, int Quality, bool Invertation)
{
    int size = DataArrayY.size();
	std::vector<double> tmp;

	tmp.resize(m_pPlotParam->m_qntPoints);

	if(m_pPlotParam->m_qntPoints <= size)
    {
		ippsConvert_32f64f(DataArrayY.data(), tmp.data(), m_pPlotParam->m_qntPoints);
    }
    else
    {
        ippsConvert_32f64f(DataArrayY.data(),
			tmp.data(), size);
        //ippsSet_64f((std::numeric_limits<double>::quiet_NaN()),
		ippsSet_64f(0.0,
			tmp.data() + size,
            m_pPlotParam->m_qntPoints - size);
    }

	if (!Invertation)
	{
		m_pPlotParam->m_axisY.paDataArray.resize(m_pPlotParam->m_qntPoints);
		m_pPlotParam->m_axisY.paDataArray = tmp;
	}

	else
	{
		m_pPlotParam->m_axisX.paDataArray.resize(m_pPlotParam->m_qntPoints);
		m_pPlotParam->m_axisX.paDataArray = tmp;
	}
}
//------------------------------------------------------------------------------
TwoDouble CCustomPlot::GetMinMaxAxisX()
{
	TwoDouble temp;
	temp.Min = m_pPlotParam->m_axisX.paMinCurrentValue;
	temp.Max = m_pPlotParam->m_axisX.paMaxCurrentValue;
	return temp;
}
//------------------------------------------------------------------------------
TwoDouble CCustomPlot::GetMinMaxAxisY()
{
    TwoDouble temp;
    temp.Min = m_pPlotParam->m_axisY.paMinCurrentValue;
    temp.Max = m_pPlotParam->m_axisY.paMaxCurrentValue;
    return temp;
}
//------------------------------------------------------------------------------
void CCustomPlot::SetAxis(double minX, double maxX, double minY, double maxY)
{
    m_pPlotParam->m_axisX.paMinCurrent = minX;
    m_pPlotParam->m_axisX.paMaxCurrent = maxX;
    m_pPlotParam->m_axisY.paMinCurrent = minY;
    m_pPlotParam->m_axisY.paMaxCurrent = maxY;
}
//------------------------------------------------------------------------------
CursorValues CCustomPlot::GetCursorValueX(int px, PlotType plotType)
{
    CursorValues temp;
	double px_step(0.0), px_pointOfStart(0.0);
	char str[64] = "" ;
    int  a(0);
    COleDateTime time;

	temp.X.Empty();
	temp.Y.Empty();
	temp.pxX = 0;
	temp.pxY = 0;

	if (m_pPlotParam->m_axisX.paDataArray.empty() || m_pPlotParam->m_axisY.paDataArray.empty())
		return temp;

    px_step = FromWorldToScreen(m_pPlotParam->m_step+m_pPlotParam->m_axisX.paMinCurrent, m_pPlotParam->m_axisX.paMinCurrent,
        m_pPlotParam->m_axisX.paMaxCurrent, m_pPlotParam->m_axisX.paGridType,
        m_pPlotParam->m_axisX.paScaleType, m_pPlotParam->m_rect);
    px_pointOfStart = FromWorldToScreen(m_pPlotParam->m_axisX.paDataArray[m_pPlotParam->m_firstPoint],
        m_pPlotParam->m_axisX.paMinCurrent, m_pPlotParam->m_axisX.paMaxCurrent,
        m_pPlotParam->m_axisX.paGridType,
        m_pPlotParam->m_axisX.paScaleType, m_pPlotParam->m_rect);
    a = floor((px - px_pointOfStart - m_pPlotParam->m_rect.left)/px_step + 0.5) + m_pPlotParam->m_firstPoint;
    if(a < 0)
        a = 0;

    if(a >=  m_pPlotParam->m_axisX.paDataArray.size())
        a = m_pPlotParam->m_axisX.paDataArray.size()-1;

    if(plotType == ptTrend)
    {
        time = COleDateTime(m_pPlotParam->m_axisX.paDataArray[a]);
        temp.X = time.Format(_T("Date: %d/%m/%Y Time: %H:%M:%S"));
    }
    else
    {
        sprintf_s(str, "X = %0.3f", m_pPlotParam->m_axisX.paDataArray[a]);
        temp.X = str;
    }
    sprintf_s(str, "Y = %0.3f", m_pPlotParam->m_axisY.paDataArray[a]);
    temp.Y = str;
    temp.pxX = FromWorldToScreen(m_pPlotParam->m_axisX.paDataArray[a],
        m_pPlotParam->m_axisX.paMinCurrent, m_pPlotParam->m_axisX.paMaxCurrent,
        m_pPlotParam->m_axisX.paGridType, m_pPlotParam->m_axisX.paScaleType,
        &m_pPlotParam->m_rect);
    temp.pxY = FromWorldToScreen(m_pPlotParam->m_axisY.paDataArray[a],
        m_pPlotParam->m_axisY.paMinCurrent, m_pPlotParam->m_axisY.paMaxCurrent,
        m_pPlotParam->m_axisY.paGridType, m_pPlotParam->m_axisY.paScaleType,
        &m_pPlotParam->m_rect);
    return temp;
};
//------------------------------------------------------------------------------
CursorValues CCustomPlot::GetCursorValueXInv(int px, PlotType plotType)
{
	CursorValues temp;
	double px_step(0.0), px_pointOfStart(0.0);
	char str[64] = "";
	int  a(0);
	COleDateTime time;

	temp.X.Empty();
	temp.Y.Empty();
	temp.pxX = 0;
	temp.pxY = 0;

	return temp;
}
//------------------------------------------------------------------------------
CursorValues CCustomPlot::GetCursorValueY(int px, PlotType plotType)
{
	CursorValues temp;
	double px_step(0.0), px_pointOfStart(0.0);
	char str[64] = "";
	int  a(0);
	COleDateTime time;

	temp.X.Empty();
	temp.Y.Empty();
	temp.pxX = 0;
	temp.pxY = 0;

	return temp;
}
//------------------------------------------------------------------------------
CursorValues CCustomPlot::GetCursorValueYInv(int px, PlotType plotType)
{
	CursorValues temp;
	double px_step(0.0), px_pointOfStart(0.0);
	char str[64] =  "" ;
	int  a;
	COleDateTime time;

	temp.X.Empty();
	temp.Y.Empty();
	temp.pxX = 0;
	temp.pxY = px;

	if (m_pPlotParam->m_axisX.paDataArray.empty() || m_pPlotParam->m_axisY.paDataArray.empty())
		return temp;

	px_step = FromWorldToScreen(m_pPlotParam->m_step + m_pPlotParam->m_axisY.paMinCurrent, m_pPlotParam->m_axisY.paMinCurrent,
		m_pPlotParam->m_axisY.paMaxCurrent, m_pPlotParam->m_axisY.paGridType,
		m_pPlotParam->m_axisY.paScaleType, m_pPlotParam->m_rect);
	px_pointOfStart = FromWorldToScreen(m_pPlotParam->m_axisY.paDataArray[m_pPlotParam->m_firstPoint],
		m_pPlotParam->m_axisY.paMinCurrent, m_pPlotParam->m_axisY.paMaxCurrent,
		m_pPlotParam->m_axisY.paGridType,
		m_pPlotParam->m_axisY.paScaleType, m_pPlotParam->m_rect);
	a = floor((m_pPlotParam->m_rect.bottom - px - px_pointOfStart) / px_step + 0.5) + m_pPlotParam->m_firstPoint;
	if (a < 0)
		a = 0;

	if (a >= m_pPlotParam->m_axisY.paDataArray.size())
		a = m_pPlotParam->m_axisY.paDataArray.size() - 1;

	//if (plotType == ptParam)
	//{
	//	time = COleDateTime(m_pPlotParam->m_axisY.paDataArray[a]);
	//	temp.Y = time.Format(_T("Date: %d/%m/%Y Time: %H:%M:%S"));
	//}
	//else
	{
		sprintf_s(str, "Y = %0.3f", m_pPlotParam->m_axisY.paDataArray[a]);
		temp.Y = str;
	}
	sprintf_s(str, "X = %0.3f", m_pPlotParam->m_axisX.paDataArray[a]);
	temp.X = str;
	temp.pxX = FromWorldToScreen(m_pPlotParam->m_axisX.paDataArray[a],
		m_pPlotParam->m_axisX.paMinCurrent, m_pPlotParam->m_axisX.paMaxCurrent,
		m_pPlotParam->m_axisX.paGridType, m_pPlotParam->m_axisX.paScaleType,
		&m_pPlotParam->m_rect);
	temp.pxY = m_pPlotParam->m_rect.bottom - FromWorldToScreen(m_pPlotParam->m_axisY.paDataArray[a],
		m_pPlotParam->m_axisY.paMinCurrent, m_pPlotParam->m_axisY.paMaxCurrent,
		m_pPlotParam->m_axisY.paGridType, m_pPlotParam->m_axisY.paScaleType,
		&m_pPlotParam->m_rect);

	return temp;
}
//------------------------------------------------------------------------------
double CCustomPlot::GetPointOfStartVal(bool invertation)
{
	if (!m_pPlotParam->m_axisX.paDataArray.empty() && !invertation)
        return m_pPlotParam->m_axisX.paDataArray[m_pPlotParam->m_firstPoint];
	if (!m_pPlotParam->m_axisY.paDataArray.empty() && invertation)
		return m_pPlotParam->m_axisY.paDataArray[m_pPlotParam->m_firstPoint]; 
	
	return std::numeric_limits<double>::quiet_NaN();
}
//------------------------------------------------------------------------------
double CCustomPlot::GetCursorPositionInPoint(unsigned int pointNumber, bool invertation)
{
	unsigned int number = pointNumber;
	if (!m_pPlotParam->m_axisX.paDataArray.empty() && !invertation)
	{
		if (pointNumber > m_pPlotParam->m_axisX.paDataArray.size() - 1)
			number = m_pPlotParam->m_axisX.paDataArray.size() - 1;
		if ((m_pPlotParam->m_axisX.paDataArray[number] >= m_pPlotParam->m_axisX.paMinCurrent))
		{
			if ((m_pPlotParam->m_axisX.paDataArray[number] <= m_pPlotParam->m_axisX.paMaxCurrent))
			{
				return m_pPlotParam->m_rect.left + FromWorldToScreen(m_pPlotParam->m_axisX.paDataArray[number],
					m_pPlotParam->m_axisX.paMinCurrent, m_pPlotParam->m_axisX.paMaxCurrent,
					m_pPlotParam->m_axisX.paGridType, m_pPlotParam->m_axisX.paScaleType,
					m_pPlotParam->m_rect);
			}
			else
			{
				return m_pPlotParam->m_rect.right;
			}
		}
		else
		{
			return m_pPlotParam->m_rect.left;
		}
	}
	if (!m_pPlotParam->m_axisY.paDataArray.empty() && invertation)
	{
		if (pointNumber > m_pPlotParam->m_axisY.paDataArray.size() - 1)
			number = m_pPlotParam->m_axisY.paDataArray.size() - 1;
		if ((m_pPlotParam->m_axisY.paDataArray[number] >= m_pPlotParam->m_axisY.paMinCurrent))
		{
			if ((m_pPlotParam->m_axisY.paDataArray[number] <= m_pPlotParam->m_axisY.paMaxCurrent))
			{
				return -FromWorldToScreen(m_pPlotParam->m_axisY.paDataArray[number],
					m_pPlotParam->m_axisY.paMinCurrent, m_pPlotParam->m_axisY.paMaxCurrent,
					m_pPlotParam->m_axisY.paGridType, m_pPlotParam->m_axisY.paScaleType,
					m_pPlotParam->m_rect) + m_pPlotParam->m_rect.bottom;
			}
			else
			{
				return m_pPlotParam->m_rect.bottom;
			}
		}
		else
		{
			return m_pPlotParam->m_rect.top;
		}
	}
	return std::numeric_limits<double>::quiet_NaN();
}
//------------------------------------------------------------------------------
int CCustomPlot::GetFirstPoint()
{
    return m_pPlotParam->m_firstPoint;
}
//------------------------------------------------------------------------------
double CCustomPlot::GetStartValue()
{
	return m_pPlotParam->m_startValue;
}
//------------------------------------------------------------------------------
double CCustomPlot::GetEndValue()
{
    return m_pPlotParam->m_endValue;
}
//------------------------------------------------------------------------------