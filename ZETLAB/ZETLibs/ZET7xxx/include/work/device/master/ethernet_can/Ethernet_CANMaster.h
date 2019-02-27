#pragma once
#include "../base/ethernet/EthernetMaster.h"
#include "../base/can/CANMaster.h"

class CEthernet_CANMaster : public CEthernetMaster, public CCANMaster
{
public:
	CEthernet_CANMaster(_In_ zet76_server* pServer, _In_ const zet76_device& device);
	~CEthernet_CANMaster();
	
protected:
	ErrorCode connect() override { return CEthernetMaster::connect(); }
	ErrorCode disconnect() override { return CEthernetMaster::disconnect(); }
	ErrorCode restartByUdp() override;

	void setConnectState(MasterConnectState mcsState) override { return CEthernetMaster::setConnectState(mcsState); }
	ErrorCode getIp(_Out_ std::wstring& sIp) override { return CEthernetMaster::getIp(sIp); }

	void initialize() override;
	ErrorCode changeBaudRate(uint32_t baudRate) override{ return CCANMaster::changeBaudRate(baudRate); }
	ErrorCode recoverBaudRate() override{ return CCANMaster::recoverBaudRate(); }
	ErrorCode changeParity(uint8_t parity) override{ return CCANMaster::changeParity(parity); }
	ErrorCode recoverParity() override{ return CCANMaster::recoverParity(); }
	void updateInfoBySlaveFinder(_In_ CFunctionModule* pFunctionModule) override { CCANMaster::updateInfoBySlaveFinder(pFunctionModule); }

private:
	DWORD threadFunction() override;
};

