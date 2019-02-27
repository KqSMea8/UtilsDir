#pragma once
#include "Diagnostician.h"

#include <random>

class CDataExchangeDiagnostician : public CDiagnostician
{
public:
	CDataExchangeDiagnostician(_In_ const std::vector<CModbusSlave*> &devices, _In_ CDiagnostic* pDiagnostic);
	~CDataExchangeDiagnostician();

	void setResponsePackage(MODBUS_PACKAGE_STRUCT& package);

protected:
	void prepareSlave() override;
	void setAddressActive(_In_ const ModbusAddress address, bool bActive) override;
	void restart() override;

	bool isDiagnosticEnable() override;

private:
	int32_t m_stage;
	CModbusDevice* m_pMaster;

	typedef std::pair<ModbusAddress, bool> ActiveAddress;
	std::list<ActiveAddress> m_activity;

	ModbusDataAddress m_currentRhrRegisterAddress;
	uint16_t m_currentRhrRequestSize;

	bool m_bNeedRestartRand;
	bool m_bNeedGenerateRand;

	std::mt19937* m_pPseudoRandomGenerator;
	std::uniform_int_distribution<> m_registerAddressDistribution;
	std::uniform_int_distribution<> m_requestSizeDistribution;

	enum Stage
	{
		stageReadHoldingRegisters = 0,
		stageReadInputRegisters,
	};

	void prepare4ReadHoldingRegisters();
	void prepare4ReadInputRegisters();
	void prepare4Activity(ActiveAddress activity);
};

