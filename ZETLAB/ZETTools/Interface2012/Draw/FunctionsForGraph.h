//------------------------------------------------------------------------------
//	FunctionForGraph.h - файл реализации
//	Функции для использования в классе графика
//------------------------------------------------------------------------------
#pragma once

#include <limits>
#include <ipps.h>

//static double NAN = std::numeric_limits<double>::quiet_NaN();

#define MAX_QNT_MARKS 20 //максимальное количество меток
#define MIN_QNT_MARKS 5 //минимальное количество меток
#define MAX_PX_STEP 90 //максимальный шаг в пикселях
#define CONST1 0.3 //дробная часть десятичного логарифма 2*10^x
#define CONST2 0.7 //дробная часть десятичного логарифма 5*10^x
#define MAX_QNT_OF_POINTS 250000

enum timeID
{
    Year,
    Month,
    Day,
    Hour,
    Minute,
    Second,
    Millisecond
};

struct TimeInterval
{
    int tiData;
    timeID tiID;
};

//тип оси: вертикальная или горизонтальная
enum GridType{
    gtHorizontal,
    gtVertical,
};

//тип шкалы: логарифическая или линейная
enum ScaleType{
    stLog,
    stLinear,
    stNone
};

//тип линий: сплошная прерывистая
enum LineType{
    ltFirm,
    ltDot
};

//области, где ловится мышь для масштабирования графика и отображения перекрестья
enum zRect{
    gLeftShift,
    gRightShift,
    gTopShift,
    gBottomShift,
    gGridArea,
    gLeftScale,
    gRightScale,
    gTopScale,
    gBottomScale,
    gCenterAutoScale,
    gNone
};

enum FitType{
    ftLeft,
    ftRight,
    ftCenter
};

enum Settings{
    sAuto,
    sManual
};

enum PlotType{
    ptSpectr, // спектр (линейная сетка, график столбчатый)
	ptSpectrLog, //спектр (логарифмическая сетка, график столбчатый)
    ptOscill, // осциллограмма (линейная сетка, график - линия)
	ptOscillLog, // осциллограмма (логарифмическая сетка, график - линия)
    ptTrend, //гтренды (по оси Х сетка в формате дат, график - линия)
	ptParam, //обратная зависимость (Х от Y, сетка линейная, график - линия)
    ptOld
};

struct TwoDouble{
    double Min;
    double Max;
	TwoDouble() 
		: Min(0)
		, Max(0)
	{
	}
};

struct TwoDate
{
    SYSTEMTIME Min;
    SYSTEMTIME Max;
	TwoDate() 
	{
		Min = {0};
		Max = {0};
	}
};

struct CursorValues
{
    CString X;
    CString Y;
    double pxX;
    double pxY;
	CursorValues()
		: X(L"")
		, Y(L"")
		, pxX(0.0)
		, pxY(0.0)
	{
	}
};