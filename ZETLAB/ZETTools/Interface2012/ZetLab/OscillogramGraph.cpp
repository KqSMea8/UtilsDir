//------------------------------------------------------------------------------
// OscillogramGraph.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\ZetLab\OscillogramGraph.h"
#include "C:\ZETTools\Dialog_ZET\Dialog_ZET.h"
//------------------------------------------------------------------------------
COscillogramGraph::COscillogramGraph(
	CCustomContainer *owner, CRect rect)
	: CCustomElement(owner)
{
	m_VerticalCursorPosition = NULL;
	m_HorizontalCursorPosition = NULL;
	m_GraphPosition = NULL;
	m_pDOscillogramGraph = NULL;
	if (m_pOwner)
	{
		m_pDOscillogramGraph = new CDOscillogramGraph(rect, m_pOwner->GetPointerToGraphics(),
			m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime() );
		m_pDClass = m_pDOscillogramGraph;
	}
	SetCursorFromFile(L"Main.cur");
	SetClassLong(GetParent()->m_hWnd, GCL_HCURSOR, (long)m_hCursor);
}
//------------------------------------------------------------------------------
COscillogramGraph::~COscillogramGraph()
{
	if (m_pDOscillogramGraph)
		delete m_pDOscillogramGraph;
}
//------------------------------------------------------------------------------
void COscillogramGraph::OnMouseWheel( UINT nFlags, CPoint point, short delta )
{
	CCustomElement::OnMouseWheel(nFlags, point, delta);

	if (m_pDOscillogramGraph->GetEvent(teMouseMove))
	{
		CPoint pointMove;
		m_pDOscillogramGraph->GetEventParameters(teMouseMove, (PVOID)(&pointMove));
		CRect rectWindow;
		GetParent()->GetWindowRect(&rectWindow);
		SetCursorPos(pointMove.x + rectWindow.left, pointMove.y + rectWindow.top);
	}
	if (m_pDOscillogramGraph->GetEvent(teChangeVerticalCursorPosition))
	{
		Position VerticalCursorPosition;
		m_pDOscillogramGraph->GetEventParameters(teChangeVerticalCursorPosition, (PVOID)(&VerticalCursorPosition));
		NewVerticalCursorPosition(VerticalCursorPosition.lIndex, VerticalCursorPosition.dPosition);
	}
	if (m_pDOscillogramGraph->GetEvent(teChangeHorizontalCursorPosition))
	{
		Position HorizontalCursorPosition;
		m_pDOscillogramGraph->GetEventParameters(teChangeHorizontalCursorPosition, (PVOID)(&HorizontalCursorPosition));
		NewHorizontalCursorPosition(HorizontalCursorPosition.lIndex, HorizontalCursorPosition.dPosition);
	}
	if (m_pDOscillogramGraph->GetEvent(teChangeGraphPosition))
	{
		Position GraphPosition;
		m_pDOscillogramGraph->GetEventParameters(teChangeGraphPosition, (PVOID)(&GraphPosition));
		NewGraphPosition(GraphPosition.lIndex, GraphPosition.dPosition);
	}
}
//------------------------------------------------------------------------------
void COscillogramGraph::OnMouseMove( UINT nFlags, CPoint point )
{
	CCustomElement::OnMouseMove(nFlags, point);
	if (m_pDOscillogramGraph->GetEvent(teChangeVerticalCursorPosition))
	{
		Position VerticalCursorPosition;
		m_pDOscillogramGraph->GetEventParameters(teChangeVerticalCursorPosition, (PVOID)(&VerticalCursorPosition));
		NewVerticalCursorPosition(VerticalCursorPosition.lIndex, VerticalCursorPosition.dPosition);
	}
	if (m_pDOscillogramGraph->GetEvent(teChangeHorizontalCursorPosition))
	{
		Position HorizontalCursorPosition;
		m_pDOscillogramGraph->GetEventParameters(teChangeHorizontalCursorPosition, (PVOID)(&HorizontalCursorPosition));
		NewHorizontalCursorPosition(HorizontalCursorPosition.lIndex, HorizontalCursorPosition.dPosition);
	}
	if (m_pDOscillogramGraph->GetEvent(teChangeGraphPosition))
	{
		Position GraphPosition;
		m_pDOscillogramGraph->GetEventParameters(teChangeGraphPosition, (PVOID)(&GraphPosition));
		NewGraphPosition(GraphPosition.lIndex, GraphPosition.dPosition);
	}
	if (m_pDOscillogramGraph->GetEvent(teChangeCursor))
	{
		TypeMouseCursor tmcMouseCursor;
		m_pDOscillogramGraph->GetEventParameters(teChangeCursor, (PVOID)(&tmcMouseCursor));
		switch(tmcMouseCursor)
		{
			case tmcNormal:
				SetCursorFromFile(L"Main.cur");
			break;
			case tmcHorizontalMoving:
				SetCursorFromFile(L"MoveH.cur");
			break;
			case tmcVerticalMoving:
				SetCursorFromFile(L"MoveV.cur");
			break;
		}
		SetClassLong(GetParent()->m_hWnd, GCL_HCURSOR, (long)m_hCursor);
	}
}
//------------------------------------------------------------------------------
void COscillogramGraph::NewVerticalCursorPosition( long lValue, double dValue )
{
	CWnd *pParent = GetParent();
	if (m_VerticalCursorPosition && pParent)
		(((CDialog_ZETi12*)pParent)->*m_VerticalCursorPosition)(lValue, dValue);
}
//------------------------------------------------------------------------------
void COscillogramGraph::NewHorizontalCursorPosition( long lValue, double dValue )
{
	CWnd *pParent = GetParent();
	if (m_HorizontalCursorPosition && pParent)
		(((CDialog_ZETi12*)pParent)->*m_HorizontalCursorPosition)(lValue, dValue);
}
//------------------------------------------------------------------------------
void COscillogramGraph::NewGraphPosition( long lValue, double dValue )
{
	CWnd *pParent = GetParent();
	if (m_GraphPosition && pParent)
		(((CDialog_ZETi12*)pParent)->*m_GraphPosition)(lValue, dValue);
}
//------------------------------------------------------------------------------
