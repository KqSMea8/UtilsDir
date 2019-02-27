//------------------------------------------------------------------------------
#pragma once
#ifndef	zdpdef_h
#define	zdpdef_h


enum zdGUIType
{
	gtGdiPlus,
	gtOpenGl,
	gtDirect2D
};

//------------------------------------------------------------------------------
enum zdDrawMode
{// типы мод прорисовки
	dmHighSpeed,
	dmHighQuality
};
//------------------------------------------------------------------------------
enum zdFigureType
{
	ftLine,
	ftRectangle,
	ftEllipse,
	ftRoundRect,
	ftPolygon,
	ftCurve
};
//------------------------------------------------------------------------------
enum zdFillType
{// типы заполнения
	ftNone,
	ftSolid,
	ftTexture,
	ftGradientLine,
	ftGradientPoint
};
//------------------------------------------------------------------------------
enum zdLineStyle
{// тип линии
	lsSolid,	// сплошная
	lsDashed,	// штриховая из коротких линий
	lsDotted,	// пунктирная (из точек)
	lsDashDot,	// штрих-пунктирная (тире, точка)
};
//------------------------------------------------------------------------------
enum zdFontStyle
{// стили шрифтов
	fsRegular    = 0,
	fsBold       = 1,
	fsItalic     = 2,
	fsBoldItalic = 3,
	fsUnderline  = 4,
	fsStrikeout  = 8
};
//------------------------------------------------------------------------------
enum zdStringAlignment
{// типы выравнивания строк
	saNear,
	saCenter,
	saFar
};
//------------------------------------------------------------------------------
struct zdRect
{// прямоугольник - левая верхняя точка, ширина, высота
	long x;
	long y;
	long cx;
	long cy;
	zdRect() { x = y = cx = cy = 0; }
};
//------------------------------------------------------------------------------
struct zdSize
{// размеры - ширина, высота
	long cx;
	long cy;
	zdSize() { cx = cy = 0; }
};
//------------------------------------------------------------------------------
struct zdFontParam
{// параметры шрифта
	const WCHAR *family;
	float size;
	zdFontStyle style;
	COLORREF color;

	zdStringAlignment aligHor;
	zdStringAlignment aligVert;

	zdFontParam()
	 : family(L"Arial")
	 , size(9.0f)
	 , style(fsRegular)
	 , color(RGB(23, 23, 23))
	 , aligHor(saCenter)
	 , aligVert(saCenter)
	{}
};
//------------------------------------------------------------------------------
// struct zdFigureParam
// {// параметры фигуры
// 	zdFigureType f_type;
// 	long size_params;
// 	long *f_params;
// 
// 	zdFigureParam()
// 	 : f_params(NULL)
// 	 , size_params(0)
// 	{}
// 
// 	~zdFigureParam()
// 	{
// 		if(f_params)			// указатель должен быть удален вызовом функции zdDeleteFigure, но на всякий случай здесь проверяется и если указатель не нулевой, то удаляем здесь.
// 			delete[] f_params;
// 	}
// 
// };
//------------------------------------------------------------------------------
struct zdPenParam
{// параметры пера
	COLORREF color;
	float thickness;
	zdLineStyle lStyle;

	zdPenParam()
	 : color(RGB(23, 23, 23))
	 , thickness(1.0f)
	 , lStyle(lsSolid)
	{}
};
//------------------------------------------------------------------------------
struct zdBrushParam
{// параметры кисти
	zdFillType type;
	COLORREF colorFrom;
	COLORREF colorTo;

	float angle;		

	float transformX;	//only for texture brush
	float transformY;	//
	
	float focus;		 
	float scale;	

	POINT centrPoint;	// only for point brush

	zdRect rect;

	zdBrushParam()
	 : type(ftNone)
	 , colorFrom(RGB(255, 255, 255))
	 , colorTo(RGB(23, 23, 23))
	 , angle(0.0f)
	 , transformX(0.0f)
	 , transformY(0.0f)
	 , focus(0.0f)
	 , scale(0.0f)
	{}
};
//------------------------------------------------------------------------------
struct zdFont
{// "шрифт", не привязанный к какому-либо графическому интерфейсу
	PVOID pointer;
	zdFontParam param;

	zdFont() 
	 : pointer(NULL)
	{}

	~zdFont()
	{
		if(pointer)				// указатель должен быть удален вызовом функции zdDeleteFont, но на всякий случай здесь проверяется и если указатель не нулевой, то удаляем здесь.
			delete pointer;
	}
};
//------------------------------------------------------------------------------
// struct zdFigure
// {// "фигура", не привязанная к какому-либо графическому интерфейсу
// 	PVOID pointer;
// 	zdFigureParam param;
// 
// 	zdFigure() 
// 	 : pointer(NULL)
// 	{}
// 
// 	~zdFigure()
// 	{
// 		if(pointer)				// указатель должен быть удален вызовом функции zdDeletePicture, но на всякий случай здесь проверяется и если указатель не нулевой, то удаляем здесь.
// 			delete pointer;
// 	}
// };
//------------------------------------------------------------------------------
struct zdPicture
{// "картинка", не привязанная к какому-либо графическому интерфейсу
	PVOID pointer;
	zdSize param;	// исходные размеры

	zdPicture() 
	 : pointer(NULL)
	{}

	~zdPicture()
	{
		if(pointer)				// указатель должен быть удален вызовом функции zdDeletePicture, но на всякий случай здесь проверяется и если указатель не нулевой, то удаляем здесь.
			delete pointer;
	}
};
//------------------------------------------------------------------------------
struct zdPen
{// "перо", не привязанное к какому-либо графическому интерфейсу
	PVOID pointer;
	zdPenParam param;

	zdPen()
	 : pointer(NULL)
	{}

	~zdPen()
	{
		if(pointer)				// указатель должен быть удален вызовом функции zdDeletePen, но на всякий случай здесь проверяется и если указатель не нулевой, то удаляем здесь.
			delete pointer;
	}
};
//------------------------------------------------------------------------------
struct zdBrush
{// "кисть", не привязанная к какому-либо графическому интерфейсу
	PVOID pointer;
	zdBrushParam param;

	zdBrush() 
	 : pointer(NULL)
	{}

	~zdBrush()
	{
		if(pointer)				// указатель должен быть удален вызовом функции zdDeleteBrush, но на всякий случай здесь проверяется и если указатель не нулевой, то удаляем здесь.
			delete pointer;
	}
};
//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------