#pragma once
#include "module\modbus\ModbusFunctionModule.h"
#include "diagnostic\Diagnostic.h"

class CDiagnostician : public CModbusFunctionModule
{
public:
	CDiagnostician(_In_ CDiagnostic* pDiagnostic);
	~CDiagnostician();

	void getResult(_Outref_result_maybenull_ CModbusFunctionModuleResult*& pResult) override { pResult = nullptr; };

	bool isCommandActive(ModbusCommand command);
	void setCommandActive(ModbusCommand command, bool bActive);

protected:
	virtual void prepareSlave() override;
	void AddInfoToLogBySlave() override {};

	virtual bool isDiagnosticEnable() = 0;
	
	std::map<ModbusCommand, bool> m_commands;
	CDiagnostic* m_pDiagnostic;
};

