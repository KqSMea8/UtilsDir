#pragma once
#include "..\ModbusFunctionModuleResult.h"

class CDeviceMarkerResult : public CModbusFunctionModuleResult
{
	friend class CDeviceMarker;

public:
	CDeviceMarkerResult();
	~CDeviceMarkerResult();

protected:
	void update(CModbusSlave* pSlave) override;
	void update(CModbusMaster* pMaster) override;

private:
	std::map<ModbusAddress, bool> m_selects;
};

