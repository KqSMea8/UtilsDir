//--------------------------------------------------------------------------
//	������ CCirculBuffer.h
//	������ 27.07.2011
//--------------------------------------------------------------------------
#pragma once
#include "afx.h"
//--------------------------------------------------------------------------
class CCirculBuffer
{
private:
	int FCurIndex;			// ������� ������ ��� ������
	int FCurIndexFiltr;		// ������� ������ ��� ������ � ������ �������
	int FDelta;				// ���������� �������� �������
	int FIndexStart0;		// ��������� �������� �������� �������
	int FLengthFiltr;		// ����� ������������� �������
	int FSize;				// ������ ������ � ������
	int sizeT_Buffer;		// ������ ������ � ������
	bool FBufferFull;		// ����� �����
	bool FFiltrEnable;		// ����� �������� ����� > ����� �������
	bool bCreateData;		// ����� ������ ������ ������

	float* pGlobalAddres;		// ����� ������
	float* pDataForFiltr;		// ����� ������� ������ ��� ����������
	long long int FIndexStart;	// ������ ������ �����
	long long int FIndexEnd;	// ������ ��������� �����

protected:

public:
	CCirculBuffer();
	virtual ~CCirculBuffer();
	bool Create(int size, int delta, int indexStart,
				int lengthFiltr, float *addBuf = NULL);

	bool m_bDecimation;

	// ������� �������
	int GetCurIndex() { return FCurIndex; }
	bool GetBufferFull() { return FBufferFull; }

	bool GetFiltrEnable() { return FFiltrEnable; }

	int GetSize() { return FSize; }

	// ������� ������
	float GetPointFromGlobalIndex(long long int index);
	float* GetDataForFiltr();
	void WriteOnce(float data);
	void NewFiltr(int newLengthFiltr);
	void Clear(bool bInitData = false, int delta = 0, int indexStart = 0);
};
//--------------------------------------------------------------------------