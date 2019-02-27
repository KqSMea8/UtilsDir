#pragma once
#include "../ModbusPackageManager.h"

class CPackageManagerRS485RTU : public CModbusPackageManager
{
public:
	CPackageManagerRS485RTU();
	~CPackageManagerRS485RTU();

	ResponseErrorCode CheckRXPackageBuffer(MODBUS_PACKAGE_STRUCT &mptPackage) override;
};

