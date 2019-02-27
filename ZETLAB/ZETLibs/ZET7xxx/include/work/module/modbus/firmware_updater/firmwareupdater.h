#pragma once
#include "module\modbus\ModbusFunctionModule.h"
#include "update_file_helper\UpdateFileHelper.h"

#include <map>

class CFirmwareUpdater : public CModbusFunctionModule
{
public:
	CFirmwareUpdater(const std::vector<CModbusSlave*>& devices, wchar_t* psPath);
	~CFirmwareUpdater();

	void setResponsePackage(MODBUS_PACKAGE_STRUCT& package);

	void getResult(_Outref_result_maybenull_ CModbusFunctionModuleResult*& pResult) override;

protected:
	void prepareSlave() override;
	void AddInfoToLogBySlave() override;
	int32_t getProgress();
	int32_t getProgress(Device device) override;

	void prepareForErase();
	void prepareForWrite();
	void prepareForRead();
	
	std::map<ModbusAddress, UpdateState> m_states;

	unsigned int m_currentFlashAddress;
	unsigned int m_currentFlashManipulateSize;

	wchar_t m_sPath[MAX_PATH];

	CFirmwareFileHelper* m_pUpdateFileHelper;
};

