#pragma once
#include "../../MasterWork.h"

class CEthernetWork : public virtual CMasterWork
{
public:
	CEthernetWork(ZET7XXX_DEVICE_TYPE zdtType);
	~CEthernetWork();

protected:
	ErrorCode restartMasterByUdp(_In_ const Device& master) override;
};

