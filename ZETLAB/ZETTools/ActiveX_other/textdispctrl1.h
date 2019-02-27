#pragma once

// Сформированные компьютером классы оболочки IDispatch, созданные при помощи Microsoft Visual C++

// ПРИМЕЧАНИЕ. Не изменяйте содержимое этого класса. Если этот класс будет повторно создан при помощи
//  Microsoft Visual C++, ваши изменения будут перезаписаны.

/////////////////////////////////////////////////////////////////////////////
// Класс оболочки CTextdispctrl1

class CTextdispctrl1 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CTextdispctrl1)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x608E17F6, 0x3EFB, 0x4E5F, { 0x92, 0x86, 0x2A, 0x5E, 0x78, 0x59, 0xA7, 0xF } };
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

// _DTextDisp

// Functions
//

	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long Text(BSTR * stroka)
	{
		long result;
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_I4, (void*)&result, parms, stroka);
		return result;
	}
	unsigned char SetText(LPCTSTR Text)
	{
		unsigned char result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_UI1, (void*)&result, parms, Text);
		return result;
	}
	void SetMinTextLength(short Value)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Value);
	}
	short GetMinTextLength()
	{
		short result;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
		return result;
	}

// Properties
//

unsigned long GetClrFon()
{
	unsigned long result;
	GetProperty(0x1, VT_UI4, (void*)&result);
	return result;
}
void SetClrFon(unsigned long propVal)
{
	SetProperty(0x1, VT_UI4, propVal);
}
unsigned long GetClrDig()
{
	unsigned long result;
	GetProperty(0x2, VT_UI4, (void*)&result);
	return result;
}
void SetClrDig(unsigned long propVal)
{
	SetProperty(0x2, VT_UI4, propVal);
}
long GetRup()
{
	long result;
	GetProperty(0x3, VT_I4, (void*)&result);
	return result;
}
void SetRup(long propVal)
{
	SetProperty(0x3, VT_I4, propVal);
}
long GetRdown()
{
	long result;
	GetProperty(0x4, VT_I4, (void*)&result);
	return result;
}
void SetRdown(long propVal)
{
	SetProperty(0x4, VT_I4, propVal);
}
long GetRleft()
{
	long result;
	GetProperty(0x5, VT_I4, (void*)&result);
	return result;
}
void SetRleft(long propVal)
{
	SetProperty(0x5, VT_I4, propVal);
}
long GetRright()
{
	long result;
	GetProperty(0x6, VT_I4, (void*)&result);
	return result;
}
void SetRright(long propVal)
{
	SetProperty(0x6, VT_I4, propVal);
}
long GetItallic()
{
	long result;
	GetProperty(0x7, VT_I4, (void*)&result);
	return result;
}
void SetItallic(long propVal)
{
	SetProperty(0x7, VT_I4, propVal);
}
long GetBold()
{
	long result;
	GetProperty(0x8, VT_I4, (void*)&result);
	return result;
}
void SetBold(long propVal)
{
	SetProperty(0x8, VT_I4, propVal);
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
CString GetBstrText()
{
	CString result;
	GetProperty(0xd, VT_BSTR, (void*)&result);
	return result;
}
void SetBstrText(CString propVal)
{
	SetProperty(0xd, VT_BSTR, propVal);
}


};
