#pragma once
#include "..\ModbusFunctionModuleResult.h"

class CFirmwareUpdaterResult : public CModbusFunctionModuleResult
{
	friend class CFirmwareUpdater;

public:
	CFirmwareUpdaterResult();
	~CFirmwareUpdaterResult();

protected:
	void update(CModbusSlave* pSlave) override;
	void update(CModbusMaster* pMaster) override;

private:
	std::map<ModbusAddress, UpdateState> m_states;
};

