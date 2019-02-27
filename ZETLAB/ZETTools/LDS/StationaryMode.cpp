//------------------------------------------------------------------------------
#include "StdAfx.h"
#include <LDS\StationaryMode.h>
//------------------------------------------------------------------------------
CStationaryMode::CStationaryMode()
	: m_WidthHalf(Min_Value_Half_Width)
	, m_valNotStat_Min(Min_Value_Half_Width)
	, m_valNotStat_Prob(0.f)
	, m_valNotStat_User(0.f)
	, m_Width(Min_Value_Half_Width + Min_Value_Half_Width)
	, m_bFull(false)
	, m_bChangeMode(false)
	, m_bThresholdEnabled(false)
	, m_size(0)
	, m_sizePage(1)
	, m_index(0)
	, m_indexMin(0)
	, m_indexMax(0)
	, m_valMin(FLT_MAX)
	, m_valMax(0.f)
	, m_ModeCur(lds_ts_Unknown)
	, m_pData(NULL)
{
	SetSize(300);
}
//------------------------------------------------------------------------------
CStationaryMode::CStationaryMode(const int size, const float valHalfWidth)
	: m_WidthHalf(Min_Value_Half_Width)
	, m_Width(Min_Value_Half_Width + Min_Value_Half_Width)
	, m_valNotStat_Min(Min_Value_Half_Width)
	, m_valNotStat_Prob(0.f)
	, m_valNotStat_User(valHalfWidth)
	, m_bFull(false)
	, m_bChangeMode(false)
	, m_bThresholdEnabled(false)
	, m_size(0)
	, m_sizePage(1)
	, m_index(0)
	, m_indexMin(0)
	, m_indexMax(0)
	, m_valMin(FLT_MAX)
	, m_valMax(0.f)
	, m_ModeCur(lds_ts_Unknown)
	, m_pData(NULL)
{
//	SetCurWidth();
	SetSize(size);
}
//------------------------------------------------------------------------------
// Конструктор копирования
CStationaryMode::CStationaryMode(const CStationaryMode &val)
	: m_valNotStat_Min(Min_Value_Half_Width)
	, m_size(0)
	, m_pData(NULL)
{
	*this = val;
}
//------------------------------------------------------------------------------
CStationaryMode::~CStationaryMode()
{
	if (m_size > 0)
	{
		delete [] m_pData;
		m_pData = nullptr;
	}
}
//------------------------------------------------------------------------------
void CStationaryMode::operator = (const CStationaryMode &val)
{
	m_Width = val.m_Width;
	m_WidthHalf = val.m_WidthHalf;
	m_valNotStat_Prob = val.m_valNotStat_Prob;
	m_valNotStat_User = val.m_valNotStat_User;
	m_indexMin = val.m_indexMin;
	m_indexMax = val.m_indexMax;
	m_valMin = val.m_valMin;
	m_valMax = val.m_valMax;
	m_sizePage = val.m_sizePage;
//	m_ModeCur = val.m_ModeCur;		позже
//	m_bChangeMode = true;			позже
//	m_bFull = val.m_bFull;			позже
//	m_index = val.m_index;			позже

//	m_size = val.m_size;	 не трогаю!!!! Но данные копирую
	if (val.m_size > 0)
	{
		if (m_size == 0)
		{
			m_size = val.m_size;
			m_pData = new float[m_size];
		}
		// копирование данных
		if (m_size > val.m_size)
		{// исходный буфер меньше по размеру
			m_bFull = false;
			m_index = val.m_bFull ? val.m_size : val.m_index;
			ippsCopy_32f(val.m_pData, m_pData, m_index);
		}
		else if (m_size == val.m_size)
		{// исходный буфер равен по размеру
			m_bFull = val.m_bFull;
			m_index = val.m_index;
			ippsCopy_32f(val.m_pData, m_pData, (m_bFull? m_size : m_index));
		}
		else// m_size < val.m_size
		{// исходный буфер больше по размеру
			if (val.m_bFull)
			{// исходный буфер полон, беру только последние данные
				m_bFull = true;
				m_index = 0;
				if (val.m_index >= m_size)
				{
					ippsCopy_32f(val.m_pData + val.m_index - m_size, m_pData, m_size);
				} 
				else
				{
					int num = m_size - val.m_index;
					ippsCopy_32f(val.m_pData + val.m_size - num, m_pData, num);
					ippsCopy_32f(val.m_pData, m_pData + num, val.m_index);
				}
				ippsMinMaxIndx_32f(m_pData, m_size,
					&m_valMin, &m_indexMin, &m_valMax, &m_indexMax);
			}
			else
			{// исходный буфер не полон
				if (val.m_index < m_size)
				{
					m_bFull = false;
					m_index = val.m_index;
					ippsCopy_32f(val.m_pData, m_pData, m_index);
				}
				else if (val.m_index == m_size)
				{
					m_bFull = true;
					m_index = 0;
					ippsCopy_32f(val.m_pData, m_pData, m_size);
				}
				else// val.m_index > m_size
				{
					m_bFull = true;
					m_index = 0;
					ippsCopy_32f(val.m_pData + val.m_index - m_size, m_pData, m_size);
					ippsMinMaxIndx_32f(m_pData, m_size,
						&m_valMin, &m_indexMin, &m_valMax, &m_indexMax);
				}
			}
		}
		CalcCurStationary();	// расчет режима стационарности, и флага изменения режима
	}
	else
		Clear();	// в исходном буфере нет массива
}
//------------------------------------------------------------------------------
// Задание размеров обоих массивов и перезапуск
bool CStationaryMode::SetSize(const int size)
{
	bool ret = size > 0;
	if (ret)
	{
		try
		{
			if (m_size != size)
			{
				if (m_size > 0)
				{
					delete [] m_pData;
					m_pData = nullptr;
				}
				m_pData = new float [size];
				m_size = size;
			}
			Clear();
		}
		catch(...)
		{
			ret = false;
		}
	}
	else
	{
		if (m_size > 0)
		{
			delete[] m_pData;
			m_size = 0;
			Clear();
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
// Добавление в массив исходных данных
bool CStationaryMode::Add(
	const float val,
	Lds_type_Stationary *pTypeStationary,
	bool *pbChangeMode)
{
	bool ret = m_size > 0;
	if (ret)
	{
		m_pData[m_index] = val;
		if ((m_index == m_indexMin) || (m_index == m_indexMax))
		{
			int size = m_bFull ? m_size : m_index;
			ippsMinMaxIndx_32f(m_pData, size,
				&m_valMin, &m_indexMin, &m_valMax, &m_indexMax);
		}
		else
		{
			if (val < m_valMin)
			{
				m_valMin = val;
				m_indexMin = m_index;
			}
			if (m_valMax < val)
			{
				m_valMax = val;
				m_indexMax = m_index;
			}
		}
		if (++m_index == m_size)
		{
			m_index = 0;
			m_bFull = true;
		}

		CalcCurStationary();
		if (pTypeStationary != NULL)
			*pTypeStationary = m_ModeCur;
		if (pbChangeMode != NULL)
			*pbChangeMode = m_bChangeMode;
	}
	return ret;
}
//------------------------------------------------------------------------------
// расчет режима стационарности, и флага изменения режима
void CStationaryMode::CalcCurStationary()
{
	Lds_type_Stationary mode;
	if (m_bFull && m_bThresholdEnabled)
		mode = ((m_valMax - m_valMin) > m_Width) ? lds_ts_No : lds_ts_Yes;
	else
		mode = lds_ts_Unknown;
	m_bChangeMode = m_ModeCur != mode;
	m_ModeCur = mode;
}
//------------------------------------------------------------------------------
bool CStationaryMode::CopyDataToSpace(CSpace<float> &sp)
{
	bool ret = m_bFull;
	if (ret)
	{
		float mean;
		int size = m_bFull ? m_size : m_index;
		ippsMean_32f(m_pData, size, &mean, ippAlgHintFast);
		sp.SetSpaceFromHalf(mean, 
			max(m_WidthHalf, max(m_valMax - mean, mean - m_valMin)) );
	}
	return ret;
}
//------------------------------------------------------------------------------
// Обнуление массивов, без изменения размеров и порогов
void CStationaryMode::Clear()
{
	m_bFull = false;
	m_bChangeMode = false;
	m_bThresholdEnabled = false;
	m_index = m_indexMin = m_indexMax = 0;
	m_ModeCur = lds_ts_Unknown;
	m_valMin = FLT_MAX;
	m_valMax = 0.f;
}
//------------------------------------------------------------------------------
// Актуализация текущего случайного значения.
// Возвращает true если заканчивается режим BadParam
// Полуширина delta = 0.5f * (m_valMax - m_valMin) + factor * sigma
// Для P=0.95 0.5f * (m_valMax - m_valMin) ~= 2 * sigma, поэтому так:
bool CStationaryMode::CalcThresholds()
{
	bool ret = m_bFull && (m_size > 0);
	if (ret)
	{
		m_valNotStat_Prob = m_valMax - m_valMin;
		m_WidthHalf = max(m_valNotStat_User,
							max(m_valNotStat_Prob, m_valNotStat_Min));
		m_Width = 2 * m_WidthHalf;
		m_bThresholdEnabled = true;
	}
	return ret;
}
//------------------------------------------------------------------------------
void CStationaryMode::SetSizePage(UINT newVal)
{
	if (newVal > 0.f)
	{
		m_valNotStat_Min *= sqrt((float)m_sizePage / (float)newVal);
		m_sizePage = newVal;
	}
}
//------------------------------------------------------------------------------