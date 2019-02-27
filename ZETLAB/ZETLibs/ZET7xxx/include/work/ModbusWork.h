#pragma once
#include "base\ModbusBase.h"
#include "work\MasterWork.h"
#include "scanner\scanner.h"

#include <boost\noncopyable.hpp>

#include <map>
#include <functional>
#include <mutex>

///	����� ��� ������ � ������������ �� ��������� MODBUS ����� ��������� RS-485 � CAN.
/**
*	����� ������������� ��������� ����������� �� ������ � ������������:
*		- �����, ����������� � ���������� ���� ����� ��������
*		- ����� �������, ������������ � ��������
*		- ���������� ����������� ������������ ����������� ���������
*		- ������ ����������� ���� ������ ���������
*		- ������������� ��������� ������������� ����� RS-485
*		- ��������� ������� ������� ��������� � ��.
*/
class CModbusWork : public CModbusBase, private boost::noncopyable
{
public:
#pragma region Constructors and destructor
	///	����������� �� ���������
	CModbusWork();
	///	����������
	~CModbusWork();
#pragma endregion ������������ � ����������

#pragma region Common system methods
	/** ����������� � ������ ������ � ������������
	*	@returns ��� ������
	*	@sa modbus::ErrorCodes
	*/
	_Check_return_
	ErrorCode connect();
	/** ���������� �� ������ ������ � ������������
	*	@returns ��� ������
	*	@sa modbus::ErrorCodes
	*/
	ErrorCode disconnect();
	/** ������ ������ � ���������
	*	@param type ��� ��������
	*	@returns ��� ������
	*	@sa ZET7XXX_DEVICE_TYPE
	*	@sa modbus::ErrorCodes
	*/
	ErrorCode connectMasters(ZET7XXX_DEVICE_TYPE type);
	/** ��������� ������ � ���������
	*	@param type ��� ��������
	*	@returns ��� ������
	*	@sa ZET7XXX_DEVICE_TYPE
	*	@sa modbus::ErrorCodes
	*/
	ErrorCode disconnectMasters(ZET7XXX_DEVICE_TYPE type);

	/** ������ ���������� ��������
	*	@param count ���������� �������� � �������
	*	@returns ��� ������
	*	@sa modbus::ErrorCodes
	*/
	ErrorCode getMasterCount(_Out_ uint32_t& count);
	/** ������ ������ ��������
	*	@param pMasters ������ �������� � �������
	*	@param count ���������� �������� � �������
	*	@returns ��� ������
	*	@sa modbus::PDevice
	*	@sa modbus::ErrorCodes
	*/
	ErrorCode getMasters(_Outref_result_maybenull_ PDevice& pMasters, _Out_ uint32_t& count);
	/** ������ ���������� ���������
	*	@param count ���������� ��������� � �������
	*	@returns ��� ������
	*	@sa modbus::ErrorCodes
	*/
	ErrorCode getDeviceCount(_Out_ uint32_t& count);
	/** ������ ������ ���������
	*	@param pDevices ������ ��������� � �������
	*	@param count ���������� ��������� � �������
	*	@returns ��� ������
	*	@sa modbus::PDevice
	*	@sa modbus::ErrorCodes
	*/
	ErrorCode getDevices(_Outref_result_maybenull_ PDevice& pDevices, _Out_ uint32_t& count);
	/** ������ ���������� �������
	*	@param count ���������� ������� � �������
	*	@returns ��� ������
	*	@sa modbus::ErrorCodes
	*/
	ErrorCode getChannelCount(_Out_ uint32_t& count);

	_Check_return_
	ErrorCode getEventLog(_Out_ LogEventList& eventLog);

	ErrorCode getFunctionModuleProgress(_Out_ uint32_t& progress);

	ErrorCode subscribeToRestartPower(const std::function<void()>& func);
	ErrorCode unsubscribeFromRestartPower();
	ErrorCode subscribeToChangeMasterConnectState(const std::function<void(Device, MasterConnectState)>& func);
	ErrorCode unsubscribeFromChangeMasterConnectState();
	ErrorCode subscribeToChangeSlaveCount(const std::function<void(Device)>& func);
	ErrorCode unsubscribeFromChangeSlaveCount();

	ErrorCode setOption(_In_ const Option option, _In_ const bool bValue);
#pragma endregion ����� ��������� ������

#pragma region Masters methods
	/// ������� ������ ������� �������.
	/**	����� ������� �������������� �� ������� �������� ������� �� ������� � 1 �� 63.
	*
	*	@param master ������������� ���������� �������
	*	@returns ��� ������
	*	@sa modbus::Device
	*	@sa modbus::ErrorCodes
	*/
	ErrorCode findSlaves(_In_ const Device& master);
	/// ������� ������������ ������ ������� �������.
	/**	����� ������� �������������� �� ���� ��������� ��������� ������� �� ������� � 1 �� 63.
	*
	*	@param master ������������� ���������� �������
	*	@returns ��� ������
	*	@sa modbus::Device
	*	@sa modbus::ErrorCodes
	*/
	ErrorCode findSlavesEx(_In_ const Device& master);
	/// ������� ������������� ������� �������.
	/**	������������� ��������� �������� ������ ��� ���������� RS485 � ����������� �� ��������� ��������. �������������	
	*	������������ ����� ������� ������� � ������������ �� ��� ����������� "�������� ���������", � ���� �������� 
	*	������ �������� ����������������� �������, � ����� �� ������� ������ �������� ���� �������� ����� � ����� �
	*	������������� �����. ����� ����� �������� ������������� ������������ ������������� ������� � ������� �
	*	����������� ��� ���������� ��������. 
	*
	*	@param master ������������� ���������� �������
	*	@returns ��� ������
	*	@sa modbus::Device
	*	@sa modbus::ErrorCodes
	*/
	ErrorCode initSlaves(_In_ const Device& master);
	/// ������� �� ������ ����������� ������������� �����
	/**	����������� ������������� ����� ������������ ����� ����������� ������� MODBUS-�������� ����������� � �������� ��
	*	1 �� 63 � ����� �� ��������� ��������� ����� ������ �������. ��� ����������� �������������, ������� ���� �������
	*	��������, ������� ���� � ���������, ������� ���� ������ ����������� ����� � �.�.
	*
	*	@param master ������������� ���������� �������
	*	@returns ��� ������
	*	@sa modbus::Device
	*	@sa modbus::ErrorCodes
	*/
	ErrorCode startDataExchangeDiagnostic(_In_ const Device& master);
	/// ������� �� ������ ����������� �������������
	/**	����������� ������������� ������������ ����� ����������� ������� MODBUS-�������� ������������ ����������� � 
	*	����� ��������� ���������� � ������������� ���������. ��� ����������� ������������� ��������� �������������:
	*	���������� �� ������� ������������� ��� PTP-������������� � CAN-����, � ����� ��������� ���.
	*
	*	@param master ������������� ���������� �������
	*	@returns ��� ������
	*	@sa modbus::Device
	*	@sa modbus::ErrorCodes
	*/
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
	ErrorCode restartMasterByUdp(_In_ const Device& master);
	ErrorCode testMaster(_In_ const Device& master);

	/// ������� �� ����� �������� ������ �������.
	/**	����� �������� �������������� �������� ��� ���������� �� ���������� ������
	*
	*	@param master ������������� ���������� �������
	*	@param baudRate ����� �������� ������
	*	@returns ��� ������
	*	@sa modbus::Device
	*	@sa modbus::ErrorCodes
	*	@sa recoverMasterBaudRate()
	*/
	ErrorCode changeMasterBaudRate(_In_ const Device& master, uint32_t baudRate);
	/// ������� �� �������������� �������� ������ �������.
	/**	����������������� �������� ��������, ���������� �� ���������� ������
	*
	*	@param master ������������� ���������� �������
	*	@returns ��� ������
	*	@sa modbus::Device
	*	@sa modbus::ErrorCodes
	*	@sa changeMasterBaudRate()
	*/
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
		std::lock_guard<std::recursive_mutex> lg(m_mutex);

		info = T(0);
		if (!IsConnected())
			return ModbusNotConnected;

		auto itMasterWork = m_masterWorks.find(master.first);
		if (itMasterWork == m_masterWorks.end())
			return DeviceNotFound;

		return itMasterWork->second->getDiagnosticInfo(master, field, info);
	}
	template <typename T>
	ErrorCode getDiagnosticInfo(_In_ const Device& master, uint8_t address, DiagnosticParameter field, _Out_ T& info)
	{
		std::lock_guard<std::recursive_mutex> lg(m_mutex);

		info = T(0);
		if (!IsConnected())
			return ModbusNotConnected;

		auto itMasterWork = m_masterWorks.find(master.first);
		if (itMasterWork == m_masterWorks.end())
			return DeviceNotFound;

		return itMasterWork->second->getDiagnosticInfo(master, address, field, info);
	}
	ErrorCode getDiagnosticStatisticSize(_In_ const Device& master, _Out_ uint32_t& size);
	ErrorCode getDiagnosticStatisticTime(_In_ const Device& master, _Outref_result_maybenull_ const DATE*& date);
	ErrorCode getDiagnosticStatisticData(_In_ const Device& master, uint8_t address, DiagnosticParameter field,
		_Outref_result_maybenull_ const float*& pData);
	ErrorCode restartDiagnostic(_In_ const Device& master);
#pragma endregion ������ �� ������ � ��������������� �����������

#pragma region Reading registers methods
	ErrorCode addRegister4Reading(_In_ const Device& master, ModbusAddress address, RegisterValue registerValue);
	ErrorCode clearRegisters4Reading(_In_ const Device& master);
	template <typename T>
	ErrorCode getRegisterValue(_In_ const Device& master, DeviceRegisterValue deviceRegister, _Out_ T& value)
	{
		std::lock_guard<std::recursive_mutex> lg(m_mutex);

		value = T();
		if (!IsConnected())
			return ModbusNotConnected;

		auto itMasterWork = m_masterWorks.find(master.first);
		if (itMasterWork == m_masterWorks.end())
			return DeviceNotFound;

		return itMasterWork->second->getRegisterValue(master, deviceRegister, value);
	}
	ErrorCode clearRegisterStorage(_In_ const Device& master);
#pragma endregion ������ �� ������ � ������� ���������

#pragma endregion ������ ��� ��������
	
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
#pragma endregion ������ ��� ���� ��������� �������

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
	ErrorCode setDeviceConfiguration4Restore(_In_ const Device& dDevice,
		_In_ const ModbusAddress address, _In_ Configuration& configuration);
#pragma endregion ������ ��� ������ ���������� ���������

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
#pragma endregion ������ ��� ������ ���������� �������
		
#pragma region Channel parameters methods
	ErrorCode getDeviceChannelInfo(_In_ const Device& slave, _Out_ ChannelInfo& info);
#pragma endregion ������ ��� ������ ���������� �������

protected:
	typedef std::map<ZET7XXX_DEVICE_TYPE, CMasterWork*> MasterWorks;
	typedef MasterWorks::reference MasterWork;

	template<class _Fn1> 
	inline _Fn1 for_each_master_work(_Fn1 _Func)
	{	
		return std::for_each(m_masterWorks.begin(), m_masterWorks.end(), _Func);
	}
	template<class _Fn1>
	inline MasterWorks::iterator find_if_master_work(_Fn1 _Func)
	{
		return std::find_if(m_masterWorks.begin(), m_masterWorks.end(), _Func);
	}

	bool m_bConnect;
	inline bool IsConnected(){ return m_bConnect; }

	MasterWorks m_masterWorks;
	
	bool m_bRestartPowerSubscribed;
	std::function<void()> m_restartPowerFunc;
	bool m_bChangeMasterConnectStateSubscribed;
	std::function<void(Device, MasterConnectState)> m_changeMasterConnectStateFunc;
	bool m_bChangeSlaveCountSubscribed;
	std::function<void(Device)> m_changeSlaveCountFunc;

private:
	void onRestartPower();
	void onChangeMasterConnectState(Device device, MasterConnectState mcsState);
	void onChangeSlaveCount(Device device);
	
	template<class _Fn1>
	inline _Fn1 for_each_scanner(_Fn1 _Func)
	{
		return std::for_each(m_scanners.begin(), m_scanners.end(), _Func);
	}
	std::list<CScanner*> m_scanners;

	uint32_t m_minTimePeriod;	

	std::recursive_mutex m_mutex;
};