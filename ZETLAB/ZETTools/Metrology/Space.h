//------------------------------------------------------------------------------
#pragma once
#include <Metrology\SpaceSimple.h>
//------------------------------------------------------------------------------
template <class T>
class CSpace : public CSpaceSimple<T>
{// ���������� ��������
public:
	//T m_left;		// ����� ������� ���������
	//T m_right;	// ������ ������� ���������
	T m_mean;		// �������� ���������
	T m_radius;		// �������� ����� ���������, ������ ���� > 0

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
{// ����������� �� ���������
}
//------------------------------------------------------------------------------
template <class T>
CSpace<T>::CSpace(T l, T r) : CSpaceSimple<T>(l, r)
{// ����������� � ����������� - ������� ���������
	SetSpace(l, r);
}
//------------------------------------------------------------------------------
template <class T>
CSpace<T>::CSpace(const CSpace<T> &spc)
{// ����������� �����������
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
{// ������� ����� �������. ������ ��� ���������
	m_left = l;
	m_mean = (m_right + m_left) / 2;
	m_radius = fabs(m_right - m_mean);
}
//------------------------------------------------------------------------------
template <class T>
void CSpace<T>::SetSpaceRight(const T r)
{// ������� ������ �������. ����� ��� ���������
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
{// ������� ��������� ����� �������
	m_left = l; m_right = r;
	m_mean = (m_right + m_left) / 2;
	m_radius = fabs(m_right - m_mean);
}
//------------------------------------------------------------------------------
template <class T>
void CSpace<T>::SetSpaceFromHalf(const T x0, const T half)
{// ������� ��������� ����� �������� � ������
	m_mean = x0;
	m_radius = half;
	m_left  = x0 - half;
	m_right = x0 + half;
}
//------------------------------------------------------------------------------
template <class T>
void CSpace<T>::SetSpaceFromLength(const T x0, const T length)
{// ������� ��������� ����� �������� � �����
	m_mean = x0;
	m_radius = length / 2;
	m_left  = x0 - m_radius;
	m_right = x0 + m_radius;
}
//------------------------------------------------------------------------------