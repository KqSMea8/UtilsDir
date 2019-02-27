#pragma once
#include "..\ModbusFunctionModuleResult.h"

class CDescriptionReaderResult : public CModbusFunctionModuleResult
{
	friend class CDescriptionReader;

public:
	CDescriptionReaderResult();
	~CDescriptionReaderResult();

protected:
	void update(CModbusSlave* pSlave) override;
	void update(CModbusMaster* pMaster) override;

private:
	std::map<ModbusAddress, Description> m_descriptions;
};

