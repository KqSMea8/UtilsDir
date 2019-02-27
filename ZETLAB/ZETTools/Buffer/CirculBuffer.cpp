//------------------------------------------------------------------------------
//	������ CCirculBuffer.cpp
//	������ 27.07.2011
//------------------------------------------------------------------------------
//	����� ������������ ��� �������� ����������� �������, ���������������
// �� ������ � ��������� ���������. � ������� ����� ����� ������ ��� �����.
// ����� ����� ���� ������� ��� ����������, �.�. ����������� ������ ������.
//	������ ����� � ������ ������������� �������� �������� ������� (���
// ��������� ������������� ������).
//	������ ������ �� ������:
// 1. ��� ����������� - �� �������� �������� �������, ������� ��������
//	  ������� (���� � 0) ������� � �������� ������� (�.�. ��� �� �� �������).
// 2. ��� ���������� - ������ ��������� ���������� �����. ����������� �������
//	  ����� ����� ������������� �������.
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "CirculBuffer.h"
//------------------------------------------------------------------------------
CCirculBuffer::CCirculBuffer()
{
	bCreateData = false;
	pGlobalAddres = NULL;
	pDataForFiltr = NULL;
}
//------------------------------------------------------------------------------
CCirculBuffer::~CCirculBuffer()
{
	if (bCreateData)
	{
		delete [] pGlobalAddres;
		pGlobalAddres = nullptr;
	}
	if (pDataForFiltr)
	{
		delete [] pDataForFiltr;
		pDataForFiltr = nullptr;
	}
}
//------------------------------------------------------------------------------
bool CCirculBuffer::Create(
	int size,			// ������ ������ � ��������
	int delta,			// ���������� �������� �������
	int indexStart,		// ��������� �������� �������� �������
	int lengthFiltr,	// ����� �������
	float *addBuf)		// NULL ���� ����� ���� ��������� ������ ������
{
	bool ret = true;
	FSize = size;
	sizeT_Buffer = sizeFloat * FSize;

	// ������ � ����� �������
	if (addBuf)
	{
		bCreateData = false;
		pGlobalAddres = addBuf;
	}
	else
	{
		bCreateData = true;
		pGlobalAddres = new float [FSize];
		if (pGlobalAddres)
			Clear(true, delta, indexStart);
		else
			ret = false;
	}
	// ������ ��������� �����, �������� � ����� �������
	if (ret)
	{
		FLengthFiltr = lengthFiltr;
		pDataForFiltr = new float [FLengthFiltr];
		if (!pDataForFiltr)
			ret = false;
	}
	return ret;
}
//------------------------------------------------------------------------------
float CCirculBuffer::GetPointFromGlobalIndex(long long int index)
{// ������ ����� ����� �� ������ �� �������� �������� �������
	float ret;
	if ( ( FIndexStart <= index ) && ( index < FIndexEnd ) )
	{
		if ( (FDelta) && (FSize) )
		{
		long long int iCur = (index - FIndexStart0) / FDelta;
		int n = iCur % FSize;
		ret = pGlobalAddres[n];
		}
		else
			ret = 0.f;
	}
	else
		ret = 0.f;
//	if (FDelta==8)
//		int nn = 17;
	return ret;
}
//------------------------------------------------------------------------------
float* CCirculBuffer::GetDataForFiltr()
{//� ��������� ����� ������� ������ �� �������� ��� ���������� ������� ������.
	// ������� ������ - ��� ��������� n �����, ���������� � ������� �����, ���
	// n = FLengthFiltr
	for(int i=0; i<FLengthFiltr; i++)
	{
		int n = FCurIndexFiltr + i;
		if ( n >= FSize )
			n -= FSize;
		pDataForFiltr[i] = pGlobalAddres[n];
	}
	return pDataForFiltr;
}
//------------------------------------------------------------------------------
void CCirculBuffer::WriteOnce(float data)
{//	���������� ����� ����� � �����
	//if (FDelta==16)
	//	int nn = 17;
	if (FBufferFull)			// ���� ����� �����, �� ������ �����
		FIndexStart += FDelta;	// �������� ��������
	FIndexEnd += FDelta;		// ��� ������
	pGlobalAddres[FCurIndex++] = data;
	if ( FCurIndex == FSize )
	{
		FBufferFull = true;
		FCurIndex = 0;
	}

	// ������ ��� ������ ������� ---------------------------------
	if (FFiltrEnable)
	{// ����� � ������ ������, ��� ����� �������
		FCurIndexFiltr++;
		if ( FCurIndexFiltr == FSize )
			FCurIndexFiltr = 0;
	}
	else
	{// ����� ����, �������� �������. ����� ������ ������ FLengthFiltr
		if ( FCurIndex == FLengthFiltr )	// ������ ==, �.�. FCurIndex++
			FFiltrEnable = true;			//			��� ����!
	}
}
//------------------------------------------------------------------------------
void CCirculBuffer::Clear(bool bInitData, int delta, int indexStart)
{// ������������� ������. �������������� ������� ����������
	// ���������� ���������� � ��������� ������ � ������
	FCurIndex = 0;
	FCurIndexFiltr = 0;
	FFiltrEnable = false;
	FBufferFull = false;
	m_bDecimation = false;		// true ������ !!!
	if (bInitData)
		FDelta = delta;
	// ����� ������ ����� � ��������� �� ������
	if (bInitData)
		FIndexStart0 = indexStart;
	FIndexStart = FIndexEnd = FIndexStart0;
	//FIndexEnd -= FDelta;
	ZeroMemory(pGlobalAddres, sizeT_Buffer);
}
//------------------------------------------------------------------------------
void CCirculBuffer::NewFiltr(int newLengthFiltr)
{// ����� ������ (��������: ������ ������������ ��������, ������� ����� �����
	// ������� �����)
	//if(FLengthFiltr != newLengthFiltr) )
	{
		if (pDataForFiltr)
		{
			delete [] pDataForFiltr;
			pDataForFiltr = nullptr;
		}
		FLengthFiltr = newLengthFiltr;
		pDataForFiltr = new float [FLengthFiltr];
	}
	Clear();
}
//------------------------------------------------------------------------------