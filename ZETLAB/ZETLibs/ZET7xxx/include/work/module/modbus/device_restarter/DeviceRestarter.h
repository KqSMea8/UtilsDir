#pragma once
#include "../ModbusFunctionModule.h"

#include <ZETSimple\include\time\timer\difftimer\DiffTimer.h>

#include <map>

class CDeviceRestarter : public CModbusFunctionModule
{
public:
	CDeviceRestarter(const std::vector<CModbusSlave*>& devices);
	~CDeviceRestarter();

	void setResponsePackage(MODBUS_PACKAGE_STRUCT& package);

	void getResult(_Outref_result_maybenull_ CModbusFunctionModuleResult*& pResult) override { pResult = nullptr; };

protected:
	void prepareSlave() override;
	void AddInfoToLogBySlave() override {};
	int32_t getProgress();

	void prepare4Restart();
	void prepare4Waiting();

private:
	CDiffTimer m_timer;
	std::map<uint8_t, uint16_t> m_alives;
	uint32_t m_stage;

	enum Stage
	{
		stageRestart = 0,
		stageSearchAliveBeforePower,
		stageSearchAliveAfterPower,
	};
};

