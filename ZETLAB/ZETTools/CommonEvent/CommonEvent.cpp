//------------------------------------------------------------------------------
//	Файл CommonEvent.cpp
//	Версия от 02.12.2013
//	Зеленоград, ЗАО "ЭТМС", Савостин Ю.М.
//------------------------------------------------------------------------------
#include "StdAfx.h"
#include <CommonEvent\CommonEvent.h>
//------------------------------------------------------------------------------
UINT GlobalFunctionThread_CommonEvent(LPVOID pParam)
{
	ParamThread par = *(ParamThread*)pParam;
	switch ( WaitForMultipleObjects(2, par.hEvent, FALSE, INFINITE) )
	{
	case WAIT_OBJECT_0:
		::PostMessage(par.hwnd, par.message, 0, 0);
//	case (WAIT_OBJECT_0 + 1):
	}
	return 0;
}
//------------------------------------------------------------------------------
CCommonEvent::CCommonEvent()
{
	m_parThread.hEvent[0] = m_parThread.hEvent[1] = NULL;
}
//------------------------------------------------------------------------------
CCommonEvent::~CCommonEvent()
{
	if (!m_vectThread.empty())
	{
		SetEvent(m_parThread.hEvent[1]);
		Sleep(100);
		for(auto it=m_vectThread.begin(); it!=m_vectThread.end(); ++it)
			WaitForSingleObject(it->handle, 500);
	}

	if (!m_vectEvent.empty())
		for(auto it=m_vectEvent.begin(); it!=m_vectEvent.end(); ++it)
			CloseHandle(it->handle);

	if (m_parThread.hEvent[1])
		CloseHandle(m_parThread.hEvent[1]);
}
//------------------------------------------------------------------------------
bool CCommonEvent::CreateCommonEvent(
	TypeCommonEvent valType,
	HWND hwnd,
	UINT message)
{
	TypeObject temp;
	bool ret = (valType == tce_Start) || (valType == tce_Stop) || (valType == tce_Close);
	if (ret)
	{
		if (!m_vectEvent.empty())
		{
			for(auto it=m_vectEvent.begin(); it!=m_vectEvent.end(); ++it)
			{
				if (it->type == valType)
				{
					ret = false;
					break;
				}
			}
		}
		if (ret)
		{
			CString str(L"CommonEvent_");
			switch (valType)
			{
			case tce_Start:	str += L"Start"; break;
			case tce_Stop:	str += L"Stop"; break;
			case tce_Close:	str += L"Close";
			}
			temp.type = valType;
			temp.handle = CreateEvent(
				NULL,	// защита по умолчанию
				TRUE,	// с ручным сбросом
				FALSE,	// в занятом состоянии
				str);	// имя

			ret = temp.handle != NULL;
			if (ret)
				m_vectEvent.push_back(temp);
		}
		if ( ret && (hwnd != NULL) && (message != NULL) )
		{
			m_parThread.hEvent[0] = temp.handle;		
			if (m_parThread.hEvent[1] == NULL)	// EventEnd
				m_parThread.hEvent[1] = CreateEvent(NULL, TRUE, FALSE, NULL);
			m_parThread.hwnd = hwnd;
			m_parThread.message = message;
			CWinThread *wt =
				AfxBeginThread( GlobalFunctionThread_CommonEvent, &m_parThread );
			temp.handle = wt->m_hThread;
			m_vectThread.push_back(temp);
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
bool CCommonEvent::SetCommonEvent(TypeCommonEvent val)
{
	bool ret = !m_vectEvent.empty();
	if (ret)
	{
		ret = false;
		for(auto it=m_vectEvent.begin(); it!=m_vectEvent.end(); ++it)
		{
			if (it->type == val)
			{
				ret = SetEvent(it->handle) != FALSE;
				break;
			}
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
bool CCommonEvent::ResetCommonEvent(TypeCommonEvent val)
{
	bool ret = !m_vectEvent.empty();
	if (ret)
	{
		ret = false;
		for(auto it=m_vectEvent.begin(); it!=m_vectEvent.end(); ++it)
		{
			if (it->type == val)
			{
				ret = ResetEvent(it->handle) != FALSE;
				break;
			}
		}
	}
	return ret;
}
//------------------------------------------------------------------------------