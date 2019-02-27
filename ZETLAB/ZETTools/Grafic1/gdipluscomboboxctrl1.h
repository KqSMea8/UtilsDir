#pragma once

// Сформированные компьютером классы оболочки IDispatch, созданные при помощи Microsoft Visual C++

// ПРИМЕЧАНИЕ. Не изменяйте содержимое этого класса. Если этот класс будет повторно создан при помощи
//  Microsoft Visual C++, ваши изменения будут перезаписаны.

/////////////////////////////////////////////////////////////////////////////
// Класс оболочки CGdipluscomboboxctrl1

class CGdipluscomboboxctrl1 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CGdipluscomboboxctrl1)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xED6C9EA4, 0xB682, 0x4020, { 0x8A, 0xEB, 0x3E, 0xFB, 0x25, 0x2F, 0x1C, 0x2A } };
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

// _DGdiPlusComboBox

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
CString GetCaption()
{
	CString result;
	GetProperty(0xa, VT_BSTR, (void*)&result);
	return result;
}
void SetCaption(CString propVal)
{
	SetProperty(0xa, VT_BSTR, propVal);
}


};
