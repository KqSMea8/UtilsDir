#include "zetLogger.h"
#include "ZETFile\include\ZETFile.h"
#include <string>

CZetLogger::CZetLogger()
{
	_zetLogger = new CBasicZetLogger[const_max_number_zet_logger_files];

	// �������� ������ ��� ������ ������ � ����� � ���������� �������� �����
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
	long numLog = -1;	// ����� ������������������� ���� ��� ������ ��� ��������� � ������������� ���������

	// ����� ������ ������
	for (int i = 0; i < const_max_number_zet_logger_files; i++)
		if (_zetLogger[i].isFree())
		{
			numLog = i;
			break;
		}
		
	// ������������ ������ ������ ��� ��� ��� ������� ���� ��������� � �������������� ���������
	if (numLog >= 0)
		_zetLogger[numLog].init(structure_, loggerCircBuffSize_);
	
	return numLog;
}

long CZetLogger::findStructureLog(const ZetHeader* structure_)
{
	long numLog = -1; // ����� ��������� ���� ��� ������ ��� ��������� � ������������� ���������

	for (int i = 0; i < const_max_number_zet_logger_files; i++)	  // ����� �� ������� ����
	{
		if (_zetLogger[i].isNeedType(structure_->type))	// ��������� �� �����, � ������� ��� ����� �������� ������� ��-�� �������� ���������� ���������
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

long CZetLogger::writeStructureToLog(const ZetHeader* structure_)	// ������ ������� � ������������� �� ���������� �������� (���� ���� ����������� ������)
{
	long numLog = findStructureLog(structure_); // ����� ��������� ���� ��� ������ ��� ��������� � ������������� ���������
	if (numLog < 0)
		numLog = registerNewStructureLog(structure_, bigLoggerCircBuffSize);

	// ������ ������ � �����
	if (numLog >= 0)
		_zetLogger[numLog].writeStructure(reinterpret_cast<char*>(const_cast<ZetHeader*>(structure_)), (long)structure_->size);

	return numLog;
}

long CZetLogger::writeStructureToLog(const ZetHeader* structure_, long numLog_)	// ������ �� ������� � ������������� �� ���������� ������
{
	long numLog = numLog_;

	// ������ ������ � �����
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

	// ����� ���������� ���������� �������� �� ZetPath
	std::wstring zetSignalsPath = std::wstring(L"D:\\�����\\");

	// ������ � ������������� �������
	std::wstring loggerPath = zetSignalsPath + ZETLOGGER_DIRECTORY_NAME_IN_ZETSIGNALS_DIRECTORY;

	tm structureTime;
	time_t structureTimeT = structure_->seconds;
	localtime_s(&structureTime, &structureTimeT);
	// ������ � ������ ���
	std::wstring filePath = loggerPath + std::to_wstring(structureTime.tm_year) + std::wstring(L"\\");

	// ������ � ������ �����
	filePath = filePath + std::to_wstring(structureTime.tm_mon + 1) + std::wstring(L"\\");

	// ������ � ������ ����
	filePath = filePath + std::to_wstring(structureTime.tm_mday) + std::wstring(L"\\");

	// ������ � ������ ���
	filePath = filePath + std::to_wstring(structureTime.tm_hour) + std::wstring(L"\\");

	// ��������� ������������ �����: {type-device_id-module_id-process_id-channel_id}
	std::wstring fileAdr = std::wstring(L"{");
	fileAdr += std::to_wstring(structure_->type) + std::wstring(L"-");
	fileAdr += std::to_wstring(structure_->device_id) + std::wstring(L"-");
	fileAdr += std::to_wstring(structure_->module_id) + std::wstring(L"-");
	fileAdr += std::to_wstring(structure_->process_id) + std::wstring(L"-");
	fileAdr += std::to_wstring(structure_->channel_id);
	fileAdr += std::wstring(L"}");

	// �������� �� ������������� ����������, ���� ��������� �����������, �����������
	if (zfDirectoryExists(filePath.c_str()) == 0)
	{
		zfCreatePath(filePath.c_str());
	}

	filePath = filePath + fileAdr;

	// ��������� ���� �� ������
	FILE* fp;
	_wfopen_s(&fp, filePath.c_str(), L"a");
	if (fp != nullptr)
	{
		retVal = fwrite(structure_, 1, size_, fp);
		fclose(fp);
	}

	return retVal;
}

// ��������� �������
void CZetLogger::worker(CZetLogger* zetLogger)
{
	long szStructuresToRead = 0; // ������ ��������� ��������� ��� ������
	char bufferForStructures[const_max_data_size_to_write_in_file];	// ����� ��� ���������

	while (true)
	{
		// ����� ���������� ���������� ����������� ������ � �� ���������		
		// ������������ ����������� ������� ��� ����������� ������

		for (int i = 0; i < const_max_number_zet_logger_files; i++)
		{
			szStructuresToRead = zetLogger->_zetLogger[i].getSizeRead();

			// 1. ������ ������ �� ���������� ������ ������ (���������� ������ �� 128 �����)
			if (szStructuresToRead >= const_max_data_size_to_write_in_file)
			{
				// ��� �� ������, �� ����� ����� ������� ��������� ��������� � ��� ���� ������� �������� � ����
				szStructuresToRead = zetLogger->_zetLogger[i].readStructures(bufferForStructures, const_max_data_size_to_write_in_file);
				zetLogger->writeStructuresToFile(reinterpret_cast<ZetHeader*>(bufferForStructures), szStructuresToRead);

				zetLogger->_zetLogger[i].updateWaitTime();
				continue;
			}

			// 2. ������ ������ �� �������� (������ N �� ���������� ������)
			if (zetLogger->_zetLogger[i].isWaitTimeFinished())
			{
				// �� ������ ����� ��� �� ������������ ������ � ������ � �������� �����
				szStructuresToRead = zetLogger->_zetLogger[i].readStructures(bufferForStructures, szStructuresToRead);
				zetLogger->writeStructuresToFile(reinterpret_cast<ZetHeader*>(bufferForStructures), szStructuresToRead);

				zetLogger->_zetLogger[i].updateWaitTime();
				continue;
			}

			// 3. ������ ������ �� ��������� ��������� ������
			if (zetLogger->_zetLogger[i].isDied())
			{
				// �� ������ ����� ��� �� ������������ ������ � ������ � �������� �����
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

	while (true)	// ����� �������� ������ �������
	{
		// ���������� ������� ����� ��� ��������� ����������� ������)
		for (int i = 0; i < const_max_number_zet_logger_files; i++)
			zetLogger->_zetLogger[i].runingLiveAndWaitTime();

		Sleep(const_mseconds_in_second);
	}
}