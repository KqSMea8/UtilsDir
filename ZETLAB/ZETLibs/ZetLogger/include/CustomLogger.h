//------------------------------------------------------------------------------
/*	
*/
//------------------------------------------------------------------------------
#pragma once

#include <list>
#include <vector>
#include <future>
#include <string>

//#pragma comment(lib, "C:\\Program Files (x86)\\Visual Leak Detector\\lib\\Win32\\vld.lib" )
//#include "C:\\Program Files (x86)\\Visual Leak Detector\\include\\vld.h"  

#include <sqlite3\include\sqlite3.h>
#include <atlstr.h>				// CString без MFC
#include "LoggerExport.h"		// ZETLOGGER_API
#include <zet_struct1.h>		// ZetHeader
#include <ZetErrorCodes.h>		// коды ощибок
#include <Sqlite\ZetRecords.h>	// ZetRecords
#include <ZetFile\include\ZetFile.h>
#include <ZetTools\include\ZetTools.h>
#include <ZETLoggerClient\include\zetloggerclient.h>
//------------------------------------------------------------------------------
#define NUMBER_STRUCTURE_FOR_TRANSACTION	10
#define SLEEP_FOR_RE_CONNECT_MILLISECONDS	300
#define TIME_OUT_FOR_WAIT_MILLISECONDS		10000
#define MAX_SIZE_OF_SINGLE_FILE				100*1024*1024 // 100 Mb
//------------------------------------------------------------------------------
typedef zet::sql::ZetRecord LogRecord;
typedef std::vector<ZetHeader*> RegisterVectorType;
typedef std::pair < std::chrono::system_clock::time_point, zet::SerializedStructure > StructWithTime;

typedef std::list<StructWithTime> WriteDataType;
typedef std::list<LogRecord> ReadDataType;
typedef std::list<std::vector<BYTE> > ReadDataRegisterType;

typedef std::vector<int8_t> Buffer;
typedef std::vector<Buffer> RegisterBufferType;
//------------------------------------------------------------------------------
enum StatusThread : long
{
	stthr_Err_Create = ErrorCodes::Err_Create_Thread,	// ошибка создания потока	
	stthr_Not_Run = ErrorCodes::Err_Thread_Not_Running,	// поток не запущен
	stthr_End = 0,			// поток завершил свою работу
	stthr_Start,			// поток запущен
	stthr_Stop,				// поток приостановлен
	stthr_Sleep,			// поток временно приостановлен
	stthr_Wait_Data,		// поток ждёт поступления данных
	stthr_Wait_Available,	// поток ждёт разрешение доступа
	stthr_Work,				// выполняется работа с данными
	stthr_End_Success,		// поток завершил свою работу успешно
	stthr_End_Error,		// поток завершил свою работу с ошибкой
};
//------------------------------------------------------------------------------
class ZETLOGGER_API CCustomLogger
{
private:
protected:
	CZETLoggerClient* _zetLoggerClient;
	HANDLE m_dbMutex = NULL;
	HANDLE m_pEventHANDLE[2];
	sqlite3 * m_ptrDb = nullptr;

	uint32_t m_counter = 0;
	uint32_t m_chainNumber = 0;		// номер файла в цепочке

	StatusThread m_statusTR_Struct = StatusThread::stthr_Not_Run;
	StatusThread m_statusTW_Struct = StatusThread::stthr_Not_Run;
	StatusThread m_statusTR_Regist = StatusThread::stthr_Not_Run;
	StatusThread m_statusTW_Regist = StatusThread::stthr_Not_Run;

	uint32_t m_numStructToTransation = NUMBER_STRUCTURE_FOR_TRANSACTION;
	DWORD m_timeOutForWaitMilliseconds = TIME_OUT_FOR_WAIT_MILLISECONDS;
	DWORD m_SleepReConnectMilliseconds = SLEEP_FOR_RE_CONNECT_MILLISECONDS;

	std::string m_sLoggerName = "Logger_";
	std::string m_sFullFileName = "";
	std::string m_sBaseFullFileName = "";
	std::string m_password = "";

	RegisterBufferType m_Descriptors;

	const std::string m_SqlQueryCreateDB =
		"create table if not exists 'logger'   (\
'row_guid' guid not null primary key,\
'time_mark' datetime not null,\
'type' integer not null,\
'device_id' bigint not null,\
'module_id' integer not null,\
'process_id' smallint not null,\
'channel_id' smallint not null,\
'seconds' bigint not null,\
'nanoseconds' integer not null,\
'ticks' integer not null,\
'structure' blob not null,\
'validation' smallint not null\
);\
create index if not exists IndexOnTime on logger(time_mark);\
create index if not exists IndexOnDevice on logger(device_id);\
create index if not exists IndexOnDeviceAndTime on logger(time_mark, device_id);\
create index if not exists IndexOnDeviceAndModule on logger(device_id, module_id);\
\
create table if not exists 'descriptors'   (\
'type' integer not null primary key,\
'name' text not null,\
'descriptor' blob not null,\
'validation' smallint not null\
)";

	const std::string m_SqlQueryWriteStructures =
		"INSERT INTO logger\
(row_guid, device_id, module_id, process_id, channel_id, seconds, nanoseconds, ticks, time_mark, structure, validation, type)\
VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";

	const std::string m_SqlQueryWriteRegister =
		"INSERT OR REPLACE INTO descriptors\
(type, name, descriptor, validation)\
VALUES(?, ?, ?, ?)";

	const std::string m_SqlQueryReadStructures =
		"SELECT row_guid, time_mark, \
type, device_id, module_id, process_id, channel_id, \
seconds, nanoseconds, ticks, structure, validation \
FROM logger ";

	const std::string m_SqlQueryReadRegister =
		"SELECT descriptor FROM descriptors";

	WriteDataType m_writeDataCur, m_writeDataNew;
	
	// описатели рабочих потоков логгера
	std::thread *m_pThreadReadStruct = nullptr;
	std::thread *m_pThreadReadRegister = nullptr;
	std::thread *m_pThreadWriteStruct = nullptr;
	std::thread *m_pThreadWriteRegister = nullptr;

	// Функции записи регистационных бинарников в файл
	static void ThreadWriteRegister(CCustomLogger * pLogger, const RegisterVectorType & descriptors);
	static void WriteRegisterToDB(CCustomLogger * pLogger, const RegisterVectorType & descriptors);
	static void CheckLoggerSize(CCustomLogger * pLogger);

	// Функции записи структур в файл
	static void ThreadWriteStructures(CCustomLogger * pLogger);
	static void WaitAvailableMutexWS(CCustomLogger * pLogger, WriteDataType & dataToDB);
	static void WriteStructuresToDB(CCustomLogger * pLogger, WriteDataType & dataToDB);

	// Функции чтения структур из файла
	static void ThreadReadStructures(CCustomLogger * pLogger, const std::string sql, ReadDataType * pData);
	static void ReadStructuresFromDB(CCustomLogger * pLogger, const std::string sql, ReadDataType * pData);

	// Функции чтения регистационных бинарников  из файла
	static void ThreadReadRegister(CCustomLogger * pLogger, const std::string sql, ReadDataRegisterType * pData);
	static void ReadRegisterFromDB(CCustomLogger * pLogger, const std::string sql, ReadDataRegisterType * pData);

	// Функции удаления устаревших файлов
	static void DeleteOldestFile(CCustomLogger * pLogger);

	void Connect(bool waitMutex = true);
	void DisConnect(bool waitMutex = true);
	void ReConnect();
	void Filter2Sql(const std::string sFilter, std::string & sql, std::string stroka);
	void Add2Sql(std::string & sql);
	

public:
	CCustomLogger(
		const std::string loggerName = "",
		const std::string fileName = "",
		const std::string password = "");

	virtual ~CCustomLogger();
};
//------------------------------------------------------------------------------