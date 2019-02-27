//------------------------------------------------------------------------------
//	CShiftBufferDATE.h
//	������ �� 05.05.2011
//	����������, ��� "����", �������� �.�.
//------------------------------------------------------------------------------
//	����� CShiftBufferDATE ������������ ��� ���������� ���������� ������
// ��������� ������ ���� DATE.
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <Constants.h>
#include <Buffer\ShiftBuffer.h>
//------------------------------------------------------------------------------
class CShiftBufferDATE
{
private:
	CShiftBuffer<DATE> *pbuf;
	DATE FdtDate;
	double FdtSec;

protected:

public:
	CShiftBufferDATE();
	virtual ~CShiftBufferDATE();
	bool Create(int size, int sizePage, double dtSec, DATE date = 0.0);

	// ������� �������
	DATE GetdtDate() { return FdtDate; }

	double GetdtSec() { return FdtSec; }
	void SetdtSec(double val);

	DATE* GetGlobalAddres() { return pbuf->GetGlobalAddres(); }
	DATE* GetPointerLastPage() { return pbuf->GetPointerLastPage(); }
	DATE* GetPointerLastElement() { return pbuf->GetPointerLastElement(); }
	int GetSize() { return pbuf->GetSize(); }
	bool SetSize(int size) { return pbuf->SetSize(size); }
	int GetSizePage() { return pbuf->GetSizePage(); }
	void SetSizePage(int val) { pbuf->SetSizePage(val); }

	// ������� ������
	void NewData();				// ����� � ������� ����� ����� ������
	void NewTime(DATE date);	// ������� ����� �� ���������� ��������
	void ShiftData() { pbuf->ShiftData(); }
};
//------------------------------------------------------------------------------