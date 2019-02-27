#pragma once

#include <windef.h>
#include <minwinbase.h>
#include <vector>
#include <ZetColors.h>
#include <ZetTools/include/ZetTools.h>

static const double DEFAULT_UPOR_DELTA = 1.e-9/*0.0001*/;

enum DisplayMode
{
	Events = 0,			// отображение точек без учета значения по Y (событийный режим)
	Default,			// отображение всех точек в одних осях
	SplittingFields,	// отображение каждой точки в отдельной области
	SeismicGrath,		// отображение каждого графика в отдельной области
	ImpositionGraths	// отображение всех графиков с наложением друг на друга
};
//*************************************************************************************************
// базовая структура для временных графиков
struct TrendGrid
{
	long sizeofgrid;						// размер  grid-ы в байтах
	int left_x,								// верхний левый
		right_x,							// дополнительное расстояние справа
		top_y,								// угол графика
		bottom_y,							// расстояние от нижнего края окна до нижнего края графика
		width_x,							// размер  графика по оси X
		height_y,							// размер  графика по оси Y
		ColorLine_Height,					// высота под CLocolLine
		mouse_x,							// позиция мыши в экранных коор - тах
		mouse_y,							// позиция мыши в экранных коор - тах
		timer,								// признак включения таймера
		update;    							// признак обновления графика
	long font_size,							// размер  шрифта
		make_upor,							// признак упоров по оси Y, 0- нет упоров, 1- есть		
		type_abs,							/*	0 -  маркировка и сетка по х и y
												1 -  все кроме горизонт сетки
												4 -  все кроме верт сетки
												5 -  только разметка по х и у
												12 - вертикальная разметка и сетка
												13 - только гор разметка
												17 - верт сетка и разметка
												21 - только верт разметка
												29 - все откл*/
		Element_width;						// ширина элементов
	double math_width_x,					// математ. ширина сетки по Х
		math_width_y,						// математ. ширина сетки по Y
		math_min_x,							// текущее минимальное значение по X
		math_min_y,							// текущее минимальное значение по Y
		upor_delta,							// минимальное значение ширины сетки по Y
		upor_down,							// нижний  предел по Y
		upor_up,							// верхний предел по Y
		x_first,							// нижний  предел по Х
		x_end;								// верхний предел по Х
	wchar_t font_name[MAX_PATH];			// название шрифта
	COLORREF cursor_color;					// цвет курсора
	COLORREF digits_color;					// цвет цифр
	COLORREF fon_color;						// цвет фона
	COLORREF grid_color;		 			// цвет сетки
	bool vert_CursValid;					// отображение вертикальных	  курсоров 
											// false - не отображать // true - отображать
	bool gor_CursValid;						// отображение горизонтальных курсоров 
											// false - не отображать // true - отображать
	DisplayMode Display_Mode;				// режим работы программы
	bool bVerticalGraphs;					// флаг что графики будут отображаться вертикально
	TrendGrid()
		: sizeofgrid		(sizeof(TrendGrid))
		, left_x			(16)
		, right_x			(15)
		, top_y				(16)
		, bottom_y			(12 + 4)/*font_size + 4*/
		, width_x			(100)
		, height_y			(100)
		, ColorLine_Height	(0)
		, mouse_x			(0)
		, mouse_y			(0)
		, timer				(0)
		, update			(0x4)/*UPDATE_DRAW_ALL*/
		, font_size			(12)
		, make_upor			(0)
		, type_abs			(0)
		, Element_width		(7)
		, math_width_x		(0)
		, math_width_y		(101)
		, math_min_x		(0)
		, math_min_y		(-1)
		, upor_delta		(DEFAULT_UPOR_DELTA)
		, upor_down			(-1.e10)
		, upor_up			(1.e10)
		, x_first			(ZetTimeMin)
		, x_end				(ZetTimeMax)
		, vert_CursValid	(true)
		, gor_CursValid		(true)
		, Display_Mode		(Events)
		, bVerticalGraphs	(false)
	{
		CZetTime zt1;
		CZetTime zt2;
		zt1.ConvertFromString(L"01.01.2015 00:00:00");
		zt2.SetCurrentTime();
		math_min_x = zt1;
		math_width_x = zt2 - zt1;
		
		ZeroMemory(&font_name, sizeof(font_name));
		fon_color	 = ZETColors::BackgroundColor;
		grid_color	 = ZETColors::GridColor;
		cursor_color = ZETColors::CursorColor;
		digits_color = ZETColors::DigitsColor;
	}
	TrendGrid & operator = (const TrendGrid & val)
	{
		auto size = sizeof(TrendGrid);
		memcpy_s(this, size, &val, size);
		return *this;
	}
	bool operator == (const TrendGrid & val)
	{
		return memcmp(this, &val, sizeof(TrendGrid)) == 0;
	}
	bool operator != (const TrendGrid & val)
	{
		return !(*this == val);
	}
};
//*************************************************************************************************
// структура с параметрами графика для ZETTrendValueGrid
struct ZetTrendValueGridStuct : public TrendGrid
{
	size_t SizeOfDataGridStruct;	// размер структуры с параметрами
	bool bFillGraph;				// флаг того что необходимо заполнять большую часть графика цветом
	COLORREF FillColor;				// цвет заполнения большей части графика
	int ActivePropSheetIndex;		// номер активной страницы с параметрами
	long CountMaxVisiblePoints;		// ограничение максимального отображаемого количество точек
	bool bLimitMaxVisiblePoints;	// флаг того что необходимо ограничивать максимальное количство видимых точек
	//*********************************************************************************************
	ZetTrendValueGridStuct()
		: TrendGrid()
		, SizeOfDataGridStruct(sizeof(ZetTrendValueGridStuct))
		, bFillGraph			(false)
		, FillColor				(RGB(0, 0, 0))
		, ActivePropSheetIndex	(0)
		, CountMaxVisiblePoints	(400000)
		, bLimitMaxVisiblePoints(false)
	{
		Display_Mode = DisplayMode::Default;
		Element_width = 2;
	}

	ZetTrendValueGridStuct(const TrendGrid Parrent)
		: TrendGrid(Parrent)
		, SizeOfDataGridStruct	(sizeof(ZetTrendValueGridStuct))
		, bFillGraph			(false)
		, FillColor				(RGB(0, 0, 0))
		, ActivePropSheetIndex	(0)
		, CountMaxVisiblePoints	(400000)
		, bLimitMaxVisiblePoints(false)
	{}
	//*********************************************************************************************
	ZetTrendValueGridStuct operator = (const ZetTrendValueGridStuct &val)
	{
		auto size = sizeof(ZetTrendValueGridStuct);
		memcpy_s(this, size, &val, size);
		return *this;
	}

	ZetTrendValueGridStuct operator = (const TrendGrid &val)
	{
		*(TrendGrid*)this = *(TrendGrid*)&val;
		return *this;
	}

	bool operator == (const ZetTrendValueGridStuct & val)
	{
		return memcmp(this, &val, sizeof(ZetTrendValueGridStuct)) == 0;
	}

	bool operator != (const ZetTrendValueGridStuct & val)
	{
		return !(*this == val);
	}
};	//*********************************************************************************************