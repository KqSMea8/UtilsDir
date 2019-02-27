#pragma once

// Сформированные компьютером классы оболочки IDispatch, созданные при помощи Microsoft Visual C++

// ПРИМЕЧАНИЕ. Не изменяйте содержимое этого класса.  Если этот класс будет повторно создан при помощи
//  Microsoft Visual C++, ваши изменения будут перезаписаны.

/////////////////////////////////////////////////////////////////////////////
// Класс оболочки CGdiplustextdispctrl1

class CGdiplustextdispctrl1 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CGdiplustextdispctrl1)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xCB3923C2, 0xB6F4, 0x4971, { 0xA0, 0x6F, 0x1E, 0x77, 0x1D, 0x4D, 0x0, 0xF9 } };
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


// Операции
public:

// _DGdiPlusTextDisp

// Functions
//

	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

// Properties
//

unsigned long GetColorBack()
{
	unsigned long result;
	GetProperty(0x1, VT_UI4, (void*)&result);
	return result;
}
void SetColorBack(unsigned long propVal)
{
	SetProperty(0x1, VT_UI4, propVal);
}
unsigned long GetColorText()
{
	unsigned long result;
	GetProperty(0x2, VT_UI4, (void*)&result);
	return result;
}
void SetColorText(unsigned long propVal)
{
	SetProperty(0x2, VT_UI4, propVal);
}
BOOL GetLeftBound()
{
	BOOL result;
	GetProperty(0x3, VT_BOOL, (void*)&result);
	return result;
}
void SetLeftBound(BOOL propVal)
{
	SetProperty(0x3, VT_BOOL, propVal);
}
BOOL GetRightBound()
{
	BOOL result;
	GetProperty(0x4, VT_BOOL, (void*)&result);
	return result;
}
void SetRightBound(BOOL propVal)
{
	SetProperty(0x4, VT_BOOL, propVal);
}
BOOL GetTopBound()
{
	BOOL result;
	GetProperty(0x5, VT_BOOL, (void*)&result);
	return result;
}
void SetTopBound(BOOL propVal)
{
	SetProperty(0x5, VT_BOOL, propVal);
}
BOOL GetBottomBound()
{
	BOOL result;
	GetProperty(0x6, VT_BOOL, (void*)&result);
	return result;
}
void SetBottomBound(BOOL propVal)
{
	SetProperty(0x6, VT_BOOL, propVal);
}
CString GetTextString()
{
	CString result;
	GetProperty(0x7, VT_BSTR, (void*)&result);
	return result;
}
void SetTextString(CString propVal)
{
	SetProperty(0x7, VT_BSTR, propVal);
}
long GetTextSize()
{
	long result;
	GetProperty(0x8, VT_I4, (void*)&result);
	return result;
}
void SetTextSize(long propVal)
{
	SetProperty(0x8, VT_I4, propVal);
}
long GetTextFontType()
{
	long result;
	GetProperty(0x9, VT_I4, (void*)&result);
	return result;
}
void SetTextFontType(long propVal)
{
	SetProperty(0x9, VT_I4, propVal);
}
long GetAlignString()
{
	long result;
	GetProperty(0xa, VT_I4, (void*)&result);
	return result;
}
void SetAlignString(long propVal)
{
	SetProperty(0xa, VT_I4, propVal);
}
BOOL GetSimpleBtn()
{
	BOOL result;
	GetProperty(0xb, VT_BOOL, (void*)&result);
	return result;
}
void SetSimpleBtn(BOOL propVal)
{
	SetProperty(0xb, VT_BOOL, propVal);
}
BOOL GetPositioning()
{
	BOOL result;
	GetProperty(0xc, VT_BOOL, (void*)&result);
	return result;
}
void SetPositioning(BOOL propVal)
{
	SetProperty(0xc, VT_BOOL, propVal);
}
long GetPrefixNumber()
{
	long result;
	GetProperty(0xd, VT_I4, (void*)&result);
	return result;
}
void SetPrefixNumber(long propVal)
{
	SetProperty(0xd, VT_I4, propVal);
}
long GetPostfixNumber()
{
	long result;
	GetProperty(0xe, VT_I4, (void*)&result);
	return result;
}
void SetPostfixNumber(long propVal)
{
	SetProperty(0xe, VT_I4, propVal);
}
CString GetTextFontName()
{
	CString result;
	GetProperty(0xf, VT_BSTR, (void*)&result);
	return result;
}
void SetTextFontName(CString propVal)
{
	SetProperty(0xf, VT_BSTR, propVal);
}


};
