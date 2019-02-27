//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
#ifdef ZETTOOLS_EXPORTS
#define ZETTOOLS_API __declspec(dllexport)
#else
#define ZETTOOLS_API __declspec(dllimport)
#endif
//-----------------------------------------------------------------------------
#include <wtypes.h>			// ����������� DATE
#include <TimeZones.h>		// ����������� TimeZones
#include <ZetErrorCodes.h>	// ���� ������
#include <zet_struct1.h>	// ZetHeader & ZetFooter
#include <atltime.h>		// ����������� CTime
#include <string>			// std::wstring
//-----------------------------------------------------------------------------
#define NUMBER_MONTHS_IN_YEAR      12	// ���������� ������� � ����

#define NUMBER_FIRST_MONTH_IN_YEAR 1	// ����� ������� ������ � ����
#define NUMBER_FIRST_DAY_IN_MONTH 1 // ����� ������� ����� � ������
#define NUMBER_FIRST_HOUR_IN_DAY  0	// ����� ������� ���� � ���
#define NUMBER_FIRST_MIN_IN_HOUR  0 // ����� ������ ������ � ����
#define NUMBER_FIRST_SEC_IN_MIN   0 // ����� ������ ������� � ������

#define NUMBER_SEC_IN_MIN   60

#define NUMBER_SEC_IN_HOUR	3600 // = 60 * 60
#define NUMBER_MIN_IN_HOUR  60

#define NUMBER_SEC_IN_DAY 86400	// = 60 * 60 * 24
#define NUMBER_MIN_IN_DAY 1440	// = 60 * 24
#define NUMBER_HOURS_IN_DAY 24	


#define NUMBER_SEC_IN_3000 32503680000	// ���-�� ������ � ������������ ��������

#define ZetTimeMin CZetTime(0Ui64, 0U)			// 01.01.1970 00:00:00
#define ZetTimeMax CZetTime(NUMBER_SEC_IN_3000, 0U)	// 01.01.3000 00:00:00
//-----------------------------------------------------------------------------
// ��������� �������� ������� UTC.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ����� ��������.
_Check_return_
ZETTOOLS_API long ztGetCurrentTimeUTC(_Inout_ DATE * pTimeUTC);
//-----------------------------------------------------------------------------
// ��������� �������� ���������� �������.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ����� ��������.
_Check_return_
ZETTOOLS_API long ztGetCurrentTimeLocal(_Inout_ DATE * pTimeLocal);
//-----------------------------------------------------------------------------
// �������������� ���������� ������� �� ����� UTC.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ����� �������������.
_Check_return_
ZETTOOLS_API long ztTimeLocalToTimeUTC(_In_ const DATE timeLocal, _Inout_ DATE * pTimeUTC);
//-----------------------------------------------------------------------------
// �������������� ������� UTC � ��������� �����.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ����� �������������.
_Check_return_
ZETTOOLS_API long ztTimeUTCtoTimeLocal(_In_ const DATE timeUTC, _Inout_ DATE * pTimeLocal);
//-----------------------------------------------------------------------------
// �������������� ���������� ������� �� ����� UTC.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ����� �������������.
_Check_return_
ZETTOOLS_API long ztTimeZoneToTimeUTC(_In_ const TimeZones zone, _In_ const DATE timeZone, _Inout_ DATE * pTimeUTC);
//-----------------------------------------------------------------------------
// �������������� ������� UTC � ��������� �����.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ����� �������������.
_Check_return_
ZETTOOLS_API long ztTimeUTCtoTimeZone(_In_ const TimeZones zone, _In_ const DATE timeUTC, _Inout_ DATE * pTimeZone);
//-----------------------------------------------------------------------------
// ���������� ������ �� �������.
// ��������� ����� ��� �������������, ��� � ������� ��������.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ���������.
_Check_return_
ZETTOOLS_API long ztAddSecondToDATE(_In_ const double second, _Inout_ DATE * pTime);
//-----------------------------------------------------------------------------
// ������ �������� ������� UTC � ��������� ���������.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ����� ��������.
_Check_return_
ZETTOOLS_API long ztSetStructureCurrentTimeUTC(_Inout_ ZetHeader * pStructure);
//-----------------------------------------------------------------------------
// ������ ������� UTC � ��������� ���������.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ����� ��������.
_Check_return_
ZETTOOLS_API long ztSetStructureTimeUTC(_Inout_ ZetHeader * pStructure, _In_ const DATE timeUTC);
//-----------------------------------------------------------------------------
// �������������� ���������� ������� �� ����� UTC � ������ � ��������� ���������.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ����� ��������.
_Check_return_
ZETTOOLS_API long ztSetStructureTimeLocal(_Inout_ ZetHeader * pStructure, _In_ const DATE timeLocal);
//-----------------------------------------------------------------------------
// ��������� �� ���������� ��������� ������� UTC.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ����� ��������.
_Check_return_
ZETTOOLS_API long ztGetStructureTimeUTC(_In_ const ZetHeader * pStructure, _Inout_ DATE * pTimeUTC);
//-----------------------------------------------------------------------------
// ��������� �� ���������� ��������� ������� UTC � �������������� ��� � ��������� �����.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ����� �������� � �������������.
_Check_return_
ZETTOOLS_API long ztGetStructureTimeLocal(_In_ const ZetHeader * pStructure, _Inout_ DATE * pTimeLocal);
//-----------------------------------------------------------------------------
// ��������� ���� ���������. � ��������� �������� ���� � �������� � ����� ���������.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ���������.
_Check_return_
ZETTOOLS_API long ztOpenStructure(_Inout_ ZetHeader * pStructure,
	_In_ const UINT structureType, _In_ const UINT structureSize);
//-----------------------------------------------------------------------------
// �������� ����� ���������, ���������� ����������� �����.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ���������.
_Check_return_
ZETTOOLS_API long ztCloseStructure(_Inout_ ZetHeader * pStructure);
//--------------------------------------------------------------------------
// �������� ��������� (����� � ����������� �����).
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - �������� �������.
_Check_return_
ZETTOOLS_API long ztIsStructureValid(_Inout_ const ZetHeader * pStructure);
//-----------------------------------------------------------------------------
// ����������� ����� ��������� � ������.
// �������� ��������� �� ������� ���� ��������.
// ���� � ������� �������� ������ ���������
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ����������� ���������.
_Check_return_
ZETTOOLS_API long ztCopyStructure(_Inout_ ZetHeader * pDestStruct, _In_ const ZetHeader * pSrcStruct);
//-----------------------------------------------------------------------------
// ����������� ������� �� ��������� ����� ��������� � ��������� ������.
// ��������� �� ������� ���� ��������� � ���� ������ ����.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ����������� ���������.
_Check_return_
ZETTOOLS_API long ztCopyStructureTime(_Inout_ ZetHeader * pDestStruct, _In_ const ZetHeader * pSrcStruct);
//-----------------------------------------------------------------------------
// ������� �������������� � ������ ��������� � ZetHeader.
// programID  - ������������� ��������� (�� 0).
// programNum - ����� ���������� ����� ��������� (���� � 1).
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ���������.
ZETTOOLS_API long ztSetProgramValuesInHeader(_Inout_ ZetHeader * pStruct,
	_In_ const uint64_t programID, _In_ const uint16_t programNum,
	_In_ const bool bNeedClose = true);
//-----------------------------------------------------------------------------
// ������� �������������� � ������ ������ ��������� � ZetHeader.
// moduleID   - ������������� ������ � ��������� (�� 0).
// moduleNum  - ����� ������ � ��������� (���� � 1).
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ���������.
ZETTOOLS_API long ztSetModuleValuesInHeader(_Inout_ ZetHeader * pStruct,
	_In_ const uint32_t moduleID, _In_ const uint16_t moduleNum,
	_In_ const bool bNeedClose = true);
//-----------------------------------------------------------------------------
// ������ � ZetWindowsLog ��������� �� ������ (char-�����)
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ���������.
ZETTOOLS_API long ztMessageErrorA(_In_ LPCSTR lpText, _In_ const long code = 0);

// ������ � ZetWindowsLog ��������� �� ������ (wchar-�����)
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ���������.
ZETTOOLS_API long ztMessageErrorW(_In_ LPCWSTR lpwText, _In_ const long code = 0);

#ifdef UNICODE
#define ztMessageError  ztMessageErrorW
#else
#define ztMessageError  ztMessageErrorA
#endif
//-----------------------------------------------------------------------------
// ������ � ZetWindowsLog ��������� � �������������� (char-�����)
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ���������.
ZETTOOLS_API long ztMessageWarningA(_In_ LPCSTR lpText, _In_ const long code = 0);

// ������ � ZetWindowsLog ��������� � �������������� (�char-�����)
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ���������.
ZETTOOLS_API long ztMessageWarningW(_In_ LPCWSTR lpwText, _In_ const long code = 0);

#ifdef UNICODE
#define ztMessageWarning  ztMessageWarningW
#else
#define ztMessageWarning  ztMessageWarningA
#endif
//-----------------------------------------------------------------------------
// ������ � ZetWindowsLog ��������������� ��������� (char-�����)
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ���������.
ZETTOOLS_API long ztMessageInformationA(_In_ LPCSTR lpText);

// ������ � ZetWindowsLog ��������������� ��������� (wchar-�����)
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ���������.
ZETTOOLS_API long ztMessageInformationW(_In_ LPCWSTR lpwText);

#ifdef UNICODE
#define ztMessageInformation  ztMessageInformationW
#else
#define ztMessageInformation  ztMessageInformationA
#endif
//-----------------------------------------------------------------------------
// ����������� char-������ �� lpwSrcBuf � lpwDestBuf.
// sizeDestBuf - ������ ������ ���������� � ��������.
// ���� � ������ ���� �����, �� ������������ ��� ������ Err_String_Less (-15)
// � ���������� ������� ��������, ������� ����������.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ���������.
ZETTOOLS_API long ztStringCopyA(_Inout_ LPSTR lpDestBuf, _In_ const UINT sizeDestBuf, _In_ LPCSTR lpSrcBuf);

// ����������� wchar-������ �� lpwSrcBuf � lpwDestBuf.
// sizeDestBuf - ������ ������ ���������� � ��������.
// ���� � ������ ���� �����, �� ������������ ��� ������ Err_String_Less (-15)
// � ���������� ������� ��������, ������� ����������.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ���������.
ZETTOOLS_API long ztStringCopyW(_Inout_ LPWSTR lpwDestBuf, _In_ const UINT sizeDestBuf, _In_ LPCWSTR lpwSrcBuf);

#ifdef UNICODE
#define ztStringCopy ztStringCopyW
#else
#define ztStringCopy  ztStringCopyA
#endif
//-----------------------------------------------------------------------------
// ���������� char-������ �� lpwSrcBuf � lpwDestBuf.
// sizeDestBuf - ������ ������ ���������� � ��������.
// ���� � ������ ���� �����, �� ������������ ��� ������ Err_String_Less (-15)
// � ���������� ������� ��������, ������� ����������.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ���������.
ZETTOOLS_API long ztStringAppendA(_Inout_ LPSTR lpDestBuf, _In_ const UINT sizeDestBuf, _In_ LPCSTR lpSrcBuf);

// ���������� wchar-������ �� lpwSrcBuf � lpwDestBuf.
// sizeDestBuf - ������ ������ ���������� � ��������.
// ���� � ������ ���� �����, �� ������������ ��� ������ Err_String_Less (-15)
// � ���������� ������� ��������, ������� ����������.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ���������.
ZETTOOLS_API long ztStringAppendW(_Inout_ LPWSTR lpwDestBuf, _In_ const UINT sizeDestBuf, _In_ LPCWSTR lpwSrcBuf);

#ifdef UNICODE
#define ztStringAppend ztStringAppendW
#else
#define ztStringAppend  ztStringAppendA
#endif
//-----------------------------------------------------------------------------
// ���������� char-������� � lpwDestBuf.
// sizeDestBuf - ������ ������ ���������� � ��������.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ���������.
ZETTOOLS_API long ztStringAppendSymbolA(_Inout_ LPSTR lpDestBuf, _In_ const UINT sizeDestBuf, _In_ const char symbol);

// ���������� wchar-������� � lpwDestBuf.
// sizeDestBuf - ������ ������ ���������� � ��������.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ���������.
ZETTOOLS_API long ztStringAppendSymbolW(_Inout_ LPWSTR lpwDestBuf, _In_ const UINT sizeDestBuf, _In_ const wchar_t symbol);

#ifdef UNICODE
#define ztStringAppendSymbol ztStringAppendSymbolW
#else
#define ztStringAppendSymbol  ztStringAppendSymbolA
#endif
//-----------------------------------------------------------------------------
// ������������� �������� ����-����� ��� �������������� ����������
ZETTOOLS_API void ztInitCreateDumpWhenException();
//-----------------------------------------------------------------------------
//------- ������ ������� ------------------------------------------------------
//-----------------------------------------------------------------------------
/*		����� CZetTimeSpan ������������ ��� �������� �������������� �������. 
(����� ��� �����). ���� ticks ��� ������������.
	����� ���� CZetTimeSpan ����� ��������/������ �� ����� ���� double.

	���������� ��������� (��������������� � ���� ������):
	double sec = seconds + nanoseconds / 1000000000
	(0, 0) == (-1, 1000000000);
	(3, 400) - (1, 200) = (2, 200)
	(1, 200) - (3, 400) = (0, 1000000200) - (3, 400) = (-3, 999999800)
	CZetTimeSpan x = -(0, 100) = (0, 0) - (0, 100) =
		(-1, 1000000000) - (0, 100) = (-1, 999999900)
	�������� ������� ����� � ������ private ������������� ���������!!!
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

	// ���������� �������� ��������� ���������� seconds
	int64_t GetSeconds() { return seconds; }
	int64_t GetSeconds() const { return seconds; }
	// ���������� �������� ��������� ���������� nanoseconds
	uint32_t GetNanoseconds() { return nanoseconds; }
	uint32_t GetNanoseconds() const { return nanoseconds; }

	// ������ ������������ ������� � ����� (���� ��� = 365,25 �����)
	double GetYears();

	// ������ ������������ ������� � ������� (���� ����� = 30 �����)
	double GetMonths();

	// ������ ������������ ������� � ����
	double GetDays();

	// ������ ������������ ������� � �����
	double GetHours();

	// ������ ������������ ������� � ������� 
	double GetMinutes();

	// ���������� �� ����� ������
	void RoundSeconds();

	// ���������� �� ����� ������ �����
	void RoundSecondsUp();

	// ���������� �� ����� ������ ����
	void RoundSecondsDown();

	// ������� ���-��� ������ � ����������
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
/*		����� CZetTime ������������ ��� �������� ����������� �������.
		����� ������ ������ ����� � ������� UTC !!!
����� �������� � ���� �����: seconds (����� ��� �����) - ���-�� ������,
��������� � 01.01.1970, � nanoseconds (����� ��� �����).
���� ticks ��� ������������.

	���������� ���������� ����� ������ ������!!!
	������� ����������� ����� ������ - ��� ��������� ������ CZetTimeSpan!!!
	��������� ����� ������ ����� ����������/�������� � ������������ CZetTimeSpan.

	��� ����������� ���������� ��������������� � ���� ������.
�������� ������� ����� � ������ private ������������� ���������!!!
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

	// ���������� �������� ��������� ���������� seconds
	uint64_t GetSeconds() const { return seconds; }
	uint64_t GetSeconds() { return seconds; }

	// ���������� �������� ��������� ���������� nanoseconds
	uint32_t GetNanoseconds() { return nanoseconds; }
	uint32_t GetNanoseconds() const { return nanoseconds; }

	// ��������� ���� � �������� �������.
	// ���� bTimeLocal = true, �� ��� ���������� �������� �������,
	// ����� - ��� �������� ������� UTC.
	// ������������ ��������:
	// 0    - ZetTimeMin <= time <= ZetTimeMax
	// -308 - �������� ������ 01.01.1970 ��� ������ 01.01.3000, ��������������� year = 0
	long GetYear(uint16_t & year, const bool bTimeLocal = true);

	// ��������� ������ � �������� �������.
	// ���� bTimeLocal = true, �� ����� ���������� �������� �������,
	// ����� - ����� �������� ������� UTC.
	// ������������ ��������:
	// 0    - ZetTimeMin <= time <= ZetTimeMax
	// -308 - �������� ������ 01.01.1970 ��� ������ 01.01.3000, ��������������� month = 0
	long GetMonth(uint16_t & month, const bool bTimeLocal = true);

	// ��������� ���� � �������� �������.
	// ���� bTimeLocal = true, �� ���� ���������� �������� �������,
	// ����� - ���� �������� ������� UTC.
	// ������������ ��������:
	// 0    - ZetTimeMin <= time <= ZetTimeMax
	// -308 - �������� ������ 01.01.1970 ��� ������ 01.01.3000, ��������������� day = 0
	long GetDay(uint16_t & day, const bool bTimeLocal = true);

	// ��������� ���� � �������� �������.
	// ���� bTimeLocal = true, �� ��� ���������� �������� �������,
	// ����� - ��� �������� ������� UTC.
	// ������������ ��������:
	// 0    - ZetTimeMin <= time <= ZetTimeMax
	// -308 - �������� ������ 01.01.1970 ��� ������ 01.01.3000, ��������������� hour = 0
	long GetHour(uint16_t & hour, const bool bTimeLocal = true);
	
	// ��������� ������ � �������� �������.
	// ���� bTimeLocal = true, �� ������ ���������� �������� �������,
	// ����� - ������ �������� ������� UTC.
	// ������������ ��������:
	// 0    - ZetTimeMin <= time <= ZetTimeMax
	// -308 - �������� ������ 01.01.1970 ��� ������ 01.01.3000, ��������������� minute = 0
	long GetMinute(uint16_t & minute, const bool bTimeLocal = true);
	
	// ��������� ������ � �������� ������� (� ������ �������).
	// ���� bTimeLocal = true, �� ������� ���������� �������� �������,
	// ����� - ������� �������� ������� UTC.
	// ������������ ��������:
	// 0    - ZetTimeMin <= time <= ZetTimeMax
	// -308 - �������� ������ 01.01.1970 ��� ������ 01.01.3000, ��������������� second = 0
	long GetSecond(double & second);

	// ���� ���� ������ � 0 (�����������), ����� 1 - (�����������) � �.�.
	// ������������ ��������:
	// 0    - ZetTimeMin <= time <= ZetTimeMax
	// -308 - �������� ������ 01.01.1970 ��� ������ 01.01.3000, ��������������� dayOfWeek = 0
	long GetDayOfWeek(uint16_t & dayOfWeek, const bool bTimeLocal = true);
	
	// �������� �������� �������
	// ������������ ��������:
	// 0    - ZetTimeMin <= time <= ZetTimeMax
	// -308 - �������� ������ 01.01.1970 ��� ������ 01.01.3000
	long TestTimeValue();

	// ������� ������� ��� ������� ����� UTC
	// ������������ ��������: ������ 0.
	long SetCurrentTime();

	// ������� ������� ��� ������� ��������� �����
	// ������������ ��������: ������ 0.
	long SetLocalTime();

	// ����������� ������� � ZetHeader (����������� �� �������� �� �������� ������������� ��������).
	// ������������ ��������:
	// 0    - ZetTimeMin <= time <= ZetTimeMax
	// -308 - �������� ������ 01.01.1970 ��� ������ 01.01.3000
	long CopyTimeToZetHeader(ZetHeader & zh);

	// ���������� ������� �� ����� ������ (� ������������ � ������������� ���������)
	void RoundSeconds();

	// ���������� ������� �� ����� ������ �����
	void RoundSecondsUp();

	// ���������� ������� �� ����� ������ ����
	void RoundSecondsDown();

	// ������� ���-��� ������ � ����������
	void SetValues(uint64_t sec = 0Ui64, uint32_t nanosec = 0U);

	// �������������� ������� � ������ � �������� ��������� � �� ��������� �������.
	// ������ �������: L"dd.MM.yyyy HH:mm:ss"
	// precision - �������� ������������� ����� �������, ������� ����� ������� ������ ������
	// ���� ������ �� �����, �� �� ������ �� ������ (������� ����, ����� �����).
	// bTimeLocal = true - � ��������� �� UTC � ��������� �����.
	// bTimeLocal = false - ��� �������� �� UTC � ��������� �����.
	// ������������ ��������:
	// ������������� �������� - ��� ������, ������������ ������ ������;
	// 0 - �������� ����������.
	long ConvertToString(_In_ LPWSTR lpwsBuffer, _In_ const DWORD sizeBuffer,
						 _In_opt_ const double precision = 1.,
						 _In_opt_ LPCWSTR lpcwsFormat = nullptr,
						 _In_opt_ const bool bTimeLocal	= true);

	// �������������� ������� � ������ � �������� ��������� � �� ��������� �������
	// �������: https://msdn.microsoft.com/query/dev12.query?appId=Dev12IDEF1&l=EN-US&k=k(wchar%2Fwcsftime);k(wcsftime);k(DevLang-C%2B%2B);k(TargetOS-Windows)&rd=true
	// ������ �������: L"%d.%m.%Y %H:%M:%S"
	// bTimeLocal = true - � ��������� �� UTC � ��������� �����.
	// bTimeLocal = false - ��� �������� �� UTC � ��������� �����.
	// ������������ ��������:
	// ������������� �������� - ��� ������, ������������ ������ ������;
	// 0 - �������� ����������.
	long ConvertToStringWithFormat(_In_ LPWSTR lpwsBuffer, _In_ const DWORD sizeBuffer,
								   _In_opt_  const double precision = 1., 
								   _In_opt_ LPCWSTR lpcwsFormat = L"%d.%m.%Y %H:%M:%S",
								   _In_opt_ const bool bTimeLocal = true);

	// �������������� �������  �� ������ �� ��������� �������.
	// ������ �������: L"dd.MM.yyyy HH:mm:ss"
	// ���� ������ �� �����, �� �� ������ �� ������ (������� ����, ����� �����).
	// bTimeLocal = true - � ������ ��������� �����.
	// bTimeLocal = false - � ������ ����� UTC.
	// ������������ ��������:
	// ������������� �������� - ��� ������;
	// -308 - �������� ������ 01.01.1970 ��� ������ 01.01.3000
	// 0 - �������� ����������.
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
// ��������� ��������� �� ����� ������� �� ZetHeader, ��������� �� ������
ZETTOOLS_API CZetTime * ztGetPointerCTimeInHeader(_In_ ZetHeader & zh);
//-----------------------------------------------------------------------------
// ��������� ��������� �� ����� ������� �� ZetHeader, ��������� �� ���������
ZETTOOLS_API CZetTime * ztGetPointerCTimeInHeader(_Inout_ ZetHeader * pzh);
//-----------------------------------------------------------------------------
// ��������� ���������� ���� � ������ �� ������ ���� � ������ ������
// _year - ����� ���� (0 ... �������������)
// _month - ����� ������ (1 - 12)
//
// ������������ ��������:
// 0 - ����� ������ �� ���������;
// >0 - ���������� ���� � ������.
ZETTOOLS_API uint16_t ztGetDaysInMonth(_In_ uint16_t _year, _In_ uint16_t _month);
//-----------------------------------------------------------------------------
// ��������� ���������� ���� � ���� �� ������ ����
// _year - ����� ���� (0 ... �������������)
//
// ������������ ��������: ���������� ���� � ������.
ZETTOOLS_API uint16_t ztGetDaysInYear(_In_ uint16_t _year);
//-----------------------------------------------------------------------------
// ������������ ������ std::wstring (�� 299 �������� !)
// _format - ������ �������������� wstring
// ... - ���������� ���������� ����������
ZETTOOLS_API std::wstring ztCreateWstring(_In_ LPCWSTR _format, ...);
//-----------------------------------------------------------------------------
// ������ 16-�� ������ ����������� ����� � ������� ������������ ����������� ����
ZETTOOLS_API unsigned short ztCRC16(_In_ unsigned short usCRC, _Inout_ const void *pBuffer, _In_ unsigned int uiSize);
//-----------------------------------------------------------------------------
// ������ 32-� ������ ����������� ����� � ������� ������������ ����������� ����
ZETTOOLS_API unsigned int ztCRC32(_In_ unsigned int uiCRC, _Inout_ const void *pBuffer, _In_ unsigned int uiSize);

// ������ 32-� ������ ����������� ����� �� ������ ������ wchar-��������
ZETTOOLS_API unsigned int ztCRC32(_Inout_ LPCWSTR lpwBuf);

// ������ 32-� ������ ����������� ����� �� ������ ������ char-��������
ZETTOOLS_API unsigned int ztCRC32(_Inout_ LPCSTR lpBuf);
//-----------------------------------------------------------------------------
// �o������� ����� ������������ exe-����� (�� ��������, �� GetAppName)
// ������������ ��������:
// ������������� �������� - ��� ������ (��� ���� ������������ ������ �����);
// 0 - ������� ��������� �������.
ZETTOOLS_API long ztGetNameSelfExe(_Inout_ LPWSTR lpBuf, _In_ unsigned int sizeBuf);
//------------------------------------------------------------------------------
// �o������� ������ ���������� ����� ��������
// ������������ ��������:
// ������������� �������� - ��� ������ (��� ���� Number = 0);
// 0 - ������� ��������� �������.
ZETTOOLS_API long ztGetNumberSelfExe(_Inout_ unsigned short * pNumber);
//------------------------------------------------------------------------------
// ��������� ��������� ���� �� ������
// ������������ ��������:
// ������������� �������� - ��� ������ (��� ���� *pZone = Utc);
// 0 - ������� ��������� �������.
_Check_return_
ZETTOOLS_API long ztGetCurrentTimeZone(_Inout_ TimeZones * pZone);
//------------------------------------------------------------------------------