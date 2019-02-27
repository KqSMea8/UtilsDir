﻿//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
#define NUMBER_GROUP_UNIT			11
//-----------------------------------------------------------------------------
// Структура параметров групп единиц измерения
struct STRUCT_GROUP_UNIT
{
	DWORD numberUnitInGroup;// кол-во единиц в группе
	WORD indxUnitSi;		// индекс единицы СИ в массиве групп единиц
	WORD indxUnitDefault;	// индекс default-единицы в массиве групп единиц

	STRUCT_GROUP_UNIT()
		: numberUnitInGroup(0)
		, indxUnitSi(0)
		, indxUnitDefault(0)
	{}
};
//-----------------------------------------------------------------------------
// Копия текстовых ресурсов, содержащих имена групп единиц
//STRINGTABLE
//BEGIN
//IDS_GROUP_UNIT_0			"Длина"
//IDS_GROUP_UNIT_1			"Масса"
//IDS_GROUP_UNIT_2			"Время"
//IDS_GROUP_UNIT_3			"Температура"
//IDS_GROUP_UNIT_4			"Скорость"
//IDS_GROUP_UNIT_5			"Ускорение"
//IDS_GROUP_UNIT_6			"Частота"
//IDS_GROUP_UNIT_7			"Давление"
//IDS_GROUP_UNIT_8			"Электрический ток"
//IDS_GROUP_UNIT_9			"Электрическое напряжение"
//IDS_GROUP_UNIT_10			"Электрическое сопротивление"
//END
//-----------------------------------------------------------------------------