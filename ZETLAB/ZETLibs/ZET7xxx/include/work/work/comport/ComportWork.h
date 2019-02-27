#pragma once
#include "../base/rs485/RS485Work.h"

#define _COMPORT_

class CComportWork : public CRS485Work
{
public:
	CComportWork();
	~CComportWork();

	void start() override;
};

