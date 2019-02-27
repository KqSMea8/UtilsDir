#pragma once
#include "afxbutton.h"
/**************************************************************************************************
// Использование контрола:
// Добавить на форму CMFCButton, настроить его свойства, создать экземпляр его как CMovingButton
// Рекомендуемые параметры: Style - No Borders, ModalFrame - True
// Вызвать конструктор с необходимыми параметрами (см. описание)
// В OnSize родителя	вызывать функцию MoveControl
// Ловить сообщение		WM_BUTTON_MOVING где WPARAM - ID кнопки
**************************************************************************************************/
#define NO_BOUND  -1	
//*************************************************************************************************
enum BindingTypes		// флаг привязки стороны квадрата
{
	BIND_NONE = 1,		// ниодна  сторона не привязана
	BIND_LEFT = 2,		// левая   сторона привязана
	BIND_TOP = 4,		// верхняя сторона привязана
	BIND_LEFT_TOP = 8,	// верхняя и левые стороны привязаны
};
//*************************************************************************************************
// Сообщение о том, что контрол перемещается, WPARAM - ID кнопки
#define WM_BUTTON_MOVING		WM_USER + 600
//*************************************************************************************************
class CMovingButton : public CMFCButton
{
public:	//*************************************************************************************
	// Parameter: _In_opt_ bool GorizontalMoving - флаг что кнопка будет перемещаться горизонтально
	// Parameter: _In_opt_ bool VerticalMoving   - флаг что кнопка будет перемещаться вертикально
	CMovingButton(_In_opt_ bool GorizontalMoving = true,
				  _In_opt_ bool VerticalMoving   = false);
	~CMovingButton();
public:	//*****************************************************************************************
	// Установка курсора который будет отображаться на кнопки при наведении на нее
	// Parameter: _In_ UINT CursorID - ID курсора
	void SetMovingCursor(_In_ UINT CursorID);
	// Перемещает контрол, достаточно вызвать в OnSize
	// Parameter: _In_opt_ BindingTypes BindType - тип привязки сторон контрола
	// Parameter: _In_opt_ CRect BoundRect - площадь которой ограничено положение контрола при перемещении
	// если BoundRect имеет поля NO_BOUND то ограничением будут края родителя
	void MoveControl(_In_opt_ BindingTypes BindType = BIND_NONE, _In_opt_ CRect BoundRect = CRect(NO_BOUND, NO_BOUND, NO_BOUND, NO_BOUND));
	//*********************************************************************************************
	// Установка рабочей области за которую не может заходить контрол, если NO_BOUND то граница не учитывается
	void SetControlBounds(_In_opt_ CRect Bound = CRect(NO_BOUND, NO_BOUND, NO_BOUND, NO_BOUND));
protected://***************************************************************************************
	CRect m_ParrentRect;		// размеры окна перента
	//*********************************************************************************************
	CPoint m_CurrentPoint;		// Текущая кнопка
	//*********************************************************************************************
	bool m_bMoving;				// Флаг перемещения кнопки
	//*********************************************************************************************
	bool m_bGorizontalMoving;	// Флаг возможности горизонтально передвигать кнопку
	bool m_bVerticalMoving;		// Флаг возможности вертикально   передвигать кнопку
	//*********************************************************************************************
	CRect m_ControlBounds;	    // граница за которую нельзя перемещать контрол
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

