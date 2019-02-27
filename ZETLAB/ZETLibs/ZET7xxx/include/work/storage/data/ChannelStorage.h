#pragma once

#ifndef channel_storage_h_
#define channel_storage_h_

#include "base\ErrorCodes.h"
#include "base\Types.h"

#include <ZETSimple\include\trends\writer\stream\StreamTrendWriter.h>

#include <boost\date_time\posix_time\posix_time.hpp>
#include <boost\noncopyable.hpp>

using namespace zet7xxx;

class CChannelStorage : private boost::noncopyable
{
public:
	CChannelStorage(_In_ const ChannelInfo& info);
	~CChannelStorage();

	void putData(_In_ const float* pData, _In_ const uint32_t size);
	uint32_t store(_In_ const bool bCompressed);
	void lastStore();

protected:
	int32_t getData(_In_ const ptime& tTime, _In_ float* pData, _In_ const uint32_t size);
	ptime getTime();
	void synchro();
	
	ChannelInfo m_info;
		
private:
	bool m_bInit;

	std::shared_ptr<CStreamTrendWriter> m_pWriter;

	ptime m_startTime;
	uint64_t m_counter;
	
	int32_t m_writePosition;
	uint32_t m_bufferSize;
	std::shared_ptr<float> m_pBuffer;
	
	ptime m_writeTime, m_writeTimePrev;
	uint32_t m_writeBufferSize;
	std::shared_ptr<float> m_pWriteBuffer;
	time_duration m_delta;

	CRITICAL_SECTION m_csData;
};

#endif //channel_storage_h_
