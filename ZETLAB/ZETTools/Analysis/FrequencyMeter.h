/* Класс предназначен для измерения частоты сигнала. 
*  Для измерения низкочастотных сигналов запоминается последняя точка
*  прохождения фронта.
*  Автор: Бегишев Сергей. ЗЭТЛАБ. 2017.
*/
#pragma once
#include <list>

#define MAX_PERIOD		4.0		//! максимальный период измеряемых сигналов

typedef struct _SignalLimits 
{
	float max_limit;
	float min_limit;
	double time;

	_SignalLimits()
	{
		max_limit = 0;
		min_limit = 0;
		time = 0;
	};
} SignalLimits;

class CFrequencyMeter
{
	double m_dLastCheck;		//! Время последненго прохождения сигналом порога
	double m_dMaxInterval;		//! Максиальный интервал ожидания очередного периода
	float m_fSamplingRate;		//! Частота дискретизации входных данных
	float m_fMaxValue;			//! Максимальное значение по измерительному каналу
	float m_fMinValue;			//! Минимальное значение по измерительному каналу
	float m_fUpperThreshold;	//! Верхнее пороговое значение
	float m_fLowerThreshold;	//! Нижнее пороговое значение
	float m_fLowerLimit;		//! Минимальное значение амплитуды сигнала необходимое для вычисления частоты
	bool m_bUpriseFront;		//! Флаг для подсчёта периода по восходящему фронту

	std::list<SignalLimits> m_limits;	//! экстремальные значения сигнала

protected:
	//! Функция для автоматического расчёта пороговых значений
	long AutoThreshold(float* data, long size, double time);	

	//! Функция для проверки достаточности амплитуды сигнала для расчёта частоты
	long CheckSignalAmplitude(float* data, long size);

public:
	CFrequencyMeter();
	~CFrequencyMeter();

	//----------------------------------------------------------------------------------------------------
	//! функции для выполнения вычислений
	//! data - указатель на массив с сигналом
	//! size - размер массива с сигналом
	//! time - время чтения массива сигнала из сервера
	//! auto_threshold - флаг автоматического расчёта пороговых значений
	//! frequency - частота сигнала (возвращаемой значение)
	//! periods - количество переходов через пороговое значение 
	long CalculateFrequency(float* data, long size, double time, bool auto_threshold, float &frequency, long &periods);

	//-----------------------------------------------------------------------------------------------------
	//! функции для задания параметров
	void SetSamplingRate(float sampling_rate) 
	{ 
		m_fSamplingRate = sampling_rate; 
	};
	void SetThresholds(float upper_threshold, float lower_threshold) 	
	{
		m_fUpperThreshold = upper_threshold;
		m_fLowerThreshold = lower_threshold;
	};
	void SetLimits(float max_value, float min_value)
	{
		m_fMaxValue = max_value;
		m_fMinValue = min_value;
	};
	void SetLowerLimit(float low_limit)
	{
		m_fLowerLimit = low_limit;
	};
	void SetMaxInterval(double time_interval)
	{
		m_dMaxInterval = time_interval;
	};


};

