//------------------------------------------------------------------------------
#include "StdAfx.h"
#include "activeunit.h"
//------------------------------------------------------------------------------
CUnit::CUnit() : CDUnit()
{// �������� Create ���� ��� �������������
	InsideUnit.ActiveXcwnd = NULL;
	InsideUnit.parent = NULL;
	m_bCreate = false;
}
//------------------------------------------------------------------------------
CUnit::~CUnit()
{
}
//------------------------------------------------------------------------------
void CUnit::SetUnitInterface(CString associatedProg, CWnd* parentCWnd)
{// ���� ������ ���������� ��� ��������� � ��� �������� ��������
	InsideUnit.associatedProgName = associatedProg;
	InsideUnit.parent = parentCWnd;
}
//------------------------------------------------------------------------------
bool CUnit::cwndValidate(CWnd* cwnd)
{// ��������
	bool result;
	if (cwnd)
		result = cwnd->GetControlUnknown() != NULL;
	else
		result = false;
	return result;
}
//------------------------------------------------------------------------------
long CUnit::startAll(WPARAM wParam, LPARAM lParam)
{//			������� ���������� ������ 2 ����:
// - � ������ ������ SCADA-������� (wParam = 1) �
// - ��� ��� ���������� (wParam = 0).
//			������������ ��������:
// 1 - ���������-������ �������� ����� Unit,
// 0 - ���������-������ �� ����������� ��� ������ ��� ��������� ��� ����������
	long ret = 0;
	if (wParam == 1)
	{	// ������� ���� ������� ����
		if (!m_bCreate)
			m_bCreate = Create(L"", WS_CHILD, CRect(0, 0, 0, 0), InsideUnit.parent, UNIT_ID + unitCounter) != FALSE;
		if (m_bCreate)
		{	// ������ ��������� ���������-������ ����� Unit
			if (Activate(InsideUnit.associatedProgName) == 0)
				ret = 1;
			InsideUnit.ActiveXcwnd = this;
		}
		else		
			InsideUnit.ActiveXcwnd = NULL;
		++unitCounter;	// �� �����, ��� ������ Create
	}
	else if (wParam == 0)
	{	// �������� ���������-������
		DisActivate();
		--unitCounter;
	}
	return ret;
}
//------------------------------------------------------------------------------