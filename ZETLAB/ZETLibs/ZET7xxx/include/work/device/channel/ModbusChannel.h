#pragma once
#include <ZET7xxx\include\base\ZET7xxxTypes.h>

using namespace zet7xxx;
using namespace modbus;

#pragma warning( disable : 4290 )

class CModbusChannel
{
	friend class CModbusSlave;
	friend class CDescriptionReaderResult;

public:
	CModbusChannel(_In_ const DataChannel channel);
	CModbusChannel(_In_ const CModbusChannel& channel);
	~CModbusChannel();
	
	void operator = (_In_ const CModbusChannel& channel);

	std::wstring getName() const;
	ModbusAddress getAddress() const;
	ModbusDataAddress getDataAddress() const;
	ModbusDataType getDataType() const;
	Configuration getConfiguration() const;
	Description getDescription() const;
	void setConfiguration4Restore(_In_ const Configuration& configuration);
	Configuration getConfiguration4Restore() const;
	float getFrequency() const;
	bool isMessage() const;

	void getInfo(ChannelInfo& info) const;
	void markAsMessage();

protected:
	DataChannel m_channel;
	ChannelInfo m_info;

	Configuration m_configuration;
	Description m_description;
	Configuration m_configuration4Restore;
	
private:
	void init(_In_ const Configuration& configuration) throw (std::exception);
};

