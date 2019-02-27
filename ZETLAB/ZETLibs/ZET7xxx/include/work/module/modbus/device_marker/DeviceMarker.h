#pragma once
#include "../ModbusFunctionModule.h"

class CDeviceMarker : public CModbusFunctionModule
{
public:
	CDeviceMarker(_In_ const std::vector<CModbusSlave*>& devices, bool bSelect);
	~CDeviceMarker();

	void setResponsePackage(MODBUS_PACKAGE_STRUCT& package);

	void getResult(_Outref_result_maybenull_ CModbusFunctionModuleResult*& pResult) override;
	
protected:
	void prepareSlave() override;
	void AddInfoToLogBySlave() override;

	std::map<ModbusAddress, bool> m_selects;
	bool m_bSelect;
};

