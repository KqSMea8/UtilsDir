#ifndef time_converter_h_
#define time_converter_h_

#include <time.h>
#include <stdint.h>

// To FileTime
static BOOL ToFileTime(FILETIME* pFileTime, const SYSTEMTIME* pSystemTime)
{
	return SystemTimeToFileTime(pSystemTime, pFileTime);
}
static BOOL ToFileTime(FILETIME* pFileTime, const __time32_t Time_t)
{
	LONGLONG ll = Int32x32To64(Time_t, 10000000) + 116444736000000000;
	pFileTime->dwLowDateTime = (DWORD)ll;
	pFileTime->dwHighDateTime = (DWORD)(ll >> 32);
	return TRUE;
}
static BOOL ToFileTime(FILETIME* pFileTime, const __time64_t Time_t)
{
	// Convert sec to Nano sec.. Multi by 10 Mil, then add the difference between 1601 and 1970
	LONGLONG ll = (Time_t * 10000000) + 116444736000000000;
	pFileTime->dwLowDateTime = (DWORD)ll;
	pFileTime->dwHighDateTime = (DWORD)(ll >> 32);
	return TRUE;
}
static BOOL ToFileTime(FILETIME* pFileTime, const double variantTime)
{
	SYSTEMTIME systime;

	if (VariantTimeToSystemTime(variantTime, &systime) == FALSE)
		return FALSE;

	return ToFileTime(pFileTime, &systime);
}
static BOOL ToFileTime(FILETIME* pFileTime, const WORD dosDate, const WORD dosTime)
{
	return DosDateTimeToFileTime(dosDate, dosTime, pFileTime);
}
static BOOL ToLocalFileTime(FILETIME* pLocalFileTime, const FILETIME* pFileTime)
{
	return ::FileTimeToLocalFileTime(pFileTime, pLocalFileTime);
}
static BOOL ToLocalFileTime(FILETIME* pLocalFileTime, const SYSTEMTIME* st)
{
	FILETIME ft;
	if (ToFileTime(&ft, st))
	{
		return ::FileTimeToLocalFileTime(&ft, pLocalFileTime);
	}
	return FALSE;
}
static BOOL ToLocalRelFileTime(FILETIME* pLocalFileTime, const FILETIME* pFileTime)
{
	SYSTEMTIME stUTC;
	SYSTEMTIME stLocal;
	if (!FileTimeToSystemTime(pFileTime, &stUTC))
		return FALSE;
	if (!SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal))
		return FALSE;
	if (!ToFileTime(pLocalFileTime, &stLocal))
		return FALSE;
	return TRUE;
}
static BOOL FromFileTimeLocal(FILETIME* pFileTime, const FILETIME* pLocalFileTime)
{
	if (!::LocalFileTimeToFileTime(pLocalFileTime, pFileTime))
		return FALSE;
	return TRUE;
}
static BOOL FromFileTimeLocalRel(FILETIME* pFileTime, const FILETIME* pLocalFileTime)
{
	SYSTEMTIME stUTC;
	SYSTEMTIME stLocal;

	if (!FileTimeToSystemTime(pLocalFileTime, &stLocal))
		return FALSE;

	if (!TzSpecificLocalTimeToSystemTime(NULL, &stLocal, &stUTC))
		return FALSE;

	if (!ToFileTime(pFileTime, &stUTC))
		return FALSE;
	return TRUE;
}
static __int64  FileTimeToINT64(FILETIME* ft)
{
	ULARGE_INTEGER ul;
	ul.HighPart = ft->dwHighDateTime;
	ul.LowPart = ft->dwLowDateTime;
	return ul.QuadPart;
}
static void INT64ToFileTime(FILETIME* ft, __int64 nValue)
{
	ULARGE_INTEGER ul;
	ul.QuadPart = nValue;
	ft->dwHighDateTime = ul.HighPart;
	ft->dwLowDateTime = ul.LowPart;
}

// To SystemTime
static BOOL ToSystemTime( SYSTEMTIME* pSystemTime , const FILETIME* pFileTime )
{
	return ::FileTimeToSystemTime( pFileTime, pSystemTime );
}
static BOOL ToSystemTime( SYSTEMTIME* pSystemTime , const __time32_t dwTime_t )
{
	FILETIME ft;
	if (!ToFileTime(&ft, dwTime_t))
		return FALSE;

	if (!ToSystemTime(pSystemTime, &ft))
		return FALSE;

	return TRUE;
}
static BOOL ToSystemTime(SYSTEMTIME* pSystemTime, const __time64_t dwTime_t)
{
	FILETIME ft;
	if( !ToFileTime( &ft , dwTime_t ) )
		return FALSE;

	if( !ToSystemTime( pSystemTime , &ft) )
		return FALSE;

	return TRUE;
	}
static BOOL ToSystemTime( SYSTEMTIME* pSystemTime , const WORD dosDate , const WORD dosTime )
{
	FILETIME ft;
	if( !ToFileTime( &ft , dosDate , dosTime ) )
		return FALSE;

	return ToSystemTime( pSystemTime , &ft );
}

// To Dos Time
static void DosTimeWORD2DWORD(DWORD* pDosDateTime, WORD Date, WORD Time)
{
	*pDosDateTime = MAKELONG(Time, Date);
}
static void DosTimeDWORD2WORD(WORD* pDate, WORD* pTime, DWORD dosDateTime)
{
	*pDate = HIWORD(dosDateTime);
	*pTime = LOWORD(dosDateTime);
}
static BOOL ToDosDateTime(WORD* pDate, WORD* pTime, const FILETIME* pFileTime)
{
	return ::FileTimeToDosDateTime(pFileTime, pDate, pTime);
}
static BOOL ToDosDateTime(DWORD* pDateTime, const FILETIME* pFileTime)
{
	WORD Date = 0;
	WORD Time = 0;
	if (ToDosDateTime(&Date, &Time, pFileTime))
	{
		*pDateTime = MAKELONG(Time, Date);
		return TRUE;
	}
	return FALSE;
}
static BOOL ToDosDateTime( DWORD* pDateTime , const __time32_t dwTime_t )
{
	FILETIME ft;
	if (!ToFileTime(&ft, dwTime_t))
		return FALSE;
	return ToDosDateTime(pDateTime, &ft);
}
static BOOL ToDosDateTime(WORD* pDate, WORD* pTime, const SYSTEMTIME* st)
{
	FILETIME ft;
	if (!ToFileTime(&ft, st))
		return FALSE;
	
	if (!ToDosDateTime(pDate, pTime, &ft))
		return FALSE;

	return TRUE;
}
static BOOL ToDosDateTime( DWORD* pDosDateTime , const SYSTEMTIME* st )
{
	WORD Date;
	WORD Time;

	if( !ToDosDateTime( &Date , &Time , st ) )
		return FALSE;

	DosTimeWORD2DWORD( pDosDateTime , Date , Time );
	return TRUE;
}

// To UnixTime
static BOOL ToTime_t( __time32_t* pdwTime_t , const FILETIME* pFileTime )
	{
		if( pdwTime_t == NULL )
			return FALSE;


		// converts 64-bit FILETIME specifying the number of 100-nanosecond
		// intervals which have passed since January 1, 1601.
		LONGLONG t = pFileTime->dwHighDateTime;
		t <<= 32;
		t += (UINT32)pFileTime->dwLowDateTime;
		t -= 116444736000000000L; // difference between 1601 and 1970

		// divide by 10million to convert to seconds
		if (t < 0)
		{
			*pdwTime_t = (DWORD)( -1 - ((-t - 1) / 10000000) );
		}
		else
		{
			*pdwTime_t = (DWORD)(t / 10000000);
		}

		return TRUE;
	}
static BOOL ToTime_t( __time64_t* pdwTime_t , const FILETIME* pFileTime )
  {
    if( pdwTime_t == NULL )
      return FALSE;
      
      
    // converts 64-bit FILETIME specifying the number of 100-nanosecond
    // intervals which have passed since January 1, 1601.
    ULARGE_INTEGER li;
    li.HighPart = pFileTime->dwHighDateTime;
    li.LowPart = pFileTime->dwLowDateTime;
    
    INT64 nSec = li.QuadPart - 116444736000000000L; // difference between 1601 and 1970

    // divide by 10million to convert to seconds
    *pdwTime_t = (DWORD)(nSec / 10000000);
    
    return TRUE;
  }
static BOOL ToTime_t( __time32_t* pdwTime_t , const SYSTEMTIME* pSystemTime )
	{
		FILETIME ft;
		if( !SystemTimeToFileTime( pSystemTime , &ft ) )
			return FALSE;

		return ToTime_t( pdwTime_t , &ft );
		
	}
static BOOL ToTime_t( __time64_t* pdwTime_t , const SYSTEMTIME* pSystemTime )
  {
    FILETIME ft;
    if( !SystemTimeToFileTime( pSystemTime , &ft ) )
      return FALSE;

    return ToTime_t( pdwTime_t , &ft );

  }
static BOOL ToTime_t(__time32_t* pdwTime_t, const WORD dosDate, const WORD dosTime)
{
	FILETIME ft;
	if (ToFileTime(&ft, dosDate, dosTime))
		return ToTime_t(pdwTime_t, &ft);

	return FALSE;
}
static BOOL ToTime_t( __time32_t* pdwTime_t , const DWORD dosDateTime )
{
	return ToTime_t( pdwTime_t , (WORD)(dosDateTime >> 16), (WORD)dosDateTime );
}
static BOOL ToTime_t(__time64_t* pdwTime_t, const WORD dosDate, const WORD dosTime)
{
	FILETIME ft;
	if (ToFileTime(&ft, dosDate, dosTime))
		return ToTime_t(pdwTime_t, &ft);

	return FALSE;
}
static BOOL ToTime_t( __time64_t* pdwTime_t , const DWORD dosDateTime )
{
	return ToTime_t( pdwTime_t , (WORD)(dosDateTime >> 16), (WORD)dosDateTime );
}

// UTC time_t to Local time_t
static __time32_t Time_tToLocalTime_t( const __time32_t Time_t )
	{
		FILETIME ft;
		FILETIME ftLocal;
		if (ToFileTime(&ft, Time_t))
		{
			if (ToLocalFileTime(&ftLocal, &ft))
			{
				__time32_t tLocal = 0;
				if (ToTime_t(&tLocal, &ftLocal))
					return tLocal;
			}
		}
		return 0;
	}
static __time64_t Time_tToLocalTime_t( const __time64_t Time_t )
  {
    FILETIME ft;
    FILETIME ftLocal;
	if (ToFileTime(&ft, Time_t))
    {
		if (ToLocalFileTime(&ftLocal, &ft))
      {
        __time64_t tLocal = 0;
		if (ToTime_t(&tLocal, &ftLocal))
          return tLocal;
      }
    }
    return 0;
  }

// UTC to local relative time ( if time is a time where dst should be active then adjust time for dst)
static __time32_t Time_tToLocalRelTime_t( const __time32_t Time_t )
	{
		if( Time_t == -1 )
			return 0;
		register struct tm stm = { 0 };
		register __time32_t time;

		if (_gmtime32_s(&stm, &Time_t) == 0)
			time = _mktime32(&stm);
		else
			return 0;

		return (2*Time_t-time);
	}
static __time64_t Time_tToLocalRelTime_t( const __time64_t Time_t )
	{
		if( Time_t == -1 )
			return 0;
		register struct tm stm;
		register __time64_t time;

		if (_gmtime64_s(&stm, &Time_t) == 0)
			time = _mktime64(&stm);
		else 
			return 0;

		return (2*Time_t - time);
	}

// Local Time_t to UTC time_t
static __time32_t LocalTime_tToUTCTime_t( const __time32_t Time_t )
	{
		// the lazy way
		FILETIME ft;
		__time32_t tUTC;
		if (ToFileTime(&ft, Time_t))
		{
			FromFileTimeLocal(&ft, &ft);
			ToTime_t(&tUTC, &ft);
			return tUTC;
		}
		return 0;
	}
static __time64_t LocalTime_tToUTCTime_t( const __time64_t Time_t )
  {
    // the lazy way
    FILETIME ft;
    __time64_t tUTC;
	if (ToFileTime(&ft, Time_t))
    {
		FromFileTimeLocal(&ft, &ft);
		ToTime_t(&tUTC, &ft);
		return tUTC;
    }
    return 0;
  }

static double operator-(const SYSTEMTIME& st1, const SYSTEMTIME& st2)
	{
		FILETIME ft1 = { 0 }, ft2 = { 0 };
		if (!SystemTimeToFileTime(&st1, &ft1))
			ft1 = { 0 };
		if (!SystemTimeToFileTime(&st2, &ft2))
			ft2 = { 0 };

		LARGE_INTEGER li1, li2, liRes;
		memcpy_s(&li1, sizeof(LARGE_INTEGER), &ft1, sizeof(FILETIME));
		memcpy_s(&li2, sizeof(LARGE_INTEGER), &ft2, sizeof(FILETIME));
		liRes.QuadPart = li1.QuadPart - li2.QuadPart;

		int64_t milliseconds = liRes.QuadPart / 10000;
		double seconds = double(milliseconds) / 1000.;

		return seconds;
	}

#endif	// time_converter_h_
