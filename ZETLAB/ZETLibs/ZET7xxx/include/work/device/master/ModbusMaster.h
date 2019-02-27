#pragma once
#include "base\thread_module\thread_module.h"
#include "device\ModbusDevice.h"
#include "device\slave\ModbusSlave.h"
#include "module\FunctionModule.h"
#include "module\modbus\pinger\Pinger.h"
#include "package_manager\modbus\ModbusPackageManager.h"
#include "package_manager\hidden_protocol\HiddenProtocolPackageManager.h"
#include "diagnostic\Diagnostic.h"
#include "storage\registers\RegistersStorage.h"

#include <boost\noncopyable.hpp>

#include <map>
#include <functional>


using namespace zet7xxx::diagnostic;
using namespace zet7xxx::modbus;
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
class CModbusMaster : public CModbusDevice, protected CThreadModule
{
	friend class CEthernetScanner;
	friend class CUsbScanner;
	friend class CDescriptionReaderResult;
//----------------------------------------------------------------------------------------------------------------------
public:
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
#pragma region Constructors and destructor
	///	Конструктор по умолчанию
	CModbusMaster();
	/// Деструктор
	virtual ~CModbusMaster();
#pragma endregion Конструкторы и деструктор
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
#pragma region Methods
	ErrorCode findSlaves();
	ErrorCode findSlavesEx();
	ErrorCode initSlaves();
	virtual ErrorCode startDataExchangeDiagnostic();
	ErrorCode startSynchronizationDiagnostic();
	ErrorCode startNetSynchronizationDiagnostic();
	ErrorCode startDataQualityDiagnostic();
	ErrorCode startReadingRegisters();
	ErrorCode startReadingData();
	ErrorCode getSlaves(_Outref_result_maybenull_ PDevice& pSlaves, _Out_ uint32_t& count);
	ErrorCode getDevices(_Outref_result_maybenull_ PDevice& pDevices, _Out_ uint32_t& count);
	uint32_t getDeviceCount();
	uint32_t getChannelCount();
	virtual ErrorCode connect() = 0;
	virtual ErrorCode disconnect() = 0;
	virtual ErrorCode restartByUdp(){ return NoError; };
	ErrorCode readErrors(_In_ const PDevice& pDevices, uint32_t count);
	ErrorCode updateFirmware(_In_ const PDevice& pDevices, uint32_t count, _In_ wchar_t* sPath);
	ErrorCode readFirmware(_In_ const PDevice& pDevices, uint32_t count);
	ErrorCode selectDevices(_In_ const PDevice& pDevices, uint32_t count, bool bSelect);
	ErrorCode restartDevices(_In_ const PDevice& pDevices, uint32_t count);
	int32_t updateConfiguration(_In_ const PDevice& pDevices, uint32_t nCount);
	int32_t readDescriptors(_In_ const PDevice& pDevices, uint32_t nCount);
	int32_t saveConfiguration(_In_ const PDevice& pDevices, uint32_t nCount, std::wstring sPath);
	int32_t saveAddressTable(_In_ const PDevice& pDevices, uint32_t nCount, std::wstring sPath, bool bMetrology);
	ErrorCode saveFirmware(_In_ const PDevice& pDevices, _In_ const uint32_t count, _In_ const std::wstring& sPath);
	virtual ErrorCode changeBaudRate(uint32_t baudRate) = 0;
	virtual ErrorCode recoverBaudRate() = 0;
	virtual ErrorCode changeParity(uint8_t parity) = 0;
	virtual ErrorCode recoverParity() = 0;
	ErrorCode getProcessDelay(uint32_t& delay);
	ErrorCode setProcessDelay(uint32_t delay);
	bool isConnected() const { return mcsConnected == m_mcsState; };
	bool isFindSlaveExEnable() const { return m_bFindSlaveExEnable; }
	ErrorCode subscribeToRestartPower(const std::function<void()>& func);
	ErrorCode subscribeToChangeMasterConnectState(const std::function<void(Device, MasterConnectState)>& func);
	ErrorCode subscribeToChangeSlaveCount(const std::function<void(Device)>& func);
	ErrorCode stopFunctionModule(_In_ bool bUpdateInfo);
	virtual uint32_t getBaudRate() const override { return CModbusDevice::getBaudRate(); }
	virtual uint8_t getParity() const override { return CModbusDevice::getParity(); }
	void getEvents(_Out_ LogEventList& eventLog) override;
	virtual ErrorCode getIp(_Out_ std::wstring& sIp);
	void setConfiguration4Restore(_In_ const Configuration& configuration);
//----------------------------------------------------------------------------------------------------------------------
#pragma region FunctionModule methods
	ErrorCode getFunctionModuleState(_Out_ FunctionModuleState& state) const;
	ErrorCode getFunctionModuleProgress(_Out_ int32_t& progress);
	ErrorCode getFunctionModuleType(_Out_ FunctionModuleType& type);
#pragma endregion Методы, касающиеся функциональных модулей
//----------------------------------------------------------------------------------------------------------------------
#pragma region Slave parameters methods
	ErrorCode getSlaveDataAddresses(_In_ const Device& slave,
		_Outref_result_maybenull_ uint16_t*& pDataAddresses, _Out_ uint32_t& count);
	ErrorCode getSlaveChannels(_In_ const Device& slave,
		_Outref_result_maybenull_ std::wstring*& pChannels, _Out_ uint32_t& count);
	ErrorCode getSlaveChannelsInfo(_In_ const Device& slave,
		_Outref_result_maybenull_ ChannelInfo*& pChannelsInfo, _Out_ uint32_t& count);
	ErrorCode getSlaveFrequencies(_In_ const Device& slave,
		_Outref_result_maybenull_ float*& pFrequencies, _Out_ uint32_t& count);
	ErrorCode getSlaveAddresses(_In_ const Device& slave,
		_Outref_result_maybenull_ ModbusAddress*& pAddresses, _Out_ uint32_t& count);
	ErrorCode getSlaveAddress(_In_ const Device& slave, _Out_ ModbusAddress& address);
#pragma endregion Методы для опроса параметров слейвов
//----------------------------------------------------------------------------------------------------------------------
#pragma region Device parameters methods
	ErrorCode getDeviceId(_In_ ModbusAddress address, _Out_ Device& device);
	bool hasDevice(_In_ const Device& device);
	ErrorCode getDeviceName(_In_ const Device& device, _Out_ std::wstring& sName);
	ErrorCode getDeviceNameWithSerial(_In_ const Device& device, _Out_ std::wstring& sNameWithSerial);
	ErrorCode getDeviceBaudRate(_In_ const Device& device, _Out_ uint32_t& baudRate);
	ErrorCode getDeviceParity(_In_ const Device& device, _Out_ uint8_t& parity);
	ErrorCode getDeviceFunctionModuleProgress(_In_ const Device& device, _Out_ uint32_t& progress);
	ErrorCode getDeviceVersion(_In_ const Device& device, _Out_ VERSION_STRUCT& version);
	ErrorCode getDeviceVersion(_In_ const Device& device, _Out_ time_t& version);
	ErrorCode getDeviceMark(_In_ const Device& device, _Out_ bool& bMark);
	ErrorCode getDeviceErrorList(_In_ const Device& device,
		_Outref_result_maybenull_ PERROR_STRUCT& pErrors, _Out_ uint32_t& count);
	ErrorCode getDeviceConfiguration(_In_ const Device& device,
		_In_ const ModbusAddress address, _Out_ Configuration& configuration);
	ErrorCode setDeviceConfiguration4Restore(_In_ const Device& device,
		_In_ const ModbusAddress address, _In_ Configuration& configuration);
#pragma endregion Методы для опроса параметров устройств
//----------------------------------------------------------------------------------------------------------------------
#pragma region Channel parameters methods
	ErrorCode getDeviceChannelInfo(_In_ const Device& slave, _Out_ ChannelInfo& info);
#pragma endregion Методы для опроса параметров каналов
//----------------------------------------------------------------------------------------------------------------------
#pragma region Diagnostic methods
	ErrorCode getDiagnosticAddressEnable(uint8_t address, _Out_ bool& bEnable);
	ErrorCode getDiagnosticAddressActive(uint8_t address, _Out_ bool& bActive);
	ErrorCode setDiagnosticAddressActive(uint8_t address, bool bActive);
	ErrorCode getDiagnosticCommandActive(ModbusCommand address, _Out_ bool& bActive);
	ErrorCode setDiagnosticCommandActive(ModbusCommand command, bool bActive);
	ErrorCode getDiagnosticAddressParameters(_Outref_result_maybenull_ DiagnosticParameter*& pParameters, _Out_ uint32_t& count);
	template <typename T>
	ErrorCode getDiagnosticInfo(DiagnosticParameter field, _Out_ T& info)
	{
		info = T(0);

		if (m_fmsState != fmsStarted)
			return ModuleNotActivated;

		EnterCriticalSection(&m_csFunctionModule);
		if (m_pFunctionModule == nullptr)
		{
			LeaveCriticalSection(&m_csFunctionModule);
			return ModuleNotActivated;
		}
		LeaveCriticalSection(&m_csFunctionModule);

		EnterCriticalSection(&m_csDiagnostic);
		if (m_pDiagnostic == nullptr)
		{
			LeaveCriticalSection(&m_csDiagnostic);
			return ModuleNotActivated;
		}

		DiagnosticErrorCode code = m_pDiagnostic->getDiagnosticInfo(field, info);
		LeaveCriticalSection(&m_csDiagnostic);

		return code == decNoError ? NoError : DiagnosticError;
	}
	template <typename T>
	ErrorCode getDiagnosticInfo(uint8_t address, DiagnosticParameter field, _Out_ T& info)
	{
		info = T(0);

		if (m_fmsState != fmsStarted)
			return ModuleNotActivated;

		EnterCriticalSection(&m_csFunctionModule);
		if (m_pFunctionModule == nullptr)
		{
			LeaveCriticalSection(&m_csFunctionModule);
			return ModuleNotActivated;
		}
		LeaveCriticalSection(&m_csFunctionModule);

		EnterCriticalSection(&m_csDiagnostic);
		if (m_pDiagnostic == nullptr)
		{
			LeaveCriticalSection(&m_csDiagnostic);
			return ModuleNotActivated;
		}

		DiagnosticErrorCode code = m_pDiagnostic->getDiagnosticInfo(address, field, info);
		LeaveCriticalSection(&m_csDiagnostic);

		return code == decNoError ? NoError : DiagnosticError;
	}
	ErrorCode getDiagnosticStatisticSize(_Out_ uint32_t& size);
	ErrorCode getDiagnosticStatisticTime(_Outref_result_maybenull_ const DATE*& pDate);
	ErrorCode getDiagnosticStatisticData(uint8_t address, DiagnosticParameter field, _Outref_result_maybenull_ const float*& pData);
	ErrorCode restartDiagnostic();
#pragma endregion Методы по работе с диагностической информацией
//----------------------------------------------------------------------------------------------------------------------
#pragma region Reading registers methods
	ErrorCode addRegister4Reading(ModbusAddress address, RegisterValue registerValue);
	ErrorCode clearRegisters4Reading();
	template <typename T>
	ErrorCode getRegisterValue(DeviceRegisterValue deviceRegister, _Out_ T& value)
	{
		value = T();

		if (m_fmsState != fmsStarted)
			return ModuleNotActivated;

		EnterCriticalSection(&m_csFunctionModule);
		if (m_pFunctionModule == nullptr)
		{
			LeaveCriticalSection(&m_csFunctionModule);
			return ModuleNotActivated;
		}
		LeaveCriticalSection(&m_csFunctionModule);

		if (m_pRegistersStorage == nullptr)
			return ModuleNotActivated;

		RegistersStorageErrorCode code = m_pRegistersStorage->getRegisterValue(deviceRegister, value);

		return rsecNoError == code ? NoError : ModuleNotActivated;
	}
	ErrorCode clearRegisterStorage();
#pragma endregion Методы по работе с чтением регистров
//----------------------------------------------------------------------------------------------------------------------
#pragma endregion Методы
//----------------------------------------------------------------------------------------------------------------------
protected:
	typedef std::map<Device, CModbusSlave*> Slaves;
	typedef Slaves::reference Slave;

	template<class _Fn1>
	inline _Fn1 for_each_slave(_Fn1 _Func)
	{
		return std::for_each(m_slaves.begin(), m_slaves.end(), _Func);
	}
	template<class _Fn1>
	inline Slaves::iterator find_if_slave(_Fn1 _Func)
	{
		return std::find_if(m_slaves.begin(), m_slaves.end(), _Func);
	}
	inline Slaves::iterator find_if_slave(ModbusAddress address)
	{
		return find_if_slave([&](Slave slave)
		{
			bool bFound(false);
			ModbusAddress* pAddresses(nullptr);
			uint32_t count = slave.second->getAddresses(pAddresses);
			if (nullptr != pAddresses && count > 0)
			{
				if (std::find(pAddresses, pAddresses + count, address) != pAddresses + count)
					bFound = true;

				delete[] pAddresses;
				pAddresses = nullptr;
			}
			return bFound;
		});
	}

	virtual void initialize() = 0;

	virtual void setConnectState(MasterConnectState mcsState);

	virtual void updateDataExchangeParametersFromRestore(){};
	
	void updateInfoByFunctionModule(_In_ CFunctionModule* pFunctionModule, _In_ const bool bInterrupt);
	void updateInfoByFunctionModule(_In_ CFunctionModule* pFunctionModule);
	virtual void updateInfoBySlaveFinder(_In_ CFunctionModule* pFunctionModule);
	
	void ClearSlaves();
	void getEventsFromFunctionModule(_In_ CFunctionModule* pFunctionModule);

	void functionModuleProcess();
	CFunctionModule* getRequestPackage(_Out_ MODBUS_PACKAGE_STRUCT& mpsPackage);
	void setResponsePackage(_In_ CFunctionModule* pFunctionModule, _Out_ MODBUS_PACKAGE_STRUCT &mpsPackage);

	bool isSettingsEnable(STRUCT_TYPE type);
	bool isPingAvailable();

	void saveConfiguration(_In_ const std::wstring sPath) override;
	void saveAddressTable(_In_ const std::wstring sPath, _In_ const bool bMetrology) override;
	
	MasterConnectState m_mcsState;
	uint32_t m_pingTimeout;

	FunctionModuleState m_fmsState;

	Slaves m_slaves;
	CModbusSlave* m_pSlavePtr;

	CFunctionModule* m_pFunctionModule;
	CPinger* m_pPinger;
	CModbusPackageManager* m_pModbusPackageManager;
	CHiddenProtocolPackageManager* m_pHiddenProtocolPackageManager;
	CDiagnostic* m_pDiagnostic;
	CRegistersStorage* m_pRegistersStorage;

	CRITICAL_SECTION m_csSLaves;

	bool m_bRestartPowerSubscribed;
	std::function<void()> m_restartPowerFunc;
	bool m_bChangeMasterConnectStateSubscribed;
	std::function<void(Device, MasterConnectState)> m_changeMasterConnectStateFunc;
	bool m_bChangeSlaveCountSubscribed;
	std::function<void(Device)> m_changeSlaveCountFunc;

	Description m_description;
	Configuration m_configuration4Restore;

	bool m_bFindSlaveExEnable;

private:
	void onRestartPower();

	CRITICAL_SECTION m_csFunctionModule;
	CRITICAL_SECTION m_csDiagnostic;

	FindMode m_fmFindMode;
};
