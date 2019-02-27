#pragma once
#include "../MasterWork.h"

#define _ETHERNET_CAN_
#define _ETHERNET_RS485_

class CEthernetWork : public CMasterWork
{
public:
	CEthernetWork(ZET7XXX_DEVICE_TYPE zdtType);
	~CEthernetWork();

protected:
	ErrorCode restartMasterByUdp(_In_ const Device& master) override;
};

