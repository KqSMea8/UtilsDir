/* ����� ������������ ��� ��������� ������� �������. 
*  ��� ��������� �������������� �������� ������������ ��������� �����
*  ����������� ������.
*  �����: ������� ������. ������. 2017.
*/
#pragma once
#include <list>

#define MAX_PERIOD		4.0		//! ������������ ������ ���������� ��������

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
	double m_dLastCheck;		//! ����� ����������� ����������� �������� ������
	double m_dMaxInterval;		//! ����������� �������� �������� ���������� �������
	float m_fSamplingRate;		//! ������� ������������� ������� ������
	float m_fMaxValue;			//! ������������ �������� �� �������������� ������
	float m_fMinValue;			//! ����������� �������� �� �������������� ������
	float m_fUpperThreshold;	//! ������� ��������� ��������
	float m_fLowerThreshold;	//! ������ ��������� ��������
	float m_fLowerLimit;		//! ����������� �������� ��������� ������� ����������� ��� ���������� �������
	bool m_bUpriseFront;		//! ���� ��� �������� ������� �� ����������� ������

	std::list<SignalLimits> m_limits;	//! ������������� �������� �������

protected:
	//! ������� ��� ��������������� ������� ��������� ��������
	long AutoThreshold(float* data, long size, double time);	

	//! ������� ��� �������� ������������� ��������� ������� ��� ������� �������
	long CheckSignalAmplitude(float* data, long size);

public:
	CFrequencyMeter();
	~CFrequencyMeter();

	//----------------------------------------------------------------------------------------------------
	//! ������� ��� ���������� ����������
	//! data - ��������� �� ������ � ��������
	//! size - ������ ������� � ��������
	//! time - ����� ������ ������� ������� �� �������
	//! auto_threshold - ���� ��������������� ������� ��������� ��������
	//! frequency - ������� ������� (������������ ��������)
	//! periods - ���������� ��������� ����� ��������� �������� 
	long CalculateFrequency(float* data, long size, double time, bool auto_threshold, float &frequency, long &periods);

	//-----------------------------------------------------------------------------------------------------
	//! ������� ��� ������� ����������
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

