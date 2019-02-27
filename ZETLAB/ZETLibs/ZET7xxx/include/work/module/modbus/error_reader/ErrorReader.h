#pragma once
#include "../ModbusFunctionModule.h"

#include <map>

class CErrorReader : public CModbusFunctionModule
{
public:
	CErrorReader(const std::vector<CModbusSlave*>& devices);
	~CErrorReader();

	void setResponsePackage(MODBUS_PACKAGE_STRUCT& package);
	void getResult(_Outref_result_maybenull_ CModbusFunctionModuleResult*& pResult) override;

protected:
	void prepareSlave() override;
	void AddInfoToLogBySlave() override;
	int32_t getProgress() override;
	int32_t getProgress(Device device) override;

	std::map<uint8_t, ErrorList> m_errors;

	uint32_t m_currentFlashAddress;
	uint32_t m_currentFlashReadSize;
};

