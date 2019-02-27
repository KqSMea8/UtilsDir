//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
//------------------------------------------------------------------------------
#define Quality_AllData_True	0
#define Quality_AllData_False	1
//------------------------------------------------------------------------------
// Структура качества данных
struct CodeQuality
{
	unsigned Synchro:	4;		// данные с пропусками
	unsigned Convert:	4;		// ошибки преобразования данных
	unsigned Algorithm:	4;		// ошибки обработки данных
	unsigned SelfTest:	4;		// ошибки тестирования
	unsigned NaN:		4;		// в данных есть NaN
	unsigned :			4;
	unsigned Comment:	4;		// комментарий
	unsigned :			1;		// 
	unsigned NeedSynchro:	1;	// требуется синхронизация
	unsigned AllData:	1;		// собраны все данные для обработки
	unsigned Error:		1;		// нет данных
	
	CodeQuality()		{ *this = 0; }
	CodeQuality(const CodeQuality &a)		{ *this = a; }
	CodeQuality(const long &a)				{ *this = a; }

	bool IsAllData()	{ return AllData == Quality_AllData_True; }
	bool NoError()		{ return Error == 0; }
	bool IsError()		{ return Error == 1; }
	bool IsQuality()
	{
		return (Error == 0) &&
				( (Synchro   != 0) || (Convert != 0) ||
				  (Algorithm != 0) || (SelfTest != 0) );
	}
	bool IsOk()
	{
		return (AllData == Quality_AllData_True) && (Synchro == 0) &&
				(Convert == 0) && (Algorithm == 0) && (SelfTest == 0);
	}

	operator long () { return *((long*)this); }
	void operator = (const long a)  { *((long*)this) = a; };
	void operator |= (const CodeQuality &a) { *((long*)this) |= *((long*)&a); }
	void operator |= (const long a) { *((long*)this) |= a; }
};
//------------------------------------------------------------------------------