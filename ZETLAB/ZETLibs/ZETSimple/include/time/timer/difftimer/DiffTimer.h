#pragma once
#include "../Timer.h"

class CDiffTimer
{
public:
	CDiffTimer();
	~CDiffTimer();

	void Start();
	double GetTime();

protected:
	void SetStartTime();
	CTimer m_tTimer;
	double m_dStartTime;
};

