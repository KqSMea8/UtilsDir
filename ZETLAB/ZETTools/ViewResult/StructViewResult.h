//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
#define SIZE_STRING_FOR_VALUE 32
#define MAX_COUNTER_FOR_SAME_LINE 7
//-----------------------------------------------------------------------------
/*		��������� ��� ������ � ����� ������� (���������� � �.�.).

	����� �������� ����������� ��������� �������:
- �������� ����������� �������� ������������ ������� ������������� ��������;
- ������������ �������� ����������� ������������ ��� ����������� ������,
  ������� �� ������ �� ���-�� ����������;
- ���-�� �������� ���� ��� �������� ������ ����� 3 - �� ����� 3;
- ���-�� �������� ���� ��� �������� ������ ����� 1 ��� 2 - �� ����� 4.
- ���-�� �������� ���� �� ����� ���������� ������, ��� �� 1.

	nIndxPoint - ������ ���������� �����, ���� �� ����� ��� �������������, ��
������������ �� ��������� MaxLevel � ������������ �������� �����������.

	������ ������� � sValueCur - ��� ������ ����.

	��� ��������� ������ �����, ��� ������������� � ������ (sValueCur),
������� ������������ �� ������� ����������� �������� (sValueOld). ��� ������
������������, � ���� ��� ��������� MAX_COUNTER_FOR_SAME_LINE ���, ��
��������� �������� ������ � sValueCur ���������� �� 1.
*/
//-----------------------------------------------------------------------------
struct STRUCT_VIEW_RESALT
{
	BYTE bFlags;		// ����� ���������
	BYTE nCounter;		// ������� ���������� �����
	BYTE nDigitOld;		// ���-�� �������� ���� � ��������� ��������
	BYTE nIndxPointInt;	// ������ ���������� �����, ����������� � ����� ������ dll
	BYTE nIndxPointExt;	// ������ ���������� �����, �������� �����
	char reserved1;		// 
	char reserved2;		// 
	char reserved3;		// 

	float alfa;			// �������� ���������� ��������� ��������
	float valueOld;		// ������� ��������
	float valueDelta;	// ������� �������� ��������� ��������
	float uncertainty;	// �������� (���������������) �����������

	wchar_t sValueCur[SIZE_STRING_FOR_VALUE];	// ������ � ������� ���������
	wchar_t sValueOld[SIZE_STRING_FOR_VALUE];	// ������ � ������� ���������
};
//-----------------------------------------------------------------------------