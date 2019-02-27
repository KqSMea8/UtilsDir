//------------------------------------------------------------------------------
// Ўаблолнный класс, реализующий расчЄт скольз€шего среднего
#pragma once
#include "afx.h"
//------------------------------------------------------------------------------
template <class T>
class CMovingAverage
{
private:
	bool m_bFlag;
	DWORD m_counter;
	DWORD m_counterMax;
	T m_average;
	double m_alfa;
	double m_alfa_1;

public:
	CMovingAverage(DWORD counterMax = 20);
	virtual ~CMovingAverage() {}

	T GetAverage() { return m_average; }

	void Init(DWORD counterMax);
	void ReStart();
	T Calculation(const T newValue);
};
//------------------------------------------------------------------------------
template <class T>
CMovingAverage<T>::CMovingAverage(DWORD counterMax)
	: m_bFlag(false)
	, m_counter(0)
	, m_counterMax(20)
	, m_average((T)0)
	, m_alfa(0.05)
	, m_alfa_1(0.95)
{
	Init(counterMax);
}
//------------------------------------------------------------------------------
template <class T>
void CMovingAverage<T>::Init(DWORD counterMax)
{
	if ((counterMax > 0) && (m_counterMax != counterMax))
	{
		m_counterMax = counterMax;
		m_alfa = 1. / m_counterMax;
		m_alfa_1 = 1. - m_alfa;
	}
	ReStart();
}
//------------------------------------------------------------------------------
template <class T>
void CMovingAverage<T>::ReStart()
{
	m_bFlag = false;
	m_counter = 0;
	m_average = (T)0;
}
//------------------------------------------------------------------------------
template <class T>
T CMovingAverage<T>::Calculation(const T newValue)
{
	if (m_bFlag)
	{
		m_average = (T)(m_alfa_1 * m_average + m_alfa * newValue);
	}
	else
	{
		m_bFlag = ++m_counter >= m_counterMax;
		double alfa = 1. / m_counter;
		double alfa_1 = 1. - alfa;
		m_average = (T)(alfa_1 * m_average + alfa * newValue);
	}
	return m_average;
}
//------------------------------------------------------------------------------