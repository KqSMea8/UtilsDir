// VerticalGraphCtrl.cpp: реализация класса CVerticalGraphCtrl элемента ActiveX.
#pragma once
#include <Interface2012\Custom\CustomAll.h>
#include <ipps.h>

class CActiveXBaseCtrl : public COleControl
{
	//DECLARE_DYNCREATE(CActiveXBaseCtrl)

	// Конструктор
public:
	CActiveXBaseCtrl();
	virtual ~CActiveXBaseCtrl();

	// Переопределение
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	void RegistrationOwnerMouse(PVOID pClass);
	CCustomAll* m_pOwnerMouse;
	// Реализация
protected:

	// Схемы сообщений
	DECLARE_MESSAGE_MAP()

	// Схемы подготовки к отправке
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

	// Схемы событий
	DECLARE_EVENT_MAP()
public:
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};