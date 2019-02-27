//------------------------------------------------------------------------------
// Глобальные функции, вызываемые из проектов Spectr.exe и ZetSpectr.ocx
//------------------------------------------------------------------------------
#pragma once
#include <mathZet.h>
#include <ZetColors.h>
#include <Algorithms\CalcFFT\CustomCalcFFT.h>
#include <ZETView_basic_class\CDSRV.h>
#include <CustomSaveStruct.h>
#include <Grafic1\gridglctrl1.h>
#include <Grafic1\grammactrl1.h>
#include <Grafic1\grammaglctrl1.h>
#include <Struct_grid.h>
//------------------------------------------------------------------------------
#define colorError			ZETColors::ControlColorError
#define colorWarning		ZETColors::ControlColorWarning
#define colorInform			ZETColors::ControlColorOk
//------------------------------------------------------------------------------
enum TypeIndexLineMain : BYTE
{
	par_line_main_cur,
	par_line_main_aver,
	par_line_main_max,
	par_line_main_norm,

	par_line_main_size
};
//------------------------------------------------------------------------------
// Индексы массива Placement[] для параметров доп. окон
// Параметры главного окна живут в CustomSaveStructure
//
enum TypeIndexPlacementAddWin : BYTE
{
	place_win_gramma_2D,
	place_win_gramma_2D_f,
	place_win_gramma_2D_t,
	place_win_gramma_3D,
	place_win_prohod,
	place_win_kepstr,

	place_win_size
};
//-----------------------------------------------------------------------------
// Тип времени усреднения отображаемого результата (для combo в окне параметров)
//
enum TypeTimeAver : BYTE
{
	tta_0_01,
	tta_0_02,
	tta_0_04,
	tta_0_05,
	tta_0_1,
	tta_0_2,
	tta_0_4,
	tta_0_5,
	tta_1,
	tta_2,
	tta_4,
	tta_5,
	tta_10,
	tta_20,
	tta_40,
	tta_50,
	tta_100,
	tta_200,

	tta_size
};
bool Convert_TypeTimeAver_TimeInSec(const TypeTimeAver type, float & timeInSec);
//-----------------------------------------------------------------------------
// Типы частотного разрешения для FFT  (для combo в окне параметров)
// Порядок FFT = tfrFFT + 7
//
enum TypeFreqResolution_FFT : BYTE
{
	tfrFFT_07,			// size = 128 = 2^7
	tfrFFT_08,
	tfrFFT_09,
	tfrFFT_10,
	tfrFFT_11,
	tfrFFT_12,
	tfrFFT_13,
	tfrFFT_14,
	tfrFFT_15,
	tfrFFT_16,

	tfrFFT_size
};
bool Convert_TypeFreqResolutionFFT_OrderFFT(const TypeFreqResolution_FFT type, int & order);
//-----------------------------------------------------------------------------
// Типы частотного разрешения для DFT (для combo в окне параметров)
// dF = Fadc / K, где k - это число после tfrDFT_
//
enum TypeFreqResolution_DFT : BYTE
{
	tfrDFT_100,
	tfrDFT_125,
	tfrDFT_200,
	tfrDFT_250,
	tfrDFT_400,
	tfrDFT_500,
	tfrDFT_800,
	tfrDFT_1000,
	tfrDFT_1250,
	tfrDFT_2000,
	tfrDFT_2500,
	tfrDFT_4000,
	tfrDFT_5000,
	tfrDFT_8000,
	tfrDFT_10000,
	tfrDFT_12500,
	tfrDFT_20000,
	tfrDFT_25000,
	tfrDFT_40000,
	tfrDFT_50000,
	tfrDFT_80000,
	tfrDFT_100000,

	tfrDFT_size

};
bool Convert_TypeFreqResolutionDFT_K(const TypeFreqResolution_DFT type, int & k);
bool Convert_TypeFreqResolutionDFT_dF(const TypeFreqResolution_DFT type, const float freqADC, float & dF);
//-----------------------------------------------------------------------------
// Типы усреднения: линейное, экспоненциальное
//
enum TypeAver : BYTE
{
	ta_line,
	ta_exp,

	ta_size
};
//-----------------------------------------------------------------------------
// Типы предствления данных:
// СКЗ, пировое значение, спектр. плотность, спектр. мощность
//
enum TypeViewValue : BYTE
{
	tvv_skz,
	tvv_ampl,
	tvv_PSD,
	tvv_PSD2,

	tvv_size
};
//-----------------------------------------------------------------------------
enum TypeTimeWindow : BYTE
{
	ttw_no,					// без окна (прямоугольное)
	ttw_Hann,				// окно Ханна
	ttw_Hanning,			// окно Хэннинга
	ttw_Hamming,			// окно Хэмминга
	ttw_Blackman,			// стандартное окно Блекмэна
	ttw_Bartlett,			// окно Бартлета
	ttw_Kaiser,				// стандартное окно Кайзера
	ttw_RifeVincent_4,		// окно Рифа-Винсента (4)
	ttw_BlackmanHarris_3,	// окно Блэкмана-Харриса (3)
	ttw_BlackmanHarris_4,	// окно Блэкмана-Харриса (4)
	ttw_Nuttall,			// окно Наталла
	ttw_BlackmanNuttall,	// окно Блэкмана-Наталла
	ttw_Flattop,			// окно с плоской вершиной

	ttw_size
};

bool Convert_TypeTimeWindows_TypeWindow(const TypeTimeWindow ttw, TypeWindow & tw);
//-----------------------------------------------------------------------------
// Типы обработки сигнала после расчета спектра:
// дифференцирование, интегрирование
//
enum TypeSignalProcessing : BYTE
{
	tsp_diff_2,
	tsp_diff_1,
	tsp_no,
	tsp_int_1,
	tsp_int_2,

	tsp_size
};
//-----------------------------------------------------------------------------
// Тип выполняемого преобразования: FFT, DFT
//
enum TypeTransform : BYTE
{
	tt_FFT,
	tt_DFT,

	tt_size
};
//-----------------------------------------------------------------------------
enum TypeSynchroAddGraf : BYTE
{
	ts_val_up,
	ts_val_down,
	ts_ampl_up,
	ts_ampl_down,

	ts_size
};
//-----------------------------------------------------------------------------
// #define SIZE_GRID_LEGEND	200
#define SIZE_GRID_FONT_NAME		32
//-----------------------------------------------------------------------------
// Параметры одной линии графика GridGL,
// записываемые в CFG-файл программы, поэтому здесь не должно быть
// умных указателей (CString, vector, SmartArray, ...).
// В фигурных скобках - значение по умолчанию
//
struct Struct_ParamLineGridGL
{// В фигурных скобках - значение по умолчанию
	long m_valid;			// отображение линии {1}
	long m_type_fill;		// 0 - без заполнения графика под линией, 1 - с заполнением {0}
	long m_width;			// толщина линии {2}
	COLORREF m_color;		// цвет линии {ZETColors::Graph_Colors[0]}

	Struct_ParamLineGridGL();
	void SetValueDefault();
	// Копирование параметров из GridGL в структуру (indexLine от 0)
	void GetParamFromGrid(CGridglctrl1 * pGridGL, const long indexLine);
	// Установка параметров из структуры в GridGL (indexLine от 0)
	void SetParamIntoGrid(CGridglctrl1 * pGridGL, const long indexLine);
};
//-----------------------------------------------------------------------------
// Параметры графика GridGL,
// записываемые в CFG-файл программы, поэтому здесь не должно быть
// умных указателей (CString, vector, SmartArray, ...)
// В фигурных скобках - значение по умолчанию.
// Без параметров линий!!!
//
struct Struct_ParamGridGL
{
	bool m_bDefault;		// значения заданы по умолчанию {true}
	bool m_bBoundaries_X;	// заданы границы отображения по Х {false}
	bool m_bBoundaries_Y;	// заданы границы отображения по Y {false}
	bool b3;
	bool b4;
// 	bool b5;
// 	bool b6;
// 	bool b7;
// 	bool b8;

	long m_numLines;		// количество графиков в GridGL {1}
	grid m_param;

	//----------------------------------------------
	Struct_ParamGridGL();
	void SetValueDefault();

	// Копирование параметров из GridGL в структуру
	void GetParamFromGrid(CGridglctrl1 * pGrid);
	// Установка параметров из структуры в GridGL
	void SetParamIntoGrid(CGridglctrl1 * pGrid);
};
//-----------------------------------------------------------------------------
// Параметры графика Gramma,
// записываемые в CFG-файл программы, поэтому здесь не должно быть
// умных указателей (CString, vector, SmartArray, ...)
// В фигурных скобках - значение по умолчанию.
//
struct Struct_ParamGramma
{
	bool m_bDefault;		// значения заданы по умолчанию {true}
	bool m_bBoundaries_X;	// заданы границы отображения по Х {false}
	bool m_bBoundaries_Y;	// заданы границы отображения по Y {false}
	bool m_bBoundaries_Z;	// заданы границы отображения по Z {false}
// 	bool b5;
// 	bool b6;
// 	bool b7;
// 	bool b8;

	long m_black;			// 0 - цветное изображение, 1 - чёрно-белое изображение {0}
	long m_Px;				// координата курсора {0}
	long m_Py;				// координата курсора {0}
	long m_type_Abs;		// type_Abs - тип осей {0}:
	long m_log;				// отображение в дБ {0}

	COLORREF m_color_Crs;	// цвет курсора {ZETColors::CursorColor}
	COLORREF m_color_Dig;	// цвет цифр на сетке {ZETColors::DigitsColor}
	COLORREF m_color_Fon;	// цвет фона {ZETColors::BackgroundColor}
	COLORREF m_color_Grd;	// цвет сетки {ZETColors::GridColor}
	COLORREF m_color_Leg;	// цвет легенды {ZETColors::LegendColor}

	long m_make_Upor;		// использование границ полного отображения по Y {0}

	double m_upor_Nis;		// нижняя граница полного отображения по Y {0}
	double m_upor_Verh;		// верхняя граница полного отображения по Y {0}
	double m_math_ly;		// нижняя граница текущего отображения по Y {0}
	double m_math_dy;		// длина текущего отображения по Y {0}

	double m_Xfirst;		// левая граница полного отображения по Х {0}
	double m_Xend;			// правая граница полного отображения по Х {0}
	double m_math_lx;		// левая граница текущего отображения по Х {0}
	double m_math_dx;		// длина текущего отображения по Х {0}

	double m_deltaT;		// шаг по времени
// 	//----------------------------------------------
	Struct_ParamGramma();
	void SetValueDefault();
	
	// Копирование параметров из Gramma в структуру
	void GetParamFromGramma(CGrammactrl1 * pGramma);
	// Установка параметров из структуры в Gramma
	void SetParamIntoGramma(CGrammactrl1 * pGramma);
};
//-----------------------------------------------------------------------------
// Параметры графика GrammaGL,
// записываемые в CFG-файл программы, поэтому здесь не должно быть
// умных указателей (CString, vector, SmartArray, ...)
// В фигурных скобках - значение по умолчанию.
//
struct Struct_ParamGrammaGL
{
	bool m_bDefault;		// значения заданы по умолчанию {true}
	bool m_bBoundaries_X;	// заданы границы отображения по Х {false}
	bool m_bBoundaries_Y;	// заданы границы отображения по Y {false}
	bool m_bBoundaries_Z;	// заданы границы отображения по Z {false}
	bool b5;
	bool b6;
	bool b7;
	bool b8;

	long m_black;			// 0 - цветное изображение, 1 - чёрно-белое изображение {0}
	long m_Px;				// координата курсора {0}
	long m_Py;				// координата курсора {0}
	long m_altCoords;
	long m_type_Abs;		// type_Abs - тип осей {0}:
	BOOL m_ordinateData;

	float m_colorMin;
	float m_colorWidth;

	COLORREF m_color_Dig;	// цвет цифр на сетке {ZETColors::DigitsColor}
	COLORREF m_color_Fon;	// цвет фона {ZETColors::BackgroundColor}
	COLORREF m_color_Grd;	// цвет сетки {ZETColors::GridColor}
	COLORREF m_color_Leg;	// цвет легенды {ZETColors::LegendColor}

	long m_typeYAxis;
	long m_typeXAxis;
	long m_typeYData;
	long m_make_Upor;		// использование границ полного отображения по Y {0}

	double m_upor_Nis;		// нижняя граница полного отображения по Y {0}
	double m_upor_Verh;		// верхняя граница полного отображения по Y {0}
	double m_math_ly;		// нижняя граница текущего отображения по Y {0}
	double m_math_dy;		// длина текущего отображения по Y {0}

	double m_Xfirst;		// левая граница полного отображения по Х {0}
	double m_Xend;			// правая граница полного отображения по Х {0}
	double m_math_lx;		// левая граница текущего отображения по Х {0}
	double m_math_dx;		// длина текущего отображения по Х {0}

	double m_deltaT;		// шаг по времени
	double m_reference;
	// 	//----------------------------------------------
	Struct_ParamGrammaGL();
	void SetValueDefault();

	// Копирование параметров из GrammaGL в структуру
	void GetParamFromGramma(CGrammaglctrl1 * pGrammaGL);
	// Установка параметров из структуры в GrammaGL
	void SetParamIntoGramma(CGrammaglctrl1 * pGrammaGL);
};
//-----------------------------------------------------------------------------
struct Struct_Param_Synchro_AddGraf
{
	bool m_bGraf_Aver;				// отображение графика среднего спектра
	bool m_bGraf_Max;				// отображение графика максимального спектра
	bool m_bGraf_Norma;				// отображение графика нормы
	bool m_bSynchro;
	TypeSynchroAddGraf m_bTypeSynchro;
	bool b6;
	bool b7;
	bool b8;

	float m_fInterval_minute;		// интервал расчёта доп. графиков
	float m_fValue;
	GUID m_guidChan;

	wchar_t m_sFileNameNorma[MAX_PATH];	// полное имя файла нормы

	//-------------------------------------------------
	Struct_Param_Synchro_AddGraf()
	{
		SetValueDefault();
	}
	void SetValueDefault()
	{
		m_bGraf_Aver = false;
		m_bGraf_Max = false;
		m_bGraf_Norma = false;
		m_bSynchro = false;
		m_bTypeSynchro = ts_val_up;
		m_fInterval_minute = 10.f;
		m_fValue = 1.f;
		m_guidChan = GUID_NULL;
		ZeroMemory(m_sFileNameNorma, sizeof(m_sFileNameNorma));
	}
};
//-----------------------------------------------------------------------------
/* Параметры контейнеров (без каналов). Составная часть структур:
- записываемых в CFG-файле программы;
- передаваемых в окно параметров;
- являющихся параметрами контейнеров (STRUCT_SPECTR).
Значения одинаковых полей этой структуры для разных контейнеров могут быть
разными.
*/
struct CfgOneKit
{
	BYTE m_bDecade;					// индекс декады, от 0 до 4
	TypeFreqResolution_FFT m_bType_dF_FFT;	// тип частотного разрешения при FFT
	TypeAver m_bType_Aver;			// тип усреднения, линейное / EMA
	TypeViewValue m_bType_ViewValue;	// тип отображаемых значений (СКЗ, амрл, СПМ, СМ)
	bool m_bValueIn_dB;				// представление в дБ
	bool m_bWriteToLogger;			// запись в логгер
	TypeTimeWindow m_bType_TimeWindow;	// тип временного окна
	TypeSignalProcessing m_bType_SigProc;	// тип обработки спектра, дифф., no, инт.

	TypeTransform m_bType_Transform;	// тип преобразования, FFT / DFT
	bool m_bFilteringDiscrete;		// фильтрация дискрет усреднённого спектра
	bool b3;
	bool b4;

// 	bool b5;
// 	bool b6;
// 	bool b7;
// 	bool b8;
	
	DWORD m_indexKit;				// индекс данного контейнера
	float m_fdF_DFT_Hz;				// частотное разрешение при DFT, Гц
	float m_fTimeAver_sec;			// период отображаемого результата, сек

	// Параметры дополнительных графиков
	Struct_Param_Synchro_AddGraf m_par_addGraf;
	
	// Параметры линии контейнера в режиме MCM (MultiChannel Mode)
	Struct_ParamLineGridGL m_lineInMCM;

	wchar_t m_sWriteComment[160];	    // комментарий для записи

	//-------------------------------------------------
	CfgOneKit();
	void SetValueDefault();
};
//------------------------------------------------------------------------------
// Параметры одного контейнера.
// Номер канала не нужен, есть GUID канала.
struct ParamOneKit
{
	GUID m_guidChan;				// идентификатор канала
	CfgOneKit m_cfg;				// параметры

	ParamOneKit()
	{
		SetValueDefault();
	}
	void SetValueDefault()
	{
		m_guidChan = GUID_NULL;
		m_cfg.SetValueDefault();
	}
};
//------------------------------------------------------------------------------
struct WinPlacement
{
	long l1;		// добавлено для выравнивания до 48 байт
	WINDOWPLACEMENT m_placement;

	WinPlacement()
	{
		SetValueDefault();
	}
	void SetValueDefault()
	{
		ZeroMemory(this, sizeof(WinPlacement));
		m_placement.length = sizeof(WINDOWPLACEMENT);
	}

	// Позиционирование окна программы
	void SetPlacement(HWND hWnd)
	{
		if ((m_placement.length == sizeof(WINDOWPLACEMENT)) &&
			((m_placement.rcNormalPosition.right - m_placement.rcNormalPosition.left) > 0) &&
			(m_placement.rcNormalPosition.bottom - m_placement.rcNormalPosition.top) > 0)
			::SetWindowPlacement(hWnd, &m_placement);
	}

	// Сохранение параметров позиционирования окна программы
	void GetPlacement(HWND hWnd)
	{
		ZeroMemory(&m_placement, sizeof(WINDOWPLACEMENT));
		m_placement.length = sizeof(WINDOWPLACEMENT);
		::GetWindowPlacement(hWnd, &m_placement);
		if (m_placement.showCmd == SW_SHOWMINIMIZED)
			m_placement.showCmd = SW_SHOWNORMAL;
	}
};
//------------------------------------------------------------------------------
struct Struct_win_Main
{
	bool m_bGrafic_def;	// у графика параметры по умолчанию {true}
	bool m_bGrafic_x;	// заданы границы отображения по Х {false}
	bool m_bGrafic_y;	// заданы границы отображения по Y {false}
	bool b4;
	bool b5;
	bool b6;
	bool b7;
	bool b8;

	grid m_parGridGL;
	// ---------------------------
	Struct_win_Main()
	{
		SetValueDefault();
	}
	void SetValueDefault()
	{
		m_bGrafic_def = true;
		m_bGrafic_x = false;
		m_bGrafic_y = false;
		m_parGridGL.SetValueDefault();
	}
};
//------------------------------------------------------------------------------
struct Struct_win_Kepstr
{
	bool m_bActivate;	// отображение окна
	bool m_bGrafic_def;	// у графика параметры по умолчанию {true}
	bool m_bGrafic_x;	// заданы границы отображения по Х {false}
	bool m_bGrafic_y;	// заданы границы отображения по Y {false}
	bool m_bDiff;
	bool b6;
	bool b7;
	bool b8;

	WinPlacement m_placement;
	grid m_parGridGL;
	// ---------------------------
	Struct_win_Kepstr()
	{
		SetValueDefault();
	}
	void SetValueDefault()
	{
		m_bActivate = false;
		m_bGrafic_def = true;
		m_bGrafic_x = false;
		m_bGrafic_y = false;
		m_bDiff = false;
		m_parGridGL.SetValueDefault();
		m_placement.SetValueDefault();
	}
};
//------------------------------------------------------------------------------
// Сечение - функция от частоты при заданном времени
struct Struct_win_Cross_2D_f
{
	bool m_bActivate;	// отображение окна
	bool m_bGrafic_def;	// у графика параметры по умолчанию {true}
	bool m_bGrafic_x;	// заданы границы отображения по Х {false}
	bool m_bGrafic_y;	// заданы границы отображения по Y {false}
	
	float m_time_sec;

	WinPlacement m_placement;
	grid m_parGridGL;
	// ---------------------------
	Struct_win_Cross_2D_f()
	{
		SetValueDefault();
	}
	void SetValueDefault()
	{
		m_bActivate = false;
		m_bGrafic_def = true;
		m_bGrafic_x = false;
		m_bGrafic_y = false;
		m_time_sec = 0.f;
		m_parGridGL.SetValueDefault();
		m_placement.SetValueDefault();
	}
};
//------------------------------------------------------------------------------
// Сечение - функция от времени при заданной частоты
struct Struct_win_Cross_2D_t
{
	bool m_bActivate;	// отображение окна
	bool m_bGrafic_def;	// у графика параметры по умолчанию {true}
	bool m_bGrafic_x;	// заданы границы отображения по Х {false}
	bool m_bGrafic_y;	// заданы границы отображения по Y {false}
	
	float m_freq_Hz;

	WinPlacement m_placement;
	grid m_parGridGL;
	// ---------------------------
	Struct_win_Cross_2D_t()
	{
		SetValueDefault();
	}
	void SetValueDefault()
	{
		m_bActivate = false;
		m_bGrafic_def = true;
		m_bGrafic_x = false;
		m_bGrafic_y = false;
		m_freq_Hz = 0.f;
		m_parGridGL.SetValueDefault();
		m_placement.SetValueDefault();
	}
};
//------------------------------------------------------------------------------
struct Struct_win_Gramma_2D
{
	bool m_bActivate;	// отображение окна
	bool m_bGrafic_def;	// у графика параметры по умолчанию {true}
	bool m_bGrafic_x;	// заданы границы отображения по Х {false}
	bool m_bGrafic_y;	// заданы границы отображения по Y {false}
	bool m_bGrafic_z;	// заданы границы отображения по Z {false}
	bool b6;
	bool b7;
	bool b8;

	float m_interval_sec;	// интервал отображения в сек
	long l2;

	Struct_win_Cross_2D_f m_parWin_f;
	Struct_win_Cross_2D_t m_parWin_t;

	WinPlacement m_placement;
	Struct_ParamGrammaGL m_parGrammaGL;
	// ---------------------------
	Struct_win_Gramma_2D()
	{
		SetValueDefault();
	}
	void SetValueDefault()
	{
		m_bActivate = false;
		m_bGrafic_def = true;
		m_bGrafic_x = false;
		m_bGrafic_y = false;
		m_bGrafic_z = false; 
		m_interval_sec = 10.f;
		m_parWin_f.SetValueDefault();
		m_parWin_t.SetValueDefault();
		m_parGrammaGL.SetValueDefault();
		m_placement.SetValueDefault();
	}
};
//------------------------------------------------------------------------------
struct Struct_win_Gramma_3D
{
	bool m_bActivate;	// отображение окна
	bool m_bGrafic_def;	// у графика параметры по умолчанию {true}
	bool m_bGrafic_x;	// заданы границы отображения по Х {false}
	bool m_bGrafic_y;	// заданы границы отображения по Y {false}
	bool m_bGrafic_z;	// заданы границы отображения по Z {false}
	bool b6;
	bool b7;
	bool b8;

	float m_interval_sec;	// интервал отображения в сек
	long l2;

	WinPlacement m_placement;
	Struct_ParamGramma m_parGramma;
	// ---------------------------
	Struct_win_Gramma_3D()
	{
		SetValueDefault();
	}
	void SetValueDefault()
	{
		m_bActivate = false;
		m_bGrafic_def = true;
		m_bGrafic_x = false;
		m_bGrafic_y = false;
		m_bGrafic_z = false;
		m_interval_sec = 10.f;
		m_parGramma.SetValueDefault();
		m_placement.SetValueDefault();
	}
};
//------------------------------------------------------------------------------
struct Struct_win_Prohod
{
	bool m_bActivate;	// отображение окна
	bool m_bGrafic_def;	// у графика параметры по умолчанию {true}
	bool m_bGrafic_x;	// заданы границы отображения по Х {false}
	bool m_bGrafic_y;	// заданы границы отображения по Y {false}

	DWORD m_index_cur;		// текущий индекс
	float m_interval_sec;	// интервал отображения в сек
	float m_freq_Hz;		// частота проходной

	WinPlacement m_placement;
	grid m_parGridGL;
	// ---------------------------	
	Struct_win_Prohod()
	{
		SetValueDefault();
	}
	void SetValueDefault()
	{
		m_bActivate = false;
		m_bGrafic_def = true;
		m_bGrafic_x = false;
		m_bGrafic_y = false;
		m_index_cur = 0;
		m_interval_sec = 10.f;
		m_freq_Hz = 0.f;
		m_parGridGL.SetValueDefault();
		m_placement.SetValueDefault();
	}
};
//------------------------------------------------------------------------------
struct Struct_win_Resonance
{
	bool m_bActivate;	// отображение окна
	bool m_bGrafic_def;	// у графика параметры по умолчанию {true}
	bool m_bGrafic_x;	// заданы границы отображения по Х {false}
	bool m_bGrafic_y;	// заданы границы отображения по Y {false}
	bool b5;
	bool b6;
	bool b7;
	bool b8;

	WinPlacement m_placement;
	grid m_parGridGL;
	// ---------------------------
	Struct_win_Resonance()
	{
		SetValueDefault();
	}
	void SetValueDefault()
	{
		m_bActivate = false;
		m_bGrafic_def = true;
		m_bGrafic_x = false;
		m_bGrafic_y = false;
		m_parGridGL.SetValueDefault();
		m_placement.SetValueDefault();
	}
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Структура для записи в конфигурационный файл программы.
struct SaveStructure : public CustomSaveStructure
{
	bool m_bMCM;			// MultiChannel Mode
	TypeViewValue m_bTypeGraf_mcm;	// тип общих графиков в MCM
	bool b3;
	bool b4;
	bool b5;
	bool b6;
	bool b7;
	bool b8;

	DWORD m_num_kits;			// кол-во контейнеров
	DWORD m_indx_kitCur;		// индекс текущего контейнера

	Struct_win_Main m_parWin_Main;
	Struct_win_Kepstr m_parWin_Kepstr;
	Struct_win_Gramma_2D m_parWin_2D;
	Struct_win_Gramma_3D m_parWin_3D;
	Struct_win_Prohod m_parWin_Prohod;
	Struct_win_Resonance m_parWin_Reson;

	ParamOneKit m_par_kit_0;		// параметры нулевого контейнера
//	ParamOneKit m_par_kit_1;		// параметры первого контейнера
//	...
//	ParamOneKit m_par_kit_N-1;	// параметры последнего контейнера

	// ---------------------------------------------------------
	SaveStructure(CDSRV * psrv = nullptr);
	void SetValueDefault(CDSRV * psrv = nullptr);

	// Проверка размера
	bool IsSizeOk();

	// Получение указателя на CFG без проверки размера
	ParamOneKit * GetPointer(DWORD indxKit);
};
//-----------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#define MinOrderFFT			10
#define MaxOrderFFT			19
#define MaxSize				524288	// 2 ^ MaxOrderFFT
#define MaxSize_2			262144	// 2 ^ (MaxOrderFFT - 1)
//------------------------------------------------------------------------------
// Константа, определяющая диапазон частот
#define g_factor_freq_band		0.4f
//-----------------------------------------------------------------------------
// Максимальное значение декады
#define g_decada_Max			4
//-----------------------------------------------------------------------------
// Минимальное и максимальное значения времени усреднения, сек
#define g_timeAver_Min			0.01f
#define g_timeAver_Max			200.f
//-----------------------------------------------------------------------------
// Минимальное и максимальное значения интервала расчета доп. графиков, мин
#define g_intervalGraf_Minute_Min			10.f
#define g_intervalGraf_Minute_Max			1440.f		// 24 * 60 = 1 сутки
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Функции проверки новых значений cfg-параметров
//
// Возвращаемое значение: true/false - новое значение входит в ОДЗ.
// Последний параметр функций: bChange - изменять или нет новые значения с целью
// вхождения их в ОДЗ.
//------------------------------------------------------------------------------
bool Test_Decada(BYTE & newVal, const bool bChange);
bool Test_FreqResolution_FFT(TypeFreqResolution_FFT & type, const bool bChange);
bool Test_FreqResolution_DFT(TypeFreqResolution_DFT & type, const bool bChange);
bool Test_TypeTimeAver(TypeTimeAver & newVal, const bool bChange);
bool Test_TypeViewValue(TypeViewValue & newVal, const bool bChange);
bool Test_TypeWindow(TypeWindow & newVal, const bool bChange);
bool Test_TypeSignalProcessing(TypeSignalProcessing & newVal, const bool bChange);
bool Test_TypeTransform(TypeTransform & newVal, const bool bChange);
bool Test_TimeAver_sec(float & newVal, const bool bChange);
bool Test_IntervalGraf_Min(float & newVal, const bool bChange);

template <class T>				// T - это float, double
bool Test_dF_DFT_Hz(T & newVal, const bool bChange, const T freqADC)
{
	T valMin((T)0), valMax((T)0);
	Convert_TypeFreqResolutionDFT_dF(tfrDFT_100000, freqADC, valMin);
	Convert_TypeFreqResolutionDFT_dF(tfrDFT_100, freqADC, valMax);

	return TestAndChangeValue(newVal, valMin, valMax, bChange);
}
//-----------------------------------------------------------------------------
// Вычисление размера CFG файла
DWORD CalcSizeFileCFG(const DWORD numKits);

// Проверка размера CFG файла.
// Возвращает true если файл с таким размером может быть CFG-файлом
bool TestSizeFileCFG(const DWORD sizeFile);
//-----------------------------------------------------------------------------
// Расчет параметров DFT:
// freqADC - частота дискретизации с учётом декады;
// df - требуемое частотное разрешение DFT;
// sizeDFT - полный размер массива спектра DFT (от 0 до freqADC);
// sizeFFT - размер исходных данных при расчёте FFT;
// orderFFT - порядок, выполняемого FFT
// void GetParamDFT(const float freqADC, const float df, int & sizeDFT, int & sizeFFT, int & orderFFT);
//-----------------------------------------------------------------------------