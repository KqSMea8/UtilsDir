#pragma once
#include "../base/rs485/RS485Work.h"

#define _USB_RS485_

class CUsb_Rs485Work : public CRS485Work
{
public:
	CUsb_Rs485Work();
	~CUsb_Rs485Work();
};

