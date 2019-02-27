#pragma once
#include "base\ErrorCodes.h"
#include "base\Types.h"

#include <boost\noncopyable.hpp>

#include <vector>

using namespace zet7xxx;
using namespace errors;

class CPackageManager : private boost::noncopyable
{
public:
	CPackageManager();
	~CPackageManager();

	void SetTXPackage(const MODBUS_PACKAGE_STRUCT &mptPackage);
	virtual long GetTXPackageBuffer(uint8_t* ucBuffer, unsigned int uiMaxLength, DWORD& dwLength) = 0;
	void ClearRXPackageBuffer();
	void AppendRXPackageBuffer(unsigned char* ucBuffer, DWORD dwLength);
	virtual ResponseErrorCode CheckRXPackageBuffer(MODBUS_PACKAGE_STRUCT &mptPackage) = 0;
	uint32_t GetRXPackageBufferSize();
	uint32_t CompareBuffers();

protected:
	MODBUS_PACKAGE_STRUCT m_mptTXPackage;
	std::vector <unsigned char> m_ucRXBuffer;
};

