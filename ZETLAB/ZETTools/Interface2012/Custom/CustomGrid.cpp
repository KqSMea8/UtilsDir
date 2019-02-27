//------------------------------------------------------------------------------
// CustomGrid.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include <Interface2012\Custom\CustomGrid.h>
//------------------------------------------------------------------------------
CCustomGrid::CCustomGrid(CCustomContainer *owner) 
	: CCustomElement(owner)
	, m_pGridParam(nullptr)
{
    //m_FunctionEvent = NULL;
    //m_hCursor = LoadCursorW(NULL, IDC_CROSS/*IDC_HAND*/);
}
//------------------------------------------------------------------------------
CCustomGrid::~CCustomGrid()
{
}
//------------------------------------------------------------------------------
double CCustomGrid::FromWorldToScreen(double val, const AxisParameters &axis, const CRect &rect)
{
	double Length(0.0);
    if(axis.apGridType == gtHorizontal) Length = rect.Width();
    if(axis.apGridType == gtVertical) Length = rect.Height();

    if(axis.apScaleType==stLinear)
        return val/(axis.apMaxCurrent-axis.apMinCurrent)*(double)(Length);

	return 0;
}
//------------------------------------------------------------------------------
CPoint CCustomGrid::GetTextDimention(CDFonts &font, wchar_t *String)
{
	wchar_t str[20] = {0};
    int Counter(0), Size(0);
	double Power(0.0);
	CPoint Dimention = {0};

    for(Counter = 0, Size = wcslen(String); Counter < Size+1; Counter++){
        str[Counter] = '0';
    }
    str[Counter] = '\0';

    FTBBox bbox;
    FTPoint p1, p2;
    bbox = font.GetFont()->BBox(&(str[0]), wcslen(str), p1, p2);
    Dimention.x = (int)(bbox.Upper().Xf() - bbox.Lower().Xf())+5;

    bbox = font.GetFont()->BBox(&(str[0]), wcslen(str), p1, p2);
    Dimention.y = (bbox.Upper().Yf() - bbox.Lower().Yf());
    return Dimention;
}
//------------------------------------------------------------------------------
void CCustomGrid::SetBounds(double Begin, double End)
{
    m_pGridParam->m_axis.apBegin = Begin;
    m_pGridParam->m_axis.apEnd = End;
};
//------------------------------------------------------------------------------
void CCustomGrid::SetMinStep(double MinStep)
{
    m_pGridParam->m_axis.apMinStep = MinStep;
};
//------------------------------------------------------------------------------
void CCustomGrid::SetFit(FitType fitType)
{
    m_pGridParam->m_axis.apFitType = fitType;
};
//------------------------------------------------------------------------------
void CCustomGrid::SetAxisTitle(CString title, GridType type)
{
    m_pGridParam->m_axis.apTitle = title;
};
//------------------------------------------------------------------------------
void CCustomGrid::SetCurrentTime(double NewTime)
{
    COleDateTimeSpan delta;
    int MS(0);
    delta = COleDateTime(NewTime) - COleDateTime(m_pGridParam->m_axis.apCurrentTime);
    
    m_pGridParam->m_axis.apMinCurrent = COleDateTime(m_pGridParam->m_axis.apMinCurrent) + delta;
    m_pGridParam->m_axis.apMaxCurrent = COleDateTime(m_pGridParam->m_axis.apMaxCurrent) + delta;
    delta = COleDateTime(m_pGridParam->m_axis.apEnd) - COleDateTime(m_pGridParam->m_axis.apBegin);
    m_pGridParam->m_axis.apEnd = COleDateTime(NewTime);
    m_pGridParam->m_axis.apBegin = COleDateTime(m_pGridParam->m_axis.apEnd) - delta;

    SetAxis(m_pGridParam->m_axis.apMinCurrent, m_pGridParam->m_axis.apMaxCurrent);
    m_pGridParam->m_axis.apCurrentTime = NewTime;
}
//------------------------------------------------------------------------------
void CCustomGrid::SetAxis(double min, double max)
{
	if (max < min)
	{
		m_pGridParam->m_axis.apMinCurrent = max;
		m_pGridParam->m_axis.apMaxCurrent = min;
	}
	else
	{
		m_pGridParam->m_axis.apMinCurrent = min;
		m_pGridParam->m_axis.apMaxCurrent = max;
	}
};
//------------------------------------------------------------------------------
double CCustomGrid::GetAxisAbsMin(GridType gridType)
{
	return m_pGridParam->m_axis.apBegin;
};
//------------------------------------------------------------------------------
double CCustomGrid::GetAxisAbsMax(GridType gridType)
{
	return m_pGridParam->m_axis.apEnd;
};
//------------------------------------------------------------------------------
double CCustomGrid::GetAxisMin(GridType gridType)
{
    return m_pGridParam->m_axis.apMinCurrent;
};
//------------------------------------------------------------------------------
double CCustomGrid::GetAxisMax(GridType gridType)
{
    return m_pGridParam->m_axis.apMaxCurrent;
};
//-----------------------------------------------------------------------------
double CCustomGrid::GetStep()
{
    return m_pGridParam->m_axis.apStep;
}
//-----------------------------------------------------------------------------