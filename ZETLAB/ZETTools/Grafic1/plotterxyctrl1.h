#pragma once

// Сформированные компьютером классы оболочки IDispatch, созданные при помощи Microsoft Visual C++

// ПРИМЕЧАНИЕ. Не изменяйте содержимое этого класса. Если этот класс будет повторно создан при помощи
//  Microsoft Visual C++, ваши изменения будут перезаписаны.

/////////////////////////////////////////////////////////////////////////////
// Класс оболочки CPlotterxyctrl1

class CPlotterxyctrl1 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CPlotterxyctrl1)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x558AB7D7, 0xD1D6, 0x401C, { 0xB7, 0x37, 0x6D, 0xD9, 0xAE, 0x3, 0xF4, 0x5E } };
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

// _DPlotterXY

// Functions
//

	long FormatX(LPCTSTR format)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_I4, (void*)&result, parms, format);
		return result;
	}
	long FormatY(LPCTSTR format)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_I4, (void*)&result, parms, format);
		return result;
	}
	long FormatT(LPCTSTR format)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_I4, (void*)&result, parms, format);
		return result;
	}
	long PutNewPoint(float x, float y)
	{
		long result;
		static BYTE parms[] = VTS_R4 VTS_R4 ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_I4, (void*)&result, parms, x, y);
		return result;
	}
	long PutXPoints(float * Xpoints)
	{
		long result;
		static BYTE parms[] = VTS_PR4 ;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Xpoints);
		return result;
	}
	long PutYPoints(float * Ypoints)
	{
		long result;
		static BYTE parms[] = VTS_PR4 ;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Ypoints);
		return result;
	}
	long ResetPlotter()
	{
		long result;
		InvokeHelper(0x18, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long SaveDateToFile(LPCTSTR FName)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x25, DISPATCH_METHOD, VT_I4, (void*)&result, parms, FName);
		return result;
	}
	long LoadComments(LPCTSTR FName)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x28, DISPATCH_METHOD, VT_I4, (void*)&result, parms, FName);
		return result;
	}
	long LoadDataFromFile(LPCTSTR FName, float * XArray, float * YArray, BOOL DrawData)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_PR4 VTS_PR4 VTS_BOOL ;
		InvokeHelper(0x29, DISPATCH_METHOD, VT_I4, (void*)&result, parms, FName, XArray, YArray, DrawData);
		return result;
	}
	long PutXYPoints(float * Xpoints, float * Ypoints)
	{
		long result;
		static BYTE parms[] = VTS_PR4 VTS_PR4 ;
		InvokeHelper(0x2b, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Xpoints, Ypoints);
		return result;
	}
	long PutZPoints(float * Zpoints)
	{
		long result;
		static BYTE parms[] = VTS_PR4 ;
		InvokeHelper(0x2d, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Zpoints);
		return result;
	}
	long PutXYZPoints(float * Xpoints, float * Ypoints, float * Zpoints)
	{
		long result;
		static BYTE parms[] = VTS_PR4 VTS_PR4 VTS_PR4 ;
		InvokeHelper(0x2e, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Xpoints, Ypoints, Zpoints);
		return result;
	}
	long PutNewPointXYZ(float x, float y, float z)
	{
		long result;
		static BYTE parms[] = VTS_R4 VTS_R4 VTS_R4 ;
		InvokeHelper(0x2f, DISPATCH_METHOD, VT_I4, (void*)&result, parms, x, y, z);
		return result;
	}
	void ChangeProperties()
	{
		InvokeHelper(0x30, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long FormatZ(LPCTSTR format)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x31, DISPATCH_METHOD, VT_I4, (void*)&result, parms, format);
		return result;
	}
	void PrintScreen()
	{
		InvokeHelper(0x36, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long AutoScale()
	{
		long result;
		InvokeHelper(0x37, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long PutXPointsNet(long pXpoints)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x38, DISPATCH_METHOD, VT_I4, (void*)&result, parms, pXpoints);
		return result;
	}
	long PutYPointsNet(long pYpoints)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x39, DISPATCH_METHOD, VT_I4, (void*)&result, parms, pYpoints);
		return result;
	}
	long PutZPointsNet(long pZpoints)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3a, DISPATCH_METHOD, VT_I4, (void*)&result, parms, pZpoints);
		return result;
	}
	long PutXYPointsNet(long pXpoints, long pYpoints)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x3b, DISPATCH_METHOD, VT_I4, (void*)&result, parms, pXpoints, pYpoints);
		return result;
	}
	long PutXYZPointsNet(long pXpoints, long pYpoints, long pZpoints)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x3c, DISPATCH_METHOD, VT_I4, (void*)&result, parms, pXpoints, pYpoints, pZpoints);
		return result;
	}
	long LoadDataFromFileNet(LPCTSTR FName, long pXArray, long pYArray, BOOL DrawData)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_I4 VTS_I4 VTS_BOOL ;
		InvokeHelper(0x3d, DISPATCH_METHOD, VT_I4, (void*)&result, parms, FName, pXArray, pYArray, DrawData);
		return result;
	}

// Properties
//

unsigned long GetClrFon()
{
	unsigned long result;
	GetProperty(0x7, VT_UI4, (void*)&result);
	return result;
}
void SetClrFon(unsigned long propVal)
{
	SetProperty(0x7, VT_UI4, propVal);
}
unsigned long GetClrGrfBeg()
{
	unsigned long result;
	GetProperty(0x8, VT_UI4, (void*)&result);
	return result;
}
void SetClrGrfBeg(unsigned long propVal)
{
	SetProperty(0x8, VT_UI4, propVal);
}
unsigned long GetClrGrfEnd()
{
	unsigned long result;
	GetProperty(0x9, VT_UI4, (void*)&result);
	return result;
}
void SetClrGrfEnd(unsigned long propVal)
{
	SetProperty(0x9, VT_UI4, propVal);
}
unsigned long GetClrGrd()
{
	unsigned long result;
	GetProperty(0xa, VT_UI4, (void*)&result);
	return result;
}
void SetClrGrd(unsigned long propVal)
{
	SetProperty(0xa, VT_UI4, propVal);
}
unsigned long GetClrLeg()
{
	unsigned long result;
	GetProperty(0xb, VT_UI4, (void*)&result);
	return result;
}
void SetClrLeg(unsigned long propVal)
{
	SetProperty(0xb, VT_UI4, propVal);
}
unsigned long GetClrNum()
{
	unsigned long result;
	GetProperty(0xc, VT_UI4, (void*)&result);
	return result;
}
void SetClrNum(unsigned long propVal)
{
	SetProperty(0xc, VT_UI4, propVal);
}
float GetMathDX()
{
	float result;
	GetProperty(0xd, VT_R4, (void*)&result);
	return result;
}
void SetMathDX(float propVal)
{
	SetProperty(0xd, VT_R4, propVal);
}
float GetMathLX()
{
	float result;
	GetProperty(0xe, VT_R4, (void*)&result);
	return result;
}
void SetMathLX(float propVal)
{
	SetProperty(0xe, VT_R4, propVal);
}
float GetMathDY()
{
	float result;
	GetProperty(0xf, VT_R4, (void*)&result);
	return result;
}
void SetMathDY(float propVal)
{
	SetProperty(0xf, VT_R4, propVal);
}
float GetMathLY()
{
	float result;
	GetProperty(0x10, VT_R4, (void*)&result);
	return result;
}
void SetMathLY(float propVal)
{
	SetProperty(0x10, VT_R4, propVal);
}
float GetMathDZ()
{
	float result;
	GetProperty(0x11, VT_R4, (void*)&result);
	return result;
}
void SetMathDZ(float propVal)
{
	SetProperty(0x11, VT_R4, propVal);
}
float GetMathLZ()
{
	float result;
	GetProperty(0x12, VT_R4, (void*)&result);
	return result;
}

void SetMathLZ(float propVal)
{
	SetProperty(0x12, VT_R4, propVal);
}
long GetNumPoints()
{
	long result;
	GetProperty(0x13, VT_I4, (void*)&result);
	return result;
}
void SetNumPoints(long propVal)
{
	SetProperty(0x13, VT_I4, propVal);
}
long GetTypeCoord()
{
	long result;
	GetProperty(0x14, VT_I4, (void*)&result);
	return result;
}
void SetTypeCoord(long propVal)
{
	SetProperty(0x14, VT_I4, propVal);
}
long GetTypeLine()
{
	long result;
	GetProperty(0x15, VT_I4, (void*)&result);
	return result;
}
void SetTypeLine(long propVal)
{
	SetProperty(0x15, VT_I4, propVal);
}
CString GetFontName()
{
	CString result;
	GetProperty(0x16, VT_BSTR, (void*)&result);
	return result;
}
void SetFontName(CString propVal)
{
	SetProperty(0x16, VT_BSTR, propVal);
}
long GetFontSize()
{
	long result;
	GetProperty(0x17, VT_I4, (void*)&result);
	return result;
}
void SetFontSize(long propVal)
{
	SetProperty(0x17, VT_I4, propVal);
}

CString GetTextXAxis()
{
	CString result;
	GetProperty(0x40, VT_BSTR, (void*)&result);
	return result;
}
void SetTextXAxis(CString propVal)
{
	SetProperty(0x40, VT_BSTR, propVal);
}
CString GetTextYAxis()
{
	CString result;
	GetProperty(0x41, VT_BSTR, (void*)&result);
	return result;
}
void SetTextYAxis(CString propVal)
{
	SetProperty(0x41, VT_BSTR, propVal);
}
CString GetTextTAxis()
{
	CString result;
	GetProperty(0x42, VT_BSTR, (void*)&result);
	return result;
}
void SetTextTAxis(CString propVal)
{
	SetProperty(0x42, VT_BSTR, propVal);
}
CString GetTextZAxis()
{
	CString result;
	GetProperty(0x43, VT_BSTR, (void*)&result);
	return result;
}
void SetTextZAxis(CString propVal)
{
	SetProperty(0x43, VT_BSTR, propVal);
}
float GetUporXNis()
{
	float result;
	GetProperty(0x19, VT_R4, (void*)&result);
	return result;
}
void SetUporXNis(float propVal)
{
	SetProperty(0x19, VT_R4, propVal);
}
float GetUporXVerh()
{
	float result;
	GetProperty(0x1a, VT_R4, (void*)&result);
	return result;
}
void SetUporXVerh(float propVal)
{
	SetProperty(0x1a, VT_R4, propVal);
}
float GetUporYNis()
{
	float result;
	GetProperty(0x1b, VT_R4, (void*)&result);
	return result;
}
void SetUporYNis(float propVal)
{
	SetProperty(0x1b, VT_R4, propVal);
}
float GetUporYVerh()
{
	float result;
	GetProperty(0x1c, VT_R4, (void*)&result);
	return result;
}
void SetUporYVerh(float propVal)
{
	SetProperty(0x1c, VT_R4, propVal);
}
float GetUporZNis()
{
	float result;
	GetProperty(0x1d, VT_R4, (void*)&result);
	return result;
}
void SetUporZNis(float propVal)
{
	SetProperty(0x1d, VT_R4, propVal);
}
float GetUporZVerh()
{
	float result;
	GetProperty(0x1e, VT_R4, (void*)&result);
	return result;
}
void SetUporZVerh(float propVal)
{
	SetProperty(0x1e, VT_R4, propVal);
}
unsigned long GetClrCur()
{
	unsigned long result;
	GetProperty(0x1f, VT_UI4, (void*)&result);
	return result;
}
void SetClrCur(unsigned long propVal)
{
	SetProperty(0x1f, VT_UI4, propVal);
}
BOOL GetFixedTime()
{
	BOOL result;
	GetProperty(0x20, VT_BOOL, (void*)&result);
	return result;
}
void SetFixedTime(BOOL propVal)
{
	SetProperty(0x20, VT_BOOL, propVal);
}
BOOL GetLeftRight()
{
	BOOL result;
	GetProperty(0x21, VT_BOOL, (void*)&result);
	return result;
}
void SetLeftRight(BOOL propVal)
{
	SetProperty(0x21, VT_BOOL, propVal);
}
BOOL GetAutoMX()
{
	BOOL result;
	GetProperty(0x22, VT_BOOL, (void*)&result);
	return result;
}
void SetAutoMX(BOOL propVal)
{
	SetProperty(0x22, VT_BOOL, propVal);
}
BOOL GetShowFPS()
{
	BOOL result;
	GetProperty(0x23, VT_BOOL, (void*)&result);
	return result;
}
void SetShowFPS(BOOL propVal)
{
	SetProperty(0x23, VT_BOOL, propVal);
}
float GetDeltaT()
{
	float result;
	GetProperty(0x24, VT_R4, (void*)&result);
	return result;
}
void SetDeltaT(float propVal)
{
	SetProperty(0x24, VT_R4, propVal);
}
long GetCommentIND()
{
	long result;
	GetProperty(0x26, VT_I4, (void*)&result);
	return result;
}
void SetCommentIND(long propVal)
{
	SetProperty(0x26, VT_I4, propVal);
}
CString GetCommentSTR()
{
	CString result;
	GetProperty(0x27, VT_BSTR, (void*)&result);
	return result;
}
void SetCommentSTR(CString propVal)
{
	SetProperty(0x27, VT_BSTR, propVal);
}
float GetUporDelta()
{
	float result;
	GetProperty(0x2a, VT_R4, (void*)&result);
	return result;
}
void SetUporDelta(float propVal)
{
	SetProperty(0x2a, VT_R4, propVal);
}
long GetSquareScale()
{
	long result;
	GetProperty(0x2c, VT_I4, (void*)&result);
	return result;
}
void SetSquareScale(long propVal)
{
	SetProperty(0x2c, VT_I4, propVal);
}
float GetMathDZSignal()
{
	float result;
	GetProperty(0x32, VT_R4, (void*)&result);
	return result;
}
void SetMathDZSignal(float propVal)
{
	SetProperty(0x32, VT_R4, propVal);
}
float GetMathLZSignal()
{
	float result;
	GetProperty(0x33, VT_R4, (void*)&result);
	return result;
}
void SetMathLZSignal(float propVal)
{
	SetProperty(0x33, VT_R4, propVal);
}
float GetUporZSignalNis()
{
	float result;
	GetProperty(0x34, VT_R4, (void*)&result);
	return result;
}
void SetUporZSignalNis(float propVal)
{
	SetProperty(0x34, VT_R4, propVal);
}
float GetUporZSignalVerh()
{
	float result;
	GetProperty(0x35, VT_R4, (void*)&result);
	return result;
}
void SetUporZSignalVerh(float propVal)
{
	SetProperty(0x35, VT_R4, propVal);
}
long GetCurNPoints()
{
	long result;
	GetProperty(0x3e, VT_I4, (void*)&result);
	return result;
}
void SetCurNPoints(long propVal)
{
	SetProperty(0x3e, VT_I4, propVal);
}


};
