#ifndef basic_zet_logger
#define basic_zet_logger

#include "ZETLogger\BasicZetLogger\circBuffBasicZetLogger\circbuffbasiczetLogger.h"
#include <ZetErrorCodes.h>

#define ZETLOGGER_WRITE_DIRECTORY L"C:\\ZETLogger\\Signals"
#define ZETLOGGER_TREND_WRITE_DIRECTORY L"C:\\ZETLogger\\Trends"
#define ZETLOGGER_WRITE_FILE_FORMAT L".zlbd"

#define ZETLOGGER_STRUCTURES_DEFINITIONS_FOLDER L"structures_definitions"

#define ZETLOGGER_SERVICE_LOGGS_NUMBER      4000 // Количество логов для записи данных
#define ZETLOGGER_SERVICE_BUFFER_SHARED_MEMORY_NAME   L"Global\\ZETLoggerServiceBuffers"
#define ZETLOGGER_TREND_NAME_POSTFIX_LENGTH 4	// Длина постфикса в названии файла тренда (_01s, _10s, _01m, ...)

#define ZETLOGGER_SERVICE_CLIENT_REGISTER_NEW_BUFFER_MUTEX_NAME L"Global\\RegisterNewBufferZETLogger"	// Имя именованного мьютекса для начала регистрации нового буфера

#define ZETLOGGER_ONEDAY_TREND      		7	// Уровень тренда по одному дню
#define ZETLOGGER_SIXHOURS_TREND            6	// Уровень тренда по 6 часов
#define ZETLOGGER_ONEHOUR_TREND             5	// Уровень тренда по одному часу
#define ZETLOGGER_TENMINUTES_TREND          4	// Уровень тренда по 10 минут
#define ZETLOGGER_ONEMINUTE_TREND			3	// Уровень тренда по одной минуте
#define ZETLOGGER_TENSECONDS_TREND			2	// Уровень тренда по 10 секунд
#define ZETLOGGER_ONESECOND_TREND			1	// Уровень тренда по 1 секунде
#define ZETLOGGER_NOTREND_TREND				0	// Уровень тренда без уплотнения

#define BASICZETLOGGER_MAX_LIVETICK 30		// Значение счетчика "жизни" при котором источник структурных данных считается потерянным
#define BASICZETLOGGER_MAX_WAITTICK 60		// Значение счетчика "ожидания" при котором данные из буфера записываются в файл принудительно
#define CONST_SECONDS_IN_HOUR       3600	// Количество секунд в часе (константа используется для расчета уникального часа, целочисленное деление и умножение)

struct BasicZetLoggerStruct
{
	uint32_t _type;			// Тип записываемых структур
	uint32_t _size;			// Размер записываемой структуры
	uint64_t _timeHourId;   // Уникальный идентификатор текущего записываемого часа времени
	uint64_t _deviceId;		// Уникальный идентификатор записываемых структур
	uint32_t _moduleId;		// Уникальный идентификатор записываемых структур
	uint16_t _processId;	// Уникальный идентификатор записываемых структур
	uint16_t _channelId;	// Уникальный идентификатор записываемых структур

	long _waittick;			// Счетчик времени ожидания ячейки для записи данных
	long _livetick;			// Счетчик жизни ячейки

	CircBuffBasicZetLoggerStruct _circBuff;	// Циклический буфер для хранения данных поступаемых в структурах
};

// Класс для работы с структурой буфера для приема структурных данных
class CBasicZetLogger
{
private:
	BasicZetLoggerStruct* _basicZETLogger;		// Указатель на структуру буфера (сделано через указатель на структуру для работы через Shared Memory)
	CCircBuffBasicZetLogger _circBuff;

	// Конвертирование текущего времени в уникальный идентификатор часа
	uint64_t convertTimeStampToTimeHourId(uint64_t timeStamp_);	// Конвертирование метки времени в уникальный идентификатор часа
public:
	CBasicZetLogger();
	CBasicZetLogger(_In_ BasicZetLoggerStruct* basicZETLogger_);
	~CBasicZetLogger();


	// Сброс ячейки записи структур данных (сбрасываются уникальные идентификационные данные структур принимаемых буфером)
	void reset();
	// Обновление счетчика жизни буфера в живой (буфер живой)
	void updateLive();
	// Обновление времени ожидания буфера в только что освобожденный (время ожидания 0)
	void updateWaitTime();
	// Установка счетчика жизни ячейки (инкрементирование счетчиков жизни и ожидания)
	void runingLiveAndWaitTime();
	// Проверка окончания времени ожидания буфера
	// Возвращаемые значения: true - время ожидания буфера истекло, пора освободить буфер в файл по времени; false - время ожидания ещё не истекло
	bool isWaitTimeFinished();
	// Проверка жизни буфера
	// Возвращаемые значения: true - буфер живой, работает; false - буфер умер, данные не поступают
	bool isLive();
	// Проверка отмирания ячейки
	// Возвращаемые значения: true - буфер умер, данные не поступают; false - буфер живой, данные поступают
	bool isDied();
	// Проверка проинициализированности
	bool isValid();

	// Инициализация ячейки записи структур данных (Установка типа структуры, уникального идентификатора зписываемого часа, уникального идентификатора источника структур и размера циклического буфера под хранение данных
	void init(_In_ const ZetHeader* strucutreHeader_);
	// Взятие типа записываемых структур
	void getType(_Out_ uint32_t & type_);
	// Взятие уникального идентификатора источника структур
	void getUniqueId(_Out_ uint64_t & deviceId_, _Out_ uint32_t & moduleId_, _Out_ uint16_t & processId_, _Out_ uint16_t & channelId_);
	// Взятие уникального идентификатора текущего записываемого часа
	void getTimeHourId(_Out_ uint64_t & timeHourId_);
	// Взятие размера структуры для записи
	void getStructureSize(_Out_ uint32_t & structureSize_);
	uint32_t getStructureSize();

	// Проверка на занятость ячейки
	// Возвращаемые значения: true - буфер живой, работает; false - буфер умер, данные не поступают
	bool isFree();
	// Проверка соответствия заголовка структуры текущим идентификационным данным буфера под запись структур
	// Возвращаемые значения: true - буфер соответствует; false - буфер не соответствует
	bool isNeedStructureHeader(const ZetHeader* structureHeader_);
	// Проверка нужной ячейки по типу и уникальному идентификатору
	// Возвращаемые значения: true - буфер соответствует; false - буфер не соответствует
	bool isNeedTypeAndId(uint32_t type_, uint64_t deviceId_, uint32_t moduleId_, uint16_t processId_, uint16_t channelId_);
	// Проверка нужной ячейки по типу
	// Возвращаемые значения: true - буфер соответствует; false - буфер не соответствует
	bool isNeedType(uint32_t type_);
	// Проверка нужной ячейки по уникальному идентификатору
	// Возвращаемые значения: true - буфер соответствует; false - буфер не соответствует
	bool isNeedId(uint64_t deviceId_, uint32_t moduleId_, uint16_t processId_, uint16_t channelId_);
	// Проверка нужной ячейки по уникальному идентификатору (device_id)
	// Возвращаемые значения: true - буфер соответствует; false - буфер не соответствует
	bool isNeedDeviceId(uint64_t deviceId_);
	// Проверка нужной ячейки по уникальному идентификатору (module_id)
	// Возвращаемые значения: true - буфер соответствует; false - буфер не соответствует
	bool isNeedModuleId(uint32_t moduleId_);
	// Проверка нужной ячейки по уникальному идентификатору (process_id)
	// Возвращаемые значения: true - буфер соответствует; false - буфер не соответствует
	bool isNeedProcessId(uint16_t processId_);
	// Проверка нужной ячейки по уникальному идентификатору (channel_id)
	// Возвращаемые значения: true - буфер соответствует; false - буфер не соответствует
	bool isNeedChannelId(uint16_t channelId_);
	// Проверка нужного идентификатора часа
	// Возвращаемые значения: true - буфер соответствует; false - буфер не соответствует
	bool isNeedTimeHourId(uint64_t timeHourId_);

	// Запись структуры в буфер
	// Возвращаемые значения: >=0 - размер записанных данных; <0 - ошибка, буфера нет
	long writeStructure(char* structure_, long size_);
	// Чтение структуры из буфера
	// Возвращаемые значения: >=0 - размер считанных данных; <0 - ошибка, буфера нет
	long readStructures(char* structure_, long size_);
	// Чтение структуры из буфера во временный буфер
	// Возвращаемые значения: >=0 - размер считанных данных; <0 - ошибка, буфера нет
	long readTempStructures(char* structure_, long size_);
	// Взятие размера доступного места для записи в циклическом буфере
	// Возвращаемые значения: >=0 - размер доступного места
	long getSizeWrite();
	// Взятие размера доступного места для чтения в циклическом буфере
	// Возвращаемые значения: >=0 - размер доступного места
	long getSizeRead();
};

#endif