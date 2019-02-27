//------------------------------------------------------------------------------
//	Файл BufferForDataWithTime.h
//	Зеленоград, ЗАО "ЭТМС", Савостин Ю.М.
//------------------------------------------------------------------------------
/*		Шаблонный класс реализует буфер для хранения и выдачи данных,
имеющих качество и время		*/
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <list>
#include <mathZet.h>
#include <CodeQuality.h>
#include <WideResult\WideResult.h>
//------------------------------------------------------------------------------
template <class T>
class CBufferForDataWithTime
{
private:
	bool m_bInit;
	double m_dt;
	DWORD m_sizeMax;
	std::list<WideResult<T>> m_list;
	WideResult<T> m_wr_SkipData;

	void AddOneNoTest(WideResult<T> &newWR);
	void AddOneWithTest(WideResult<T> &newWR);

protected:
public:
	CBufferForDataWithTime();
	CBufferForDataWithTime(const CBufferForDataWithTime &val);
	virtual ~CBufferForDataWithTime();
	// size - размер буфера, frequency - частота дискретизации данных
	bool Create(DWORD size, T frequency);

	// методы чтения/записи свойств -----------------
	DWORD GetSize() { return m_sizeMax; }
	double CurrentTime();
	double GetFirstTime();

	// методы класса ----------------------------
	bool AddOneData(double time, T val, long quality);
	bool AddData(double time, int number, T *pData, long quality);
	long GetData(double time, int number, T *pData, long *pQuality = NULL);
};
//------------------------------------------------------------------------------
template <class T>
CBufferForDataWithTime<T>::CBufferForDataWithTime()
	: m_bInit(false)
	, m_sizeMax(0)
	, m_dt(0.)
{
	CodeQuality skipData;
	skipData.Synchro = 1;
	m_wr_SkipData.quality = skipData;
	m_wr_SkipData.value = std::numeric_limits<T>::quiet_NaN();
}
//------------------------------------------------------------------------------
template <class T>
CBufferForDataWithTime<T>::~CBufferForDataWithTime()
{
	if (m_sizeMax > 0)
	{
		m_list.clear();
	}
}
//------------------------------------------------------------------------------
template <class T>
bool CBufferForDataWithTime<T>::Create(
	DWORD size,			// размер буфера для хранения отсчётов
	T frequency)	// частота дискретизации хранимых отсчётов
{
	m_bInit = (size > 0) && (frequency > (T)0);
	if (m_bInit && (m_sizeMax < size))
	{
		m_dt = 1.f / frequency;
		try
		{
			m_sizeMax = size;
			m_list.resize(m_sizeMax);
		}
		catch (CMemoryException*)
		{
			m_sizeMax = 0;
			m_list.clear();
			m_bInit = false;
		}
	}
	return m_bInit;
}
//------------------------------------------------------------------------------
template <class T>
// Приватная функция добавления одного отчёта в конец буфера
void CBufferForDataWithTime<T>::AddOneNoTest(WideResult<T> &newWR)
{// Вначале удалить, потом добавить, чтобы resize не вызывался
	if (m_list.size() == m_sizeMax)
		m_list.pop_front();
	m_list.push_back(newWR);
}
//------------------------------------------------------------------------------
template <class T>
// Приватная функция добавления одного отчёта в буфер. Место в буфере
// определяется в соответствии со временем отсчёта.
void CBufferForDataWithTime<T>::AddOneWithTest(WideResult<T> &newWR)
{
	double lastTime = m_list.back().time;
	// проверяю время
	double timeApproximate = lastTime + m_dt;
	if (IsEqual(timeApproximate, newWR.time, 0.001))
	{// штатная ситуация
		AddOneNoTest(newWR);
	}
	else
	{/* Не штатная ситуация, возможны 3 варианта:
	 1. время отсчета сильно впереди, надо добавить NaN-ы;
	 2. в буфере уже есть такое время, надо заменить данные;
	 3. время отсчета сильно позади, точка уже никому не нужна		*/
		if (newWR.time > timeApproximate)
		{// Вариант 1.
			WideResult<T> temp = m_wr_SkipData;
			temp.time = timeApproximate;
			int n = (int)floor((newWR.time - lastTime) / m_dt + 0.5) - 1;
			for (int i = 0; i < n; ++i)
			{
				AddOneNoTest(temp);	// добавляю NaN с расчётным временем
				temp.time += m_dt;
			}
			AddOneNoTest(newWR);
		}
		else
		{
			if (newWR.time >= m_list.front().time)
			{// Вариант 2.
				int n = (int)floor((lastTime - newWR.time) / m_dt + 0.5);
				// вероятность, что newWR.time ближе к GetLastTime, выше,
				// поэтому пойду с конца
				auto it = m_list.end(), ie = m_list.begin();
				--it;
				for (int i = 0; i < n; ++i, --it)
				{
					if (it != ie)
						--it;
					else
						break;
				}
				it->value = newWR.value;
				it->quality = newWR.quality;
			}
			// else Вариант 3. - ничего делать не надо
		}
	}
}
//------------------------------------------------------------------------------
template <class T>
double CBufferForDataWithTime<T>::CurrentTime()
{
	return (m_list.size() > 0) ? m_list.back().time : 0.;
}
//------------------------------------------------------------------------------
template <class T>
double CBufferForDataWithTime<T>::GetFirstTime()
{
	return (m_list.size() > 0) ? m_list.front().time : 0.;
}
//------------------------------------------------------------------------------
template <class T>
// Публичная функция добавления в буфер одного отсчета
bool CBufferForDataWithTime<T>:: AddOneData(double time, T val, long quality)
{
	static WideResult<T> temp;
	bool ret = m_bInit;
	if (ret)
	{
		temp.quality = quality;
		temp.time = time;
		temp.value = val;
		AddOneWithTest(temp);
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
// Публичная функция добавления в буфер массива отсчетов
bool CBufferForDataWithTime<T>::AddData(
	double time,	// время последнего отсчёта
	int number,		// размер массива
	T *pData,		// массив
	long quality)	// качество данных
{
	static WideResult<T> temp;
	bool ret = m_bInit && (number > 0);
	if (ret)
	{
		temp.quality = quality;
		int n_1 = number - 1;
		int i = 0;
		// первые точки без последней
		temp.time = time - m_dt * n_1;
		for (; i < n_1; ++i)
		{
			temp.value = pData[i];
			AddOneWithTest(temp);
			temp.time += m_dt;
		}
		// последняя точка
		temp.time = time;
		temp.value = pData[i];
		AddOneWithTest(temp);
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
// Чтение из буфера массива данных
long CBufferForDataWithTime<T>::GetData(
	double time,	// время последнего отсчёта
	int number,		// размер массива
	T *pData,		// массив
	long *pQuality)	// качество данных
{// отрицательные коды возврата заданы как у одноимённой функции ZetServer
	long ret(0);
	if (m_bInit)
	{
		if (pData == NULL)
			ret = -13;
		if (ret == 0)
		{
			if ((0 <= number) || (number > (int)m_sizeMax))
				ret = -8;
			if (ret == 0)
			{
				if (time < 0.)
					ret = -12;
				if (ret == 0)
				{
					if (time < (m_list.front().time + number * m_dt))
						ret = -14;
					if (ret == 0)
					{
						if (time > m_list.back().time)
							ret = -11;
						if (ret == 0)
						{// данные есть, ищу их и возвращаю
							auto it = m_list.cend(), ie = m_list.cbegin();
							--it;
							while (true)
							{// чаще требуемые данные в хвосте
								if ((time <= it->time) || (it == ie))
									break;
								else
									--it;
							}
							if (pQuality)
								*pQuality = 0;
							T *padd = pData + number;
							for (int i = number - 1; i <= 0; --i, --padd)
							{
								if (it != ie)
								{
									*padd = it->value;
									if (pQuality)
										*pQuality |= it->quality;
									--it;
								}
								else
								{
									*padd = std::numeric_limits<T>::quiet_NaN();
								}
							}
						}
					}
				}
			}
		}
	}
	else ret = -1;	// нет буфера с данными
	return ret;
}
//------------------------------------------------------------------------------