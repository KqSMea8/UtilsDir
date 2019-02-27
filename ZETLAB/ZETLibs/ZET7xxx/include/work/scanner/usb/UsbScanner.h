#pragma once
#include "scanner\scanner.h"

#include "iohelper\ftdi\libFTDI.h"

class CUsbScanner : public CScanner
{
	friend class CModbusWork;

public:
	CUsbScanner();
	~CUsbScanner();

	void start() override;
	void stop() override;
	
	void addMasterWork(_In_ const std::pair<ZET7XXX_DEVICE_TYPE, CMasterWork*>& masterWork) override;
	void removeMasterWork(ZET7XXX_DEVICE_TYPE type) override;

protected:

private:
	DWORD threadFunction() override;

	void scan();
	void scanRs485(_In_opt_ FT_DEVICE_LIST_INFO_NODE* pInfo, _In_ uint32_t count);
	void scanCan(_In_opt_ FT_DEVICE_LIST_INFO_NODE* pInfo, _In_ uint32_t count);

	LibFTDI* m_pFtdi;
};

