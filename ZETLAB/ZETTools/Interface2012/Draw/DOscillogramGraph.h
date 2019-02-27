//------------------------------------------------------------------------------
// DOscillogramGraph.h : файл заголовка (Draw Oscillogram Graph)
//------------------------------------------------------------------------------
// Класс отображения графика-осциллограммы.
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Draw\DCustomGraph.h"
#include <vector>
//------------------------------------------------------------------------------
#define SQUARE_COUNT				10
#define MAX_VERTICAL_CURSOR_COUNT	2
#define MAX_HORIZONTAL_CURSOR_COUNT	2
#define MAX_GRAPH_QUANTITY			8
//------------------------------------------------------------------------------
enum TypeCursor
{
	tcTriangle = 0,
	tcEllipse,	
};
enum TypeEvent
{
	teMouseMove = 0,
	teChangeVerticalCursorPosition,
	teChangeHorizontalCursorPosition,
	teChangeGraphPosition,
	teChangeCursor,
};
enum TypeMouseCursor
{
	tmcNormal = 0,
	tmcHorizontalMoving,
	tmcVerticalMoving,
};
//------------------------------------------------------------------------------
struct ParamOscillogramGraph : public ParamCustomGraph
{// Структура параметров отображения графика-осциллограммы
// В родительской структуре - общие параметры, здесь - частные параметры
	int iFrameHeight;
	COLORREF clrFrameColor;
	int iFrameBorderHeight;
	COLORREF clrFrameBorderGradientColorStart;
	COLORREF clrFrameBorderGradientColorEnd;
	COLORREF clrBackColor;
	COLORREF clrGridColor;
	int iVerticalCursorCount;
	COLORREF clrVerticalCursorColor[MAX_VERTICAL_CURSOR_COUNT];
	short shVerticalCursorVisibility[MAX_VERTICAL_CURSOR_COUNT];
	double dVerticalCursorPosition[MAX_VERTICAL_CURSOR_COUNT];
	TypeCursor tcVerticalCursorType[MAX_VERTICAL_CURSOR_COUNT];
	int iHorizontalCursorCount;
	COLORREF clrHorizontalCursorColor[MAX_HORIZONTAL_CURSOR_COUNT];
	short shHorizontalCursorVisibility[MAX_HORIZONTAL_CURSOR_COUNT];
	double dHorizontalCursorPosition[MAX_HORIZONTAL_CURSOR_COUNT];
	TypeCursor tcHorizontalCursorType[MAX_HORIZONTAL_CURSOR_COUNT];
	int iGraphQuantity;
	COLORREF clrGraphColor[MAX_GRAPH_QUANTITY];
	short shGraphVisibility[MAX_GRAPH_QUANTITY];
	double dGraphScale[MAX_GRAPH_QUANTITY];
	double dGraphOffset[MAX_GRAPH_QUANTITY];

	ParamOscillogramGraph()
	{
		Size = sizeof(ParamOscillogramGraph);
		iFrameHeight = 16;
		clrFrameColor = RGB(216, 212, 213);
		iFrameBorderHeight = 3;
		clrFrameBorderGradientColorStart = RGB(140, 140, 140);
		clrFrameBorderGradientColorEnd = RGB(168, 168, 168);
		clrBackColor = RGB(23, 23, 23);
		clrGridColor = RGB(230, 230, 230);
		iVerticalCursorCount = 1;
		for (int i = 0; i < MAX_VERTICAL_CURSOR_COUNT; ++i)
		{
			clrVerticalCursorColor[i] = RGB(255, 0, 0);
			shVerticalCursorVisibility[i] = 1;
			dVerticalCursorPosition[i] = 0.f;
			tcVerticalCursorType[i] = tcTriangle;
		}
		iHorizontalCursorCount = 1;
		for (int i = 0; i < MAX_HORIZONTAL_CURSOR_COUNT; ++i)
		{
			clrHorizontalCursorColor[i] = RGB(255, 0, 0);
			shHorizontalCursorVisibility[i] = 1;
			dHorizontalCursorPosition[i] = 0.f;
			tcHorizontalCursorType[i] = tcTriangle;
		}
		iGraphQuantity = 1;
		for (int i = 0; i < MAX_GRAPH_QUANTITY; ++i)
		{
			clrGraphColor[i] = RGB(255, 0, 0);
			shGraphVisibility[i] = 1;
			dGraphScale[i] = 1.;
			dGraphOffset[i] = 0.;
		}
	}
};

struct Position
{
	long lIndex;
	double dPosition;
};
//------------------------------------------------------------------------------
class CDOscillogramGraph : public CDCustomGraph
{
public:
	CDOscillogramGraph(CRect rect, PVOID ppGraphics,
				bool *pFlagIsChange, double *pTimeAnimation);
	virtual ~CDOscillogramGraph();

private:
	ParamOscillogramGraph m_param;// структура всех параметров отображения

protected:
	void GetHorizontalPanelRect(CRect* rRect);
	void GetVerticalPanelRect(CRect* rRect);
	void GetVerticalCursorRect(int iIndex, CRect* rRect);
	void GetVerticalCursorLineRect(int iIndex, CRect* rRect);
	void GetHorizontalCursorRect(int iIndex, CRect* rRect);
	void GetHorizontalCursorLineRect(int iIndex, CRect* rRect);
	void GetPanelRect(CRect* rRect);
	void GetGraphCursorRect(int iIndex, CRect* rRect);

	void DrawPanel(Graphics *pGraphics);
	void DrawGrid(Graphics *pGraphics);
	void DrawVerticalCursors(Graphics *pGraphics);
	void DrawHorizontalCursors(Graphics *pGraphics);
	void DrawGraphics(Graphics* pGraphics);

	int IsVerticalCursorClick(CPoint point);
	int IsHorizontalCursorClick(CPoint point);
	int IsGraphClick(CPoint point);

	int IsAnyVerticalCursorSelected();
	int IsAnyHorizontalCursorSelected();
	int IsAnyGraphSelected();

	void CountVerticalCursorPositionByCell(double* pPosition);
	double GetHorizontalStep();
	void CountHorizontalCursorPositionByCell(double* pPosition);
	double GetVerticalStep();
	void CountGraphPositionByCell(double* pPosition, double dScale);

	int m_iVerticalCursorStatus[MAX_VERTICAL_CURSOR_COUNT];
	int m_iHorizontalCursorStatus[MAX_VERTICAL_CURSOR_COUNT];
	int m_iGraphIsSelected[MAX_GRAPH_QUANTITY];
	std::vector<float>* m_pData;
	std::vector<Point> m_pPointData;
	bool m_bNeedCheckAllSelectedElements;
	
	struct 
	{
		bool bMouseMove;
		CPoint MouseMovePoint;
		bool bVerticalCursorPosition;
		Position VerticalCursorPosition;
		bool bHorizontalCursorPosition;
		Position HorizontalCursorPosition;
		bool bGraphPosition;
		Position GraphPosition;
		bool bMouseCursor;
		TypeMouseCursor tmcCursor;
	} m_Event;

public:
	// функции параметров отображения
	int GetFrameHeight() { return m_param.iFrameHeight; }
	void SetFrameHeight(int iValue);

	COLORREF GetFrameColor() { return m_param.clrFrameColor; }
	void SetFrameColor(COLORREF clrValue);

	int GetFrameBorderHeight() { return m_param.iFrameBorderHeight; }
	void SetFrameBorderHeight(int iValue);

	COLORREF GetFrameBorderGradientColorStart() { return m_param.clrFrameBorderGradientColorStart; }
	void SetFrameBorderGradientColorStart(COLORREF clrValue);

	COLORREF GetFrameBorderGradientColorEnd() { return m_param.clrFrameBorderGradientColorEnd; }
	void SetFrameBorderGradientColorEnd(COLORREF clrValue);

	COLORREF GetBackColor() { return m_param.clrBackColor; }
	void SetBackColor(COLORREF clrValue);

	COLORREF GetGridColor() { return m_param.clrGridColor; }
	void SetGridColor(COLORREF clrValue);

	int GetVerticalCursorCount() { return m_param.iVerticalCursorCount; }
	void SetVerticalCursorCount(int iValue);

	COLORREF GetVerticalCursorColor(int iIndex); 
	void SetVerticalCursorColor(int iIndex, COLORREF clrValue);

	short GetVerticalCursorVisibility(int iIndex);
	void SetVerticalCursorVisibility(int iIndex, short shValue);

	double GetVerticalCursorPosition(int iIndex);
	void SetVerticalCursorPosition(int iIndex, double dValue);

	TypeCursor GetVerticalCursorType(int iIndex);
	void SetVerticalCursorType(int iIndex, TypeCursor tcValue);

	int GetHorizontalCursorCount() { return m_param.iVerticalCursorCount; }
	void SetHorizontalCursorCount(int iValue);

	COLORREF GetHorizontalCursorColor(int iIndex); 
	void SetHorizontalCursorColor(int iIndex, COLORREF clrValue);

	short GetHorizontalCursorVisibility(int iIndex);
	void SetHorizontalCursorVisibility(int iIndex, short shValue);

	double GetHorizontalCursorPosition(int iIndex);
	void SetHorizontalCursorPosition(int iIndex, double dValue);

	TypeCursor GetHorizontalCursorType(int iIndex);
	void SetHorizontalCursorType(int iIndex, TypeCursor tcValue);

	int GetGraphQuantity() { return m_param.iGraphQuantity; }
	void SetGraphQuantity(int iValue);

	COLORREF GetGraphColor(int iIndex); 
	void SetGraphColor(int iIndex, COLORREF clrValue);

	short GetGraphVisibility(int iIndex);
	void SetGraphVisibility(int iIndex, short shValue);

	double GetGraphScale(int iIndex);
	void SetGraphScale(int iIndex, double dValue);

	double GetGraphOffset(int iIndex);
	void SetGraphOffset(int iIndex, double dValue);

	void PutData(int iIndex, float* pData, long lSize);
		
	bool GetEvent(TypeEvent teTypeEvent);
	void GetEventParameters(TypeEvent teTypeEvent, PVOID pParameters);

	// виртуальные функции, переопределяемые в классе
	virtual void OnDraw();
	virtual void OnLButtonClk(UINT nFlags, CPoint point);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnMouseWheel(UINT nflags, CPoint point, short delta);
	virtual void OnMouseLeave();
	virtual void OnMouseEnter();
};
//------------------------------------------------------------------------------