#ifndef zet_logger_service
#define zet_logger_service

#include <WinService\WinService.h>
#include <ZETLogger\BasicZetLogger\basiczetLogger.h>
#include <ZetErrorCodes.h>
#include <future>

#define ZETLOGGER_SERVICE_LOGGS_NUMBER      4000 // ���������� ����� ��� ������ ������
#define ZETLOGGER_SERVICE_BUFFER_SHARED_MEMORY_NAME   L"Global\\ZETLoggerServiceBuffers"
#define ZETLOGGER_SERVICE_PROCESSOR_FREE_TIME 5	// ����� ����������� ��������� ������ ������

#define ZETLOGGER_SERVICE_NAME				L"ZETLogger"
#define ZETLOGGER_SERVICE_DISPLAY_NAME		L"ZETLogger"
#define ZETLOGGER_SERVICE_DESCRIPTION		L"Service for write Zetlab programms events to log"

#define MSEKOND_IN_SECOND 1000			// ���������� ����������� � �������
#define ZETLOGGER_MAX_DATA_SIZE_TO_WRITE_IN_FILE ZETLOGGER_CIRC_BUFF_SIZE/2   // ������������ ������ ����� ������ ��� ������ (������ ����. ������ �������)

class CZETLoggerService : public CWinService
{
private:
	HANDLE _mapFileZETLoggerServiceBuffers;
	long _workerThreadCount = 0;	// ���������� �������
	std::thread* _workerThread[10]; // ������ ����������� �������� �������
	std::thread* _runningLiveThread = nullptr;

public:
	BasicZetLoggerStruct* _zetLoggsStructs;	// C����� ������� ��� ������ ����������� ������
	CBasicZetLogger _zetLoggs[ZETLOGGER_SERVICE_LOGGS_NUMBER];

	CZETLoggerService(_In_ const uint32_t _sleepTime = 1000);
	CZETLoggerService(_In_ const SYSTEMTIME& _st, _In_ const uint32_t _period = 1);
	~CZETLoggerService();

	bool init() { return serviceInit(); }
	bool process(){ return serviceProcess(); }

protected:
	bool serviceInit() override;
	bool serviceProcess() override;
	bool serviceStop() override;

private:
};

void workerThreadFunction(LPVOID param_, long numberThread_, long countThread_);
void runningLiveThreadFunction(LPVOID param_);
long writeStructuresToFile(const ZetHeader* structure_, long size_);

#endif