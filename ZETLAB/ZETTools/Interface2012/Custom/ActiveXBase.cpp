#include "stdafx.h"
#include "ActiveXBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ����� ���������

BEGIN_MESSAGE_MAP(CActiveXBaseCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
//	ON_WM_LBUTTONDOWN()
//	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// ����� ���������� � ��������

BEGIN_DISPATCH_MAP(CActiveXBaseCtrl, COleControl)
	DISP_FUNCTION_ID(CActiveXBaseCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()

// ����� �������

BEGIN_EVENT_MAP(CActiveXBaseCtrl, COleControl)
END_EVENT_MAP()

// CVerticalGraphCtrl::CVerticalGraphCtrl - �����������

CActiveXBaseCtrl::CActiveXBaseCtrl()
	:m_pOwnerMouse(nullptr)
{

}

// CVerticalGraphCtrl::~CVerticalGraphCtrl - ����������

CActiveXBaseCtrl::~CActiveXBaseCtrl()
{
	// TODO: ��������� ����� ������� ������ ���������� �������� ����������.
}

// CVerticalGraphCtrl::OnDraw - ������� ���������

void CActiveXBaseCtrl::OnDraw(
	CDC* pdc, const CRect& rcBounds, const CRect& /* rcInvalid */)
{
	if (!pdc)
		return;

	// TODO: �������� ��������� ��� ����������� ����� ���������.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}

// CVerticalGraphCtrl::DoPropExchange - ��������� ����������

void CActiveXBaseCtrl::DoPropExchange(CPropExchange* pPX)
{
	COleControl::DoPropExchange(pPX);
}


// CVerticalGraphCtrl::OnResetState - ����� �������� ���������� � ��������� �� ���������

void CActiveXBaseCtrl::OnResetState()
{
	COleControl::OnResetState();  // ���������� �������� �� ���������, ��������� � DoPropExchange

	// TODO: �������� ����� ��������� ������ ������� �������� ����������.
}


// CVerticalGraphCtrl::AboutBox - ���������� ������������ ������ "� ���������"

void CActiveXBaseCtrl::AboutBox()
{

}

void CActiveXBaseCtrl::RegistrationOwnerMouse(PVOID pClass)
{
	m_pOwnerMouse = (CCustomAll*)pClass;
}

