//------------------------------------------------------------------------------
//	FunctionForAll.h - файл заголовка
//	Функции для использования во всех классах
//------------------------------------------------------------------------------
#pragma once
#include <Interface2012\Custom\DeleteObject.h>
#ifdef _GDIPLUS
#include <Interface2012\Custom\Font.h>
#else

//#define GLEW_STATIC

extern "C"

#include <ipps.h>
#include <Interface2012\Custom\STL\STLRead.h>
#include <Interface2012\Custom\STL\Detail.h>
#include <Interface2012\Custom\CFonts.h>
#//include <GL\glew.h>
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\GLAux.h"

#ifdef	_WIN32
//#include <GL/wglew.h>
#else
#include <GL/glxew.h>
#endif
#ifdef _ACTIVEX_ZIS
#include "CDZetIntSource.h"
#endif
//#include <GL\freeglut.h>
#pragma comment(lib, "glaux.lib")
#pragma comment (lib, "winmm.lib")     /* link with Windows MultiMedia lib */
#pragma comment (lib, "opengl32.lib")  /* link with Microsoft OpenGL lib */
//#pragma comment (lib, "glew32.lib")
//#pragma comment (lib, "glew32mx.lib")
//#pragma comment (lib, "glew32s.lib")
#define M_PI       3.14159265358979323846
#endif
#ifdef _ACTIVEX_ZIS
extern CDZetIntSource *ZetIntSrc; 
#endif

//------------------------------------------------------------------------------
bool PointInRect(CPoint point, CRect *pRect);

bool LineCrossRect(CPoint point1, CPoint point2, CRect *pRect);

void DefineRect2(CRect *pRect, CRect *pRect2, int height);

void DefineRect3(CRect *pRect, CRect *pRect2);

void DefinePointForRoundRect(CRect *pRect,
	Point *pPointForLine, Point *pPointForArc, int radius);

#ifdef _GDIPLUS
bool LoadImageFromFile(Image **ppImage, CString fileName);

void DrawString(Graphics *pGraphics, CString text, CZFont *pFont, RectF rect,
	StringAlignment alignmentHorizontal = StringAlignmentCenter,
	StringAlignment alignmentVertical = StringAlignmentCenter);

void DrawLine(Graphics *pGraphics, CRect *pRect, COLORREF color, int width);

void RoundRectInGraphicsPath(GraphicsPath *pGraphPath,
	CRect *pRect, int radius);

void RoundRectInGraphicsPathLeft(GraphicsPath *pGraphPath,
	CRect *pRect, int radius);

void RoundRectInGraphicsPathDown(GraphicsPath *pGraphPath,
	CRect *pRect, int radius);

void FillRoundRectSolid(Graphics *pGraphics, CRect *pRect,
	int radius, COLORREF color, GraphicsPath *pGraphicsPath = NULL);

void FillRoundRectSolidTwo(Graphics *pGraphics, CRect *pRect,
	int radius, COLORREF color, GraphicsPath *pGraphicsPath = NULL);

void FillRoundRectLinearGradient(Graphics *pGraphics, CRect *pRect,
	int radius, COLORREF colorStart, COLORREF colorEnd,
	LinearGradientMode gradientMode, GraphicsPath *pGraphicsPath = NULL);

void FillRoundRectLinGradLeft(Graphics *pGraphics, CRect *pRect,
	int radius, COLORREF colorStart, COLORREF colorEnd,
	LinearGradientMode gradientMode, GraphicsPath *pGraphicsPath = NULL);

void FillRoundRectLinGradDown(Graphics *pGraphics, CRect *pRect,
	int radius, COLORREF colorStart, COLORREF colorEnd,
	LinearGradientMode gradientMode, GraphicsPath *pGraphicsPath = NULL);
//
void DrawRadiusRect(Graphics& graphics, Pen* pen, float X, float Y, float width, float height, float radius);
//
void FillRectSolid(Graphics *pGraphics, CRect *pRect, COLORREF color);

void FillRectLinearGradient(Graphics *pGraphics, CRect *pRect,
	COLORREF colorStart, COLORREF colorEnd, LinearGradientMode gradientMode);
#else

GLuint LoadGLTextures(CString FileName, int *size, int number);

GLuint LoadGLNormalMap(CString FileName, int *size, long number);

AUX_RGBImageRec *LoadBMP(char *Filename);

void FillRectSolid(CRect *pRect, float Z, COLORREF color);

void FillTriangleSolid(CRect *pRect, float Z, COLORREF color);

void FillRoundRectSolid(CRect *pRect, float Z, int radius,
	COLORREF color);

void FillRoundRectLinearGradient(CRect *pRect, float Z,
	int radius, COLORREF colorStart, COLORREF colorEnd);

void FillRectLinearGradient(CRect *pRect, float Z,
	COLORREF colorStart, COLORREF colorEnd);
#endif

bool ColorCompare(COLORREF color1, COLORREF color2, double threshold);

COLORREF DarkerColor(COLORREF clrColor, int iPercent);

COLORREF LighterColor(COLORREF clrColor, int iPercent);

COLORREF BrightnessChange(COLORREF color, int iPercent);

int ChangeColorComponent(int comp, int factor);

void FillParallepiped(CRect *pRect, float Z_up, float Z_down, COLORREF color);

enum eButtonType { Fixed, NotFixed, CheckBox, RadioButton, ComboboxMenuButton, ChanSelectorMenuButton, ChanSelectorScroll }; //типы кнопок. ComboboxMenuButton - кнопки для  меню Combobox. ChanSelectorMenuButton - для "выбора канала"
																				
//double Round(double val, int i);
//------------------------------------------------------------------------------