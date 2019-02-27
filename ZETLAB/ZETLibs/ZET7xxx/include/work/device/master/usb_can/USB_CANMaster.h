#pragma once
#include "../base/usb/USBMaster.h"
#include "../base/can/CANMaster.h"
#include <ftd2xx.h>

class CUSB_CANMaster : public CUSBMaster, public CCANMaster
{
public:
	CUSB_CANMaster(FT_DEVICE_LIST_INFO_NODE info);
	~CUSB_CANMaster();

protected:
	ErrorCode connect() override;
	ErrorCode disconnect() override;

	void initialize() override;
	ErrorCode changeBaudRate(uint32_t baudRate) override{ return CCANMaster::changeBaudRate(baudRate); }
	ErrorCode recoverBaudRate() override{ return CCANMaster::recoverBaudRate(); }
	ErrorCode changeParity(uint8_t parity) override{ return CCANMaster::changeParity(parity); }
	ErrorCode recoverParity() override{ return CCANMaster::recoverParity(); }
	void updateInfoBySlaveFinder(_In_ CFunctionModule* pFunctionModule) override { CCANMaster::updateInfoBySlaveFinder(pFunctionModule); }

private:
	DWORD threadFunction() override;
};

