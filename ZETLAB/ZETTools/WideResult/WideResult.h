//------------------------------------------------------------------------------
#pragma once
#include <ZetDef.h>
//------------------------------------------------------------------------------
// �������� ������� ����� � ��������� ������! ��. UnitDef.h
//------------------------------------------------------------------------------
template <class T>
struct WideResult
{
	double	time;
	long	quality;
	T		value;

	WideResult()
		: time(0.)
		, quality(0)
	{
	//	value = (T)0;
	}
};
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
template <class Tzt, class Tvalue>
// Ttime - ��� CZetTime ��� CZetTimeSpan
struct WideResult_ZetTime
{
	Tzt	time;
	Tvalue	value;

	WideResult_ZetTime()
		: time()
	{
	}

// 	bool operator < (const WideResult_ZetTime & val)
// 	{
// 		return time < val.time;
// 	}
};
//------------------------------------------------------------------------------