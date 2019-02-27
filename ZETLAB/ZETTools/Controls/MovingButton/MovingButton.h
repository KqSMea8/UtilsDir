#pragma once
#include "afxbutton.h"
/**************************************************************************************************
// ������������� ��������:
// �������� �� ����� CMFCButton, ��������� ��� ��������, ������� ��������� ��� ��� CMovingButton
// ������������� ���������: Style - No Borders, ModalFrame - True
// ������� ����������� � ������������ ����������� (��. ��������)
// � OnSize ��������	�������� ������� MoveControl
// ������ ���������		WM_BUTTON_MOVING ��� WPARAM - ID ������
**************************************************************************************************/
#define NO_BOUND  -1	
//*************************************************************************************************
enum BindingTypes		// ���� �������� ������� ��������
{
	BIND_NONE = 1,		// ������  ������� �� ���������
	BIND_LEFT = 2,		// �����   ������� ���������
	BIND_TOP = 4,		// ������� ������� ���������
	BIND_LEFT_TOP = 8,	// ������� � ����� ������� ���������
};
//*************************************************************************************************
// ��������� � ���, ��� ������� ������������, WPARAM - ID ������
#define WM_BUTTON_MOVING		WM_USER + 600
//*************************************************************************************************
class CMovingButton : public CMFCButton
{
public:	//*************************************************************************************
	// Parameter: _In_opt_ bool GorizontalMoving - ���� ��� ������ ����� ������������ �������������
	// Parameter: _In_opt_ bool VerticalMoving   - ���� ��� ������ ����� ������������ �����������
	CMovingButton(_In_opt_ bool GorizontalMoving = true,
				  _In_opt_ bool VerticalMoving   = false);
	~CMovingButton();
public:	//*****************************************************************************************
	// ��������� ������� ������� ����� ������������ �� ������ ��� ��������� �� ���
	// Parameter: _In_ UINT CursorID - ID �������
	void SetMovingCursor(_In_ UINT CursorID);
	// ���������� �������, ���������� ������� � OnSize
	// Parameter: _In_opt_ BindingTypes BindType - ��� �������� ������ ��������
	// Parameter: _In_opt_ CRect BoundRect - ������� ������� ���������� ��������� �������� ��� �����������
	// ���� BoundRect ����� ���� NO_BOUND �� ������������ ����� ���� ��������
	void MoveControl(_In_opt_ BindingTypes BindType = BIND_NONE, _In_opt_ CRect BoundRect = CRect(NO_BOUND, NO_BOUND, NO_BOUND, NO_BOUND));
	//*********************************************************************************************
	// ��������� ������� ������� �� ������� �� ����� �������� �������, ���� NO_BOUND �� ������� �� �����������
	void SetControlBounds(_In_opt_ CRect Bound = CRect(NO_BOUND, NO_BOUND, NO_BOUND, NO_BOUND));
protected://***************************************************************************************
	CRect m_ParrentRect;		// ������� ���� �������
	//*********************************************************************************************
	CPoint m_CurrentPoint;		// ������� ������
	//*********************************************************************************************
	bool m_bMoving;				// ���� ����������� ������
	//*********************************************************************************************
	bool m_bGorizontalMoving;	// ���� ����������� ������������� ����������� ������
	bool m_bVerticalMoving;		// ���� ����������� �����������   ����������� ������
	//*********************************************************************************************
	CRect m_ControlBounds;	    // ������� �� ������� ������ ���������� �������
private:
	void CheckBounds(_In_ CRect CurrentRect, _In_ CRect PossibleRect);
public:	//*****************************************************************************************
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = 0xffff);
};	//*********************************************************************************************

