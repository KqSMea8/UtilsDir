#pragma once
#include "base\ModbusBase.h"
#include "device\master\ModbusMaster.h"

#include <boost\noncopyable.hpp>

#include <map>
#include <functional>
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
///	Базовый класс для работы с конкретным типом устройств по протоколу MODBUS через интерфейс RS-485 и CAN.
/**
*	Класс предоставляет следующие возможности по работе с устройствами:
*		- поиск, подключение и отключение мастеров
*		- поиск слейвов, подключенных к мастеру
*		- обновление внутреннего программного обеспечения устройств
*		- чтение внутреннего лога ошибок устройств
*		- инициализация устройств измерительной линии RS-485
*		- генерация таблицы адресов устройств и др.
*/
class CMasterWork : public CModbusBase, private boost::noncopyable
{
	friend class CEthernetScanner;
	friend class CUsbScanner;
//----------------------------------------------------------------------------------------------------------------------
public:
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
#pragma region Constructors and destructor
	///	Конструктор по умолчанию
	CMasterWork(ZET7XXX_DEVICE_TYPE zdtType);
	///	Деструктор
	virtual ~CMasterWork();
#pragma endregion Конструкторы и деструктор
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
#pragma region Common system methods
	virtual void start();
	void stop();
	uint32_t getMasterCount();
	uint32_t getDeviceCount();
	uint32_t getChannelCount();
	ErrorCode getMasters(_In_ PDevice pMasters, _Out_ uint32_t& count);
	ErrorCode getDevices(_In_ PDevice pDevices, _Out_ uint32_t& count);
	void getEvents(_Out_ LogEventList& eventLog) override;
	ErrorCode getFunctionModuleProgress(_Out_ int32_t& progress);
	ErrorCode subscribeToRestartPower(const std::function<void()>& func);
	ErrorCode subscribeToChangeMasterConnectState(const std::function<void(Device, MasterConnectState)>& func);
	ErrorCode subscribeToChangeSlaveCount(const std::function<void(Device)>& func);
#pragma endregion Общие системные методы
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
#pragma region Masters methods
	ErrorCode findSlaves(_In_ const Device& master);
	ErrorCode findSlavesEx(_In_ const Device& master);
	ErrorCode initSlaves(_In_ const Device& master);
	ErrorCode startDataExchangeDiagnostic(_In_ const Device& master);
	ErrorCode startSynchronizationDiagnostic(_In_ const Device& master);
	ErrorCode startNetSynchronizationDiagnostic(_In_ const Device& master);
	ErrorCode startDataQualityDiagnostic(_In_ const Device& master);
	ErrorCode startReadingRegisters(_In_ const Device& master);
	ErrorCode startReadingData(_In_ const Device& master);
	ErrorCode getSlaves(_In_ const Device& master, _Outref_result_maybenull_ PDevice& pSlaves, _Out_ uint32_t& count);
	ErrorCode getDevices(_In_ const Device& master, _Outref_result_maybenull_ PDevice& pDevices, _Out_ uint32_t& count);
	ErrorCode getFunctionModuleType(_In_ const Device& master, _Out_ FunctionModuleType& type);
	ErrorCode stopFunctionModule(_In_ const Device& master, _In_ bool bUpdateInfo);
	ErrorCode connectMaster(_In_ const Device& master);
	ErrorCode disconnectMaster(_In_ const Device& master);
	virtual ErrorCode restartMasterByUdp(_In_ const Device& master){ return NoError; }
	ErrorCode testMaster(_In_ const Device& master);
	ErrorCode changeMasterBaudRate(_In_ const Device& master, uint32_t baudRate);
	ErrorCode recoverMasterBaudRate(_In_ const Device& master);
	ErrorCode changeMasterParity(_In_ const Device& master, uint8_t parity);
	ErrorCode recoverMasterParity(_In_ const Device& master);
	ErrorCode getProcessDelay(_In_ const Device& master, _Out_ uint32_t& delay);
	ErrorCode setProcessDelay(_In_ const Device& master, uint32_t delay);
	ErrorCode getIp(_In_ const Device& master, _Out_ std::wstring& sIp);
	ErrorCode getMasterConnect(_In_ const Device& master, _Out_ bool& bConnect);
	ErrorCode getMasterFindSlaveExEnable(_In_ const Device& master, _Out_ bool& bFindSlaveExEnable);
	ErrorCode getMasterFunctionModuleState(_In_ const Device& master, _Out_ FunctionModuleState& state);
//----------------------------------------------------------------------------------------------------------------------
#pragma region Diagnostic methods
	ErrorCode getDiagnosticAddressEnable(_In_ const Device& master, uint8_t address, _Out_ bool& bEnable);
	ErrorCode getDiagnosticAddressActive(_In_ const Device& master, uint8_t address, _Out_ bool& bActive);
	ErrorCode setDiagnosticAddressActive(_In_ const Device& master, uint8_t address, bool bActive);
	ErrorCode getDiagnosticCommandActive(_In_ const Device& master, ModbusCommand command, _Out_ bool& bActive);
	ErrorCode setDiagnosticCommandActive(_In_ const Device& master, ModbusCommand command, bool bActive);
	ErrorCode getDiagnosticAddressParameters(_In_ const Device& master,
		_Outref_result_maybenull_ DiagnosticParameter*& pParameters, _Out_ uint32_t& count);
	template <typename T>
	ErrorCode getDiagnosticInfo(_In_ const Device& master, DiagnosticParameter field, _Out_ T& info)
	{
		info = T(0);

		if (m_type != master.first)
			return DeviceNotFound;

		EnterCriticalSection(&m_csMasters);

		auto itMaster = m_masters.find(master);
		if (itMaster == m_masters.end())
		{
			LeaveCriticalSection(&m_csMasters);
			return DeviceNotFound;
		}
		ErrorCode errorCode = itMaster->second->getDiagnosticInfo(field, info);

		LeaveCriticalSection(&m_csMasters);

		return errorCode;
	}
	template <typename T>
	ErrorCode getDiagnosticInfo(_In_ const Device& master, uint8_t address, DiagnosticParameter field, _Out_ T& info)
	{
		info = T(0);

		if (m_type != master.first)
			return DeviceNotFound;

		EnterCriticalSection(&m_csMasters);

		auto itMaster = m_masters.find(master);
		if (itMaster == m_masters.end())
		{
			LeaveCriticalSection(&m_csMasters);
			return DeviceNotFound;
		}
		ErrorCode errorCode = itMaster->second->getDiagnosticInfo(address, field, info);

		LeaveCriticalSection(&m_csMasters);

		return errorCode;
	}
	ErrorCode getDiagnosticStatisticSize(_In_ const Device& master, _Out_ uint32_t& size);
	ErrorCode getDiagnosticStatisticTime(_In_ const Device& master, _Outref_result_maybenull_ const DATE*& date);
	ErrorCode getDiagnosticStatisticData(_In_ const Device& master, uint8_t address, DiagnosticParameter field,
		_Outref_result_maybenull_ const float*& pData);
	ErrorCode restartDiagnostic(_In_ const Device& master);
#pragma endregion Методы по работе с диагностической информацией
//----------------------------------------------------------------------------------------------------------------------
#pragma region Reading registers methods
	ErrorCode addRegister4Reading(_In_ const Device& master, ModbusAddress address, RegisterValue registerValue);
	ErrorCode clearRegisters4Reading(_In_ const Device& master);
	template <typename T>
	ErrorCode getRegisterValue(_In_ const Device& master, DeviceRegisterValue deviceRegister, _Out_ T& value)
	{
		value = T();

		if (m_type != master.first)
			return DeviceNotFound;

		EnterCriticalSection(&m_csMasters);

		auto itMaster = m_masters.find(master);
		if (itMaster == m_masters.end())
		{
			LeaveCriticalSection(&m_csMasters);
			return DeviceNotFound;
		}
		ErrorCode errorCode = itMaster->second->getRegisterValue(deviceRegister, value);

		LeaveCriticalSection(&m_csMasters);

		return errorCode;

	}
	ErrorCode clearRegisterStorage(_In_ const Device& master);
#pragma endregion Методы по работе с чтением регистров
//----------------------------------------------------------------------------------------------------------------------
#pragma endregion Методы для мастеров
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
#pragma region All devices methods
	ErrorCode readErrors(_In_ const PDevice& pDevices, uint32_t count);
	ErrorCode updateFirmware(_In_ const PDevice& pDevices, uint32_t count, _In_ wchar_t* sPath);
	ErrorCode readFirmware(_In_ const PDevice& pDevices, uint32_t count);
	ErrorCode selectDevices(_In_ const PDevice& pDevices, uint32_t count, bool bSelect);
	ErrorCode restartDevices(_In_ const PDevice& pDevices, uint32_t count);
	ErrorCode updateConfiguration(_In_ const PDevice& pDevices, uint32_t count);
	ErrorCode readDescriptors(_In_ const PDevice& pDevices, uint32_t count);
	ErrorCode saveConfiguration(_In_ const PDevice& pDevices, uint32_t count, std::wstring sPath);
	ErrorCode saveAddressTable(_In_ const PDevice& pDevices, uint32_t count, std::wstring sPath, bool bMetrology);
	ErrorCode saveFirmware(_In_ const PDevice& pDevices, _In_ const uint32_t count, _In_ const std::wstring& sPath);
#pragma endregion Методы для всех устройств системы
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
#pragma region Device parameters methods
	ErrorCode getDeviceId(_In_ const Device& master, uint8_t address, _Out_ Device& device);
	ErrorCode getDeviceMaster(_In_ const Device& device, _Out_ Device& master);
	ErrorCode getDeviceName(_In_ const Device& device, _Out_ std::wstring& sName);
	ErrorCode getDeviceNameWithSerial(_In_ const Device& device, _Out_ std::wstring& sNameWithSerial);
	ErrorCode getDeviceBaudRate(_In_ const Device& device, _Out_ uint32_t& baudRate);
	ErrorCode getDeviceFunctionModuleProgress(_In_ const Device& device, _Out_ uint32_t& progress);
	ErrorCode getDeviceParity(_In_ const Device& device, _Out_ uint8_t& parity);
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
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
#pragma region Slave parameters methods
	ErrorCode getSlaveDataAddresses(_In_ const Device& slave,
		_Outref_result_maybenull_ uint16_t*& pDataAddresses,_Out_ uint32_t& count);
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
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
#pragma region Channel parameters methods
	ErrorCode getDeviceChannelInfo(_In_ const Device& slave, _Out_ ChannelInfo& info);
#pragma endregion Методы для опроса параметров каналов
//----------------------------------------------------------------------------------------------------------------------
protected:
	typedef std::map<Device, CModbusMaster*> Masters;
	typedef Masters::reference Master;

	template<class _Fn1>
	inline _Fn1 for_each_master(_Fn1 _Func)
	{
		return std::for_each(m_masters.begin(), m_masters.end(), _Func);
	}
	template<class _Fn1>
	inline Masters::iterator find_if_master(_Fn1 _Func)
	{
		return std::find_if(m_masters.begin(), m_masters.end(), _Func);
	}

	Masters m_masters;
	ZET7XXX_DEVICE_TYPE m_type;

	CRITICAL_SECTION m_csMasters;

	bool m_bRestartPowerSubscribed;
	std::function<void()> m_restartPowerFunc;
	bool m_bChangeMasterConnectStateSubscribed;
	std::function<void(Device, MasterConnectState)> m_changeMasterConnectStateFunc;
	bool m_bChangeSlaveCountSubscribed;
	std::function<void(Device)> m_changeSlaveCountFunc;
//----------------------------------------------------------------------------------------------------------------------
private:
	void onRestartPower();
	void onChangeMasterConnectState(Device device, MasterConnectState mcsState);
	void onChangeSlaveCount(Device device);
//----------------------------------------------------------------------------------------------------------------------
};

