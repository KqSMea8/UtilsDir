//------------------------------------------------------------------------------
#pragma once
#include <ZetTools\include\ZetTools.h>
//------------------------------------------------------------------------------
/* ��������� ������, ������������ �� Corr.exe � UNIT-2.
	��������� ���������� ���� ��������� �� �������!
	������ ���� - ��� ����������� ������ ������ � ���������.
	size - ��� �� sizeof(StructForUNIT2)!!!
	size = ��. � TestSize		
*/ 
//------------------------------------------------------------------------------
#define SizeItemsInByte (sizeof(StructCorrForUNIT2))
#define SizeOneArrayInByte(sizeArray) (sizeof(float) * sizeArray)
#define SizeAllArraysInByte(sizeArray, nArray) (SizeOneArrayInByte(sizeArray) * nArray)
//------------------------------------------------------------------------------
struct StructCorrForUNIT2
{
	DWORD size;				// ������ ���� ������ ���� ���������
	DWORD sizeOneArray;		// ������ ������ ������� � ��������

	DWORD indxKit;			// ������ ����������
	WORD numArray;			// ���-�� ��������, ������ ������� ���� � ���������
	bool bOldData;			// ������ ���� � ��������� ���������� ��� ���
	bool b1;

	float xPosExtremum_1;	// � ���������� ���������� 1 - �������� �������������� �������
	float xPosExtremum_2;	// � ���������� ���������� 2 - ���������� �������� ���������� �������
	float xPosExtremum_3;	// � ���������� ���������� 3 - �������� ��������� �������������� �������
	float xPosExtremum_4;	// � ���������� ���������� 4 - ������� �������������� �������
	float xPosIntersection;
	
// 	float yValExtremum_1;	// �������� ���������� 1 - �������� �������������� �������
// 	float yValExtremum_2;	// �������� ���������� 2 - ���������� �������� ���������� �������
// 	float yValExtremum_3;	// �������� ���������� 3 - �������� ��������� �������������� �������
// 	float yValExtremum_4;	// �������� ���������� 4 - ������� �������������� �������
	
	CZetTime ztTime;		// ����� ������

	StructCorrForUNIT2();
	// ������� �������� ���� size ���������
	void SetSizeOfStruct();

	// ������������ ����� ���������, ��������� � ���������.
	// ������������ �������� - ���������� �����.
	bool TestSize(DWORD * pShift = nullptr);

	// ������������ ��������� �� ������� ������� � ��������� ���������
	// �� ������ � �������� indxArray (���� � 0), ������������ � ���������.
	// ������������ �������� - ���������� �����.
	// ���� ���� �� ��������, �� ��������� �������������� nullptr.
	bool GetPointers(const int indxArray, float ** ppDataArray);
};
//------------------------------------------------------------------------------