#ifndef win_service_h
#define win_service_h

#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <stdint.h>

#define SERVICE_INSTANCE_EXP_1	L"ServiceInstance not initialized"

#define WS_SERVICE_NAME			L"ServiceInstance"
#define WS_SERVICE_DISPLAY_NAME L"Service instance"
#define WS_SERVICE_DESCRIPTION	L"Service description"

class CWinService 
{
public:
	static void setServiceInstance(_In_ CWinService* Instance);
	static void ServiceProcess(_In_ int argc, _In_ TCHAR* argv[]);

	int getSleepTime() const { return m_sleepTime; }

protected:
	CWinService(_In_ const uint32_t sleepTime = 1000);
	CWinService(_In_ const SYSTEMTIME& st, _In_ const uint32_t period = 1);
	~CWinService();

	const LPWSTR getServiceName();
	const LPWSTR getServiceDName();
	const LPWSTR getServiceDescription();
	SERVICE_STATUS getStatus(); 

	virtual bool serviceInit();
	virtual bool serviceProcess();
	virtual bool serviceStop();
	//virtual bool servicePause();
	//virtual bool serviceContinue();
	
	void installService();
	void removeService();
	static void mainProcess();

	uint32_t m_acceptedControl;
	uint32_t m_sleepTime;
	SYSTEMTIME m_time;
	
	LPWSTR m_sServiceName;
	LPWSTR m_sServiceDisplayName;
	LPWSTR m_sServiceDescription;

	static CWinService* m_pServiceInstance;

private:
	static void svcReportEvent(_In_ LPTSTR szFunction, _In_ int eventID);
	static void WINAPI svcCtrlHandler(_In_ DWORD dwCtrl);
	void reportSvcStatus(_In_ DWORD dwCurrentState, _In_ DWORD dwWin32ExitCode, _In_ DWORD dwWaitHint);

	SERVICE_STATUS m_svcStatus; 
	SERVICE_STATUS_HANDLE m_svcStatusHandle;
	HANDLE m_hSvcStopEvent;

	uint32_t m_type;
	HANDLE m_hWaitEvent[2];
};

#endif //win_service_h