#pragma once
// Класс CDUnit

class CDUnit : public CWnd
{
protected:
	DECLARE_DYNCREATE(CDUnit)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x81485D29, 0x20E5, 0x4356, { 0x80, 0x61, 0xA3, 0x55, 0x2F, 0xE2, 0xA, 0xDC } };
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

// _DUnit

// Functions
//

	long DisActivate()
	{
		long result;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long UnitReg(long hw)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_I4, (void*)&result, parms, hw);
		return result;
	}
	long UnitWrite(long size, float * data, long param)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PR4 VTS_I4 ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_I4, (void*)&result, parms, size, data, param);
		return result;
	}
	long UnitRead(long * size, float * data, long param)
	{
		long result;
		static BYTE parms[] = VTS_PI4 VTS_PR4 VTS_I4 ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_I4, (void*)&result, parms, size, data, param);
		return result;
	}
	long Read(long * size, float * data, long * param)
	{
		long result;
		static BYTE parms[] = VTS_PI4 VTS_PR4 VTS_PI4 ;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_I4, (void*)&result, parms, size, data, param);
		return result;
	}
	long Write(long size, float * data, long param)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PR4 VTS_I4 ;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_I4, (void*)&result, parms, size, data, param);
		return result;
	}
	long Activate(LPCTSTR name)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_I4, (void*)&result, parms, name);
		return result;
	}
	long SetParam(long param, double value)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_R8 ;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_I4, (void*)&result, parms, param, value);
		return result;
	}
	long UnitParam(long * param, double * value)
	{
		long result;
		static BYTE parms[] = VTS_PI4 VTS_PR8 ;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_I4, (void*)&result, parms, param, value);
		return result;
	}
	void IamActive()
	{
		InvokeHelper(0xa, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long SetUSize(double left, double top, double width, double height)
	{
		long result;
		static BYTE parms[] = VTS_R8 VTS_R8 VTS_R8 VTS_R8 ;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_I4, (void*)&result, parms, left, top, width, height);
		return result;
	}
	long ShowUnit(short View)
	{
		long result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_I4, (void*)&result, parms, View);
		return result;
	}
	long MoveUnit(double X, double Y)
	{
		long result;
		static BYTE parms[] = VTS_R8 VTS_R8 ;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_I4, (void*)&result, parms, X, Y);
		return result;
	}
	double GetUnitWidth()
	{
		double result;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	double GetUnitHeight()
	{
		double result;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	double GetUnitLeft()
	{
		double result;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	double GetUnitTop()
	{
		double result;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	long WriteUnitData(LPCTSTR FileName)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x12, DISPATCH_METHOD, VT_I4, (void*)&result, parms, FileName);
		return result;
	}
	long UnitRecFile(signed char * FileName)
	{
		long result;
		static BYTE parms[] = VTS_PI1 ;
		InvokeHelper(0x14, DISPATCH_METHOD, VT_I4, (void*)&result, parms, FileName);
		return result;
	}
	long InitUnit()
	{
		long result;
		InvokeHelper(0x15, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long UnitUnReg(long UHw)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x16, DISPATCH_METHOD, VT_I4, (void*)&result, parms, UHw);
		return result;
	}
	long SetParamString(long param, signed char * StringVal)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PI1 ;
		InvokeHelper(0x17, DISPATCH_METHOD, VT_I4, (void*)&result, parms, param, StringVal);
		return result;
	}
	long UnitReadString(long * param, signed char * StringVal)
	{
		long result;
		static BYTE parms[] = VTS_PI4 VTS_PI1 ;
		InvokeHelper(0x18, DISPATCH_METHOD, VT_I4, (void*)&result, parms, param, StringVal);
		return result;
	}
	long ReadNet(long * size, long pData, long * param)
	{
		long result;
		static BYTE parms[] = VTS_PI4 VTS_I4 VTS_PI4 ;
		InvokeHelper(0x19, DISPATCH_METHOD, VT_I4, (void*)&result, parms, size, pData, param);
		return result;
	}
	long WriteNet(long size, long pData, long param)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x1a, DISPATCH_METHOD, VT_I4, (void*)&result, parms, size, pData, param);
		return result;
	}
	long IsReady()
	{
		long result;
		InvokeHelper(0x1b, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long SetParamTimeString(long param, LPCTSTR StringVal, double timer)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_BSTR VTS_R8 ;
		InvokeHelper(0x1c, DISPATCH_METHOD, VT_I4, (void*)&result, parms, param, StringVal, timer);
		return result;
	}
	long UnitReadTimeString(long * param, BSTR * StringVal, double * timer)
	{
		long result;
		static BYTE parms[] = VTS_PI4 VTS_PBSTR VTS_PR8 ;
		InvokeHelper(0x1d, DISPATCH_METHOD, VT_I4, (void*)&result, parms, param, StringVal, timer);
		return result;
	}
};
