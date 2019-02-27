#pragma once
#include "module\modbus\ModbusFunctionModule.h"

#include <ZETSimple\include\time\timer\Timer.h>

class CDataReader : public CModbusFunctionModule
{
public:
	CDataReader();
	~CDataReader();

	void getResult(_Outref_result_maybenull_ CModbusFunctionModuleResult*& pResult) override { pResult = nullptr; };

	int32_t getRequestPackage(_Out_ MODBUS_PACKAGE_STRUCT& package) override;
	void setResponsePackage(MODBUS_PACKAGE_STRUCT& package) override;

	void updateSlave(_In_ const CModbusSlave* pSlave) override;
	void removeDevice(_In_ const Device& device) override;

protected:
	virtual void prepareSlave() override{};
	void AddInfoToLogBySlave() override {};

private:
	double getDelta(double dFrequency);

	std::map<Device, std::map<DataChannel, std::pair<ChannelInfo, double>>> m_channels;

	CTimer m_timer;
	CRITICAL_SECTION m_csChannels;
};

