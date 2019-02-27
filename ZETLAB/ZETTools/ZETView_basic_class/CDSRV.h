// CDSRV.h: объявление классов оболочки для элементов управления ActiveX, созданных при помощи Microsoft Visual C++

#pragma once
#include "afx.h"
#include <chaninfo.h>
// Подключаю словарь
#ifdef _TRANSLATE
#include <Dialog_Zet/ConnectToZetDictionary.h>
#endif
/////////////////////////////////////////////////////////////////////////////
// CDSRV

/////////////////////////////////////////////////////////////////////////////
class CDSRV : public CWnd
{
private:
	wchar_t m_str[128];

	// Не было подключения к серверу
	CString m_sMsg1 = L"Не было подключения к серверу";

	// Сервер не загружен на компьютере
	CString m_sMsg2 = L"Сервер не загружен на компьютере";

	// К серверу подключено слишком много программ
	CString m_sMsg3 = L"К серверу подключено слишком много программ";

	// На компьютере не загружается сервер
	CString m_sMsg4 = L"На компьютере не загружается сервер";

	// Мало оперативной памяти или дискового пространства
	CString m_sMsg5 = L"Мало оперативной памяти или дискового пространства";

	// Номер канала меньше нуля
	CString m_sMsg6 = L"Номер канала меньше нуля";

	// Номер канала больше максимально возможного значения
	CString m_sMsg7 = L"Номер канала больше максимально возможного значения";

	// Невозможно создать экземпляр компонента Server
	CString m_sMsg8 = L"Невозможно создать экземпляр компонента Server";

protected:
	DECLARE_DYNCREATE(CDSRV)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xF563E0CD, 0xC570, 0x411B, { 0xBE, 0x7E, 0x8C, 0x45, 0xA1, 0x70, 0x28, 0x95 } };
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
	bool cwndValidate(CWnd* cwnd)	//проверка CWnd на актуальность
	{
		bool result = false;
		if (cwnd!=NULL)
			if (cwnd->m_hWnd != NULL)
			{
				IUnknown* iUnkn = NULL;
				iUnkn = cwnd->GetControlUnknown();
				if (iUnkn != NULL)
					result = true;
				else 
					result = false;
			} else
				result = false;

		return result;
	}

	void AddMessageToWinLog(LPCTSTR message)
	{
		CString str(L"");
		str.Preallocate(MAX_PATH);
		auto parent = this->GetParent();
		if ((parent != NULL) && (parent->m_hWnd != NULL))
		{
			GetWindowModuleFileNameW(parent->m_hWnd, str.GetBuffer(), MAX_PATH);
			PathStripPathW(str.GetBuffer());
			str.ReleaseBuffer();
			str += L": " + (CString)message;
			LPCWSTR lp(str.GetString());
			ReportEventW(RegisterEventSourceW(NULL, L"ZET"),
				EVENTLOG_ERROR_TYPE, NULL, 0xC0020008L, NULL, 1, 0, &lp, NULL);
		}
	}
	
	void SERVER_ErrorMessage(long res, CWnd* cwnd)
	{
		long ret(0);
		DWORD size(128);
		CString windowName(L"");
		cwnd->GetWindowTextW(windowName);
		m_str[0] = 0;
		switch (res)
		{		
		case -1:
#ifdef _TRANSLATE
			ret = zdTranslate(m_sMsg1, L"SRC.ocx", m_str, &size);
			AddMessageToWinLog(windowName + L": " + m_str);
#else
			AddMessageToWinLog(windowName + L": " + m_sMsg1);
#endif
		break;

		case -2:
#ifdef _TRANSLATE
			ret = zdTranslate(m_sMsg2, L"SRC.ocx", m_str, &size);
			AddMessageToWinLog(windowName + L": " + m_str);
#else
			AddMessageToWinLog(windowName + L": " + m_sMsg2);
#endif
		default:break;
		}
	}

	void ErrorMessage_connect(long res, CWnd* cwnd)
	{
		long ret(0);
		DWORD size(128);
		CString windowName(L"");
		cwnd->GetWindowTextW(windowName);
		switch (res)
		{
		case -1:
		case -4:
		case -8:
#ifdef _TRANSLATE
			ret = zdTranslate(m_sMsg2, L"SRC.ocx", m_str, &size);
			AddMessageToWinLog(windowName + L": " + m_str);
#else
			AddMessageToWinLog(windowName + L": " + m_sMsg2);
#endif
			break;
		case -2:
#ifdef _TRANSLATE
			ret = zdTranslate(m_sMsg4, L"SRC.ocx", m_str, &size);
			AddMessageToWinLog(windowName + L": " + m_str);
#else
			AddMessageToWinLog(windowName + L": " + m_sMsg4);
#endif
			break;
		case -3:
		case -7:
#ifdef _TRANSLATE
			ret = zdTranslate(m_sMsg5, L"SRC.ocx", m_str, &size);
			AddMessageToWinLog(windowName + L": " + m_str);
#else
			AddMessageToWinLog(windowName + L": " + m_sMsg5);
#endif
			break;
		default:break;
		}
	}

	void ErrorMessage_IDrequest(long res, CWnd* cwnd)
	{
		long ret(0);
		DWORD size(128);
		CString windowName(L"");
		cwnd->GetWindowTextW(windowName);
		switch (res)
		{
		case -1:
#ifdef _TRANSLATE
			ret = zdTranslate(m_sMsg1, L"SRC.ocx", m_str, &size);
			AddMessageToWinLog(windowName + L": " + m_str);
#else
			AddMessageToWinLog(windowName + L": " + m_sMsg1);
#endif
			break;
		case -2:
#ifdef _TRANSLATE
			ret = zdTranslate(m_sMsg2, L"SRC.ocx", m_str, &size);
			AddMessageToWinLog(windowName + L": " + m_str);
#else
			AddMessageToWinLog(windowName + L": " + m_sMsg2);
#endif
			break;
		case -3:
#ifdef _TRANSLATE
			ret = zdTranslate(m_sMsg6, L"SRC.ocx", m_str, &size);
			AddMessageToWinLog(windowName + L": " + m_str);
#else
			AddMessageToWinLog(windowName + L": " + m_sMsg6);
#endif
			break;
		case -4:
#ifdef _TRANSLATE
			ret = zdTranslate(m_sMsg7, L"SRC.ocx", m_str, &size);
			AddMessageToWinLog(windowName + L": " + m_str);
#else
			AddMessageToWinLog(windowName + L": " + m_sMsg7);
#endif
			break;
		default:break;
		}
	}

	bool createServer(CWnd* cwnd)
	{
		long ret(0);
		DWORD size(128);
		bool result(false);
		CLSID clsid(CLSID_NULL);
		HRESULT res(NULL);
		res = CLSIDFromProgID(L"SRV.SRVCtrl.1", &clsid);
		if (res == S_OK)
		{
			if (CreateControl(clsid, L"", WS_CHILD, CRect(0, 0, 0, 0), cwnd, 0) != FALSE)
			{
				if (cwndValidate(this))
				{	//аналог server(connect);
					result = true;
					long retVal = Connect();
					//если произошла ошибка - то выведем подробную информацию
					if (retVal != 0)
						ErrorMessage_connect(retVal, cwnd);
				}
			}
		}
		if (!result)
		{
#ifdef _TRANSLATE
			ret = zdTranslate(m_sMsg8, L"SRC.ocx", m_str, &size);
			AddMessageToWinLog(m_str);
#else
			AddMessageToWinLog(m_sMsg8);
#endif
		}
		return result;
	}


// Операции
public:
	long detectChanNum(long ID)
	{//перебираем все каналы и ищем нужный по ID
		long tempID;
		long lQuantity = QuanChan();
		for (long j = 0; j<lQuantity; ++j)
		{
			IdChan(j, &tempID);
			if (tempID == ID)
				return j;
		}
		return -1;
	};

	long detectChanNum(const GUID &guid)
	{//перебираем все каналы и ищем нужный по GUID
		GUID temp(GUID_NULL);
		long lQuantity = QuanChan();
		for (long j = 0; j<lQuantity; ++j)
		{
			IdChan(j, &temp);
			if (temp == guid)
				return j;
		}
		return -1;
	};

	DATE GetTimeSRVasDATE(double timeSRV)
	{
		COleDateTime odt = COleDateTime( __time64_t( GetStartTime() ) );
		if (odt.m_status != COleDateTime::valid)	// иначе упадёт
			odt = COleDateTime();		// 30.12.1899 00:00:00.00
		return odt.m_dt +
			COleDateTimeSpan( timeSRV / 86400 );	// 86400 = 24 * 60 * 60
	}

	CString ConversionEx(long channel)
	{	// Замена в размерности канала всех L"^2" на L"²"
		CString ret = Conversion(channel);
		if (!ret.IsEmpty())
			ret.Replace(L"^2", L"²");
		return ret;
	}

	long GetChannelParEx(long channel, LPCHANNEL_STRUCT pChannelPar)
	{	// Замена в размерности канала всех L"^2" на L"²"
		long ret = GetChannelPar(channel, (BYTE*)pChannelPar, SIZE_CHANNEL_STRUCT);
		if (pChannelPar->measure[0] != 0)
		{
			WCHAR * pointer = wcschr(pChannelPar->measure, L'^');
			if (pointer != nullptr)
			{
				while (true)
				{
					pointer = wcschr(pointer, L'^');
					if (pointer != nullptr)
					{
						UINT len = wcslen(pointer + 1) + 1;	// + последний 0
						if ((len > 1) && (*(pointer + 1) == L'2'))
						{
							wmemmove_s(pointer, len, pointer + 1, len);
							*pointer = L'²';
						}
						else
							++pointer;
					}
					else
						break;
				}
			}
		}
		return ret;
	}
	//-----------------------------------------------------------------------

	void AboutBox()
	{
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long PointAllInfo(LPDISPATCH * point)
	{
		long result = -101;
		static BYTE parms[] = VTS_PDISPATCH ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x1, DISPATCH_METHOD, VT_I4, (void*)&result, parms, point);
		return result;
	}
	long Connect()
	{
		long result = -101;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x2, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long Disconnect()
	{
		long result = -101;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x3, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	float Frequency(long channel)
	{
		float result = -101.f;
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x4, DISPATCH_METHOD, VT_R4, (void*)&result, parms, channel);
		return result;
	}
	long BufferSize()
	{
		long result = -101;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x5, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long NullPointer(long channel, long decade)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x6, DISPATCH_METHOD, VT_I4, (void*)&result, parms, channel, decade);
		return result;
	}
	long QuanChan()
	{
		long result = -101;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x7, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long Pointer(long channel, long decade)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x8, DISPATCH_METHOD, VT_I4, (void*)&result, parms, channel, decade);
		return result;
	}
	CString Commentary(long channel)
	{
		CString result(L"");
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x9, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, channel);
		return result;
	}
	CString Conversion(long channel)
	{
		CString result(L"");
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0xa, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, channel);
		return result;
	}
	float MaxLevel(long channel)
	{
		float result = -101.f;
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0xb, DISPATCH_METHOD, VT_R4, (void*)&result, parms, channel);
		return result;
	}
	float MinLevel(long channel)
	{
		float result = -101.f;
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0xc, DISPATCH_METHOD, VT_R4, (void*)&result, parms, channel);
		return result;
	}
	float Reference(long channel)
	{
		float result = -101.f;
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0xd, DISPATCH_METHOD, VT_R4, (void*)&result, parms, channel);
		return result;
	}
	float CurLevel(long channel)
	{
		float result = -101.f;
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0xe, DISPATCH_METHOD, VT_R4, (void*)&result, parms, channel);
		return result;
	}
	long PrusEna(long channel)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x17, DISPATCH_METHOD, VT_I4, (void*)&result, parms, channel);
		return result;
	}
	long TypeAdc(long channel)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x18, DISPATCH_METHOD, VT_I4, (void*)&result, parms, channel);
		return result;
	}
	long NumDSP(long channel)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x19, DISPATCH_METHOD, VT_I4, (void*)&result, parms, channel);
		return result;
	}
	long NumPhChan(long channel)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x1a, DISPATCH_METHOD, VT_I4, (void*)&result, parms, channel);
		return result;
	}
	long NumFileUsed(long NumFile)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x1b, DISPATCH_METHOD, VT_I4, (void*)&result, parms, NumFile);
		return result;
	}
	long SetSrvWin(LPUNKNOWN Hwnd)
	{
		long result = -101;
		static BYTE parms[] = VTS_UNKNOWN ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x1c, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Hwnd);
		return result;
	}
	long ConnectDac(long NumberDac)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x1d, DISPATCH_METHOD, VT_I4, (void*)&result, parms, NumberDac);
		return result;
	}
	long DisconnectDac(long NumberDac)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x1e, DISPATCH_METHOD, VT_I4, (void*)&result, parms, NumberDac);
		return result;
	}
	long TalkAbout(long Param, long Value)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x1f, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Param, Value);
		return result;
	}
	double CurrentTime(long channel)
	{
		double result = -101.;
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x20, DISPATCH_METHOD, VT_R8, (void*)&result, parms, channel);
		return result;
	}
	long GetData(long channel, long decade, double time, long size, float * data)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_R8 VTS_I4 VTS_PR4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x21, DISPATCH_METHOD, VT_I4, (void*)&result, parms, channel, decade, time, size, data);
		return result;
	}
	long QuanDSP()
	{
		long result = -101;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x22, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long QuanPhChan(long NumDSP)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x23, DISPATCH_METHOD, VT_I4, (void*)&result, parms, NumDSP);
		return result;
	}
	long PutData(long channel, double time, long size, float * data)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 VTS_R8 VTS_I4 VTS_PR4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x24, DISPATCH_METHOD, VT_I4, (void*)&result, parms, channel, time, size, data);
		return result;
	}
	long DisconnectVrtCh()
	{
		long result = -101;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x25, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long ConnectVrtCh(long NumChan)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x27, DISPATCH_METHOD, VT_I4, (void*)&result, parms, NumChan);
		return result;
	}
	float ShifLevel(long channel)
	{
		float result = -101.f;
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x28, DISPATCH_METHOD, VT_R4, (void*)&result, parms, channel);
		return result;
	}
	CString AFCH(long channel)
	{
		CString result(L"");
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x29, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, channel);
		return result;
	}
	long VrtChnInfo(long virtchannel, BSTR * comment, BSTR * Conversion, float freq, float MaxLevel, float MinLevel, float refer)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 VTS_PBSTR VTS_PBSTR VTS_R4 VTS_R4 VTS_R4 VTS_R4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x2a, DISPATCH_METHOD, VT_I4, (void*)&result, parms, virtchannel, comment, Conversion, freq, MaxLevel, MinLevel, refer);
		return result;
	}
	long NumModule(long channel)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x2b, DISPATCH_METHOD, VT_I4, (void*)&result, parms, channel);
		return result;
	}
	long SetDigOutEnable(long Zmodule, long OutMask)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x2c, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Zmodule, OutMask);
		return result;
	}
	long GetDigOutEnable(long Zmodule)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x2d, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Zmodule);
		return result;
	}
	long GetDigInput(long Zmodule)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x2e, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Zmodule);
		return result;
	}
	long GetDigOutput(long Zmodule)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x2f, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Zmodule);
		return result;
	}
	long SetDigOutput(long Zmodule, long DigOut)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x30, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Zmodule, DigOut);
		return result;
	}
	float GetAttenuation(long channel)
	{
		float result = -101.f;
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x31, DISPATCH_METHOD, VT_R4, (void*)&result, parms, channel);
		return result;
	}
	long SetAttenuation(long channel, float attenuator)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 VTS_R4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x32, DISPATCH_METHOD, VT_I4, (void*)&result, parms, channel, attenuator);
		return result;
	}
	long MaxQuanChanDac()
	{
		long result = -101;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x33, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long IsFreeChanDac(long channel)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x34, DISPATCH_METHOD, VT_I4, (void*)&result, parms, channel);
		return result;
	}
	long SetChanDac(long channel)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x35, DISPATCH_METHOD, VT_I4, (void*)&result, parms, channel);
		return result;
	}
	long RemChanDac(long channel)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x36, DISPATCH_METHOD, VT_I4, (void*)&result, parms, channel);
		return result;
	}
	long OrderConnected()
	{
		long result = -101;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x37, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetPWMEnable(long Zmodule)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x38, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Zmodule);
		return result;
	}
	long SetFreqPWM(long Zmodule, long Rate, long Period)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x39, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Zmodule, Rate, Period);
		return result;
	}
	long SetOnDutyPWM(long Zmodule, long channel, long OnDutyPWM, long ShiftPWM)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x3a, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Zmodule, channel, OnDutyPWM, ShiftPWM);
		return result;
	}
	long RegulatorPWM(long Zmodule, float * data, long size)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 VTS_PR4 VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x3b, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Zmodule, data, size);
		return result;
	}
	long StartPWM(long Zmodule, long channel)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x3c, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Zmodule, channel);
		return result;
	}
	long StopPWM(long Zmodule, long channel)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x3d, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Zmodule, channel);
		return result;
	}
	long IdChanDac(long channel, long * Id)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 VTS_PI4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x3e, DISPATCH_METHOD, VT_I4, (void*)&result, parms, channel, Id);
		return result;
	}
	long IdChan(long channel, long * Id)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 VTS_PI4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x3f, DISPATCH_METHOD, VT_I4, (void*)&result, parms, channel, Id);
		return result;
	}
	long IdChanVirt(long channel, long * Id)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 VTS_PI4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x40, DISPATCH_METHOD, VT_I4, (void*)&result, parms, channel, Id);
		return result;
	}
	float FrequencyDac(long channelDac)
	{
		float result = -101.f;
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x41, DISPATCH_METHOD, VT_R4, (void*)&result, parms, channelDac);
		return result;
	}
	long GetDataVar(long channel, long decade, double time, long size, VARIANT * datvar)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_R8 VTS_I4 VTS_PVARIANT ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x42, DISPATCH_METHOD, VT_I4, (void*)&result, parms, channel, decade, time, size, datvar);
		return result;
	}
	long GetDataNet(long channel, long decade, double time, long size, long pData)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_R8 VTS_I4 VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x43, DISPATCH_METHOD, VT_I4, (void*)&result, parms, channel, decade, time, size, pData);
		return result;
	}
	long PutDataNet(long channel, double time, long size, long pData)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 VTS_R8 VTS_I4 VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x44, DISPATCH_METHOD, VT_I4, (void*)&result, parms, channel, time, size, pData);
		return result;
	}
	long WorkChanADC(long channel)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x45, DISPATCH_METHOD, VT_I4, (void*)&result, parms, channel);
		return result;
	}
	float Sense(long channel)
	{
		float result = -101.f;
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x46, DISPATCH_METHOD, VT_R4, (void*)&result, parms, channel);
		return result;
	}
	long SetBitDigOutput(long Zmodule, long bit)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x47, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Zmodule, bit);
		return result;
	}
	long ClrBitDigOutput(long Zmodule, long bit)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x48, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Zmodule, bit);
		return result;
	}
	CString GetDataGPS(long maxsize, long * realsize)
	{
		CString result(L"");
		static BYTE parms[] = VTS_I4 VTS_PI4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x49, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, maxsize, realsize);
		return result;
	}
	long PutDataGPS(LPCTSTR nmea, long size)
	{
		long result = -101;
		static BYTE parms[] = VTS_BSTR VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x4a, DISPATCH_METHOD, VT_I4, (void*)&result, parms, nmea, size);
		return result;
	}
	long IsFileGPS()
	{
		long result = -101;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x4b, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long SetShiftGPS(float seconds)
	{
		long result = -101;
		static BYTE parms[] = VTS_R4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x4c, DISPATCH_METHOD, VT_I4, (void*)&result, parms, seconds);
		return result;
	}
	float GetShiftGPS()
	{
		float result = -101.f;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x4d, DISPATCH_METHOD, VT_R4, (void*)&result, NULL);
		return result;
	}
	double GetStartTime()
	{
		double result = -101.;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x4e, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	long enaGPSync(long enable)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x4f, DISPATCH_METHOD, VT_I4, (void*)&result, parms, enable);
		return result;
	}
	long advanceGPS(long seconds)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x50, DISPATCH_METHOD, VT_I4, (void*)&result, parms, seconds);
		return result;
	}
	long satGPS(long satinfo)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x51, DISPATCH_METHOD, VT_I4, (void*)&result, parms, satinfo);
		return result;
	}
	double WorkingTime()
	{
		double result = -101.;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x52, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	CString GroupName(long channel)
	{
		CString result(L"");
		static BYTE parms[] = VTS_I4;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x53, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, channel);
		return result;
	}
	long SetGroupName(long virtchannel, LPCTSTR GrName)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 VTS_BSTR ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x54, DISPATCH_METHOD, VT_I4, (void*)&result, parms, virtchannel, GrName);
		return result;
	}
	float GetFrequency(long Modul)
	{
		float result = -101.f;
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x56, DISPATCH_METHOD, VT_R4, (void*)&result, parms, Modul);
		return result;
	}
	float SetFrequency(long Modul, float freq)
	{
		float result = -101.f;
		static BYTE parms[] = VTS_I4 VTS_R4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x57, DISPATCH_METHOD, VT_R4, (void*)&result, parms, Modul, freq);
		return result;
	}
	float GetNextFreq(long Modul, long Par)
	{
		float result = -101.f;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x58, DISPATCH_METHOD, VT_R4, (void*)&result, parms, Modul, Par);
		return result;
	}
	long OnOffChannel(long Modul, long channel, long Par)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x59, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Modul, channel, Par);
		return result;
	}
	long SetSinDiffChannel(long Modul, long channel, long Par)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x5a, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Modul, channel, Par);
		return result;
	}
	long GetDigBits(long Modul)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x5b, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Modul);
		return result;
	}
	CString DacGroupName(long dacchannel)
	{
		CString result(L"");
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x5c, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, dacchannel);
		return result;
	}
	long Triger(long Modul, long Parametr, long * ValInt, float * ValFloat)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PI4 VTS_PR4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x5d, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Modul, Parametr, ValInt, ValFloat);
		return result;
	}
	long SetChannelStatus(long channel, long status)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL) InvokeHelper(0x5e, DISPATCH_METHOD, VT_I4, (void*)&result, parms, channel, status);
		return result;
	}
	long GetStatus(long channel)
	{/* Возвращаемые значения (статус канала):
		-4: channel >= QuanChan
		-3: channel < 0
		-2: не запущен ZetServer.exe
		-1: не было Connect
		0: АЦП (может быть сетевым)
		1: ЦАП
		2: виртуальный
		3: цифровой
		4: отключенный канал АЦП
		5: канал отключенного АЦП
		6: интеллектуальный датчик (серия 7000, FreqADC может быть больше 100 Гц)
		7: отключенный интеллектуальный датчик
		8: быстрый канал (с большим FreqADC, который данные выдаёт пачками, по таймеру или по событию)
		9: неактивный виртуальный канал*/
		long result = -101;
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x5f, DISPATCH_METHOD, VT_I4, (void*)&result, parms, channel);
		return result;
	}
	long DecadeBufferSize(long decade)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x60, DISPATCH_METHOD, VT_I4, (void*)&result, parms, decade);
		return result;
	}

	long Start()
	{
		long result = -101;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x61, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}

	long Stop()
	{
		long result = -101;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x62, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}

	long SetICP(long lModule, long lChannel, long lType)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x63, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lModule, lChannel, lType);
		return result;
	}

	long GetICP(long lModule, long lChannel)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 VTS_I4;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x64, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lModule, lChannel);
		return result;
	}

	float GetAmplify(long channel)
	{
		float result = -101.f;
		static BYTE parms[] = VTS_I4 ;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x65, DISPATCH_METHOD, VT_R4, (void*)&result, parms, channel);
		return result;
	}

	long SetStartUTCTime(unsigned __int64 UtcTime)
	{
		long result = -101;
		static BYTE parms[] = VTS_UI8;
		/*if (this != NULL)*/ if (this->m_hWnd != NULL)
			InvokeHelper(0x66, DISPATCH_METHOD, VT_I4, (void*)&result, parms, UtcTime);
		return result;
	}

	long TimeGPS(unsigned __int64 UtcTime)
	{
		long result = -101;
		static BYTE parms[] = VTS_UI8;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x67, DISPATCH_METHOD, VT_I4, (void*)&result, parms, UtcTime);
		return result;
	}

	long GetDatawFreq(long lChannel, float fFrequency, double dTime, long lSize, float* pData)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4 VTS_R4 VTS_R8 VTS_I4 VTS_PR4;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x68, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lChannel, fFrequency, dTime, lSize, pData);
		return result;
	}

	long SetStatus(long lVirtChannel, long lStatus)
	{
		long result(-101);
		static BYTE parms[] = VTS_I4 VTS_I4;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x69, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lVirtChannel, lStatus);
		return result;
	}

	long PutPulse(long lChannel, double dTime, long lSize, float* pData)
	{
		long result(-101);
		static BYTE parms[] = VTS_I4 VTS_R8 VTS_I4 VTS_PR4;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x6a, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lChannel, dTime, lSize, pData);
		return result;
	}

	long GetInfLast(long lChannel, double* pTime, long* pSize, long* pIndex)
	{
		long result(-101);
		static BYTE parms[] = VTS_I4 VTS_PR8 VTS_PI4 VTS_PI4;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x6b, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lChannel, pTime, pSize, pIndex);
		return result;
	}

	long GetInfIndex(long lChannel, double* pTime, long* pSize, long lIndex)
	{
		long result(-101);
		static BYTE parms[] = VTS_I4 VTS_PR8 VTS_PI4 VTS_I4;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x6c, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lChannel, pTime, pSize, lIndex);
		return result;
	}

	long GetPulse(long lChannel, long lIndex, double* pTime, long* lSize, float* pData)
	{
		long result(-101);
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PR8 VTS_PI4 VTS_PR4;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x6d, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lChannel, lIndex, pTime, lSize, pData);
		return result;
	}

	long SetID(long lVirtChannel, long lID)
	{
		long result(-101);
		static BYTE parms[] = VTS_I4 VTS_I4;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x6e, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lVirtChannel, lID);
		return result;
	}

	long AddVirtualChannel(long lIndex, long lQuantity)
	{
		long result(-101);
		static BYTE parms[] = VTS_I4 VTS_I4;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x6f, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lIndex, lQuantity);
		return result;
	}

	long DeleteVirtualChannel(LONG lIndex, LONG lQuantity)
	{
		long result(-101);
		static BYTE parms[] = VTS_I4 VTS_I4;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x70, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lIndex, lQuantity);
		return result;
	}

	long GetWatermarkFlag (LONG lChannel, LONG* pFlag)
	{
		long result(-101);
		static BYTE parms[] = VTS_I4 VTS_PI4;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x71, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lChannel, pFlag);
		return result;
	}

	long SetID(long virtChannel_, GUID id_)
	{
		long result(-101);
		CString strGUID(L"");
		static BYTE parms[] = VTS_I4 VTS_BSTR;

		long long tempLongLong, tempLongLong2;
		tempLongLong  = *(long long*)( &id_ );
		tempLongLong2 = *(long long*)( (long)&id_ + 8);
		
		strGUID.Format(L"%I64x_%I64x", tempLongLong, tempLongLong2);
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x72, DISPATCH_METHOD, VT_I4, (void*)&result, parms, virtChannel_, strGUID);
		return result;
	}

	long IdChan(long virtChannel_, GUID* id_)
	{
		long result(-101);
		static BYTE parms[] = VTS_I4 VTS_PI4;
		
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x73, DISPATCH_METHOD, VT_I4, (void*)&result, parms, virtChannel_, (long*)id_);
		return result;
	}

	long IdChanVirt(long virtChannel_, GUID* id_)
	{
		long result(-101);
		static BYTE parms[] = VTS_I4 VTS_PI4;

		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x74, DISPATCH_METHOD, VT_I4, (void*)&result, parms, virtChannel_, (long*)id_);
		return result;
	}

	// Возвращаемое значение
	// 0 - реальный режим
	// 1 - демо-режим
	// 2 - режим нормального воспроизведения
	// 3 - режим ускоренного воспроизведения
	// иначе - код ошибки
	LONG GetMode()
	{
		LONG result(0);
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x75, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}

	LONG GetChannelPar(LONG channel, BYTE* pChannelPar, ULONG size)
	{
		long result(-101);
		static BYTE parms[] = VTS_I4 VTS_PUI1 VTS_UI4;

		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x76, DISPATCH_METHOD, VT_I4, (void*)&result, parms, channel, pChannelPar, size);
		return result;
	}

	LONG GetChannelPar2(LONG channel, BYTE* pChannelPar, ULONG size)
	{
		long result(-101);
		static BYTE parms[] = VTS_I4 VTS_PUI1 VTS_UI4;

		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x77, DISPATCH_METHOD, VT_I4, (void*)&result, parms, channel, pChannelPar, size);
		return result;
	}
	long GetQuality(long channel)
	{
		long result = -101;
		static BYTE parms[] = VTS_I4;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x78, DISPATCH_METHOD, VT_I4, (void*)&result, parms, channel);
		return result;
	}

	long SetDigitalChannelEvent(GUID* guid, LONG seconds, LONG nanoseconds, LONG front)
	{
		long result(-101);

		static BYTE parms[] = VTS_PI4 VTS_I4 VTS_I4 VTS_I4;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x79, DISPATCH_METHOD, VT_I4, (void*)&result, parms, (long*)guid, seconds, nanoseconds, front);

		return result;
	}

	long SetDigitalVirtChannelEvent(LONG channel, LONG seconds, LONG nanoseconds, LONG front)
	{
		long result;

		// Берем GUID канала
		GUID chanId;
		result = IdChanVirt(channel, &chanId);

		// Если GUID получилось получить
		if (result >= 0)
			result = SetDigitalChannelEvent(&chanId, seconds, nanoseconds, front);

		return result;
	}

	long SetDigitalChannelEvent(LONG channel, LONG seconds, LONG nanoseconds, LONG front)
	{
		long result;

		// Берем GUID канала
		GUID chanId;
		result = IdChan(channel, &chanId);

		// Если GUID получилось получить
		if (result == 0)
			result = SetDigitalChannelEvent(&chanId, seconds, nanoseconds, front);

		return result;
	}

	long SetStartChannelEvent(GUID* guid, LONG seconds, LONG nanoseconds)
	{
		long result(-101);

		static BYTE parms[] = VTS_PI4 VTS_I4 VTS_I4;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x7A, DISPATCH_METHOD, VT_I4, (void*)&result, parms, (long*)guid, seconds, nanoseconds);

		return result;
	}

	long SetStartVirtChannelEvent(LONG channel, LONG seconds, LONG nanoseconds)
	{
		long result;

		// Берем GUID канала
		GUID chanId;
		result = IdChanVirt(channel, &chanId);

		// Если GUID получилось получить
		if (result >= 0)
			result = SetStartChannelEvent(&chanId, seconds, nanoseconds);

		return result;
	}

	long SetStartChannelEvent(LONG channel, LONG seconds, LONG nanoseconds)
	{
		long result;

		// Берем GUID канала
		GUID chanId;
		result = IdChan(channel, &chanId);

		// Если GUID получилось получить
		if (result == 0)
			result = SetStartChannelEvent(&chanId, seconds, nanoseconds);

		return result;
	}

	long SetEndChannelEvent(GUID* guid, LONG seconds, LONG nanoseconds)
	{
		long result(-101);

		static BYTE parms[] = VTS_PI4 VTS_I4 VTS_I4;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x7B, DISPATCH_METHOD, VT_I4, (void*)&result, parms, (long*)guid, seconds, nanoseconds);

		return result;
	}

	long SetEndVirtChannelEvent(LONG channel, LONG seconds, LONG nanoseconds)
	{
		long result;

		// Берем GUID канала
		GUID chanId;
		result = IdChanVirt(channel, &chanId);

		// Если GUID получилось получить
		if (result >= 0)
			result = SetEndChannelEvent(&chanId, seconds, nanoseconds);

		return result;
	}

	long SetEndChannelEvent(LONG channel, LONG seconds, LONG nanoseconds)
	{
		long result;

		// Берем GUID канала
		GUID chanId;
		result = IdChan(channel, &chanId);

		// Если GUID получилось получить
		if (result == 0)
			result = SetEndChannelEvent(&chanId, seconds, nanoseconds);

		return result;
	}

	long SetProcessChannelEvent(GUID* guid, LONG seconds, LONG nanoseconds, LONG process)
	{
		long result(-101);

		static BYTE parms[] = VTS_PI4 VTS_I4 VTS_I4 VTS_I4;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x7C, DISPATCH_METHOD, VT_I4, (void*)&result, parms, (long*)guid, seconds, nanoseconds, process);

		return result;
	}

	long SetProcessVirtChannelEvent(LONG channel, LONG seconds, LONG nanoseconds, LONG process)
	{
		long result;

		// Берем GUID канала
		GUID chanId;
		result = IdChanVirt(channel, &chanId);

		// Если GUID получилось получить
		if (result >= 0)
		{
			result = SetProcessChannelEvent(&chanId, seconds, nanoseconds, process);
			//TRACE(L"Modbus channel %d seconds %d, nanoseconds %d, process %d\n", channel, seconds, nanoseconds, process);
		}

		return result;
	}

	long SetProcessChannelEvent(LONG channel, LONG seconds, LONG nanoseconds, LONG process)
	{
		long result;

		// Берем GUID канала
		GUID chanId;
		result = IdChan(channel, &chanId);

		// Если GUID получилось получить
		if (result == 0)
			result = SetProcessChannelEvent(&chanId, seconds, nanoseconds, process);

		return result;
	}

	long SetCurLevel(LONG channel, FLOAT level)
	{
		long result(-101);

		static BYTE parms[] = VTS_I4 VTS_R4;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x7D, DISPATCH_METHOD, VT_I4, (void*)&result, parms, channel, level);

		return result;
	}

	long IsFeedBackChan(LONG channel)
	{
		long result(-101);
		static BYTE parms[] = VTS_I4;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x7e, DISPATCH_METHOD, VT_I4, (void*)&result, parms, channel);
		return result;
	}
	long IsHaveSineBuiltinGen(long Modul)
	{
		long result(-101);
		static BYTE parms[] = VTS_I4;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x7f, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Modul);
		return result;
	}
	long GetSineBuiltinGenParam(long Modul, long * Enabled, double * freq, double * level, double * Offset)
	{
		long result(-101);
		static BYTE parms[] = VTS_I4 VTS_PI4 VTS_PR8 VTS_PR8 VTS_PR8;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x80, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Modul, Enabled, freq, level, Offset);
		return result;
	}
	long SetSineBuiltinGenParam(long Modul, long Enabled, double freq, double level, double Offset)
	{
		long result(-101);
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_R8 VTS_R8 VTS_R8;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x81, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Modul, Enabled, freq, level, Offset);
		return result;
	}
	long SetEnableBuiltinGenerator(long Modul, long enable)
	{
		long result(-101);
		static BYTE parms[] = VTS_I4 VTS_I4;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x82, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Modul, enable);
		return result;
	}
	long GetEnableBuiltinGenerator(long Modul, long * Enabled)
	{
		long result(-101);
		static BYTE parms[] = VTS_I4 VTS_PI4;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x83, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Modul, Enabled);
		return result;
	}
	long Get1PPSBuiltinGen(long Modul, long * Enabled)
	{
		long result(-101);
		static BYTE parms[] = VTS_I4 VTS_PI4;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x8f, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Modul, Enabled);
		return result;
	}
	long Set1PPSBuiltinGen(long Modul, long enable)
	{
		long result(-101);
		static BYTE parms[] = VTS_I4 VTS_I4;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x90, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Modul, enable);
		return result;
	}
	long PutDACDataByPointer(long channel, long PointerDAC, long size, float * data)
	{
		long result(-101);
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_PR4;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x85, DISPATCH_METHOD, VT_I4, (void*)&result, parms, channel, PointerDAC, size, data);
		return result;
	}
	long GetADCInfoExt(long channel, PADC_INFO_EXT pADCInfoExt)
	{
		long result(-101);
		static BYTE parms[] = VTS_I4 VTS_PUI1 VTS_UI4;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x86, DISPATCH_METHOD, VT_I4, (void*)&result, parms, channel, (BYTE*)pADCInfoExt, sizeof(ADC_INFO_EXT));
		return result;
	}
	long GetModulelInfoExt(long Modul, PADC_INFO_EXT pADCInfoExt)
	{
		long result(-101);
		static BYTE parms[] = VTS_I4 VTS_PUI1 VTS_UI4;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x92, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Modul,   (BYTE*)pADCInfoExt, sizeof(ADC_INFO_EXT));
		return result;
	}
	long PutDataByte(long channel, double time, long size, unsigned char * data, long Alignment)
	{
		long result(-101);
		static BYTE parms[] = VTS_I4 VTS_R8 VTS_I4 VTS_PUI1 VTS_I4;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x87, DISPATCH_METHOD, VT_I4, (void*)&result, parms, channel, time, size, data, Alignment);
		return result;
	}
	long SetModeDACMUX(long Modul, long Mode, long Enable)
	{
		long result(-101);
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x88, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Modul, Mode, Enable);
		return result;
	}
	long GetWaitDACMUX(long Modul, long * WaitCode)
	{
		long result(-101);
		static BYTE parms[] = VTS_I4 VTS_PI4;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x89, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Modul, WaitCode);
		return result;
	}
	long SetWaitDACMUX(long Modul, long WaitCode)
	{
		long result(-101);
		static BYTE parms[] = VTS_I4 VTS_I4;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x8a, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Modul, WaitCode);
		return result;
	}
	long ResetWaitDACMUX(long Modul, long WaitCode)
	{
		long result(-101);
		static BYTE parms[] = VTS_I4 VTS_I4;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x8b, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Modul, WaitCode);
		return result;
	}
	double GetRealCurrentTime(long channel)
	{
		double result(-1);
		static BYTE parms[] = VTS_I4;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x8c, DISPATCH_METHOD, VT_R8, (void*)&result, parms, channel);
		return result;
	}
	double GetTimeFromStartAdc()
	{
		double result = -101.;
		/*if (this != NULL) */if (this->m_hWnd != NULL) 
			InvokeHelper(0x8d, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	uint16_t Hpf(long channel)
	{
		uint16_t result = 0;
		static BYTE parms[] = VTS_I4;
		/*if (this != NULL) */if (this->m_hWnd != NULL)
			InvokeHelper(0x8e, DISPATCH_METHOD, VT_UI2, (void*)&result, parms, channel);
		return result;
	}


	// Properties
	//

	BOOL GetUseConvertUnitByXML()
	{
		BOOL result;
		GetProperty(0x91, VT_BOOL, (void*)&result);
		return result;
	}
	void SetUseConvertUnitByXML(BOOL propVal)
	{
		SetProperty(0x91, VT_BOOL, propVal);
	}


};
//------------------------------------------------------------------------------
struct Channel_SRV
{
	bool bSet;		// задан правильно
	bool bStatus;	// статус хороший
	bool bValue;	// значение входит в ОДЗ
	BYTE counter;	// счётчик
	long number;	// номер
	long id;		// идентификатор
	CString name;	// имя канала

	Channel_SRV()
		: bSet(false)
		, bStatus(false)
		, bValue(true)
		, counter(0)
		, number(-1)
		, id(0)
		, name(L"")
	{}

	void GetStatus(CDSRV *psrv)
	{
		bStatus = number >= 0;
		if (bStatus)
		{
			auto sta = psrv->GetStatus(number);
			bStatus = (sta >= 0) && (sta != 4) && (sta != 5) && (sta != 7) && (sta != 9);
			// 4 - отключенный канал АЦП; 5 - канал отключенного АЦП
			// 7 - отключенный канал интеллектуального датчика
			// 9 - неактивный виртуальный канал
		}
	}

	void SetChannel(const long chan, CDSRV *psrv)
	{
		number = chan;
		bSet = number >= 0;
		if (bSet)
		{
			psrv->IdChan(number, &id);
			name = psrv->Commentary(number);
		}
		else
			name.Empty();
		GetStatus(psrv);
	}

	void Modify(CDSRV *psrv)
	{
		if (id)
		{
			number = psrv->detectChanNum(id);
			bSet = number >= 0;
			GetStatus(psrv);
			name = psrv->Commentary(number);
		}
		else
		{
			bSet = bStatus = false;
			name.Empty();
		}
	}
};
//------------------------------------------------------------------------------