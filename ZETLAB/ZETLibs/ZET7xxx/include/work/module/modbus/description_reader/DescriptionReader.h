#pragma once
#include "../ModbusFunctionModule.h"

#include <map>

class CDescriptionReader : public CModbusFunctionModule
{
public:
	CDescriptionReader(const std::vector<CModbusSlave*>& devices);
	~CDescriptionReader();

	void setResponsePackage(MODBUS_PACKAGE_STRUCT& package);

	void getResult(_Outref_result_maybenull_ CModbusFunctionModuleResult*& pResult) override;

protected:
	void prepareSlave() override;
	void AddInfoToLogBySlave() override;

private:
	std::map<ModbusAddress, Description> m_descriptions;
};

