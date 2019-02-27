#pragma once
#include <ZetColors.h>
#include <Grafic1\gridglctrl1.h>
//*************************************************************************************************
//#define MaxNumber 80
#ifdef MaxNumber
#error Error: "MaxNumber" defined in some other file.
#endif
//*************************************************************************************************
namespace GridGL
{
	//---------------------------------------------------------------------------------------------
	const static int g_Axis_Count(4);				// количество осей для графика
	const static int g_Max_Count_AdditionWnd(99);	// максимальная количество дополнительных окон на графике
	const static int g_Max_Text_Length(200);		// максимальная длина надписи на шкале
	//---------------------------------------------------------------------------------------------
	enum AxisSide			// перечисление сторон для шкалы
	{
		AxisDefault = 0,					// главная левая шкала
		AxisSideRight_Right,				// самая правая шкала
		AxisSideRight_Left,					// шкала справа слева  от оси
		AxisSideLeft_Right,					// шкала слева  справа от оси
	};
};
//*************************************************************************************************
struct AxisInfo				// структура с параметрами вертикальной шкалы
{
	GridGL::AxisSide Side;					// сторона расположения шкалы 
	bool bUseAxis;							// true - отображать вторую ось | false - не отображать вторую ось
	float Multiplicative;					// мильтипликативный коэффициент различия данных с основной шкалой
	float Additive;							// аддитивный        коэффициент различия данных с основной шкалой
	long CurrentMaxCX;						// текущая максимальная ширина текста для шкалы
	long CurrentTextHeight;					// текущая высота текста для шкалы
	COLORREF Color;							// цвет шкалы
	WCHAR Text[GridGL::g_Max_Text_Length];	// текст на шкале
	AxisInfo()
	{
		SetValueDefault();
	}
	void SetValueDefault()
	{
		Side			= GridGL::AxisSide::AxisDefault;
		bUseAxis		= false;
		Multiplicative	= 1.f;
		Additive		= 0.f;
		CurrentMaxCX	= 0;
		CurrentTextHeight = 0;
		Color			= ZETColors::DigitsColor;
		ZeroMemory(Text, sizeof(Text));
	}
};
//*************************************************************************************************
struct AdditionalWindow		// структура с параметрами дополнительных окон
{
	bool bVisible;								// true - отображать | false - не отображать
	long GraphIndex;							// индекс графика к которому привязано окно
	long PointIndex;							// индекс точки на графике
	CPoint WindowPoint;							// расположение центра окна
	COLORREF TextColor;							// Цвет текста
	COLORREF FonColor;							// Цвет фона
	WCHAR Text[GridGL::g_Max_Text_Length * 2];	// Текст в окне
	AdditionalWindow()
	{
		SetValueDefault();
	}
	void SetValueDefault()
	{
		bVisible	= false;
		GraphIndex	= 0;
		PointIndex	= 0;
		WindowPoint.SetPoint(0, 0);
		TextColor	= ZETColors::TextColor;
		FonColor	= ZETColors::AddtionalWindowFon;
		ZeroMemory(Text, sizeof(Text));
	}
};
//*************************************************************************************************
struct grid 
{
	//поскольку #define не имеет границ видимости и может быть переопределен,
	//пользуемся константой внутри grid
	static const int MaxNumber = 240;
	static const int MaxTextLengthInByte = GridGL::g_Max_Text_Length;
	 long sizeofgrid;				//размер grid-ы в байтах
	 int left_x,					//верхний левый
		 right_x,					//отступ справа
		 top_y,						//угол графика
		 bottom_y,					//расстояние от нижнего края окна до нижнего края графика
		 width_x,					//размер графика по оси X
		 height_y,					//размер графика по оси Y
		 number_of_strings_in_hat,	//количество строк в шапке
		 cursor_x,					//позиция курсора по оси x в экранных координатах 
		 step,						//шаг графика, расстояние между соседними точками в экранных коор-тах
		 mouse_x,					//позиция мыши в экранных коор - тах
		 mouse_y,					//позиция мыши в экранных коор - тах
		 timer,						//признак включения таймера
		 update,    				//признак обновления графика
		 spectr,					//признак того, что входные данные - спектр
		 osc;						//признак того, что входные данные - временная реализация
	 long font_size,				//размер шрифта
		  make_upor,				//признак упоров по оси Y, 0- нет упоров, 1- есть
		  number,					//кол-во графиков
		  ind,						//номер графика
		  num_visible_points,		//число видимых точек графика
		  position_of_cursor,		//номер элемента массива bf, на котором стоит курсор, параметр принимает значение от 0 до Size
		  size,						//размер буфера
		  type_grf[MaxNumber],      //тип графика 0-положительный, 1-знако переменный
		  type_line,			    //0 - гор. линиями, 1- ломанной линией,
		  type_fill[MaxNumber],		//0 - с заполнением, 1 - без заполнения					
		  valid[MaxNumber],			//0 - не рисует выбранный график, 
									//1 - рисует
		  type_abs,					//тип осей 0- норма, 
									//1- лог по Х,4-нет осей по Y, 
									//2 - нет осей по , 
									//8 - нет надписей по Y, 
									//16 - нет надписей по Х
		  type_x_axis,				//тип шкалы по оси X, 0 - линейная, 
									//1 - логарифмическая
									//2 - экспоненциальная
		  type_y_axis,				//тип шкалы по оси X, 
									//0 - линейная, 
									//1 - логарифмическая
									//2 - децибельная
		  type_y_data,              //0 - данные заданы в линейно,
									//1 - данные заданы децибельно
		  log,						//1 - в альтернативных коодинатах линейная шкала по Х
									//0 - 1/n - октавная
		  alt_lin,
		  line_width[MaxNumber],	//толщина линии
		  graph_axis[MaxNumber];	//индекс шкалы, к которой привязан график | 0 - главная шкала слева / 1 - дополнительная шкала справа
	double math_width_x,			//математ. ширина сетки по Х
		   math_width_y,			//математ. ширина сетки по Y
		   math_min_x,				//текущее минимальное значение по X
		   math_min_y,				//текущее минимальное значение по Y
		   upor_delta,				//минимальное значение ширины сетки по Y
		   upor_down,				//нижний предел по Y
		   upor_up,					//верхний предел по Y
		   x_first,					//нижний предел по Х
		   x_end,					//верхний предел по Х
		   reference;               //референс
	WCHAR
		 frm1[MaxTextLengthInByte],					//формат надпись координаты х
		 frm2[MaxNumber][MaxTextLengthInByte],		//формат надпись координаты у
		 graph_text[MaxNumber][MaxTextLengthInByte],//текст у графика
		 caption[MaxTextLengthInByte],				//
		 text_x_axis[MaxTextLengthInByte],			//подпись оси X
		 font_name[MaxTextLengthInByte];			//название шрифта

	COLORREF graph_color[MaxNumber];	//цвет графика, для каждого графика задается отдельно	
	COLORREF cursor_color;				//цвет курсора
	COLORREF digits_color;				//цвет цифр
	COLORREF fon_color;					//цвет фона
	COLORREF grid_color;				//цвет сетки
	COLORREF legend_color;				//цвет легенды
	long show_scale_prop_page;          //FALSE - скрывать закладку листка свойств "шкала" 
										//TRUE  - показывать
	long alt_coords;                    //TRUE - альтернативная шкала по оси X  
	long power_coeff;					//	TRUE - в децибеллах показана мощность, FALSE - в децибеллах показана амплитуда
	AxisInfo AxisInfoStruct[GridGL::g_Axis_Count];	// масссив параметров для шкал
	AdditionalWindow AdditionalWindows[GridGL::g_Max_Count_AdditionWnd];	// масссив параметров для дополнительных окон
	grid()
	{
		SetValueDefault();
	}
	AxisInfo * GetAxisBySide(_In_ const GridGL::AxisSide &SideID)
	{
		if (SideID < 0 ||
			SideID > GridGL::g_Axis_Count)
			return nullptr;

		for (auto &AxisData : AxisInfoStruct)
		{
			if (AxisData.Side == SideID)
				return &AxisData;
		}

		return nullptr;
	}
	void SetValueDefault()
	{
		sizeofgrid		= sizeof(grid);
		left_x			= 0;
		right_x			= 0;
		top_y			= 0;
		bottom_y		= 0;
		width_x			= 0;
		height_y		= 0;
		number_of_strings_in_hat = 0;
		cursor_x		= 0;
		step			= 0;
		mouse_x			= 0;
		mouse_y			= 0;
		timer			= 0;
		update			= 0;
		spectr			= 0;
		osc				= 0;
		font_size		= 12;
		make_upor		= 0;
		number			= 1;
		ind				= 0;
		num_visible_points = -1;
		position_of_cursor = 0;
		size			= 100;
		type_line		= 1;
		type_abs		= 0;
		type_x_axis		= 0;
		type_y_axis		= 0;
		type_y_data		= 0;
		log				= 0;
		alt_lin			= 0;
		math_width_x	= 100.;
		math_width_y	= 2.;
		math_min_x		= 1.;
		math_min_y		= 0.;
		upor_delta		= 0.000001;
		upor_down		= -100000000.;
		upor_up			=  100000000.;
		x_first			= 0.;
		x_end			= 100.;
		reference		= 0.000001;
		show_scale_prop_page = 1;
		alt_coords		= 0;
		power_coeff		= 0;
		cursor_color	= ZETColors::CursorColor;
		digits_color	= ZETColors::DigitsColor;
		fon_color		= ZETColors::BackgroundColor;
		grid_color		= ZETColors::GridColor;
		legend_color	= ZETColors::LegendColor;

		ZeroMemory(frm1,		sizeof(frm1));
		ZeroMemory(frm2,		sizeof(frm2));
		ZeroMemory(caption,		sizeof(caption));
		ZeroMemory(text_x_axis, sizeof(text_x_axis));
		ZeroMemory(font_name,	sizeof(font_name));

		for (int i = 0; i < MaxNumber; ++i)
		{
			valid[i] = 1;
			type_fill[i] = 0;
			type_grf[i] = 0;
			line_width[i] = 1;
			graph_axis[i] = GridGL::AxisSide::AxisDefault;
			graph_color[i] = ZETColors::Graph_Colors[i % GRAPH_COLORS_COUNT];
			swprintf_s(graph_text[i], _T("Сигнал %d"), i);
		}

		for (int i = 0; i < GridGL::g_Axis_Count; i++)
		{
			AxisInfoStruct[i].SetValueDefault();
			AxisInfoStruct[i].Side = GridGL::AxisSide(i);

			if (AxisInfoStruct[i].Side == GridGL::AxisSide::AxisDefault)
				AxisInfoStruct[i].bUseAxis = true;
			else
				AxisInfoStruct[i].bUseAxis = false;
		}

		for (auto & it : AdditionalWindows)
			it.SetValueDefault();
	}
	// копирование значений из графика в структуру
	void GetParamFromGrid(CGridglctrl1 * pGridGL)
	{
		if (pGridGL)
			pGridGL->InWindPtr((signed char*)this);
	}
	// передача графику значений из структуры
	void SetParamIntoGrid(CGridglctrl1 * pGridGL)
	{
		if (pGridGL)
			pGridGL->OutWindPtr((signed char*)this);
	}
};
//*************************************************************************************************