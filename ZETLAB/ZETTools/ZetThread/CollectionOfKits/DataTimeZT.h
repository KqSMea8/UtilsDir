//------------------------------------------------------------------------------
//		Класс "Время данных". Определяет текущее время чтенния данных.
//		Цели, преследуемые при разработке класса:
// 1. Избежать ошибок округления.
// 2. Однотипное определение времени первого считывания.
// 3. Учет частот дискретитзации.
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <ZetTools\include\ZetTools.h>
//------------------------------------------------------------------------------
// Класс "Время данных"
class CDataTimeZT
{
private:
	CZetTimeSpan m_zts_dtBuf;	// интервал времени для расчета размера буфера чтения

	CZetTimeSpan m_zts_dtRead;	// период чтения данных
	CZetTimeSpan m_zts_dtСalc;	// период расчета промежуточных результатов
	CZetTimeSpan m_zts_dtAver;	// период усреднения промежуточных результатов

	CZetTimeSpan m_zts_read;	// время чтения данных
	CZetTimeSpan m_zts_calc;	// время расчета промежуточных результатов
	CZetTimeSpan m_zts_aver;	// время усреднения промежуточных результатов
	
protected:

public:
	CDataTimeZT();
	~CDataTimeZT();
	CDataTimeZT(const CDataTimeZT & val);
	CDataTimeZT & operator = (const CDataTimeZT & val);
	
	// Интервал времени для расчета размера буфера чтения ----------------------------
	CZetTimeSpan GetIntervalBuffer();
	void SetIntervalBuffer(const CZetTimeSpan & ztInterval);

	// Период чтения данных --------------------------------------------------------
	CZetTimeSpan & GetIntervalRead();
	void SetIntervalRead(const CZetTimeSpan & ztInterval);
	void SetIntervalRead(const double dInterval);

	// Период расчета промежуточных результатов ---------------------------------
	CZetTimeSpan & GetIntervalCalc();
	void SetIntervalCalc(const CZetTimeSpan & ztInterval, const bool bRound = true);
	void SetIntervalCalc(const double dInterval, const bool bRound = true);

	// Период усреднения промежуточных результатов ---------------------------------
	CZetTimeSpan & GetIntervalAver();
	void SetIntervalAver(const CZetTimeSpan & ztInterval);
	void SetIntervalAver(const double dInterval);
	
	// Время чтения данных ----------------------------------------------------
	CZetTimeSpan & GetTimeRead();
	void SetTimeRead(const CZetTimeSpan & zTime);

	// Время расчета промежуточных результатов --------------------------------
	CZetTimeSpan & GetTimeCalc();
	void SetTimeCalc(const CZetTimeSpan & zTime);

	// Время усреднения промежуточных результатов -----------------------------
	CZetTimeSpan & GetTimeAver();
	void SetTimeAver(const CZetTimeSpan & zTime);

	//--------------------------------------------------------------------------
	// Другие функции ----------------------------------------------------------
	//--------------------------------------------------------------------------

	// Установка первого времени чтения данных
	void SetTimeReadStart(const double timeCur);

	// Инкремент времени чтения данных
	void IncrementTime();

	// Время относительного чтения данных в сек
	double GetTime();

	// Инкремент времени расчета промежуточных результатов
	void IncrementTimeCalc();
	// Проверка нвступления времени расчета промежуточных результатов
	bool IsTimeCalc();

	// Инкремент времени усреднения промежуточных результатов
	void IncrementTimeAver();
	// Проверка нвступления усреднения промежуточных результатов
	bool IsTimeAver();

	// Сдвиг времен класса при перестартовке сервера
	// zts_dt - разница нового и старого времен старта сервера
	// (положительная величина)
	void ReStartSRV(const CZetTimeSpan & zts_dt);

	// Установка первых значений времён чтения, расчета и усреднения в
	// соответствии с правилами синхронизации работы нескольких программ.
	// Периоды чтения, расчета и усреднения должны быть заданы.
	// timeCur - текущее время сервера;
	// timeForInit - время инициализации.
	// Устанавливаются следующии значения:
	// TimeRead = RoundingByRules(timeCur - timeForInit);
	// TimeCalc = TimeRead + timeForInit;
	// TimeAver = RoundingByRules(timeCur + IntervalTimeAver)
	void SetFirstTimesByRules(const double timeCur, const double timeForInit);
};
//------------------------------------------------------------------------------