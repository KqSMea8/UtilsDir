//------------------------------------------------------------------------------
//	UNIT-2.0
//------------------------------------------------------------------------------
#pragma once
#include <UnitDef.h>						// Unit-2
// Сформированные компьютером классы оболочки IDispatch, созданные при помощи Microsoft Visual C++

// ПРИМЕЧАНИЕ. Не изменяйте содержимое этого класса. Если этот класс будет повторно создан при помощи
//  Microsoft Visual C++, ваши изменения будут перезаписаны.

/////////////////////////////////////////////////////////////////////////////
// Класс оболочки CDUnitSrv

class CDUnitSrv : public CWnd
{
protected:
	DECLARE_DYNCREATE(CDUnitSrv)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xDCBCEBF4, 0xFDDD, 0x4CC9, { 0xB6, 0x3D, 0xF2, 0x3, 0x95, 0x1F, 0x70, 0x57 } };
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
	long Activate()
	{
		long result;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long DisActivate()
	{
		long result;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetAmountParam(long * pAmount)
	{
		long result;
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_I4, (void*)&result, parms, pAmount);
		return result;
	}
	long GetParam(Unit2DataStruct * pVal)
	{
		long* pFullValue = (long*)pVal;
		long result;
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_I4, (void*)&result, parms, pFullValue);
		return result;
	}
	long WriteResult_txt(long number, LPCTSTR text)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_BSTR ;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_I4, (void*)&result, parms, number, text);
		return result;
	}
	long WriteResult_32s(long number, long value)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_I4, (void*)&result, parms, number, value);
		return result;
	}
	long WriteResult_32f(long number, float value)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_R4 ;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_I4, (void*)&result, parms, number, value);
		return result;
	}
	long WriteResult_64f(long number, double value)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_R8 ;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_I4, (void*)&result, parms, number, value);
		return result;
	}
	long WriteResult_32s_arr(long number, DWORD sizeArray, long * pArray)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_UI4 VTS_PI4 ;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_I4, (void*)&result, parms, number, sizeArray, pArray);
		return result;
	}
	long WriteResult_32f_arr(long number, DWORD sizeArray, float * pArray)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_UI4 VTS_PR4 ;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_I4, (void*)&result, parms, number, sizeArray, pArray);
		return result;
	}
	long WriteResult_64f_arr(long number, DWORD sizeArray, double * pArray)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_UI4 VTS_PR8 ;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_I4, (void*)&result, parms, number, sizeArray, pArray);
		return result;
	}

	long WriteResult_wrs_32s(long number, WideResult<long> * pWrs)
	{
		long *pValue = (long*)pWrs;
		long result;
		static BYTE parms[] = VTS_I4 VTS_PI4 ;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_I4, (void*)&result, parms, number, pValue);
		return result;
	}
	long WriteResult_wrs_32f(long number, WideResult<float> * pWrs)
	{
		long *pValue = (long*)pWrs;
		long result;
		static BYTE parms[] = VTS_I4 VTS_PI4 ;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_I4, (void*)&result, parms, number, pValue);
		return result;
	}
	long WriteResult_wrs_64f(long number, WideResult<double> * pWrs)
	{
		long *pValue = (long*)pWrs;
		long result;
		static BYTE parms[] = VTS_I4 VTS_PI4 ;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_I4, (void*)&result, parms, number, pValue);
		return result;
	}
	long WriteResult_wrs_32s_arr(long number, DWORD sizeArray, WideResult<long*> * pWrsArray)
	{
		long *pValue = (long*)pWrsArray;
		long result;
		static BYTE parms[] = VTS_I4  VTS_UI4 VTS_PI4 ;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_I4, (void*)&result, parms, number, sizeArray, pValue);
		return result;
	}
	//long WriteResult_wrs_32s_arr(long number, WideResult<std::vector<long>> * pWrsArray)
	//{
	//	long *pValue = (long*)pWrsArray;
	//	long result;
	//	static BYTE parms[] = VTS_I4 VTS_PI4 ;
	//	InvokeHelper(0xf, DISPATCH_METHOD, VT_I4, (void*)&result, parms, number, pValue);
	//	return result;
	//}
	long WriteResult_wrs_32f_arr(long number, DWORD sizeArray, WideResult<float*> * pWrsArray)
	{
		long *pValue = (long*)pWrsArray;
		long result;
		static BYTE parms[] = VTS_I4 VTS_UI4 VTS_PI4;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_I4, (void*)&result, parms, number, sizeArray, pValue);
		return result;
	}
	//long WriteResult_wrs_32f_arr(long number, WideResult<std::vector<float>> * pWrsArray)
	//{
	//	long *pValue = (long*)pWrsArray;
	//	long result;
	//	static BYTE parms[] = VTS_I4 VTS_PI4 ;
	//	InvokeHelper(0x10, DISPATCH_METHOD, VT_I4, (void*)&result, parms, number, pValue);
	//	return result;
	//}
	long WriteResult_wrs_64f_arr(long number, DWORD sizeArray, WideResult<double*> * pWrsArray)
	{
		long *pValue = (long*)pWrsArray;
		long result;
		static BYTE parms[] = VTS_I4 VTS_UI4 VTS_PI4;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_I4, (void*)&result, parms, number, sizeArray, pValue);
		return result;
	}
	//long WriteResult_wrs_64f_arr(long number, WideResult<std::vector<double>> * pWrsArray)
	//{
	//	long *pValue = (long*)pWrsArray;
	//	long result;
	//	static BYTE parms[] = VTS_I4 VTS_PI4 ;
	//	InvokeHelper(0x11, DISPATCH_METHOD, VT_I4, (void*)&result, parms, number, pValue);
	//	return result;
	//}
	long GetSizeCurParam(DWORD *pSize)
	{
		long result;
		static BYTE parms[] = VTS_PUI4 ;
		InvokeHelper(0x12, DISPATCH_METHOD, VT_I4, (void*)&result, parms, pSize);
		return result;
	}
	long GetSizeMaxParam(DWORD *pSize)
	{
		long result;
		static BYTE parms[] = VTS_PUI4 ;
		InvokeHelper(0x13, DISPATCH_METHOD, VT_I4, (void*)&result, parms, pSize);
		return result;
	}
	long WriteResult_str(long number, DWORD size, PVOID pointer)
	{
		long result;
		long *pValue = (long*)pointer;
		static BYTE parms[] = VTS_I4 VTS_UI4 VTS_PI4 ;
		InvokeHelper(0x14, DISPATCH_METHOD, VT_I4, (void*)&result, parms, number, size, pValue);
		return result;
	}
};