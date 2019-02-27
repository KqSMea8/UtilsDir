#pragma once
#include "device\slave\ModbusSlave.h"
#include "device\master\ModbusMaster.h"

class CModbusFunctionModuleResult
{
public:
	CModbusFunctionModuleResult();
	virtual ~CModbusFunctionModuleResult();

	virtual void update(CModbusSlave* pSlave) = 0;
	virtual void update(CModbusMaster* pMaster) = 0;
};

