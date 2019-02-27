#pragma once
#include "..\ModbusFunctionModuleResult.h"

class CErrorReaderResult : public CModbusFunctionModuleResult
{
	friend class CErrorReader;

public:
	CErrorReaderResult();
	~CErrorReaderResult();

protected:
	void update(CModbusSlave* pSlave) override;
	void update(CModbusMaster* pMaster) override;

private:
	std::map<uint8_t, ErrorList> m_errors;
};

