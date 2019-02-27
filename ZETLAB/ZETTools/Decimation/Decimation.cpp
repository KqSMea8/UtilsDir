//------------------------------------------------------------------------------
//	Файл Decimation.cpp
//	Зеленоград, ЗАО "ЭТМС", Савостин Ю.М.
//------------------------------------------------------------------------------
#include "StdAfx.h"
#include <Decimation\Decimation.h>
//------------------------------------------------------------------------------
CDecimation::CDecimation()
	: m_bInit(false)
	, m_bEnableFirst(false)
	, m_index(0)
	, m_indexMax(10)
	, m_FreqADC(0.)
	, m_dt(0.)
	, m_precision(0.001)
	, m_lastTime(0.)
	, m_pBufferDecimal(NULL)
	, m_pFiltr(NULL)
	, m_dataDouble()
{}
//------------------------------------------------------------------------------
CDecimation::~CDecimation()
{
	if (m_pFiltr)
	{
		delete m_pFiltr;
		m_pFiltr = nullptr;
	}
	
	if (m_pBufferDecimal)
	{
		delete m_pBufferDecimal;
		m_pBufferDecimal = nullptr;
	}
}
//------------------------------------------------------------------------------
bool CDecimation::Create(float freqADC, DWORD sizeDecimalBuffer)
{
	m_bInit = freqADC > 0.f;
	if (m_bInit)
	{// По умолчанию размеры буферов создаются под интервал в 300 сек
		m_FreqADC = freqADC;
		m_dt = 1 / m_FreqADC;
		// буфер для цецимированных данных
		DWORD size = (sizeDecimalBuffer > 0) ?
						sizeDecimalBuffer : (DWORD)(int)floor(freqADC * 30);
		m_pBufferDecimal = new CBufferForDataWithTime<double>();
		m_pBufferDecimal->Create(size, m_FreqADC * 0.1);
		// фильтр
		m_pFiltr = new CIirFiltrInternalDouble();
		m_pFiltr->Create(&m_FreqADC);
		m_pFiltr->InitFiltr(tfLowPass, 0.05 * m_FreqADC);
	}
	return m_bInit;
}
//------------------------------------------------------------------------------
long CDecimation::GetData(double time, int number, double *pData, long *pQuality)
{
	return m_pBufferDecimal->GetData(time, number, pData, pQuality);
}
//------------------------------------------------------------------------------
long CDecimation::GetData(double time, int number, float *pData, long *pQuality)
{
	long ret = m_dataDouble.ReSize((UINT)number) ? 0 : -1;
	if (ret == 0)
		ret = GetData(time, number, m_dataDouble.pData, pQuality);
	if (ret == 0)
		ippsConvert_64f32f(m_dataDouble.pData, pData, number);
	return ret;
}
//------------------------------------------------------------------------------
bool CDecimation::PutData(double time, int number, float *pData, long quality)
{
	bool ret = m_bInit && (time > 0.) && (number > 0) && (pData != NULL);
	if (ret)
	{	// проверка времени
		double lastTime = m_lastTime + number * m_dt;
		if (IsNotEqual(lastTime, time, m_precision))
		{
			if (time > lastTime)
			{// были пропуски
				m_pFiltr->ReStart();
				m_bEnableFirst = false;
			}
			else
				ret = false;// пришли пропущенные данные, они уже не актуальны
		}		
		if (ret)
		{// штатный режим работы
			// фильтрация
			ret = m_dataDouble.ReSize((UINT)number) &&
				(ippsConvert_32f64f(pData, m_dataDouble.pData, number) == ippStsNoErr) &&
				m_pFiltr->Filtration_I(m_dataDouble.pData, number);

			if (ret)
			{
				if (m_bEnableFirst)
				{
					for (int i = 0; i < number; ++i)
					{
						if (++m_index >= m_indexMax)
						{
							m_index = 0;
							m_pBufferDecimal->AddOneData(time - (number - 1 - i) * m_dt,
								m_dataDouble.pData[i], quality);
						}
					}
				}
				else
				{// Ищу отсчет со временем, равным целому числу сек
					int i = 0;
					double timeCur = time - (number - 1) * m_dt;
					for (; i < number; ++i)
					{
						if (IsEqual(timeCur - floor(timeCur), 0., m_precision))
						{
							m_bEnableFirst = false;
							m_index = 0;
							m_pBufferDecimal->AddOneData(timeCur,
								m_dataDouble.pData[i], quality);
							break;
						}
						timeCur += m_dt;
					}
					for (++i; i < number; ++i)
					{
						if (++m_index >= m_indexMax)
						{
							m_index = 0;
							m_pBufferDecimal->AddOneData(time - (number - 1 - i) * m_dt,
								m_dataDouble.pData[i], quality);
						}
					}
				}
			}
		}
		m_lastTime = time;
	}
	return ret;
}
//------------------------------------------------------------------------------