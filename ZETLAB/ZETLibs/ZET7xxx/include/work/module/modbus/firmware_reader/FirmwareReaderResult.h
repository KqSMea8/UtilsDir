#pragma once
#include "module\modbus\ModbusFunctionModuleResult.h"

class CFirmwareReaderResult : public CModbusFunctionModuleResult
{
	friend class CFirmwareReader;

public:
	CFirmwareReaderResult();
	~CFirmwareReaderResult();

protected:
	void update(CModbusSlave* pSlave) override;
	void update(CModbusMaster* pMaster) override;

private:
	std::map<ModbusAddress, std::vector<uint8_t>> m_firmwares;
};

