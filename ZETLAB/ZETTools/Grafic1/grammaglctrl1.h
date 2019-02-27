#pragma once

// Сформированные компьютером классы оболочки IDispatch, созданные при помощи Microsoft Visual C++

// ПРИМЕЧАНИЕ. Не изменяйте содержимое этого класса. Если этот класс будет повторно создан при помощи
//  Microsoft Visual C++, ваши изменения будут перезаписаны.

/////////////////////////////////////////////////////////////////////////////
// Класс оболочки CGrammaglctrl1

class CGrammaglctrl1 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CGrammaglctrl1)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x2511D7FA, 0xC6BF, 0x4AA8, { 0xB5, 0xE3, 0x5C, 0x78, 0x81, 0xD8, 0x68, 0x92 } };
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

// _DGrammaGL

// Functions
//

	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long Paint(float * buffer)
	{
		long result;
		static BYTE parms[] = VTS_PR4 ;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_I4, (void*)&result, parms, buffer);
		return result;
	}
	void Formatxy(LPCTSTR fmt)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, fmt);
	}
	float GetPointAt(long X, long Y)
	{
		float result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x1b, DISPATCH_METHOD, VT_R4, (void*)&result, parms, X, Y);
		return result;
	}
	void Reset()
	{
		InvokeHelper(0x1c, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long SaveGLDataToFile(LPCTSTR FileName)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1f, DISPATCH_METHOD, VT_I4, (void*)&result, parms, FileName);
		return result;
	}
	long GetGLDataFromFile(LPCTSTR FileName)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x20, DISPATCH_METHOD, VT_I4, (void*)&result, parms, FileName);
		return result;
	}
	long Display()
	{
		long result;
		InvokeHelper(0x21, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	void AltC(float * buffer)
	{
		static BYTE parms[] = VTS_PR4 ;
		InvokeHelper(0x27, DISPATCH_METHOD, VT_EMPTY, NULL, parms, buffer);
	}
	long PaintNet(long pBuffer)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2a, DISPATCH_METHOD, VT_I4, (void*)&result, parms, pBuffer);
		return result;
	}
	void AltCNet(long pBuffer)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pBuffer);
	}
	CString GetFormatXY()
	{
		CString result;
		InvokeHelper(0x2c, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	long PaintNotDraw(float * buffer)
	{
		long result;
		static BYTE parms[] = VTS_PR4 ;
		InvokeHelper(0x2d, DISPATCH_METHOD, VT_I4, (void*)&result, parms, buffer);
		return result;
	}
	long PaintAll(float * buffer, long xsize, long ysize)
	{
		long result;
		static BYTE parms[] = VTS_PR4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x2e, DISPATCH_METHOD, VT_I4, (void*)&result, parms, buffer, xsize, ysize);
		return result;
	}
	long GetGrammaLine(long line, float * buffer)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PR4 ;
		InvokeHelper(0x31, DISPATCH_METHOD, VT_I4, (void*)&result, parms, line, buffer);
		return result;
	}
	void PutPictureToClipBoard()
	{
		InvokeHelper(0x32, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long GetMassivePointer(long i, long j)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4;
		InvokeHelper(0x33, DISPATCH_METHOD, VT_I4, (void*)&result, parms, i, j);
		return result;
	}
	long SetData(double data)
	{
		long result;
		static BYTE parms[] = VTS_R8;
		InvokeHelper(0x35, DISPATCH_METHOD, VT_I4, (void*)&result, parms, data);
		return result;
	}	
	LONG GetPointerOfLineData()
	{
		long result;
		InvokeHelper(0x36, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetGrammaColumn(long column, float * buffer)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PR4;
		InvokeHelper(0x37, DISPATCH_METHOD, VT_I4, (void*)&result, parms, column, buffer);
		return result;
	}

// Properties
//

double GetXfirst()
{
	double result;
	GetProperty(0x1, VT_R8, (void*)&result);
	return result;
}
void SetXfirst(double propVal)
{
	SetProperty(0x1, VT_R8, propVal);
}
double GetXend()
{
	double result;
	GetProperty(0x2, VT_R8, (void*)&result);
	return result;
}
void SetXend(double propVal)
{
	SetProperty(0x2, VT_R8, propVal);
}
long GetBlack()
{
	long result;
	GetProperty(0x3, VT_I4, (void*)&result);
	return result;
}
void SetBlack(long propVal)
{
	SetProperty(0x3, VT_I4, propVal);
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
double GetDeltaT()
{
	double result;
	GetProperty(0x6, VT_R8, (void*)&result);
	return result;
}
void SetDeltaT(double propVal)
{
	SetProperty(0x6, VT_R8, propVal);
}
unsigned long GetClrLeg()
{
	unsigned long result;
	GetProperty(0x7, VT_UI4, (void*)&result);
	return result;
}
void SetClrLeg(unsigned long propVal)
{
	SetProperty(0x7, VT_UI4, propVal);
}
unsigned long GetClrGrd()
{
	unsigned long result;
	GetProperty(0x8, VT_UI4, (void*)&result);
	return result;
}
void SetClrGrd(unsigned long propVal)
{
	SetProperty(0x8, VT_UI4, propVal);
}
double GetMathlx()
{
	double result;
	GetProperty(0x9, VT_R8, (void*)&result);
	return result;
}
void SetMathlx(double propVal)
{
	SetProperty(0x9, VT_R8, propVal);
}
double GetMathdx()
{
	double result;
	GetProperty(0xa, VT_R8, (void*)&result);
	return result;
}
void SetMathdx(double propVal)
{
	SetProperty(0xa, VT_R8, propVal);
}
double GetMathly()
{
	double result;
	GetProperty(0xb, VT_R8, (void*)&result);
	return result;
}
void SetMathly(double propVal)
{
	SetProperty(0xb, VT_R8, propVal);
}
double GetMathdy()
{
	double result;
	GetProperty(0xc, VT_R8, (void*)&result);
	return result;
}
void SetMathdy(double propVal)
{
	SetProperty(0xc, VT_R8, propVal);
}
long GetSize()
{
	long result;
	GetProperty(0xd, VT_I4, (void*)&result);
	return result;
}
void SetSize(long propVal)
{
	SetProperty(0xd, VT_I4, propVal);
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
long GetFontSize()
{
	long result;
	GetProperty(0x10, VT_I4, (void*)&result);
	return result;
}
void SetFontSize(long propVal)
{
	SetProperty(0x10, VT_I4, propVal);
}
CString GetFontName()
{
	CString result;
	GetProperty(0x11, VT_BSTR, (void*)&result);
	return result;
}
void SetFontName(CString propVal)
{
	SetProperty(0x11, VT_BSTR, propVal);
}
long GetTypeYAxis()
{
	long result;
	GetProperty(0x12, VT_I4, (void*)&result);
	return result;
}
void SetTypeYAxis(long propVal)
{
	SetProperty(0x12, VT_I4, propVal);
}
long GetTypeXAxis()
{
	long result;
	GetProperty(0x13, VT_I4, (void*)&result);
	return result;
}
void SetTypeXAxis(long propVal)
{
	SetProperty(0x13, VT_I4, propVal);
}
long GetTypeYData()
{
	long result;
	GetProperty(0x14, VT_I4, (void*)&result);
	return result;
}
void SetTypeYData(long propVal)
{
	SetProperty(0x14, VT_I4, propVal);
}
CString GetTextXAxis()
{
	CString result;
	GetProperty(0x15, VT_BSTR, (void*)&result);
	return result;
}
void SetTextXAxis(CString propVal)
{
	SetProperty(0x15, VT_BSTR, propVal);
}
CString GetTextYAxis()
{
	CString result;
	GetProperty(0x16, VT_BSTR, (void*)&result);
	return result;
}
void SetTextYAxis(CString propVal)
{
	SetProperty(0x16, VT_BSTR, propVal);
}
double GetReference()
{
	double result;
	GetProperty(0x17, VT_R8, (void*)&result);
	return result;
}
void SetReference(double propVal)
{
	SetProperty(0x17, VT_R8, propVal);
}
long GetAltCoords()
{
	long result;
	GetProperty(0x18, VT_I4, (void*)&result);
	return result;
}
void SetAltCoords(long propVal)
{
	SetProperty(0x18, VT_I4, propVal);
}
long GetNumber()
{
	long result;
	GetProperty(0x19, VT_I4, (void*)&result);
	return result;
}
void SetNumber(long propVal)
{
	SetProperty(0x19, VT_I4, propVal);
}
long GetCommentInd()
{
	long result;
	GetProperty(0x1d, VT_I4, (void*)&result);
	return result;
}
void SetCommentInd(long propVal)
{
	SetProperty(0x1d, VT_I4, propVal);
}
CString GetCommentSTR()
{
	CString result;
	GetProperty(0x1e, VT_BSTR, (void*)&result);
	return result;
}
void SetCommentSTR(CString propVal)
{
	SetProperty(0x1e, VT_BSTR, propVal);
}
long GetPx()
{
	long result;
	GetProperty(0x22, VT_I4, (void*)&result);
	return result;
}
void SetPx(long propVal)
{
	SetProperty(0x22, VT_I4, propVal);
}
long GetPy()
{
	long result;
	GetProperty(0x23, VT_I4, (void*)&result);
	return result;
}
void SetPy(long propVal)
{
	SetProperty(0x23, VT_I4, propVal);
}
long GetMakeUpor()
{
	long result;
	GetProperty(0x24, VT_I4, (void*)&result);
	return result;
}
void SetMakeUpor(long propVal)
{
	SetProperty(0x24, VT_I4, propVal);
}
double GetUporNis()
{
	double result;
	GetProperty(0x25, VT_R8, (void*)&result);
	return result;
}
void SetUporNis(double propVal)
{
	SetProperty(0x25, VT_R8, propVal);
}
double GetUporVerh()
{
	double result;
	GetProperty(0x26, VT_R8, (void*)&result);
	return result;
}
void SetUporVerh(double propVal)
{
	SetProperty(0x26, VT_R8, propVal);
}
float GetColorMin()
{
	float result;
	GetProperty(0x28, VT_R4, (void*)&result);
	return result;
}
void SetColorMin(float propVal)
{
	SetProperty(0x28, VT_R4, propVal);
}
float GetColorWidth()
{
	float result;
	GetProperty(0x29, VT_R4, (void*)&result);
	return result;
}
void SetColorWidth(float propVal)
{
	SetProperty(0x29, VT_R4, propVal);
}
long GetbwithPicture()
{
	long result;
	GetProperty(0x2f, VT_I4, (void*)&result);
	return result;
}
void SetbwithPicture(long propVal)
{
	SetProperty(0x2f, VT_I4, propVal);
}
CString GetsPictureName()
{
	CString result;
	GetProperty(0x30, VT_BSTR, (void*)&result);
	return result;
}
void SetsPictureName(CString propVal)
{
	SetProperty(0x30, VT_BSTR, propVal);
}
BOOL GetOrdinateData()
{
	BOOL result;
	GetProperty(0x34, VT_BOOL, (void*)&result);
	return result;
}
void SetOrdinateData(BOOL propVal)
{
	SetProperty(0x34, VT_BOOL, propVal);
}

};
