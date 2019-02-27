#pragma once
#include "module\modbus\ModbusFunctionModule.h"

#include <Zet7xxxDefinitions.h>

#include <map>

class CSlaveFinder : public CModbusFunctionModule
{
public:
	CSlaveFinder();
	~CSlaveFinder();

	void setResponsePackage(MODBUS_PACKAGE_STRUCT& package);

	void getDevices(_Outref_result_maybenull_ PDevice& pDevices, _Out_ uint32_t& count);
	void getResult(_Outref_result_maybenull_ CModbusFunctionModuleResult*& pResult) override;

protected:
	void prepareSlave() override;
	void AddInfoToLogBySlave() override;
	
private:
	void addDataToConfiguration(_In_ const ModbusAddress address, _In_ const std::vector<uint16_t>& data);
		
	std::map<ModbusAddress, VERSION_STRUCT> m_versions;
	std::map<ModbusAddress, Configuration> m_configurations;
	std::vector<ModbusAddress> m_added2Log;

	int32_t m_stage;

	enum Stage
	{
		stageUnknown = 0,
		stageGetVersion,
		stageReadConfiguration,
		stageSynchronization,
		stageStopDataStream,
	};

	void prepare4GetVersion();
	void prepare4ReadConfiguration();
	void prepare4Synchronization();
	void prepare4StopDataStream();
};

