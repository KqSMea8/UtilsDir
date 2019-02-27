//------------------------------------------------------------------------------
#pragma once
#include <Metrology\SpaceSimple.h>
//------------------------------------------------------------------------------
template <class T>
class CSpace : public CSpaceSimple<T>
{// одномерный интервал
public:
	//T m_left;		// левая граница интервала
	//T m_right;	// правая граница интервала
	T m_mean;		// середина интервала
	T m_radius;		// половина длины интервала, должна быть > 0

	CSpace();
	CSpace(T l, T r);
	CSpace(const CSpace<T> &spc);
	virtual ~CSpace() {}

	void Clear();
	void SetSpaceLeft(const T l);
	void SetSpaceRight(const T r);
	void SetSpaceRadius(const T radius);
	void SetSpace(const T l, const T r);
	void SetSpaceFromHalf(const T x0, const T half);
	void SetSpaceFromLength(const T x0, const T length);
};
//------------------------------------------------------------------------------
template <class T>
CSpace<T>::CSpace() : CSpaceSimple<T>()
	, m_mean	( (T)0 )
	, m_radius	( (T)0 )
{// конструктор по умолчанию
}
//------------------------------------------------------------------------------
template <class T>
CSpace<T>::CSpace(T l, T r) : CSpaceSimple<T>(l, r)
{// конструктор с параметрами - границы интервала
	SetSpace(l, r);
}
//------------------------------------------------------------------------------
template <class T>
CSpace<T>::CSpace(const CSpace<T> &spc)
{// конструктор копирования
	m_left   = spc.m_left;
	m_right  = spc.m_right;
	m_mean   = spc.m_mean;
	m_radius = spc.m_radius;
}
//------------------------------------------------------------------------------
template <class T>
void CSpace<T>::Clear()
{
	CSpaceSimple::Clear();
	m_mean = m_radius = (T)0;
}
//------------------------------------------------------------------------------
template <class T>
void CSpace<T>::SetSpaceLeft(const T l)
{// задание левой границы. правая без изменения
	m_left = l;
	m_mean = (m_right + m_left) / 2;
	m_radius = fabs(m_right - m_mean);
}
//------------------------------------------------------------------------------
template <class T>
void CSpace<T>::SetSpaceRight(const T r)
{// задание правой границы. левая без изменения
	m_right = r;
	m_mean = (m_right + m_left) / 2;
	m_radius = fabs(m_right - m_mean);
}
//------------------------------------------------------------------------------
template <class T>
void CSpace<T>::SetSpaceRadius(const T radius)
{
	if (radius > (T)0)
	{
		m_radius = radius;
		m_left = m_mean - m_radius;
		m_right = m_mean + m_radius;
	}
}
//------------------------------------------------------------------------------
template <class T>
void CSpace<T>::SetSpace(const T l, const T r)
{// задание интервала через границы
	m_left = l; m_right = r;
	m_mean = (m_right + m_left) / 2;
	m_radius = fabs(m_right - m_mean);
}
//------------------------------------------------------------------------------
template <class T>
void CSpace<T>::SetSpaceFromHalf(const T x0, const T half)
{// задание интервала через середину и радиус
	m_mean = x0;
	m_radius = half;
	m_left  = x0 - half;
	m_right = x0 + half;
}
//------------------------------------------------------------------------------
template <class T>
void CSpace<T>::SetSpaceFromLength(const T x0, const T length)
{// задание интервала через середину и длину
	m_mean = x0;
	m_radius = length / 2;
	m_left  = x0 - m_radius;
	m_right = x0 + m_radius;
}
//------------------------------------------------------------------------------