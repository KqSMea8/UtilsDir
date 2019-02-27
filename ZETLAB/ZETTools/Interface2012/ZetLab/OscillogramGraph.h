//------------------------------------------------------------------------------
// OscillogramGraph.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Custom\CustomElement.h"
#include "C:\ZETTools\Interface2012\Draw\DOscillogramGraph.h"
#include "C:\ZETTools\Interface2012\Custom\Dialog_ZETi12.h"
//------------------------------------------------------------------------------
typedef void (CDialog_ZETi12::*NewLongDouble)(long, double);
//------------------------------------------------------------------------------
class COscillogramGraph : public CCustomElement
{
public:
	COscillogramGraph(CCustomContainer *owner, CRect rect);
	virtual ~COscillogramGraph();

	NewLongDouble m_VerticalCursorPosition;
	NewLongDouble m_HorizontalCursorPosition;
	NewLongDouble m_GraphPosition;
	void NewVerticalCursorPosition(long lValue, double dValue);
	void NewHorizontalCursorPosition(long lValue, double dValue);
	void NewGraphPosition(long lValue, double dValue);

private:
	CDOscillogramGraph *m_pDOscillogramGraph;
	
protected:

public:
	int GetFrameHeight() { return m_pDOscillogramGraph->GetFrameHeight(); }
	void SetFrameHeight(int iValue) {m_pDOscillogramGraph->SetFrameHeight(iValue); };

	COLORREF GetFrameColor() { return m_pDOscillogramGraph->GetFrameColor(); }
	void SetFrameColor(COLORREF clrValue) {m_pDOscillogramGraph->SetFrameColor(clrValue); };

	int GetFrameBorderHeight() { return m_pDOscillogramGraph->GetFrameBorderHeight(); }
	void SetFrameBorderHeight(int iValue) {m_pDOscillogramGraph->SetFrameBorderHeight(iValue); };

	COLORREF GetFrameBorderGradientColorStart() { return m_pDOscillogramGraph->GetFrameBorderGradientColorStart(); }
	void SetFrameBorderGradientColorStart(COLORREF clrValue) {m_pDOscillogramGraph->SetFrameBorderGradientColorStart(clrValue); };

	COLORREF GetFrameBorderGradientColorEnd() { return m_pDOscillogramGraph->GetFrameBorderGradientColorEnd(); }
	void SetFrameBorderGradientColorEnd(COLORREF clrValue) {m_pDOscillogramGraph->SetFrameBorderGradientColorEnd(clrValue); };

	COLORREF GetBackColor() { return m_pDOscillogramGraph->GetBackColor(); }
	void SetBackColor(COLORREF clrValue) {m_pDOscillogramGraph->SetBackColor(clrValue); };

	COLORREF GetGridColor() { return m_pDOscillogramGraph->GetGridColor(); }
	void SetGridColor(COLORREF clrValue) {m_pDOscillogramGraph->SetGridColor(clrValue); };

	int GetVerticalCursorCount() { return m_pDOscillogramGraph->GetVerticalCursorCount(); }
	void SetVerticalCursorCount(int iValue) {m_pDOscillogramGraph->SetVerticalCursorCount(iValue); };

	COLORREF GetVerticalCursorColor(int iIndex) { return m_pDOscillogramGraph->GetVerticalCursorColor(iIndex); }
	void SetVerticalCursorColor(int iIndex, COLORREF clrValue) {m_pDOscillogramGraph->SetVerticalCursorColor(iIndex, clrValue); };

	short GetVerticalCursorVisibility(int iIndex) { return m_pDOscillogramGraph->GetVerticalCursorVisibility(iIndex); }
	void SetVerticalCursorVisibility(int iIndex, short shValue) {m_pDOscillogramGraph->SetVerticalCursorVisibility(iIndex, shValue); };

	double GetVerticalCursorPosition(int iIndex) { return m_pDOscillogramGraph->GetVerticalCursorPosition(iIndex); }
	void SetVerticalCursorPosition(int iIndex, double dValue) {m_pDOscillogramGraph->SetVerticalCursorPosition(iIndex, dValue); };

	TypeCursor GetVerticalCursorType(int iIndex) { return m_pDOscillogramGraph->GetVerticalCursorType(iIndex); }
	void SetVerticalCursorType(int iIndex, TypeCursor tcValue) {m_pDOscillogramGraph->SetVerticalCursorType(iIndex, tcValue); };

	int GetHorizontalCursorCount() { return m_pDOscillogramGraph->GetHorizontalCursorCount(); }
	void SetHorizontalCursorCount(int iValue) {m_pDOscillogramGraph->SetHorizontalCursorCount(iValue); };

	COLORREF GetHorizontalCursorColor(int iIndex) { return m_pDOscillogramGraph->GetHorizontalCursorColor(iIndex); }
	void SetHorizontalCursorColor(int iIndex, COLORREF clrValue) {m_pDOscillogramGraph->SetHorizontalCursorColor(iIndex, clrValue); };

	short GetHorizontalCursorVisibility(int iIndex) { return m_pDOscillogramGraph->GetHorizontalCursorVisibility(iIndex); }
	void SetHorizontalCursorVisibility(int iIndex, short shValue) {m_pDOscillogramGraph->SetHorizontalCursorVisibility(iIndex, shValue); };

	double GetHorizontalCursorPosition(int iIndex) { return m_pDOscillogramGraph->GetHorizontalCursorPosition(iIndex); }
	void SetHorizontalCursorPosition(int iIndex, double dValue) {m_pDOscillogramGraph->SetHorizontalCursorPosition(iIndex, dValue); };

	TypeCursor GetHorizontalCursorType(int iIndex) { return m_pDOscillogramGraph->GetHorizontalCursorType(iIndex); }
	void SetHorizontalCursorType(int iIndex, TypeCursor tcValue) {m_pDOscillogramGraph->SetHorizontalCursorType(iIndex, tcValue); };

	int GetGraphQuantity() { return m_pDOscillogramGraph->GetGraphQuantity(); }
	void SetGraphQuantity(int iValue) { m_pDOscillogramGraph->SetGraphQuantity(iValue); }

	COLORREF GetGraphColor(int iIndex) { return m_pDOscillogramGraph->GetGraphColor(iIndex); }
	void SetGraphColor(int iIndex, COLORREF clrValue) {m_pDOscillogramGraph->SetGraphColor(iIndex, clrValue); };

	short GetGraphVisibility(int iIndex) { return m_pDOscillogramGraph->GetGraphVisibility(iIndex); }
	void SetGraphVisibility(int iIndex, short shValue) {m_pDOscillogramGraph->SetGraphVisibility(iIndex, shValue); };

	double GetGraphScale(int iIndex) { return m_pDOscillogramGraph->GetGraphScale(iIndex); }
	void SetGraphScale(int iIndex, double dValue) {m_pDOscillogramGraph->SetGraphScale(iIndex, dValue); };

	double GetGraphOffset(int iIndex) { return m_pDOscillogramGraph->GetGraphOffset(iIndex); }
	void SetGraphOffset(int iIndex, double dValue) {m_pDOscillogramGraph->SetGraphOffset(iIndex, dValue); };

	void PutData(int iIndex, float* pData, long lSize){m_pDOscillogramGraph->PutData(iIndex, pData, lSize); };

	virtual void OnMouseWheel(UINT nFlags, CPoint point, short delta);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
};
//------------------------------------------------------------------------------