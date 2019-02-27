#include "stdafx.h"
#include "MovingButton.h"

#define CONTROL_DELTA 7		// расстояние от края родителя до границ контрола

CMovingButton::CMovingButton(_In_opt_ bool GorizontalMoving /*= true*/,
							 _In_opt_ bool VerticalMoving   /*= false*/)
	: m_bGorizontalMoving	(GorizontalMoving)
	, m_bVerticalMoving		(VerticalMoving)
	, m_bMoving				(false)
	, m_ControlBounds		(NO_BOUND, NO_BOUND, NO_BOUND, NO_BOUND)
{
	m_ParrentRect.SetRectEmpty();
}

CMovingButton::~CMovingButton()
{
}

BEGIN_MESSAGE_MAP(CMovingButton, CMFCButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void CMovingButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bMoving = true;
	m_CurrentPoint = point;
	CMFCButton::OnLButtonDown(nFlags, point);
}

void CMovingButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bMoving = false;
	CMFCButton::OnLButtonUp(nFlags, point);
}

void CMovingButton::OnMouseMove(UINT nFlags, CPoint point)
{		
	if (nFlags & MK_LBUTTON)
	{
		CRect ParrentRect;
		GetParent()->GetClientRect(ParrentRect);

		// получаем местоположение кнопки
		CRect Rect;
		GetWindowRect(Rect);
		GetParent()->ScreenToClient(Rect);

		// если на кнопку переместилась мышь без нажатия на сам контрол
		if (!m_bMoving)
			OnLButtonDown(nFlags, point);
		
		
		// область масштабирования контрола
		CRect CheckRect;
		CheckRect.top	 = m_ControlBounds.top     != NO_BOUND ? m_ControlBounds.top    : m_ParrentRect.top    + CONTROL_DELTA;
		CheckRect.bottom = m_ControlBounds.bottom  != NO_BOUND ? m_ControlBounds.bottom : m_ParrentRect.bottom - CONTROL_DELTA;
		CheckRect.left	 = m_ControlBounds.left    != NO_BOUND ? m_ControlBounds.left   : m_ParrentRect.left   + CONTROL_DELTA;
		CheckRect.right  = m_ControlBounds.right   != NO_BOUND ? m_ControlBounds.right  : m_ParrentRect.right  - CONTROL_DELTA;
		// рассчитываем на сколько перемещаем кнопку
		CSize Moving(NULL, NULL);
		if (m_bGorizontalMoving)
		{
			Moving.cx = point.x - m_CurrentPoint.x;
			
			// првоеряем что не вышли за допустимые границы

			if (Rect.left  + Moving.cx < CheckRect.left)
				Moving.cx = CheckRect.left  - Rect.left;
			if (Rect.right + Moving.cx > CheckRect.right)
				Moving.cx = CheckRect.right - Rect.right;
		}
		if (m_bVerticalMoving)
		{
			Moving.cy = point.y - m_CurrentPoint.y;
			
			// првоеряем что не вышли за допустимые границы
			if (Rect.top    + Moving.cy < CheckRect.top)
				Moving.cy = CheckRect.top    - Rect.top;
			if (Rect.bottom + Moving.cy > CheckRect.bottom)
				Moving.cy = CheckRect.bottom - Rect.bottom;
		}
		
		Rect.OffsetRect(Moving);
		CMFCButton::MoveWindow(Rect);

		if (Moving.cx != 0 || Moving.cy != 0)
			GetParent()->PostMessageW(WM_BUTTON_MOVING, GetDlgCtrlID());
	}
	
	CMFCButton::OnMouseMove(nFlags, point);
}

void CMovingButton::SetMovingCursor(_In_ UINT CursorID)
{
	CMFCButton::SetMouseCursor(LoadCursor(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(CursorID)));
}

void CMovingButton::MoveControl(_In_opt_ BindingTypes BindType /*= BIND_NONE*/, 
								_In_opt_ CRect BoundRect /*= CRect(NO_BOUND, NO_BOUND, NO_BOUND, NO_BOUND)*/)
{
	m_ControlBounds = BoundRect;

	if (!GetParent()->IsWindowVisible())
		return;
	
	// основное окно могло свернуться
	CRect NewParrentRect;
	GetParent()->GetClientRect(NewParrentRect);
	if (NewParrentRect.IsRectEmpty())
		return;

	if (m_ParrentRect.IsRectEmpty())
	{
		m_ParrentRect = NewParrentRect;
		return;
	}
	
	CRect Rect;
	GetWindowRect(Rect);
	GetParent()->ScreenToClient(Rect);

	// на сколько расширился/сузился основной контрол
	CSize Moving(NewParrentRect.Width() - m_ParrentRect.Width(), 
				 NewParrentRect.Height() - m_ParrentRect.Height());

	if (Moving.cx != 0 || Moving.cy != 0)
	{
		switch (BindType)
		{
			case BIND_NONE:
				Rect.OffsetRect(Moving);
				break;
			case BIND_LEFT:
				Rect.OffsetRect(0, Moving.cy);
				Rect.right += Moving.cx;
				break;
			case BIND_TOP:
				Rect.OffsetRect(Moving.cx, 0);
				Rect.bottom += Moving.cy;
				break;
			case BIND_LEFT_TOP:
				Rect.right += Moving.cx;
				Rect.bottom += Moving.cy;
				break;
			default:
				break;
		}

		m_ParrentRect = NewParrentRect;
		CheckBounds(Rect, m_ParrentRect);

		GetParent()->PostMessageW(WM_BUTTON_MOVING, GetDlgCtrlID());
	}
}

void CMovingButton::PreSubclassWindow()
{
	GetParent()->GetClientRect(m_ParrentRect);

	CMFCButton::PreSubclassWindow();
}

void CMovingButton::SetControlBounds(_In_opt_ CRect Bound /*= CRect(NO_BOUND, NO_BOUND, NO_BOUND, NO_BOUND)*/)
{
	m_ControlBounds = Bound;

	CRect Rect;
	GetWindowRect(Rect);
	GetParent()->ScreenToClient(Rect);

	CRect ParrentRect;
	GetParent()->GetClientRect(ParrentRect);
	CheckBounds(Rect, ParrentRect);
}

void CMovingButton::CheckBounds(_In_ CRect CurrentRect, _In_ CRect PossibleRect)
{
	// область масштабирования контрола
	CRect CheckRect;
	CheckRect.top	 = m_ControlBounds.top     != NO_BOUND ? m_ControlBounds.top    : PossibleRect.top;
	CheckRect.bottom = m_ControlBounds.bottom  != NO_BOUND ? m_ControlBounds.bottom : PossibleRect.bottom;
	CheckRect.left	 = m_ControlBounds.left    != NO_BOUND ? m_ControlBounds.left   : PossibleRect.left;
	CheckRect.right  = m_ControlBounds.right   != NO_BOUND ? m_ControlBounds.right  : PossibleRect.right;
	
	// смотрим чтобы контрол не ушел за допустимые границы
	if (CurrentRect.left  < CheckRect.left)
		CurrentRect.OffsetRect(CheckRect.left  - CurrentRect.left, 0);
	if (CurrentRect.right > CheckRect.right)
		CurrentRect.OffsetRect(CheckRect.right - CurrentRect.right, 0);

	// после сдвига граница может вылезать слева, уменьшаем ширину до необходимой
	if (CurrentRect.left < CheckRect.left)
		CurrentRect.left = CheckRect.left;

	if (CurrentRect.top    < CheckRect.top)
		CurrentRect.OffsetRect(0, CheckRect.top    - CurrentRect.top);
	if (CurrentRect.bottom > CheckRect.bottom)
		CurrentRect.OffsetRect(0, CheckRect.bottom - CurrentRect.bottom);

	// после сдвига граница может вылезать сверху, уменьшаем высоту до необходимой
	if (CurrentRect.top < CheckRect.top)
		CurrentRect.top = CheckRect.top;

	CMFCButton::MoveWindow(CurrentRect);
}

BOOL CMovingButton::Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	GetParent()->GetClientRect(m_ParrentRect);
	return CMFCButton::Create(lpszText, dwStyle, rect, pParentWnd, nID);
}