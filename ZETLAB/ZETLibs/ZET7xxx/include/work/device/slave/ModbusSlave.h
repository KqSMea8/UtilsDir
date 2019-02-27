#pragma once
#include "device\ModbusDevice.h"
#include "device\channel\ModbusChannel.h"
#include "..\..\base\ErrorCodes.h"

#include <boost\noncopyable.hpp>

#include <algorithm>
#include <memory>

using namespace zet7xxx::errors;

class CModbusSlave : public CModbusDevice, private boost::noncopyable
{
	friend class CCANMaster;
	friend class CSlaveFinderResult;
	friend class CConfigurationWriter;
	friend class CDescriptionReaderResult;
	
public:
	CModbusSlave(_In_ const Device& device);
	~CModbusSlave();

	uint32_t getChannelCount();
	uint32_t getChannels(_Outref_ std::wstring*& pChannels);
	uint32_t getChannels(_Outref_ PDataChannel& pChannels);
	uint32_t getFrequencies(_Outref_ float*& pFrequencies);
	uint32_t getAddresses(_Outref_ ModbusAddress*& pAddresses);
	ModbusAddress getAddress();
	uint32_t getDataAddresses(_Outref_ ModbusDataAddress*& pDataAddresses);
	ModbusDataAddress getDataAddress();
	ModbusDataAddress getDataAddress(_In_ const ModbusAddress address);
	ModbusDataAddress getNextDataAddress(_In_ const ModbusDataAddress address);
	
	ErrorCode getChannelInfo(_In_ const DataChannel& channel, _Out_ ChannelInfo& info);
	ErrorCode getChannelInfo(_Out_ ChannelInfo& info);

	ErrorCode getChannelsInfo(_Outref_ ChannelInfo*& pChannelsInfo);

	void saveConfiguration(_In_ const std::wstring sPath) override;
	void saveAddressTable(_In_ const std::wstring sPath, _In_ const bool bMetrology) override;
	void getConfiguration(_In_ const ModbusAddress address, _Out_ Configuration& configuration);
	void getConfiguration4Restore(_In_ const ModbusAddress address, _Out_ Configuration& configuration);
	void setConfiguration4Restore(_In_ const ModbusAddress address, _In_ const Configuration& configuration);

protected:
	typedef std::map<DataChannel, std::shared_ptr<CModbusChannel>> Channels;
	typedef Channels::reference Channel;

	Channels m_channels;
	
	template<class _Fn1>
	inline _Fn1 for_each_channel(_Fn1 _Func)
	{
		return std::for_each(m_channels.begin(), m_channels.end(), _Func);
	}
	template<class _Fn1>
	inline Channels::iterator find_if_channel(_Fn1 _Func)
	{
		return std::find_if(m_channels.begin(), m_channels.end(), _Func);
	}
	inline Channels::iterator find_if_channel(_In_ const ModbusAddress address)
	{
		return find_if_channel([&](Channel channel)
		{
			return address == channel.second->getAddress();
		});
	}
	inline Channels::iterator find_if_channel(_In_ const DataChannel chan)
	{
		return find_if_channel([&](Channel channel)
		{
			return chan.first == channel.second->getAddress() &&
				   chan.second == channel.second->getDataAddress();
		});
	}

private:
	void init(_In_ const VERSION_STRUCT& version, _In_ const std::vector<Configuration>& configurations);
	void update(_In_ const VERSION_STRUCT& version, _In_ const std::vector<Configuration>& configurations);

	CRITICAL_SECTION m_csChannels;
};

