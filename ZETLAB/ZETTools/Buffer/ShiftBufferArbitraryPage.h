//------------------------------------------------------------------------------
//	����� CShiftBufferArbitraryPage ������������ ��� ���������� ���������
// ������� � ������������ �������� ������ ����� ������ (����� � ������������)
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <Buffer\SmartArray.h>
//------------------------------------------------------------------------------
template <class T>
class CShiftBufferArbitraryPage
{
private:
	// ��������� �� ������ "�����" ������ ����� ���������� NewData
	T *m_pFirstNewPoint = nullptr;
	long xz;	// ��������� ��� ������������ ���������� ��� 8 ����
	SmartArray<T> m_saData;		// ������ 16 ����

protected:

public:
	CShiftBufferArbitraryPage<T>() {}
	CShiftBufferArbitraryPage<T>(const int sizeBuff);
	virtual ~CShiftBufferArbitraryPage<T>();
	bool Create(const int sizeBuff);

	// ������� �������
	T* GetGlobalAddres() { return m_saData.pData; }

	int GetSize() { return (int)m_saData.size; }
	bool SetSize(int sizeBuff);			// ����� ������ ������

	// ������� ������
	void Clear() { m_saData.Clear(); }
	T* GetFirstNewPoint() { return m_pFirstNewPoint; }
	bool NewData(const T *pData, const int sizePage, const int sizeBack = 0);
	bool ShiftData(const int sizePage, const int sizeBack = 0);
	void Zero();					// ��������� ����� ������
};
//------------------------------------------------------------------------------
template <class T>
CShiftBufferArbitraryPage<T>::CShiftBufferArbitraryPage(const int sizeBuff)
{
	SetSize(sizeBuff);
}
//------------------------------------------------------------------------------
template <class T>
CShiftBufferArbitraryPage<T>::~CShiftBufferArbitraryPage()
{
	m_saData.Clear();
	m_pFirstNewPoint = nullptr;
}
//------------------------------------------------------------------------------
template <class T>
bool CShiftBufferArbitraryPage<T>::Create(int sizeBuff)
{
	return SetSize(sizeBuff);
}
//------------------------------------------------------------------------------
template <class T>
bool CShiftBufferArbitraryPage<T>::SetSize(int sizeBuff)
{
	bool ret = m_saData.ReSize((DWORD)sizeBuff);
	m_saData.Zero();
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
void CShiftBufferArbitraryPage<T>::Zero()
{
	m_saData.Zero();
}
//------------------------------------------------------------------------------
template <class T>
bool CShiftBufferArbitraryPage<T>::ShiftData(
	const int size, const int sizeBack = 0)
{
	bool ret = (size > 0) && (sizeBack >= 0) &&
		((int)m_saData.size > (size + sizeBack));
	if (ret)
	{
		int num = (int)m_saData.size - size - sizeBack;	
		UINT sizeMove = num * sizeof(T);
		memmove_s(m_saData.pData, sizeMove, m_saData.pData + size, sizeMove);
		m_pFirstNewPoint = m_saData.pData + num;
	}
	return ret;
}
//------------------------------------------------------------------------------
// sizeBack - ���-�� �������� � ����� ������, ������� ��������
template <class T>
bool CShiftBufferArbitraryPage<T>::NewData(
	const T *pData, const int size, const int sizeBack = 0)
{	// ����� ����� ������, ������� �� ������
	bool ret = ShiftData(size, sizeBack) && (pData != nullptr);
	if (ret)
	{
		UINT sizeCopy = size * sizeof(T);
		memcpy_s(m_pFirstNewPoint, sizeCopy, pData, sizeCopy);
	}
	return ret;
}
//------------------------------------------------------------------------------