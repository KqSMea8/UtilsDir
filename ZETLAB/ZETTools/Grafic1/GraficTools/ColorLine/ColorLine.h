#pragma once
#include <ZetTools\include\ZetTools.h>
#include <vector>

#define COLORLINE_HEIGHT		30		// ширина области с COLORLINE
//-------------------------------------------------------------------------------------------------
// структура с отображаемыми данными
struct SpanColorLinePixStruct
{
	float PixStart;			// начальный пиксель
	float PixEnd;			// конечный пиксель
	COLORREF Color;			// цвет интервала
	SpanColorLinePixStruct()
	{
		SpanColorLinePixStruct(0.f, 0.f, RGB(255, 255, 255));
	}
	SpanColorLinePixStruct(float _Start, float _End, COLORREF _Color)
		: PixStart		(_Start)
		, PixEnd		(_End)
		, Color			(_Color)
	{}
};
//-------------------------------------------------------------------------------------------------
// структура с реальными данными для отображения
struct SpanColorLineIntervalStruct
{
	// интервал в индексах для раскраски цветом
	int IndexStart, IndexEnd;
	// временной интервал для раскраски цветом
	double IntervalStart, IntervalEnd;			
	COLORREF Color;			// цвет интервала
	SpanColorLineIntervalStruct()
		: IndexStart(0)
		, IndexEnd(0)
		, Color(0)
	{
		SpanColorLineIntervalStruct(ZetTimeMin, ZetTimeMax, 0, 0, RGB(255, 255, 255));
	}
	SpanColorLineIntervalStruct(double _TimeStart, double _TimeEnd, int _IndexBegin, int _IndexEnd, COLORREF _Color)
		: IntervalStart	(_TimeStart)
		, IntervalEnd	(_TimeEnd)
		, IndexStart	(_IndexBegin)
		, IndexEnd		(_IndexEnd)
		, Color			(_Color)
	{}
	SpanColorLineIntervalStruct(double _TimeStart, double _TimeEnd, COLORREF _Color)
		: IntervalStart	(_TimeStart)
		, IntervalEnd	(_TimeEnd)
		, Color			(_Color)
	{
	}
	SpanColorLineIntervalStruct(int _IndexBegin, int _IndexEnd, COLORREF _Color)
		: IndexStart	(_IndexBegin)
		, IndexEnd		(_IndexEnd)
		, Color			(_Color)
	{}
};
//-------------------------------------------------------------------------------------------------
struct ColorLineStruct		// структура с информацией по контролу
{
	bool bIndexWork;		// флаг что разбиваются данные согласно индексам

	COLORREF FontColor;		// цвет общего фона
	COLORREF TextColor;		// цвет текста для надписей

	CRect FullRect;			// координаты прямоугольника

	float PixBegin,			// координаты начала и конца
		  PixEnd;				// отрисовки рабочей области контрола

	CString Caption;		// название компонента
	GLuint FontList;		// шрифт

	// вектор интервалов по пикселям для отрисовки разбиений по цветам
	std::vector<SpanColorLinePixStruct> SpanDrowVector;	
	// вектор интервалов по времени для отрисовки разбиений по цветам
	std::vector<SpanColorLineIntervalStruct> SpanDateVector;

	ColorLineStruct()
		: FontColor			(RGB(255, 255, 255))
		, TextColor			(RGB(0, 0, 0))
		, bIndexWork(false)
		, PixBegin(0.f)
		, PixEnd(0.f)
	{
	}
};
//-------------------------------------------------------------------------------------------------
class CColorLine
{
public:	//-----------------------------------------------------------------------------------------
	CColorLine();
	~CColorLine();
	// отрисовать контрол c параметрами
	void DrowColorLine(_In_ CDC* pdc, _In_ struct ColorLineStruct *Struct);
private://-----------------------------------------------------------------------------------------
public:	//-----------------------------------------------------------------------------------------
private://-----------------------------------------------------------------------------------------
};

