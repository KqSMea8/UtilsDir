#pragma once
#include "base\ModbusBase.h"

class CModbusDevice : public CModbusBase
{
	friend class CErrorReaderResult;
	friend class CFirmwareUpdaterResult;
	friend class CFirmwareReaderResult;
	friend class CDeviceMarkerResult;
	
public:
	CModbusDevice();
	CModbusDevice(_In_ const CModbusDevice &value);
	virtual ~CModbusDevice();

	void operator = (_In_ const CModbusDevice &value);

	Device getId() const;
	std::wstring getName() const;
	virtual std::wstring getNameWithSerial() const;
	virtual uint32_t getBaudRate() const;
	virtual uint8_t getParity() const;
	VERSION_STRUCT getFirmwareVersion() const;
	time_t getFirmwareVersionTime() const;

	bool IsMarked() const;

	uint32_t getErrorList(_Outref_result_maybenull_ PERROR_STRUCT& pErrors) const;
	
	void getConfiguration(_Out_ Configuration& configuration) const;
	
	void saveFirmware(_In_ const std::wstring& sPath);
	virtual void saveConfiguration(_In_ const std::wstring sPath) = 0;
	virtual void saveAddressTable(_In_ const std::wstring sPath, _In_ const bool bMetrology) = 0;
	
	std::pair<uint16_t, uint16_t> getStructure(_In_ const STRUCT_TYPE type) const;
	
protected:
	Device m_id;
	uint32_t m_baudRate;
	uint8_t m_parity;
	std::wstring m_sName;
	VERSION_STRUCT m_firmwareVersion;
	time_t m_tVersionTime;
	ErrorList m_errors;
	bool m_bMarked;
	UpdateState m_updateState;
	ModbusAddress m_address;

	Configuration m_configuration;
	Description m_description;
	std::multimap<STRUCT_TYPE, std::pair<uint16_t, uint16_t>> m_structures;
	std::vector<uint8_t> m_firmware;
};

