#include "zetLogger.h"
#include "ZETFile\include\ZETFile.h"
#include <string>

CZetLogger::CZetLogger()
{
	_zetLogger = new CBasicZetLogger[const_max_number_zet_logger_files];

	// Создание потока для записи данных в файлы и обновления статусов жизни
	_pWorkerThread = new std::thread(&CZetLogger::worker, this);
	_pRunningLiveThread = new std::thread(&CZetLogger::runningLive, this);
}

CZetLogger::~CZetLogger()
{
	if (_pWorkerThread)
	{
		_pWorkerThread->join();
		delete _pWorkerThread;
		_pWorkerThread = nullptr;
	}

	if (_pRunningLiveThread)
	{
		_pRunningLiveThread->join();
		delete _pRunningLiveThread;
		_pRunningLiveThread = nullptr;
	}

	delete[] _zetLogger;
	_zetLogger = nullptr;
}

long CZetLogger::registerNewStructureLog(const ZetHeader* structure_, long loggerCircBuffSize_)
{
	long numLog = -1;	// Номер зарегестрированного лога под данный тип структуры и идентификатор источника

	// Поиск пустой ячейки
	for (int i = 0; i < const_max_number_zet_logger_files; i++)
		if (_zetLogger[i].isFree())
		{
			numLog = i;
			break;
		}
		
	// Регистрируем данную ячейку под лог для данного типа структуры и идентификатора источника
	if (numLog >= 0)
		_zetLogger[numLog].init(structure_, loggerCircBuffSize_);
	
	return numLog;
}

long CZetLogger::findStructureLog(const ZetHeader* structure_)
{
	long numLog = -1; // Поиск открытого лога под данный тип структуры и идентификатор источника

	for (int i = 0; i < const_max_number_zet_logger_files; i++)	  // Поиск по каждому полю
	{
		if (_zetLogger[i].isNeedType(structure_->type))	// Расписано по полям, в надежде что будет работать быстрее из-за меньшего количества сравнения
			if (_zetLogger[i].isNeedDeviceId(structure_->device_id))
				if (_zetLogger[i].isNeedModuleId(structure_->module_id))
					if (_zetLogger[i].isNeedProcessId(structure_->process_id))
						if (_zetLogger[i].isNeedChannelId(structure_->channel_id))
							if (_zetLogger[i].isNeedTimeHourId(structure_->seconds))
						{
							numLog = i;
							break;
						}
	}

	return numLog;
}

long CZetLogger::writeStructureToLog(const ZetHeader* structure_)	// Запись удобная в использовании но работающая медленно (сама ищет необходимую ячейку)
{
	long numLog = findStructureLog(structure_); // Поиск открытого лога под данный тип структуры и идентификатор источника
	if (numLog < 0)
		numLog = registerNewStructureLog(structure_, bigLoggerCircBuffSize);

	// Запись данных в буфер
	if (numLog >= 0)
		_zetLogger[numLog].writeStructure(reinterpret_cast<char*>(const_cast<ZetHeader*>(structure_)), (long)structure_->size);

	return numLog;
}

long CZetLogger::writeStructureToLog(const ZetHeader* structure_, long numLog_)	// Запись не удобная в использовании но работающая быстро
{
	long numLog = numLog_;

	// Запись данных в буфер
	if (numLog >= 0)
		if (_zetLogger[numLog].isNeedTypeAndId(structure_->type, structure_->device_id, structure_->module_id, structure_->process_id, structure_->channel_id))
			_zetLogger[numLog].writeStructure(reinterpret_cast<char*>(const_cast<ZetHeader*>(structure_)), (long)structure_->size);
		else
			numLog = -1;

	return numLog;
}

long CZetLogger::getSizeWrite()
{
	long size = 0;

	for (int i = 0; i < const_max_number_zet_logger_files; i++)
		size += _zetLogger[i].getSizeWrite();

	return size;
}

long CZetLogger::getSizeRead()
{
	long size = 0;

	for (int i = 0; i < const_max_number_zet_logger_files; i++)
		size += _zetLogger[i].getSizeRead();

	return size;
}

long CZetLogger::writeStructuresToFile(const ZetHeader* structure_, long size_)
{
	long retVal = -1;

	// Берем директорию сохранения сигналов из ZetPath
	std::wstring zetSignalsPath = std::wstring(L"D:\\Файлы\\");

	// Уходим в поддиректорию логгера
	std::wstring loggerPath = zetSignalsPath + ZETLOGGER_DIRECTORY_NAME_IN_ZETSIGNALS_DIRECTORY;

	tm structureTime;
	time_t structureTimeT = structure_->seconds;
	localtime_s(&structureTime, &structureTimeT);
	// Уходим в нужный год
	std::wstring filePath = loggerPath + std::to_wstring(structureTime.tm_year) + std::wstring(L"\\");

	// Уходим в нужный месяц
	filePath = filePath + std::to_wstring(structureTime.tm_mon + 1) + std::wstring(L"\\");

	// Уходим в нужный день
	filePath = filePath + std::to_wstring(structureTime.tm_mday) + std::wstring(L"\\");

	// Уходим в нужный час
	filePath = filePath + std::to_wstring(structureTime.tm_hour) + std::wstring(L"\\");

	// Формируем наименование файла: {type-device_id-module_id-process_id-channel_id}
	std::wstring fileAdr = std::wstring(L"{");
	fileAdr += std::to_wstring(structure_->type) + std::wstring(L"-");
	fileAdr += std::to_wstring(structure_->device_id) + std::wstring(L"-");
	fileAdr += std::to_wstring(structure_->module_id) + std::wstring(L"-");
	fileAdr += std::to_wstring(structure_->process_id) + std::wstring(L"-");
	fileAdr += std::to_wstring(structure_->channel_id);
	fileAdr += std::wstring(L"}");

	// Проверка на существование директории, если диектория отсутствует, добавляется
	if (zfDirectoryExists(filePath.c_str()) == 0)
	{
		zfCreatePath(filePath.c_str());
	}

	filePath = filePath + fileAdr;

	// Открываем файл на запись
	FILE* fp;
	_wfopen_s(&fp, filePath.c_str(), L"a");
	if (fp != nullptr)
	{
		retVal = fwrite(structure_, 1, size_, fp);
		fclose(fp);
	}

	return retVal;
}

// Потоковые функции
void CZetLogger::worker(CZetLogger* zetLogger)
{
	long szStructuresToRead = 0; // Размер следующей структуры для чтения
	char bufferForStructures[const_max_data_size_to_write_in_file];	// Буфер под структуру

	while (true)
	{
		// Поиск потерянных источников структурных данных и их отмирание		
		// Освобождение циклических буферов под структурные данные

		for (int i = 0; i < const_max_number_zet_logger_files; i++)
		{
			szStructuresToRead = zetLogger->_zetLogger[i].getSizeRead();

			// 1. Запись данных по превышению порога буфера (накопилось данных на 128 КБайт)
			if (szStructuresToRead >= const_max_data_size_to_write_in_file)
			{
				// Все бы хорошо, но нужно найти границу последней структуры и вот этот участок записать в файл
				szStructuresToRead = zetLogger->_zetLogger[i].readStructures(bufferForStructures, const_max_data_size_to_write_in_file);
				zetLogger->writeStructuresToFile(reinterpret_cast<ZetHeader*>(bufferForStructures), szStructuresToRead);

				zetLogger->_zetLogger[i].updateWaitTime();
				continue;
			}

			// 2. Запись данных по таймауту (прошло N ое количество секунд)
			if (zetLogger->_zetLogger[i].isWaitTimeFinished())
			{
				// Не парясь берем все из циклического буфера и пихаем в выходной поток
				szStructuresToRead = zetLogger->_zetLogger[i].readStructures(bufferForStructures, szStructuresToRead);
				zetLogger->writeStructuresToFile(reinterpret_cast<ZetHeader*>(bufferForStructures), szStructuresToRead);

				zetLogger->_zetLogger[i].updateWaitTime();
				continue;
			}

			// 3. Запись данных по отмиранию источника данных
			if (zetLogger->_zetLogger[i].isDied())
			{
				// Не парясь берем все из циклического буфера и пихаем в выходной поток
				szStructuresToRead = zetLogger->_zetLogger[i].readStructures(bufferForStructures, szStructuresToRead);
				zetLogger->writeStructuresToFile(reinterpret_cast<ZetHeader*>(bufferForStructures), szStructuresToRead);

				zetLogger->_zetLogger[i].reset();
				continue;
			}
		}

		Sleep(const_max_free_thread_time);
	}
}

void CZetLogger::runningLive(CZetLogger* zetLogger)
{
	long counterSecondsInMinute = 0;

	while (true)	// Поток крутится каждую СЕКУНДУ
	{
		// Обновление статуса ячеек под источники структурных данных)
		for (int i = 0; i < const_max_number_zet_logger_files; i++)
			zetLogger->_zetLogger[i].runingLiveAndWaitTime();

		Sleep(const_mseconds_in_second);
	}
}