//------------------------------------------------------------------------------
#include "stdafx.h"
#include "Dialog_ZET\DataTime.h"
//------------------------------------------------------------------------------
// ѕо умолчанию работа с кодами и код = 0, т.е. врем€ инкремента = 0,1 сек
CDataTime::CDataTime()
	: m_bCodeMode(true)
	, m_bCalcNumberMode(false)
	, m_bZetMode(false)
	, m_bInDependentMode(false)
	, m_codeFast(0)
	, m_mode10sec(m10s_every_1sec)
	, m_timeAverage(1.)
	, m_zdt(0.1)
	, m_zTimeRead(0.)
	, reserved3(0)
{}
//------------------------------------------------------------------------------
CDataTime::CDataTime(const CDataTime & val)
{
	*this = val;
}
//------------------------------------------------------------------------------
CDataTime & CDataTime::operator = (const CDataTime & val)
{
	m_bCodeMode = val.m_bCodeMode;
	m_bCalcNumberMode = val.m_bCalcNumberMode;
	m_bZetMode = val.m_bZetMode;
	m_bInDependentMode = val.m_bInDependentMode;
	m_codeFast = val.m_codeFast;
	m_mode10sec = val.m_mode10sec;
	m_timeAverage = val.m_timeAverage;
	m_zdt = val.m_zdt;
	m_zTimeRead = val.m_zTimeRead;

	return *this;
}
//------------------------------------------------------------------------------
// Cпособ расчЄта кол-ва считываемых отстетов:
// true - по m_timeAverage, false - по m_zdt
bool CDataTime::GetCalcNumberMode()
{
	return m_bCalcNumberMode;
}
//------------------------------------------------------------------------------
void CDataTime::SetCalcNumberMode(bool bCalcByAver)
{
	m_bCalcNumberMode = bCalcByAver;
}
//------------------------------------------------------------------------------
long CDataTime::TestFast(const long val)
{	// реализаци€ ограничений 0 <= val <= 2
	long ret;
	if (val < 0)
		ret = 0;
	else if (val > 2)
		ret = 2;
	else
		ret = val;
	return ret;
}
//------------------------------------------------------------------------------
/* «адание приращени€ времени в режиме User.
//------------------------------------------------------------------------------
	ƒва варианта:
	1. «адаЄм только timeInterval (indexMax = 0).
¬ этом случае timeInterval должно быть кратно 1 сек. “.е. есть целое число
приращений, которое даЄт суммарный интервал в 1 сек., т.е.
		timeInterval * indexMax = 1.
Ётот режим требуетс€ дл€ совместимости с уже используемым кодом.

	2. «адаЄм оба параметра. Ёто дл€ случа€, когда timeInterval не кратно 1
сек. ¬ этом случае необходимо, чтобы за некоторое число приращений получалс€
интервал, равный целому числу сек., т.е. должно выполн€тьс€ равенство:
		timeInterval * indexMax = m_time0_delta.
Ќапример: приращение времени 0,128 сек, indexMax = 125, тогда m_time0_delta = 16.
*/
bool CDataTime::SetUserParamMode(
	double timeInterval,
	long indexMax,
	double timeAverage)
{
	bool ret = timeInterval > 0.;
	if (ret)
	{
		m_bInDependentMode = false;
		m_bCodeMode = false;
		m_bZetMode = false;
		m_zdt = timeInterval;
		m_timeAverage = timeAverage;
		if (indexMax == 0)
		{
			uint32_t nanosec = m_zdt.GetNanoseconds();
			if (nanosec > 0)
			{
				div_t dvt = div(1000000000, nanosec);
				ret = dvt.rem == 0;
			}
		}
		else
		{
			CZetTimeSpan zts(m_zdt);
			zts *= indexMax;
			ret = 0 ==  m_zdt.GetNanoseconds();
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
// «адание кода времени усреднени€.
// ƒл€ каналов серии 7000 следует задавать частоту дискретизации в √ц
void CDataTime::SetAverageCodeMode(
	const long val,			// требуемый код усреднени€
	const double freqADC)	// частота дискретизации канала (актуально дл€ 7000)
{
	m_bCodeMode = true;
	m_bZetMode = false;
	m_bInDependentMode = false;
	// реализаци€ ограничений 0 <= val <= 2
	m_codeFast = TestFast(val);
	if ((freqADC == 0.) || (freqADC >= 50.))
	{// определение параметров
		switch (m_codeFast)
		{
		case 0:
			m_timeAverage = 0.1;
			m_zdt = CZetTimeSpan((int64_t)0, (uint32_t)100000000);
			break;
		case 1:
			m_timeAverage = 1.;
			m_zdt = 1;
			break;
		case 2:
			m_timeAverage = 10.;
			m_zdt = (int)m_mode10sec;
			//	break;
		}
	}
	else   // проверка дл€ 7000
	{
		switch (m_codeFast)
		{
		case 0:
			m_timeAverage = 0.1;
			m_zdt = CZetTimeSpan((int64_t)0, (uint32_t)100000000);
			if ((double)(m_zdt * freqADC) >= 1.)
				break;
		case 1:
			m_timeAverage = 1.;
			m_codeFast = 1;
			// не ошибка, могло не быть break раньше
			m_zdt = 1;
			if (freqADC >= 1.)
				break;
		case 2:
			m_timeAverage = 10.;
			m_codeFast = 2;
			// не ошибка, могло не быть break раньше
			if (m_mode10sec == m10s_every_1sec)
			{
				m_zdt = 1;
				if (freqADC < 1.)
				{
					m_mode10sec = m10s_every_10sec;
					m_zdt = 10;
				}
			}
			else
			{//m_mode10sec == m10s_every_10sec
				m_zdt = 10;
			}
			//	break;
		}
	}
}
//------------------------------------------------------------------------------
// «адание первого времени считывани€ данных.
// ¬сегда кратно целому числу секунд.
void CDataTime::SetStartTime(const double timeCur)		// текущее врем€
{// врем€ cервера может быть отрицательным
	if (m_codeFast == 0)
	{
		m_zTimeRead = timeCur;
		m_zTimeRead.RoundSecondsDown();
	}
	else if (m_codeFast == 1)
	{
		m_zTimeRead = timeCur;
		m_zTimeRead += CZetTimeSpan(0, 100000000);	// + 0.1 сек
	}
	else if (m_codeFast == 2)
	{
		double time = floor(timeCur);
		time /= 10;
		time = floor(time);
		time *= 10;
		m_zTimeRead = timeCur;
	}

// 	double time = floor(timeCur);
// 	if (m_bCodeMode)
// 	{
// 		if (m_codeFast == 2)
// 		{
// 			time /= 10;
// 			time = floor(time);
// 			time *= 10;
// 		}
// 	}
// 	m_zTimeRead = time;
}
//------------------------------------------------------------------------------
// ѕриращение времени считывани€ данных.
// ѕри не целом m_dt (например: 0,1 сек) уходим от ошибок округлени€
void CDataTime::IncrementTime()
{
	m_zTimeRead += m_zdt;
}
//------------------------------------------------------------------------------
// «адание интервала в режиме Zet
void CDataTime::SetZetMode(CZetTimeSpan & zts)
{
	m_bInDependentMode = false;
	m_bCodeMode = false;
	m_bZetMode = true;
	
	m_zdt = zts;
	m_timeAverage = zts;
}
//------------------------------------------------------------------------------
// «адание параметров в режиме, когда интервал и врем€ усреднени€ не св€заны
void CDataTime::SetInDependentMode(const CZetTimeSpan & zTimeInterval, const double timeAver)
{
	m_bInDependentMode = true;
	m_bCodeMode = false;
	m_bZetMode = false;

	m_zdt = zTimeInterval;
	m_timeAverage = timeAver;
}
//------------------------------------------------------------------------------
// «адание времиени чтени€ данных напр€мую
void CDataTime::SetTimeRead(const CZetTimeSpan & zTime)
{
	m_zTimeRead = zTime;
}
//------------------------------------------------------------------------------