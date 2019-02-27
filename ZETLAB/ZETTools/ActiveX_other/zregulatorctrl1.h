#pragma once

// �������������� ����������� ������ �������� IDispatch, ��������� ��� ������ Microsoft Visual C++

// ����������. �� ��������� ���������� ����� ������. ���� ���� ����� ����� �������� ������ ��� ������
//  Microsoft Visual C++, ���� ��������� ����� ������������.

/////////////////////////////////////////////////////////////////////////////
// ����� �������� CZregulatorctrl1

class CZregulatorctrl1 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CZregulatorctrl1)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x84058765, 0x60A7, 0x4EB0, { 0x85, 0xC5, 0xE4, 0x8, 0xDE, 0xD6, 0x1D, 0x97 } };
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

// ��������
public:


// ��������
public:

// _DZRegulator

// Functions
//

	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Reset()
	{
		InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void AddString(LPCTSTR sItem)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, sItem);
	}
	long EnaWindow(long enable)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_I4, (void*)&result, parms, enable);
		return result;
	}
	void SetMinStringLength(short Value)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Value);
	}
	short GetMinStringLength()
	{
		short result;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
		return result;
	}

// Properties
//

BOOL GetBevelRight()
{
	BOOL result;
	GetProperty(0x3, VT_BOOL, (void*)&result);
	return result;
}
void SetBevelRight(BOOL propVal)
{
	SetProperty(0x3, VT_BOOL, propVal);
}
BOOL GetBevelLeft()
{
	BOOL result;
	GetProperty(0x4, VT_BOOL, (void*)&result);
	return result;
}
void SetBevelLeft(BOOL propVal)
{
	SetProperty(0x4, VT_BOOL, propVal);
}
BOOL GetBevelUp()
{
	BOOL result;
	GetProperty(0x5, VT_BOOL, (void*)&result);
	return result;
}
void SetBevelUp(BOOL propVal)
{
	SetProperty(0x5, VT_BOOL, propVal);
}
BOOL GetBevelDown()
{
	BOOL result;
	GetProperty(0x6, VT_BOOL, (void*)&result);
	return result;
}
void SetBevelDown(BOOL propVal)
{
	SetProperty(0x6, VT_BOOL, propVal);
}
unsigned long GetClrBgr()
{
	unsigned long result;
	GetProperty(0x7, VT_UI4, (void*)&result);
	return result;
}
void SetClrBgr(unsigned long propVal)
{
	SetProperty(0x7, VT_UI4, propVal);
}
unsigned long GetClrText()
{
	unsigned long result;
	GetProperty(0x8, VT_UI4, (void*)&result);
	return result;
}
void SetClrText(unsigned long propVal)
{
	SetProperty(0x8, VT_UI4, propVal);
}
BOOL GetBold()
{
	BOOL result;
	GetProperty(0x9, VT_BOOL, (void*)&result);
	return result;
}
void SetBold(BOOL propVal)
{
	SetProperty(0x9, VT_BOOL, propVal);
}
BOOL GetItalic()
{
	BOOL result;
	GetProperty(0xa, VT_BOOL, (void*)&result);
	return result;
}
void SetItalic(BOOL propVal)
{
	SetProperty(0xa, VT_BOOL, propVal);
}
CString GetFontFace()
{
	CString result;
	GetProperty(0xb, VT_BSTR, (void*)&result);
	return result;
}
void SetFontFace(CString propVal)
{
	SetProperty(0xb, VT_BSTR, propVal);
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
CString GetValue()
{
	CString result;
	GetProperty(0xd, VT_BSTR, (void*)&result);
	return result;
}
void SetValue(CString propVal)
{
	SetProperty(0xd, VT_BSTR, propVal);
}
long GetStringCount()
{
	long result;
	GetProperty(0xe, VT_I4, (void*)&result);
	return result;
}
void SetStringCount(long propVal)
{
	SetProperty(0xe, VT_I4, propVal);
}


};
