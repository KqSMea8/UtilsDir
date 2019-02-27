#pragma once

#ifndef __wminfo
#define __wminfo

#include <vector>
#include <string.h>

#define WMFSIZE 100


struct WMInfo
{
	WMInfo::WMInfo()
		:ppsSample(0)

	{
		timelabel.resize(WMFSIZE);
		memset(timelabel.data(), 0, timelabel.size() * sizeof(long));
		rtcTime = { 0 };
		ucQuality = { 0 };
	}

	long ppsSample;
	time_t rtcTime;
	unsigned char ucQuality;
	std::vector<long> timelabel;
};

#endif