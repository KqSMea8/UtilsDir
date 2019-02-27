#pragma once
#include "../PackageManager.h"

class CHiddenProtocolPackageManager: public CPackageManager
{
public:
	CHiddenProtocolPackageManager();
	~CHiddenProtocolPackageManager();

	long GetTXPackageBuffer(unsigned char* ucBuffer, unsigned int uiMaxLength, DWORD& dwLength) override;
	virtual ResponseErrorCode CheckRXPackageBuffer(MODBUS_PACKAGE_STRUCT &mptPackage) override;

protected:
	unsigned char m_ucWaitAnswerUntilTimeout;
};

