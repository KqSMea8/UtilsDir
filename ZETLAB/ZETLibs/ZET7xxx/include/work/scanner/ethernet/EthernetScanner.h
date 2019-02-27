#pragma once
#include "scanner\scanner.h"
#include "iohelper\ethernet\Zet76Work.h"

class CEthernetScanner : public CScanner
{
public:
	CEthernetScanner();
	~CEthernetScanner();

	void start() override;
	void stop() override;
	
	void addMasterWork(_In_ const std::pair<ZET7XXX_DEVICE_TYPE, CMasterWork*>& masterWork) override;
	void removeMasterWork(ZET7XXX_DEVICE_TYPE type) override;

protected:
	void statusEthernet(_In_ zet76_server* pServer, _In_ const zet76_device* pDevice, int state, int flags);

private:
	DWORD threadFunction() override;

	zet76_server* m_pZet76Server;

	std::vector<std::tuple<zet76_device, int, int>> m_devices;
	CRITICAL_SECTION m_csScan;
};

