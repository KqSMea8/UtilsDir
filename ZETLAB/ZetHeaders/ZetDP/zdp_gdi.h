#pragma once

#ifndef	zdp_gdi_h 
#define	zdp_gdi_h


#ifndef ZDPLIBAPI
#define ZDPLIBAPI __declspec(dllimport)
#endif	//ZDPLIBAPI

#include <zdpdef.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus 



 	ZDPLIBAPI long zInit					(HWND hWnd);			// инициализация
 
 	
 	ZDPLIBAPI long zSetDrawMode				(zdDrawMode dm);
 	ZDPLIBAPI long zSetSizes				(const long cx, const long cy);


 
 	ZDPLIBAPI long zSetFont					(zdFont *pFont);
 
 	ZDPLIBAPI long zSetPicture				(zdPicture *pPicture, const WCHAR *filename);	
	ZDPLIBAPI long zSetPictureFromIcon		(zdPicture *pPicture, HICON hIcon);
 
 	ZDPLIBAPI long zSetBrushSolid			(zdBrush *pBrush);											
 	ZDPLIBAPI long zSetBrushTexture			(zdBrush *pBrush, const WCHAR *fileName);									
 	ZDPLIBAPI long zSetBrushGradientLine	(zdBrush *pBrush);	
 	ZDPLIBAPI long zSetBrushGradientPoint	(zdBrush *pBrush,  zdFigureType figureType);

 	ZDPLIBAPI long zSetPenSolid				(zdPen *pPen);		
 	ZDPLIBAPI long zSetPenFromBrush			(zdPen *pPen, zdBrush *pBrush); 
 

 
 	// Функции прорисовки и заливки фигур
	ZDPLIBAPI long zDrawLine			(zdPen *pPen, long x1, long y1, long x2, long y2);
	ZDPLIBAPI long zDrawRectangle		(zdPen *pPen, long x, long y, long cx, long cy);
	ZDPLIBAPI long zDrawEllipse			(zdPen *pPen, long x, long y, long cx, long cy);
	ZDPLIBAPI long zDrawPolygon			(zdPen *pPen, POINT *points, long count);
	ZDPLIBAPI long zDrawCurve			(zdPen *pPen, POINT *points, long count);
	ZDPLIBAPI long zDrawBezier			(zdPen *pPen, long x1, long y1, long x2, long y2, long x3, long y3, long x4, long y4);

	ZDPLIBAPI long zDrawRadiusRect		(zdPen *pPen, long x, long y, long cx, long cy, long radius);

	ZDPLIBAPI long zFillRectangle		(zdBrush *pBrush, long x, long y, long cx, long cy);
	ZDPLIBAPI long zFillEllipse			(zdBrush *pBrush, long x, long y, long cx, long cy);
	ZDPLIBAPI long zFillPolygon			(zdBrush *pBrush, POINT *points, long count);

	ZDPLIBAPI long zFillRadiusRect		(zdBrush* pBrush, long x, long y, long cx, long cy, long radius);

 
 	ZDPLIBAPI long zDrawPicture			(zdPicture *pPicture, long x, long y, long cx, long cy);
 
 	// Функции для работы со строками
 	ZDPLIBAPI long zDrawString			(const WCHAR *str, zdFont *pFont, long x, long y, long cx, long cy);
 	ZDPLIBAPI long zMeasureString		(const WCHAR *str, zdFont *pFont, zdRect &rect);
	ZDPLIBAPI long zFitFontToRect		(const WCHAR *str, zdFont *pFont, const zdRect &rect);


	ZDPLIBAPI long zDeleteFont			(zdFont *pFont);		
	ZDPLIBAPI long zDeletePen			(zdPen *pPen);				
	ZDPLIBAPI long zDeleteBrush			(zdBrush *pBrush);
	ZDPLIBAPI long zDeletePicture		(zdPicture *pPicture);


 	ZDPLIBAPI long zInvalidate();

#ifdef __cplusplus
}
#endif	//__cplusplus

#endif	//zdp_gdi_h

