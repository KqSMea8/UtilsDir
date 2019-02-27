//------------------------------------------------------------------------------
//		Шаблонный класс синхронизации двух результатов разных типов с временем
// синхронизации CZetTime или CZetTimeSpan.
//
//      Tzt - CZetTime или CZetTimeSpan.
//      Классы Т1 и Т2 должны иметь деструктор, конструктор копирования и
// оператор присваивания.
//------------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------------
#include "afx.h"
#include <list>
#include <WideResult\WideResult.h>
#include <ZetTools/include/ZetTools.h>
//------------------------------------------------------------------------------
// Тип соответствия времен результатов
enum TypeAccordanceTimeResults
{
	trs_accurate,       // точное соответствие
	trs_byMin,          // по минимальному значению
};
//------------------------------------------------------------------------------
template <class Tzt, class T1, class T2>
class CResultSynchronization2_zt
{
private:
	TypeAccordanceTimeResults m_typeAccTimeRes;
	CZetTimeSpan m_ztsTimeMax;	// максимальное время между крайними результатами
	WideResult_ZetTime<Tzt, T1> m_wr1;
	WideResult_ZetTime<Tzt, T2> m_wr2;
	std::list<WideResult_ZetTime<Tzt, T1> > m_list1;	// лист первого канала
	std::list<WideResult_ZetTime<Tzt, T2> > m_list2;	// лист второго канала

template <class T>
	void AddResult(
		const Tzt & ztime, const T & result,
		WideResult_ZetTime<Tzt, T> & wr,
		std::list< WideResult_ZetTime<Tzt, T> > & list);

public:
	CResultSynchronization2_zt();
	~CResultSynchronization2_zt();
	CResultSynchronization2_zt(const CResultSynchronization2_zt & val);
	CResultSynchronization2_zt & operator = (const CResultSynchronization2_zt & val);
	
	// методы класса --------------------------------
	TypeAccordanceTimeResults GetTypeAccordanceTimeResults();
	void SetTypeAccordanceTimeResults(const TypeAccordanceTimeResults type);
	double & GetTimeMaxInSec();
	void SetTimeMaxInSec(const double & sec);
	
    void AddResult1(const Tzt & ztime, const T1 & result);
	void AddResult2(const Tzt & ztime, const T2 & result);
	
    bool GetSynchroResult(Tzt & ztime, T1 & result1, T2 & result2);
};
//------------------------------------------------------------------------------
template <class Tzt, class T1, class T2>
CResultSynchronization2_zt<Tzt, T1, T2>::CResultSynchronization2_zt()
	: m_ztsTimeMax(10i64, 0U)
	, m_typeAccTimeRes(trs_accurate)
{
	m_wr1.time = m_wr2.time = Tzt(0i64, 0U);
	// какие у T1 и T2 есть конструкторы не известно, поэтому так:
	ZeroMemory(&m_wr1.value, sizeof(T1));
	ZeroMemory(&m_wr2.value, sizeof(T2));
}
//------------------------------------------------------------------------------
template <class Tzt, class T1, class T2>
CResultSynchronization2_zt<Tzt, T1, T2>::~CResultSynchronization2_zt()
{
	m_list1.clear();
	m_list2.clear();
}
//------------------------------------------------------------------------------
template <class Tzt, class T1, class T2>
CResultSynchronization2_zt<Tzt, T1, T2>::
	CResultSynchronization2_zt(const CResultSynchronization2_zt<Tzt, T1, T2> & val)
{
	*this = val;
}
//------------------------------------------------------------------------------
template <class Tzt, class T1, class T2>
CResultSynchronization2_zt<Tzt, T1, T2> & CResultSynchronization2_zt<Tzt, T1, T2>::
	operator = (const CResultSynchronization2_zt<Tzt, T1, T2> & val)
{
	m_typeAccTimeRes = val.m_typeAccTimeRes;
	m_ztsTimeMax = val.m_ztsTimeMax;
	m_wr1 = val.m_wr1;
	m_wr2 = val.m_wr2;
	m_list1 = val.m_list1;
	m_list2 = val.m_list2;
	return *this;
}
//------------------------------------------------------------------------------
template <class Tzt, class T1, class T2>
TypeAccordanceTimeResults CResultSynchronization2_zt<Tzt, T1, T2>::GetTypeAccordanceTimeResults()
{
	return m_typeAccTimeRes;
}
//------------------------------------------------------------------------------
template <class Tzt, class T1, class T2>
void CResultSynchronization2_zt<Tzt, T1, T2>::SetTypeAccordanceTimeResults(const TypeAccordanceTimeResults type)
{
	if ((type == trs_accurate) || (type == trs_byMin))
        m_typeAccTimeRes = type;
}
//------------------------------------------------------------------------------
template <class Tzt, class T1, class T2>
double & CResultSynchronization2_zt<Tzt, T1, T2>::GetTimeMaxInSec()
{
	double ret = (double)m_ztsTimeMax;
	return ret;
}
//------------------------------------------------------------------------------
template <class Tzt, class T1, class T2>
void CResultSynchronization2_zt<Tzt, T1, T2>::SetTimeMaxInSec(const double & sec)
{
	m_ztsTimeMax = sec;
}
//------------------------------------------------------------------------------
template <class Tzt, class T1, class T2>
template <class T>
void CResultSynchronization2_zt<Tzt, T1, T2>::AddResult(
	const Tzt & ztime, const T & result,
	WideResult_ZetTime<Tzt, T> & wr,
	std::list< WideResult_ZetTime<Tzt, T> > & list)
{
	if (list.empty())
	{
		wr.time = ztime;
		wr.value = result;
		list.push_back(wr);
	}
	else
	{// данные уже есть
		Tzt timeBack = list.crbegin()->time;

		if ((Tzt)ztime > timeBack)
		{// новое время
			wr.time = ztime;
			wr.value = result;
			list.push_back(wr);

			while (true)
			{
				CZetTimeSpan dzt = (Tzt)ztime - list.cbegin()->time;
				if (dzt > m_ztsTimeMax)
					list.pop_front();
				else
					break;
			}
		}
		else if ((Tzt)ztime < list.cbegin()->time)
		{// время старое
			CZetTimeSpan dzt = timeBack - ztime;
			if (dzt <= m_ztsTimeMax)
			{// если данные не очень старые
				wr.time = ztime;
				wr.value = result;
				list.push_front(wr);
			}
		}
		else
		{// время известное
			wr.time = ztime;
			wr.value = result;

			auto it = list.begin();
			++it;	// время не старое
			for (auto ie = list.end(); it != ie; ++it)
			{
				if ((Tzt)ztime < it->time)
				{
					list.insert(it, wr);
					break;
				}
			}
		}
	}
}
//------------------------------------------------------------------------------
template <class Tzt, class T1, class T2>
void CResultSynchronization2_zt<Tzt, T1, T2>::AddResult1(const Tzt & ztime, const T1 & result)
{
	AddResult(ztime, result, m_wr1, m_list1);
}
//------------------------------------------------------------------------------
template <class Tzt, class T1, class T2>
void CResultSynchronization2_zt<Tzt, T1, T2>::AddResult2(const Tzt & ztime, const T2 & result)
{
	AddResult(ztime, result, m_wr2, m_list2);
}
//------------------------------------------------------------------------------
template <class Tzt, class T1, class T2>
bool CResultSynchronization2_zt<Tzt, T1, T2>::GetSynchroResult(
	Tzt & ztime, T1 & result1, T2 & result2)
{
	bool ret = ( !m_list1.empty() ) && ( !m_list2.empty() );
	if (ret)
	{
		auto it1 = m_list1.cbegin();
		auto it2 = m_list2.cbegin();
		Tzt ztime1 = it1->time;
		Tzt ztime2 = it2->time;

		ret = ztime1 == ztime2;
		if (ret)
		{
			ztime = ztime1;
			result1 = it1->value;
            result2 = it2->value;
	    	m_list1.pop_front();
	    	m_list2.pop_front();
		}
		else
		{
			switch (m_typeAccTimeRes)
			{
			case trs_accurate:
				if (ztime1 < ztime2)
				{// берём ztime2 из m_list2, в m_list1 надо найти элемент с таким временм
					auto it = m_list1.cbegin();
					for (auto ie = m_list1.cend(); it != ie; ++it)
					{
						ret = ztime2 == it->time;
						if (ret)
							break;
					}
					if (ret)
					{
						ztime = ztime2;
						result1 = it->value;
						result2 = it2->value;
						m_list1.erase(it);
						m_list2.pop_front();
					}
				}
				else// ztime1 > ztime2
				{// берём ztime1 из m_list1, в m_list2 надо найти элемент с таким временм
					auto it = m_list2.cbegin();
					for (auto ie = m_list2.cend(); it != ie; ++it)
					{
						ret = ztime1 == it->time;
						if (ret)
							break;
					}
					if (ret)
					{
						ztime = ztime1;
						result1 = it1->value;
						result2 = it->value;
						m_list1.pop_front();
                        m_list2.erase(it);
					}
				}
				break;

			case trs_byMin:
				result1 = it1->value;
				result2 = it2->value;
				if (ztime1 < ztime2)
				{
					ztime = ztime1;
					m_list1.pop_front();
				}
				else
				{
					ztime = ztime2;
					m_list2.pop_front();
				}
				break;
			}
		}
	}
	return ret;
}
//------------------------------------------------------------------------------