//-----------------------------------------------------------------------------
// Файл содержит общие объявления и константы,
// используемые в ПО ZetLab и ZetWiew
//-----------------------------------------------------------------------------
#pragma once
#include "afxstr.h"
#include "WTypes.h"
#include <WinDef.h>
#include <WinError.h>	// константы, используемые в сообщениях WinLog
#include <DefineWM.h>	// Оконные сообщения
//------------------------------------------------------------------------------
// Минимальная щирина для окон с изменяемыми размерами
#ifndef Width_Min
#define Width_Min		320
#endif

// Минимальная высота для окон с изменяемыми размерами
#ifndef Height_Min
#define Height_Min		256
#endif

// Ширина окна с НЕ изменяемыми размерами
#ifndef WidthWin
#define WidthWin 492	// 492 = 512-20
#endif

// Максимальное кол-во отсчетов в графике на GridGl
#ifndef MaxSizeForGridGl
#define MaxSizeForGridGl 500000
#endif

// Максимальное кол-во отсчетов в линии двухмерного графика на Granna
#ifndef MaxSizePointInLineForGramma
#define MaxSizePointInLineForGramma 50000
#endif

//------------------------------------------------------------------------------
// Типы новых параметров. Используется в CDialog_ZET::OnStructureNew.
// Новые типы можно добавлять только в конец.
enum TypeNewParam : BYTE
{
	param_no,			// параметры не заданы
	param_default,		// по умолчанию
	param_cfg_file,		// из cfg-файла
	param_win_param,	// из окна задания параметров
	param_zet_project,	// от ZET-панели при запуске в составе Z-проекта
	param_modify_srv,	// после обработки события сервера
	param_unit_1,		// из UNIT-1
	param_unit_2,		// из UNIT-2
	param_unit_3,		// из UNIT-3
	param_cfg_change,	// из изменённого cfg-файла
	param_cfg_struct,	// из cfg-структуры
};
//------------------------------------------------------------------------------