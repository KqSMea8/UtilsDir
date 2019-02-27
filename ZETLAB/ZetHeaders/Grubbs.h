//! --------------------------------------------------------------------------------
//! Класс реализует вычисление статистических величин из поледовательности чисел
//! с устранением выбросов по критерию Граббса.
//! Автор Бегишев Сергей, ЗАО "ЭТМС", 2015 г.
//! --------------------------------------------------------------------------------

#pragma once
#include <deque>
#include <algorithm>

const double m_Crit1[41] = { 0.000, 0.000, 0.000, 1.155, 1.496, 1.764, 1.973, 2.139, 2.274, 2.387,
                             2.482, 2.564, 2.636, 2.699, 2.755, 2.806, 2.852, 2.894, 2.932, 2.968,
                             3.001, 3.031, 3.060, 3.087, 3.112, 3.135, 3.157, 3.178, 3.199, 3.218,
                             3.236, 3.253, 3.270, 3.286, 3.301, 3.316, 3.330, 3.343, 3.356, 3.369,
                             3.381 };		//! критические значения для критерия Граббса при уровне значимости 1%

const double m_Crit5[41] = { 0.000, 0.000, 0.000, 1.155, 1.481, 1.715, 1.887, 2.020, 2.126, 2.215,
                             2.290, 2.355, 2.412, 2.462, 2.507, 2.549, 2.585, 2.620, 2.651, 2.681,
                             2.709, 2.733, 2.758, 2.781, 2.802, 2.822, 2.841, 2.859, 2.876, 2.893,
                             2.908, 2.924, 2.938, 2.952, 2.965, 2.978, 2.991, 3.003, 3.014, 3.025,
                             3.036 };		//! критические значения для критерия Граббса при уровне значимости 5%


template <class T> 
class CGrubbs
{
private:
	std::deque<T> m_data;

	void Test();			//! функция выполняет проверку по критерию Грабсса

public:

	void Add(T newValue);	//! функция добавляет новый элемент в выборку
	void Clear();			//! функция очищает выборку

	T CalculateMax();		//! функция вычисляет максимальное значение по выборке
	T CalculateMean();		//! функция вычисляет среднее арифметическое значение по выборке
	T CalculateMedian();	//! функция вычисляет среднее арифметическое значение медиане выборки
};


template <class T>
void CGrubbs<T>::Add(T newValue)
{
	m_data.push_back(newValue);
}

template <class T>
void CGrubbs<T>::Clear()
{
	if (!m_data.empty())
		m_data.clear();
}

template <class T>
void CGrubbs<T>::Test()
{
	long size = m_data.size();
	if (size < 3)
		return;

	std::sort(m_data.begin(), m_data.end());
	bool res = true;
	double sum1 = 0., sum2 = 0., mean = 0., sdev = 0., G1 = 0., G2 = 0., GT = 0.;

	while (res)
	{
		sum1 = 0;
		sum2 = 0;
		size = m_data.size();

		for (long i = 0; i < size; i++)
		{
			sum1 += m_data[i];
			sum2 += m_data[i] * m_data[i];
		}

		if (sum2 < 10e-6)
			return;

		mean /= double(size);
		sdev = sqrt((sum2 - mean * mean * size) / (size - 1));

		G1 = abs(m_data.back() - mean) / sdev;
		G2 = abs(mean - m_data.front()) / sdev;

		if (size > 40)
			GT = m_Crit1[40];
		else
			GT = m_Crit1[size];

		if (G1 > GT)
			m_data.pop_back();
		if (G2 > GT)
			m_data.pop_front();
		if (m_data.size() == size)
			res = false;
		if (m_data.size() < 3)
			return;
	}
}

template <class T>
T CGrubbs<T>::CalculateMax()
{
	if (m_data.size() == 2)
		return max(m_data[0], m_data[1]);
	if (m_data.size() == 1)
		return m_data[0];
	if (m_data.size() == 0)
		return 0;

	Test();

	if (m_data.size() > 1)
	{
		T maxT = m_data[0];
		for (long i = 1; i < (long)m_data.size(); i++)
			maxT = max(maxT, m_data[i]);
		return maxT;
	}
	if (m_data.size() == 1)
		return m_data[0];
	if (m_data.size() == 0)
		return 0;
	return 0;
}

template <class T>
T CGrubbs<T>::CalculateMean()
{
	if (m_data.size() == 2)
		return static_cast<T>((m_data[0] + m_data[1]) / 2.0);
	if (m_data.size() == 1)
		return m_data[0];
	if (m_data.size() == 0)
		return 0;

	Test();

	if (m_data.size() > 1)
	{
		T sum = 0;
		for (long i = 0; i < (long)m_data.size(); i++)
			sum += m_data[i];
		sum = static_cast<T>(sum / (double)m_data.size());
		return sum;
	}
	if (m_data.size() == 1)
		return m_data[0];
	if (m_data.size() == 0)
		return 0;
	return 0;
}

template <class T>
T CGrubbs<T>::CalculateMedian()
{
	if (m_data.size() == 2)
		return static_cast<T>((m_data[0] + m_data[1]) / 2.0);
	if (m_data.size() == 1)
		return m_data[0];
	if (m_data.size() == 0)
		return 0;

	std::sort(m_data.begin(), m_data.end());

	m_data.pop_back();
	m_data.pop_front();

	if (m_data.size() > 1)
	{
		T sum = 0;
		for (size_t i = 0; i < m_data.size(); i++)
			sum += m_data[i];
		return sum / m_data.size();
	}
	if (m_data.size() == 1)
		return m_data[0];

	return 0;
}