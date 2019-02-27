#pragma once
#include <zet76\zet76.h>

#include <stdint.h>

class CZet76Work
{
public:
	CZet76Work();
	~CZet76Work();
	
	int32_t zet76TcpWrite(_In_ zet76_server* server, _In_ const zet76_device* device,
		uint16_t size, _In_ uint8_t* buffer);
	int32_t zet76TcpReadResponse(_In_ zet76_server* server, _In_ const zet76_device* device,
		_Inout_ uint16_t& size, _Inout_opt_ uint8_t* buffer);
	int32_t zet76TcpReadChain(_In_ zet76_server* server, _In_ const zet76_device* device,
		_Inout_ uint16_t& size, _Inout_opt_ uint8_t* buffer);

	int32_t zet76RtuWrite(_In_ zet76_server* server, _In_ const zet76_device* device,
		uint16_t size, _In_ uint8_t* buffer);
	int32_t zet76RtuRead(_In_ zet76_server* server, _In_ const zet76_device* device,
		_Inout_ uint16_t& size, _Inout_ uint8_t* buffer);

protected:
	uint16_t m_transaction;

	uint8_t* m_txBuffer;
	uint8_t* m_rxBuffer;
};

