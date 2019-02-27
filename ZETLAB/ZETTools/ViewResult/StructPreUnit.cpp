//-----------------------------------------------------------------------------
#pragma once
#include "stdafx.h"
#include <windows.h>
#include <math.h>
#include "StructPreUnit.h"
//-----------------------------------------------------------------------------
// Определение флага кратной/дольной единицы по степени
//
DWORD DefFlagByN(short n)
{
	DWORD ret(FLAG_ERROR);
	switch (n)
	{
	case -12:	ret = FLAG_PRE_UNIT_m12;	break;
	case -9:		ret = FLAG_PRE_UNIT_m9;		break;
	case -6:		ret = FLAG_PRE_UNIT_m6;		break;
	case -3:		ret = FLAG_PRE_UNIT_m3;		break;
	case -2:		ret = FLAG_PRE_UNIT_m2;		break;
	case -1:		ret = FLAG_PRE_UNIT_m1;		break;
	case 0:		ret = FLAG_PRE_UNIT_0;		break;
	case 1:		ret = FLAG_PRE_UNIT_p1;		break;
	case 2:		ret = FLAG_PRE_UNIT_p2;		break;
	case 3:		ret = FLAG_PRE_UNIT_p3;		break;
	case 6:		ret = FLAG_PRE_UNIT_p6;		break;
	case 9:		ret = FLAG_PRE_UNIT_p9;		break;
	case 12:	ret = FLAG_PRE_UNIT_p12;	break;
	}
	return ret;
}
//-----------------------------------------------------------------------------
// Определение степени кратной/дольной единицы по флагу
//
short DefNByFlag(DWORD flag)
{
	short ret(0);
	switch (flag)
	{
	case FLAG_PRE_UNIT_m12:		ret = -12;		break;
	case FLAG_PRE_UNIT_m9:		ret = -9;		break;
	case FLAG_PRE_UNIT_m6:		ret = -6;		break;
	case FLAG_PRE_UNIT_m3:		ret = -3;		break;
	case FLAG_PRE_UNIT_m2:		ret = -2;		break;
	case FLAG_PRE_UNIT_m1:		ret = -1;		break;
	case FLAG_PRE_UNIT_0:		ret = 0;		break;
	case FLAG_PRE_UNIT_p1:		ret = 1;		break;
	case FLAG_PRE_UNIT_p2:		ret = 2;		break;
	case FLAG_PRE_UNIT_p3:		ret = 3;		break;
	case FLAG_PRE_UNIT_p6:		ret = 6;		break;
	case FLAG_PRE_UNIT_p9:		ret = 9;		break;
	case FLAG_PRE_UNIT_p12:		ret = 12;		break;
	}
	return ret;
}
//-----------------------------------------------------------------------------
// Определение индекса кратной/дольной единицы (в массиве этих единиц) по степени
//
WORD DefIndxByN(const short n)
{
	WORD ret(0);
	switch (n)
	{
	case -12:	ret = 8;		break;
	case -9:		ret = 6;		break;
	case -6:		ret = 4;		break;
	case -3:		ret = 2;		break;
	case -2:		ret = 12;		break;
	case -1:		ret = 10;		break;
	case 0:		ret = 0;		break;
	case 1:		ret = 9;		break;
	case 2:		ret = 11;		break;
	case 3:		ret = 1;		break;
	case 6:		ret = 3;		break;
	case 9:		ret = 5;		break;
	case 12:	ret = 7;		break;
	}
	return ret;
}
//-----------------------------------------------------------------------------
// Определение по текущей степени кратной/дольной единицы степени допустимой
// соседней кратной/дольной единицы при увеличении степени
//
bool DefN_up(
	const short nSrc,           // степень текущей единицы
    short & nDst,               // степень соседней единицы
    const DWORD flagsTypeUnit)  // флаги единиц
{
	bool ret = (flagsTypeUnit & FLAG_TRANSFORM_ENABLE) != 0;
	if (ret)
	{
		DWORD flag = DefFlagByN(nSrc);
		ret = flag != FLAG_ERROR;
		if (ret)
		{
			long num(0);
			switch (flag)
			{
			case FLAG_PRE_UNIT_m12:		num = 12;		break;
			case FLAG_PRE_UNIT_m9:		num = 11;		break;
			case FLAG_PRE_UNIT_m6:		num = 10;		break;
			case FLAG_PRE_UNIT_m3:		num = 9;		break;
			case FLAG_PRE_UNIT_m2:		num = 8;		break;
			case FLAG_PRE_UNIT_m1:		num = 7;		break;
			case FLAG_PRE_UNIT_0:		num = 6;		break;
			case FLAG_PRE_UNIT_p1:		num = 5;		break;
			case FLAG_PRE_UNIT_p2:		num = 4;		break;
			case FLAG_PRE_UNIT_p3:		num = 3;		break;
			case FLAG_PRE_UNIT_p6:		num = 2;		break;
			case FLAG_PRE_UNIT_p9:		num = 1;		break;
			case FLAG_PRE_UNIT_p12:		num = 0;		break;
			}
			ret = num > 0;
			if (ret)
			{
				for (int i = 0; i < num; ++i)
				{
					flag = flag << 1;
					ret = (flagsTypeUnit & flag) != 0;
					if (ret)
					{
						nDst = DefNByFlag(flag);
						break;
					}
				}
			}
		}
	}
	return ret;
}
//-----------------------------------------------------------------------------
// Определение по текущей степени кратной/дольной единицы степени допустимой
// соседней кратной/дольной единицы при уменьшении степени
//
bool DefN_doun(
	const short nSrc,           // степень текущей единицы
	short & nDst,               // степень соседней единицы
	const DWORD flagsTypeUnit)  // флаги единиц
{
	bool ret = (flagsTypeUnit & FLAG_TRANSFORM_ENABLE) != 0;
	if (ret)
	{
		DWORD flag = DefFlagByN(nSrc);
		ret = flag != FLAG_ERROR;
		if (ret)
		{
			long num(0);
			switch (flag)
			{
			case FLAG_PRE_UNIT_m12:		num = 0;		break;
			case FLAG_PRE_UNIT_m9:		num = 1;		break;
			case FLAG_PRE_UNIT_m6:		num = 2;		break;
			case FLAG_PRE_UNIT_m3:		num = 3;		break;
			case FLAG_PRE_UNIT_m2:		num = 4;		break;
			case FLAG_PRE_UNIT_m1:		num = 5;		break;
			case FLAG_PRE_UNIT_0:		num = 6;		break;
			case FLAG_PRE_UNIT_p1:		num = 7;		break;
			case FLAG_PRE_UNIT_p2:		num = 8;		break;
			case FLAG_PRE_UNIT_p3:		num = 9;		break;
			case FLAG_PRE_UNIT_p6:		num = 10;		break;
			case FLAG_PRE_UNIT_p9:		num = 11;		break;
			case FLAG_PRE_UNIT_p12:		num = 12;		break;
			}
			ret = num > 0;
			if (ret)
			{
				for (int i = 0; i < num; ++i)
				{
					flag = flag >> 1;
					ret = (flagsTypeUnit & flag) != 0;
					if (ret)
					{
						nDst = DefNByFlag(flag);
						break;
					}
				}
			}
		}
	}
	return ret;
}
//-----------------------------------------------------------------------------
STRUCT_PRE_UNIT::STRUCT_PRE_UNIT(const short _n)
{
	SetOrder(_n);
//	ZeroMemory(nameInt, sizeof(nameInt));
	ZeroMemory(nameLoc, sizeof(nameLoc));
}
//-----------------------------------------------------------------------------
void STRUCT_PRE_UNIT::SetOrder(const short _n)
{
	n = _n;
	flag = DefFlagByN(n);
	k = pow(10., n);
}
//----------------------------------------------------------------------------
// void STRUCT_PRE_UNIT::SetNameInt(LPCWCH name)
// {
// 	wcsncpy_s(nameInt, name, SIZE_NAME_PRE_UNIT - 1);
// }
//----------------------------------------------------------------------------
void STRUCT_PRE_UNIT::SetNameLoc(LPCWCH name)
{
	wcsncpy_s(nameLoc, name, SIZE_NAME_PRE_UNIT - 1);
}
//----------------------------------------------------------------------------