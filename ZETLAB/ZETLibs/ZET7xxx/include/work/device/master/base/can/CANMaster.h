#pragma once
#include "../../ModbusMaster.h"

class CCANMaster : public virtual CModbusMaster
{
public:
	CCANMaster();
	virtual ~CCANMaster();

protected:
	virtual void initialize() override = 0;
	ErrorCode changeBaudRate(uint32_t baudRate) override{ return NoError; }
	ErrorCode recoverBaudRate() override{ return NoError; }
	ErrorCode changeParity(uint8_t parity) override{ return NoError; }
	ErrorCode recoverParity() override{ return NoError; }

	virtual void updateInfoBySlaveFinder(_In_ CFunctionModule* pFunctionModule) override;
};

