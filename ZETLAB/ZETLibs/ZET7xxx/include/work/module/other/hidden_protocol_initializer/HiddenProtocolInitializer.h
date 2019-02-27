#pragma once
#include "module\FunctionModule.h"

#include <ZETSimple\include\time\timer\difftimer\DiffTimer.h>

#include <map>

class CHiddenProtocolInitializer : public CFunctionModule
{
public:
	CHiddenProtocolInitializer();
	~CHiddenProtocolInitializer();

	int32_t getRequestPackage(_Out_ MODBUS_PACKAGE_STRUCT& package);
	void setResponsePackage(MODBUS_PACKAGE_STRUCT& package);

	int32_t getProgress();

protected:
	void restart() override {};
	void parseAnswerFromBroadcast(unsigned char* ucAnswer, unsigned short &usSize);
	void MakeNewAddressDistribution();
	void prepare4Broadcast();
	void prepare4AddressAsignment();

	std::map<uint64_t, std::pair<uint8_t, uint8_t>> m_mDevices;
	uint8_t m_ucState;

	CDiffTimer m_timer;
	uint32_t m_nDeviceCount;

	enum 
	{
		hpStateBroadcast = 0,
		hpStateAddressAsignment,
		hpStateEnd,
	};
};

