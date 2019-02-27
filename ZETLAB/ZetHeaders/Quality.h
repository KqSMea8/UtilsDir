//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
//------------------------------------------------------------------------------
#define Quality_AllData_True	0
#define Quality_AllData_False	1
//------------------------------------------------------------------------------
// —труктура качества данных
struct Quality
{
	unsigned Synchro:	4;		// данные с пропусками
	unsigned Convert:	4;		// ошибки преобразовани€ данных
	unsigned Algorithm:	4;		// ошибки обработки данных
	unsigned SelfTest:	4;		// ошибки тестировани€
	unsigned :			4;
	unsigned :			4;
	unsigned Comment:	6;		// комментарий
	unsigned AllData:	1;		// собраны все данные дл€ обработки
	unsigned Error:		1;		// нет данных
	// из-за того, что Quality входит в union (см. ниже),
	// то конструктора у этой структуры не должно быть
	bool IsAllData()	{ return AllData == 0; }
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
		return (AllData == 0) && (Synchro == 0) && (Convert == 0) &&
				(Algorithm == 0) && (SelfTest == 0);
	}

	void operator = (const long a)  { *((long*)this) = a; };
	void operator = (const DWORD a) { *((long*)this) = a; };
};
//------------------------------------------------------------------------------
// качество данных
union QualityCode
{
	long	valL;
	Quality	valQ;
	
	QualityCode() { valL = 0; }
	QualityCode(const long a)    { valL = a; }
	QualityCode(const Quality &a) { valL = *((long*)&a); }
	QualityCode(const QualityCode &val) { valL = val.valL; }

	void operator = ( const long a )    { valL = a; }
	void operator = ( const Quality &a ) { valL = *((long*)&a); }
};
//------------------------------------------------------------------------------