#ifndef basic_zet_logger
#define basic_zet_logger

#include "ZETLogger\BasicZetLogger\circBuffBasicZetLogger\circbuffbasiczetLogger.h"
#include <ZetErrorCodes.h>

#define ZETLOGGER_WRITE_DIRECTORY L"C:\\ZETLogger\\Signals"
#define ZETLOGGER_TREND_WRITE_DIRECTORY L"C:\\ZETLogger\\Trends"
#define ZETLOGGER_WRITE_FILE_FORMAT L".zlbd"

#define ZETLOGGER_STRUCTURES_DEFINITIONS_FOLDER L"structures_definitions"

#define ZETLOGGER_SERVICE_LOGGS_NUMBER      4000 // ���������� ����� ��� ������ ������
#define ZETLOGGER_SERVICE_BUFFER_SHARED_MEMORY_NAME   L"Global\\ZETLoggerServiceBuffers"
#define ZETLOGGER_TREND_NAME_POSTFIX_LENGTH 4	// ����� ��������� � �������� ����� ������ (_01s, _10s, _01m, ...)

#define ZETLOGGER_SERVICE_CLIENT_REGISTER_NEW_BUFFER_MUTEX_NAME L"Global\\RegisterNewBufferZETLogger"	// ��� ������������ �������� ��� ������ ����������� ������ ������

#define ZETLOGGER_ONEDAY_TREND      		7	// ������� ������ �� ������ ���
#define ZETLOGGER_SIXHOURS_TREND            6	// ������� ������ �� 6 �����
#define ZETLOGGER_ONEHOUR_TREND             5	// ������� ������ �� ������ ����
#define ZETLOGGER_TENMINUTES_TREND          4	// ������� ������ �� 10 �����
#define ZETLOGGER_ONEMINUTE_TREND			3	// ������� ������ �� ����� ������
#define ZETLOGGER_TENSECONDS_TREND			2	// ������� ������ �� 10 ������
#define ZETLOGGER_ONESECOND_TREND			1	// ������� ������ �� 1 �������
#define ZETLOGGER_NOTREND_TREND				0	// ������� ������ ��� ����������

#define BASICZETLOGGER_MAX_LIVETICK 30		// �������� �������� "�����" ��� ������� �������� ����������� ������ ��������� ����������
#define BASICZETLOGGER_MAX_WAITTICK 60		// �������� �������� "��������" ��� ������� ������ �� ������ ������������ � ���� �������������
#define CONST_SECONDS_IN_HOUR       3600	// ���������� ������ � ���� (��������� ������������ ��� ������� ����������� ����, ������������� ������� � ���������)

struct BasicZetLoggerStruct
{
	uint32_t _type;			// ��� ������������ ��������
	uint32_t _size;			// ������ ������������ ���������
	uint64_t _timeHourId;   // ���������� ������������� �������� ������������� ���� �������
	uint64_t _deviceId;		// ���������� ������������� ������������ ��������
	uint32_t _moduleId;		// ���������� ������������� ������������ ��������
	uint16_t _processId;	// ���������� ������������� ������������ ��������
	uint16_t _channelId;	// ���������� ������������� ������������ ��������

	long _waittick;			// ������� ������� �������� ������ ��� ������ ������
	long _livetick;			// ������� ����� ������

	CircBuffBasicZetLoggerStruct _circBuff;	// ����������� ����� ��� �������� ������ ����������� � ����������
};

// ����� ��� ������ � ���������� ������ ��� ������ ����������� ������
class CBasicZetLogger
{
private:
	BasicZetLoggerStruct* _basicZETLogger;		// ��������� �� ��������� ������ (������� ����� ��������� �� ��������� ��� ������ ����� Shared Memory)
	CCircBuffBasicZetLogger _circBuff;

	// ��������������� �������� ������� � ���������� ������������� ����
	uint64_t convertTimeStampToTimeHourId(uint64_t timeStamp_);	// ��������������� ����� ������� � ���������� ������������� ����
public:
	CBasicZetLogger();
	CBasicZetLogger(_In_ BasicZetLoggerStruct* basicZETLogger_);
	~CBasicZetLogger();


	// ����� ������ ������ �������� ������ (������������ ���������� ����������������� ������ �������� ����������� �������)
	void reset();
	// ���������� �������� ����� ������ � ����� (����� �����)
	void updateLive();
	// ���������� ������� �������� ������ � ������ ��� ������������� (����� �������� 0)
	void updateWaitTime();
	// ��������� �������� ����� ������ (����������������� ��������� ����� � ��������)
	void runingLiveAndWaitTime();
	// �������� ��������� ������� �������� ������
	// ������������ ��������: true - ����� �������� ������ �������, ���� ���������� ����� � ���� �� �������; false - ����� �������� ��� �� �������
	bool isWaitTimeFinished();
	// �������� ����� ������
	// ������������ ��������: true - ����� �����, ��������; false - ����� ����, ������ �� ���������
	bool isLive();
	// �������� ��������� ������
	// ������������ ��������: true - ����� ����, ������ �� ���������; false - ����� �����, ������ ���������
	bool isDied();
	// �������� �����������������������
	bool isValid();

	// ������������� ������ ������ �������� ������ (��������� ���� ���������, ����������� �������������� ������������ ����, ����������� �������������� ��������� �������� � ������� ������������ ������ ��� �������� ������
	void init(_In_ const ZetHeader* strucutreHeader_);
	// ������ ���� ������������ ��������
	void getType(_Out_ uint32_t & type_);
	// ������ ����������� �������������� ��������� ��������
	void getUniqueId(_Out_ uint64_t & deviceId_, _Out_ uint32_t & moduleId_, _Out_ uint16_t & processId_, _Out_ uint16_t & channelId_);
	// ������ ����������� �������������� �������� ������������� ����
	void getTimeHourId(_Out_ uint64_t & timeHourId_);
	// ������ ������� ��������� ��� ������
	void getStructureSize(_Out_ uint32_t & structureSize_);
	uint32_t getStructureSize();

	// �������� �� ��������� ������
	// ������������ ��������: true - ����� �����, ��������; false - ����� ����, ������ �� ���������
	bool isFree();
	// �������� ������������ ��������� ��������� ������� ����������������� ������ ������ ��� ������ ��������
	// ������������ ��������: true - ����� �������������; false - ����� �� �������������
	bool isNeedStructureHeader(const ZetHeader* structureHeader_);
	// �������� ������ ������ �� ���� � ����������� ��������������
	// ������������ ��������: true - ����� �������������; false - ����� �� �������������
	bool isNeedTypeAndId(uint32_t type_, uint64_t deviceId_, uint32_t moduleId_, uint16_t processId_, uint16_t channelId_);
	// �������� ������ ������ �� ����
	// ������������ ��������: true - ����� �������������; false - ����� �� �������������
	bool isNeedType(uint32_t type_);
	// �������� ������ ������ �� ����������� ��������������
	// ������������ ��������: true - ����� �������������; false - ����� �� �������������
	bool isNeedId(uint64_t deviceId_, uint32_t moduleId_, uint16_t processId_, uint16_t channelId_);
	// �������� ������ ������ �� ����������� �������������� (device_id)
	// ������������ ��������: true - ����� �������������; false - ����� �� �������������
	bool isNeedDeviceId(uint64_t deviceId_);
	// �������� ������ ������ �� ����������� �������������� (module_id)
	// ������������ ��������: true - ����� �������������; false - ����� �� �������������
	bool isNeedModuleId(uint32_t moduleId_);
	// �������� ������ ������ �� ����������� �������������� (process_id)
	// ������������ ��������: true - ����� �������������; false - ����� �� �������������
	bool isNeedProcessId(uint16_t processId_);
	// �������� ������ ������ �� ����������� �������������� (channel_id)
	// ������������ ��������: true - ����� �������������; false - ����� �� �������������
	bool isNeedChannelId(uint16_t channelId_);
	// �������� ������� �������������� ����
	// ������������ ��������: true - ����� �������������; false - ����� �� �������������
	bool isNeedTimeHourId(uint64_t timeHourId_);

	// ������ ��������� � �����
	// ������������ ��������: >=0 - ������ ���������� ������; <0 - ������, ������ ���
	long writeStructure(char* structure_, long size_);
	// ������ ��������� �� ������
	// ������������ ��������: >=0 - ������ ��������� ������; <0 - ������, ������ ���
	long readStructures(char* structure_, long size_);
	// ������ ��������� �� ������ �� ��������� �����
	// ������������ ��������: >=0 - ������ ��������� ������; <0 - ������, ������ ���
	long readTempStructures(char* structure_, long size_);
	// ������ ������� ���������� ����� ��� ������ � ����������� ������
	// ������������ ��������: >=0 - ������ ���������� �����
	long getSizeWrite();
	// ������ ������� ���������� ����� ��� ������ � ����������� ������
	// ������������ ��������: >=0 - ������ ���������� �����
	long getSizeRead();
};

#endif