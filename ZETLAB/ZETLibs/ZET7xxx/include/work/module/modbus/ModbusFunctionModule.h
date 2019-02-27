#pragma once
#include "module\FunctionModule.h"
#include "device\slave\ModbusSlave.h"

#include <ZETSimple\include\time\timer\difftimer\DiffTimer2.h>

#include <list>
#include <tuple>

class CModbusFunctionModuleResult;

class CModbusFunctionModule : public CFunctionModule
{
public:
	CModbusFunctionModule();
	virtual ~CModbusFunctionModule();

	virtual int32_t getRequestPackage(_Out_ MODBUS_PACKAGE_STRUCT& package);
	
	virtual int32_t getProgress() override;
	
	virtual void getResult(_Outref_result_maybenull_ CModbusFunctionModuleResult*& pResult);

	bool isAddressEnable(_In_ const ModbusAddress address);
	bool isAddressActive(_In_ const ModbusAddress address);
	virtual void setAddressActive(_In_ const ModbusAddress address, bool bActive);

	virtual void updateSlave(_In_ const CModbusSlave* pSlave){};
	virtual void removeDevice(_In_ const Device& device){};

	void setDelay(uint32_t delay);
	uint32_t getDelay();

protected:
	virtual void restart() override;
	virtual void prepareSlave() = 0;
	virtual void AddInfoToLogBySlave() = 0;
	void goToNextDevice();
	void goToFirstDevice();
	void repeat();
	
	typedef std::tuple<ModbusAddress, bool, CModbusSlave*> ActiveDevice;
	typedef std::list<ActiveDevice> ActiveDevices;
	
	template<class _Fn1>
	inline _Fn1 for_each_device(_Fn1 _Func)
	{
		return std::for_each(m_devices.begin(), m_devices.end(), _Func);
	}
	template<class _Fn1>
	inline ActiveDevices::iterator find_if_device(_Fn1 _Func)
	{
		return std::find_if(m_devices.begin(), m_devices.end(), _Func);
	}
	inline ActiveDevices::iterator find_if_device(ModbusAddress address)
	{
		return find_if_device([&](ActiveDevice& device)
		{
			return std::get<0>(device) == address;
		});
	}

	ActiveDevices m_devices;

	ModbusAddress m_currentDeviceAddress;
	ModbusCommand m_currentCommand;
	ModbusDataAddress m_currentRegisterAddress;
	uint16_t m_currentRequestSize;
	
	uint32_t m_repeatCounter;
	uint32_t m_delay;

	CDiffTimer2 m_timer;
};
