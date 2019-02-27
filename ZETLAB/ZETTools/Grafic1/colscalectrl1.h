#pragma once

// Сформированные компьютером классы оболочки IDispatch, созданные при помощи Microsoft Visual C++

// ПРИМЕЧАНИЕ. Не изменяйте содержимое этого класса. Если этот класс будет повторно создан при помощи
//  Microsoft Visual C++, ваши изменения будут перезаписаны.

/////////////////////////////////////////////////////////////////////////////
// Класс оболочки CColscalectrl1

class CColscalectrl1 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CColscalectrl1)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xEA643268, 0x8282, 0x422A, { 0xB3, 0x5D, 0x9B, 0xA3, 0x5, 0x47, 0xE9, 0x83 } };
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

// _DColScale

// Functions
//

	long Redraw()
	{
		long result;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}

// Properties
//

double GetMathly()
{
	double result;
	GetProperty(0x1, VT_R8, (void*)&result);
	return result;
}
void SetMathly(double propVal)
{
	SetProperty(0x1, VT_R8, propVal);
}
double GetMathdy()
{
	double result;
	GetProperty(0x2, VT_R8, (void*)&result);
	return result;
}
void SetMathdy(double propVal)
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
unsigned long GetClrGrd()
{
	unsigned long result;
	GetProperty(0x5, VT_UI4, (void*)&result);
	return result;
}
void SetClrGrd(unsigned long propVal)
{
	SetProperty(0x5, VT_UI4, propVal);
}
unsigned long GetClrDig()
{
	unsigned long result;
	GetProperty(0x6, VT_UI4, (void*)&result);
	return result;
}
void SetClrDig(unsigned long propVal)
{
	SetProperty(0x6, VT_UI4, propVal);
}
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
long GetAutoColor()
{
	long result;
	GetProperty(0x8, VT_I4, (void*)&result);
	return result;
}
void SetAutoColor(long propVal)
{
	SetProperty(0x8, VT_I4, propVal);
}


};
