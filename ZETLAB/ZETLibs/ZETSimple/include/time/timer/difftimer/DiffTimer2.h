#pragma once

#include <windows.h>

class CDiffTimer2
{
public:
	CDiffTimer2();
	~CDiffTimer2();

	void Start();
	double GetTime();

protected:
	void SetStartTime();
	SYSTEMTIME m_startTime;
};

