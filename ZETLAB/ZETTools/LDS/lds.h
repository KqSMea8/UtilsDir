//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <intsafe.h>
#include "mathZet.h"
#include "Metrology\SpaceProbability.h"
#include "Algorithms\SomeFeatures.h"
//------------------------------------------------------------------------------
#define SizeForTraining 300
//------------------------------------------------------------------------------
// Идентификаторы датчиков, используемых в СОУ (тип BYTE)
#define Lds_ID_Sensor_Press	1		// датчик давления (статическое Р)
#define Lds_ID_Sensor_Hydro	2		// гидрофон (динамическое Р)
//------------------------------------------------------------------------------
// Флаги типов алгоритмов СОУ - младшее слово в Lds_Type_Algr_All
#define Lds_Type_Algr_Prof		1	// анализ профиля давления
#define Lds_Type_Algr_Wave		2	// по волне давления
#define Lds_Type_Algr_Acst		4	// по акустическим сигналам

#define Lds_Num_Type_Algr		3	// кол-во типов алгоритмов
//------------------------------------------------------------------------------
// Флаги под-алгоритмов СОУ - старшее слово в Lds_Type_Algr_All
// профиль ----------------------
#define Lds_Algr_Prof_6			1	// профиль давления по 6 мин
#define Lds_Algr_Prof_12		2	// профиль давления по 12 мин
#define Lds_Algr_Prof_18		4	// профиль давления по 18 мин
#define Lds_Algr_Prof_24		8	// профиль давления по 24 мин
#define Lds_Algr_Prof_30		16	// профиль давления по 30 мин

#define Lds_Num_Type_SubAlgr_Prof	5	// кол-во под-алгоритмов Prof
// волна ------------------------
#define Lds_Algr_Wave_DWF		1	// волна по цифровой вейвлет-фильтрации

#define Lds_Num_Type_SubAlgr_Wave	1	// кол-во под-алгоритмов Wave
// акустика ---------------------
#define Lds_Algr_Acst			1	// акустика

#define Lds_Num_Type_SubAlgr_Acst	1	// кол-во под-алгоритмов Acst
//------------------------------------------------------------------------------
// Индексы алгоритмов
#define Lds_ind_Algr_Prof		0	// профиль давления
#define Lds_ind_Algr_Wave		1	// волна давления
#define Lds_ind_Algr_Acst		2	// акустика

// Индексы под-алгоритмов
// профиль --------------------------
#define Lds_ind_SubAlgr_Prof_6		0
#define Lds_ind_SubAlgr_Prof_12		1
#define Lds_ind_SubAlgr_Prof_18		2
#define Lds_ind_SubAlgr_Prof_24		3
#define Lds_ind_SubAlgr_Prof_30		4
// волна ----------------------------
#define Lds_ind_SubAlgr_Wave_DWF	0
// акустика -------------------------
#define Lds_ind_SubAlgr_Acst		0
//------------------------------------------------------------------------------
enum Lds_type_Status_Event
{// наблюдаемый статус события
	lds_tse_no,			// событие не обнаружено
	lds_tse_start,		// событие началось
	lds_tse_enable,		// событие наблюдается
	lds_tse_end,		// событие завершилось
};
//------------------------------------------------------------------------------
enum Lds_type_Stationary
{
	lds_ts_No = -1,		// стационарности нет
	lds_ts_Unknown,		// тип стационарности не определен
	lds_ts_Yes,			// стационарность есть
};
//------------------------------------------------------------------------------
enum Lds_type_Event
{// тип события ДС, определяется после анализа
	lds_te_False = -1,	// ложное событие
	lds_te_No,			// нет события
	lds_te_Press_Up,	// событие с увеличением давления
	lds_te_Press_Douw,	// событие с уменьшением давления 
	lds_te_DetectEvent,	// началось событие
	lds_te_Leak,		// утечка
	lds_te_Press_Up_SRMC,	// событие с сообщением от СДКУ
	lds_te_Press_Douw_SRMC,	// событие с сообщением от СДКУ
	lds_te_Yes,			// какое-то событие, длительностью более TimeYellow
	lds_te_Yes_SRMC,	// событие с каким-то сообщением от СДКУ
	lds_te_PumpStart_SRMC,	// начало перекачки
	lds_te_PumpStop_SRMC,	// конец перекачки
};
//------------------------------------------------------------------------------
enum Lds_type_ReStart : WORD
{// типы команды ReStart
	lds_trs_No,				// ничего не надо
	lds_trs_ZeroCounter,	// только обнуление счётчика
	lds_trs_NewThresholds,	// немедленный расчет новых порогов
	lds_trs_DeferNewThresh,	// отложенный расчет новых порогов
	lds_trs_Start_Hot,		// горячий перезапуск
	lds_trs_PumpStop,		// остановка насосов
	lds_trs_PumpStart,		// запуск насосов
};
//------------------------------------------------------------------------------
enum Lds_type_event_SRMC
{// типы сообщений о смене режима работы
	lds_SRMC_No,		// нет изменений
	lds_SRMC_Start,		// начало перекачки
	lds_SRMC_Stop,		// завершение перекачки
	lds_SRMC_Up_Productive,		// увеличение производительности перекачки
	lds_SRMC_Down_Productive,	// уменьшение производительности перекачки
};
//------------------------------------------------------------------------------
struct Lds_str_AlgrAll
{// Структура-описатель используемых алгоритмов. sizeof() = 4
	WORD	AlgrMain;	// младшее слово
	WORD	AlgrSub;	// старшее слово

	Lds_str_AlgrAll()
	{
		*this = 0;
	}
	~Lds_str_AlgrAll() {}

	Lds_str_AlgrAll(WORD algr_main, WORD algr_sub)
	{
		AlgrMain = algr_main;
		AlgrSub = algr_sub;
	}
	Lds_str_AlgrAll(const Lds_str_AlgrAll &val)
	{
		*this = val;
	}
	void operator = (const Lds_str_AlgrAll &val)
	{
		AlgrMain = val.AlgrMain;
		AlgrSub = val.AlgrSub;
	}
	void operator = (const DWORD &val)
	{
		*((DWORD*)this) = val;
	}
	operator DWORD ()
	{
		return *((DWORD*)this);
	}
};
//------------------------------------------------------------------------------
struct Lds_str_ObjectID
{//// Структура с идентификаторами. sizeof() = 4
	// Если какой-либо ID = 0, то это значит, что он не задан
	BYTE sensor;		// младший байт, ID датчика
	BYTE type;			// средний байт, ID типа датчика
	BYTE line;			// средний байт, ID линейной части нефтепровода
	BYTE pipe;			// старший байт, ID нефтепровода

	Lds_str_ObjectID()
	{
		*this = 0;
	}
	~Lds_str_ObjectID() {}

	Lds_str_ObjectID(BYTE _sensor, BYTE _type, BYTE _line, BYTE _pipe)
	{
		sensor = _sensor;
		type = _type;
		line = _line;
		pipe = _pipe;
	}
	Lds_str_ObjectID(const Lds_str_ObjectID &val)
	{
		*this = val;
	}
	operator DWORD ()
	{
		return *((DWORD*)this);
	}
	void operator = (const Lds_str_ObjectID &val)
	{
		sensor = val.sensor;
		type = val.type;
		line = val.line;
		pipe = val.pipe;
	}
	void operator = (const DWORD &val)
	{
		*((DWORD*)this) = val;
	}
};
//------------------------------------------------------------------------------
struct Lds_str_EventData
{
	float	value;		// изменение давления
	double	Time;		// время наступления события

	Lds_str_EventData()
		: value(0.f)
		, Time(0.)
	{}
	~Lds_str_EventData() {}

	Lds_str_EventData(const Lds_str_EventData &val)
	{
		*this = val;
	}
	void operator = (const Lds_str_EventData &val)
	{
		value = val.value;
		Time = val.Time;
	}
	void Clear()
	{
		value = 0.f;
		Time = 0.;
	}
};
//------------------------------------------------------------------------------
struct Lds_str_CustomData
{// Структура-описатель утечки
	long	typeValue;	// -1 если value < 0, и +1 если value > 0
	float	average;	// среднее значение
	float	valuePeak;	// пиковая величина события (dP или энергия)
	float	intensity;	// параметр события (интенсивность в м^3/час)
	UINT	counter;	// счетчик для расчета среднего
	Lds_type_Status_Event	status;		// статус утечки
	CSpaceSimple<double>	Times;		// временной интервал

	Lds_str_CustomData()
		: typeValue(-1)
		, average(0.f)
		, counter(0)
		, intensity(0.f)
		, status(lds_tse_no)
		, valuePeak(0.f)
		, Times()
	{}
	~Lds_str_CustomData() {}

	Lds_str_CustomData(const Lds_str_CustomData &val)
	{
		*this = val;
	}
	void operator = (const Lds_str_CustomData &val)
	{
		typeValue = val.typeValue;
		average = val.average;
		counter = val.counter;
		valuePeak = val.valuePeak;
		intensity = val.intensity;
		status = val.status;
		Times = val.Times;
	}
	void Add(float newVal)
	{
		if (counter == 0)
			intensity = 0.f;
		CalcAverageOnFly(newVal, ++counter, average);
		if (typeValue < 0)
		{// IsNotEqual - нельзя
			if ((valuePeak != 0.f) && (newVal != 0.f))
			{
				if ((valuePeak < 0.f) && (newVal < 0.f))
					valuePeak = min(valuePeak, newVal);
				else if ((valuePeak > 0.f) && (newVal > 0.f))
					valuePeak = max(valuePeak, newVal);
				else if ((valuePeak > 0.f) && (newVal < 0.f))
					valuePeak = newVal;
				//	else//((value < 0.f) && (newVal > 0.f))
				//		valuePeak			без изменения
			}
			else if (valuePeak == 0.f)	// newVal != 0.f
				valuePeak = newVal;
		//	else //newVal == 0.f
		//		valuePeak			без изменения
		}
		else
			valuePeak = max(valuePeak, newVal);
	}
	void Clear()
	{
		average = 0.f;
		counter = 0;
		valuePeak = 0.f;
		intensity = 0.f;
		status = lds_tse_no;
		Times.Clear();
	}
	bool TestStartWithEvent()
	{// Событие наблюдается, есть новое значение.
	//	Времена и параметр не изменяю
		bool ret(false);
		switch (status)
		{
		case lds_tse_no:
		case lds_tse_end:
			status = lds_tse_start;
			ret = true;
			break;

		case lds_tse_start:
			status = lds_tse_enable;
			break;

		//case lds_tse_enable:
		//	status = lds_tse_enable;
		//	break;
		}
		return ret;
	}
	bool TestEndWithOutEvent()
	{// Событие завершилось. Времена и параметр не изменяю
		bool ret(false);
		switch (status)
		{
		case lds_tse_no:
			break;

		case lds_tse_start:
		case lds_tse_enable:
			status = lds_tse_end;
			ret = true;
			break;

		case lds_tse_end:
			status = lds_tse_no;
		//	break;
		}
		if (ret)
			counter = 0;
		return ret;
	}
};
//------------------------------------------------------------------------------
struct Lds_str_CustomData_Hdr : public Lds_str_CustomData
{
	float	numBandWithEvent;	// кол-во полос с событием
	float	freqMin;			// мin частота у полос с событием
	float	freqMax;			// мах частота у полос с событием

	Lds_str_CustomData_Hdr() : Lds_str_CustomData()
		, numBandWithEvent(0)
		, freqMin(0.f)
		, freqMax(0.f)
	{
		typeValue = 1;
	}
	Lds_str_CustomData_Hdr(const Lds_str_CustomData_Hdr &val)
		: Lds_str_CustomData(val)
	{
		*this = val;
	}
	void operator = (const Lds_str_CustomData_Hdr &val)
	{
		*((Lds_str_CustomData*)this) = val;
		numBandWithEvent = val.numBandWithEvent;
		freqMin = val.freqMin;
		freqMax = val.freqMax;
	}
	void Clear()
	{
		Lds_str_CustomData::Clear();
		numBandWithEvent = 0;
		freqMin = 0.f;
		freqMax = 0.f;
	}
};
//------------------------------------------------------------------------------
struct Lds_str_CustomLeak
{// Структура с данными о событии по одному датчику
	// Используется для передачи между компонентами СОУ.
	DWORD	size;		// размер структуры в байтах
	DWORD	idStr;		// идентификатор структуры
	DATE	dataTime;	// время данных
	Lds_str_ObjectID	idSens;	// идентификатор датчика, определившего утечку

	Lds_str_CustomLeak()
		: idStr(0)
		, dataTime(0.)
	{
		size = sizeof(Lds_str_CustomLeak);
	}
	~Lds_str_CustomLeak() {}

	Lds_str_CustomLeak(const Lds_str_CustomLeak &val)
		: idStr(0)
	{
		size = sizeof(Lds_str_CustomLeak);
		*this = val;
	}
	void operator = (const Lds_str_CustomLeak &val)
	{
		dataTime = val.dataTime;
		idSens = val.idSens;
	}
};
//------------------------------------------------------------------------------
struct Lds_str_Leak_Prs : public Lds_str_CustomLeak
{// Структура с данными о событии по датчику давления.
	// Используется для передачи между компонентами СОУ.
	Lds_str_CustomData	data;	// описатель утечки

	Lds_str_Leak_Prs()
	{
		size = sizeof(Lds_str_Leak_Prs);
		idStr = 0x10010;
	}
	~Lds_str_Leak_Prs() {}

	Lds_str_Leak_Prs(const Lds_str_Leak_Prs &val)
	{
		size = sizeof(Lds_str_Leak_Prs);
		idStr = 0x10010;
		*this = val;
	}
	void operator = (const Lds_str_Leak_Prs &val)
	{
		*((Lds_str_CustomLeak*)this) = val;
		data = val.data;
	}
	void Clear()
	{
		data.Clear();
	}
};
//------------------------------------------------------------------------------
struct Lds_str_Leak_Hdr : public Lds_str_CustomLeak
{// Структура с данными о событии по гидрофону.
	// Используется для передачи между компонентами СОУ.
	Lds_str_CustomData_Hdr	data;	// описатель утечки

	Lds_str_Leak_Hdr()
	{
		size = sizeof(Lds_str_Leak_Hdr);
		idStr = 0x10011;
	}
	~Lds_str_Leak_Hdr() {}

	Lds_str_Leak_Hdr(const Lds_str_Leak_Hdr &val)
	{
		size = sizeof(Lds_str_Leak_Hdr);
		idStr = 0x10011;
		*this = val;
	}
	void operator = (const Lds_str_Leak_Hdr &val)
	{
		*((Lds_str_CustomLeak*)this) = val;
		data = val.data;
	}
	void Clear()
	{
		data.Clear();
	}
};
//------------------------------------------------------------------------------
struct Lds_str_Fault
{// Структура, сообщающая о неисправности. sizeof() = 12
	// Используется для передачи между компонентами СОУ.
	DWORD	size;		// размер структуры в байтах
	DWORD	idStr;		// идентификатор структуры
	DATE	dataTime;	// время данных
	Lds_str_ObjectID	idSens;	// идентификатор датчика
	Type_Fault			type;	// тип неисправности

	Lds_str_Fault()
		: idStr(0x10012)
		, dataTime(0.)
	{
		size = sizeof(Lds_str_Fault);
		type = fault_Ok;
	//	idSens = 0;			см. конструктор Lds_ObjectID
	}
	~Lds_str_Fault() {}

	Lds_str_Fault(const Lds_str_Fault &val)
		: idStr(0x10012)
	{
		size = sizeof(Lds_str_Fault);
		*this = val;
	}
	void operator = (const Lds_str_Fault &val)
	{
		dataTime = val.dataTime;
		idSens = val.idSens;
		type = val.type;
	}
};
//------------------------------------------------------------------------------
struct Lds_str_Stationary
{// Структура-описатель стационарности.
	// Используется для передачи между компонентами СОУ.
	DWORD	size;		// размер структуры в байтах
	DWORD	idStr;		// идентификатор структуры
	DATE	dataTime;	// время данных
	Lds_str_ObjectID	idSens;	// идентификатор датчика
	Lds_type_Stationary	type;	// тип стационарности

	Lds_str_Stationary()
		: idStr(0x10013)
		, dataTime(0.)
	{
		size = sizeof(Lds_str_Stationary);
	//	idSens = 0;			см. конструктор Lds_ObjectID
		type = lds_ts_Unknown;
	}
	~Lds_str_Stationary() {}

	Lds_str_Stationary(const Lds_str_Stationary &val)
		: idStr(0x10013)
	{
		size = sizeof(Lds_str_Stationary);
		*this = val;
	}
	void operator = (const Lds_str_Stationary &val)
	{
		dataTime = val.dataTime;
		idSens = val.idSens;
		type = val.type;
	}
};
//------------------------------------------------------------------------------
struct Lds_str_BadParam
{// Структура c текущим значением давления для передачи м/д компонентами
	// Используется для передачи между компонентами СОУ.
	DWORD	size;		// размер структуры в байтах
	DWORD	idStr;		// идентификатор структуры
	DATE	dataTime;	// время данных
	Lds_str_ObjectID	idSens;	// идентификатор датчика
	DWORD				value;	// само значение - 0 или 1

	Lds_str_BadParam()
		: idStr(0x10014)
		, dataTime(0.)
	{
		size = sizeof(Lds_str_BadParam);
	//	idSens = 0;			см. конструктор Lds_ObjectID
		value = DWORD_MAX;		// в начале плохие параметры
	}
	~Lds_str_BadParam() {}

	Lds_str_BadParam(const Lds_str_BadParam &val)
		: idStr(0x10014)
	{
		size = sizeof(Lds_str_BadParam);
		*this = val;
	}
	void operator = (const Lds_str_BadParam &val)
	{
		dataTime = val.dataTime;
		idSens = val.idSens;
		value = val.value;
	}
};
//------------------------------------------------------------------------------
struct Lds_str_ReStart
{
	Lds_type_ReStart	command;	// команда - младшее слово в 32 битах
	BYTE				type;		// тип датчика
	BYTE				id8;		// его идентификатор
	
	Lds_str_ReStart()
	{
		*this = 0;
	}
	~Lds_str_ReStart() {}
	Lds_str_ReStart(const Lds_str_ReStart &val)
	{
		*this = val;
	}
	Lds_str_ReStart(const long val)
	{
		*this = val;
	}
	void operator = (const Lds_str_ReStart &val)
	{
		*this = val;
	}
	operator long() { return *((long*)this); }
	void operator = (const long a)  { *((long*)this) = a; };
};
//------------------------------------------------------------------------------
