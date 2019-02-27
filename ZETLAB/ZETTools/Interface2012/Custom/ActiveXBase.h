// VerticalGraphCtrl.cpp: ���������� ������ CVerticalGraphCtrl �������� ActiveX.
#pragma once
#include <Interface2012\Custom\CustomAll.h>
#include <ipps.h>

class CActiveXBaseCtrl : public COleControl
{
	//DECLARE_DYNCREATE(CActiveXBaseCtrl)

	// �����������
public:
	CActiveXBaseCtrl();
	virtual ~CActiveXBaseCtrl();

	// ���������������
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	void RegistrationOwnerMouse(PVOID pClass);
	CCustomAll* m_pOwnerMouse;
	// ����������
protected:

	// ����� ���������
	DECLARE_MESSAGE_MAP()

	// ����� ���������� � ��������
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

	// ����� �������
	DECLARE_EVENT_MAP()
public:
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};