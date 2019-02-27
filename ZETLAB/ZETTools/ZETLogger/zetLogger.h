#include <windows.h>
#include <future>

#include "BasicZetLogger\basiczetLogger.h"

#define ZETLOGGER_DIRECTORY_NAME_IN_ZETSIGNALS_DIRECTORY L"Logger\\"
class CZetLogger
{
private:
	CBasicZetLogger* _zetLogger;
	std::thread *_pWorkerThread = nullptr;
	std::thread *_pRunningLiveThread = nullptr;
	
	const static long const_max_number_zet_logger_files = 20000;	// ������������ ���������� ����� ��� ������
	const static long const_max_data_size_to_write_in_file = 131072;	// ������������ ������ ����� ������ ��� ������ (128 �����)
	const static long const_max_free_thread_time = 1;	// ��������� ����� ������
	const static long const_mseconds_in_second = 1000;			// ���������� ����������� � �������
	const static long const_duration_measure_baud_rate_in_seconds = 10;				// ���������� ������ ��� ��������� ��������
	const static long const_bytes_in_kbyte = 1024;				// ���������� ���� � ���������

	// ������ ��������� ���� ���������� ������
	static void worker(CZetLogger* zetLogger);
	static void runningLive(CZetLogger* zetLogger);

	// ������ ��������� � ����
	long writeStructuresToFile(const ZetHeader* structures_, long size_);
public:
	enum {
		smallLoggerCircBuffSize = 655360,
		bigLoggerCircBuffSize = 1310720
	};

	CZetLogger();
	~CZetLogger();

	// ����������� ������ ��������� ������ � ����������� ����
	long registerNewStructureLog(const ZetHeader* structure_, long loggerCircBuffSize_);
	// ����� ��������� ������ � ����������� ����
	long findStructureLog(const ZetHeader* structure_);
	// ������ ��������� (���������)
	long writeStructureToLog(const ZetHeader* structure_);
	// ������ ��������� (�������)
	long writeStructureToLog(const ZetHeader* structure_, long numLog_);

	// ������ ������� ������ ��� ������
	long getSizeWrite();
	// ������ ������� ������ ��� ������
	long getSizeRead();
};