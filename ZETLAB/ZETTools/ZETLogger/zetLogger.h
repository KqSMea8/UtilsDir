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
	
	const static long const_max_number_zet_logger_files = 20000;	// Максимальное количество ячеек под запись
	const static long const_max_data_size_to_write_in_file = 131072;	// Максимальный размер куска данных для записи (128 Кбайт)
	const static long const_max_free_thread_time = 1;	// Свободное время потока
	const static long const_mseconds_in_second = 1000;			// Количество миллисекунд в секунде
	const static long const_duration_measure_baud_rate_in_seconds = 10;				// Количество секунд для измерения скорости
	const static long const_bytes_in_kbyte = 1024;				// Количество байт в килобайте

	// Потоки обработки всех источников данных
	static void worker(CZetLogger* zetLogger);
	static void runningLive(CZetLogger* zetLogger);

	// Запись структуры в файл
	long writeStructuresToFile(const ZetHeader* structures_, long size_);
public:
	enum {
		smallLoggerCircBuffSize = 655360,
		bigLoggerCircBuffSize = 1310720
	};

	CZetLogger();
	~CZetLogger();

	// Регистрация нового источника данных в структурном виде
	long registerNewStructureLog(const ZetHeader* structure_, long loggerCircBuffSize_);
	// Поиск источника данных в структурном виде
	long findStructureLog(const ZetHeader* structure_);
	// Запись структуры (медленная)
	long writeStructureToLog(const ZetHeader* structure_);
	// Запись структуры (быстрая)
	long writeStructureToLog(const ZetHeader* structure_, long numLog_);

	// Взятие размера буфера для записи
	long getSizeWrite();
	// Взятие размера буфера для чтения
	long getSizeRead();
};