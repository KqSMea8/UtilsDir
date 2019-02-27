#pragma once
#include "../../ModbusMaster.h"
#include "../../../../iohelper/ethernet/Zet76Work.h"

class CEthernetMaster : public virtual CModbusMaster
{
public:
	CEthernetMaster(_In_ zet76_server* pServer, _In_ const zet76_device& device);
	virtual ~CEthernetMaster();

protected:
	virtual ErrorCode connect() override;
	virtual ErrorCode disconnect() override;

	virtual void initialize() override;
	virtual void setConnectState(MasterConnectState mcsState) override;
	
	virtual ErrorCode getIp(_Out_ std::wstring& sIp) override;

	zet76_server* m_server;
	zet76_device m_76device;
	CZet76Work m_zet76Work;

	std::wstring m_sIp;
	
	enum
	{
		unknown = -1,
		rtu = 0,
		tcp = 1,
	};
};

