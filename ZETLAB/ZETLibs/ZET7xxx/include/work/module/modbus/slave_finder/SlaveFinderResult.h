#pragma once
#include "module\modbus\ModbusFunctionModuleResult.h"

class CSlaveFinderResult : public CModbusFunctionModuleResult
{
	friend class CSlaveFinder;

public:
	CSlaveFinderResult();
	~CSlaveFinderResult();

	ModbusAddress getAddress() const { return m_address; }
	bool isAddress() const { return m_configurations.end() != m_configurations.find(m_address); }

protected:
	void update(CModbusSlave* pSlave) override;
	void update(CModbusMaster* pMaster) override {};

private:
	std::map<ModbusAddress, VERSION_STRUCT> m_versions;
	std::map<ModbusAddress, Configuration> m_configurations;
	ModbusAddress m_address;
};

