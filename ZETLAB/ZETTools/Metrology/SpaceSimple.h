//------------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------------
template <class T>
class CSpaceSimple
{// одномерный интервал
public:
	T m_left;		// левая граница интервала
	T m_right;		// правая граница интервала

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
{// конструктор по умолчанию
}
//------------------------------------------------------------------------------
template <class T>
CSpaceSimple<T>::CSpaceSimple(T l, T r)
{// конструктор с параметрами - границы интервала
	m_left  = l;
	m_right = r;
}
//------------------------------------------------------------------------------
template <class T>
CSpaceSimple<T>::CSpaceSimple(const CSpaceSimple<T> &spc)
{// конструктор копирования
	m_left  = spc.m_left;
	m_right = spc.m_right;
}
//------------------------------------------------------------------------------
template <class T>
T CSpaceSimple<T>::GetLength()
{// расчёт длины интервала
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
{// проверка - левая граница меньше правой границы
	return m_left <= m_right;
}
//------------------------------------------------------------------------------
template <class T>
bool CSpaceSimple<T>::IsSpaceNotEmpty()
{// проверка - не пустой интервал
	return fabs(m_right - m_left) > (T)0;
}
//------------------------------------------------------------------------------
template <class T>
bool CSpaceSimple<T>::IsSpaceInsert(CSpaceSimple<T> &spc)
{// проверка - интервалы вложены один в другой?
	bool ret = IsSpaceOk() && spc.IsSpaceOk();
	if (ret)
		ret = ( (m_left <= spc.m_left) && (spc.m_right <= m_right) ) ||
			  ( (spc.m_left <= m_left) && (m_right <= spc.m_right) );
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
bool CSpaceSimple<T>::IsSpaceCross(CSpaceSimple<T> &spc)
{// проверка - интервалы пересекаются?
	bool ret = IsSpaceOk() && spc.IsSpaceOk();
	if (ret)
		ret = (spc.m_left <= m_right) && (m_left <= spc.m_right);
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
bool CSpaceSimple<T>::IsValueInsideSpace(const T val)
{// проверка - точка принадлежит интервалу?
	bool ret = IsSpaceNotEmpty() && IsSpaceOk();
	if (ret)
		ret = (m_left <= val) && (val <= m_right);
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
bool CSpaceSimple<T>::SetSpaceOnlyCross(CSpaceSimple<T> &spc)
{// Оставляем только пересечение интервалов
	bool ret = IsSpaceCross(spc);
	if (ret)
	{// есть пересечение
		m_left  = (std::max)(m_left,  spc.m_left);
		m_right = (std::min)(m_right, spc.m_right);
	}
	return ret;
}
//------------------------------------------------------------------------------