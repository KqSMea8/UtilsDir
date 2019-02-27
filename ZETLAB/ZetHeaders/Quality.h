//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
//------------------------------------------------------------------------------
#define Quality_AllData_True	0
#define Quality_AllData_False	1
//------------------------------------------------------------------------------
// ��������� �������� ������
struct Quality
{
	unsigned Synchro:	4;		// ������ � ����������
	unsigned Convert:	4;		// ������ �������������� ������
	unsigned Algorithm:	4;		// ������ ��������� ������
	unsigned SelfTest:	4;		// ������ ������������
	unsigned :			4;
	unsigned :			4;
	unsigned Comment:	6;		// �����������
	unsigned AllData:	1;		// ������� ��� ������ ��� ���������
	unsigned Error:		1;		// ��� ������
	// ��-�� ����, ��� Quality ������ � union (��. ����),
	// �� ������������ � ���� ��������� �� ������ ����
	bool IsAllData()	{ return AllData == 0; }
	bool NoError()		{ return Error == 0; }
	bool IsError()		{ return Error == 1; }
	bool IsQuality()
	{
		return (Error == 0) &&
				( (Synchro   != 0) || (Convert != 0) ||
				  (Algorithm != 0) || (SelfTest != 0) );
	}
	bool IsOk()
	{
		return (AllData == 0) && (Synchro == 0) && (Convert == 0) &&
				(Algorithm == 0) && (SelfTest == 0);
	}

	void operator = (const long a)  { *((long*)this) = a; };
	void operator = (const DWORD a) { *((long*)this) = a; };
};
//------------------------------------------------------------------------------
// �������� ������
union QualityCode
{
	long	valL;
	Quality	valQ;
	
	QualityCode() { valL = 0; }
	QualityCode(const long a)    { valL = a; }
	QualityCode(const Quality &a) { valL = *((long*)&a); }
	QualityCode(const QualityCode &val) { valL = val.valL; }

	void operator = ( const long a )    { valL = a; }
	void operator = ( const Quality &a ) { valL = *((long*)&a); }
};
//------------------------------------------------------------------------------