#pragma once
#include "../ModbusFunctionModule.h"

#include <map>

class CConfigurationWriter : public CModbusFunctionModule
{
public:
	CConfigurationWriter(const std::vector<CModbusSlave*>& devices);
	~CConfigurationWriter();

	int32_t getRequestPackage(_Out_ MODBUS_PACKAGE_STRUCT& package) override;
	void setResponsePackage(MODBUS_PACKAGE_STRUCT& package);

	void getResult(_Outref_result_maybenull_ CModbusFunctionModuleResult*& pResult) override { pResult = nullptr; };

protected:
	void prepareSlave() override;
	void AddInfoToLogBySlave() override;

	int32_t getProgress();
	int32_t getProgress(Device device) override;

	bool IsValidForRestore(pSTRUCT_HEAD pSrc, pSTRUCT_HEAD pDst);
	bool IsValidForRestoreAddress();

private:
	void ConstructDevice4Restore(Configuration& Src, Configuration& Dst);
	void ConstructDevice4RestoreAddress(Configuration& Src, Configuration& Dst);
	
	void Prepare4Write1();
	void Prepare4Write2();
	void Prepare4Write3();

	std::map<uint16_t, Configuration> m_mDeviceRestore;
	std::map<uint16_t, Configuration>::iterator m_itDeviceRestore;
	double m_deviceRestoreProgress;
};

