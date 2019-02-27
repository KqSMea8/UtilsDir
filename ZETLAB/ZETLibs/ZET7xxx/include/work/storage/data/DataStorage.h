#pragma once

#ifndef data_storage_h_
#define data_storage_h_

#include "ChannelStorage.h"
#include "base\thread_module\thread_module.h"

#include <ZETSimple\include\singleton\singleton.h>

#include <boost\noncopyable.hpp>

#include <memory>
#include <list>

using namespace modbus;

class CDataStorageImpl : protected CThreadModule
{
public:
	CDataStorageImpl();
	~CDataStorageImpl();
	
	void open();
	void close();

	void openChannel(_In_ const Device& device, _In_ const DataChannel& channel, _In_ const ChannelInfo& info);
	void closeChannel(_In_ const Device& device, _In_ const DataChannel& channel);

	void putData(_In_ const Device& device, _In_ const DataChannel& channel, _In_ const StreamData& data);

	bool isChannelExists(_In_ const Device& device, _In_ const DataChannel& channel);
	void initChannel(_In_ const Device& device, _In_ const DataChannel& channel);

protected:
	std::map<std::pair<Device, DataChannel>, std::shared_ptr<CChannelStorage>> m_data;
	std::list<std::pair<std::pair<Device, DataChannel>, uint32_t>> m_channels;

private:
	DWORD threadFunction() override;

	std::vector<std::tuple<Device, DataChannel, ChannelInfo>> m_openCloseChannels;
	CRITICAL_SECTION m_csOpenClose;

	CRITICAL_SECTION m_csData;
};

typedef CSingleton<CDataStorageImpl> CDataStorage;

#endif //data_storage_h_
