#pragma once
#include <boost\date_time\posix_time\posix_time.hpp>

#include <tchar.h>

using namespace boost::posix_time;

typedef std::pair<float, float> CompressionPoint;

class CCompressionInfo
{
public:
	CCompressionInfo(_In_ const time_duration interval);
	~CCompressionInfo();

	time_duration getInterval() const;
	std::wstring getExtention() const;
	CompressionPoint getPoint() const;

	void putData(_In_ float* pData, _In_ const uint32_t size, _In_ const ptime& tStart, _In_ const ptime& tFinish);
	void getInfo(_Out_ std::vector<std::pair<time_period, CompressionPoint>>& data);
	void getInfo(_Out_ std::map<std::pair<uint32_t, uint32_t>, std::vector<std::pair<time_period, CompressionPoint>>>& data);
	void clear();

	void endPeriod();

private:
	void setExtension();
	void checkPoint(CompressionPoint& dstPoint, CompressionPoint& srcPoint);

	time_duration m_compressionInterval;
	std::wstring m_sExtention;

	time_period m_currentPeriod;
	CompressionPoint m_currentPoint;
	std::map<time_period, CompressionPoint> m_compressionBuffer;
};

