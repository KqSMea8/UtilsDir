#pragma once
#include "../../ModbusMaster.h"

class CRS485Master : public virtual CModbusMaster
{
public:
	CRS485Master();
	virtual ~CRS485Master();

protected:
	virtual void initialize() override;

	long getSleepTimeout(uint32_t packageLength);
	long Gett3_5();

	virtual void updateInfoBySlaveFinder(_In_ CFunctionModule* pFunctionModule);
};

