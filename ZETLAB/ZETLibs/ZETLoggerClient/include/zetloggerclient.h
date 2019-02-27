#ifndef zet_logger_client
#define zet_logger_client

#include <ZETLogger\BasicZetLogger\basiczetLogger.h>
#include <ZetTools\include\ZetTools.h>

#include <vector>
#include <Windows.h>

#define MAX_ZETLOGGER_SERVICE_CLIENT_FALSE_CONNECT_COUNTER 100	// ������������ ���������� ��������� � ����� ������� ������ � ������ �����������, ����� �������� ����������� ������� ����������������������

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
	uint32_t              structureType;	// ��� ���������
	SZetLoggerStructureId structureId;		// ���������� ������������� ��������� ��������
	WCHAR                 fileFullAdr[MAX_PATH*10];	// ����� ������� ���� �� �����
	long                  posReadStart;	// ��������� �� ������ � �������� �����
	long                  posReadFinish;	// ��������� �� ����� � �������� �����
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
	CZETLoggerClient(bool onlyReader_ = false);		// ������ �������� ������� - �� ������ ��� �� ������ � ������, �� ��������� �� ������ � ������
	~CZETLoggerClient();

	// ����������� ������ ��������� ������ � ����������� ����
	// ������������ ��������: >= 0 - ����� ���������������������� ������, <0 - ������
	long registerNewStructureBuff(const ZetHeader* structure_);
	// ����� ��������� ������ � ����������� ����
	// ������������ ��������: >= 0 - ����� ���������������������� ������, <0 - ������
	long findStructureBuff(const ZetHeader* structure_);
	// ������ ��������� (���������)
	// ������������ ��������: >= 0 - ���������� ���������� ������ <0 - ������
	long writeStructureToBuff(const ZetHeader* structure_);
	// ������ ������ ������� ������ ��� ������
	long getMinBuffWriteSize();

	// ������ ������������ ������ ������ ������
	// begin_ - ����� ������ ���������, end_ - ����� ����� ���������, trendLevel - ������������ ������� ������������ ������
	// ��������� ������������ �� ���������� ���������� CZETLoggerClient _readParts � _uniqueTypes
	void readStructuresParts(CZetTime begin_, CZetTime end_, long trendLevel_, float* executionPercent_ = nullptr);
	// ������ ������������ ������ ������ ������
	// readParts_, uniqueTypes_ - ������� ������ � ���������� �����
	// begin_ - ����� ������ ���������, end_ - ����� ����� ���������, trendLevel - ������������ ������� ������������ ������
	// ��������� ������������ �� ���������� ���������� CZETLoggerClient _readParts � _uniqueTypes
	void readStructuresParts(std::vector<SZetLoggerReadPart>* readParts_, std::vector<uint32_t>* uniqueTypes_, CZetTime begin_, CZetTime end_, long trendLevel_, float* executionPercent_ = nullptr);
	// ��������� ������ ������������ ������ ������
	void breakReadStructuresParts();

	// ������ ������ ���������� ��������������� ���������� ��������
	// uniqueType_ - ��� ���������, �� �������� ���� ���������� ��������������
	// ��������� ������������ �� ���������� ���������� CZETLoggerClient _uniqueStructuresIds
	void readUniqueStructuresIdsFromStructureParts(uint32_t uniqueType);
	// ������ ������ ���������� ��������������� ���������� ��������
	// readParts_, uniqueStructureIds_ - ������� ������ � ���������� ���������������
	// uniqueType_ - ��� ���������, �� �������� ���� ���������� ��������������
	// ��������� ������������ �� ���������� ���������� CZETLoggerClient _uniqueStructuresIds
	void readUniqueStructuresIdsFromStructureParts(std::vector<SZetLoggerReadPart>* readParts_, std::vector<SZetLoggerStructureId>* uniqueStructureIds_, uint32_t uniqueType_);

	// �������� ������ ��������� ������ ������
	// uniqueType_ - ���������� ��� ��������� ��� ������
	// structureId_ - ���������� ������������� ������������ ��������
	// ��������� ������������ �� ���������� ���������� structuresFromFilesData � structuresFromFilesDataSz
	void openReadStructuresFromFiles(uint32_t uniqueType_, SZetLoggerStructureId structureId_);
	// ������ ������� ������ � ������
	uint32_t getStructuresFromFilesSz();
	// ������ ��������� �� ��������� ���������
	BYTE* getStructuresFromFiles();
	// �������� ������ �� ��������
	void closeReadStructuresFromFiles();


	// �������� ��������� ��������� (��������� �������� ��� ���� ��������������� �������)
	// structureType_ - ��� ���������, � ������� ������������ ���������
	// ������ ���������� �� ���������� ���������� _structureDefinitionData � _structureDefinitionDataSz
	void openReadStructureDefinition(uint32_t structureType_);
	// �������� ��������� ��������� (���������� �������� �� ����������� ��������������)
	// structureType_ - ��� ���������, � ������� ������������ ���������
	// ������ ���������� �� ���������� ���������� _structureDefinitionData � _structureDefinitionDataSz
	void openReadStructureDefinition(uint32_t structureType_, SZetLoggerStructureId structureId_);
	// ����������� ������� ��������� ���������
	uint32_t getReadStructureDefinitionSz();
	// ������ ��������� �� ��������� ���������
	BYTE* getReadStructureDefinition();
	// �������� ��������� ���������
	void closeReadStructureDefinition();

	// ���������� ��������� ���������
	// definition_ - ��������� ��������� � �������� ����
	// structureType_ - ��� ���������
	// structureId_ - ������������� ��������� (���� ������������� 0, ������������ ��� ��������� ���������)
	// ������������ ��������: >= 0 - ���������� ���������� ������, <0 - ������
	long saveStructureDefinition(const BYTE* definition_, uint32_t definitionSz_, uint32_t structureType_, SZetLoggerStructureId structureId_);
	long saveStructureDefinition(const ZetHeader * definition, const SZetLoggerStructureId & structureId);
	long saveStructureDefinition(const ZetHeader & definition, const SZetLoggerStructureId & structureId);

	// ��������� ������� ���������� ����� ��������
	const std::vector<uint32_t> &getUniqueTypes() const;
	// ��������� ������� ���������� ��������������� ��������
	const std::vector<SZetLoggerStructureId> &getUniqueStructureIds() const;
};

#endif