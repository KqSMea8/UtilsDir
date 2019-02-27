//-----------------------------------------------------------------------------
#pragma once

#define NUMBER_TYPE_UNIT	23

#include "structPreUnit.h"			// STRUCT_PRE_UNIT
#include "StructUnit.h"				// родительские структуры
#include "TypeTransform.h"			// VALUE_TRANSFORM_UP & VALUE_TRANSFORM_DOUN;
//-----------------------------------------------------------------------------
//		Служебные структуры (плюсовые)
//-----------------------------------------------------------------------------
// Структура параметров оператора преобразования одной величины в другую
struct STRUCT_OPERATOR_CONVERT_SERVICE : public STRUCT_OPERATOR_CONVERT
{
	//double k;	// мультипликативный коэффициет преобразования
	//double a;	// аддитивный коэффициет преобразования

	STRUCT_OPERATOR_CONVERT_SERVICE(double _k = 1., double _a = 0.)
	{
		k = _k;
		a = _a;
	}

	~STRUCT_OPERATOR_CONVERT_SERVICE() {}

	STRUCT_OPERATOR_CONVERT_SERVICE(const STRUCT_OPERATOR_CONVERT_SERVICE & val)
	{
		k = val.k;
		a = val.a;
	}

	STRUCT_OPERATOR_CONVERT_SERVICE & operator = (const STRUCT_OPERATOR_CONVERT_SERVICE & val)
	{
		k = val.k;
		a = val.a;
		return *this;
	}

	STRUCT_OPERATOR_CONVERT_SERVICE & operator = (const double & val)
	{
		k = val;
		a = 0.;
		return *this;
	}

	void SetValues(const double _k, const double _a = 0.)
	{
		k = _k;
		a = _a;
	}

	void SetValueK(const double val)
	{
		k = val;
	}

	void SetValueA(const double val)
	{
		a = val;
	}

	void Reset()
	{
		k = 1.;
		a = 0.;
	}

	STRUCT_OPERATOR_CONVERT_SERVICE Inversion()
	{
		STRUCT_OPERATOR_CONVERT_SERVICE sc(*this);
		if (k != 0.)
			sc.k = 1. / k;
		else
			sc.k = 1.;
		sc.a = -a * sc.k;
		return sc;
	}

	void Mul(const double val)
	{
		k *= val;
	}

	void Mul(const float val)
	{
		k *= val;
	}

	double operator * (const double val)
	{
		return k * val + a;
	}

	float operator * (const float val)
	{
		return (float)(k * (double)val + a);
	}

	STRUCT_OPERATOR_CONVERT_SERVICE operator * (const STRUCT_OPERATOR_CONVERT_SERVICE val)
	{// ВНИМАНИЕ! Данная операция НЕ коммутативна!
		STRUCT_OPERATOR_CONVERT_SERVICE sc(*this);
		sc.k *= val.k;
		sc.a += k * val.a;
		return sc;
	}
};
//-----------------------------------------------------------------------------
// Структура параметров типа единицы измерения
struct STRUCT_TYPE_UNIT_SERVICE //: public STRUCT_TYPE_UNIT
{
	WORD	indxGroup;			// индекс группы единиц
	WORD	reserved;
	DWORD	flagsTypeUint;		// флаги типа
	//	// Оператор преобр. единицы СИ данной группы в основную единицу типа,
	//	//		у которой n_Pre_Unit = 0.
	//	// Пример из группы "Длина", в которой единица СИ - метр.
	//	// Едиица - моская миля, в одной морской миле 1852 м. 
	//	// Тогда convertSiToMain.k = 1852, convertSiToMain.a = 0.
	STRUCT_OPERATOR_CONVERT convertSiToMain;
//	wchar_t nameInt[SIZE_NAME_UNIT];	// международное обозначение единицы
	wchar_t nameLoc[SIZE_NAME_UNIT];	// локальное обозначение единицы
	
	STRUCT_TYPE_UNIT_SERVICE()
	{
		indxGroup = 0;
		flagsTypeUint = FLAG_PRE_UNIT_0;
		((STRUCT_OPERATOR_CONVERT_SERVICE*)&convertSiToMain)->Reset();
	//	ZeroMemory(nameInt, sizeof(nameInt));
		ZeroMemory(nameLoc, sizeof(nameLoc));
	}

	~STRUCT_TYPE_UNIT_SERVICE() {}

	STRUCT_TYPE_UNIT_SERVICE(const STRUCT_TYPE_UNIT_SERVICE & val)
	{
		*this = val;
	}

	STRUCT_TYPE_UNIT_SERVICE & operator = (const STRUCT_TYPE_UNIT_SERVICE & val)
	{
		indxGroup = val.indxGroup;
		flagsTypeUint = val.flagsTypeUint;
		*(STRUCT_OPERATOR_CONVERT_SERVICE*)&convertSiToMain =
			*(STRUCT_OPERATOR_CONVERT_SERVICE*)&val.convertSiToMain;
	//	wcscpy_s(nameInt, val.nameInt);
		wcscpy_s(nameLoc, val.nameLoc);

		return *this;
	}

	bool SetFlagsPreUnitPlus(LPCWCH sFlags)
	{
		bool ret(false);
		if (sFlags[0] == L'1')
		{
			flagsTypeUint |= FLAG_PRE_UNIT_p1;
			ret = true;
		}
		if (sFlags[1] == L'1')
		{
			flagsTypeUint |= FLAG_PRE_UNIT_p2;
			ret = true;
		}
		if (sFlags[2] == L'1')
		{
			flagsTypeUint |= FLAG_PRE_UNIT_p3;
			ret = true;
		}
		if (sFlags[3] == L'1')
		{
			flagsTypeUint |= FLAG_PRE_UNIT_p6;
			ret = true;
		}
		if (sFlags[4] == L'1')
		{
			flagsTypeUint |= FLAG_PRE_UNIT_p9;
			ret = true;
		}
		if (sFlags[5] == L'1')
		{
			flagsTypeUint |= FLAG_PRE_UNIT_p12;
			ret = true;
		}
		return ret;
	}

	bool SetFlagsPreUnitMinus(LPCWCH sFlags)
	{
		bool ret(false);
		if (sFlags[0] == L'1')
		{
			flagsTypeUint |= FLAG_PRE_UNIT_m1;
			ret = true;
		}
		if (sFlags[1] == L'1')
		{
			flagsTypeUint |= FLAG_PRE_UNIT_m2;
			ret = true;
		}
		if (sFlags[2] == L'1')
		{
			flagsTypeUint |= FLAG_PRE_UNIT_m3;
			ret = true;
		}
		if (sFlags[3] == L'1')
		{
			flagsTypeUint |= FLAG_PRE_UNIT_m6;
			ret = true;
		}
		if (sFlags[4] == L'1')
		{
			flagsTypeUint |= FLAG_PRE_UNIT_m9;
			ret = true;
		}
		if (sFlags[5] == L'1')
		{
			flagsTypeUint |= FLAG_PRE_UNIT_m12;
			ret = true;
		}
		return ret;
	}

// 	void SetNameInt(LPCWCH name)
// 	{
// 		wcsncpy_s(nameInt, name, SIZE_NAME_UNIT - 1);
// 	}

	void SetNameLoc(LPCWCH name)
	{
		wcsncpy_s(nameLoc, name, SIZE_NAME_UNIT - 1);
	}
};
//-----------------------------------------------------------------------------
// Структура параметров входной величины измерения
struct STRUCT_UNIT_SRC_SERVICE : public STRUCT_UNIT_SRC
{
// 	WORD indxGroupUnit;	// индекс группы единиц, в которую входит данная единица
// 	WORD indxTypeUnit;	// индекс типа единиц, в которую входит данная единица
// 	short nPreUnit;		// показатель степени кратной/дольной единицы
// 	WORD flagsCur;		// текущие флаги единицы, в этой структуре не используются
// 
// 	// оператор преобр. единицы в единицу СИ данной группы с учётом nPreUnit
// 	STRUCT_OPERATOR_CONVERT convertToSi;
// 
// 	wchar_t nameInt[SIZE_NAME_UNIT];	// международное имя с учётом nPreUnit
// 	wchar_t nameLoc[SIZE_NAME_UNIT];	// локальное имя с учётом nPreUnit

	STRUCT_UNIT_SRC_SERVICE()
	{
		Reset();
	}

	~STRUCT_UNIT_SRC_SERVICE() {}

	STRUCT_UNIT_SRC_SERVICE(const STRUCT_UNIT_SRC_SERVICE & val)
	{
		*this = val;
	}

	STRUCT_UNIT_SRC_SERVICE & operator = (const STRUCT_UNIT_SRC_SERVICE & val)
	{
		indxGroupUnit = val.indxGroupUnit;
		indxTypeUnit = val.indxTypeUnit;
		nPreUnit = val.nPreUnit;
		flagsCur = val.flagsCur;
		*(STRUCT_OPERATOR_CONVERT_SERVICE*)&convertToSi =
			*(STRUCT_OPERATOR_CONVERT_SERVICE*)&val.convertToSi;
	//	wcscpy_s(nameInt, val.nameInt);
		wcscpy_s(nameLoc, val.nameLoc);

		return *this;
	}

	void Reset()
	{
		indxGroupUnit = 0;
		indxTypeUnit = 0;
		nPreUnit = 0;
		flagsCur = 0;
		((STRUCT_OPERATOR_CONVERT_SERVICE*)&convertToSi)->Reset();
	//	ZeroMemory(nameInt, sizeof(nameInt));
		ZeroMemory(nameLoc, sizeof(nameLoc));
	}
};
//-----------------------------------------------------------------------------
// Структура параметров выходной величины измерения
struct STRUCT_UNIT_DST_SERVICE : public STRUCT_UNIT_DST
{
// 	WORD indxGroupUnit;	// индекс группы единиц, в которую входит данная единица
// 	WORD indxTypeUnit;	// индекс типа единиц, в которую входит данная единица
// 	short nPreUnit;		// показатель степени кратной/дольной единицы
// 	WORD flagsCur;		// текущие флаги единицы
// 	// оператор преобр. единицы в единицу СИ данной группы с учётом nPreUnit
// 	STRUCT_OPERATOR_CONVERT convertToSi;
// 	wchar_t nameInt[SIZE_NAME_UNIT];	// международное имя с учётом nPreUnit
// 	// 	wchar_t nameLoc[SIZE_NAME_UNIT];	// локальное имя с учётом nPreUnit
// 	// степень соседней допустимой кратной/дольной единицы при
// 	short n_up;			// увеличении степени
// 	short n_doun;		// уменьшении степени
// 	long rezerved;
// 	// величины перехода с текущей кратной/дольной единицы на соседнюю,
// 	double k_up;		// с большим n
// 	double k_doun;		// с меньшим n
// 	double k_event;

	STRUCT_UNIT_DST_SERVICE()
	{
		Reset();
	}

	~STRUCT_UNIT_DST_SERVICE() {}

	STRUCT_UNIT_DST_SERVICE(const STRUCT_UNIT_DST_SERVICE & val)
	{
		*this = val;
	}

	STRUCT_UNIT_DST_SERVICE & operator = (const STRUCT_UNIT_DST_SERVICE & val)
	{
		indxGroupUnit = val.indxGroupUnit;
		indxTypeUnit = val.indxTypeUnit;
		nPreUnit = val.nPreUnit;
		flagsCur = val.flagsCur;
		*(STRUCT_OPERATOR_CONVERT_SERVICE*)&convertToSi =
			*(STRUCT_OPERATOR_CONVERT_SERVICE*)&val.convertToSi;
	//	wcscpy_s(nameInt, val.nameInt);
		wcscpy_s(nameLoc, val.nameLoc);
		
		k_up = val.k_up;
		k_doun = val.k_doun;
		n_up = val.n_up;
		n_doun = val.n_doun;
		k_event = val.k_event;

		return *this;
	}

	void Reset()
	{
		indxGroupUnit = 0;
		indxTypeUnit = 0;
		nPreUnit = 0;
		flagsCur = 0;
		((STRUCT_OPERATOR_CONVERT_SERVICE*)&convertToSi)->Reset();
	//	ZeroMemory(nameInt, sizeof(nameInt));
		ZeroMemory(nameLoc, sizeof(nameLoc));
		k_up = VALUE_TRANSFORM_UP_3_ORDER;
		k_doun = VALUE_TRANSFORM_DOUN_3_ORDER;
		k_event = 1.;
	}

	void operator = (const STRUCT_UNIT_SRC & val)
	{
		indxGroupUnit = val.indxGroupUnit;
		indxTypeUnit = val.indxTypeUnit;
		nPreUnit = val.nPreUnit;
		flagsCur = 0;	//	в STRUCT_UNIT_SRC флаги не используются
		convertToSi.k = val.convertToSi.k;
		convertToSi.a = val.convertToSi.a;
	//	wcscpy_s(nameInt, val.nameInt);
		wcscpy_s(nameLoc, val.nameLoc);

// 		k_up = 1.;
// 		k_doun = 1.;
// 		n_up = 0;
// 		n_doun = 0;
 		k_event = 1.;
	}
};
//-----------------------------------------------------------------------------
// Структура параметров преобразования одной величины в другую
struct STRUCT_UNIT_CONVERT_SERVICE : public STRUCT_UNIT_CONVERT
{
	//STRUCT_OPERATOR_CONVERT convert;	// оператор преобразования
	//STRUCT_UNIT unitSrc;		// параметры входной величины
	//STRUCT_UNIT unitDst;		// параметры выходной величины

	STRUCT_UNIT_CONVERT_SERVICE()
	{
		Reset();
	}

	~STRUCT_UNIT_CONVERT_SERVICE() {}

	STRUCT_UNIT_CONVERT_SERVICE(const STRUCT_UNIT_CONVERT_SERVICE & val)
	{
		*this = val;
	}

	STRUCT_UNIT_CONVERT_SERVICE & operator = (const STRUCT_UNIT_CONVERT_SERVICE & val)
	{
		*(STRUCT_OPERATOR_CONVERT_SERVICE*)&convert =
			*(STRUCT_OPERATOR_CONVERT_SERVICE*)&val.convert;
		*(STRUCT_UNIT_SRC_SERVICE*)&unitSrc = *(STRUCT_UNIT_SRC_SERVICE*)&val.unitSrc;
		*(STRUCT_UNIT_DST_SERVICE*)&unitDst = *(STRUCT_UNIT_DST_SERVICE*)&val.unitDst;
		return *this;
	}

	void Reset()
	{
		((STRUCT_OPERATOR_CONVERT_SERVICE*)&convert)->Reset();
		((STRUCT_UNIT_SRC_SERVICE*)&unitSrc)->Reset();
		((STRUCT_UNIT_DST_SERVICE*)&unitDst)->Reset();
	}

	void ReStart()
	{
	//	convert - ничего не нало
	//	unitSrc - ничего не нало
	//	unitDst - ничего не нало
	}
};
//-----------------------------------------------------------------------------