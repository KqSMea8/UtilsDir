//------------------------------------------------------------------------------
//		 ласс "¬рем€ данных". ќпредел€ет текущее врем€ чтенни€ данных.
// ѕредназначен дл€ программ, в которых используетс€ усреднение 0,1 сек.
//		÷ели, преследуемые при разработке класса:
// 1. »збежать ошибок округлени€ при прибавлении 0,1 сек.
// 2. ќднотипное определение времени первого считывани€.
// 3. ”чет частот дискретитзации устройств серии 7000.
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <math.h>
#include <mathZet.h>
#include <ZetTools\include\ZetTools.h>
//------------------------------------------------------------------------------
enum Mode10sec
{// тип выдачи данных при осреднении 10 сек
	m10s_every_1sec = 1,	// данные выдаютс€ каждую секунду
	m10s_every_10sec = 10	// данные выдаютс€ каждые 10 секутд
};
//------------------------------------------------------------------------------
//  ласс "¬рем€ данных"
class CDataTime
{
private:
	// режим работы: true - работа с кодами, false - с временами
	bool m_bCodeMode;
	// ѕри работе с кодами определ€ет способ расчЄта кол-ва считываемых отсчетов:
	// true - по m_timeAverage, false - по m_zdt
	bool m_bCalcNumberMode;
	// –ежим работы, когда интервал = врем€ усреднени€ и задаюс€ как CZetTimeSpan
	bool m_bZetMode;
	// –ежим работы, когда величины m_zdt и m_timeAverage не св€заны друг с другом,
	// режим используетс€ при перекрытии интервалов чтени€
	bool m_bInDependentMode;

	long reserved3;

	long m_codeFast;		// код усреднени€ по времени
	Mode10sec m_mode10sec;	// тип выдачи данных при осреднении 10 сек

	double m_timeAverage;	// врем€ усреднени€
	CZetTimeSpan m_zdt;		// приращение времени при инкременте, сек
	CZetTimeSpan m_zTimeRead;// врем€ чтени€ данных

protected:

public:
	CDataTime();
	~CDataTime() {}
	CDataTime(const CDataTime & val);
	CDataTime & operator = (const CDataTime & val);

	// функции свойств --------------------------------------------------------
	//  од усреднени€
	long GetAverageCode() const { return m_codeFast; }
	void SetAverageCodeMode(const long code, const double freqADC = 0.);

	// Cпособ расчЄта кол-ва считываемых отстетов:
	// true - по m_timeAverage, false - по m_zdt
	bool GetCalcNumberMode();
	void SetCalcNumberMode(bool bCalcByAver);

	// “ип усредени€ при 10 сек (скольз€щее средрее - да/нет)
	Mode10sec GetMode10sec() const { return m_mode10sec; }
	void SetMode10sec(const Mode10sec val) { m_mode10sec = val; }

	// интервал чтени€
	double GetTimeInterval() { return m_zdt; }
	double GetTimeAverage() { return m_timeAverage; }
	
	// функции класса ---------------------------------------------------------
	// „тение моды работы класса
	bool IsCodeMode() const { return m_bCodeMode; }
	// «адание параметров в user моде
	bool SetUserParamMode(double timeInterval, long indexMax = 0, double timeAverage = 0.);
	// ѕроверка кода усреднени€
	long TestFast(const long val);
	// ѕолучение текущего времени чтени€ данных
	double GetTime() { return m_zTimeRead; }
	CZetTimeSpan & GetzTime() { return m_zTimeRead; }
	CZetTimeSpan & GetzTimeInterval() { return m_zdt; }

	// ”становка первого времени чтени€ данных
	void SetStartTime(const double timeCur);
	// »нкремент времени чтени€ данных
	void IncrementTime();
	// «адание интервала в режиме Zet (интервал = врем€ усреднени€)
	void SetZetMode(CZetTimeSpan & zts);
	// «адание параметров в режиме, когда интервал и врем€ усреднени€ не св€заны
	void SetInDependentMode(const CZetTimeSpan & zTimeInterval, const double timeAver);
	// «адание времиени чтени€ данных напр€мую
	void SetTimeRead(const CZetTimeSpan & zTime);
};
//------------------------------------------------------------------------------