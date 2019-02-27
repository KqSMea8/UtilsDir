#pragma once

// Сформированные компьютером классы оболочки IDispatch, созданные при помощи Microsoft Visual C++

// ПРИМЕЧАНИЕ. Не изменяйте содержимое этого класса.  Если этот класс будет повторно создан при помощи
//  Microsoft Visual C++, ваши изменения будут перезаписаны.

/////////////////////////////////////////////////////////////////////////////
// Класс оболочки CHorizontalSlider

class CHorizontalSlider : public CWnd
{
protected:
	DECLARE_DYNCREATE(CHorizontalSlider)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x4106A107, 0xCBAA, 0x42E6, { 0x9A, 0xD7, 0xFA, 0xC7, 0x58, 0x2D, 0x94, 0xD5 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// Атрибуты
public:
enum
{
    TEXT_STYLE_REGULAR = 0,
    TEXT_STYLE_BOLD = 1,
    TEXT_STYLE_ITALIC = 2,
    TEXT_STYLE_BOLD_ITALIC = 3,
    TEXT_STYLE_UNDERLINE = 4,
    TEXT_STYLE_STRIKEOUT = 8
}TEXTSTYLE;
enum
{
    CURSOR_FORM_ARROW = 0,
    CURSOR_FORM_ELLIPSE = 1,
    CURSOR_FORM_RECTANGLE = 2,
    CURSOR_FORM_TRIANGLE = 3
}CURSORFORM;
enum
{
    LINEAR_POSITION_BOTTOM = 0,
    LINEAR_POSITION_TOP = 1
}LINEARPOSITION;
enum
{
    LINEAR_SCALE = 0,
    LOGARIPHM_SCALE = 1
}SCALETYPE;


// Операции
public:

// _DHorizontalSlider

// Functions
//

	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

// Properties
//

unsigned long GetBackgroundColor()
{
	unsigned long result;
	GetProperty(0x1, VT_UI4, (void*)&result);
	return result;
}
void SetBackgroundColor(unsigned long propVal)
{
	SetProperty(0x1, VT_UI4, propVal);
}
unsigned long GetTextColor()
{
	unsigned long result;
	GetProperty(0x2, VT_UI4, (void*)&result);
	return result;
}
void SetTextColor(unsigned long propVal)
{
	SetProperty(0x2, VT_UI4, propVal);
}
unsigned long GetLineGradientSideColor()
{
	unsigned long result;
	GetProperty(0x3, VT_UI4, (void*)&result);
	return result;
}
void SetLineGradientSideColor(unsigned long propVal)
{
	SetProperty(0x3, VT_UI4, propVal);
}
unsigned long GetLineGradientCenterColor()
{
	unsigned long result;
	GetProperty(0x4, VT_UI4, (void*)&result);
	return result;
}
void SetLineGradientCenterColor(unsigned long propVal)
{
	SetProperty(0x4, VT_UI4, propVal);
}
unsigned long GetEndRectGradientSideColor()
{
	unsigned long result;
	GetProperty(0x5, VT_UI4, (void*)&result);
	return result;
}
void SetEndRectGradientSideColor(unsigned long propVal)
{
	SetProperty(0x5, VT_UI4, propVal);
}
unsigned long GetEndRectGradientCenterColor()
{
	unsigned long result;
	GetProperty(0x6, VT_UI4, (void*)&result);
	return result;
}
void SetEndRectGradientCenterColor(unsigned long propVal)
{
	SetProperty(0x6, VT_UI4, propVal);
}
unsigned long GetCursorGradientSideColor()
{
	unsigned long result;
	GetProperty(0x7, VT_UI4, (void*)&result);
	return result;
}
void SetCursorGradientSideColor(unsigned long propVal)
{
	SetProperty(0x7, VT_UI4, propVal);
}
unsigned long GetCursorGradientCenterColor()
{
	unsigned long result;
	GetProperty(0x8, VT_UI4, (void*)&result);
	return result;
}
void SetCursorGradientCenterColor(unsigned long propVal)
{
	SetProperty(0x8, VT_UI4, propVal);
}
long GetFontSize()
{
	long result;
	GetProperty(0x9, VT_I4, (void*)&result);
	return result;
}
void SetFontSize(long propVal)
{
	SetProperty(0x9, VT_I4, propVal);
}
CString GetFontName()
{
	CString result;
	GetProperty(0xa, VT_BSTR, (void*)&result);
	return result;
}
void SetFontName(CString propVal)
{
	SetProperty(0xa, VT_BSTR, propVal);
}
long GetTEXTSTYLE()
{
	long result;
	GetProperty(0xb, VT_I4, (void*)&result);
	return result;
}
void SetTEXTSTYLE(long propVal)
{
	SetProperty(0xb, VT_I4, propVal);
}
double GetStartValue()
{
	double result;
	GetProperty(0xc, VT_R8, (void*)&result);
	return result;
}
void SetStartValue(double propVal)
{
	SetProperty(0xc, VT_R8, propVal);
}
double GetEndValue()
{
	double result;
	GetProperty(0xd, VT_R8, (void*)&result);
	return result;
}
void SetEndValue(double propVal)
{
	SetProperty(0xd, VT_R8, propVal);
}
double GetCurrentValue()
{
	double result;
	GetProperty(0xe, VT_R8, (void*)&result);
	return result;
}
void SetCurrentValue(double propVal)
{
	SetProperty(0xe, VT_R8, propVal);
}
BOOL GetCurrentValueVisibility()
{
	BOOL result;
	GetProperty(0xf, VT_BOOL, (void*)&result);
	return result;
}
void SetCurrentValueVisibility(BOOL propVal)
{
	SetProperty(0xf, VT_BOOL, propVal);
}
BOOL GetUnderlineVisibility()
{
	BOOL result;
	GetProperty(0x10, VT_BOOL, (void*)&result);
	return result;
}
void SetUnderlineVisibility(BOOL propVal)
{
	SetProperty(0x10, VT_BOOL, propVal);
}
BOOL GetLinearVisibility()
{
	BOOL result;
	GetProperty(0x11, VT_BOOL, (void*)&result);
	return result;
}
void SetLinearVisibility(BOOL propVal)
{
	SetProperty(0x11, VT_BOOL, propVal);
}
long GetEndRectWidth()
{
	long result;
	GetProperty(0x12, VT_I4, (void*)&result);
	return result;
}
void SetEndRectWidth(long propVal)
{
	SetProperty(0x12, VT_I4, propVal);
}
long GetCursorWidth()
{
	long result;
	GetProperty(0x13, VT_I4, (void*)&result);
	return result;
}
void SetCursorWidth(long propVal)
{
	SetProperty(0x13, VT_I4, propVal);
}
long GetCURSORFORM()
{
	long result;
	GetProperty(0x14, VT_I4, (void*)&result);
	return result;
}
void SetCURSORFORM(long propVal)
{
	SetProperty(0x14, VT_I4, propVal);
}
long GetLINEARPOSITION()
{
	long result;
	GetProperty(0x15, VT_I4, (void*)&result);
	return result;
}
void SetLINEARPOSITION(long propVal)
{
	SetProperty(0x15, VT_I4, propVal);
}
double GetCursorStep()
{
	double result;
	GetProperty(0x16, VT_R8, (void*)&result);
	return result;
}
void SetCursorStep(double propVal)
{
	SetProperty(0x16, VT_R8, propVal);
}
BOOL GetEnable()
{
	BOOL result;
	GetProperty(0x17, VT_BOOL, (void*)&result);
	return result;
}
void SetEnable(BOOL propVal)
{
	SetProperty(0x17, VT_BOOL, propVal);
}
long GetScale()
{
	long result;
	GetProperty(0x18, VT_I4, (void*)&result);
	return result;
}
void SetScale(long propVal)
{
	SetProperty(0x18, VT_I4, propVal);
}


};
