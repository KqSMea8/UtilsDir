//------------------------------------------------------------------------------
#pragma once
#include "afxwin.h"
#include "afxdisp.h"
#include <Dialog_ZET\CDUnit.h>

#define UNIT_ID 1001
static long unitCounter;
//------------------------------------------------------------------------------
//���������, ����������� ����� ��������� � ������
struct InsideUnitXS
{
//	LPOLESTR clsidStr;			// ����� �� �������������
//	CLSID clsidActiveX;			// CLSID Unit-a (�� �� ��������)
	CWnd* ActiveXcwnd;			// ���. ������ CDUnit
	CWnd* parent;				// ���. ������ ZET-���������� - ������ ��� ����������
	CString associatedProgName;	// ��� ����������� ���������, ��� exe
//	long unit_id;				// ID ���. ������ CDUnit
};
//------------------------------------------------------------------------------
class CUnit : public CDUnit
{
private:
	bool m_bCreate;
public:
	CUnit();
	virtual ~CUnit();
	InsideUnitXS InsideUnit;
	bool cwndValidate(CWnd* cwnd);	//�������� ������������ CWbnd*
	void SetUnitInterface(CString associatedProg, CWnd* parentCWnd);
	long startAll(WPARAM wParam, LPARAM lParam);
};
//------------------------------------------------------------------------------