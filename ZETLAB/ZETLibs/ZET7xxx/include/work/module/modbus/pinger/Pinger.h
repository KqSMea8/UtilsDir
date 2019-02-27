#pragma once
#include "module\modbus\slave_finder\SlaveFinder.h"

class CPinger : public CSlaveFinder
{
public:
	CPinger(_In_ const ModbusAddress address);
	~CPinger();
};

