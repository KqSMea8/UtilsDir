#pragma once

// Сформированные компьютером классы оболочки IDispatch, созданные при помощи Microsoft Visual C++

// ПРИМЕЧАНИЕ. Не изменяйте содержимое этого класса. Если этот класс будет повторно создан при помощи
//  Microsoft Visual C++, ваши изменения будут перезаписаны.

/////////////////////////////////////////////////////////////////////////////
// Класс оболочки CGrammactrl1

class CGrammactrl1 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CGrammactrl1)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xB9AAE9F4, 0x60B0, 0x47D0, { 0xBC, 0x69, 0xD3, 0x16, 0x73, 0x89, 0x43, 0x34 } };
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

// _DGramma

// Functions
//

	void AltC(float * buffer)
	{
		static BYTE parms[] = VTS_PR4 ;
		InvokeHelper(0x17, DISPATCH_METHOD, VT_EMPTY, NULL, parms, buffer);
	}
	short Display()
	{
		short result;
		InvokeHelper(0x18, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
		return result;
	}
	void Formatxy(LPCTSTR fmt)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x19, DISPATCH_METHOD, VT_EMPTY, NULL, parms, fmt);
	}
	long Paint(float * buffer)
	{
		long result;
		static BYTE parms[] = VTS_PR4 ;
		InvokeHelper(0x1a, DISPATCH_METHOD, VT_I4, (void*)&result, parms, buffer);
		return result;
	}
	void Reset()
	{
		InvokeHelper(0x1b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long GetGLDataFromFile(LPCTSTR FileName)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1f, DISPATCH_METHOD, VT_I4, (void*)&result, parms, FileName);
		return result;
	}
	long SaveGLDataToFile(LPCTSTR FileName)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x20, DISPATCH_METHOD, VT_I4, (void*)&result, parms, FileName);
		return result;
	}
	void PushToClipBoard()
	{
		InvokeHelper(0x26, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	float GetPointAt(long X, long Y)
	{
		float result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x27, DISPATCH_METHOD, VT_R4, (void*)&result, parms, X, Y);
		return result;
	}
	long PaintNet(long pBuffer)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x28, DISPATCH_METHOD, VT_I4, (void*)&result, parms, pBuffer);
		return result;
	}
	void AltCNet(long pBuffer)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x29, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pBuffer);
	}

// Properties
//

long GetSize()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}
void SetSize(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}
long GetNumber()
{
	long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}
void SetNumber(long propVal)
{
	SetProperty(0x2, VT_I4, propVal);
}
unsigned long GetClrCrs()
{
	unsigned long result;
	GetProperty(0x3, VT_UI4, (void*)&result);
	return result;
}
void SetClrCrs(unsigned long propVal)
{
	SetProperty(0x3, VT_UI4, propVal);
}
unsigned long GetClrDig()
{
	unsigned long result;
	GetProperty(0x4, VT_UI4, (void*)&result);
	return result;
}
void SetClrDig(unsigned long propVal)
{
	SetProperty(0x4, VT_UI4, propVal);
}
unsigned long GetClrFon()
{
	unsigned long result;
	GetProperty(0x5, VT_UI4, (void*)&result);
	return result;
}
void SetClrFon(unsigned long propVal)
{
	SetProperty(0x5, VT_UI4, propVal);
}
unsigned long GetClrGrd()
{
	unsigned long result;
	GetProperty(0x6, VT_UI4, (void*)&result);
	return result;
}
void SetClrGrd(unsigned long propVal)
{
	SetProperty(0x6, VT_UI4, propVal);
}
unsigned long GetClrGrf()
{
	unsigned long result;
	GetProperty(0x7, VT_UI4, (void*)&result);
	return result;
}
void SetClrGrf(unsigned long propVal)
{
	SetProperty(0x7, VT_UI4, propVal);
}
unsigned long GetClrLeg()
{
	unsigned long result;
	GetProperty(0x8, VT_UI4, (void*)&result);
	return result;
}
void SetClrLeg(unsigned long propVal)
{
	SetProperty(0x8, VT_UI4, propVal);
}
double GetMathdx()
{
	double result;
	GetProperty(0x9, VT_R8, (void*)&result);
	return result;
}
void SetMathdx(double propVal)
{
	SetProperty(0x9, VT_R8, propVal);
}
double GetMathlx()
{
	double result;
	GetProperty(0xa, VT_R8, (void*)&result);
	return result;
}
void SetMathlx(double propVal)
{
	SetProperty(0xa, VT_R8, propVal);
}
double GetMathdy()
{
	double result;
	GetProperty(0xb, VT_R8, (void*)&result);
	return result;
}
void SetMathdy(double propVal)
{
	SetProperty(0xb, VT_R8, propVal);
}
double GetMathly()
{
	double result;
	GetProperty(0xc, VT_R8, (void*)&result);
	return result;
}
void SetMathly(double propVal)
{
	SetProperty(0xc, VT_R8, propVal);
}
long GetPx()
{
	long result;
	GetProperty(0xd, VT_I4, (void*)&result);
	return result;
}
void SetPx(long propVal)
{
	SetProperty(0xd, VT_I4, propVal);
}
long GetPy()
{
	long result;
	GetProperty(0xe, VT_I4, (void*)&result);
	return result;
}
void SetPy(long propVal)
{
	SetProperty(0xe, VT_I4, propVal);
}
long GetTypeAbs()
{
	long result;
	GetProperty(0xf, VT_I4, (void*)&result);
	return result;
}
void SetTypeAbs(long propVal)
{
	SetProperty(0xf, VT_I4, propVal);
}
double GetXend()
{
	double result;
	GetProperty(0x10, VT_R8, (void*)&result);
	return result;
}
void SetXend(double propVal)
{
	SetProperty(0x10, VT_R8, propVal);
}
double GetXfirst()
{
	double result;
	GetProperty(0x11, VT_R8, (void*)&result);
	return result;
}
void SetXfirst(double propVal)
{
	SetProperty(0x11, VT_R8, propVal);
}
long Getlog()
{
	long result;
	GetProperty(0x12, VT_I4, (void*)&result);
	return result;
}
void Setlog(long propVal)
{
	SetProperty(0x12, VT_I4, propVal);
}
long GetInd()
{
	long result;
	GetProperty(0x13, VT_I4, (void*)&result);
	return result;
}
void SetInd(long propVal)
{
	SetProperty(0x13, VT_I4, propVal);
}
long GetBlack()
{
	long result;
	GetProperty(0x14, VT_I4, (void*)&result);
	return result;
}
void SetBlack(long propVal)
{
	SetProperty(0x14, VT_I4, propVal);
}
double GetDeltaT()
{
	double result;
	GetProperty(0x15, VT_R8, (void*)&result);
	return result;
}
void SetDeltaT(double propVal)
{
	SetProperty(0x15, VT_R8, propVal);
}
long GetMaxBuffer()
{
	long result;
	GetProperty(0x16, VT_I4, (void*)&result);
	return result;
}
void SetMaxBuffer(long propVal)
{
	SetProperty(0x16, VT_I4, propVal);
}
long GetCommentInd()
{
	long result;
	GetProperty(0x21, VT_I4, (void*)&result);
	return result;
}
void SetCommentInd(long propVal)
{
	SetProperty(0x21, VT_I4, propVal);
}
CString GetCommentSTR()
{
	CString result;
	GetProperty(0x22, VT_BSTR, (void*)&result);
	return result;
}
void SetCommentSTR(CString propVal)
{
	SetProperty(0x22, VT_BSTR, propVal);
}
long GetMakeUpor()
{
	long result;
	GetProperty(0x23, VT_I4, (void*)&result);
	return result;
}
void SetMakeUpor(long propVal)
{
	SetProperty(0x23, VT_I4, propVal);
}
double GetUporNis()
{
	double result;
	GetProperty(0x24, VT_R8, (void*)&result);
	return result;
}
void SetUporNis(double propVal)
{
	SetProperty(0x24, VT_R8, propVal);
}
double GetUporVerh()
{
	double result;
	GetProperty(0x25, VT_R8, (void*)&result);
	return result;
}
void SetUporVerh(double propVal)
{
	SetProperty(0x25, VT_R8, propVal);
}


};
