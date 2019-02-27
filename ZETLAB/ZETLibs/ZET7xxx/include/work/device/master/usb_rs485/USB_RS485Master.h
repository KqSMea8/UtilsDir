#pragma once
#include "../base/usb/USBMaster.h"
#include "../base/rs485/RS485Master.h"

class CUSB_RS485Master: public CUSBMaster, public CRS485Master
{
public:
	CUSB_RS485Master(FT_DEVICE_LIST_INFO_NODE info);
	~CUSB_RS485Master();

	uint32_t getBaudRate() const override;
	uint8_t getParity() const override;

protected:
	ErrorCode connect() override;
	ErrorCode disconnect() override;

	void initialize() override;
	ErrorCode changeBaudRate(uint32_t baudRate) override;
	ErrorCode recoverBaudRate() override;
	ErrorCode changeParity(uint8_t parity) override;
	ErrorCode recoverParity() override;
	void updateDataExchangeParametersFromRestore() override;
	void updateInfoBySlaveFinder(_In_ CFunctionModule* pFunctionModule) override { CRS485Master::updateInfoBySlaveFinder(pFunctionModule); }

private:
	DWORD threadFunction() override;
};

