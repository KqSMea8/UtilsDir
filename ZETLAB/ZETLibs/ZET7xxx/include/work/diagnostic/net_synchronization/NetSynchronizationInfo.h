#pragma once
#include "base\Types.h"
#include "base\ErrorCodes.h"
#include "diagnostic\DiagnosticInfo.h"

using namespace zet7xxx;
using namespace modbus;

class CNetSynchronizationInfo : public CDiagnosticInfo
{
public:
	CNetSynchronizationInfo();
	~CNetSynchronizationInfo();

	void putInfo(ModbusAddress address, _In_ Configuration& configuration);

	ModbusAddress getAddress(){ return m_address; }
	void getInfo(_Out_ Configuration& configuration){ configuration = m_configuration; }

protected:
	ModbusAddress m_address;
	Configuration m_configuration;
};

