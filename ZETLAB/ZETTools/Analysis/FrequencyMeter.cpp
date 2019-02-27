#include "stdafx.h"
#include "FrequencyMeter.h"
#include <Intel/ipp/ipp.h>
#include "mathZet.h"

CFrequencyMeter::CFrequencyMeter()
	: m_dLastCheck(0)
	, m_dMaxInterval(MAX_PERIOD)
	, m_fSamplingRate(0)
	, m_fMaxValue(0)
	, m_fMinValue(0)
	, m_fUpperThreshold(0)
	, m_fLowerThreshold(0)
	, m_fLowerLimit(0)
	, m_bUpriseFront(true)
{
}

CFrequencyMeter::~CFrequencyMeter()
{
}

long CFrequencyMeter::AutoThreshold(float* data, long size, double time)
{
	SignalLimits lim;
	lim.time = time;
	ippsMinMax_32f(data, size, &lim.min_limit, &lim.max_limit);

	m_limits.push_back(lim);
	while (time - m_limits.front().time > m_dMaxInterval)
	{
		m_limits.pop_front();
	}

	float min_value(m_limits.front().min_limit);
	float max_value(m_limits.front().max_limit);
	for (auto iter : m_limits)
	{
		if (min_value > iter.min_limit)
			min_value = iter.min_limit;
		if (max_value < iter.max_limit)
			max_value = iter.max_limit;
	}

	float mean_value = (max_value + min_value) / 2;
	float delta_value = (max_value - min_value) / 6;

	m_fLowerThreshold = mean_value - delta_value;
	m_fUpperThreshold = mean_value + delta_value;

	float dynamic_range = log10(m_fMaxValue / m_fMinValue);
	m_fLowerLimit = m_fMinValue * pow(10.f, dynamic_range * 0.2f);
	//m_fLowerLimit = m_fMaxValue / 2000.f;
	if (m_fLowerLimit < m_fMinValue * 8.f)
		m_fLowerLimit = m_fMinValue * 8.f;
	return 0;
}

long CFrequencyMeter::CheckSignalAmplitude(float* data, long size)
{
	long ret_value(0);
	float min_value(0);
	float max_value(0);
	ippsMinMax_32f(data, size, &min_value, &max_value);

	float scale = max_value - min_value;
	if (scale < m_fLowerLimit)
		ret_value = 1;
	//else if (scale < m_fUpperThreshold - m_fLowerThreshold)
	//	ret_value = 1;
	
	return ret_value;
}

long CFrequencyMeter::CalculateFrequency(float* data, long size, double time, bool auto_threshold, float &frequency, long &periods)
{
	long error_value = 0;
	bool flag = false;

	if (auto_threshold)
	{
		long ret1 = AutoThreshold(data, size, time);
		long ret2 = CheckSignalAmplitude(data, size);
		if (ret2 == 0)
			flag = true;
	}
	else
		flag = true;


	frequency = 0.f;
	periods = 0;
	TRACE("CFrequencyMeter   input time %f\n", time);

	if (flag)
	{
		long first_index(-1);		// индекс начала первого периода
		long last_index(-1);		// индекс начала последнего периода
		long period_counter(0);		// количество полных периодов сигнала
		bool positive_wave(false);	// флаг нахождения на положительной полуволне

		// определяем начальное состояние пришедшего сигнала
		if (data[0] > m_fUpperThreshold)
			positive_wave = true;
		else
			positive_wave = false;

		// перебираем все точки сигнала и ищем начало следующего периода
		for (long i = 1; i < size; i++)
		{
			if (positive_wave)
			{
				if (data[i] < m_fLowerThreshold)
					positive_wave = false;
			}
			else
			{
				if (data[i] > m_fUpperThreshold)
				{
					if (first_index < 0)
						first_index = i;
					last_index = i;
					period_counter++;
					positive_wave = true;
				}
			}
		}
		periods = period_counter;
		period_counter--;	// уменьшаем количество периодов на 1 для правильного подсчёта

		// если насчитали более одного полного периода, то вычисляем частоту по текущему сигналу
		if (period_counter >= 1)
		{
			frequency = m_fSamplingRate * period_counter / (float(last_index) - float(first_index));
			m_dLastCheck = time + double(last_index) / m_fSamplingRate;
			TRACE("CFrequencyMeter   by INDEX    last %d    first %d    periods %d\n",
				last_index, first_index, period_counter);
		}
		// если насчитали менее одного полного периода, то используем данные предыдущего расчёта
		else
		{
			double current_check = time + double(last_index) / m_fSamplingRate;
			TRACE("CFrequencyMeter   by TIME   last %f   cur %f    period %f\n",
				m_dLastCheck, current_check, current_check - m_dLastCheck);
			// если не нашли ни одного периода, то возвращаем ошибку
			if (last_index < 0)
			{
				error_value = -1;
			}
			// если предыдущий период был зафиксирован позднее чем 3 секунды назад, то обнуляем частоту
			else if (current_check - m_dLastCheck > MAX_PERIOD)
			{
				frequency = 0;
				error_value = -1;
				if (last_index > 0)
					m_dLastCheck = time + double(last_index) / m_fSamplingRate;
			}
			// иначе считаем частоту
			else if (current_check - m_dLastCheck > 1e-6)
			{
				frequency = float(1. / (current_check - m_dLastCheck));
				m_dLastCheck = time + double(last_index) / m_fSamplingRate;
			}
			else
				error_value = -1;
		}
	}
	else
	{
		if (time - m_dLastCheck > m_dMaxInterval)
			error_value = 0;
		else
			error_value = -2;
	}

	return error_value;
}
