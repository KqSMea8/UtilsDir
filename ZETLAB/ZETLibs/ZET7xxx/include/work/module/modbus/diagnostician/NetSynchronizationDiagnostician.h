#pragma once
#include "Diagnostician.h"

class CNetSynchronizationDiagnostician : public CDiagnostician
{
public:
	CNetSynchronizationDiagnostician(_In_ const std::vector<CModbusSlave*> &devices, _In_ CDiagnostic* pDiagnostic);
	~CNetSynchronizationDiagnostician();

	void setResponsePackage(MODBUS_PACKAGE_STRUCT& package);

protected:
	void prepareSlave() override;
	
	bool isDiagnosticEnable() override { return true; };
private:
	int32_t m_stage;
	
	enum Stage
	{
		stageReadSensorPtp = 0,
		stageReadDiagnost,
	};

	void prepare4ReadSensorPtp();
	void prepare4ReadDiagnost();
};

