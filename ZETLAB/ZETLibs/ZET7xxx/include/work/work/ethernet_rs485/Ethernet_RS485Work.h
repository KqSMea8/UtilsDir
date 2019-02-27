#pragma once
#include "../base/ethernet/EthernetWork.h"
#include "../base/rs485/RS485Work.h"

#define _ETHERNET_RS485_

class CEthernet_RS485Work : public CEthernetWork, public CRS485Work
{
public:
	CEthernet_RS485Work();
	~CEthernet_RS485Work();

protected:
	ErrorCode restartMasterByUdp(_In_ const Device& master) override { return CEthernetWork::restartMasterByUdp(master); }
};

