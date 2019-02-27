#pragma once
#include "Struct_trend.h"
#include <mathZet.h>
#include <tuple>
#include <Grafic1\GraficTools\ColorLine\ColorLine.h>
#include <freetype_bin\include\FreeTypeFont.h>
#include <Grafic1/GraficTools/GridDigits/GridDigits.h>
#include <Grafic1\GraficTools\GridLayoutX\GridLayoutX.h>
//*************************************************************************************************
#define INTERVAL_BETWENN_COLORLINES		4			// интервал между событийными линиями
#define SPLIT_FIELDS_DELTA				17			// отступ для линий отображающийх максимальное и минимальное значение полей для режима SplittingFields
#define MAX_PIX_SIZE					150			// расстояние между вертинкальной сеткой после которого рисуются промежуточные линии
#define SHAG_CURSOR_INPIX				3			// максимальная ширина шага курсора
#define NEED_INVALIDATE					2			// возвращаемое значение в случае необходимости перерисовать контрол
#define CURSOR_SIZE						7			// размер курсора
#define PERSENTAGE_INDENTION			0.01 * 5	// на какой процент от ширины графика сделать отступ по краям при автомасштабе
#define MIN_WIDTH						1.e-10		// минимальная ширина при расширении
//*************************************************************************************************
// Функция вычисляет красивое значение числа, и ближайших к нему
float CalculateStunnerValues(_In_ float Val, _Out_opt_ float *PrevVal = nullptr, _Out_opt_ float *NextVal = nullptr);
//*************************************************************************************************
class CTimeGridDrow
{
public:
	CTimeGridDrow();
	~CTimeGridDrow();
protected:	
	//*********************************************************************************************
	struct MyPointsStruct   // структура с точками для отрисовки
	{
		GLint x;
		GLfloat y;
	};
	//*********************************************************************************************
	typedef struct MarkStruct
	{
		MarkStruct()
			: MarkStruct(0, _T(""), RGB(255, 0, 0))
		{}
		MarkStruct(double _Time, CString _Text, COLORREF _Color)
			: Time	(_Time)
			, Text	(_Text)
			, Color	(_Color)
		{}
		double Time;
		CString Text;
		COLORREF Color;
	};
	std::vector<MarkStruct> m_MarkLines;			// список отметок на графике
	//*********************************************************************************************
	typedef struct ToolTipMemb
	{
		ToolTipMemb()
		{
			text.Empty();
			pos = 0;
			rect = CRect(0, 0, 20, 20);
			addedToVisible = false;
		}
		CString text;
		long pos;
		CRect rect;
		bool addedToVisible;
	};
	std::vector <ToolTipMemb> m_toolTips;			// для удобства доступа
	//*********************************************************************************************
	// фигура отображаемая около курсора
	enum CursorFigure
	{
		Figure_Thriangle = 0,
		Figure_Square,
		Figure_Circle
	};
	// типы курсора
	enum CursorType
	{
		Type_Default = 0,		// обычный курсор, использует цвет, настраиваемый в сетке
		Type_Imposition,		// курсор, использующийся при работе в режиме наложения графиков
		Type_Users				// созданный пользователем
	};
	typedef struct CursorStruct
	{
		CursorStruct()
			: CursorStruct(0, CursorFigure::Figure_Thriangle, RGB(0, 0, 255), CursorType::Type_Default)
		{}
		CursorStruct(double _CurrentPos, CursorFigure _Figure, COLORREF _Color, CursorType _Type)
			: CurrentPos	(_CurrentPos)
			, Figure		(_Figure)
			, Color			(_Color)
			, Type			(_Type)
		{}
		double CurrentPos;		// текущее положение курсора
		CursorFigure Figure;	// фигура отображаемая при перетаскивании курсора
		COLORREF Color;			// цвет курсора
		CursorType Type;		// тип курсора
	};
	//*********************************************************************************************
	typedef struct SplitFieldsStruct
	{
		float MinValue;			// минимальное  значение для поля
		float MaxValue;			// максимальное значение для поля
		float PixYMin;			// пиксель на котором находится минимальное  значение для этого поля
		float PixYMax;			// пиксель на котором находится максимальное значение для этого поля
		float TopLineCoord;		// координата верхней линии разделяющей графики			

		SplitFieldsStruct()
		{
			ZeroMemory(this, sizeof(SplitFieldsStruct));
		}
		SplitFieldsStruct(const SplitFieldsStruct & val)
		{
			memcpy_s(this, sizeof(SplitFieldsStruct), &val, sizeof(SplitFieldsStruct));
		}
		SplitFieldsStruct & operator = (const SplitFieldsStruct & val)
		{
			memcpy_s(this, sizeof(SplitFieldsStruct), &val, sizeof(SplitFieldsStruct));
			return *this;
		}
	};
	float m_bSplitFieldsHeight;			// высота, доступная каждому полю
	// флаг необходимости отображать имя сверху полей
	// false - не отображается имя графика сверху, по возможности отображаем его сбоку
	// true - имя поля отображается сверху
	bool m_bSplitFieldsNeedNameOnTop;
	std::vector<SplitFieldsStruct> m_SplitFieldsData;		// список параметров для каждого поля
	//*********************************************************************************************
	std::vector<ColorLineStruct> m_ColorLine;		// список цветных линий
	//*********************************************************************************************
	std::vector<bool> m_bValidGraphs;				// видимость графика		| true - отображается / false - не отображается
	std::vector<CString> m_FieldsName;				// вектор с называниями для каждого поля
	std::vector<CString> m_FieldsUnits;				// вектор с единицами измерения для каждого поля
	//*********************************************************************************************
	GridLayoutX CursorCoords;						// возможные координаты вертикальных курсоров
	CGridDigits m_VertDigits;							// вертикальная разметка
	std::vector<CursorStruct> m_CoordVertCurs;		// координата вертикальных курсоров
	std::vector<CursorStruct> m_CoordGorCurs;		// координата горизонтальных курсоров
	//std::vector<double> m_CoordVertCurs;				// координата вертикальных курсоров
	//std::vector<double> m_CoordGorCurs;				// координата горизонтальных курсоров
	int CurSelectedCursX;							// индекс текущего выделеного вертикального курсора 
	int CurSelectedCursY;							// индекс текущего выделеного горизонтального курсора 
	bool VertCursMoves;								// флаг перемещения вертикального курсора
	bool GorCursMoves;								// фдаг перетаскивания горизонтального курсора
	//*********************************************************************************************
	GLuint m_FontList;
	HPALETTE m_hPalette;
	HGLRC m_hRC;
	HDC  m_hDC;										//указатель на контекст устройства	

	bool m_bNeedLoadNewFont;						// флаг того что был загружен новый шрифт
	FreeTypeFont m_TextFont;						// класс отображения текста

	HFONT m_FontNew;
	bool stop;
	bool bUseOrtho2D;								// флаг того что нужно использовать 2мерное рисование
	//*********************************************************************************************
public:
	double m_LeftXCoord, m_RightXCoord;					// левая и правая граница по оси Х
	double m_VisibleLeftXCoord, m_VisibleRightXCoord;	// левая и правая граница по оси Х текущих отображенных структур
	double m_TopYCoord, m_BottomYCoord;					// верхняя и нижняя граница по оси Y
	double m_VisibleTopYCoord, m_VisibleBottomYCoord;	// верхняя и нижняя граница по оси Y текущих отображенных структур
	double m_IntervalTopYCoord, m_IntervalBottomYCoord;	// верхняя и нижняя граница по оси Y на текущем интервале
	//*********************************************************************************************
public:
	CString m_SelectedGraphName;						// имя выбранного графика
	int m_FontListsNum;
	size_t m_CountGraph;								// общее количество отображаемых полей
	size_t m_CurGraphCount;								// количество отображаемых полей
protected:
	void TextGL(GLfloat x, GLfloat y, LPCTSTR text, GLubyte R, GLubyte G, GLubyte B);
	void glPrint(LPCTSTR text);
	//*********************************************************************************************
	float GetPixX(struct TrendGrid *grd, double x);
	float GetPixY(struct TrendGrid *grd, double y);
	//*********************************************************************************************
	// получаем разницу в текстовом виде
	CString GetTimeText(_In_ CZetTimeSpan DeltaTime);	// разница во времени
	CString GetDataText(_In_ double DeltaTime);			// разница в данных
	// расстояние между курсорами
	void GetCursDistance(struct TrendGrid* grd, _Out_ CString &HorStr, _Out_ CString &VertStr);
	//*********************************************************************************************
	// получаем текущую единицу измерения если она совпадает для всех графиков
	CString GetCurrentUnitStr();
	//*********************************************************************************************
	// получение значений из координат по Х и Y
	double GetXFromCoord(struct TrendGrid *grd, long);
	double GetYFromCoord(struct TrendGrid *grd, long);
	//*********************************************************************************************
#ifndef _UNICODE
	enum { m_FontListsStart = 0, m_FontListsEnd = 257 };
#else
	enum { m_FontListsStart = 0, m_FontListsEnd = (1 + (int)MAXWORD) };
#endif
	//*********************************************************************************************
	void InicializeOrtho(CRect Rect);									// инициализация режима ORTHO
	bool PrepareOGL(struct TrendGrid *grd, CDC* pdc, const CRect &rc);	// подготока OPENGL
	bool BuildFontOGL(HDC hDC, HFONT hFont, long FontSize = -1);		// установка шрифта
	//*********************************************************************************************
	void CalculateControlSize(CDC* pdc, struct TrendGrid *grd, const CRect &rcBounds);	// проверяет размеры контрола
	int  SetSetka(CDC* pdc, struct TrendGrid *grd, const CRect &rcBounds, bool m_bEnable, 
				  int CountGraphs = 0);	// рисуем сам прямоугольник и сетку
	//*********************************************************************************************
	void DrowVertLine(struct TrendGrid* grd, MyPointsStruct pnt1, MyPointsStruct pnt2, long posX);
	long VertLines(CDC* pdc, struct TrendGrid* grd);					// вертикальные линии
	long DefaultHorLines(struct TrendGrid* grd);						// стандартные горизонтальные линии
	void PaintIntermediateLines(struct TrendGrid* grd, float Top, float Bottom);// отрисовываем промежуточные линии в режиме разделения на графики
	long SplitFieldsHorLines(CDC* pdc, struct TrendGrid* grd);			// горизонтальные линии для режима разделения на графики
	long ImpositionHorLines(CDC* pdc, struct TrendGrid* grd);			// горизонтальные линии для режима разделения на графики
	//*********************************************************************************************
	void DrowVertCursor(_In_ struct TrendGrid *grd, _In_ const CursorStruct &CursInfo, _In_ bool bSelected);
	void DrowHorzCursor(_In_ struct TrendGrid *grd, _In_ const CursorStruct &CursInfo, _In_ bool bSelected);	
	void DrawCursorsX(_Inout_ struct TrendGrid *grd);					// нарисовать вертикальные курсоры
	void DrawCursorsY(_Inout_ struct TrendGrid *grd);					// нарисовать горизонтальные курсоры
	//*********************************************************************************************
	void PaintDistinguish(CPoint, CPoint);								// рисуем прямоугольник с выделением
	//*********************************************************************************************
	_Check_return_ int SetSetkaDigits(CDC* pdc, struct TrendGrid *grd, const CRect &rcBounds);	// возвращает NEED_INVALIDATE если нужно обновить контрол
	long VertDigits(CDC* pdc, struct TrendGrid* grd, const CRect &rcBounds);					// вертикальная разметка
	//*********************************************************************************************
	/*	*Функция рассчитывает где и какие названия графиков расположить
		@param Data - итоговый вектор с именами [Y координата на которой надо расположить подпись, подпись]
		@return NEED_INVALIDATE если нужно будет передвинуть график	*/	
	//long CalculateSeismoNames(CDC* pdc, struct TrendGrid* grd, const CRect &rcBounds, _Out_ std::vector<std::pair<float, CString>> *Data);
	long VertDigitsSeismo(CDC* pdc, struct TrendGrid* grd, const CRect &rcBounds);				// разметка в виде имен каждого графика
	//*********************************************************************************************
	long SplitFieldsVertDigits(CDC* pdc, struct TrendGrid* grd, const CRect &rcBounds);			// вертикальная разметка для режима разделений на графики
	// промежуточная вертикальная разметка
	long CalculateIntermediateSplitDigits(_Inout_ struct TrendGrid *grd, _In_ CDC* pdc,				
										  _In_ double MaxVal, _In_ double MinVal, 
										  _In_ GLfloat CurCoord, _In_ GLfloat HeightVert, _In_ bool NeedDrow = false);
	void CalculateSplitFieldsSettings(_In_ struct TrendGrid* grd, _In_ size_t CountOfFields, _In_opt_ bool bCheckHeight = true);
	//*********************************************************************************************
	long HorDigits(CDC* pdc, struct TrendGrid* grd);											// разметка по горизонтали
	//*********************************************************************************************
	virtual void CheckScaleX(struct TrendGrid *grd);							// проверка при масштабировании сетки по оси Х
	void CheckScaleY(struct TrendGrid *grd);							// проверка при масштабировании сетки по оси Y
	//*********************************************************************************************
	void AutoScaleVisibleData(struct TrendGrid* grd);					// автомасштабирование отображаемых точек
	void AutoScaleAllData(struct TrendGrid* grd);						// автомасштабирование всех точек
	void AutoScaleYData(struct TrendGrid* grd);							// автомасштабирование данных в видимой области по оси Y
	//*********************************************************************************************
	void DrowColorLine(CDC* pdc, struct TrendGrid* grd, const CRect &rcBounds);	// рисуем линии снизу контрола
	void DrowMarkLine (CDC* pdc, struct TrendGrid* grd, const CRect &rcBounds);	// отрисовываем отметки на графике
	//*********************************************************************************************
	void DrawToolTips(struct TrendGrid* grd, CDC* pdc);
	void AddOrRemoveToolTipToVisible(CPoint point);
private:
	void SetupPalette();
	void AutoScale (_Inout_ struct TrendGrid* grd, _In_ double _LeftXCoord,   _In_ double _RightXCoord, _In_ double _BottomYCoord, _In_ double _TopYCoord);
	void AutoScaleX(_Inout_ struct TrendGrid* grd, _In_ double _LeftXCoord,   _In_ double _RightXCoord);
	void AutoScaleY(_Inout_ struct TrendGrid* grd, _In_ double _BottomYCoord, _In_ double _TopYCoord);
};