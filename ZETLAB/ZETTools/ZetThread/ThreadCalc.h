//------------------------------------------------------------------------------
//	������ CThreadCalc.h
//	������ 02.08.2012
//	����������, ��� "����", �������� �.�.
//------------------------------------------------------------------------------
//   ��������� ���������� ������� Calc_Thread - ��� ������� ������. Ÿ �������� -
// ��� ��������� �� ��������� ��������� ����� ����������, ������� �������� �
// ���� ��������� �� ��������� ������� ���������� (��. ������� CreateZetThread
// ������-��������).
//   � ������� ������ ����� ���������� ������ ���������� ��������� ����������
// �������, ��� ������ ����������� ��������� ��������� ������. ����� ���������
// ���������� ������� - ��� ���������� function ������� Create ������� ������. 
// �������� ��������� ���������� ������� - ��� ���������� paramFunction
// ������� Create ������� ������.
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <ZetThread\CustomZetThread.h>
//------------------------------------------------------------------------------
class CThreadCalc : public CCustomZetThread
{
private:
protected:

public:
	CThreadCalc();
	virtual bool Create( CString nameThread, HWND hwnd,
					FunctionThread function, PVOID paramFunction = NULL );
	virtual ~CThreadCalc(void);

	// ������ �������
	// ����� ������
	// ������ ������
};
//------------------------------------------------------------------------------