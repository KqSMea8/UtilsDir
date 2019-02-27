#pragma once
#include "../ModbusPackageManager.h"

class CPackageManagerUSB_CAN : public CModbusPackageManager
{
public:
	CPackageManagerUSB_CAN();
	~CPackageManagerUSB_CAN();

	ResponseErrorCode CheckRXPackageBuffer(MODBUS_PACKAGE_STRUCT &mptPackage) override;
};

