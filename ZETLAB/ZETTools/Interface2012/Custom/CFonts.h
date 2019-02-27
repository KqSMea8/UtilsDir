//#include "stdafx.h"
#include <stdlib.h> 
//#include <windows.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
//#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/GLAux.h>
#include "ftgl.h"
#include "afxdisp.h"
#include "afxwin.h"
#include <atlstr.h>
//#include <Interface2012\Custom\FunctionForAll.h>
//------------------------------------------------------------------------------
enum FontsStyle
{
	FontsStyleRegular    = 0,
	FontsStyleBold       = 1,
	FontsStyleItalic     = 2,
	FontsStyleBoldItalic = 3,
	FontsStyleUnderline  = 4,
	FontsStyleStrikeout  = 8
};
//------------------------------------------------------------------------------
enum StringAlignmentOGL
{
	StrAlignmentNear   = 0,
	StrAlignmentCenter = 1,
	StrAlignmentFar    = 2
};
//------------------------------------------------------------------------------
struct ParamFonts
{// Структура параметров отображения
	char* Name;				//имя шрифта
	float Size;				//размер
	FontsStyle Style;		//стиль
	COLORREF Color;			//цвет
	bool MonoSpace;			//флаг моноширинного шрифта
	ParamFonts()
	{
		Name = "Calibri";
		Size = 12.f;
		Style = FontsStyleRegular;
		Color = RGB(255, 255, 255);
		MonoSpace = false;
	}
};
//------------------------------------------------------------------------------
class CDFonts
{
public:
	static int counter;
	CDFonts();
	CDFonts(ParamFonts paramFont);
	CDFonts(char* font_name, float h, FontsStyle style = FontsStyleRegular, COLORREF Color = RGB(255, 255, 255));
	virtual ~CDFonts();
private:
	ParamFonts m_param;
	FTFont* fonts;
	StringAlignmentOGL AligmentHorizontal, AligmentVertical;
	void Init();
	bool m_bSetCeils;
	CRect m_rect;
	char* scat (char *d, char *s);
	bool checkMonoSpace();
protected:
public:
	void DrawString(CRect rect, const wchar_t *text, StringAlignmentOGL AligmentHor, StringAlignmentOGL AligmentVert);
	void DrawString(CRect rect, const wchar_t *text);

	void DrawNumber(CRect rect, const wchar_t *text, StringAlignmentOGL AligmentHor, StringAlignmentOGL AligmentVert);
	void DrawNumber(CRect rect, const wchar_t *text);
	void DrawNumberOnCeils(CRect rect, const wchar_t *text);
	void FillRectSolid(CRect *pRect, float Z, COLORREF color);

	float GetSize() { return m_param.Size; }
	void SetSize(float newVal);

	char* GetName() { return m_param.Name; }
	void SetName(char* newVal);

	COLORREF GetColor() { return m_param.Color; }
	void SetColor(COLORREF newVal) { m_param.Color = newVal; }

	FontsStyle GetStyle() { return m_param.Style; }
	void SetStyle(FontsStyle newVal);

	StringAlignmentOGL GetAligmentHorizontal() { return AligmentHorizontal; }
	void SetAligmentHorizontal(StringAlignmentOGL newVal);

	void SetCeils (bool val) {m_bSetCeils = val; }
	bool GetCeils (void) { return m_bSetCeils; }

	StringAlignmentOGL GetAligmentVertical() { return AligmentVertical; }
	void SetAligmentVertical(StringAlignmentOGL newVal);
	void Clear (void);
	FTFont* GetFont() { return fonts; }

	CString CutString(CString val);
	float CalcSize(CRect rect, const wchar_t *text);
};
//------------------------------------------------------------------------------
