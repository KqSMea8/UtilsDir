#pragma once

// Сформированные компьютером классы оболочки IDispatch, созданные при помощи Microsoft Visual C++

// ПРИМЕЧАНИЕ. Не изменяйте содержимое этого класса. Если этот класс будет повторно создан при помощи
//  Microsoft Visual C++, ваши изменения будут перезаписаны.

/////////////////////////////////////////////////////////////////////////////
// Класс оболочки CChanselector1

class CChanselector1 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CChanselector1)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xD4CE5E4E, 0xCA, 0x4FF1, { 0x94, 0xC0, 0xF7, 0xAD, 0xE, 0xC6, 0xBA, 0xB8 } };
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

// _DZETChanSelector

// Functions
//

	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

// Properties
//

long GetChannel()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}
void SetChannel(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}
long GetType()
{
	long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}
void SetType(long propVal)
{
	SetProperty(0x2, VT_I4, propVal);
}
BOOL GetFilter()
{
	BOOL result;
	GetProperty(0x3, VT_BOOL, (void*)&result);
	return result;
}
void SetFilter(BOOL propVal)
{
	SetProperty(0x3, VT_BOOL, propVal);
}
CString GetConversionFilter()
{
	CString result(L"");
	GetProperty(0x4, VT_BSTR, (void*)&result);
	return result;
}
void SetConversionFilter(CString propVal)
{
	SetProperty(0x4, VT_BSTR, propVal);
}
CString GetToolTipText()
{
	CString result(L"");
	GetProperty(0x5, VT_BSTR, (void*)&result);
	return result;
}
void SetToolTipText(CString propVal)
{
	SetProperty(0x5, VT_BSTR, propVal);
}
BOOL GetZETLabStyle()
{
	BOOL result;
	GetProperty(0x6, VT_BOOL, (void*)&result);
	return result;
}
void SetZETLabStyle(BOOL propVal)
{
	SetProperty(0x6, VT_BOOL, propVal);
}
unsigned long GetColorBack()
{
	unsigned long result;
	GetProperty(0x7, VT_UI4, (void*)&result);
	return result;
}
void SetColorBack(unsigned long propVal)
{
	SetProperty(0x7, VT_UI4, propVal);
}
unsigned long GetColorText()
{
	unsigned long result;
	GetProperty(0x8, VT_UI4, (void*)&result);
	return result;
}
void SetColorText(unsigned long propVal)
{
	SetProperty(0x8, VT_UI4, propVal);
}
BOOL GetLeftBound()
{
	BOOL result;
	GetProperty(0x9, VT_BOOL, (void*)&result);
	return result;
}
void SetLeftBound(BOOL propVal)
{
	SetProperty(0x9, VT_BOOL, propVal);
}
BOOL GetRightBound()
{
	BOOL result;
	GetProperty(0xa, VT_BOOL, (void*)&result);
	return result;
}
void SetRightBound(BOOL propVal)
{
	SetProperty(0xa, VT_BOOL, propVal);
}
BOOL GetTopBound()
{
	BOOL result;
	GetProperty(0xb, VT_BOOL, (void*)&result);
	return result;
}
void SetTopBound(BOOL propVal)
{
	SetProperty(0xb, VT_BOOL, propVal);
}
BOOL GetBottomBound()
{
	BOOL result;
	GetProperty(0xc, VT_BOOL, (void*)&result);
	return result;
}
void SetBottomBound(BOOL propVal)
{
	SetProperty(0xc, VT_BOOL, propVal);
}
CString GetTextString()
{
	CString result(L"");
	GetProperty(0xd, VT_BSTR, (void*)&result);
	return result;
}
void SetTextString(CString propVal)
{
	SetProperty(0xd, VT_BSTR, propVal);
}
CString GetCaption()
{
	CString result(L"");
	GetProperty(0x10, VT_BSTR, (void*)&result);
	return result;
}
void SetCaption(CString propVal)
{
	SetProperty(0x10, VT_BSTR, propVal);
}


};
