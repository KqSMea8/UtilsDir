#pragma once

// Сформированные компьютером классы оболочки IDispatch, созданные при помощи Microsoft Visual C++

// ПРИМЕЧАНИЕ. Не изменяйте содержимое этого класса. Если этот класс будет повторно создан при помощи
//  Microsoft Visual C++, ваши изменения будут перезаписаны.

/////////////////////////////////////////////////////////////////////////////
// Класс оболочки CZbuttonctrl1

class CZbuttonctrl1 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CZbuttonctrl1)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xFBED57CD, 0xABB4, 0x4140, { 0x9C, 0xC9, 0xCB, 0xA3, 0x20, 0xDE, 0x9C, 0x7B } };
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
    False = 0,
    True = 1
}MyBool;
enum
{
    NotFixing = 0,
    Fixing = 1
}PressStyle;


// Операции
public:

// _DZButton

// Functions
//

	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void AddString(LPCTSTR sValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, NULL, parms, sValue);
	}
	void Reset()
	{
		InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void SetMinStringLength(short Value)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Value);
	}
	short GetMinStringLength()
	{
		short result;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
		return result;
	}

// Properties
//

unsigned long GetClrText()
{
	unsigned long result;
	GetProperty(0x1, VT_UI4, (void*)&result);
	return result;
}
void SetClrText(unsigned long propVal)
{
	SetProperty(0x1, VT_UI4, propVal);
}
unsigned long GetClrBgr()
{
	unsigned long result;
	GetProperty(0x2, VT_UI4, (void*)&result);
	return result;
}
void SetClrBgr(unsigned long propVal)
{
	SetProperty(0x2, VT_UI4, propVal);
}
unsigned long GetClrPressed()
{
	unsigned long result;
	GetProperty(0x3, VT_UI4, (void*)&result);
	return result;
}
void SetClrPressed(unsigned long propVal)
{
	SetProperty(0x3, VT_UI4, propVal);
}
long GetBold()
{
	long result;
	GetProperty(0x6, VT_I4, (void*)&result);
	return result;
}
void SetBold(long propVal)
{
	SetProperty(0x6, VT_I4, propVal);
}
long GetItalic()
{
	long result;
	GetProperty(0x7, VT_I4, (void*)&result);
	return result;
}
void SetItalic(long propVal)
{
	SetProperty(0x7, VT_I4, propVal);
}
CString GetFontFace()
{
	CString result;
	GetProperty(0x8, VT_BSTR, (void*)&result);
	return result;
}
void SetFontFace(CString propVal)
{
	SetProperty(0x8, VT_BSTR, propVal);
}
long GetFlatButton()
{
	long result;
	GetProperty(0x9, VT_I4, (void*)&result);
	return result;
}
void SetFlatButton(long propVal)
{
	SetProperty(0x9, VT_I4, propVal);
}
CString GetCaption()
{
	CString result;
	GetProperty(DISPID_CAPTION, VT_BSTR, (void*)&result);
	return result;
}
void SetCaption(CString propVal)
{
	SetProperty(DISPID_CAPTION, VT_BSTR, propVal);
}
long GetDown()
{
	long result;
	GetProperty(0xa, VT_I4, (void*)&result);
	return result;
}
void SetDown(long propVal)
{
	SetProperty(0xa, VT_I4, propVal);
}
long GetPressStyle()
{
	long result;
	GetProperty(0xb, VT_I4, (void*)&result);
	return result;
}
void SetPressStyle(long propVal)
{
	SetProperty(0xb, VT_I4, propVal);
}
long GetItemIndex()
{
	long result;
	GetProperty(0xc, VT_I4, (void*)&result);
	return result;
}
void SetItemIndex(long propVal)
{
	SetProperty(0xc, VT_I4, propVal);
}


};
