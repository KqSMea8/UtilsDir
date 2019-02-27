#pragma once

#include "module\modbus\ModbusFunctionModule.h"

class CFirmwareReader : public CModbusFunctionModule
{
public:
	CFirmwareReader(_In_ const std::vector<CModbusSlave*>& devices);
	~CFirmwareReader();

	void setResponsePackage(MODBUS_PACKAGE_STRUCT& package);

	void getResult(_Outref_result_maybenull_ CModbusFunctionModuleResult*& pResult) override;

protected:
	void prepareSlave() override;
	void AddInfoToLogBySlave() override;
	int32_t getProgress();
	int32_t getProgress(Device device) override;

	void prepareForRead();

private:
	uint32_t m_currentFlashAddress;
	uint32_t m_currentFlashManipulateSize;

	std::map<ModbusAddress, std::vector<uint8_t>> m_firmwares;
};

