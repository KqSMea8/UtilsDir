#pragma once
#include "../ModbusPackageManager.h"

class CPackageManagerRS485TCP : public CModbusPackageManager
{
public:
	CPackageManagerRS485TCP();
	~CPackageManagerRS485TCP();

	ResponseErrorCode CheckRXPackageBuffer(MODBUS_PACKAGE_STRUCT &mptPackage) override;
};

