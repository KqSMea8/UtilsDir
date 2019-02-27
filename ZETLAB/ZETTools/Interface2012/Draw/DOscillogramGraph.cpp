//------------------------------------------------------------------------------
// DOscillogramGraph.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "Interface2012\Draw\DOscillogramGraph.h"
#pragma comment(lib, "C:\\ZETLibs\\Intel\\ipp\\ia32\\ZetIpp.lib")
#include "Intel\ipp\ipps.h"
#include "Intel\ipp\ippcore.h"
//------------------------------------------------------------------------------
int g_nOscillogramGraph = 0;		// счётчик графиков осциллограмм
//------------------------------------------------------------------------------
CDOscillogramGraph::CDOscillogramGraph(CRect rect, PVOID ppGraphics,
				bool *pFlagIsChange, double *pTimeAnimation) :
				CDCustomGraph(rect, ppGraphics, pFlagIsChange, pTimeAnimation)
{
	m_pParamDraw = &m_param;	// обеспечение ОБЩИХ параметров
	// изменение значений общих параметров, заданных по умолчанию
	m_param.nElement = ++g_nOscillogramGraph;
	SetNameWithNumber(L"OscillogramGraph");

	for (int i = 0; i < MAX_VERTICAL_CURSOR_COUNT; ++i)
		m_iVerticalCursorStatus[i] = 0;
	for (int i = 0; i < MAX_HORIZONTAL_CURSOR_COUNT; ++i)
		m_iHorizontalCursorStatus[i] = 0;
	for (int i = 0; i < MAX_GRAPH_QUANTITY; ++i)
		m_iGraphIsSelected[i] = 0;
	m_pData = new std::vector<float>[m_param.iGraphQuantity];
	m_bNeedCheckAllSelectedElements = false;
	m_Event.bMouseMove = false;
	m_Event.MouseMovePoint = CPoint(0, 0);
	m_Event.bVerticalCursorPosition = false;
	m_Event.VerticalCursorPosition.lIndex = 0;
	m_Event.VerticalCursorPosition.dPosition = 0.;
	m_Event.bHorizontalCursorPosition = false;
	m_Event.HorizontalCursorPosition.lIndex = 0;
	m_Event.HorizontalCursorPosition.dPosition = 0.;
	m_Event.bGraphPosition = false;
	m_Event.GraphPosition.lIndex = 0;
	m_Event.GraphPosition.dPosition = 0.;
	m_Event.bMouseCursor = false;
	m_Event.tmcCursor = tmcNormal;

	ippInit();
}
//------------------------------------------------------------------------------
CDOscillogramGraph::~CDOscillogramGraph()
{
	for (int i = 0; i < m_param.iGraphQuantity; ++i)
		m_pData[i].clear();
	delete[] m_pData;
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::OnDraw()
{
	if (m_param.bVisible)
	{
		Graphics *pGraphics = *((Graphics**)m_ppGraphics);
		
		DrawPanel(pGraphics);
		DrawGrid(pGraphics);
		DrawGraphics(pGraphics);
		DrawVerticalCursors(pGraphics);
		DrawHorizontalCursors(pGraphics);
	}
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::DrawPanel(Graphics *pGraphics)
{
	//рисование панели
	CRect rTempRect(m_rect);
	if (m_param.iBorderHeight > 0)
	{
		FillRectLinearGradient(pGraphics, 
			&m_rect,
			m_param.clrBorderGradientColorStart,
			m_param.clrBorderGradientColorEnd,
			LinearGradientModeForwardDiagonal);
		DefineRect2(&rTempRect, &rTempRect, m_param.iBorderHeight);
	}

	if (m_param.iFrameHeight > 0)
	{
		FillRectSolid(pGraphics, &rTempRect, RGB(216, 212, 213));	
		rTempRect.left += m_param.iFrameHeight;
		rTempRect.top += m_param.iFrameHeight;
	}

	if (m_param.iFrameBorderHeight > 0)
	{
		FillRectLinearGradient(pGraphics, 
			&rTempRect,
			m_param.clrFrameBorderGradientColorStart,
			m_param.clrFrameBorderGradientColorEnd,
			LinearGradientModeForwardDiagonal);
		rTempRect.left += m_param.iFrameBorderHeight;
		rTempRect.top += m_param.iFrameBorderHeight;
	}

	FillRectSolid(pGraphics, &rTempRect, m_param.clrBackColor);	
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::DrawGrid(Graphics* pGraphics)
{
	//рисование сетки
	CRect rTempRect(m_rect);
	GetPanelRect(&rTempRect);
	Color colorDotGrid(100, 
		GetRValue(m_param.clrGridColor),
		GetGValue(m_param.clrGridColor), 
		GetBValue(m_param.clrGridColor));
	Color colorSolidGrid(255, 
		GetRValue(m_param.clrGridColor),
		GetGValue(m_param.clrGridColor), 
		GetBValue(m_param.clrGridColor));
	Pen penDotGrid(colorDotGrid);
	penDotGrid.SetDashStyle(Gdiplus::DashStyleDash);
	Pen penSolidGrid(colorSolidGrid, 1.5f);

	float fCoordinate(0.f);
	for (int i = 1; i < SQUARE_COUNT; ++i)
	{
		if (i != SQUARE_COUNT / 2)
		{
			fCoordinate = float(i * rTempRect.Width()) / float(SQUARE_COUNT) + rTempRect.left;
			pGraphics->DrawLine(&penDotGrid, 
				fCoordinate, 
				float(rTempRect.top), 
				fCoordinate, 
				float(rTempRect.bottom));
			fCoordinate = float(i * rTempRect.Height()) / float(SQUARE_COUNT) + rTempRect.top;
			pGraphics->DrawLine(&penDotGrid, 
				float(rTempRect.left), 
				fCoordinate, 
				float(rTempRect.right), 
				fCoordinate);
		}
		else
		{
			fCoordinate = float(i * rTempRect.Width()) / 10.f + rTempRect.left;
			pGraphics->DrawLine(&penSolidGrid, 
				fCoordinate, 
				float(rTempRect.top),
				fCoordinate, 
				float(rTempRect.bottom));
			fCoordinate = float(i * rTempRect.Height()) / 10.f + rTempRect.top;
			pGraphics->DrawLine(&penSolidGrid, 
				float(rTempRect.left), 
				fCoordinate, 
				float(rTempRect.right), 
				fCoordinate);
		}
	}
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::DrawVerticalCursors( Graphics *pGraphics )
{
	//рисование вертикальных курсоров
	CRect rTempRect(m_rect);
	GetPanelRect(&rTempRect);
	GraphicsPath gpCursor;
	CRect rectCursor;

	for (int i = 0; i < m_param.iVerticalCursorCount; ++i)
	{
		if (m_param.shVerticalCursorVisibility != 0)
		{
			GetVerticalCursorRect(i, &rectCursor);
			if (m_iVerticalCursorStatus[i] & 0x2)
				DefineRect2(&rectCursor, &rectCursor, 1);

			Color colorVerticalCursor(150,
										GetRValue(m_param.clrVerticalCursorColor[i]),
										GetGValue(m_param.clrVerticalCursorColor[i]),
										GetBValue(m_param.clrVerticalCursorColor[i]));
			if (m_iVerticalCursorStatus[i] & 0x1)
				colorVerticalCursor.SetFromCOLORREF(m_param.clrVerticalCursorColor[i]);
			COLORREF clrDarker(DarkerColor(m_param.clrVerticalCursorColor[i], 30));
			Color colorVerticalCursorDarker(150,
											GetRValue(clrDarker),
											GetGValue(clrDarker),
											GetBValue(clrDarker));
			if (m_iVerticalCursorStatus[i] & 0x1)
				colorVerticalCursorDarker.SetFromCOLORREF(clrDarker);
			COLORREF clrLighter(LighterColor(m_param.clrVerticalCursorColor[i], 30));
			Color colorVerticalCursorLighter(150,
												GetRValue(clrLighter),
												GetGValue(clrLighter),
												GetBValue(clrLighter));
			if (m_iVerticalCursorStatus[i] & 0x1)
				colorVerticalCursorLighter.SetFromCOLORREF(clrLighter);

			gpCursor.Reset();
			if (m_param.tcVerticalCursorType[i] == tcTriangle)
			{
				Point Triangle[4];
				Triangle[0] = Triangle[3] = Point(rectCursor.left + rectCursor.Width() / 2, rectCursor.bottom);
				Triangle[1] = Point(rectCursor.left, rectCursor.top);
				Triangle[2] = Point(rectCursor.right, rectCursor.top);
				gpCursor.AddClosedCurve(Triangle, 3, 0.1f);
				PathGradientBrush pgBrushCursor(&gpCursor);
				pgBrushCursor.SetCenterColor(colorVerticalCursorLighter);
				int iTemp = 1;
				pgBrushCursor.SetSurroundColors(&colorVerticalCursorDarker, &iTemp);
				pgBrushCursor.SetCenterPoint(Point(rectCursor.left + int(double(rectCursor.Width()) * 0.2),  rectCursor.top + int(double(rectCursor.Height()) * 0.2)));
				pgBrushCursor.SetFocusScales(0.2f, 0.3f);
				pGraphics->DrawPath(&Pen(&SolidBrush(colorVerticalCursorDarker)), &gpCursor);
				pGraphics->FillPath(&pgBrushCursor, &gpCursor);
			}
			else
			if(m_param.tcVerticalCursorType[i] == tcEllipse)
			{
				gpCursor.AddEllipse(float(rectCursor.left), 
									float(rectCursor.top),
									float(rectCursor.Width()),
									float(rectCursor.Height()));

				PathGradientBrush pgBrushCursor(&gpCursor);
				pgBrushCursor.SetCenterColor(colorVerticalCursorLighter);
				int iTemp = 1;
				pgBrushCursor.SetSurroundColors(&colorVerticalCursorDarker, &iTemp);
				pgBrushCursor.SetCenterPoint(Point(rectCursor.left + int(double(rectCursor.Width()) * 0.2),  rectCursor.top + int(double(rectCursor.Height()) * 0.2)));
				pgBrushCursor.SetFocusScales(0.2f, 0.2f);
				pGraphics->DrawPath(&Pen(&SolidBrush(colorVerticalCursorDarker)), &gpCursor);
				pGraphics->FillPath(&pgBrushCursor, &gpCursor);
			}
			
			Pen penVerticalCursor(colorVerticalCursor);
			pGraphics->DrawLine(&penVerticalCursor, 
								float(rectCursor.left) + float(rectCursor.Width()) / 2.f,
								float(rTempRect.top),
								float(rectCursor.left) + float(rectCursor.Width()) / 2.f,
								float(rTempRect.bottom));
		}
	}
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::DrawHorizontalCursors( Graphics *pGraphics )
{
	//рисование горизонтальных курсоров
	CRect rTempRect(m_rect);
	GetPanelRect(&rTempRect);
	GraphicsPath gpCursor;
	CRect rectCursor;
	for (int i = 0; i < m_param.iHorizontalCursorCount; ++i)
	{
		if (m_param.shHorizontalCursorVisibility != 0)
		{
			GetHorizontalCursorRect(i, &rectCursor);
			if (m_iHorizontalCursorStatus[i] & 0x2)
				DefineRect2(&rectCursor, &rectCursor, 1);
						
			Color colorHorizontalCursor(150,
										GetRValue(m_param.clrHorizontalCursorColor[i]),
										GetGValue(m_param.clrHorizontalCursorColor[i]),
										GetBValue(m_param.clrHorizontalCursorColor[i]));
			if (m_iHorizontalCursorStatus[i] & 0x1)
				colorHorizontalCursor.SetFromCOLORREF(m_param.clrHorizontalCursorColor[i]);
			COLORREF clrDarker(DarkerColor(m_param.clrHorizontalCursorColor[i], 30));
			Color colorHorizontalCursorDarker(150,
												GetRValue(clrDarker),
												GetGValue(clrDarker),
												GetBValue(clrDarker));
			if (m_iHorizontalCursorStatus[i] & 0x1)
				colorHorizontalCursorDarker.SetFromCOLORREF(clrDarker);
			COLORREF clrLighter(LighterColor(m_param.clrHorizontalCursorColor[i], 30));
			Color colorHorizontalCursorLighter(150,
												GetRValue(clrLighter),
												GetGValue(clrLighter),
												GetBValue(clrLighter));
			if (m_iHorizontalCursorStatus[i] & 0x1)
				colorHorizontalCursorLighter.SetFromCOLORREF(clrLighter);

			gpCursor.Reset();
			if (m_param.tcHorizontalCursorType[i] == tcTriangle)
			{
				Point Triangle[4];
				Triangle[0] = Triangle[3] = Point(rectCursor.left  + rectCursor.Width(), rectCursor.top + rectCursor.Height() / 2);
				Triangle[1] = Point(rectCursor.left, rectCursor.top);
				Triangle[2] = Point(rectCursor.left, rectCursor.bottom);
				gpCursor.AddCurve(Triangle, 4, 0.1f);
				
				PathGradientBrush pgBrushCursor(&gpCursor);
				pgBrushCursor.SetCenterColor(colorHorizontalCursorLighter);
				int iTemp = 1;
				pgBrushCursor.SetSurroundColors(&colorHorizontalCursorDarker, &iTemp);
				pgBrushCursor.SetCenterPoint(Point(rectCursor.left + int(double(rectCursor.Width()) * 0.2),  rectCursor.top + int(double(rectCursor.Height()) * 0.2)));
				pgBrushCursor.SetFocusScales(0.3f, 0.2f);
				pGraphics->DrawPath(&Pen(&SolidBrush(colorHorizontalCursor)), &gpCursor);
				pGraphics->FillPath(&pgBrushCursor, &gpCursor);
			}
			else
			if(m_param.tcHorizontalCursorType[i] == tcEllipse)
			{
				gpCursor.AddEllipse(float(rectCursor.left), 
									float(rectCursor.top), 
									float(rectCursor.Width()),
									float(rectCursor.Height()));
				
				PathGradientBrush pgBrushCursor(&gpCursor);
				pgBrushCursor.SetCenterColor(colorHorizontalCursorLighter);
				int iTemp = 1;
				pgBrushCursor.SetSurroundColors(&colorHorizontalCursorDarker, &iTemp);
				pgBrushCursor.SetCenterPoint(Point(rectCursor.left + int(double(rectCursor.Width()) * 0.2),  rectCursor.top + int(double(rectCursor.Height()) * 0.2)));
				pgBrushCursor.SetFocusScales(0.2f, 0.2f);
				pGraphics->DrawPath(&Pen(&SolidBrush(colorHorizontalCursor)), &gpCursor);
				pGraphics->FillPath(&pgBrushCursor, &gpCursor);
			}
			
			Pen penHorizontalCursor(colorHorizontalCursor);
			pGraphics->DrawLine(&penHorizontalCursor, 
								float(rTempRect.left),
								float(rectCursor.top) + float(rectCursor.Height()) / 2.f,
								float(rTempRect.right),
								float(rectCursor.top) + float(rectCursor.Height()) / 2.f);
		}
	}
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::DrawGraphics( Graphics* pGraphics )
{
	CRect rGraphCursorRect;
	CString sTemp;
	CZFont fontGraph(L"Arial", float(m_param.iFrameHeight - 4), FontStyleRegular, m_param.clrBackColor);
	GraphicsPath gpCursor;
	
	for (int i = 0; i < m_param.iGraphQuantity; ++i)
	{
		if (m_param.shGraphVisibility[i])
		{
			sTemp.Format(L"%d", i + 1);
			GetGraphCursorRect(i, &rGraphCursorRect);
			if (m_iGraphIsSelected[i] & 0x2)
			{
				DefineRect2(&rGraphCursorRect, &rGraphCursorRect, 1);
				fontGraph.SetSize(fontGraph.GetSize() - 1);
			}
			gpCursor.Reset();
			RoundRectInGraphicsPath(&gpCursor, &rGraphCursorRect, int(rGraphCursorRect.Width() * 0.3));
			Color colorGraph(150,
								GetRValue(m_param.clrGraphColor[i]),
								GetGValue(m_param.clrGraphColor[i]),
								GetBValue(m_param.clrGraphColor[i]));
			if (m_iGraphIsSelected[i] & 0x1)
				colorGraph.SetFromCOLORREF(m_param.clrGraphColor[i]);
			COLORREF clrDarker(DarkerColor(m_param.clrGraphColor[i], 40));
			Color colorGraphDarker(150,
									GetRValue(clrDarker),
									GetGValue(clrDarker),
									GetBValue(clrDarker));
			if (m_iGraphIsSelected[i] & 0x1)
				colorGraphDarker.SetFromCOLORREF(clrDarker);
			COLORREF clrLighter(LighterColor(m_param.clrGraphColor[i], 40));
			Color colorGraphLighter(150,
									GetRValue(clrLighter),
									GetGValue(clrLighter),
									GetBValue(clrLighter));
			if (m_iGraphIsSelected[i] & 0x1)
				colorGraphLighter.SetFromCOLORREF(clrLighter);

			PathGradientBrush pgBrushCursor(&gpCursor);
			pgBrushCursor.SetCenterColor(colorGraphLighter);
			int iTemp = 1;
			pgBrushCursor.SetSurroundColors(&colorGraphDarker, &iTemp);
			pgBrushCursor.SetCenterPoint(Point(rGraphCursorRect.left + int(double(rGraphCursorRect.Width()) * 0.2),  rGraphCursorRect.top + int(double(rGraphCursorRect.Height()) * 0.2)));
			pgBrushCursor.SetFocusScales(0.3f, 0.3f);
			pGraphics->DrawPath(&Pen(&SolidBrush(colorGraphDarker)), &gpCursor);
			pGraphics->FillPath(&pgBrushCursor, &gpCursor);
			DrawString(pGraphics,
						sTemp,
						&fontGraph,
						RectF(float(rGraphCursorRect.left), float(rGraphCursorRect.top + 1), float(rGraphCursorRect.Width()), float(rGraphCursorRect.Height())),
						StringAlignmentCenter,
						StringAlignmentCenter);
			fontGraph.SetSize(float(m_param.iFrameHeight - 4));
		}
	}

	CRect rTempRect(m_rect);
	GetPanelRect(&rTempRect);
	Region rPanelRegion(Rect(rTempRect.left, 
								rTempRect.top,
								rTempRect.Width(),
								rTempRect.Height()));
	pGraphics->SetClip(&rPanelRegion);
	int iPanelSize(rTempRect.Width());
	double dCellSize(double(rTempRect.Height()) / 10.);
	if (m_pPointData.size() != iPanelSize * 2)
		m_pPointData.resize(iPanelSize * 2);
	for (int i = 0; i < m_param.iGraphQuantity; ++i)
	{
		if (m_param.shGraphVisibility[i])
		{
			Pen penGraph(Color(150,
								GetRValue(m_param.clrGraphColor[i]),
								GetGValue(m_param.clrGraphColor[i]),
								GetBValue(m_param.clrGraphColor[i])));
			if (m_iGraphIsSelected[i] & 0x1)
				penGraph.SetColor(Color(255,
										GetRValue(m_param.clrGraphColor[i]),
										GetGValue(m_param.clrGraphColor[i]),
										GetBValue(m_param.clrGraphColor[i])));
			long lGraphSize(m_pData[i].size());
			if (iPanelSize <= lGraphSize)
			{
				long lStartIndex(0), lLength(0);
				float fMin(0.f), fMax(0.f);
				long lTopLine(0), lBottomLine(0);
				for (int j = 0; j < iPanelSize; ++j)
				{
					lStartIndex = long(double(j) / double(iPanelSize) * double(lGraphSize));
					lLength = long(double(j + 1) / double(iPanelSize) * double(lGraphSize)) - lStartIndex;
					ippsMinMax_32f(&m_pData[i][lStartIndex], lLength, &fMin, &fMax);
					lBottomLine = rTempRect.top + long(double(rTempRect.Height()) / 2. - (double(fMin) + m_param.dGraphOffset[i]) * dCellSize / m_param.dGraphScale[i]);
					lTopLine = rTempRect.top + long(double(rTempRect.Height()) / 2. - (double(fMax) + m_param.dGraphOffset[i]) * dCellSize / m_param.dGraphScale[i]);
					if (lBottomLine == lTopLine)
						lTopLine++;
					if ((lBottomLine < rTempRect.top) && (lTopLine < rTempRect.top))
					{
						lBottomLine = rTempRect.top + 1;
						lTopLine = rTempRect.top;
					}
					if ((lBottomLine > rTempRect.bottom) && (lTopLine > rTempRect.bottom))
					{
						lBottomLine = rTempRect.bottom;
						lTopLine = rTempRect.bottom - 1;
					}
					m_pPointData[2 * j] = Point(rTempRect.left + j, lBottomLine); 
					m_pPointData[2 * j + 1] = Point(rTempRect.left + j, lTopLine);
				}
				pGraphics->DrawLines(&penGraph, m_pPointData.data(), iPanelSize * 2);
			}
			else
			{
				GraphicsPath gpNodes;
				gpNodes.Reset();
				for (int j = 0; j < lGraphSize; ++j)
				{
					m_pPointData[j].X = rTempRect.left + long(double(j) / double(lGraphSize - 1) * double(iPanelSize));
					m_pPointData[j].Y = rTempRect.top + long(double(rTempRect.Height()) / 2. - double(m_pData[i][j] + m_param.dGraphOffset[i]) * dCellSize / m_param.dGraphScale[i]);
					if(m_pPointData[j].Y < rTempRect.top)
						m_pPointData[j].Y = rTempRect.top + 1;
					if(m_pPointData[j].Y > rTempRect.bottom)
						m_pPointData[j].Y = rTempRect.bottom - 1;
					gpNodes.AddEllipse(m_pPointData[j].X - 2, m_pPointData[j].Y - 2, 4, 4);
				}
				pGraphics->DrawCurve(&penGraph, m_pPointData.data(), lGraphSize, 0.5);
				if (iPanelSize / lGraphSize > 3)
					pGraphics->DrawPath(&penGraph, &gpNodes);
			}
		}
	}

	pGraphics->ResetClip();
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::OnLButtonClk( UINT nFlags, CPoint point )
{
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::OnLButtonDown( UINT nFlags, CPoint point )
{
	int iResult(-1);
	iResult = IsVerticalCursorClick(point);
	if(iResult >= 0)
	{
		m_iVerticalCursorStatus[iResult] |= 0x2;
		IsChange();
		return;
	}
	iResult = IsHorizontalCursorClick(point);
	if(iResult >= 0)
	{
		m_iHorizontalCursorStatus[iResult] |= 0x2;
		IsChange();
		return;
	}
	iResult = IsGraphClick(point);
	if(iResult >= 0)
	{
		m_iGraphIsSelected[iResult] |= 0x2;
		IsChange();
		return;
	}
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::OnLButtonUp( UINT nFlags, CPoint point )
{
	for (int i = 0; i < m_param.iVerticalCursorCount; ++i)
	{
		if (m_iVerticalCursorStatus[i] & 0x2)
			IsChange();
		m_iVerticalCursorStatus[i] &= (~0x2);
	}
	for (int i = 0; i < m_param.iHorizontalCursorCount; ++i)
	{
		if (m_iVerticalCursorStatus[i] & 0x2)
			IsChange();
		m_iHorizontalCursorStatus[i] &= (~0x2);
	}
	for (int i = 0; i < m_param.iGraphQuantity; ++i)
	{
		if (m_iGraphIsSelected[i] & 0x2)
			IsChange();
		m_iGraphIsSelected[i] &= (~0x2);
	}
	OnMouseMove(nFlags, point);
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::OnMouseMove( UINT nFlags, CPoint point )
{
	if (nFlags & MK_LBUTTON)
	{
		double dPreviousPosition;
		CRect rectPanel;
		GetPanelRect(&rectPanel);
		for (int i = 0; i < m_param.iVerticalCursorCount; ++i)
		{
			if(m_iVerticalCursorStatus[i] & 0x2)
			{
				if (point.x < rectPanel.left)
					point.x = rectPanel.left;
				if (point.x > rectPanel.right)
					point.x = rectPanel.right;
				dPreviousPosition = m_param.dVerticalCursorPosition[i];
				m_param.dVerticalCursorPosition[i] = ((point.x - rectPanel.left) - float(rectPanel.Width()) / 2.f) * SQUARE_COUNT / float(rectPanel.Width());
				CountVerticalCursorPositionByCell(&m_param.dVerticalCursorPosition[i]);
				if (dPreviousPosition != m_param.dVerticalCursorPosition[i])
				{
					m_Event.VerticalCursorPosition.lIndex = i;
					m_Event.VerticalCursorPosition.dPosition = m_param.dVerticalCursorPosition[i];
					m_Event.bVerticalCursorPosition = true;
					IsChange();
				}
				return;
			}
		}
		for (int i = 0; i < m_param.iHorizontalCursorCount; ++i)
		{
			if(m_iHorizontalCursorStatus[i] & 0x2)
			{
				if (point.y < rectPanel.top)
					point.y = rectPanel.top;
				if (point.y > rectPanel.bottom)
					point.y = rectPanel.bottom;
				dPreviousPosition = m_param.dHorizontalCursorPosition[i];
				m_param.dHorizontalCursorPosition[i] = (float(rectPanel.Height()) / 2.f - float(point.y - rectPanel.top)) * SQUARE_COUNT / float(rectPanel.Height());
				CountHorizontalCursorPositionByCell(&m_param.dHorizontalCursorPosition[i]);
				if (dPreviousPosition != m_param.dHorizontalCursorPosition[i])
				{
					m_Event.HorizontalCursorPosition.lIndex = i;
					m_Event.HorizontalCursorPosition.dPosition = m_param.dHorizontalCursorPosition[i];
					m_Event.bHorizontalCursorPosition = true;
					IsChange();
				}
				return;
			}
		}
		for (int i = 0; i < m_param.iGraphQuantity; ++i)
		{
			if(m_iGraphIsSelected[i] & 0x2)
			{
				if (point.y < rectPanel.top)
					point.y = rectPanel.top;
				if (point.y > rectPanel.bottom)
					point.y = rectPanel.bottom;
				dPreviousPosition = m_param.dGraphOffset[i];
				m_param.dGraphOffset[i] = (float(rectPanel.Height()) / 2.f - float(point.y - rectPanel.top)) * SQUARE_COUNT / float(rectPanel.Height()) * m_param.dGraphScale[i];
				CountGraphPositionByCell(&m_param.dGraphOffset[i], m_param.dGraphScale[i]);
				if (dPreviousPosition != m_param.dGraphOffset[i])
				{
					m_Event.GraphPosition.lIndex = i;
					m_Event.GraphPosition.dPosition = m_param.dGraphOffset[i];
					m_Event.bGraphPosition = true;
					IsChange();
				}
				return;
			}
		}
	}
	else
	{
		if (m_bNeedCheckAllSelectedElements)
		{
			for (int i = 0; i < m_param.iVerticalCursorCount; ++i)
			{
				if (m_iVerticalCursorStatus[i] & 0x2)
					IsChange();
				m_iVerticalCursorStatus[i] &= (~0x2);
			}
			for (int i = 0; i < m_param.iHorizontalCursorCount; ++i)
			{
				if (m_iHorizontalCursorStatus[i] & 0x2)
					IsChange();
				m_iHorizontalCursorStatus[i] &= (~0x2);
			}
			for (int i = 0; i < m_param.iGraphQuantity; ++i)
			{
				if (m_iGraphIsSelected[i] & 0x2)
					IsChange();
				m_iGraphIsSelected[i] &= (~0x2);
			}
			m_bNeedCheckAllSelectedElements = false;
		}
		//выделен ли какой-нибудь из вертикальных курсоров
		int iVerticalCursorSelected(IsAnyVerticalCursorSelected());
		//выделен ли какой-нибудь из горизонтальных курсоров
		int iHorizontalCursorSelected(IsAnyHorizontalCursorSelected());
		//выделен ли какой-нибудь из графиков
		int iGraphSelected(IsAnyGraphSelected());
		int iResult(-1);
		//пришлось ли движение мыши на какой нибудь из вертикальных курсоров
		iResult = IsVerticalCursorClick(point);
		//если мышь находится над каким-нибудь из вертикальных курсоров
		if (iResult >= 0)
		{
			//если ничего не было выделено
			if ((iVerticalCursorSelected < 0) && (iHorizontalCursorSelected < 0) && (iGraphSelected < 0))
			{
				//ставим признак нахождения мыши над вертикальным курсором
				m_iVerticalCursorStatus[iResult] |= 0x1;
				//формируем событие изменения вида курсора мыши
				IsChange();
			}
			//если был выделен какой-то курсор
			else
			{
				if(iVerticalCursorSelected < 0)
					iResult = -1;
			}
		}
		for (int i = 0; i < m_param.iVerticalCursorCount; ++i)
		{
			if (i != iResult)
			{
				if (m_iVerticalCursorStatus[i] & 0x1)
					IsChange();
				m_iVerticalCursorStatus[i] &= (~0x1);
			}
		}
		iResult = IsHorizontalCursorClick(point);
		if (iResult >= 0)
		{
			if ((iVerticalCursorSelected < 0) && (iHorizontalCursorSelected < 0) && (iGraphSelected < 0))
			{
				m_iHorizontalCursorStatus[iResult] |= 0x1;
				IsChange();
			}
			else
			{
				if (iHorizontalCursorSelected < 0)
					iResult = -1;
			}
		}
		for (int i = 0; i < m_param.iHorizontalCursorCount; ++i)
		{
			if (i != iResult)
			{
				if (m_iHorizontalCursorStatus[i] & 0x1)
					IsChange();
				m_iHorizontalCursorStatus[i] &= (~0x1);
			}
		}
		iResult = IsGraphClick(point);
		if (iResult >= 0)
		{
			if ((iVerticalCursorSelected < 0) && (iHorizontalCursorSelected < 0) && (iGraphSelected < 0))
			{
				m_iGraphIsSelected[iResult] |= 0x1;
				IsChange();
			}
			else
			{
				if (iGraphSelected < 0)
					iResult = -1;
			}
		}
		for (int i = 0; i < m_param.iGraphQuantity; ++i)
		{
			if (i != iResult)
			{
				if (m_iGraphIsSelected[i] & 0x1)
					IsChange();
				m_iGraphIsSelected[i] &= (~0x1);
			}
		}

		int iVerticalCursorSelectedAfter(IsAnyVerticalCursorSelected());
		int iHorizontalCursorSelectedAfter(IsAnyHorizontalCursorSelected());
		int iGraphSelectedAfter(IsAnyGraphSelected());

		if (((iVerticalCursorSelected < 0) && (iHorizontalCursorSelected < 0) && (iGraphSelected < 0)) && 
			((iVerticalCursorSelectedAfter >= 0) || (iHorizontalCursorSelectedAfter >= 0) || iGraphSelectedAfter >= 0))
		{
			if (iVerticalCursorSelectedAfter >= 0)
				m_Event.tmcCursor = tmcHorizontalMoving;
			else
				m_Event.tmcCursor = tmcVerticalMoving;
			m_Event.bMouseCursor = true;
		}
		if (((iVerticalCursorSelected >= 0) || (iHorizontalCursorSelected >= 0) || (iGraphSelected >= 0)) && 
			((iVerticalCursorSelectedAfter < 0) && (iHorizontalCursorSelectedAfter < 0) && iGraphSelectedAfter < 0))
		{
			m_Event.tmcCursor = tmcNormal;
			m_Event.bMouseCursor = true;
		}
	}
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::OnMouseWheel( UINT nflags, CPoint point, short delta )
{
	double dStep(0.);
	for (int i = 0; i < m_param.iVerticalCursorCount; ++i)
	{
		if(m_iVerticalCursorStatus[i] & 0x1)
		{
			dStep = GetHorizontalStep();
			if (delta > 0)
				m_param.dVerticalCursorPosition[i] += dStep;
			else
				m_param.dVerticalCursorPosition[i] -= dStep;
			
			CRect rTempRect;
			GetVerticalCursorRect(i, &rTempRect);
			m_Event.MouseMovePoint = CPoint(long(rTempRect.left + float(rTempRect.Width() / 2.f)), point.y);
			m_Event.bMouseMove = true;
			m_Event.VerticalCursorPosition.lIndex = i;
			m_Event.VerticalCursorPosition.dPosition = m_param.dVerticalCursorPosition[i];
			m_Event.bVerticalCursorPosition = true;
			IsChange();
			return;
		}
	}
	for (int i = 0; i < m_param.iHorizontalCursorCount; ++i)
	{
		if(m_iHorizontalCursorStatus[i] & 0x1)
		{
			dStep = GetVerticalStep();
			if (delta > 0)
				m_param.dHorizontalCursorPosition[i] += dStep;
			else
				m_param.dHorizontalCursorPosition[i] -= dStep;

			CRect rTempRect;
			GetHorizontalCursorRect(i, &rTempRect);
			m_Event.MouseMovePoint = CPoint(point.x, long(rTempRect.top + float(rTempRect.Height() / 2.f)));
			m_Event.bMouseMove = true;
			m_Event.HorizontalCursorPosition.lIndex = i;
			m_Event.HorizontalCursorPosition.dPosition = m_param.dHorizontalCursorPosition[i];
			m_Event.bHorizontalCursorPosition = true;
			IsChange();
			return;
		}
	}
	for (int i = 0; i < m_param.iGraphQuantity; ++i)
	{
		if(m_iGraphIsSelected[i] & 0x1)
		{
			dStep = GetVerticalStep();
			if (delta > 0)
				m_param.dGraphOffset[i] += dStep * m_param.dGraphScale[i];
			else
				m_param.dGraphOffset[i] -= dStep * m_param.dGraphScale[i];

			CRect rTempRect;
			GetGraphCursorRect(i, &rTempRect);
			m_Event.MouseMovePoint = CPoint(long(rTempRect.left + float(rTempRect.Width() / 2.f)), long(rTempRect.top + float(rTempRect.Height() / 2.f)));
			m_Event.bMouseMove = true;
			m_Event.GraphPosition.lIndex = i;
			m_Event.GraphPosition.dPosition = m_param.dGraphOffset[i];
			m_Event.bGraphPosition = true;
			IsChange();
			return;
		}
	}
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::OnMouseLeave()
{
	m_bNeedCheckAllSelectedElements = true;

	CCustomDraw::OnMouseLeave();
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::OnMouseEnter()
{
	m_bNeedCheckAllSelectedElements = true;

	CCustomDraw::OnMouseEnter();
}
//------------------------------------------------------------------------------
int CDOscillogramGraph::IsVerticalCursorClick( CPoint point )
{
	CRect rectCursor, rectLineCursor;
	int iVerticalCursorSelected(IsAnyVerticalCursorSelected());
	if (iVerticalCursorSelected >= 0 && (m_param.shVerticalCursorVisibility[iVerticalCursorSelected] != 0))
	{
		GetVerticalCursorRect(iVerticalCursorSelected, &rectCursor);
		if (PointInRect(point, &rectCursor))
			return iVerticalCursorSelected;
		GetVerticalCursorLineRect(iVerticalCursorSelected, &rectLineCursor);
		if (PointInRect(point, &rectLineCursor))
			return iVerticalCursorSelected;
	}

	for (int i = m_param.iVerticalCursorCount - 1; i >= 0; --i)
	{
		if (m_param.shVerticalCursorVisibility[i] != 0)
		{
			GetVerticalCursorRect(i, &rectCursor);
			if (PointInRect(point, &rectCursor))
				return i;
			
			GetVerticalCursorLineRect(i, &rectLineCursor);
			if (PointInRect(point, &rectLineCursor))
				return i;
		}
	}
	return -1;
}
//------------------------------------------------------------------------------
int CDOscillogramGraph::IsHorizontalCursorClick( CPoint point )
{
	CRect rectCursor, rectLineCursor;
	int iHorizontalCursorSelected(IsAnyVerticalCursorSelected());
	if ((iHorizontalCursorSelected >= 0) && (m_param.shHorizontalCursorVisibility[iHorizontalCursorSelected] != 0))
	{
		GetHorizontalCursorRect(iHorizontalCursorSelected, &rectCursor);
		if (PointInRect(point, &rectCursor))
			return iHorizontalCursorSelected;
		GetHorizontalCursorLineRect(iHorizontalCursorSelected, &rectLineCursor);
		if (PointInRect(point, &rectLineCursor))
			return iHorizontalCursorSelected;
	}
	for (int i = m_param.iHorizontalCursorCount - 1; i >= 0; --i)
	{
		if (m_param.shHorizontalCursorVisibility[i] != 0)
		{
			GetHorizontalCursorRect(i, &rectCursor);
			if (PointInRect(point, &rectCursor))
				return i;
			GetHorizontalCursorLineRect(i, &rectLineCursor);
			if (PointInRect(point, &rectLineCursor))
				return i;
		}
	}
	return -1;
}
//------------------------------------------------------------------------------
int CDOscillogramGraph::IsGraphClick( CPoint point )
{
	CRect rectCursor;
	int iGraphSelected(IsAnyGraphSelected());
	if ((iGraphSelected >= 0) && (m_param.shGraphVisibility[iGraphSelected] != 0))
	{
		GetGraphCursorRect(iGraphSelected, &rectCursor);
		if (PointInRect(point, &rectCursor))
			return iGraphSelected;
	}

	for (int i = m_param.iGraphQuantity - 1; i >= 0; --i)
	{
		if (m_param.shGraphVisibility[i] != 0)
		{
			GetGraphCursorRect(i, &rectCursor);
			if (PointInRect(point, &rectCursor))
			{
				return i;
			}
		}
	}
	return -1;
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::SetFrameHeight( int iValue )
{
	if ((iValue >= 0) && (m_param.iFrameHeight != iValue))
	{
		m_param.iFrameHeight = iValue;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::SetFrameColor( COLORREF clrValue )
{
	if (m_param.clrFrameColor != clrValue)
	{
		m_param.clrFrameColor = clrValue;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::SetFrameBorderHeight( int iValue )
{
	if ((iValue >= 0) && (m_param.iFrameBorderHeight != iValue))
	{
		m_param.iFrameBorderHeight = iValue;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::SetFrameBorderGradientColorStart( COLORREF clrValue )
{
	if (m_param.clrFrameBorderGradientColorStart != clrValue)
	{
		m_param.clrFrameBorderGradientColorStart = clrValue;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::SetFrameBorderGradientColorEnd( COLORREF clrValue )
{
	if (m_param.clrFrameBorderGradientColorEnd != clrValue)
	{
		m_param.clrFrameBorderGradientColorEnd = clrValue;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::SetBackColor( COLORREF clrValue )
{
	if (m_param.clrBackColor != clrValue)
	{
		m_param.clrBackColor = clrValue;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::SetGridColor( COLORREF clrValue )
{
	if (m_param.clrGridColor != clrValue)
	{
		m_param.clrGridColor = clrValue;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::SetVerticalCursorCount( int iValue )
{
	if ((iValue >= 0) && (iValue <= MAX_VERTICAL_CURSOR_COUNT))
	{
		if (m_param.iVerticalCursorCount != iValue)
		{
			m_param.iVerticalCursorCount = iValue;
			IsChange();
		}
	}
}
//------------------------------------------------------------------------------
COLORREF CDOscillogramGraph::GetVerticalCursorColor( int iIndex )
{
	if ((iIndex >= 0) && (iIndex < MAX_VERTICAL_CURSOR_COUNT)) 
		return m_param.clrVerticalCursorColor[iIndex];
	else
		return 0;
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::SetVerticalCursorColor( int iIndex, COLORREF clrValue )
{
	if ((iIndex >= 0) && (iIndex < MAX_VERTICAL_CURSOR_COUNT)) 
	{
		if (m_param.clrVerticalCursorColor[iIndex] != clrValue)
		{
			m_param.clrVerticalCursorColor[iIndex] = clrValue;
			if (m_param.shVerticalCursorVisibility[iIndex] != 0)
				IsChange();
		}
	}
}
//------------------------------------------------------------------------------
short CDOscillogramGraph::GetVerticalCursorVisibility( int iIndex )
{
	if ((iIndex >= 0) && (iIndex < MAX_VERTICAL_CURSOR_COUNT)) 
		return m_param.shVerticalCursorVisibility[iIndex];
	else
		return 0;
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::SetVerticalCursorVisibility( int iIndex, short shValue )
{
	if ((iIndex >= 0) && (iIndex < MAX_VERTICAL_CURSOR_COUNT)) 
	{
		if (m_param.shVerticalCursorVisibility[iIndex] != shValue)
		{
			m_param.shVerticalCursorVisibility[iIndex] = shValue;
			IsChange();
		}
	}
}
//------------------------------------------------------------------------------
double CDOscillogramGraph::GetVerticalCursorPosition( int iIndex )
{
	if ((iIndex >= 0) && (iIndex < MAX_VERTICAL_CURSOR_COUNT)) 
		return m_param.dVerticalCursorPosition[iIndex];
	else
		return 0;
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::SetVerticalCursorPosition( int iIndex, double dValue )
{
	if ((iIndex >= 0) && (iIndex < MAX_VERTICAL_CURSOR_COUNT)) 
	{
		CountVerticalCursorPositionByCell(&dValue);
		if (m_param.dVerticalCursorPosition[iIndex] != dValue)
		{
			m_param.dVerticalCursorPosition[iIndex] = dValue;
			if (m_param.shVerticalCursorVisibility[iIndex] != 0)
				IsChange();
		}
	}
}
//------------------------------------------------------------------------------
TypeCursor CDOscillogramGraph::GetVerticalCursorType( int iIndex )
{
	if ((iIndex >= 0) && (iIndex < MAX_VERTICAL_CURSOR_COUNT)) 
		return m_param.tcVerticalCursorType[iIndex];
	else
		return TypeCursor(-1);
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::SetVerticalCursorType( int iIndex, TypeCursor tcValue )
{
	if ((iIndex >= 0) && (iIndex < MAX_VERTICAL_CURSOR_COUNT)) 
	{
		if (m_param.tcVerticalCursorType[iIndex] != tcValue)
		{
			m_param.tcVerticalCursorType[iIndex] = tcValue;
			if (m_param.shVerticalCursorVisibility[iIndex] != 0)
				IsChange();
		}
	}
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::SetHorizontalCursorCount( int iValue )
{
	if ((iValue >= 0) && (iValue <= MAX_HORIZONTAL_CURSOR_COUNT))
	{
		if (m_param.iHorizontalCursorCount != iValue)
		{
			m_param.iHorizontalCursorCount = iValue;
			IsChange();
		}
	}
}
//------------------------------------------------------------------------------
COLORREF CDOscillogramGraph::GetHorizontalCursorColor( int iIndex )
{
	if ((iIndex >= 0) && (iIndex < MAX_VERTICAL_CURSOR_COUNT)) 
		return m_param.clrHorizontalCursorColor[iIndex];
	else
		return 0;
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::SetHorizontalCursorColor( int iIndex, COLORREF clrValue )
{
	if ((iIndex >= 0) && (iIndex < MAX_HORIZONTAL_CURSOR_COUNT)) 
	{
		if (m_param.clrHorizontalCursorColor[iIndex] != clrValue)
		{
			m_param.clrHorizontalCursorColor[iIndex] = clrValue;
			if (m_param.shHorizontalCursorVisibility[iIndex] != 0)
				IsChange();
		}
	}
}
//------------------------------------------------------------------------------
short CDOscillogramGraph::GetHorizontalCursorVisibility( int iIndex )
{
	if ((iIndex >= 0) && (iIndex < MAX_HORIZONTAL_CURSOR_COUNT)) 
		return m_param.shHorizontalCursorVisibility[iIndex];
	else
		return 0;
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::SetHorizontalCursorVisibility( int iIndex, short shValue )
{
	if ((iIndex >= 0) && (iIndex < MAX_HORIZONTAL_CURSOR_COUNT)) 
	{
		if (m_param.shHorizontalCursorVisibility[iIndex] != shValue)
		{
			m_param.shHorizontalCursorVisibility[iIndex] = shValue;
			IsChange();
		}
	}
}
//------------------------------------------------------------------------------
double CDOscillogramGraph::GetHorizontalCursorPosition( int iIndex )
{
	if ((iIndex >= 0) && (iIndex < MAX_HORIZONTAL_CURSOR_COUNT)) 
		return m_param.dHorizontalCursorPosition[iIndex];
	else
		return 0;
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::SetHorizontalCursorPosition( int iIndex, double dValue )
{
	if ((iIndex >= 0) && (iIndex < MAX_HORIZONTAL_CURSOR_COUNT)) 
	{
		CountHorizontalCursorPositionByCell(&dValue);
		if (m_param.dHorizontalCursorPosition[iIndex] != dValue)
		{
			m_param.dHorizontalCursorPosition[iIndex] = dValue;
			if (m_param.shHorizontalCursorVisibility[iIndex] != 0)
				IsChange();
		}
	}
}
//------------------------------------------------------------------------------
TypeCursor CDOscillogramGraph::GetHorizontalCursorType( int iIndex )
{
	if ((iIndex >= 0) && (iIndex < MAX_HORIZONTAL_CURSOR_COUNT)) 
		return m_param.tcHorizontalCursorType[iIndex];
	else
		return TypeCursor(-1);
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::SetHorizontalCursorType( int iIndex, TypeCursor tcValue )
{
	if ((iIndex >= 0) && (iIndex < MAX_HORIZONTAL_CURSOR_COUNT)) 
	{
		if (m_param.tcHorizontalCursorType[iIndex] != tcValue)
		{
			m_param.tcHorizontalCursorType[iIndex] = tcValue;
			if (m_param.shHorizontalCursorVisibility[iIndex] != 0)
				IsChange();
		}
	}
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::SetGraphQuantity( int iValue )
{
	if ((iValue >= 0) && (iValue < MAX_GRAPH_QUANTITY)) 
	{
		m_param.iGraphQuantity = iValue;
		delete[]m_pData;
		m_pData = new std::vector<float>[m_param.iGraphQuantity];
		CRect rectPanel;
		GetPanelRect(&rectPanel);
		for (int i = 0; i < m_param.iGraphQuantity; ++i)
			m_pData[i].resize(rectPanel.Width());
	}
}
//------------------------------------------------------------------------------
COLORREF CDOscillogramGraph::GetGraphColor( int iIndex )
{
	if ((iIndex >= 0) && (iIndex < MAX_GRAPH_QUANTITY)) 
		return m_param.clrGraphColor[iIndex];
	else
		return 0;
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::SetGraphColor( int iIndex, COLORREF clrValue )
{
	if ((iIndex >= 0) && (iIndex < MAX_GRAPH_QUANTITY)) 
	{
		if (m_param.clrGraphColor[iIndex] != clrValue)
		{
			m_param.clrGraphColor[iIndex] = clrValue;
			if (m_param.shGraphVisibility[iIndex] != 0)
				IsChange();
		}
	}
}
//------------------------------------------------------------------------------
short CDOscillogramGraph::GetGraphVisibility( int iIndex )
{
	if ((iIndex >= 0) && (iIndex < MAX_GRAPH_QUANTITY)) 
		return m_param.shGraphVisibility[iIndex];
	else
		return 0;
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::SetGraphVisibility( int iIndex, short shValue )
{
	if ((iIndex >= 0) && (iIndex < MAX_GRAPH_QUANTITY)) 
	{
		if (m_param.shGraphVisibility[iIndex] != shValue)
		{
			m_param.shGraphVisibility[iIndex] = shValue;
			IsChange();
		}
	}
}
//------------------------------------------------------------------------------
double CDOscillogramGraph::GetGraphScale( int iIndex )
{
	if ((iIndex >= 0) && (iIndex < MAX_GRAPH_QUANTITY)) 
		return m_param.dGraphScale[iIndex];
	else
		return 0.;
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::SetGraphScale( int iIndex, double dValue )
{
	if ((iIndex >= 0) && (iIndex < MAX_GRAPH_QUANTITY)) 
	{
		if (m_param.dGraphScale[iIndex] != dValue)
		{
			m_param.dGraphScale[iIndex] = dValue;
			if (m_param.shGraphVisibility[iIndex] != 0)
				IsChange();
		}
	}
}
//------------------------------------------------------------------------------
double CDOscillogramGraph::GetGraphOffset( int iIndex )
{
	if ((iIndex >= 0) && (iIndex < MAX_GRAPH_QUANTITY)) 
		return m_param.dGraphOffset[iIndex];
	else
		return 0.;
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::SetGraphOffset( int iIndex, double dValue )
{
	if ((iIndex >= 0) && (iIndex < MAX_GRAPH_QUANTITY)) 
	{
		if (m_param.dGraphOffset[iIndex] != dValue)
		{
			m_param.dGraphOffset[iIndex] = dValue;
			if (m_param.shGraphVisibility[iIndex] != 0)
				IsChange();
		}
	}
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::PutData( int iIndex, float* pData, long lSize )
{
	if ((iIndex >= 0) && (iIndex < m_param.iGraphQuantity)) 
	{
		if(m_pData[iIndex].size() != lSize)
			m_pData[iIndex].resize(lSize);
		ippsCopy_32f(pData, m_pData[iIndex].data(), lSize);
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::GetHorizontalPanelRect( CRect* rRect )
{
	(*rRect).left = m_rect.left + m_param.iBorderHeight + m_param.iFrameHeight + m_param.iFrameBorderHeight;
	(*rRect).right = m_rect.right - m_param.iBorderHeight;
	(*rRect).top = m_rect.top + m_param.iBorderHeight;
	(*rRect).bottom = m_rect.top + m_param.iBorderHeight + m_param.iFrameHeight;
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::GetVerticalPanelRect( CRect* rRect )
{
	(*rRect).left = m_rect.left + m_param.iBorderHeight;
	(*rRect).right = m_rect.left + m_param.iBorderHeight + m_param.iFrameHeight;
	(*rRect).top = m_rect.top + m_param.iBorderHeight + m_param.iFrameHeight + m_param.iFrameBorderHeight;
	(*rRect).bottom = m_rect.bottom - m_param.iBorderHeight;
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::GetVerticalCursorRect( int iIndex, CRect* rRect )
{
	CRect rectHorizontalPanel;
	GetHorizontalPanelRect(&rectHorizontalPanel);
	(*rRect) = rectHorizontalPanel;
	(*rRect).left = long(double(rectHorizontalPanel.left) + double(rectHorizontalPanel.Width()) / 2. + 
					double(rectHorizontalPanel.Width()) / double(SQUARE_COUNT) * 
					m_param.dVerticalCursorPosition[iIndex] - double(m_param.iFrameHeight) / 2.);
	(*rRect).right =	long(double(rectHorizontalPanel.left) + double(rectHorizontalPanel.Width()) / 2. + 
						double(rectHorizontalPanel.Width()) / double(SQUARE_COUNT) * 
						m_param.dVerticalCursorPosition[iIndex] + double(m_param.iFrameHeight) / 2.);
	if ((*rRect).right > rectHorizontalPanel.right + rectHorizontalPanel.Height() / 2)
	{
		(*rRect).left = long(double(rectHorizontalPanel.right) - double(rectHorizontalPanel.Height()) / 2.);
		(*rRect).right = (*rRect).left + rectHorizontalPanel.Height();
	}
	if ((*rRect).left < rectHorizontalPanel.left - rectHorizontalPanel.Height() / 2)
	{
		(*rRect).left = long(double(rectHorizontalPanel.left) - double(rectHorizontalPanel.Height()) / 2.);
		(*rRect).right = (*rRect).left + rectHorizontalPanel.Height();
	}
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::GetVerticalCursorLineRect( int iIndex, CRect* rRect )
{
	CRect rectPanel;
	GetPanelRect(&rectPanel);
	CRect rectHorizontalPanel;
	GetHorizontalPanelRect(&rectHorizontalPanel);
	(*rRect) = rectPanel;
	(*rRect).left = long(double(rectHorizontalPanel.left) + double(rectHorizontalPanel.Width()) / 2. + 
					double(rectHorizontalPanel.Width()) / double(SQUARE_COUNT) * 
					m_param.dVerticalCursorPosition[iIndex] - double(m_param.iFrameHeight) / 2.);
	(*rRect).right =	long(double(rectHorizontalPanel.left) + double(rectHorizontalPanel.Width()) / 2. + 
						double(rectHorizontalPanel.Width()) / double(SQUARE_COUNT) * 
						m_param.dVerticalCursorPosition[iIndex] + double(m_param.iFrameHeight) / 2.);
	if ((*rRect).right > rectPanel.right)
	{
		(*rRect).right = rectPanel.right;
		(*rRect).left = (*rRect).right - rectHorizontalPanel.Height() / 2;
	}
	if ((*rRect).left < rectPanel.left)
	{
		(*rRect).left = rectPanel.left;
		(*rRect).right = (*rRect).left + rectHorizontalPanel.Height() / 2;
	}
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::GetHorizontalCursorRect( int iIndex, CRect* rRect )
{
	CRect rectVerticalPanel;
	GetVerticalPanelRect(&rectVerticalPanel);
	(*rRect) = rectVerticalPanel;
	(*rRect).top =	long(double(rectVerticalPanel.top) + double(rectVerticalPanel.Height()) / 2. - 
					double(rectVerticalPanel.Height()) / double(SQUARE_COUNT) * m_param.dHorizontalCursorPosition[iIndex] - 
					double(m_param.iFrameHeight) / 2.);
	(*rRect).bottom =	long(double(rectVerticalPanel.top) + double(rectVerticalPanel.Height()) / 2. - 
						double(rectVerticalPanel.Height()) / double(SQUARE_COUNT) * m_param.dHorizontalCursorPosition[iIndex] + 
						double(m_param.iFrameHeight) / 2.);
	if ((*rRect).bottom > rectVerticalPanel.bottom + rectVerticalPanel.Width() / 2)
	{
		(*rRect).top = long(double(rectVerticalPanel.bottom) - double(rectVerticalPanel.Width()) / 2.);
		(*rRect).bottom = (*rRect).top + rectVerticalPanel.Width();
	}
	if ((*rRect).top < rectVerticalPanel.top - rectVerticalPanel.Width() / 2)
	{
		(*rRect).top = long(double(rectVerticalPanel.top) - double(rectVerticalPanel.Width()) / 2.);
		(*rRect).bottom = (*rRect).top + rectVerticalPanel.Width();
	}
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::GetHorizontalCursorLineRect( int iIndex, CRect* rRect )
{
	CRect rectPanel;
	GetPanelRect(&rectPanel);
	CRect rectVerticalPanel;
	GetVerticalPanelRect(&rectVerticalPanel);
	(*rRect) = rectPanel;
	(*rRect).top =	long(double(rectVerticalPanel.top) + double(rectVerticalPanel.Height()) / 2. - 
		double(rectVerticalPanel.Height()) / double(SQUARE_COUNT) * m_param.dHorizontalCursorPosition[iIndex] - 
		double(m_param.iFrameHeight) / 2.);
	(*rRect).bottom =	long(double(rectVerticalPanel.top) + double(rectVerticalPanel.Height()) / 2. - 
		double(rectVerticalPanel.Height()) / double(SQUARE_COUNT) * m_param.dHorizontalCursorPosition[iIndex] + 
		double(m_param.iFrameHeight) / 2.);
	if ((*rRect).bottom > rectPanel.bottom)
	{
		(*rRect).bottom = rectPanel.bottom;
		(*rRect).top = (*rRect).bottom - rectVerticalPanel.Width() / 2;
	}
	if ((*rRect).top < rectPanel.top)
	{
		(*rRect).top = rectPanel.top;
		(*rRect).bottom = (*rRect).top + rectVerticalPanel.Width() / 2;
	}
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::GetPanelRect( CRect* rRect )
{
	(*rRect).left = m_rect.left + m_param.iBorderHeight + m_param.iFrameHeight + m_param.iFrameBorderHeight;
	(*rRect).right = m_rect.right - m_param.iBorderHeight;
	(*rRect).top = m_rect.top + m_param.iBorderHeight + m_param.iFrameHeight + m_param.iFrameBorderHeight;
	(*rRect).bottom = m_rect.bottom - m_param.iBorderHeight;
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::GetGraphCursorRect( int iIndex, CRect* rRect )
{
	CRect rectVerticalPanel;
	GetVerticalPanelRect(&rectVerticalPanel);
	(*rRect) = rectVerticalPanel;
	(*rRect).top =	long(double(rectVerticalPanel.top) + double(rectVerticalPanel.Height()) / 2. - 
					double(rectVerticalPanel.Height()) / double(SQUARE_COUNT) * m_param.dGraphOffset[iIndex] / m_param.dGraphScale[iIndex] - 
					double(m_param.iFrameHeight) / 2.);
	(*rRect).bottom =	long(double(rectVerticalPanel.top) + double(rectVerticalPanel.Height()) / 2. - 
						double(rectVerticalPanel.Height()) / double(SQUARE_COUNT) * m_param.dGraphOffset[iIndex] / m_param.dGraphScale[iIndex] + 
						double(m_param.iFrameHeight) / 2.);
	if ((*rRect).bottom > rectVerticalPanel.bottom + rectVerticalPanel.Width() / 2)
	{
		(*rRect).top = long(double(rectVerticalPanel.bottom) - double(rectVerticalPanel.Width()) / 2.);
		(*rRect).bottom = (*rRect).top + rectVerticalPanel.Width();
	}
	if ((*rRect).top < rectVerticalPanel.top - rectVerticalPanel.Width() / 2)
	{
		(*rRect).top = long(double(rectVerticalPanel.top) - double(rectVerticalPanel.Width()) / 2.);
		(*rRect).bottom = (*rRect).top + rectVerticalPanel.Width();
	}
}
//------------------------------------------------------------------------------
int CDOscillogramGraph::IsAnyVerticalCursorSelected()
{
	for (int i = 0; i < m_param.iVerticalCursorCount; ++i)
	{
		if(m_iVerticalCursorStatus[i] != 0)
			return i;
	}
	return -1;
}
//------------------------------------------------------------------------------
int CDOscillogramGraph::IsAnyHorizontalCursorSelected()
{
	for (int i = 0; i < m_param.iHorizontalCursorCount; ++i)
	{
		if(m_iHorizontalCursorStatus[i] != 0)
			return i;
	}
	return -1;
}
//------------------------------------------------------------------------------
int CDOscillogramGraph::IsAnyGraphSelected()
{
	for (int i = 0; i < m_param.iGraphQuantity; ++i)
	{
		if(m_iGraphIsSelected[i] != 0)
			return i;
	}
	return -1;
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::CountVerticalCursorPositionByCell( double* pPosition )
{
	double dSignum(abs(*pPosition) / (*pPosition));
	if ((*pPosition) == 0.)
		dSignum = 1.;
	double dPosition = abs(*pPosition);
	double dStep(GetHorizontalStep());
	
	(*pPosition) = double(long(dPosition / dStep + 0.5)) * dStep * dSignum;
}
//------------------------------------------------------------------------------
double CDOscillogramGraph::GetHorizontalStep()
{
	CRect rectPanel;
	GetPanelRect(&rectPanel);
	double dCellWidth(double(rectPanel.Width()) / double(SQUARE_COUNT));
	double dStep(0.);
	if (dCellWidth < 4.)
		dStep = 0.5;
	else
		if (dCellWidth < 5.)
			dStep = 0.25;
		else
			if (dCellWidth < 10.)
				dStep = 0.2;
			else
				if (dCellWidth < 20.)
					dStep = 0.1;
				else
					if (dCellWidth < 40.)
						dStep = 0.05;
					else
						if (dCellWidth < 50.)
							dStep = 0.025;
						else
							if (dCellWidth < 100.)
								dStep = 0.02;
							else
								dStep = 0.01;
	return dStep;
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::CountHorizontalCursorPositionByCell( double* pPosition )
{
	double dSignum(abs(*pPosition) / (*pPosition));
	if ((*pPosition) == 0.)
		dSignum = 1.;
	double dPosition = abs(*pPosition);
	double dStep(GetVerticalStep());

	(*pPosition) = double(long(dPosition / dStep + 0.5)) * dStep * dSignum;
}
//------------------------------------------------------------------------------
double CDOscillogramGraph::GetVerticalStep()
{
	CRect rectPanel;
	GetPanelRect(&rectPanel);
	double dCellHeight(double(rectPanel.Height()) / double(SQUARE_COUNT));
	double dStep(0.);
	if (dCellHeight < 4.)
		dStep = 0.5;
	else
		if (dCellHeight < 5.)
			dStep = 0.25;
		else
			if (dCellHeight < 10.)
				dStep = 0.2;
			else
				if (dCellHeight < 20.)
					dStep = 0.1;
				else
					if (dCellHeight < 40.)
						dStep = 0.05;
					else
						if (dCellHeight < 50.)
							dStep = 0.025;
						else
							if (dCellHeight < 100.)
								dStep = 0.02;
							else
								dStep = 0.01;
	return dStep;
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::CountGraphPositionByCell( double* pPosition, double dScale )
{
	double dSignum(abs(*pPosition) / (*pPosition));
	if ((*pPosition) == 0.)
		dSignum = 1.;
	double dPosition = abs(*pPosition) / dScale;
	double dStep(GetVerticalStep());

	(*pPosition) = double(long(dPosition / dStep + 0.5)) * dStep * dScale * dSignum;
}
//------------------------------------------------------------------------------
bool CDOscillogramGraph::GetEvent( TypeEvent teTypeEvent )
{
	bool bReturn(false);
	switch (teTypeEvent)
	{
		case teMouseMove:
			bReturn = m_Event.bMouseMove;
			if (m_Event.bMouseMove)
				m_Event.bMouseMove = false;
		break;
		case teChangeVerticalCursorPosition:
			bReturn = m_Event.bVerticalCursorPosition;
			if (m_Event.bVerticalCursorPosition)
				m_Event.bVerticalCursorPosition = false;
		break;
		case teChangeHorizontalCursorPosition:
			bReturn = m_Event.bHorizontalCursorPosition;
			if (m_Event.bHorizontalCursorPosition)
				m_Event.bHorizontalCursorPosition = false;
		break;
		case teChangeGraphPosition:
			bReturn = m_Event.bGraphPosition;
			if (m_Event.bGraphPosition)
				m_Event.bGraphPosition = false;
		break;
		case teChangeCursor:
			bReturn = m_Event.bMouseCursor;
			if (m_Event.bMouseCursor)
				m_Event.bMouseCursor = false;
		break;
	}

	return bReturn;
}
//------------------------------------------------------------------------------
void CDOscillogramGraph::GetEventParameters( TypeEvent teTypeEvent, PVOID pParameters )
{
	switch (teTypeEvent)
	{
		case teMouseMove:
			memcpy_s(pParameters, sizeof(CPoint), PVOID(&m_Event.MouseMovePoint), sizeof(CPoint));
		break;
		case teChangeVerticalCursorPosition:
			memcpy_s(pParameters, sizeof(Position), PVOID(&m_Event.VerticalCursorPosition), sizeof(Position));
		break;
		case teChangeHorizontalCursorPosition:
			memcpy_s(pParameters, sizeof(Position), PVOID(&m_Event.HorizontalCursorPosition), sizeof(Position));
		break;
		case teChangeGraphPosition:
			memcpy_s(pParameters, sizeof(Position), PVOID(&m_Event.GraphPosition), sizeof(Position));
		break;
		case teChangeCursor:
			memcpy_s(pParameters, sizeof(TypeMouseCursor), PVOID(&m_Event.tmcCursor), sizeof(TypeMouseCursor));
		break;
	}
}
//------------------------------------------------------------------------------
