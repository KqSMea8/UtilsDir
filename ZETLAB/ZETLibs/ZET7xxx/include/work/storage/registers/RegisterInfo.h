#pragma once

#include "base\Types.h"

using namespace zet7xxx::modbus;

class CRegisterInfo
{
public:
	CRegisterInfo();
	~CRegisterInfo();

	void putInfo(DeviceRegisterValue registerValue, _In_ RegisterData& data);

	DeviceRegisterValue getRegister(){ return m_registerValue; }
	void getInfo(_Out_ RegisterData& data){ data = m_data; }

protected:
	DeviceRegisterValue m_registerValue;
	RegisterData m_data;
};


