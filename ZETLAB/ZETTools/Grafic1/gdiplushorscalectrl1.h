#pragma once

// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CGdiplushorscalectrl1 wrapper class

class CGdiplushorscalectrl1 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CGdiplushorscalectrl1)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x5242B3CD, 0x2FC2, 0x4D53, { 0x80, 0xD8, 0x84, 0x10, 0xFE, 0x29, 0x99, 0xAA } };
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

// _DGdiPlusHorScale

// Functions
//

	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

// Properties
//

double GetAmplitude()
{
	double result;
	GetProperty(0x1, VT_R8, (void*)&result);
	return result;
}
void SetAmplitude(double propVal)
{
	SetProperty(0x1, VT_R8, propVal);
}
long GetLinLog()
{
	long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}
void SetLinLog(long propVal)
{
	SetProperty(0x2, VT_I4, propVal);
}
double GetMemory()
{
	double result;
	GetProperty(0x3, VT_R8, (void*)&result);
	return result;
}
void SetMemory(double propVal)
{
	SetProperty(0x3, VT_R8, propVal);
}
unsigned long GetbackColor()
{
	unsigned long result;
	GetProperty(0x4, VT_UI4, (void*)&result);
	return result;
}
void SetbackColor(unsigned long propVal)
{
	SetProperty(0x4, VT_UI4, propVal);
}
BOOL GetHorisontal()
{
	BOOL result;
	GetProperty(0x5, VT_BOOL, (void*)&result);
	return result;
}
void SetHorisontal(BOOL propVal)
{
	SetProperty(0x5, VT_BOOL, propVal);
}
BOOL GetLeftBound()
{
	BOOL result;
	GetProperty(0x6, VT_BOOL, (void*)&result);
	return result;
}
void SetLeftBound(BOOL propVal)
{
	SetProperty(0x6, VT_BOOL, propVal);
}
BOOL GetRightBound()
{
	BOOL result;
	GetProperty(0x7, VT_BOOL, (void*)&result);
	return result;
}
void SetRightBound(BOOL propVal)
{
	SetProperty(0x7, VT_BOOL, propVal);
}
BOOL GetTopBound()
{
	BOOL result;
	GetProperty(0x8, VT_BOOL, (void*)&result);
	return result;
}
void SetTopBound(BOOL propVal)
{
	SetProperty(0x8, VT_BOOL, propVal);
}
BOOL GetBottomBound()
{
	BOOL result;
	GetProperty(0x9, VT_BOOL, (void*)&result);
	return result;
}
void SetBottomBound(BOOL propVal)
{
	SetProperty(0x9, VT_BOOL, propVal);
}
float GetDiapason()
{
	float result;
	GetProperty(0xa, VT_R4, (void*)&result);
	return result;
}
void SetDiapason(float propVal)
{
	SetProperty(0xa, VT_R4, propVal);
}


};