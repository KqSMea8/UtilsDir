//--------------------------------------------------------------------------
//	Циклический буфер
//--------------------------------------------------------------------------
#pragma once
#include "afx.h"
//--------------------------------------------------------------------------
template <class T>		// Т - это float или double
class CBufferCircular
{
private:
	bool m_bFull;
	UINT m_size;
	T *m_pArray;
	T *m_pRead;
	T *m_pWrite;

protected:

public:
	CBufferCircular();
	CBufferCircular(const UINT size);
	virtual ~CBufferCircular();

	// функции свойств
	UINT GetSize() { return m_size; }
	bool SetSize(const UINT size);

	// функции класса
	T* GetBufferPointer() { return m_pArray; }
	bool Read(T &val);
	void Write(const T val);
};
//--------------------------------------------------------------------------
template <class T>
CBufferCircular<T>::CBufferCircular()
: m_bFull(false)
, m_size(0)
, m_pArray(NULL)
, m_pRead(NULL)
, m_pWrite(NULL)
{
}
//--------------------------------------------------------------------------
template <class T>
CBufferCircular<T>::CBufferCircular(const UINT size)
: m_bFull(false)
, m_size(0)
, m_pArray(NULL)
, m_pRead(NULL)
, m_pWrite(NULL)
{
	if (size > 0)
		SetSize(size);
}
//--------------------------------------------------------------------------
template <class T>
CBufferCircular<T>::~CBufferCircular()
{
	if (m_size > 0)
		delete [] m_pArray;
}
//--------------------------------------------------------------------------
template <class T>
bool CBufferCircular<T>::SetSize(const UINT size)
{
	bool ret = m_size == size;
	if (!ret)
	{
		if (m_size > 0)
			delete [] m_pArray;
		try
		{
			m_pArray = new T [size];
			m_size = size;
		}
		catch (CMemoryException*)
		{
			m_size = 0;
			m_pArray = NULL;
		}
		m_pRead = m_pWrite = m_pArray;
	}
	return ret;
}
//--------------------------------------------------------------------------
template <class T>
bool CBufferCircular<T>::Read(T &val)
{
	bool ret = m_size > 0
	if (ret)
	{
		ret = m_pRead != m_pWrite;
		if (ret)
		{
			val = *m_pRead++;
			if ( m_pRead == (m_pArray + m_size) )
				m_pRead == m_pArray;
		}
	}
	return ret;
}
//--------------------------------------------------------------------------
template <class T>
void CBufferCircular<T>::Write(const T val)
{
	*m_pWrite++ = val;
	if ( m_pWrite == (m_pArray + m_size) )
		m_pWrite = m_pArray;

	if (m_pWrite == m_pRead)
	{
		++m_pRead;
		if ( m_pRead == (m_pArray + m_size) )
			m_pRead = m_pArray;
	}
}
//--------------------------------------------------------------------------