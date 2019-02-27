#pragma once
#include "../ModbusPackageManager.h"

class CPackageManagerEthernet_CAN :	public CModbusPackageManager
{
public:
	CPackageManagerEthernet_CAN();
	~CPackageManagerEthernet_CAN();

	ResponseErrorCode CheckRXPackageBuffer(MODBUS_PACKAGE_STRUCT &mptPackage) override;
};

