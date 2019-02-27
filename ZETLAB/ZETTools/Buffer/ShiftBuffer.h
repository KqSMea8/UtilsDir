//------------------------------------------------------------------------------
//	��������� ����� CShiftBuffer ������������ ��� ���������� ��������� �������
//	������ 1.1 �� 19.04.2011
//	����������, ��� "����", �������� �.�.
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
//------------------------------------------------------------------------------
template <class T>
class CShiftBuffer
{
private:
	int FSize;				// ���-�� ��������� � ������
	int FSizePage;			// ���-�� ��������� � �������� ������

	bool bCreateData;		// ����� ����������
	int NumberPage;			// ���-�� ������� � ������
	int NumberPage_1;		// ���-�� ������� � ������ - 1
	T* pGlobalAddres;		// ����� ������
	T* pPointerFirstPage;	// ����� ������ �������� (���� � 0)
	T* pPointerLastPage;	// ����� ��������� ��������
	T* pPointerLastElement;	// ����� ���������� ��������
	int sizeT;				// ������ � ������ ������ ��������
	int sizeT_Page;			// ������ � ������ ��������
	int sizeT_NumberPage_1;	// ������ � ������ ������ ��� ����� ��������
	int sizeT_Buffer;		// ������ � ������ ����� ������

protected:

public:
	CShiftBuffer<T>();
	virtual ~CShiftBuffer();
	bool Create(int size, int sizePage, T* addBuf = NULL);

	// ������� �������
	T* GetGlobalAddres() { return pGlobalAddres; }
	T* GetPointerLastPage() { return pPointerLastPage; }
	T* GetPointerLastElement() { return pPointerLastElement; }

	int GetSize() { return FSize; }
	bool SetSize(int size);			// ����� ������ ������

	int GetSizePage() { return FSizePage; }
	bool SetSizePage(int val);		// ������ ���� ���������� ������

	int GetSizeInByte() { return sizeT_Buffer; }
	int GetSizePageInByte() { return sizeT_Page; }

	// ������� ������
	void Clear();					// ��������� ����� ������
	void NaN();						// ������� NaN-��
	void CopyNewData(T *addLastPage);// ������ ����������� ����� ������
	void NewData(T *addLastPage);	// ����� ������ � ����������� ����� ������
	void ShiftData();		// ������ ����� ������ � ������ ��� ����� ������
};
//------------------------------------------------------------------------------
template <class T>
CShiftBuffer<T>::CShiftBuffer()
{
	sizeT = sizeof(T);				// ������ � ������ ������ �������� ������
}
//------------------------------------------------------------------------------
template <class T>
CShiftBuffer<T>::~CShiftBuffer()
{
	if (bCreateData)
	{ 
		delete pGlobalAddres;
		pGlobalAddres = nullptr;
	}
}
//------------------------------------------------------------------------------
template <class T>
bool CShiftBuffer<T>::Create(int size, int sizePage, T* addBuf)
{
	bool ret = true;
	FSize = size;				// ���-�� ��������� � ������

	if (addBuf)
	{
		bCreateData = false;	// ����� �������
		pGlobalAddres = addBuf;
	}
	else
	{
		bCreateData = true;		// ����� ����������
		try
		{
			pGlobalAddres = new T [FSize];
		}
		catch (CMemoryException*)
		{
			pGlobalAddres = NULL;
			ret = false;			
		}
	}

	if (ret)
		SetSizePage(sizePage);
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
bool CShiftBuffer<T>::SetSize(int size)
{
	bool ret = true;
	if ( FSize != size )
	{
		if (bCreateData)
		{	// ���������� �����
			delete pGlobalAddres;
			pGlobalAddres = nullptr;
			try
			{
				pGlobalAddres = new T [FSize];
			}
			catch (CMemoryException*)
			{
				pGlobalAddres = NULL;
				ret = false;			
			}
		}
		FSize = size;
		if (ret)
			SetSizePage(FSizePage);		// �����������
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
bool CShiftBuffer<T>::SetSizePage(int val)
{// ������, ��� ������ �������� ����� ���������, �.�. ������ ������� ������
	// ������������ ��� ���������� ������ � ������� ����� !!!
	bool ret;
	if (val > 0)
	{
		FSizePage = val;
		NumberPage_1 = NumberPage = FSize / FSizePage;
		NumberPage_1--;
		pPointerLastElement = pGlobalAddres + FSize - 1;
		pPointerLastPage = pPointerLastElement - FSizePage + 1;
		pPointerFirstPage = pGlobalAddres + FSizePage;
		sizeT_Page = sizeT * FSizePage;
		sizeT_NumberPage_1 = sizeT_Page * NumberPage_1;
		sizeT_Buffer = sizeT_Page + sizeT_NumberPage_1;
		Clear();
		ret = true;
	}
	else
		ret = false;
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
void CShiftBuffer<T>::Clear()
{	
	ZeroMemory(pGlobalAddres, sizeT_Buffer);
}
//------------------------------------------------------------------------------
template <class T>
void CShiftBuffer<T>::NaN()
{
	memset(pGlobalAddres, 0xFF, sizeT_Buffer);
}
//------------------------------------------------------------------------------
template <class T>
void CShiftBuffer<T>::CopyNewData(T *addLastPage)
{	// ����� ������ ���������� � ��������� ������
	memcpy_s(pPointerLastPage, sizeT_Page, addLastPage, sizeT_Page);
}	
//------------------------------------------------------------------------------
template <class T>
void CShiftBuffer<T>::NewData(T *addLastPage)
{	// ������� ������� ������ � ������ (����� �� ���� ������),
	// ����� �� �������������� ����� ��������� ������ ������� ����� ������
	ShiftData();
	CopyNewData(addLastPage);
}
//------------------------------------------------------------------------------
template <class T>
void CShiftBuffer<T>::ShiftData()
{	// ������� ������ � ������ (����� �� ���� ������), � ������ ������ �� �����
	memcpy_s(pGlobalAddres, sizeT_NumberPage_1,
				pPointerFirstPage, sizeT_NumberPage_1);
}
//------------------------------------------------------------------------------