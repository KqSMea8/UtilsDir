#pragma once

// Сформированные компьютером классы оболочки IDispatch, созданные при помощи Microsoft Visual C++

// ПРИМЕЧАНИЕ. Не изменяйте содержимое этого класса. Если этот класс будет повторно создан при помощи
//  Microsoft Visual C++, ваши изменения будут перезаписаны.

/////////////////////////////////////////////////////////////////////////////
// Класс оболочки CPolarctrl1

class CPolarctrl1 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CPolarctrl1)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xBD059969, 0x690A, 0x4F19, { 0x84, 0xD, 0x52, 0x72, 0xEB, 0xB2, 0xFF, 0x43 } };
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

// _DPolar

// Functions
//

	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Reset()
	{
		InvokeHelper(0x11, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long FormatAmpl(LPCTSTR fmt)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1d, DISPATCH_METHOD, VT_I4, (void*)&result, parms, fmt);
		return result;
	}
	long FormatUgol(LPCTSTR fmt)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1e, DISPATCH_METHOD, VT_I4, (void*)&result, parms, fmt);
		return result;
	}
	long FormatCap(LPCTSTR fmt)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1f, DISPATCH_METHOD, VT_I4, (void*)&result, parms, fmt);
		return result;
	}
	long Mpaint(float * bf)
	{
		long result;
		static BYTE parms[] = VTS_PR4 ;
		InvokeHelper(0x20, DISPATCH_METHOD, VT_I4, (void*)&result, parms, bf);
		return result;
	}
	short Display()
	{
		short result;
		InvokeHelper(0x21, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
		return result;
	}
	void put_ChhTextPol(LPCTSTR ch_text, long num)
	{
		static BYTE parms[] = VTS_BSTR VTS_I4 ;
		InvokeHelper(0x23, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ch_text, num);
	}
	void put_cht(LPCTSTR text, long num)
	{
		static BYTE parms[] = VTS_BSTR VTS_I4 ;
		InvokeHelper(0x24, DISPATCH_METHOD, VT_EMPTY, NULL, parms, text, num);
	}
	short DrawGear(long num, long vkl)
	{
		short result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x25, DISPATCH_METHOD, VT_I2, (void*)&result, parms, num, vkl);
		return result;
	}
	void PutPictureToClipboard()
	{
		InvokeHelper(0x28, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void PutDataToClipboard()
	{
		InvokeHelper(0x29, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

// Properties
//

float Getstartu()
{
	float result;
	GetProperty(0x3, VT_R4, (void*)&result);
	return result;
}
void Setstartu(float propVal)
{
	SetProperty(0x3, VT_R4, propVal);
}
float Getendu()
{
	float result;
	GetProperty(0x4, VT_R4, (void*)&result);
	return result;
}
void Setendu(float propVal)
{
	SetProperty(0x4, VT_R4, propVal);
}
long Getmysize()
{
	long result;
	GetProperty(0x6, VT_I4, (void*)&result);
	return result;
}
void Setmysize(long propVal)
{
	SetProperty(0x6, VT_I4, propVal);
}
long Getdegrad()
{
	long result;
	GetProperty(0x8, VT_I4, (void*)&result);
	return result;
}
void Setdegrad(long propVal)
{
	SetProperty(0x8, VT_I4, propVal);
}
long Getpochas()
{
	long result;
	GetProperty(0x9, VT_I4, (void*)&result);
	return result;
}
void Setpochas(long propVal)
{
	SetProperty(0x9, VT_I4, propVal);
}
long Getind()
{
	long result;
	GetProperty(0xa, VT_I4, (void*)&result);
	return result;
}
void Setind(long propVal)
{
	SetProperty(0xa, VT_I4, propVal);
}
unsigned long GetCtrlFon()
{
	unsigned long result;
	GetProperty(0xb, VT_UI4, (void*)&result);
	return result;
}
void SetCtrlFon(unsigned long propVal)
{
	SetProperty(0xb, VT_UI4, propVal);
}
float GetMathDA()
{
	float result;
	GetProperty(0xc, VT_R4, (void*)&result);
	return result;
}
void SetMathDA(float propVal)
{
	SetProperty(0xc, VT_R4, propVal);
}
float GetMathLA()
{
	float result;
	GetProperty(0xd, VT_R4, (void*)&result);
	return result;
}
void SetMathLA(float propVal)
{
	SetProperty(0xd, VT_R4, propVal);
}
float GetUporAVerh()
{
	float result;
	GetProperty(0xe, VT_R4, (void*)&result);
	return result;
}
void SetUporAVerh(float propVal)
{
	SetProperty(0xe, VT_R4, propVal);
}
float GetUporANis()
{
	float result;
	GetProperty(0xf, VT_R4, (void*)&result);
	return result;
}
void SetUporANis(float propVal)
{
	SetProperty(0xf, VT_R4, propVal);
}
float GetUporDelta()
{
	float result;
	GetProperty(0x10, VT_R4, (void*)&result);
	return result;
}
void SetUporDelta(float propVal)
{
	SetProperty(0x10, VT_R4, propVal);
}
unsigned long GetClrNum()
{
	unsigned long result;
	GetProperty(0x12, VT_UI4, (void*)&result);
	return result;
}
void SetClrNum(unsigned long propVal)
{
	SetProperty(0x12, VT_UI4, propVal);
}
unsigned long GetClrCur()
{
	unsigned long result;
	GetProperty(0x13, VT_UI4, (void*)&result);
	return result;
}
void SetClrCur(unsigned long propVal)
{
	SetProperty(0x13, VT_UI4, propVal);
}
unsigned long GetClrOsi()
{
	unsigned long result;
	GetProperty(0x14, VT_UI4, (void*)&result);
	return result;
}
void SetClrOsi(unsigned long propVal)
{
	SetProperty(0x14, VT_UI4, propVal);
}
float GetZero()
{
	float result;
	GetProperty(0x15, VT_R4, (void*)&result);
	return result;
}
void SetZero(float propVal)
{
	SetProperty(0x15, VT_R4, propVal);
}
long Getpol()
{
	long result;
	GetProperty(0x17, VT_I4, (void*)&result);
	return result;
}
void Setpol(long propVal)
{
	SetProperty(0x17, VT_I4, propVal);
}
long GetNumber()
{
	long result;
	GetProperty(0x1b, VT_I4, (void*)&result);
	return result;
}
void SetNumber(long propVal)
{
	SetProperty(0x1b, VT_I4, propVal);
}
unsigned long GetClrGrf()
{
	unsigned long result;
	GetProperty(0x1c, VT_UI4, (void*)&result);
	return result;
}
void SetClrGrf(unsigned long propVal)
{
	SetProperty(0x1c, VT_UI4, propVal);
}
unsigned short GetLineWidth()
{
	unsigned short result;
	GetProperty(0x26, VT_UI2, (void*)&result);
	return result;
}
void SetLineWidth(unsigned short propVal)
{
	SetProperty(0x26, VT_UI2, propVal);
}
unsigned long GetClrGear()
{
	unsigned long result;
	GetProperty(0x27, VT_UI4, (void*)&result);
	return result;
}
void SetClrGear(unsigned long propVal)
{
	SetProperty(0x27, VT_UI4, propVal);
}


};
