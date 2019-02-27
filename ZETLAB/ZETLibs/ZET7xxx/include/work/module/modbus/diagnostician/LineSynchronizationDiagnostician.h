#pragma once
#include "Diagnostician.h"

class CLineSynchronizationDiagnostician : public CDiagnostician
{
public:
	CLineSynchronizationDiagnostician(_In_ const std::vector<CModbusSlave*> &devices, _In_ CDiagnostic* pDiagnostic);
	~CLineSynchronizationDiagnostician();

	void setResponsePackage(MODBUS_PACKAGE_STRUCT& package);

protected:
	void prepareSlave() override;
	
	bool isDiagnosticEnable() override { return true; };
private:
	int32_t m_stage;
	
	enum Stage
	{
		stageReadUartPar = 0,
		stageReadCanPar,
		stageReadSensorPtp,
		stageReadSensorGps,
		stageReadDiagnost,
	};

	void prepare4ReadUartPar();
	void prepare4ReadCanPar();
	void prepare4ReadSensorPtp();
	void prepare4ReadSensorGps();
	void prepare4ReadDiagnost();
};

