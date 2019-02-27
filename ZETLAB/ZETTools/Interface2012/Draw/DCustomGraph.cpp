//------------------------------------------------------------------------------
// DCustomGraph.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\Draw\DCustomGraph.h"
//------------------------------------------------------------------------------
CDCustomGraph::CDCustomGraph(CRect rect, PVOID ppGraphics,
	bool *pFlagIsChange, double *pTimeAnimation) :
CCustomDraw(rect, ppGraphics, pFlagIsChange, pTimeAnimation)
{
	m_pParamDraw = &m_param;	// обеспечение ОБЩИХ параметров
}
//------------------------------------------------------------------------------
CDCustomGraph::~CDCustomGraph()
{
}
//------------------------------------------------------------------------------
void CDCustomGraph::SetBorderHeight( int iValue )
{
	if ((iValue >= 0) && (m_param.iBorderHeight != iValue))
	{
		m_param.iBorderHeight = iValue;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDCustomGraph::SetBorderGradientColorStart( COLORREF clrValue )
{
	if (m_param.clrBorderGradientColorStart != clrValue)
	{
		m_param.clrBorderGradientColorStart = clrValue;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDCustomGraph::SetBorderGradientColorEnd( COLORREF clrValue )
{
	if (m_param.clrBorderGradientColorEnd != clrValue)
	{
		m_param.clrBorderGradientColorEnd = clrValue;
		IsChange();
	}
}
//------------------------------------------------------------------------------