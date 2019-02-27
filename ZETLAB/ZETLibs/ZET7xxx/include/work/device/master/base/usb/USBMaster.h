#pragma once
#include "device\master\ModbusMaster.h"

#include "iohelper\ftdi\libFTDI.h"
#include <ftd2xx.h>

class CUSBMaster : public virtual CModbusMaster
{
public:
	CUSBMaster(_In_ FT_DEVICE_LIST_INFO_NODE info);
	virtual ~CUSBMaster();

protected:
	virtual void initialize() override = 0;

	FT_HANDLE m_ftHandle;
	FT_DEVICE_LIST_INFO_NODE m_ftDeviceInfo;

	LibFTDI* m_pFtdi;
};

