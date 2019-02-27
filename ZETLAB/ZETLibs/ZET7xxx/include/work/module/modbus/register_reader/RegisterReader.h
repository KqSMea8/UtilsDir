#pragma once
#include "module\modbus\ModbusFunctionModule.h"
#include "storage\registers\RegistersStorage.h"

class CRegisterReader : public CModbusFunctionModule
{
public:
	CRegisterReader(_In_ CRegistersStorage* pRegistersStorage);
	~CRegisterReader();

	void getResult(_Outref_result_maybenull_ CModbusFunctionModuleResult*& pResult) override { pResult = nullptr; }

	int32_t getRequestPackage(_Out_ MODBUS_PACKAGE_STRUCT& package) override;
	void setResponsePackage(MODBUS_PACKAGE_STRUCT& package) override;

	void addRegister(ModbusAddress address, RegisterValue registerValue);
	void clearRegisters();

protected:
	virtual void prepareSlave() override;
	void AddInfoToLogBySlave() override {};

	std::map <ModbusAddress, RegisterValueList> m_registers;
	RegisterValue m_currentRegister;

	CRegistersStorage* m_pRegistersStorage;
};

