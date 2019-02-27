#pragma once

// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CGridglctrl1 wrapper class

class CGridglctrl1 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CGridglctrl1)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x2C9986C7, 0x18D6, 0x4474, { 0xBD, 0x2D, 0xDE, 0x6C, 0xA0, 0x36, 0x2B, 0x99 } };
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

// _DGridGL

// Functions
//

	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	short Display()
	{
		short result;
		InvokeHelper(0x1b, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
		return result;
	}
	long Paint(float * buffer)
	{
		long result;
		static BYTE parms[] = VTS_PR4 ;
		InvokeHelper(0x1c, DISPATCH_METHOD, VT_I4, (void*)&result, parms, buffer);
		return result;
	}
	void Formatxy(LPCTSTR fmt)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, fmt);
	}
	long FormatX(LPCTSTR fmt)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1e, DISPATCH_METHOD, VT_I4, (void*)&result, parms, fmt);
		return result;
	}
	long FormatY(LPCTSTR fmt)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1f, DISPATCH_METHOD, VT_I4, (void*)&result, parms, fmt);
		return result;
	}
	void AltC(float * buffer)
	{
		static BYTE parms[] = VTS_PR4 ;
		InvokeHelper(0x20, DISPATCH_METHOD, VT_EMPTY, NULL, parms, buffer);
	}
	void PushToClipBoard()
	{
		InvokeHelper(0x21, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long PutChnText(LPCTSTR text, long index)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_I4 ;
		InvokeHelper(0x29, DISPATCH_METHOD, VT_I4, (void*)&result, parms, text, index);
		return result;
	}
	long ShowPropSheet()
	{
		long result;
		InvokeHelper(0x2f, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long OutWindPtr(signed char * ptr)
	{
		long result;
		static BYTE parms[] = VTS_PI1 ;
		InvokeHelper(0x31, DISPATCH_METHOD, VT_I4, (void*)&result, parms, ptr);
		return result;
	}
	long InWindPtr(signed char * ptr)
	{
		long result;
		static BYTE parms[] = VTS_PI1 ;
		InvokeHelper(0x32, DISPATCH_METHOD, VT_I4, (void*)&result, parms, ptr);
		return result;
	}
	void AltDate(DATE * buffer)
	{
		static BYTE parms[] = VTS_PDATE ;
		InvokeHelper(0x34, DISPATCH_METHOD, VT_EMPTY, NULL, parms, buffer);
	}
	long PaintNet(long pBuffer)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x35, DISPATCH_METHOD, VT_I4, (void*)&result, parms, pBuffer);
		return result;
	}
	void AltCNet(long pBuffer)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x36, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pBuffer);
	}
	void AltDateNet(long pBuffer)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x37, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pBuffer);
	}
	void PushDataToClipBoard()
	{
		InvokeHelper(0x38, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DrawPicture(unsigned long hdc, long x, long y, long width, long height)
	{
		static BYTE parms[] = VTS_UI4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x39, DISPATCH_METHOD, VT_EMPTY, NULL, parms, hdc, x, y, width, height);
	}
	void AutoScaleY()
	{
		InvokeHelper(0x3a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long SaveGLDataToFile(LPCTSTR FileName)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x3d, DISPATCH_METHOD, VT_I4, (void*)&result, parms, FileName);
		return result;
	}
	BOOL AddToolTip(LPCTSTR text, long pos)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_I4 ;
		InvokeHelper(0x3f, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, text, pos);
		return result;
	}
	BOOL RemoveAllToolTips()
	{
		BOOL result;
		InvokeHelper(0x40, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	long getStartPoint()
	{
		long result;
		InvokeHelper(0x41, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long getEndPoint()
	{
		long result;
		InvokeHelper(0x42, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long SaveGridToFile(LPCTSTR FileName)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x43, DISPATCH_METHOD, VT_I4, (void*)&result, parms, FileName);
		return result;
	}
	double GetStep()
	{
		double result;
		InvokeHelper(0x44, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	long VisibleWidthPx()
	{
		long result;
		InvokeHelper(0x45, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	void DrawRift(double riftMathStart, double riftMathStop, long riftMinWidthPx)
	{
		static BYTE parms[] = VTS_R8 VTS_R8 VTS_I4 ;
		InvokeHelper(0x46, DISPATCH_METHOD, VT_EMPTY, NULL, parms, riftMathStart, riftMathStop, riftMinWidthPx);
	}
	long AddMark(double x, LPCTSTR text)
	{
		long result;
		static BYTE parms[] = VTS_R8 VTS_BSTR ;
		InvokeHelper(0x47, DISPATCH_METHOD, VT_I4, (void*)&result, parms, x, text);
		return result;
	}
	long EditMark(long index, LPCTSTR newText)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_BSTR ;
		InvokeHelper(0x48, DISPATCH_METHOD, VT_I4, (void*)&result, parms, index, newText);
		return result;
	}
	long GetMark(long index, double * x, BSTR * text)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PR8 VTS_PBSTR ;
		InvokeHelper(0x49, DISPATCH_METHOD, VT_I4, (void*)&result, parms, index, x, text);
		return result;
	}
	long MoveMark(long index, double newX)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_R8 ;
		InvokeHelper(0x4a, DISPATCH_METHOD, VT_I4, (void*)&result, parms, index, newX);
		return result;
	}
	long RemoveMark(long index)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x4b, DISPATCH_METHOD, VT_I4, (void*)&result, parms, index);
		return result;
	}
	long PaintDouble(double * buffer)
	{
		long result;
		static BYTE parms[] = VTS_PR8 ;
		InvokeHelper(0x4c, DISPATCH_METHOD, VT_I4, (void*)&result, parms, buffer);
		return result;
	}
	long SetColorLineCount(long Count)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x4f, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Count);
		return result;
	}
	long SetColorLineFont(long Number, unsigned long _Color)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_UI4 ;
		InvokeHelper(0x50, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Number, _Color);
		return result;
	}
	long SetColorLineName(long Number, LPCTSTR Name)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_BSTR ;
		InvokeHelper(0x51, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Number, Name);
		return result;
	}
	long SetColorLineSpans(long Number, long * IndexBeginArray, long * IndexEndArray, unsigned long * ColorsArray, long ArraySize)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PI4 VTS_PI4 VTS_PUI4 VTS_I4 ;
		InvokeHelper(0x52, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Number, IndexBeginArray, IndexEndArray, ColorsArray, ArraySize);
		return result;
	}
	long SetColorLineSpansByTime(long Number, DATE * DateBeginArray, DATE * DateEndArray, unsigned long * ColorsArray, long ArraySize)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PDATE VTS_PDATE VTS_PUI4 VTS_I4 ;
		InvokeHelper(0x53, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Number, DateBeginArray, DateEndArray, ColorsArray, ArraySize);
		return result;
	}
	long GetPointerToFloatData()
	{
		long result;
		InvokeHelper(0x55, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetPointerToXCoords()
	{
		long result;
		InvokeHelper(0x56, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	void AutoScaleX()
	{
		InvokeHelper(0x57, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long SetAxisSetting(long index, long UseAxis, LPCTSTR AxisText, unsigned long AxisColor, float CoeffMultiplicative, float CoeffAdditive)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BSTR VTS_UI4 VTS_R4 VTS_R4;
		InvokeHelper(0x5a, DISPATCH_METHOD, VT_I4, (void*)&result, parms, index, UseAxis, AxisText, AxisColor, CoeffMultiplicative, CoeffAdditive);
		return result;
	}
	long SetAdditionalWindowSettings(long index, long UseWindow, long GraphNumber, long IndexPoint, unsigned long TextColor, unsigned long BackgroundColor, LPCTSTR WindowText)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_UI4 VTS_UI4 VTS_BSTR;
		InvokeHelper(0x5c, DISPATCH_METHOD, VT_I4, (void*)&result, parms, index, UseWindow, GraphNumber, IndexPoint, TextColor, BackgroundColor, WindowText);
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
	GetProperty(0x16, VT_I4, (void*)&result);
	return result;
}
void SetMakeUpor(long propVal)
{
	SetProperty(0x16, VT_I4, propVal);
}
double GetUporVerh()
{
	double result;
	GetProperty(0x17, VT_R8, (void*)&result);
	return result;
}
void SetUporVerh(double propVal)
{
	SetProperty(0x17, VT_R8, propVal);
}
double GetUporNis()
{
	double result;
	GetProperty(0x18, VT_R8, (void*)&result);
	return result;
}
void SetUporNis(double propVal)
{
	SetProperty(0x18, VT_R8, propVal);
}
long GetNumVisiblePoints()
{
	long result;
	GetProperty(0x19, VT_I4, (void*)&result);
	return result;
}
void SetNumVisiblePoints(long propVal)
{
	SetProperty(0x19, VT_I4, propVal);
}
double GetUporDelta()
{
	double result;
	GetProperty(0x1a, VT_R8, (void*)&result);
	return result;
}
void SetUporDelta(double propVal)
{
	SetProperty(0x1a, VT_R8, propVal);
}
CString GetFontName()
{
	CString result;
	GetProperty(0x22, VT_BSTR, (void*)&result);
	return result;
}
void SetFontName(CString propVal)
{
	SetProperty(0x22, VT_BSTR, propVal);
}
long GetFontSize()
{
	long result;
	GetProperty(0x23, VT_I4, (void*)&result);
	return result;
}
void SetFontSize(long propVal)
{
	SetProperty(0x23, VT_I4, propVal);
}
long GetTypeXAxis()
{
	long result;
	GetProperty(0x24, VT_I4, (void*)&result);
	return result;
}
void SetTypeXAxis(long propVal)
{
	SetProperty(0x24, VT_I4, propVal);
}
long GetTypeYAxis()
{
	long result;
	GetProperty(0x25, VT_I4, (void*)&result);
	return result;
}
void SetTypeYAxis(long propVal)
{
	SetProperty(0x25, VT_I4, propVal);
}
long GetTypeFill()
{
	long result;
	GetProperty(0x26, VT_I4, (void*)&result);
	return result;
}
void SetTypeFill(long propVal)
{
	SetProperty(0x26, VT_I4, propVal);
}
long GetLineWidth()
{
	long result;
	GetProperty(0x27, VT_I4, (void*)&result);
	return result;
}
void SetLineWidth(long propVal)
{
	SetProperty(0x27, VT_I4, propVal);
}
long GetShowScalePropPage()
{
	long result;
	GetProperty(0x28, VT_I4, (void*)&result);
	return result;
}
void SetShowScalePropPage(long propVal)
{
	SetProperty(0x28, VT_I4, propVal);
}
long GetTypeYData()
{
	long result;
	GetProperty(0x2a, VT_I4, (void*)&result);
	return result;
}
void SetTypeYData(long propVal)
{
	SetProperty(0x2a, VT_I4, propVal);
}
double GetReference()
{
	double result;
	GetProperty(0x2b, VT_R8, (void*)&result);
	return result;
}
void SetReference(double propVal)
{
	SetProperty(0x2b, VT_R8, propVal);
}
long GetSpectr()
{
	long result;
	GetProperty(0x2c, VT_I4, (void*)&result);
	return result;
}
void SetSpectr(long propVal)
{
	SetProperty(0x2c, VT_I4, propVal);
}
CString GetTextYAxis()
{
	CString result;
	GetProperty(0x2d, VT_BSTR, (void*)&result);
	return result;
}
void SetTextYAxis(CString propVal)
{
	SetProperty(0x2d, VT_BSTR, propVal);
}
CString GetTextXAxis()
{
	CString result;
	GetProperty(0x2e, VT_BSTR, (void*)&result);
	return result;
}
void SetTextXAxis(CString propVal)
{
	SetProperty(0x2e, VT_BSTR, propVal);
}
long GetAltCoords()
{
	long result;
	GetProperty(0x30, VT_I4, (void*)&result);
	return result;
}
void SetAltCoords(long propVal)
{
	SetProperty(0x30, VT_I4, propVal);
}
long GetOsc()
{
	long result;
	GetProperty(0x33, VT_I4, (void*)&result);
	return result;
}
void SetOsc(long propVal)
{
	SetProperty(0x33, VT_I4, propVal);
}
long GetCommentInd()
{
	long result;
	GetProperty(0x3b, VT_I4, (void*)&result);
	return result;
}
void SetCommentInd(long propVal)
{
	SetProperty(0x3b, VT_I4, propVal);
}
CString GetCommentSTR()
{
	CString result;
	GetProperty(0x3c, VT_BSTR, (void*)&result);
	return result;
}
void SetCommentSTR(CString propVal)
{
	SetProperty(0x3c, VT_BSTR, propVal);
}
BOOL GetIsShowPropSheet()
{
	BOOL result;
	GetProperty(0x3e, VT_BOOL, (void*)&result);
	return result;
}
void SetIsShowPropSheet(BOOL propVal)
{
	SetProperty(0x3e, VT_BOOL, propVal);
}
BOOL GetHideZeros()
{
	BOOL result;
	GetProperty(0x4d, VT_BOOL, (void*)&result);
	return result;
}
void SetHideZeros(BOOL propVal)
{
	SetProperty(0x4d, VT_BOOL, propVal);
}
BOOL GetPowerDecibell()
{
	BOOL result;
	GetProperty(0x4e, VT_BOOL, (void*)&result);
	return result;
}
void SetPowerDecibell(BOOL propVal)
{
	SetProperty(0x4e, VT_BOOL, propVal);
}
long GetShowLegend()
{
	long result;
	GetProperty(0x54, VT_I4, (void*)&result);
	return result;
}
void SetShowLegend(long propVal)
{
	SetProperty(0x54, VT_I4, propVal);
}

BOOL GetEnableYDecibelScale()
{
	BOOL result;
	GetProperty(0x58, VT_BOOL, (void*)&result);
	return result;
}

void SetEnableYDecibelScale(BOOL propVal)
{
	SetProperty(0x58, VT_BOOL, propVal);
}

BOOL GetEnableXOctaveScale()
{
	BOOL result;
	GetProperty(0x59, VT_BOOL, (void*)&result);
	return result;
}

void SetEnableXOctaveScale(BOOL propVal)
{
	SetProperty(0x59, VT_BOOL, propVal);
}

long GetGraphAxis()
{
	long result;
	GetProperty(0x5b, VT_I4, (void*)&result);
	return result;
}
void SetGraphAxis(long propVal)
{
	SetProperty(0x5b, VT_I4, propVal);
}

};