#pragma once

#include <ZET7xxx\include\base\ZET7xxxTypes.h>
#include <ZETSimple\include\mysqlcore\MysqlCore.h>

using namespace zet7xxx;

#define	LOCAL_HOST	_T("127.0.0.1")
#define ZETLAB_HOST	_T("10.0.170.4")

class CDataBase
{
public:
	CDataBase(_In_ const std::wstring& sHost);
	virtual ~CDataBase();

	virtual void update(_In_ const Device& device, _In_ const DeviceInfo& info) = 0;
	virtual void remove(_In_ const Device& device) = 0;

	virtual bool getDeviceInfo(_In_ const Device& device, _Out_ DeviceInfo& info) = 0;

	virtual void getActiveMasters(_In_ const std::wstring& sIp, _Outref_result_maybenull_ PDevice& pMasters, _Out_ uint32_t& count) = 0;
	virtual void getInactiveMasters(_In_ const std::wstring& sIp, _Outref_result_maybenull_ PDevice& pMasters, _Out_ uint32_t& count) = 0;
	virtual void getActiveDevices(_In_ const Device& master, _Outref_result_maybenull_ PDevice& pDevices, _Out_ uint32_t& count) = 0;
	virtual bool getRepresentative(_In_ const std::wstring& sId, _Out_ Representative& rep) = 0;
	virtual bool getAllRepresentative(_Out_ DataBaseInfo& dbInfo) = 0;

	virtual bool ExecuteQuery(_In_ const std::wstring sQuery) = 0;
	virtual bool getQuery_Select(_In_ const std::wstring sQuery, _Out_ DataBaseInfo& dbInfo) = 0;
	virtual bool getDeviceHardwares(_In_ std::wstring type, _Out_ DataBaseInfo& dbInfo) = 0;
	virtual bool getDeviceHardwaresWithMaxlevel(_In_ std::wstring type, _In_ float max_level, _Out_ DataBaseInfo& dbInfo) = 0;
	virtual bool getDeviceTypesTable(_In_ std::wstring type, _Out_ DataBaseInfo& dbInfo) = 0;
	virtual bool getDeviceMaxLevels(_In_ std::wstring type, _In_ std::wstring hardware, _Out_ DataBaseInfo& dbInfo) = 0;
	//*********************************************************************************************
	virtual bool getLastID(_In_ CString sTableName, _Out_ int &LastID) = 0;
	//*********************************************************************************************
	virtual bool SetTypePriority(_In_ int type, _In_ int value) = 0;
	
	virtual bool SetTypeMinBalance(_In_ int type, _In_ CString value) = 0;
	virtual bool SetTypeMinBalancePressure(_In_ std::wstring name, _In_ float max_level, _In_ CString value) = 0;
	
	virtual bool SetTypeInternalUse(_In_ int type, _In_ CString value) = 0;	
	virtual bool SetTypeInternalUsePressure(_In_ std::wstring name, _In_ float max_level, _In_ CString value) = 0;

	virtual std::wstring getCountOfDevicesWithType(_In_ std::wstring type) = 0;
	virtual std::wstring getCountOfDevicesWithTypeAndMaxlevel(_In_ std::wstring type, _In_ float max_level) = 0;

	virtual CString getTypeName(_In_ int type) = 0;	
protected:
	std::wstring m_sHost;

	std::map <Device, DeviceInfo> m_deviceInfo;
};

