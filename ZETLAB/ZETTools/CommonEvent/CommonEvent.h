//------------------------------------------------------------------------------
//	Файл CommonEvent.h
//	Версия от 02.12.2013
//	Зеленоград, ЗАО "ЭТМС", Савостин Ю.М.
//------------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------------
#include "afx.h"
#include "afxwin.h"
#include <vector>
//------------------------------------------------------------------------------
enum TypeCommonEvent
{
	tce_No,
	tce_Start,
	tce_Stop,
	tce_Close
};
//------------------------------------------------------------------------------
struct	TypeObject
{
	HANDLE handle;
	TypeCommonEvent type;
	TypeObject()
	{
		handle = NULL;
		type = tce_No;
	}
};
//------------------------------------------------------------------------------
struct	ParamThread
{
	HANDLE hEvent[2];
	HWND hwnd;
	UINT message;
	ParamThread()
	{
		hEvent[0] = hEvent[1] = NULL;
		hwnd = NULL;
		message = 0;
	}
};
//------------------------------------------------------------------------------
class CCommonEvent
{
private:
	std::vector<TypeObject> m_vectEvent;
	std::vector<TypeObject> m_vectThread;
	ParamThread m_parThread;
	// приватные функции класса ------------------------------
public:
	CCommonEvent();
	virtual ~CCommonEvent();

	// функции свойств

	// функции класса
	bool CreateCommonEvent(TypeCommonEvent valType,
							HWND hwnd = NULL, UINT message = 0);
	bool SetCommonEvent(TypeCommonEvent valType);
	bool ResetCommonEvent(TypeCommonEvent valType);
};
//------------------------------------------------------------------------------