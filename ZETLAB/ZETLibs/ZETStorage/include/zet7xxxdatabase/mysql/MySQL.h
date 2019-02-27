#pragma once
#include "..\DataBase.h"

#include <ZETSimple\include\mysqlcore\mysqlcore_auto_link.h>
#include <ZETSimple\include\mysqlcore\MysqlCore.h>

using namespace zet7xxx::modbus;

class CMySQL : public CDataBase
{
public:
	CMySQL(_In_ const std::wstring& sHost);

	// _ODBC_	_T("MySQL")
	// _DB_		_T("zet7xxx_storage")
	//	USER_	_T("zet7xxx_store")
	// _PASS_	_T("Zet12345")
	CMySQL(_In_ const std::wstring& sHost, _In_ const std::wstring& sODBC,
		   _In_ const std::wstring& sDB,   _In_ const std::wstring& sUSER,
		   _In_ const std::wstring& sPASS);

	~CMySQL();

	void update(_In_ const Device& device, _In_ const DeviceInfo& info) override;
	void remove(_In_ const Device& device) override;

	bool getDeviceInfo(_In_ const Device& device, _Out_ DeviceInfo& info) override;

	void getActiveMasters(_In_ const std::wstring& sIp, _Outref_result_maybenull_ PDevice& pMasters, _Out_ uint32_t& count) override;
	void getInactiveMasters(_In_ const std::wstring& sIp, _Outref_result_maybenull_ PDevice& pMasters, _Out_ uint32_t& count) override;
	void getActiveDevices(_In_ const Device& master, _Outref_result_maybenull_ PDevice& pDevices, _Out_ uint32_t& count) override;
	bool getRepresentative(_In_ const std::wstring& sId, _Out_ Representative& rep) override;
	bool getAllRepresentative(_Out_ DataBaseInfo& dbInfo) override;

	bool getDeviceHardwares(_In_ std::wstring type, _Out_ DataBaseInfo& dbInfo) override;
	bool getDeviceHardwaresWithMaxlevel(_In_ std::wstring type, _In_ float max_level, _Out_ DataBaseInfo& dbInfo) override;
	bool getDeviceTypesTable(_In_ std::wstring type, _Out_ DataBaseInfo& dbInfo) override;	
	bool getDeviceMaxLevels(_In_ std::wstring type, _In_ std::wstring hardware, _Out_ DataBaseInfo& dbInfo) override;
	//*******************	ОБЩИЕ ЗАПРОСЫ	*******************************************************
	bool getLastID(_In_ CString sTableName, _Out_ int &LastID) override;				// получаем последний ID из таблицы
	bool ExecuteQuery(_In_ const std::wstring sQuery) override;							// выполнить запрос
	bool getQuery_Select(_In_ const std::wstring sQuery, _Out_ DataBaseInfo& dbInfo) override;	// получить результат выполнения запроса
	//******************	SENSORTRACKING	*******************************************************
	bool SetTypePriority(_In_ int type, _In_ int value) override;						// установка приоритета для отпределенного типа датчиков
	
	bool SetTypeMinBalance(_In_ int type, _In_ CString value) override;					// установка неснижаемого остатка для отпределенного типа датчиков
	bool SetTypeMinBalancePressure(_In_ std::wstring name, _In_ float max_level, _In_ CString value) override;	// установка неснижаемого остатка для датчика давления
	
	bool SetTypeInternalUse(_In_ int type, _In_ CString value) override;				// установка количества устройств во внетреннем пользовании
	bool SetTypeInternalUsePressure(_In_ std::wstring name, _In_ float max_level, _In_ CString value) override;	// установка количества устройств во внетреннем пользовании для датчика давления

	std::wstring getCountOfDevicesWithType(_In_ std::wstring type) override;			// получение количества устройств с определенным типом
	std::wstring getCountOfDevicesWithTypeAndMaxlevel(_In_ std::wstring type, _In_ float max_level) override;	// получение количества устройств определенного типа

	CString getTypeName(_In_ int type) override;										// получение названия типа
protected:
	bool update(_In_ const Device& device, _In_ const std::wstring& sTable, _In_ const DeviceInfo& info);
	bool remove(_In_ const Device& device, _In_ const std::wstring& sTable);

private:
	std::wstring getDeviceTableName(_In_ const Device& device);

	void createTypeTable(_In_ const std::wstring& sTable);
	void createType(_In_ ZET7XXX_DEVICE_TYPE type, _In_ const std::wstring& sTable);

	bool createDeviceTable(_In_ const std::wstring& sTable);
	
	bool getDeviceInfo(_In_ const Device& device, _In_ const std::wstring& sTable, _Out_ DeviceInfo& info);
	bool isDeviceInfoCorrect(_In_ const Device& device, _In_ const DeviceInfo& info);
	
	bool createDevice(_In_ const Device& device, _In_ const std::wstring& sTable, _In_ const DeviceInfo& info);
	bool updateDevice(_In_ const Device& device, _In_ const std::wstring& sTable,
		_In_ const DeviceInfo& info, _In_ const DeviceInfo& infoFromDb);
	bool removeDevice(_In_ const Device& device, _In_ const std::wstring& sTable);

	bool updateDeviceType(_In_ const std::wstring& sTable, _In_ const Device& device);
	bool updateDeviceMaster(_In_ const std::wstring& sTable, _In_ const Device& device, _In_ const Device& master,
		_In_ const std::wstring& sMasterName);
	bool updateDeviceNode(_In_ const std::wstring& sTable, _In_ const Device& device, _In_ ModbusAddress node);
	bool updateDeviceIOParameters(_In_ const std::wstring& sTable, _In_ const Device& device,
		_In_ uint32_t baudRate, _In_ uint8_t parity);
	bool updateDeviceParameters(_In_ const std::wstring& sTable, _In_ const Device& device, _In_ float maxLevel);
	bool updateDeviceFirmware(_In_ const std::wstring& sTable, _In_ const Device& device, _In_ const VERSION_STRUCT& version);
	bool updateDevicePc(_In_ const std::wstring& sTable, _In_ const Device& device,
		_In_ const std::wstring& sIp, _In_ const std::wstring& sName);
	bool updateDeviceStatus(_In_ const std::wstring& sTable, _In_ const Device& device, _In_ int32_t status);
	
	void CalculateMinAndMaxVal(_In_ const float _Val, _Out_ float *_MinVal, _Out_ float *_MaxVal);	// для чисел с плавающей точкой

	bool CheckResult(_In_ const DataBaseInfo& dbInfo);		// проверка результата выполения запроса


	CMysqlCore* m_pMysqlCore;

	const uint32_t m_maxCounter = 5;
};

