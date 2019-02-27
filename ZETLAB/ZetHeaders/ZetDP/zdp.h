//------------------------------------------------------------------------------
// zdp.h - этот файл должен использоваться как в исходных файлах dll, так и в 
// файлах разрабатываемых приложений
//------------------------------------------------------------------------------
#pragma once

#ifndef	zdp_h
#define	zdp_h

#include <zdpdef.h>
// Т.к. использовать def-файл не планируется, то будем использовать ZDPLIBAPI.
// В файле zdp.cpp следует писать так:
// #define ZDPLIBAPI __declspec(dllexport)
// #include "zdp.h"
// В файлах приложений только
// #include "zdp.h"
#ifndef ZDPLIBAPI
#define ZDPLIBAPI __declspec(dllimport)
#endif



#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//стандартные функции, предостаавляемые библиотеками нижнего уровня
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 	// Функции инициализации
	ZDPLIBAPI long zdInit					(HWND hWnd, zdGUIType typeGUI);

	ZDPLIBAPI long zdSetDrawMode			(zdDrawMode dm);
	ZDPLIBAPI long zdSetSizes				(long cx, long cy);

	//функции управляющие отображением
	ZDPLIBAPI long zdStartFrame				();
	ZDPLIBAPI long zdInvalidate				();

	// Функции задания стандартных инструментов
	ZDPLIBAPI long zdSetPicture				(zdPicture *pPicture, const WCHAR *filename);
	ZDPLIBAPI long zdSetPictureFromIcon		(zdPicture *pPicture, HICON hIcon);

	ZDPLIBAPI long zdSetFont				(zdFont *pFont, const WCHAR *family, float size, zdFontStyle style, COLORREF color, zdStringAlignment aligHor, zdStringAlignment aligVert);		
	
	ZDPLIBAPI long zdSetBrushSolid			(zdBrush *pBrush, COLORREF color);											
	ZDPLIBAPI long zdSetBrushTexture		(zdBrush *pBrush, const WCHAR *fileName, float transformX, float transformY);									
	ZDPLIBAPI long zdSetBrushGradientLine	(zdBrush *pBrush, COLORREF colFrom, COLORREF colTo, const zdRect &rect, float angle, float focus, float scale);	
	ZDPLIBAPI long zdSetBrushGradientPoint	(zdBrush *pBrush, COLORREF colFigure, COLORREF colCentr, const zdRect &rect, const POINT &centrPoint, float scaleX, float scaleY,  zdFigureType figureType);					

	ZDPLIBAPI long zdSetPenSolid			(zdPen *pPen, COLORREF color, float thickness, zdLineStyle lStyle);		
	ZDPLIBAPI long zdSetPenFromBrush		(zdPen *pPen, zdBrush *pBrush, float thickness, zdLineStyle lStyle); 

	// Функции для работы со строками
	ZDPLIBAPI long zdDrawString				(const WCHAR *str, zdFont *pFont, long x, long y, long cx, long cy);
	ZDPLIBAPI long zdMeasureString			(const WCHAR *str, zdFont *pFont, zdRect &rect);
	ZDPLIBAPI long zdFitFontToRect			(const WCHAR *str, zdFont *pFont, const zdRect &rect);

	// Функции отображения
	ZDPLIBAPI long zdDrawPicture			(zdPicture *pPicture, long x, long y, long cx, long cy);

	ZDPLIBAPI long zdDrawLine				(zdPen *pPen, long x1, long y1, long x2, long y2);
	ZDPLIBAPI long zdDrawRectangle			(zdPen *pPen, long x, long y, long cx, long cy);
	ZDPLIBAPI long zdDrawEllipse			(zdPen *pPen, long x, long y, long cx, long cy);
	ZDPLIBAPI long zdDrawPolygon			(zdPen *pPen, POINT *points, long count);
	ZDPLIBAPI long zdDrawCurve				(zdPen *pPen, POINT *points, long count);
	ZDPLIBAPI long zdDrawBezier				(zdPen *pPen, long x1, long y1, long x2, long y2, long x3, long y3, long x4, long y4);

	ZDPLIBAPI long zdDrawRadiusRect			(zdPen *pPen, long x, long y, long cx, long cy, long radius);

	ZDPLIBAPI long zdFillRectangle			(zdBrush *pBrush, long x, long y, long cx, long cy);
	ZDPLIBAPI long zdFillEllipse			(zdBrush *pBrush, long x, long y, long cx, long cy);
	ZDPLIBAPI long zdFillPolygon			(zdBrush *pBrush, POINT *points, long count);

	ZDPLIBAPI long zdFillRadiusRect			(zdBrush* pBrush, long x, long y, long cx, long cy, long radius);	

	// Функции удаления инструментов
	ZDPLIBAPI long zdDeletePicture			(zdPicture *pPicture);
	ZDPLIBAPI long zdDeleteFont				(zdFont *pFont);				
	ZDPLIBAPI long zdDeletePen				(zdPen *pPen);				
	ZDPLIBAPI long zdDeleteBrush			(zdBrush *pBrush);	


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//функции, не входящие в стандартный набор инструментов библиотек нижнего уровня
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Функции изменения цвета
	ZDPLIBAPI long zdDarkerColor			(COLORREF &color, long percent);
	ZDPLIBAPI long zdLighterColor			(COLORREF &color, long percent);

	// Функции отображения кастомных фигур
	ZDPLIBAPI long zdDrawGroupBox			(zdPen *pPen, zdBrush* pBrush, zdFont *pFont, const WCHAR *str, const zdRect &rect, long radius);

#ifdef __cplusplus
}
#endif

#endif	//zdp_h