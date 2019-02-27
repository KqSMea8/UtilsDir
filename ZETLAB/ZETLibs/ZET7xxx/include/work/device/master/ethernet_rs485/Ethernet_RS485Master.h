#pragma once
#include "../base/ethernet/EthernetMaster.h"
#include "../base/rs485/RS485Master.h"

class CEthernet_RS485Master : public CEthernetMaster, public CRS485Master
{
public:
	CEthernet_RS485Master(_In_ zet76_server* pServer, _In_ const zet76_device& device);
	~CEthernet_RS485Master();

protected:
	ErrorCode connect() override { return CEthernetMaster::connect(); }
	ErrorCode disconnect() override { return CEthernetMaster::disconnect(); }

	ErrorCode getIp(_Out_ std::wstring& sIp) override { return CEthernetMaster::getIp(sIp); }

	void initialize() override;
	ErrorCode changeBaudRate(uint32_t baudRate) override{ return NoError; };
	ErrorCode recoverBaudRate() override{ return NoError; };
	ErrorCode changeParity(uint8_t parity) override{ return NoError; }
	ErrorCode recoverParity() override{ return NoError; }
	void setConnectState(MasterConnectState mcsState) override;
	void updateInfoBySlaveFinder(_In_ CFunctionModule* pFunctionModule) override { CRS485Master::updateInfoBySlaveFinder(pFunctionModule); }

private:
	DWORD threadFunction() override;

	uint8_t m_type;
};

