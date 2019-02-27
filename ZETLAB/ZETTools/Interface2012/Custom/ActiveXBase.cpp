#include "stdafx.h"
#include "ActiveXBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Схема сообщений

BEGIN_MESSAGE_MAP(CActiveXBaseCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
//	ON_WM_LBUTTONDOWN()
//	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// Схема подготовки к отправке

BEGIN_DISPATCH_MAP(CActiveXBaseCtrl, COleControl)
	DISP_FUNCTION_ID(CActiveXBaseCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()

// Схема событий

BEGIN_EVENT_MAP(CActiveXBaseCtrl, COleControl)
END_EVENT_MAP()

// CVerticalGraphCtrl::CVerticalGraphCtrl - Конструктор

CActiveXBaseCtrl::CActiveXBaseCtrl()
	:m_pOwnerMouse(nullptr)
{

}

// CVerticalGraphCtrl::~CVerticalGraphCtrl - Деструктор

CActiveXBaseCtrl::~CActiveXBaseCtrl()
{
	// TODO: Выполните здесь очистку данных экземпляра элемента управления.
}

// CVerticalGraphCtrl::OnDraw - функция рисования

void CActiveXBaseCtrl::OnDraw(
	CDC* pdc, const CRect& rcBounds, const CRect& /* rcInvalid */)
{
	if (!pdc)
		return;

	// TODO: Замените следующий код собственным кодом рисования.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}

// CVerticalGraphCtrl::DoPropExchange - поддержка сохранения

void CActiveXBaseCtrl::DoPropExchange(CPropExchange* pPX)
{
	COleControl::DoPropExchange(pPX);
}


// CVerticalGraphCtrl::OnResetState - сброс элемента управления к состоянию по умолчанию

void CActiveXBaseCtrl::OnResetState()
{
	COleControl::OnResetState();  // Сбрасывает значения по умолчанию, найденные в DoPropExchange

	// TODO: Сбросьте здесь состояние любого другого элемента управления.
}


// CVerticalGraphCtrl::AboutBox - показывает пользователю панель "О программе"

void CActiveXBaseCtrl::AboutBox()
{

}

void CActiveXBaseCtrl::RegistrationOwnerMouse(PVOID pClass)
{
	m_pOwnerMouse = (CCustomAll*)pClass;
}

