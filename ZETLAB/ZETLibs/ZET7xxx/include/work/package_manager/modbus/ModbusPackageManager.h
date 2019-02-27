#pragma once
#include "../PackageManager.h"

class CModbusPackageManager: public CPackageManager
{
public:
	CModbusPackageManager();
	~CModbusPackageManager();

	long GetTXPackageBuffer(unsigned char* ucBuffer, unsigned int uiMaxLength, DWORD& dwLength) override;
	virtual ResponseErrorCode CheckRXPackageBuffer(MODBUS_PACKAGE_STRUCT &mptPackage) = 0;
};

