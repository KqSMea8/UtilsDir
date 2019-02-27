#pragma once
#include "base\ErrorCodes.h"
#include "base\Types.h"
#include <ZETSimple\include\time\TimeConverter.h>
#include "RegisterInfo.h"

#include <codecvt\codecvt_cp1251.hpp>

#include <boost\noncopyable.hpp>

using namespace zet7xxx::errors;

class CRegistersStorage : private boost::noncopyable
{
public:
	CRegistersStorage();
	~CRegistersStorage();

	void addInfo(_In_ CRegisterInfo* pInfo);
	void clear();

	template <typename T>
	RegistersStorageErrorCode getRegisterValue(DeviceRegisterValue deviceRegister, _Out_ T& value)
	{
		value = T(0);

		EnterCriticalSection(&m_csRegisters);

		auto itDeviceRegisters = m_registersInfo.find(deviceRegister.device);
		if (itDeviceRegisters == m_registersInfo.end())
		{
			LeaveCriticalSection(&m_csRegisters);
			return rsecNoDevice;
		}

		auto itRegister = itDeviceRegisters->second.find(deviceRegister.address);
		if (itRegister == itDeviceRegisters->second.end())
		{
			LeaveCriticalSection(&m_csRegisters);
			return rsecNoRegister;
		}

		if (sizeof(value) > itRegister->second.second.size() * 2)
		{
			LeaveCriticalSection(&m_csRegisters);
			return rsecNoRegister;
		}

		T* pValue = reinterpret_cast<T*>(itRegister->second.second.data());
		value = *pValue;

		LeaveCriticalSection(&m_csRegisters);

		return rsecNoError;
	}

	template<>
	RegistersStorageErrorCode getRegisterValue(DeviceRegisterValue deviceRegister, _Out_ std::wstring& value)
	{
		value = _T("");

		EnterCriticalSection(&m_csRegisters);

		auto itDeviceRegisters = m_registersInfo.find(deviceRegister.device);
		if (itDeviceRegisters == m_registersInfo.end())
		{
			LeaveCriticalSection(&m_csRegisters);
			return rsecNoDevice;
		}

		auto itRegister = itDeviceRegisters->second.find(deviceRegister.address);
		if (itRegister == itDeviceRegisters->second.end())
		{
			LeaveCriticalSection(&m_csRegisters);
			return rsecNoRegister;
		}

		char* pValue = reinterpret_cast<char*>(itRegister->second.second.data());
		value = WStringFromCP1251(pValue);

		LeaveCriticalSection(&m_csRegisters);

		return rsecNoError;
	}

	template<>
	RegistersStorageErrorCode getRegisterValue(DeviceRegisterValue deviceRegister, _Out_ FILETIME& value)
	{
		memset(&value, 0, sizeof(FILETIME));

		EnterCriticalSection(&m_csRegisters);

		auto itDeviceRegisters = m_registersInfo.find(deviceRegister.device);
		if (itDeviceRegisters == m_registersInfo.end())
		{
			LeaveCriticalSection(&m_csRegisters);
			return rsecNoDevice;
		}

		auto itRegister = itDeviceRegisters->second.find(deviceRegister.address);
		if (itRegister == itDeviceRegisters->second.end())
		{
			LeaveCriticalSection(&m_csRegisters);
			return rsecNoRegister;
		}

		__time32_t* pValue = reinterpret_cast<__time32_t*>(itRegister->second.second.data());
		if (ToFileTime(&value, *pValue) != TRUE)
		{
			LeaveCriticalSection(&m_csRegisters);
			return rsecNoRegister;
		}

		LeaveCriticalSection(&m_csRegisters);

		return rsecNoError;
	}

protected:
	std::map<ModbusAddress, std::map<uint16_t, std::pair<DeviceRegisterValue, RegisterData>>> m_registersInfo;

private:
	CRITICAL_SECTION m_csRegisters;
};


