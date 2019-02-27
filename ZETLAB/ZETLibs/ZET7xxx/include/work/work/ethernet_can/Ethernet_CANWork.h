#pragma once
#include "../base/ethernet/EthernetWork.h"

#define _ETHERNET_CAN_

class CEthernet_CANWork : public CEthernetWork
{
public:
	CEthernet_CANWork();
	~CEthernet_CANWork();
};

