//------------------------------------------------------------------------------
//	UNIT-2.0
//------------------------------------------------------------------------------
#pragma once
#include <UnitDef.h>						// Unit-2
// Сформированные компьютером классы оболочки IDispatch, созданные при помощи Microsoft Visual C++

// ПРИМЕЧАНИЕ. Не изменяйте содержимое этого класса. Если этот класс будет повторно создан при помощи
//  Microsoft Visual C++, ваши изменения будут перезаписаны.

/////////////////////////////////////////////////////////////////////////////
// Класс оболочки CDUnitCln

class CDUnitCln : public CWnd
{
protected:
	DECLARE_DYNCREATE(CDUnitCln)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xE5B5E2, 0x8B11, 0x4C20, { 0x86, 0x57, 0x1C, 0x8F, 0xCF, 0xBC, 0xC5, 0xA } };
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

// Functions
//

	long Activate(LPCTSTR nameServer)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_I4, (void*)&result, parms, nameServer);
		return result;
	}
	long DisActivate()
	{
		long result;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long ShowUnit(short view)
	{
		long result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_I4, (void*)&result, parms, view);
		return result;
	}
	long GetAmountResult(long * pAmount)
	{
		long result;
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_I4, (void*)&result, parms, pAmount);
		return result;
	}
	long ReadResult(Unit2DataStruct * pVal)
	{
		long* pFullValue = (long*)pVal;
		long result;
		static BYTE parms[] = VTS_PI4;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_I4, (void*)&result, parms, pFullValue);
		return result;
	}
	long SetParam_txt(long number, LPCTSTR text)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_BSTR ;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_I4, (void*)&result, parms, number, text);
		return result;
	}
	long SetParam_32s(long number, long value)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_I4, (void*)&result, parms, number, value);
		return result;
	}
	long SetParam_32f(long number, float value)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_R4 ;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_I4, (void*)&result, parms, number, value);
		return result;
	}
	long SetParam_64f(long number, double value)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_R8 ;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_I4, (void*)&result, parms, number, value);
		return result;
	}
	long GetSizeCurResult(DWORD * pSize)
	{
		long result;
		static BYTE parms[] = VTS_PUI4 ;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_I4, (void*)&result, parms, pSize);
		return result;
	}
	long GetSizeMaxResult(DWORD * pSize)
	{
		long result;
		static BYTE parms[] = VTS_PUI4 ;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_I4, (void*)&result, parms, pSize);
		return result;
	}
};