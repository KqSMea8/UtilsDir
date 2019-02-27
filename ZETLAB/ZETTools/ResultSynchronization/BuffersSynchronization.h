//------------------------------------------------------------------------------
//		Ўаблонный класс синхронизации N однотипных результатов
//------------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------------
#include "afx.h"
#include <list>
#include <vector>
//------------------------------------------------------------------------------
const DWORD g_NumberChanMax = 271;
//------------------------------------------------------------------------------
template <class T>
class CBuffersSynchronization
{
private:
	std::vector< std::list<T> > m_VectList;	// вектор листов

public:
	CBuffersSynchronization(){ m_SizeMax = 10; }
	CBuffersSynchronization(const long numChan)
		{  m_SizeMax = 10; SetNumberChan(numChan); }

	DWORD m_SizeMax;	// максимальный размер буферов, т.е. list-ов

	// методы чтени€/записи свойств -----------------
	DWORD GetNumberChan() { return (DWORD)m_VectList.size(); }
	bool SetNumberChan(const DWORD numChan);

	// методы класса --------------------------------
	bool AddResult(const DWORD index, T &result);
	bool GetSynchroResult(std::vector<T> &vect);
	void Reset();
};
//------------------------------------------------------------------------------
template <class T>
bool CBuffersSynchronization<T>::SetNumberChan(const DWORD numChan)
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
bool CBuffersSynchronization<T>::AddResult(const DWORD index, T &result)
{
	bool ret = ( !m_VectList.empty() ) && ( index < (DWORD)m_VectList.size() );
	if (ret)
	{
		auto plist = &m_VectList[(size_t)index];
		plist->push_back(result);
		if ( plist->size() > m_SizeMax )
			plist->pop_front();
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
bool CBuffersSynchronization<T>::GetSynchroResult(
	std::vector<T> &vect)
{// вначале определ€ю наличие данных
	bool ret = !m_VectList.empty();
	if (ret)
	{
		for(auto it = m_VectList.cbegin(), ie=m_VectList.cend(); it!=ie; ++it)
		{
			if ( it->empty() )
			{
				ret = false;
				break;
			}
		}
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
					it->pop_front();
				}
			}
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
void CBuffersSynchronization<T>::Reset()
{
	if ( !m_VectList.empty() )
	{
		for (auto it=m_VectList.begin(), ie=m_VectList.end(); it!=ie; ++it)
			it->clear();
	}
}
//------------------------------------------------------------------------------