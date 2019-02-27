//------------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------------
template <class T>
class CSpaceSimple
{// ���������� ��������
public:
	T m_left;		// ����� ������� ���������
	T m_right;		// ������ ������� ���������

	CSpaceSimple();
	CSpaceSimple(T l, T r);
	CSpaceSimple(const CSpaceSimple<T> &spc);
	virtual ~CSpaceSimple() {}

	T GetLength();
	void Clear();

	bool IsSpaceOk();
	bool IsSpaceNotEmpty();
	bool IsSpaceInsert(CSpaceSimple &spc);
	bool IsSpaceCross(CSpaceSimple &spc);
	bool IsValueInsideSpace(const T val);
	bool SetSpaceOnlyCross(CSpaceSimple &spc);
};
//------------------------------------------------------------------------------
template <class T>
CSpaceSimple<T>::CSpaceSimple()
	: m_left	( (T)0 )
	, m_right	( (T)0 )
{// ����������� �� ���������
}
//------------------------------------------------------------------------------
template <class T>
CSpaceSimple<T>::CSpaceSimple(T l, T r)
{// ����������� � ����������� - ������� ���������
	m_left  = l;
	m_right = r;
}
//------------------------------------------------------------------------------
template <class T>
CSpaceSimple<T>::CSpaceSimple(const CSpaceSimple<T> &spc)
{// ����������� �����������
	m_left  = spc.m_left;
	m_right = spc.m_right;
}
//------------------------------------------------------------------------------
template <class T>
T CSpaceSimple<T>::GetLength()
{// ������ ����� ���������
	return fabs(m_right - m_left);
}
//------------------------------------------------------------------------------
template <class T>
void CSpaceSimple<T>::Clear()
{
	m_left = m_right = (T)0;
}
//------------------------------------------------------------------------------
template <class T>
bool CSpaceSimple<T>::IsSpaceOk()
{// �������� - ����� ������� ������ ������ �������
	return m_left <= m_right;
}
//------------------------------------------------------------------------------
template <class T>
bool CSpaceSimple<T>::IsSpaceNotEmpty()
{// �������� - �� ������ ��������
	return fabs(m_right - m_left) > (T)0;
}
//------------------------------------------------------------------------------
template <class T>
bool CSpaceSimple<T>::IsSpaceInsert(CSpaceSimple<T> &spc)
{// �������� - ��������� ������� ���� � ������?
	bool ret = IsSpaceOk() && spc.IsSpaceOk();
	if (ret)
		ret = ( (m_left <= spc.m_left) && (spc.m_right <= m_right) ) ||
			  ( (spc.m_left <= m_left) && (m_right <= spc.m_right) );
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
bool CSpaceSimple<T>::IsSpaceCross(CSpaceSimple<T> &spc)
{// �������� - ��������� ������������?
	bool ret = IsSpaceOk() && spc.IsSpaceOk();
	if (ret)
		ret = (spc.m_left <= m_right) && (m_left <= spc.m_right);
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
bool CSpaceSimple<T>::IsValueInsideSpace(const T val)
{// �������� - ����� ����������� ���������?
	bool ret = IsSpaceNotEmpty() && IsSpaceOk();
	if (ret)
		ret = (m_left <= val) && (val <= m_right);
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
bool CSpaceSimple<T>::SetSpaceOnlyCross(CSpaceSimple<T> &spc)
{// ��������� ������ ����������� ����������
	bool ret = IsSpaceCross(spc);
	if (ret)
	{// ���� �����������
		m_left  = (std::max)(m_left,  spc.m_left);
		m_right = (std::min)(m_right, spc.m_right);
	}
	return ret;
}
//------------------------------------------------------------------------------