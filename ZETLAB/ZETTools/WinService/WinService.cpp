#include "WinService.h"

#include <ZETSimple\include\debugtools\debugtools_auto_link.h>
#include <ZETSimple\include\debugtools\DebugTools.h>

#include <time.h>
//----------------------------------------------------------------------------------------------------------------------
CWinService::CWinService(_In_ const uint32_t sleepTime) 
: m_acceptedControl(SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN)
, m_sleepTime(sleepTime)
, m_hSvcStopEvent(NULL)
, m_type(0)
{
	m_time = { 0 };
	m_sServiceName = WS_SERVICE_NAME;
	m_sServiceDisplayName = WS_SERVICE_DISPLAY_NAME;
	m_sServiceDescription = WS_SERVICE_DESCRIPTION;

	trace(_T("CWinService::CWinService() - infinite with period %d ms\n"), m_sleepTime);
}
//----------------------------------------------------------------------------------------------------------------------
CWinService::CWinService(_In_ const SYSTEMTIME& st, _In_ const uint32_t period)
: m_acceptedControl(SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN)
, m_sleepTime(period * 1000)
, m_time(st)
, m_hSvcStopEvent(NULL)
, m_type(1)
{
	m_sServiceName = WS_SERVICE_NAME;
	m_sServiceDisplayName = WS_SERVICE_DISPLAY_NAME;
	m_sServiceDescription = WS_SERVICE_DESCRIPTION;

	trace(_T("CWinService::CWinService() - event from [%04u.%02u.%02u %02u:%02u:%02u.%03u] with period %d s\n"),
		m_time.wYear,
		m_time.wMonth,
		m_time.wDay,
		m_time.wHour,
		m_time.wMinute,
		m_time.wSecond,
		m_time.wMilliseconds,
		m_sleepTime / 1000);
}
//----------------------------------------------------------------------------------------------------------------------
CWinService::~CWinService() 
{
	OutputDebugString(_T("CWinService::~CWinService()\n"));
}
//----------------------------------------------------------------------------------------------------------------------
const LPWSTR CWinService::getServiceName() 
{
	return m_sServiceName;
}
//----------------------------------------------------------------------------------------------------------------------
const LPWSTR CWinService::getServiceDName() 
{
	return m_sServiceDisplayName;
}
//----------------------------------------------------------------------------------------------------------------------
const LPWSTR CWinService::getServiceDescription()
{
	return m_sServiceDescription;
}
//----------------------------------------------------------------------------------------------------------------------
SERVICE_STATUS CWinService::getStatus() 
{
	return m_svcStatus;
}
//----------------------------------------------------------------------------------------------------------------------
bool CWinService::serviceInit() 
{
	OutputDebugString(_T("CWinService::serviceInit()\n"));
	return true;
}
//----------------------------------------------------------------------------------------------------------------------
bool CWinService::serviceProcess() 
{
	OutputDebugString(_T("CWinService::serviceProcess()\n"));
	return true;
}
//----------------------------------------------------------------------------------------------------------------------
bool CWinService::serviceStop() 
{
	OutputDebugString(_T("CWinService::serviceStop()\n"));
	return true;
}
//----------------------------------------------------------------------------------------------------------------------
/*bool CWinService::servicePause() 
{
	OutputDebugString(_T("CWinService::servicePause()\n"));
	return true;
}*/
//----------------------------------------------------------------------------------------------------------------------
/*bool CWinService::serviceContinue() 
{
	OutputDebugString(_T("CWinService::serviceContinue()\n"));
	return true;
}*/
//----------------------------------------------------------------------------------------------------------------------
void CWinService::svcReportEvent(_In_ LPTSTR szFunction, _In_ int eventID) 
{
	int lastError = GetLastError();

	if (nullptr == m_pServiceInstance)
		throw SERVICE_INSTANCE_EXP_1;

	HANDLE hEventSource;
    LPCTSTR lpszStrings[2];
	TCHAR Buffer[80] = {0};

	hEventSource = RegisterEventSource(NULL, m_pServiceInstance->getServiceName());
	if(NULL != hEventSource) 
	{
        StringCchPrintf(Buffer, 80, TEXT("%s failed with %d"), szFunction, lastError);

        lpszStrings[0] = m_pServiceInstance->getServiceName();
        lpszStrings[1] = Buffer;

        ReportEvent(hEventSource,        
                    EVENTLOG_ERROR_TYPE, 
                    0,                   
                    eventID,			 
                    NULL,                
                    2,                   
                    0,                   
                    lpszStrings,         
                    NULL);               

        DeregisterEventSource(hEventSource);
    }
}
//----------------------------------------------------------------------------------------------------------------------
void CWinService::reportSvcStatus(_In_ DWORD dwCurrentState, _In_ DWORD dwWin32ExitCode, _In_ DWORD dwWaitHint) 
{
	trace(_T("CWinService::reportSvcStatus()\n"));
	
    static DWORD dwCheckPoint = 1;

    m_svcStatus.dwCurrentState = dwCurrentState;
	if (dwCurrentState == SERVICE_STOP_PENDING && m_type == 1)
		SetEvent(m_hWaitEvent[1]);

	//trace(_T("reportSvcStatus: m_svcStatus.dwCurrentState %d\n"), m_svcStatus.dwCurrentState);
	m_svcStatus.dwWin32ExitCode = dwWin32ExitCode;
	m_svcStatus.dwWaitHint = dwWaitHint;

    if (dwCurrentState == SERVICE_START_PENDING) 
		m_svcStatus.dwControlsAccepted = 0;
	else 
		m_svcStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	
    if ((dwCurrentState == SERVICE_RUNNING) || (dwCurrentState == SERVICE_STOPPED))
		m_svcStatus.dwCheckPoint = 0;
	else 
		m_svcStatus.dwCheckPoint = dwCheckPoint++;

    SetServiceStatus(m_svcStatusHandle, &m_svcStatus);
}
//----------------------------------------------------------------------------------------------------------------------
void WINAPI CWinService::svcCtrlHandler(_In_ DWORD dwCtrl)
{
	trace(_T("CWinService::svcCtrlHandler()\n"));

	if (nullptr == m_pServiceInstance) 
		throw SERVICE_INSTANCE_EXP_1;
		
	switch(dwCtrl) 
	{
		case SERVICE_CONTROL_STOP: 
			m_pServiceInstance->reportSvcStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);
			
			WaitForSingleObject(m_pServiceInstance->m_hSvcStopEvent, 5000);
			
			m_pServiceInstance->m_svcStatus.dwWin32ExitCode = 0;
			m_pServiceInstance->m_svcStatus.dwCurrentState = SERVICE_STOPPED;
			//trace(_T("svcCtrlHandler: m_svcStatus.dwCurrentState %d\n"), SERVICE_STOPPED);
			break;
		/*case SERVICE_CONTROL_PAUSE: 
			m_pServiceInstance->reportSvcStatus(SERVICE_PAUSE_PENDING, NO_ERROR, 0);
			
			if (m_pServiceInstance->servicePause()) 
			{
				m_pServiceInstance->m_svcStatus.dwWin32ExitCode = 0;
				m_pServiceInstance->m_svcStatus.dwCurrentState = SERVICE_PAUSED;
			} 
			else 
				m_pServiceInstance->m_svcStatus.dwCurrentState = SERVICE_RUNNING;
			break;
 		case SERVICE_CONTROL_CONTINUE: 
			m_pServiceInstance->reportSvcStatus(SERVICE_CONTINUE_PENDING, NO_ERROR, 0);
			
			if (m_pServiceInstance->serviceContinue()) 
			{
				m_pServiceInstance->m_svcStatus.dwWin32ExitCode = 0;
				m_pServiceInstance->m_svcStatus.dwCurrentState = SERVICE_RUNNING;
			} 
			else
				m_pServiceInstance->m_svcStatus.dwCurrentState = SERVICE_PAUSED;
			break;
		case SERVICE_CONTROL_INTERROGATE: 
			break; */
		default: 
			break;
	}
	m_pServiceInstance->reportSvcStatus(m_pServiceInstance->m_svcStatus.dwCurrentState, NO_ERROR, 0);
}
//----------------------------------------------------------------------------------------------------------------------
void CWinService::installService() 
{
	TCHAR szPath[MAX_PATH] = {0};

    if(!GetModuleFileName(NULL, szPath, MAX_PATH)) 
	{
        printf_s("Cannot install service (%ul)\n", GetLastError());
        return;
    }

	SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (NULL == schSCManager)  
	{
		int lastError = GetLastError();
        printf_s("OpenSCManager failed (%d)\n", lastError);
		if (lastError == 5)
			printf_s("please, try run with admin rights\n");
        return;
    }

	SC_HANDLE schService = CreateService(schSCManager, getServiceName(), getServiceDName(), SERVICE_ALL_ACCESS,
		SERVICE_WIN32_OWN_PROCESS, SERVICE_AUTO_START, SERVICE_ERROR_NORMAL, szPath, NULL, NULL, NULL, NULL, NULL);
	if (NULL == schService) 
	{
        printf_s("CreateService failed (%ul)\n", GetLastError());
        CloseServiceHandle(schSCManager);
        return;
    } 
	else
		printf_s("Service installed successfully\n");

	SERVICE_DESCRIPTION sd = { 0 };
	sd.lpDescription = getServiceDescription();
	if (FALSE == ChangeServiceConfig2(schService, SERVICE_CONFIG_DESCRIPTION, &sd))
		printf_s("ChangeServiceConfig2 failed (%ul)\n", GetLastError());

    CloseServiceHandle(schService); 
    CloseServiceHandle(schSCManager);
}
//----------------------------------------------------------------------------------------------------------------------
void CWinService::removeService() 
{
	SC_HANDLE schSCManager;
	SC_HANDLE schService;
	TCHAR szPath[MAX_PATH] = {0};

	if (!GetModuleFileName(NULL, szPath, MAX_PATH)) 
	{
		printf_s("Cannot remove service (%ul)\n", GetLastError());
		return;
	}

	schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == schSCManager)  
	{
		int lastError = GetLastError();
		printf_s("OpenSCManager failed (%d)\n", lastError);
		if (lastError == 5)
			printf_s("please, try run with admin rights\n");
		return;
	}

	schService = OpenService(schSCManager, getServiceName(), SERVICE_STOP | DELETE);
	if (NULL == schService)
	{
		printf_s("OpenService failed (%ul)\n", GetLastError());
		CloseServiceHandle(schSCManager);
		return;
	}
	else 
	{
		if (FALSE != DeleteService(schService))
			printf_s("Service removed successfully\n");
		else
			printf_s("DeleteService failed (%ul)\n", GetLastError());
	}

	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);
}
//----------------------------------------------------------------------------------------------------------------------
void CWinService::setServiceInstance(_In_ CWinService* Instance)
{
	m_pServiceInstance = Instance;
}
//----------------------------------------------------------------------------------------------------------------------
void CWinService::ServiceProcess(_In_ int argc, _In_ TCHAR *argv[])
{
	if (nullptr == m_pServiceInstance)
		throw SERVICE_INSTANCE_EXP_1;
	
    if(lstrcmpi(argv[1], TEXT("install")) == 0)
	{
		m_pServiceInstance->installService();
        return;
    }
	else if (lstrcmpi(argv[1], TEXT("remove")) == 0)
	{
		m_pServiceInstance->removeService();
		return;
	}

    SERVICE_TABLE_ENTRY DispatchTable[] = 
    { 
		{ m_pServiceInstance->getServiceName(), (LPSERVICE_MAIN_FUNCTION)CWinService::mainProcess },
        { NULL, NULL } 
    }; 
 
    if (!StartServiceCtrlDispatcher(DispatchTable)) 
		CWinService::svcReportEvent(TEXT("StartServiceCtrlDispatcher"), 1);

	OutputDebugString(L"ServiceProcess ended\n");
}
//----------------------------------------------------------------------------------------------------------------------
void CWinService::mainProcess()
{
	if (nullptr == m_pServiceInstance)
		throw SERVICE_INSTANCE_EXP_1;

	trace(_T("CWinService::mainProcess() started\n"));

	m_pServiceInstance->m_svcStatus.dwServiceType = SERVICE_WIN32;
	m_pServiceInstance->m_svcStatus.dwCurrentState = SERVICE_START_PENDING;
	//trace(_T("mainProcess: m_svcStatus.dwCurrentState %d\n"), SERVICE_START_PENDING);
	m_pServiceInstance->m_svcStatus.dwControlsAccepted = m_pServiceInstance->m_acceptedControl;
	m_pServiceInstance->m_svcStatus.dwWin32ExitCode = 0;
	m_pServiceInstance->m_svcStatus.dwServiceSpecificExitCode = 0;
	m_pServiceInstance->m_svcStatus.dwCheckPoint = 0;
	m_pServiceInstance->m_svcStatus.dwWaitHint = 0;
 
	m_pServiceInstance->m_svcStatusHandle = RegisterServiceCtrlHandler(m_pServiceInstance->getServiceName(),
		(LPHANDLER_FUNCTION)CWinService::svcCtrlHandler);
	if (!m_pServiceInstance->m_svcStatusHandle) 
	{
		OutputDebugString(_T("Registering Control Handler failed\n"));
		return;
    }

	if (!m_pServiceInstance->serviceInit()) 
	{
		OutputDebugString(_T("Initialization failed\n"));
		m_pServiceInstance->m_svcStatus.dwCurrentState = SERVICE_STOPPED;
		m_pServiceInstance->m_svcStatus.dwWin32ExitCode = -1;
		SetServiceStatus(m_pServiceInstance->m_svcStatusHandle, &(m_pServiceInstance->m_svcStatus));
        return; 
    } 

	m_pServiceInstance->m_hSvcStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (NULL == m_pServiceInstance->m_hSvcStopEvent)
	{
		OutputDebugString(_T("CreateEvent failed\n"));
		m_pServiceInstance->m_svcStatus.dwCurrentState = SERVICE_STOPPED;
		m_pServiceInstance->m_svcStatus.dwWin32ExitCode = -1;
		SetServiceStatus(m_pServiceInstance->m_svcStatusHandle, &(m_pServiceInstance->m_svcStatus));
		return;
	}

	m_pServiceInstance->m_svcStatus.dwCurrentState = SERVICE_RUNNING;
	//trace(_T("mainProcess: m_svcStatus.dwCurrentState %d\n"), SERVICE_RUNNING);
	SetServiceStatus(m_pServiceInstance->m_svcStatusHandle, &(m_pServiceInstance->m_svcStatus));
 
	if (m_pServiceInstance->m_type == 1)
	{
		m_pServiceInstance->m_hWaitEvent[1] = CreateEvent(NULL, FALSE, FALSE, NULL);
		m_pServiceInstance->m_hWaitEvent[0] = CreateWaitableTimer(NULL, FALSE, NULL);

		FILETIME ftLocal = { 0 }, ftUTC = { 0 };
		LARGE_INTEGER li = { 0 };
		if (SystemTimeToFileTime(&m_pServiceInstance->m_time, &ftLocal) != NULL)// converts to file time format
		{
			li.LowPart  = ftLocal.dwLowDateTime;
			li.HighPart = ftLocal.dwHighDateTime;
		}
		SetWaitableTimer(m_pServiceInstance->m_hWaitEvent[0], &li, m_pServiceInstance->m_sleepTime, NULL, NULL, FALSE);
	}

	while (m_pServiceInstance->m_svcStatus.dwCurrentState == SERVICE_RUNNING)
	{
		bool bCallServiceProcess(true);
		if (m_pServiceInstance->m_type == 1)
		{
			switch (WaitForMultipleObjects(2, m_pServiceInstance->m_hWaitEvent, FALSE, INFINITE))
			{
				case WAIT_OBJECT_0:
					break;
				case (WAIT_OBJECT_0 + 1) :
					bCallServiceProcess = false;
					break;
				default:
					break;
			}
		}

		if (bCallServiceProcess)
		{
			if (!m_pServiceInstance->serviceProcess())
			{
				m_pServiceInstance->m_svcStatus.dwCurrentState = SERVICE_STOPPED;
				m_pServiceInstance->m_svcStatus.dwWin32ExitCode = -1;
				SetServiceStatus(m_pServiceInstance->m_svcStatusHandle, &(m_pServiceInstance->m_svcStatus));
				return;
			}
		}

		if (m_pServiceInstance->m_type == 0)
			Sleep(m_pServiceInstance->m_sleepTime);
	}
	if (m_pServiceInstance->m_type == 1)
		CancelWaitableTimer(m_pServiceInstance->m_hWaitEvent[0]);

	m_pServiceInstance->serviceStop();

	OutputDebugString(_T("CWinService::mainProcess() ended\n"));
	
	SetEvent(m_pServiceInstance->m_hSvcStopEvent);
}
//----------------------------------------------------------------------------------------------------------------------
CWinService* CWinService::m_pServiceInstance = nullptr;
//----------------------------------------------------------------------------------------------------------------------