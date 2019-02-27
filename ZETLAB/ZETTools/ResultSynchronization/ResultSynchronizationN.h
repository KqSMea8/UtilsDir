//------------------------------------------------------------------------------
//		Ўаблонный класс синхронизации N однотипных результатов
//------------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------------
#include "afx.h"
#include <list>
#include <vector>
#include <WideResult\WideResult.h>
//------------------------------------------------------------------------------
const long g_NumberChanMax = 271;
//------------------------------------------------------------------------------
template <class T>
class CResultSynchronizationN
{
private:
	std::vector< std::list< WideResult<T> > > m_VectList;	// вектор листов
	bool GetTimeMin(double &tm);

public:
	CResultSynchronizationN(){ m_deltaTimeMax = 10; }
	CResultSynchronizationN(const long numChan)
		{  m_deltaTimeMax = 10; SetNumberChan(numChan); }

	DWORD m_deltaTimeMax;	// максимальное врем€ между крайними результатами
							//		целое число секунд

	// методы чтени€/записи свойств -----------------
	long GetNumberChan() { return (long)m_VectList.size(); }
	bool SetNumberChan(const long numChan);

	// методы класса --------------------------------
	bool AddResult(const long index, WideResult<T> &result);
	bool GetSynchroResult(WideResult< std::vector<T> > &wrs, double delta = 0);
	bool GetSynchroResult(std::vector< WideResult<T> > &vect, double delta = 0);
	void Reset();
};
//------------------------------------------------------------------------------
template <class T>
bool CResultSynchronizationN<T>::SetNumberChan(const long numChan)
{
	bool ret = (0 < numChan) && (numChan <= g_NumberChanMax);
	if (ret)
	{
		if ( !m_VectList.empty() )
			for (auto it=m_VectList.begin(), ie=m_VectList.end(); it!=ie; ++it)
				it->clear();
		try
		{
			m_VectList.resize((size_t)numChan);
		}
		catch (CMemoryException*)
		{
			m_VectList.clear();
			ret = false;
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
bool CResultSynchronizationN<T>::AddResult(const long index, WideResult<T> &result)
{
	bool ret = ( !m_VectList.empty() ) &&
				(0 <= index) && ( index < (long)m_VectList.size() );
	if (ret)
	{
		auto plist = &m_VectList[(size_t)index];
		plist->push_back(result);
		if (plist->size() > 0)
			if ( (plist->back().time - plist->front().time) > (double)m_deltaTimeMax )
				plist->pop_front();
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
bool CResultSynchronizationN<T>::GetTimeMin(double &tm)
{
	bool ret = false;
	auto it = m_VectList.cbegin();
	if ( !it->empty() )
	{
		tm = it->front().time;
		for(auto ie=m_VectList.cend(); it!=ie; ++it)
		{
			ret = !it->empty();			
			if (ret)
			{
				double time = it->cbegin()->time;
				if (tm > time)
					tm = time;
			}
			else
				break;
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
bool CResultSynchronizationN<T>::GetSynchroResult(
	WideResult< std::vector<T> > &wrs,
	double delta /* = 0 */)
{
	double timeMin;
	bool ret = GetTimeMin(timeMin);
	if (ret)
	{	// размер выходного вектора
		try
		{
			wrs.value.resize( m_VectList.size() );
		}
		catch (CMemoryException*)
		{
			wrs.value.clear();
			ret = false;
		}				
		// создаю синхронные результаты
		if (ret)
		{
			wrs.time = timeMin;
			wrs.quality = 0;
			auto itOut = wrs.value.begin();
			for(auto it=m_VectList.begin(), ie=m_VectList.end();
				it!=ie; ++it, ++itOut)
			{
				if (it->size() == 0)
					break;
				auto res = it->front();
				*itOut = res.value;
				wrs.quality |= res.quality;
				if (delta == 0)
				{
					if (res.time == timeMin)
						if (!it->empty())
							it->pop_front();
				}
				else
				{
					if (abs(res.time - timeMin) < delta)
						if (!it->empty())
							it->pop_front();
				}
			}
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
bool CResultSynchronizationN<T>::GetSynchroResult(
	std::vector< WideResult<T> > &vect,
	double delta /* = 0 */)
{// вначале определ€ю минимальное врем€ timeMin и наличие данных
	double timeMin;
	bool ret = GetTimeMin(timeMin);
	if (ret)
	{	// размер выходного вектора
		try
		{
			vect.resize( m_VectList.size() );
		}
		catch (CMemoryException*)
		{
			vect.clear();
			ret = false;
		}				
		// создаю вектор синхронных результатов
		if (ret)
		{
			auto itOut = vect.begin();
			for(auto it=m_VectList.begin(), ie=m_VectList.end();
												it!=ie; ++it, ++itOut)
			{
				*itOut = it->front();
				if (delta == 0)
				{
					if (itOut->time == timeMin)
						it->pop_front();
					else
						itOut->time = timeMin;
				}
				else
				{
					if (abs(itOut->time - timeMin) < delta)
						it->pop_front();
					else
						itOut->time = timeMin;
				}
			}
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
void CResultSynchronizationN<T>::Reset()
{
	if ( !m_VectList.empty() )
	{
		for (auto it=m_VectList.begin(), ie=m_VectList.end(); it!=ie; ++it)
			it->clear();
	}
}
//------------------------------------------------------------------------------