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
#define SizeItemsInByte (sizeof(StructSpectrForUNIT2))
#define SizeOneArrayInByte(sizeArray) (sizeof(float) * sizeArray)
#define SizeAllArraysInByte(sizeArray, nArray) (SizeOneArrayInByte(sizeArray) * nArray)
//------------------------------------------------------------------------------
// ���� bOldData ������������ � SCADA-����������
struct StructSpectrForUNIT2
{
	DWORD size;				// ������ ���� ������ ���� ���������
	DWORD sizeOneArray;		// ������ ������ ������� � ��������

	DWORD indxKit;			// ������ ����������
	BYTE numArray;			// ���-�� ��������, ������ ������� ���� � ���������
	bool bOldData;			// ������ ���� � ��������� ���������� ��� ���
	bool bGraf_Max;
	bool bGraf_Aver;

	CZetTime ztTime;		// ����� ������

	StructSpectrForUNIT2();
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