#pragma once

// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CGridctrl1 wrapper class

class CGridctrl1 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CGridctrl1)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x735C6A09, 0x9364, 0x496D, { 0xA8, 0x7D, 0xDC, 0x70, 0x50, 0x68, 0x45, 0x31 } };
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

// Attributes
public:


// Operations
public:

// _DGrid

// Functions
//

	short Display()
	{
		short result;
		InvokeHelper(0x16, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
		return result;
	}
	long Paint(float * buffer)
	{
		long result;
		static BYTE parms[] = VTS_PR4 ;
		InvokeHelper(0x17, DISPATCH_METHOD, VT_I4, (void*)&result, parms, buffer);
		return result;
	}
	void Formatxy(LPCTSTR fmt)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x18, DISPATCH_METHOD, VT_EMPTY, NULL, parms, fmt);
	}
	void AltC(float * buffer)
	{
		static BYTE parms[] = VTS_PR4 ;
		InvokeHelper(0x19, DISPATCH_METHOD, VT_EMPTY, NULL, parms, buffer);
	}
	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void PushToClipBoard()
	{
		InvokeHelper(0x1d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long FormatX(LPCTSTR fmt)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1f, DISPATCH_METHOD, VT_I4, (void*)&result, parms, fmt);
		return result;
	}
	long FormatY(LPCTSTR fmt)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x20, DISPATCH_METHOD, VT_I4, (void*)&result, parms, fmt);
		return result;
	}
	long PaintNet(long pBuffer)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x22, DISPATCH_METHOD, VT_I4, (void*)&result, parms, pBuffer);
		return result;
	}
	void AltCNet(long pBuffer)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x23, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pBuffer);
	}
	void autoScaleY()
	{
		InvokeHelper(0x24, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void savePng(LPCTSTR FileName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x25, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName);
	}

// Properties
//

unsigned long GetBackColor()
{
	unsigned long result;
	GetProperty(DISPID_BACKCOLOR, VT_UI4, (void*)&result);
	return result;
}
void SetBackColor(unsigned long propVal)
{
	SetProperty(DISPID_BACKCOLOR, VT_UI4, propVal);
}
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
unsigned long GetClrGrf()
{
	unsigned long result;
	GetProperty(0x2, VT_UI4, (void*)&result);
	return result;
}
void SetClrGrf(unsigned long propVal)
{
	SetProperty(0x2, VT_UI4, propVal);
}
unsigned long GetClrGrd()
{
	unsigned long result;
	GetProperty(0x3, VT_UI4, (void*)&result);
	return result;
}
void SetClrGrd(unsigned long propVal)
{
	SetProperty(0x3, VT_UI4, propVal);
}
unsigned long GetClrCrs()
{
	unsigned long result;
	GetProperty(0x4, VT_UI4, (void*)&result);
	return result;
}
void SetClrCrs(unsigned long propVal)
{
	SetProperty(0x4, VT_UI4, propVal);
}
unsigned long GetClrDig()
{
	unsigned long result;
	GetProperty(0x5, VT_UI4, (void*)&result);
	return result;
}
void SetClrDig(unsigned long propVal)
{
	SetProperty(0x5, VT_UI4, propVal);
}
unsigned long GetClrLeg()
{
	unsigned long result;
	GetProperty(0x6, VT_UI4, (void*)&result);
	return result;
}
void SetClrLeg(unsigned long propVal)
{
	SetProperty(0x6, VT_UI4, propVal);
}
long GetSize()
{
	long result;
	GetProperty(0x7, VT_I4, (void*)&result);
	return result;
}
void SetSize(long propVal)
{
	SetProperty(0x7, VT_I4, propVal);
}
long GetNumber()
{
	long result;
	GetProperty(0x8, VT_I4, (void*)&result);
	return result;
}
void SetNumber(long propVal)
{
	SetProperty(0x8, VT_I4, propVal);
}
long GetTypeLine()
{
	long result;
	GetProperty(0x9, VT_I4, (void*)&result);
	return result;
}
void SetTypeLine(long propVal)
{
	SetProperty(0x9, VT_I4, propVal);
}
long GetTypeGrf()
{
	long result;
	GetProperty(0xa, VT_I4, (void*)&result);
	return result;
}
void SetTypeGrf(long propVal)
{
	SetProperty(0xa, VT_I4, propVal);
}
double GetXfirst()
{
	double result;
	GetProperty(0xb, VT_R8, (void*)&result);
	return result;
}
void SetXfirst(double propVal)
{
	SetProperty(0xb, VT_R8, propVal);
}
double GetXend()
{
	double result;
	GetProperty(0xc, VT_R8, (void*)&result);
	return result;
}
void SetXend(double propVal)
{
	SetProperty(0xc, VT_R8, propVal);
}
double GetMathlx()
{
	double result;
	GetProperty(0xd, VT_R8, (void*)&result);
	return result;
}
void SetMathlx(double propVal)
{
	SetProperty(0xd, VT_R8, propVal);
}
double GetMathdx()
{
	double result;
	GetProperty(0xe, VT_R8, (void*)&result);
	return result;
}
void SetMathdx(double propVal)
{
	SetProperty(0xe, VT_R8, propVal);
}
double GetMathly()
{
	double result;
	GetProperty(0xf, VT_R8, (void*)&result);
	return result;
}
void SetMathly(double propVal)
{
	SetProperty(0xf, VT_R8, propVal);
}
double GetMathdy()
{
	double result;
	GetProperty(0x10, VT_R8, (void*)&result);
	return result;
}
void SetMathdy(double propVal)
{
	SetProperty(0x10, VT_R8, propVal);
}
long GetPx()
{
	long result;
	GetProperty(0x11, VT_I4, (void*)&result);
	return result;
}
void SetPx(long propVal)
{
	SetProperty(0x11, VT_I4, propVal);
}
long GetInd()
{
	long result;
	GetProperty(0x12, VT_I4, (void*)&result);
	return result;
}
void SetInd(long propVal)
{
	SetProperty(0x12, VT_I4, propVal);
}
long Getlog()
{
	long result;
	GetProperty(0x13, VT_I4, (void*)&result);
	return result;
}
void Setlog(long propVal)
{
	SetProperty(0x13, VT_I4, propVal);
}
long GetValid()
{
	long result;
	GetProperty(0x14, VT_I4, (void*)&result);
	return result;
}
void SetValid(long propVal)
{
	SetProperty(0x14, VT_I4, propVal);
}
long GetTypeAbs()
{
	long result;
	GetProperty(0x15, VT_I4, (void*)&result);
	return result;
}
void SetTypeAbs(long propVal)
{
	SetProperty(0x15, VT_I4, propVal);
}
long GetMakeUpor()
{
	long result;
	GetProperty(0x1a, VT_I4, (void*)&result);
	return result;
}
void SetMakeUpor(long propVal)
{
	SetProperty(0x1a, VT_I4, propVal);
}
double GetUporVerh()
{
	double result;
	GetProperty(0x1b, VT_R8, (void*)&result);
	return result;
}
void SetUporVerh(double propVal)
{
	SetProperty(0x1b, VT_R8, propVal);
}
double GetUporNis()
{
	double result;
	GetProperty(0x1c, VT_R8, (void*)&result);
	return result;
}
void SetUporNis(double propVal)
{
	SetProperty(0x1c, VT_R8, propVal);
}
long GetNumVisiblePoints()
{
	long result;
	GetProperty(0x1e, VT_I4, (void*)&result);
	return result;
}
void SetNumVisiblePoints(long propVal)
{
	SetProperty(0x1e, VT_I4, propVal);
}
double GetUporDelta()
{
	double result;
	GetProperty(0x21, VT_R8, (void*)&result);
	return result;
}
void SetUporDelta(double propVal)
{
	SetProperty(0x21, VT_R8, propVal);
}
unsigned char Getluminescence()
{
	unsigned char result;
	GetProperty(0x26, VT_UI1, (void*)&result);
	return result;
}
void Setluminescence(unsigned char propVal)
{
	SetProperty(0x26, VT_UI1, propVal);
}
BOOL GetmarkPlainData()
{
	BOOL result;
	GetProperty(0x27, VT_BOOL, (void*)&result);
	return result;
}
void SetmarkPlainData(BOOL propVal)
{
	SetProperty(0x27, VT_BOOL, propVal);
}

CString GetTextYAxis()
{
	CString result;
	GetProperty(0x29, VT_BSTR, (void*)&result);
	return result;
}
void SetTextYAxis(CString propVal)
{
	SetProperty(0x29, VT_BSTR, propVal);
}
CString GetTextXAxis()
{
	CString result;
	GetProperty(0x28, VT_BSTR, (void*)&result);
	return result;
}
void SetTextXAxis(CString propVal)
{
	SetProperty(0x28, VT_BSTR, propVal);
}


};