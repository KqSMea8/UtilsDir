#ifndef zet_logger_client
#define zet_logger_client

#include <ZETLogger\BasicZetLogger\basiczetLogger.h>
#include <ZetTools\include\ZetTools.h>

#include <vector>
#include <Windows.h>

#define MAX_ZETLOGGER_SERVICE_CLIENT_FALSE_CONNECT_COUNTER 100	// Максимальное количество обращений к общей области памяти с ложным результатом, после которого принимаются попытки переинициализироваться

#ifdef ZETLOGGERCLIENT_EXPORTS
#define ZETLOGGER_CLIENT_API __declspec(dllexport)
#else
#define ZETLOGGER_CLIENT_API __declspec(dllimport)
#endif

struct ZETLOGGER_CLIENT_API SZetLoggerStructureId
{
	uint64_t     device_id;		
	uint32_t     module_id;
	uint16_t     process_id;
	uint16_t     channel_id;
		
	SZetLoggerStructureId(uint64_t device_id_ = 0, uint32_t module_id_ = 0, uint16_t process_id_ = 0, uint16_t channel_id_ = 0) {
		device_id = device_id_; module_id = module_id_; process_id = process_id_; channel_id = channel_id_;
	}

	bool operator == (const SZetLoggerStructureId& other) const
	{
		return (device_id == other.device_id) && (module_id == other.module_id) &&
			(process_id == other.process_id) && (channel_id == other.channel_id);
	};

	bool operator < (const SZetLoggerStructureId& other) const
	{		
		if (device_id < other.device_id)
			return true;
		else if (device_id == other.device_id)
		{
			if (module_id < other.module_id)
				return true;
			else if (module_id == other.module_id)
			{
				if (process_id < other.process_id)
					return true;
				else if (process_id == other.process_id)
				{
					if (channel_id < other.channel_id)
						return true;
					else 
						return false;
				}
				else
					return false;
			}
			else
				return false;
		}
		else
			return false;		
	};
};

struct ZETLOGGER_CLIENT_API SZetLoggerReadPart
{
	uint32_t              structureType;	// Тип структуры
	SZetLoggerStructureId structureId;		// Уникальный идентификатор источника структур
	WCHAR                 fileFullAdr[MAX_PATH*10];	// Адрес полного пути до файла
	long                  posReadStart;	// Указатель на начало в читаемом файле
	long                  posReadFinish;	// Указатель на конец в читаемом файле
};

class ZETLOGGER_CLIENT_API CZETLoggerClient
{
private:
	class Impl;

	Impl *_impl;
	
	long _saveStructureDefinition(
		const PVOID definition_,
		const uint32_t definitionSz_,
		const uint32_t structureType_,
		const SZetLoggerStructureId & structureId_);

public:
	CZETLoggerClient(std::wstring signalsDir_, std::wstring trendsDir_, bool onlyReader_ = false);
	CZETLoggerClient(bool onlyReader_ = false);		// Способ открытия клиента - на чтение или на запись и чтение, по умолчанию на чтение и запись
	~CZETLoggerClient();

	// Регистрация нового источника данных в структурном виде
	// Возвращаемое значение: >= 0 - номер проинициализированного буфера, <0 - ошибка
	long registerNewStructureBuff(const ZetHeader* structure_);
	// Поиск источника данных в структурном виде
	// Возвращаемое значение: >= 0 - номер проинициализированного буфера, <0 - ошибка
	long findStructureBuff(const ZetHeader* structure_);
	// Запись структуры (медленная)
	// Возвращаемое значение: >= 0 - количество записанных данных <0 - ошибка
	long writeStructureToBuff(const ZetHeader* structure_);
	// Взятие общего размера буфера для чтения
	long getMinBuffWriteSize();

	// Чтение расположений частей чтения данных
	// begin_ - время начала интервала, end_ - время конца интервала, trendLevel - интересующий уровень отображаемых данных
	// результат возвращается во внутреннюю переменную CZETLoggerClient _readParts и _uniqueTypes
	void readStructuresParts(CZetTime begin_, CZetTime end_, long trendLevel_, float* executionPercent_ = nullptr);
	// Чтение расположений частей чтения данных
	// readParts_, uniqueTypes_ - вектора частей и уникальных типов
	// begin_ - время начала интервала, end_ - время конца интервала, trendLevel - интересующий уровень отображаемых данных
	// результат возвращается во внутреннюю переменную CZETLoggerClient _readParts и _uniqueTypes
	void readStructuresParts(std::vector<SZetLoggerReadPart>* readParts_, std::vector<uint32_t>* uniqueTypes_, CZetTime begin_, CZetTime end_, long trendLevel_, float* executionPercent_ = nullptr);
	// Остановка чтения расположения частей данных
	void breakReadStructuresParts();

	// Чтение списка уникальных идентификаторов источников структур
	// uniqueType_ - тип структуры, по которому ищем уникальные идентификаторы
	// результат возвращается во внутреннюю переменную CZETLoggerClient _uniqueStructuresIds
	void readUniqueStructuresIdsFromStructureParts(uint32_t uniqueType);
	// Чтение списка уникальных идентификаторов источников структур
	// readParts_, uniqueStructureIds_ - вектора частей и уникальных идентификаторов
	// uniqueType_ - тип структуры, по которому ищем уникальные идентификаторы
	// результат возвращается во внутреннюю переменную CZETLoggerClient _uniqueStructuresIds
	void readUniqueStructuresIdsFromStructureParts(std::vector<SZetLoggerReadPart>* readParts_, std::vector<SZetLoggerStructureId>* uniqueStructureIds_, uint32_t uniqueType_);

	// Открытие начала процедуры чтения данных
	// uniqueType_ - уникальный тип структуры для чтения
	// structureId_ - уникальный идентификатор используемых структур
	// результат возвращается во внутреннюю переменную structuresFromFilesData и structuresFromFilesDataSz
	void openReadStructuresFromFiles(uint32_t uniqueType_, SZetLoggerStructureId structureId_);
	// Взятие размера данных к чтению
	uint32_t getStructuresFromFilesSz();
	// Взятие указателя на считанные структуры
	BYTE* getStructuresFromFiles();
	// Закрытие данных из структур
	void closeReadStructuresFromFiles();


	// Открытие описателя структуры (дефолтное значение для всех идентификаторов каналов)
	// structureType_ - тип структуры, у которой вычитывается описатель
	// чтение происходит во внутреннюю переменную _structureDefinitionData и _structureDefinitionDataSz
	void openReadStructureDefinition(uint32_t structureType_);
	// Открытие описателя структуры (конкретное значения по уникальному идентификатору)
	// structureType_ - тип структуры, у которой вычитывается описатель
	// чтение происходит во внутреннюю переменную _structureDefinitionData и _structureDefinitionDataSz
	void openReadStructureDefinition(uint32_t structureType_, SZetLoggerStructureId structureId_);
	// Определение размера считанной структуры
	uint32_t getReadStructureDefinitionSz();
	// Взятие указателя на считанную структуру
	BYTE* getReadStructureDefinition();
	// Закрытие описателя структуры
	void closeReadStructureDefinition();

	// Сохранение описателя структуры
	// definition_ - описатель структуры в байтовом виде
	// structureType_ - тип структуры
	// structureId_ - идентификатор структуры (если идентификатор 0, записывается как дефолтный описатель)
	// Возвращаемое значение: >= 0 - количество записанных данных, <0 - ошибка
	long saveStructureDefinition(const BYTE* definition_, uint32_t definitionSz_, uint32_t structureType_, SZetLoggerStructureId structureId_);
	long saveStructureDefinition(const ZetHeader * definition, const SZetLoggerStructureId & structureId);
	long saveStructureDefinition(const ZetHeader & definition, const SZetLoggerStructureId & structureId);

	// Получение вектора уникальных типов структур
	const std::vector<uint32_t> &getUniqueTypes() const;
	// Получение вектора уникальных идентификаторов структур
	const std::vector<SZetLoggerStructureId> &getUniqueStructureIds() const;
};

#endif