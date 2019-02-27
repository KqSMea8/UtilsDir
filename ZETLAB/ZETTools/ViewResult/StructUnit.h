﻿//-----------------------------------------------------------------------------
/*	Структуры данных, используемые при работе с единицами измерений.
Примеры единиц измерений - метры, мили, секунды, атмостеры, Вольты, Амперы, и т.д.
Все единицы сгруппированы в некоторое кол-во групп (пока 10):
единицы длины, времени, массы, давления, температуры, электрического напряжения,
элетрического тока, ...
*/
//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
#define SIZE_NAME_UNIT					16
#define SIZE_MAX_NAME_GROUP_UNIT		28
//-----------------------------------------------------------------------------
#define g_wcharDegree	L'°'	// символ градуса
#define g_wcharMin		L'′'	// символ минуты
#define g_wcharSec		L'″'	// символ секунды
//-----------------------------------------------------------------------------
// Структура параметров оператора преобразования одной величины в другую.
// Содержит мультипликативный и аддитивный коэффициеты преобразования
// y(x) = k * x + a
//
struct STRUCT_OPERATOR_CONVERT
{
	double k;	// мультипликативный коэффициент преобразования
	double a;	// аддитивный коэффициент преобразования
};
//-----------------------------------------------------------------------------
// Структура параметров исходной величины измерения
// 
struct STRUCT_UNIT_SRC
{
	WORD indxGroupUnit;	// индекс группы единиц, в которую входит данная единица
	WORD indxTypeUnit;	// индекс типа единиц, в которую входит данная единица
	short nPreUnit;		// показатель степени кратной/дольной единицы
	WORD flagsCur;		// текущие флаги единицы

	// оператор преобр. единицы в единицу СИ данной группы с учётом nPreUnit
	STRUCT_OPERATOR_CONVERT convertToSi;

//	wchar_t nameInt[SIZE_NAME_UNIT];	// международное имя с учётом nPreUnit
	wchar_t nameLoc[SIZE_NAME_UNIT];	// локальное имя с учётом nPreUnit
};
//-----------------------------------------------------------------------------
// Структура параметров выходной величины измерения
struct STRUCT_UNIT_DST
{
	WORD indxGroupUnit;	// индекс группы единиц, в которую входит данная единица
	WORD indxTypeUnit;	// индекс типа единиц, в которую входит данная единица
	short nPreUnit;		// показатель степени кратной/дольной единицы
	WORD flagsCur;		// текущие флаги единицы

	// оператор преобр. единицы в единицу СИ данной группы с учётом nPreUnit
	STRUCT_OPERATOR_CONVERT convertToSi;

//	wchar_t nameInt[SIZE_NAME_UNIT];	// международное имя с учётом nPreUnit
	wchar_t nameLoc[SIZE_NAME_UNIT];	// локальное имя с учётом nPreUnit

	// степень соседней допустимой кратной/дольной единицы при
	short n_up;			// увеличении степени
	short n_doun;		// уменьшении степени
	long rezerved;

	// величины перехода с текущей кратной/дольной единицы на соседнюю,
	double k_up;		// с большим n
	double k_doun;		// с меньшим n
	double k_event;
};
//-----------------------------------------------------------------------------
// Структура параметров преобразования одной величины в другую
struct STRUCT_UNIT_CONVERT
{
	STRUCT_OPERATOR_CONVERT convert;	// оператор преобразования
	STRUCT_UNIT_SRC unitSrc;			// параметры входной величины
	STRUCT_UNIT_DST unitDst;			// параметры выходной величины
};
//-----------------------------------------------------------------------------