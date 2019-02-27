#pragma once

#include <windows.h>

class CTimer
{
public:
	CTimer();
	~CTimer();
	
	double GetTime();

protected:
	LARGE_INTEGER m_liFrequency;
	LARGE_INTEGER m_liCounter;
};

