//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
#ifdef ZETTOOLS_EXPORTS
#define ZETTOOLS_API __declspec(dllexport)
#else
#define ZETTOOLS_API __declspec(dllimport)
#endif
//-----------------------------------------------------------------------------
#include <wtypes.h>			// определение DATE
#include <TimeZones.h>		// определение TimeZones
#include <ZetErrorCodes.h>	// коды ощибок
#include <zet_struct1.h>	// ZetHeader & ZetFooter
#include <atltime.h>		// определение CTime
#include <string>			// std::wstring
//-----------------------------------------------------------------------------
#define NUMBER_MONTHS_IN_YEAR      12	// Количество месяцев в году

#define NUMBER_FIRST_MONTH_IN_YEAR 1	// Номер первого месяца в году
#define NUMBER_FIRST_DAY_IN_MONTH 1 // Номер первого числа в месяце
#define NUMBER_FIRST_HOUR_IN_DAY  0	// Номер первого часа в дне
#define NUMBER_FIRST_MIN_IN_HOUR  0 // Номер первой минуты в часе
#define NUMBER_FIRST_SEC_IN_MIN   0 // Номер первой секунды в минуте

#define NUMBER_SEC_IN_MIN   60

#define NUMBER_SEC_IN_HOUR	3600 // = 60 * 60
#define NUMBER_MIN_IN_HOUR  60

#define NUMBER_SEC_IN_DAY 86400	// = 60 * 60 * 24
#define NUMBER_MIN_IN_DAY 1440	// = 60 * 24
#define NUMBER_HOURS_IN_DAY 24	


#define NUMBER_SEC_IN_3000 32503680000	// кол-во секунд в максимальном значении

#define ZetTimeMin CZetTime(0Ui64, 0U)			// 01.01.1970 00:00:00
#define ZetTimeMax CZetTime(NUMBER_SEC_IN_3000, 0U)	// 01.01.3000 00:00:00
//-----------------------------------------------------------------------------
// Получение текущего времени UTC.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - время получено.
_Check_return_
ZETTOOLS_API long ztGetCurrentTimeUTC(_Inout_ DATE * pTimeUTC);
//-----------------------------------------------------------------------------
// Получение текущего локального времени.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - время получено.
_Check_return_
ZETTOOLS_API long ztGetCurrentTimeLocal(_Inout_ DATE * pTimeLocal);
//-----------------------------------------------------------------------------
// Преобразование локального времени во время UTC.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - время преобразовано.
_Check_return_
ZETTOOLS_API long ztTimeLocalToTimeUTC(_In_ const DATE timeLocal, _Inout_ DATE * pTimeUTC);
//-----------------------------------------------------------------------------
// Преобразование времени UTC в локальное время.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - время преобразовано.
_Check_return_
ZETTOOLS_API long ztTimeUTCtoTimeLocal(_In_ const DATE timeUTC, _Inout_ DATE * pTimeLocal);
//-----------------------------------------------------------------------------
// Преобразование зонального времени во время UTC.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - время преобразовано.
_Check_return_
ZETTOOLS_API long ztTimeZoneToTimeUTC(_In_ const TimeZones zone, _In_ const DATE timeZone, _Inout_ DATE * pTimeUTC);
//-----------------------------------------------------------------------------
// Преобразование времени UTC в зональное время.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - время преобразовано.
_Check_return_
ZETTOOLS_API long ztTimeUTCtoTimeZone(_In_ const TimeZones zone, _In_ const DATE timeUTC, _Inout_ DATE * pTimeZone);
//-----------------------------------------------------------------------------
// Добавление секунд ко времени.
// Добавлять можно как отрицательное, так и дробное значения.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - выполнено.
_Check_return_
ZETTOOLS_API long ztAddSecondToDATE(_In_ const double second, _Inout_ DATE * pTime);
//-----------------------------------------------------------------------------
// Запись текущего времени UTC в заголовок структуры.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - время записано.
_Check_return_
ZETTOOLS_API long ztSetStructureCurrentTimeUTC(_Inout_ ZetHeader * pStructure);
//-----------------------------------------------------------------------------
// Запись времени UTC в заголовок структуры.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - время записано.
_Check_return_
ZETTOOLS_API long ztSetStructureTimeUTC(_Inout_ ZetHeader * pStructure, _In_ const DATE timeUTC);
//-----------------------------------------------------------------------------
// Преобразование локального времени во время UTC и запись в заголовок структуры.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - время записано.
_Check_return_
ZETTOOLS_API long ztSetStructureTimeLocal(_Inout_ ZetHeader * pStructure, _In_ const DATE timeLocal);
//-----------------------------------------------------------------------------
// Получение из заголовока структуры времени UTC.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - время получено.
_Check_return_
ZETTOOLS_API long ztGetStructureTimeUTC(_In_ const ZetHeader * pStructure, _Inout_ DATE * pTimeUTC);
//-----------------------------------------------------------------------------
// Получение из заголовока структуры времени UTC и преобразование его в локальное время.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - время получено и преобразовано.
_Check_return_
ZETTOOLS_API long ztGetStructureTimeLocal(_In_ const ZetHeader * pStructure, _Inout_ DATE * pTimeLocal);
//-----------------------------------------------------------------------------
// Обнуление всей структуры. В заголовке задаются поля с размером и типом структуры.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - выполнено.
_Check_return_
ZETTOOLS_API long ztOpenStructure(_Inout_ ZetHeader * pStructure,
	_In_ const UINT structureType, _In_ const UINT structureSize);
//-----------------------------------------------------------------------------
// Проверка полей структуры, добавление контрольной суммы.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - выполнено.
_Check_return_
ZETTOOLS_API long ztCloseStructure(_Inout_ ZetHeader * pStructure);
//--------------------------------------------------------------------------
// Проверка структуры (полей и контрольной суммы).
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - проверка успешна.
_Check_return_
ZETTOOLS_API long ztIsStructureValid(_Inout_ const ZetHeader * pStructure);
//-----------------------------------------------------------------------------
// Копирование одной структуры в другую.
// Исходная структура не обязана быть валидной.
// Типы и размеры структур должны совпадать
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - копирование выполнено.
_Check_return_
ZETTOOLS_API long ztCopyStructure(_Inout_ ZetHeader * pDestStruct, _In_ const ZetHeader * pSrcStruct);
//-----------------------------------------------------------------------------
// Копирование времени из заголовка одной структуры в заголовок другой.
// Структуры не обязаны быть валидными и быть одного типа.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - копирование выполнено.
_Check_return_
ZETTOOLS_API long ztCopyStructureTime(_Inout_ ZetHeader * pDestStruct, _In_ const ZetHeader * pSrcStruct);
//-----------------------------------------------------------------------------
// Задание идентификатора и номера программы в ZetHeader.
// programID  - идентификатор программы (не 0).
// programNum - номер запущенной копии программы (счёт с 1).
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - выполнено.
ZETTOOLS_API long ztSetProgramValuesInHeader(_Inout_ ZetHeader * pStruct,
	_In_ const uint64_t programID, _In_ const uint16_t programNum,
	_In_ const bool bNeedClose = true);
//-----------------------------------------------------------------------------
// Задание идентификатора и номера модуля программы в ZetHeader.
// moduleID   - идентификатор модуля в программе (не 0).
// moduleNum  - номер модуля в программе (счёт с 1).
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - выполнено.
ZETTOOLS_API long ztSetModuleValuesInHeader(_Inout_ ZetHeader * pStruct,
	_In_ const uint32_t moduleID, _In_ const uint16_t moduleNum,
	_In_ const bool bNeedClose = true);
//-----------------------------------------------------------------------------
// Запись в ZetWindowsLog сообщения об ошибке (char-текст)
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - выполнено.
ZETTOOLS_API long ztMessageErrorA(_In_ LPCSTR lpText, _In_ const long code = 0);

// Запись в ZetWindowsLog сообщения об ошибке (wchar-текст)
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - выполнено.
ZETTOOLS_API long ztMessageErrorW(_In_ LPCWSTR lpwText, _In_ const long code = 0);

#ifdef UNICODE
#define ztMessageError  ztMessageErrorW
#else
#define ztMessageError  ztMessageErrorA
#endif
//-----------------------------------------------------------------------------
// Запись в ZetWindowsLog сообщения о предупреждении (char-текст)
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - выполнено.
ZETTOOLS_API long ztMessageWarningA(_In_ LPCSTR lpText, _In_ const long code = 0);

// Запись в ZetWindowsLog сообщения о предупреждении (цchar-текст)
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - выполнено.
ZETTOOLS_API long ztMessageWarningW(_In_ LPCWSTR lpwText, _In_ const long code = 0);

#ifdef UNICODE
#define ztMessageWarning  ztMessageWarningW
#else
#define ztMessageWarning  ztMessageWarningA
#endif
//-----------------------------------------------------------------------------
// Запись в ZetWindowsLog информационного сообщения (char-текст)
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - выполнено.
ZETTOOLS_API long ztMessageInformationA(_In_ LPCSTR lpText);

// Запись в ZetWindowsLog информационного сообщения (wchar-текст)
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - выполнено.
ZETTOOLS_API long ztMessageInformationW(_In_ LPCWSTR lpwText);

#ifdef UNICODE
#define ztMessageInformation  ztMessageInformationW
#else
#define ztMessageInformation  ztMessageInformationA
#endif
//-----------------------------------------------------------------------------
// Копирование char-текста из lpwSrcBuf в lpwDestBuf.
// sizeDestBuf - размер буфера назначения в символах.
// Если в буфере мало места, то возвращается код ошибки Err_String_Less (-15)
// и копируется столько символов, сколько помещается.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - выполнено.
ZETTOOLS_API long ztStringCopyA(_Inout_ LPSTR lpDestBuf, _In_ const UINT sizeDestBuf, _In_ LPCSTR lpSrcBuf);

// Копирование wchar-текста из lpwSrcBuf в lpwDestBuf.
// sizeDestBuf - размер буфера назначения в символах.
// Если в буфере мало места, то возвращается код ошибки Err_String_Less (-15)
// и копируется столько символов, сколько помещается.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - выполнено.
ZETTOOLS_API long ztStringCopyW(_Inout_ LPWSTR lpwDestBuf, _In_ const UINT sizeDestBuf, _In_ LPCWSTR lpwSrcBuf);

#ifdef UNICODE
#define ztStringCopy ztStringCopyW
#else
#define ztStringCopy  ztStringCopyA
#endif
//-----------------------------------------------------------------------------
// Добавление char-текста из lpwSrcBuf в lpwDestBuf.
// sizeDestBuf - размер буфера назначения в символах.
// Если в буфере мало места, то возвращается код ошибки Err_String_Less (-15)
// и копируется столько символов, сколько помещается.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - выполнено.
ZETTOOLS_API long ztStringAppendA(_Inout_ LPSTR lpDestBuf, _In_ const UINT sizeDestBuf, _In_ LPCSTR lpSrcBuf);

// Добавление wchar-текста из lpwSrcBuf в lpwDestBuf.
// sizeDestBuf - размер буфера назначения в символах.
// Если в буфере мало места, то возвращается код ошибки Err_String_Less (-15)
// и копируется столько символов, сколько помещается.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - выполнено.
ZETTOOLS_API long ztStringAppendW(_Inout_ LPWSTR lpwDestBuf, _In_ const UINT sizeDestBuf, _In_ LPCWSTR lpwSrcBuf);

#ifdef UNICODE
#define ztStringAppend ztStringAppendW
#else
#define ztStringAppend  ztStringAppendA
#endif
//-----------------------------------------------------------------------------
// Добавление char-символа в lpwDestBuf.
// sizeDestBuf - размер буфера назначения в символах.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - выполнено.
ZETTOOLS_API long ztStringAppendSymbolA(_Inout_ LPSTR lpDestBuf, _In_ const UINT sizeDestBuf, _In_ const char symbol);

// Добавление wchar-символа в lpwDestBuf.
// sizeDestBuf - размер буфера назначения в символах.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - выполнено.
ZETTOOLS_API long ztStringAppendSymbolW(_Inout_ LPWSTR lpwDestBuf, _In_ const UINT sizeDestBuf, _In_ const wchar_t symbol);

#ifdef UNICODE
#define ztStringAppendSymbol ztStringAppendSymbolW
#else
#define ztStringAppendSymbol  ztStringAppendSymbolA
#endif
//-----------------------------------------------------------------------------
// Инициализация создания дамп-файла при необработанном исключении
ZETTOOLS_API void ztInitCreateDumpWhenException();
//-----------------------------------------------------------------------------
//------- Классы времени ------------------------------------------------------
//-----------------------------------------------------------------------------
/*		Класс CZetTimeSpan предназначен для значений относительного времени. 
(целое без знака). Поле ticks для выравнивания.
	Время типа CZetTimeSpan можно умножать/делить на число типа double.

	Арифметика следующая (инкапсулировано в коде класса):
	double sec = seconds + nanoseconds / 1000000000
	(0, 0) == (-1, 1000000000);
	(3, 400) - (1, 200) = (2, 200)
	(1, 200) - (3, 400) = (0, 1000000200) - (3, 400) = (-3, 999999800)
	CZetTimeSpan x = -(0, 100) = (0, 0) - (0, 100) =
		(-1, 1000000000) - (0, 100) = (-1, 999999900)
	Изменять порядок полей в секции private категорически запрещено!!!
*/
//-----------------------------------------------------------------------------
class ZETTOOLS_API CZetTimeSpan
{
private:
	int64_t seconds;
	uint32_t nanoseconds;
	int32_t ticks;

protected:

public:
	CZetTimeSpan(const int64_t sec = 0, const uint32_t nanosec = 0);
	CZetTimeSpan(const CZetTimeSpan & dtime);
	CZetTimeSpan(const double & dtime);
	CZetTimeSpan(const float dtime);
	CZetTimeSpan(const uint32_t dtime);
	CZetTimeSpan(const DWORD dtime);
	CZetTimeSpan(const int32_t dtime);
	~CZetTimeSpan() {}
	
	CZetTimeSpan & operator = (const CZetTimeSpan & dtime);
	CZetTimeSpan & operator = (const double & dtime);
	CZetTimeSpan & operator = (const float dtime);
	CZetTimeSpan & operator = (const uint32_t dtime);
	CZetTimeSpan & operator = (const DWORD dtime);
	CZetTimeSpan & operator = (const int32_t dtime);

	CZetTimeSpan operator + (const CZetTimeSpan & dtime);
	CZetTimeSpan operator - (const CZetTimeSpan & dtime);
	CZetTimeSpan & operator += (const CZetTimeSpan & dtime);
	CZetTimeSpan & operator -= (const CZetTimeSpan & dtime);

	//*************************************************************************
	template<class T>
	CZetTimeSpan operator * (T & factor);
	template<class T>
	CZetTimeSpan operator * (const T & factor);
	//*************************************************************************
	template<class T>
	CZetTimeSpan & operator *= (T & factor);
	template<class T>
	CZetTimeSpan & operator *= (const T & factor);
	//*************************************************************************
	template<class T>
	CZetTimeSpan operator / (T & factor);
	template<class T>
	CZetTimeSpan operator / (const T & factor);
	//*************************************************************************
	template<class T>
	CZetTimeSpan & operator /= (T & factor);
	template<class T>
	CZetTimeSpan & operator /= (const T & factor);
	//*************************************************************************

	operator double();

	bool operator == (const CZetTimeSpan & dtime);
	bool operator != (const CZetTimeSpan & dtime);
	bool operator <  (const CZetTimeSpan & dtime);
	bool operator >  (const CZetTimeSpan & dtime);
	bool operator <= (const CZetTimeSpan & dtime);
	bool operator >= (const CZetTimeSpan & dtime);

	// Возвращает значение приватной переменной seconds
	int64_t GetSeconds() { return seconds; }
	int64_t GetSeconds() const { return seconds; }
	// Возвращает значение приватной переменной nanoseconds
	uint32_t GetNanoseconds() { return nanoseconds; }
	uint32_t GetNanoseconds() const { return nanoseconds; }

	// Расчет длительности времени в годах (один год = 365,25 суток)
	double GetYears();

	// Расчет длительности времени в месяцах (один месяц = 30 суток)
	double GetMonths();

	// Расчет длительности времени в днях
	double GetDays();

	// Расчет длительности времени в часах
	double GetHours();

	// Расчет длительности времени в минутах 
	double GetMinutes();

	// Округление до целых секунд
	void RoundSeconds();

	// Округление до целых секунд вверх
	void RoundSecondsUp();

	// Округление до целых секунд вниз
	void RoundSecondsDown();

	// Задание кол-ств секунд и наносекунд
	void SetValues(int64_t sec = 0i64, uint32_t nanosec = 0U);
};
//-----------------------------------------------------------------------------
template<class T>
CZetTimeSpan CZetTimeSpan::operator * (const T & factor)
{
	return *this * const_cast<T&>(factor);
}
//-----------------------------------------------------------------------------
template<class T>
CZetTimeSpan & CZetTimeSpan::operator *= (const T & factor)
{
	*this = *this * const_cast<T&>(factor);
	return *this;
}
//-----------------------------------------------------------------------------
template<class T>
CZetTimeSpan CZetTimeSpan::operator / (const T & factor)
{
	return *this / const_cast<T&>(factor);
}
//-----------------------------------------------------------------------------
template<class T>
CZetTimeSpan & CZetTimeSpan::operator /= (const T & factor)
{
	*this = *this / const_cast<T&>(factor);
	return *this;
}
//-----------------------------------------------------------------------------
template<class T>
CZetTimeSpan CZetTimeSpan::operator * (T & factor)
{
	CZetTimeSpan ret(*this);
	ret *= (double)factor;
	return ret;
}
//-----------------------------------------------------------------------------
template<class T>
CZetTimeSpan & CZetTimeSpan::operator *= (T & factor)
{
	double time = *this;
	time *= (double)factor;
	*this = time;
	return *this;
}
//-----------------------------------------------------------------------------
template<class T>
CZetTimeSpan CZetTimeSpan::operator / (T & factor)
{
	CZetTimeSpan ret(*this);
	ret /= (double)factor;
	return ret;
}
//-----------------------------------------------------------------------------
template<class T>
CZetTimeSpan & CZetTimeSpan::operator /= (T & factor)
{
	double time = *this;
	time /= (double)factor;
	*this = time;
	return *this;
}
//-----------------------------------------------------------------------------
/*		Класс CZetTime предназначен для значений абсолютного времени.
		Класс всегда хранит время в формате UTC !!!
Время хранится в двух полях: seconds (целое без знака) - кол-во секунд,
прошедших с 01.01.1970, и nanoseconds (целое без знака).
Поле ticks для выравнивания.

	Складывать экземпляры этого класса НЕЛЬЗЯ!!!
	Разница экземпляров этого класса - это экземпляр класса CZetTimeSpan!!!
	Экземпляр этого класса можно складывать/вычитать с экземплярами CZetTimeSpan.

	Вся необходимая арифметика инкапсулировано в коде класса.
Изменять порядок полей в секции private категорически запрещено!!!
*/
//-----------------------------------------------------------------------------
class ZETTOOLS_API CZetTime
{
private:
	uint64_t seconds;
	uint32_t nanoseconds;
	int32_t ticks;

	long GetTimeBiasInSeconds();

protected:

public:
	CZetTime(const uint64_t sec = 0, const uint32_t nanosec = 0);
	CZetTime(const CZetTime & time);
	CZetTime(const FILETIME & time);
	CZetTime(const SYSTEMTIME & time);
	CZetTime(const DATE & time);
	CZetTime(const CTime & time);
	CZetTime(const ZetHeader & zh);
	CZetTime(const time_t & time);

	~CZetTime() {}
		
	CZetTime & operator = (const CZetTime & time);
	CZetTime & operator = (const FILETIME & time);
	CZetTime & operator = (const SYSTEMTIME & time);
	CZetTime & operator = (const DATE & time);
	CZetTime & operator = (const CTime & time);
	CZetTime & operator = (const ZetHeader & zh);
	CZetTime & operator = (const time_t & time);

	operator FILETIME();
	operator SYSTEMTIME();
	operator DATE();
	operator CTime();
	operator time_t();

	CZetTime operator + (const CZetTimeSpan & dtime);
	CZetTime operator - (const CZetTimeSpan & dtime);
	CZetTime & operator += (const CZetTimeSpan & dtime);
	CZetTime & operator -= (const CZetTimeSpan & dtime);

	bool operator == (const CZetTime & time);
	bool operator != (const CZetTime & time);
	bool operator <  (const CZetTime & time);
	bool operator >  (const CZetTime & time);
	bool operator <= (const CZetTime & time);
	bool operator >= (const CZetTime & time);

	// Возвращает значение приватной переменной seconds
	uint64_t GetSeconds() const { return seconds; }
	uint64_t GetSeconds() { return seconds; }

	// Возвращает значение приватной переменной nanoseconds
	uint32_t GetNanoseconds() { return nanoseconds; }
	uint32_t GetNanoseconds() const { return nanoseconds; }

	// Получение года в значении времени.
	// Если bTimeLocal = true, то год локального значения времени,
	// иначе - год значения времени UTC.
	// Возвращаемое значение:
	// 0    - ZetTimeMin <= time <= ZetTimeMax
	// -308 - значение меньше 01.01.1970 или больше 01.01.3000, устанавливается year = 0
	long GetYear(uint16_t & year, const bool bTimeLocal = true);

	// Получение месяца в значении времени.
	// Если bTimeLocal = true, то месяц локального значения времени,
	// иначе - месяц значения времени UTC.
	// Возвращаемое значение:
	// 0    - ZetTimeMin <= time <= ZetTimeMax
	// -308 - значение меньше 01.01.1970 или больше 01.01.3000, устанавливается month = 0
	long GetMonth(uint16_t & month, const bool bTimeLocal = true);

	// Получение день в значении времени.
	// Если bTimeLocal = true, то день локального значения времени,
	// иначе - день значения времени UTC.
	// Возвращаемое значение:
	// 0    - ZetTimeMin <= time <= ZetTimeMax
	// -308 - значение меньше 01.01.1970 или больше 01.01.3000, устанавливается day = 0
	long GetDay(uint16_t & day, const bool bTimeLocal = true);

	// Получение часа в значении времени.
	// Если bTimeLocal = true, то час локального значения времени,
	// иначе - час значения времени UTC.
	// Возвращаемое значение:
	// 0    - ZetTimeMin <= time <= ZetTimeMax
	// -308 - значение меньше 01.01.1970 или больше 01.01.3000, устанавливается hour = 0
	long GetHour(uint16_t & hour, const bool bTimeLocal = true);
	
	// Получение минуты в значении времени.
	// Если bTimeLocal = true, то минута локального значения времени,
	// иначе - минута значения времени UTC.
	// Возвращаемое значение:
	// 0    - ZetTimeMin <= time <= ZetTimeMax
	// -308 - значение меньше 01.01.1970 или больше 01.01.3000, устанавливается minute = 0
	long GetMinute(uint16_t & minute, const bool bTimeLocal = true);
	
	// Получение секунд в значении времени (с долями секунды).
	// Если bTimeLocal = true, то секунды локального значения времени,
	// иначе - секунды значения времени UTC.
	// Возвращаемое значение:
	// 0    - ZetTimeMin <= time <= ZetTimeMax
	// -308 - значение меньше 01.01.1970 или больше 01.01.3000, устанавливается second = 0
	long GetSecond(double & second);

	// Счет дней недели с 0 (воскресенье), затем 1 - (понедельник) и т.д.
	// Возвращаемое значение:
	// 0    - ZetTimeMin <= time <= ZetTimeMax
	// -308 - значение меньше 01.01.1970 или больше 01.01.3000, устанавливается dayOfWeek = 0
	long GetDayOfWeek(uint16_t & dayOfWeek, const bool bTimeLocal = true);
	
	// Проверка значения времени
	// Возвращаемое значение:
	// 0    - ZetTimeMin <= time <= ZetTimeMax
	// -308 - значение меньше 01.01.1970 или больше 01.01.3000
	long TestTimeValue();

	// Задание времени как текущее время UTC
	// Возвращаемое значение: всегда 0.
	long SetCurrentTime();

	// Задание времени как текущее локальное время
	// Возвращаемое значение: всегда 0.
	long SetLocalTime();

	// Копирование времени в ZetHeader (выполняется не зависимо от величины возвращаемого значения).
	// Возвращаемое значение:
	// 0    - ZetTimeMin <= time <= ZetTimeMax
	// -308 - значение меньше 01.01.1970 или больше 01.01.3000
	long CopyTimeToZetHeader(ZetHeader & zh);

	// Округление времени до целых секунд (в сллтветствии с существующими правилами)
	void RoundSeconds();

	// Округление времени до целых секунд вверх
	void RoundSecondsUp();

	// Округление времени до целых секунд вниз
	void RoundSecondsDown();

	// Задание кол-ств секунд и наносекунд
	void SetValues(uint64_t sec = 0Ui64, uint32_t nanosec = 0U);

	// Преобразование времени в строку с заданной точностью и по заданному формату.
	// Пример формата: L"dd.MM.yyyy HH:mm:ss"
	// precision - точность представления долей секунды, пишется после формата вывода секунд
	// Если формат не задан, то он берётся из локали (вначале дата, затем время).
	// bTimeLocal = true - с переводом из UTC в локальное время.
	// bTimeLocal = false - без перевода из UTC в локальное время.
	// Возвращаемое значение:
	// отрицательное значение - код ошибки, возвращается пустая строка;
	// 0 - успешное выполнение.
	long ConvertToString(_In_ LPWSTR lpwsBuffer, _In_ const DWORD sizeBuffer,
						 _In_opt_ const double precision = 1.,
						 _In_opt_ LPCWSTR lpcwsFormat = nullptr,
						 _In_opt_ const bool bTimeLocal	= true);

	// Преобразование времени в строку с заданной точностью и по заданному формату
	// Форматы: https://msdn.microsoft.com/query/dev12.query?appId=Dev12IDEF1&l=EN-US&k=k(wchar%2Fwcsftime);k(wcsftime);k(DevLang-C%2B%2B);k(TargetOS-Windows)&rd=true
	// Пример формата: L"%d.%m.%Y %H:%M:%S"
	// bTimeLocal = true - с переводом из UTC в локальное время.
	// bTimeLocal = false - без перевода из UTC в локальное время.
	// Возвращаемое значение:
	// отрицательное значение - код ошибки, возвращается пустая строка;
	// 0 - успешное выполнение.
	long ConvertToStringWithFormat(_In_ LPWSTR lpwsBuffer, _In_ const DWORD sizeBuffer,
								   _In_opt_  const double precision = 1., 
								   _In_opt_ LPCWSTR lpcwsFormat = L"%d.%m.%Y %H:%M:%S",
								   _In_opt_ const bool bTimeLocal = true);

	// Преобразование времени  из строки по заданному формату.
	// Пример формата: L"dd.MM.yyyy HH:mm:ss"
	// Если формат не задан, то он берётся из локали (вначале дата, затем время).
	// bTimeLocal = true - в тексте локальное время.
	// bTimeLocal = false - в тексте время UTC.
	// Возвращаемое значение:
	// отрицательное значение - код ошибки;
	// -308 - значение меньше 01.01.1970 или больше 01.01.3000
	// 0 - успешное выполнение.
	long ConvertFromString(LPCWSTR lpwsBuffer, 
		LPCWSTR lpcwsFormat = nullptr, const bool bTimeLocal = true);
};
//-----------------------------------------------------------------------------
inline CZetTimeSpan operator - (const CZetTime & t1, const CZetTime & t2)
{
	int64_t sec = *(int64_t*)(&t1) - *(int64_t*)(&t2);

	uint32_t nanosec(0);
	uint32_t n1 = *(uint32_t*)((BYTE*)&t1 + sizeof(int64_t));
	uint32_t n2 = *(uint32_t*)((BYTE*)&t2 + sizeof(int64_t));

	if (n1 >= n2)
		nanosec = n1 - n2;
	else
	{
		--sec;
		nanosec = (1000000000 + n1) - n2;
	}
	return CZetTimeSpan(sec, nanosec);
}
//-----------------------------------------------------------------------------
inline bool operator < (const CZetTimeSpan & t1, const CZetTimeSpan & t2)
{
	return const_cast<CZetTimeSpan&>(t1) < t2;
}
//-----------------------------------------------------------------------------
inline bool operator > (const CZetTimeSpan & t1, const CZetTimeSpan & t2)
{
	return const_cast<CZetTimeSpan&>(t1) > t2;
}
//-----------------------------------------------------------------------------
inline bool operator == (const CZetTimeSpan & t1, const CZetTimeSpan & t2)
{
	return const_cast<CZetTimeSpan&>(t1) == t2;
}
//-----------------------------------------------------------------------------
inline bool operator != (const CZetTimeSpan & t1, const CZetTimeSpan & t2)
{
	return const_cast<CZetTimeSpan&>(t1) != t2;
}
//-----------------------------------------------------------------------------
inline bool operator < (const CZetTime & t1, const CZetTime & t2)
{
	return const_cast<CZetTime&>(t1) < t2;
}
//-----------------------------------------------------------------------------
inline bool operator > (const CZetTime & t1, const CZetTime & t2)
{
	return const_cast<CZetTime&>(t1) > t2;
}
//-----------------------------------------------------------------------------
inline bool operator == (const CZetTime & t1, const CZetTime & t2)
{
	return const_cast<CZetTime&>(t1) == t2;
}
//-----------------------------------------------------------------------------
inline bool operator != (const CZetTime & t1, const CZetTime & t2)
{
	return const_cast<CZetTime&>(t1) != t2;
}
//-----------------------------------------------------------------------------
// Получение указателя на класс времени из ZetHeader, заданного по ссылке
ZETTOOLS_API CZetTime * ztGetPointerCTimeInHeader(_In_ ZetHeader & zh);
//-----------------------------------------------------------------------------
// Получение указателя на класс времени из ZetHeader, заданного по указателю
ZETTOOLS_API CZetTime * ztGetPointerCTimeInHeader(_Inout_ ZetHeader * pzh);
//-----------------------------------------------------------------------------
// Получение количества дней в месяце по номеру года и номеру месяца
// _year - номер года (0 ... бесконечности)
// _month - номер месяца (1 - 12)
//
// Возвращаемое значение:
// 0 - месяц указан не правильно;
// >0 - количество дней в месяце.
ZETTOOLS_API uint16_t ztGetDaysInMonth(_In_ uint16_t _year, _In_ uint16_t _month);
//-----------------------------------------------------------------------------
// Получение количества дней в году по номеру года
// _year - номер года (0 ... бесконечности)
//
// Возвращаемое значение: количество дней в месяце.
ZETTOOLS_API uint16_t ztGetDaysInYear(_In_ uint16_t _year);
//-----------------------------------------------------------------------------
// Формирование строки std::wstring (до 299 символов !)
// _format - строка форматирования wstring
// ... - переменное количество аргументов
ZETTOOLS_API std::wstring ztCreateWstring(_In_ LPCWSTR _format, ...);
//-----------------------------------------------------------------------------
// Расчет 16-ти битной контрольной суммы с помощью циклического избыточного кода
ZETTOOLS_API unsigned short ztCRC16(_In_ unsigned short usCRC, _Inout_ const void *pBuffer, _In_ unsigned int uiSize);
//-----------------------------------------------------------------------------
// Расчет 32-х битной контрольной суммы с помощью циклического избыточного кода
ZETTOOLS_API unsigned int ztCRC32(_In_ unsigned int uiCRC, _Inout_ const void *pBuffer, _In_ unsigned int uiSize);

// Расчет 32-х битной контрольной суммы на основе строки wchar-символов
ZETTOOLS_API unsigned int ztCRC32(_Inout_ LPCWSTR lpwBuf);

// Расчет 32-х битной контрольной суммы на основе строки char-символов
ZETTOOLS_API unsigned int ztCRC32(_Inout_ LPCSTR lpBuf);
//-----------------------------------------------------------------------------
// Пoлучение имени собственного exe-файла (по честному, не GetAppName)
// Возвращаемое значение:
// отрицательное значение - код ошибки (при этом возвращается пустая строк);
// 0 - функция выполнена успешно.
ZETTOOLS_API long ztGetNameSelfExe(_Inout_ LPWSTR lpBuf, _In_ unsigned int sizeBuf);
//------------------------------------------------------------------------------
// Пoлучение номера запущенной копии процесса
// Возвращаемое значение:
// отрицательное значение - код ошибки (при этом Number = 0);
// 0 - функция выполнена успешно.
ZETTOOLS_API long ztGetNumberSelfExe(_Inout_ unsigned short * pNumber);
//------------------------------------------------------------------------------
// Получение временной зоны из локали
// Возвращаемое значение:
// отрицательное значение - код ошибки (при этом *pZone = Utc);
// 0 - функция выполнена успешно.
_Check_return_
ZETTOOLS_API long ztGetCurrentTimeZone(_Inout_ TimeZones * pZone);
//------------------------------------------------------------------------------