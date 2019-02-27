//------------------------------------------------------------------------------
//		Шаблонный класс синхронизации трёх результатов разных типов
//------------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------------
#include "afx.h"
#include <list>
#include "WideResult\WideResult.h"
//------------------------------------------------------------------------------
template <class T1, class T2, class T3>
class CResultSynchronization3
{
private:
	std::list< WideResult<T1> > m_list1;	// лист первого канала
	std::list< WideResult<T2> > m_list2;	// лист второго канала
	std::list< WideResult<T3> > m_list3;	// лист третьего канала

public:
	CResultSynchronization3(){ m_deltaTimeMax = 10; }

	DWORD m_deltaTimeMax;	// максимальное время между крайними результатами
							//		целое число секунд

	// методы класса --------------------------------
	void AddResult1(WideResult<T1> &result);
	void AddResult2(WideResult<T2> &result);
	void AddResult3(WideResult<T3> &result);
	bool GetSynchroResult(
		WideResult<T1> &wr1, WideResult<T2> &wr2, WideResult<T3> &wr3);
};
//------------------------------------------------------------------------------
template <class T1, class T2, class T3>
void CResultSynchronization3<T1, T2, T3>::AddResult1(WideResult<T1> &result)
{
	m_list1.push_back(result);
	if ( (m_list1.back().time - m_list1.front().time) > (double)m_deltaTimeMax )
		m_list1.pop_front();
}
//------------------------------------------------------------------------------
template <class T1, class T2, class T3>
void CResultSynchronization3<T1, T2, T3>::AddResult2(WideResult<T2> &result)
{
	m_list2.push_back(result);
	if ( (m_list2.back().time - m_list2.front().time) > (double)m_deltaTimeMax )
		m_list2.pop_front();
}
//------------------------------------------------------------------------------
template <class T1, class T2, class T3>
void CResultSynchronization3<T1, T2, T3>::AddResult3(WideResult<T3> &result)
{
	m_list3.push_back(result);
	if ( (m_list3.back().time - m_list3.front().time) > (double)m_deltaTimeMax )
		m_list3.pop_front();
}
//------------------------------------------------------------------------------
template <class T1, class T2, class T3>
bool CResultSynchronization3<T1, T2, T3>::GetSynchroResult(
	WideResult<T1> &wr1, WideResult<T2> &wr2, WideResult<T3> &wr3)
{
	bool ret = ( !m_list1.empty() ) && ( !m_list2.empty() ) &&
										( !m_list3.empty() );
	if (ret)
	{
		double timeMin = min ( m_list1.front().time,
				min (m_list2.front().time, m_list3.front().time) )

		wr1 = m_list1.front();
		if (wr1.time == timeMin)
			m_list1.pop_front();
		else
			wr1.time = timeMin;

		wr2 = m_list2.front();
		if (wr2.time == timeMin)
			m_list2.pop_front();
		else
			wr2.time = timeMin;

		wr3 = m_list3.front();
		if (wr1.time == timeMin)
			m_list3.pop_front();
		else
			wr1.time = timeMin;
	}
	return ret;
}
//------------------------------------------------------------------------------