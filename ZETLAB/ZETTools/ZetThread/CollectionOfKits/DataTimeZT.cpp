//------------------------------------------------------------------------------
#include "stdafx.h"
#include <mathZet.h>
#include "DataTimeZT.h"
//------------------------------------------------------------------------------
CDataTimeZT::CDataTimeZT()
	: m_zts_dtBuf (1i64, 0U)	// 1 сек

	, m_zts_dtRead (1i64, 0U)	// 1 сек
	, m_zts_dtСalc (1i64, 0U)	// 1 сек
	, m_zts_dtAver (1i64, 0U)	// 1 сек

	, m_zts_read (0i64, 0U)		// 0 сек
	, m_zts_calc (1i64, 0U)		// 1 сек
	, m_zts_aver (1i64, 0U)		// 1 сек
{
}
//------------------------------------------------------------------------------
CDataTimeZT::~CDataTimeZT()
{
}
//------------------------------------------------------------------------------
CDataTimeZT::CDataTimeZT(const CDataTimeZT & val)
{
	*this = val;
}
//------------------------------------------------------------------------------
CDataTimeZT & CDataTimeZT::operator = (const CDataTimeZT & val)
{
	m_zts_dtBuf = val.m_zts_dtBuf;

	m_zts_dtRead = val.m_zts_dtRead;
	m_zts_dtСalc = val.m_zts_dtСalc;
	m_zts_dtAver = val.m_zts_dtAver;

	m_zts_read = val.m_zts_read;
	m_zts_calc = val.m_zts_calc;
	m_zts_aver = val.m_zts_aver;

	return *this;
}
//------------------------------------------------------------------------------
// Интервал времени для расчета размера буфера
CZetTimeSpan CDataTimeZT::GetIntervalBuffer()
{
	return m_zts_dtBuf;
}

void CDataTimeZT::SetIntervalBuffer(const CZetTimeSpan & ztInterval)
{
	if ((CZetTimeSpan)ztInterval > CZetTimeSpan(0i64, 0U))
		m_zts_dtBuf = ztInterval;
}
//------------------------------------------------------------------------------
// Период чтения данных
CZetTimeSpan & CDataTimeZT::GetIntervalRead()
{
	return m_zts_dtRead;
}

void CDataTimeZT::SetIntervalRead(const CZetTimeSpan & ztInterval)
{
	if ((CZetTimeSpan)ztInterval > CZetTimeSpan(0i64, 0U))
		m_zts_dtRead = ztInterval;
}

void CDataTimeZT::SetIntervalRead(const double dInterval)
{
	if (dInterval > 0.)
		m_zts_dtRead = dInterval;
}
//------------------------------------------------------------------------------
// Период расчета промежуточных результатов
CZetTimeSpan & CDataTimeZT::GetIntervalCalc()
{
	return m_zts_dtСalc;
}

void CDataTimeZT::SetIntervalCalc(const CZetTimeSpan & ztInterval, const bool bRound)
{
	if ((CZetTimeSpan)ztInterval > CZetTimeSpan(0i64, 0U))
		m_zts_dtСalc = ztInterval;
	
	if (bRound)
	{
		m_zts_dtСalc.RoundSeconds();
		if (m_zts_dtСalc < m_zts_dtRead)
			m_zts_dtСalc = m_zts_dtRead;
	}
}

void CDataTimeZT::SetIntervalCalc(const double dInterval, const bool bRound)
{
	if (dInterval > 0.)
		m_zts_dtСalc = dInterval;

	if (bRound)
	{
		m_zts_dtСalc.RoundSeconds();
		if (m_zts_dtСalc < m_zts_dtRead)
			m_zts_dtСalc = m_zts_dtRead;
	}
}
//------------------------------------------------------------------------------
// Период усреднения промежуточных результатов
CZetTimeSpan & CDataTimeZT::GetIntervalAver()
{
	return m_zts_dtAver;
}

void CDataTimeZT::SetIntervalAver(const CZetTimeSpan & ztInterval)
{
	if ((CZetTimeSpan)ztInterval > CZetTimeSpan(0i64, 0U))
		m_zts_dtAver = ztInterval;
}

void CDataTimeZT::SetIntervalAver(const double dInterval)
{
	if (dInterval > 0.)
		m_zts_dtAver = dInterval;
}
//------------------------------------------------------------------------------
// Время чтения данных
CZetTimeSpan & CDataTimeZT::GetTimeRead()
{
	return m_zts_read;
}

void CDataTimeZT::SetTimeRead(const CZetTimeSpan & zTime)
{
	m_zts_read = zTime;
}
//------------------------------------------------------------------------------
// Время расчета промежуточных результатов
CZetTimeSpan & CDataTimeZT::GetTimeCalc()
{
	return m_zts_calc;
}

void CDataTimeZT::SetTimeCalc(const CZetTimeSpan & zTime)
{
	m_zts_calc = zTime;
}
//------------------------------------------------------------------------------
// Время усреднения промежуточных результатов
CZetTimeSpan & CDataTimeZT::GetTimeAver()
{
	return m_zts_aver;
}

void CDataTimeZT::SetTimeAver(const CZetTimeSpan & zTime)
{
	m_zts_aver = zTime;
}
//------------------------------------------------------------------------------





//------------------------------------------------------------------------------
void CDataTimeZT::SetTimeReadStart(const double timeCur)
{
	m_zts_read = timeCur;
	m_zts_read.RoundSecondsUp();

	if ((0i64 == m_zts_dtRead.GetSeconds()) && (0 < m_zts_dtRead.GetNanoseconds()))
		m_zts_read += m_zts_dtRead;
}
//------------------------------------------------------------------------------
void CDataTimeZT::IncrementTime()
{
	m_zts_read += m_zts_dtRead;
}
//------------------------------------------------------------------------------
// Инкремент времени усреднения данных
void CDataTimeZT::IncrementTimeAver()
{
	m_zts_aver += m_zts_dtAver;
}
//------------------------------------------------------------------------------
double CDataTimeZT::GetTime()
{
	return (double)m_zts_read;
}
//------------------------------------------------------------------------------
// Инкремент времени расчета промежуточных результатов
void CDataTimeZT::IncrementTimeCalc()
{
	m_zts_calc += m_zts_dtСalc;
}
//------------------------------------------------------------------------------
// Проверка нвступления времени расчета промежуточных результатов
bool CDataTimeZT::IsTimeCalc()
{
	return m_zts_read >= m_zts_calc;
}
//------------------------------------------------------------------------------
// Проверка завершения времени усреднения данных
bool CDataTimeZT::IsTimeAver()
{
	return m_zts_read >= m_zts_aver;
}
//------------------------------------------------------------------------------
// Сдвиг времен класса при перестартовке сервера
// zts_dt - разница нового и старого времен старта сервера
// (положительная величина)
void CDataTimeZT::ReStartSRV(const CZetTimeSpan & zts_dt)
{
	m_zts_read -= zts_dt;
}
//------------------------------------------------------------------------------
// Установка первых значений времён чтения, расчета и усреднения в
// соответствии с правилами синхронизации работы нескольких программ.
// Периоды чтения, расчета и усреднения должны быть заданы.
// timeCur - текущее время сервера;
// timeForInit - время инициализации.
// Устанавливаются следующии значения:
// TimeRead = RoundingByRules(timeCur - timeForInit);
// TimeCalc = TimeRead + timeForInit;
// TimeAver = RoundingByRules(timeCur + IntervalTimeAver)

void CDataTimeZT::SetFirstTimesByRules(
	const double timeCur,		// текущее время сервера
	const double timeForInit)	// время инициализации
{	// Время первого чтения данных ---------------------------------------------
	bool bNotOk(true);
	double tRead = timeCur - timeForInit;
	double dtAver = m_zts_dtAver;

	if (m_zts_dtAver > CZetTimeSpan(1, 0))
	{	// Проверка m_zts_aver на кратность интервала усреднения суткам.
		// Если m_zts_aver меньше 1 мин и кратно минуте, m_zts_aver кратно и суткам
		// Если m_zts_aver меньше 1 час и кратно часу, m_zts_aver кратно и суткам
		if ((RoundToInt(86400 / dtAver) * dtAver == 86400.))
		{
			bNotOk = false;
			m_zts_read = floor(tRead / dtAver) * dtAver;
		}
	}
	if (bNotOk)
	{
		m_zts_read = tRead;
		m_zts_read.RoundSecondsDown();
	}
	CZetTimeSpan m_zts_read_round(m_zts_read);
	// если интервал времени чтения меньше 1 сек
	if ((0i64 == m_zts_dtRead.GetSeconds()) && (0 < m_zts_dtRead.GetNanoseconds()))
			m_zts_read += m_zts_dtRead;

	// Теперь время первого усреднения данных ---------------------------------
	int m = RoundToInt(timeForInit / dtAver);
	double t = m * dtAver;
	CZetTimeSpan zts(t);
	if (timeForInit > t)
		zts += m_zts_dtAver;
	m_zts_aver = m_zts_read_round + zts + m_zts_dtAver;

	// время первого расчета
	zts = (double)m_zts_read + timeForInit;
	zts.RoundSeconds();
	SetTimeCalc(zts);
}
//------------------------------------------------------------------------------