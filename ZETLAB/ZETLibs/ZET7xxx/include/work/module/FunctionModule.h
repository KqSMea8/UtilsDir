#pragma once
#include "..\base\ModbusBase.h"

#include <boost\noncopyable.hpp>

#include <functional>

class CFunctionModule : public CModbusBase, private boost::noncopyable
{
public:
	CFunctionModule();
	virtual ~CFunctionModule();

	virtual void restart() = 0;

	virtual int32_t getRequestPackage(_Out_ MODBUS_PACKAGE_STRUCT& package);
	virtual void setResponsePackage(MODBUS_PACKAGE_STRUCT& package) = 0;

	FunctionModuleType getType();
	std::wstring getEndStatusString();

	virtual int32_t getProgress() = 0;
	virtual int32_t getProgress(Device device);

	void subscribeToRestartPower(const std::function<void()>& func);

	enum : int32_t
	{
		NO_REQUEST_PACKAGE = -1,
		PACKAGE_FOR_REQUEST,
	};

protected:
	void transformDataBuffer(_In_ uint8_t* pBuffer, _In_ uint32_t nSize, _Out_ std::vector<uint16_t>& data);
	void transformDataBuffer(_In_ uint16_t* pBuffer, _In_ uint32_t nSize, _Out_ std::vector<uint8_t>& data);

	uint16_t m_currentDataSize;
	uint8_t m_buffer[g_maxDataSize];

	FunctionModuleType m_type;
	std::wstring m_sEndStatusString;
	uint32_t m_maxWaitTimeout;
	uint8_t m_waitAnswerUntilTimeout;

	bool m_bRestartPowerSubscribed;
	std::function<void()> m_restartPowerFunc;
};