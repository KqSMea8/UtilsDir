#pragma once
#include <SimpleEvent/Event.h>

#include <stdint.h>

class CDriveChecker
{
public:
	CDriveChecker(void);
	~CDriveChecker(void);

	BOOL GetFreeDiskSpace(CString sSignalDirectory);
	unsigned long long GetFreeDiskSize(CString sSignalDirectory);
	double GetSumFrequency(){return m_dSumFrequency;}
	void SetSumFrequency(double dFrequency){m_dSumFrequency = dFrequency;}
	void SetEndRecordTime(double dTime){m_dEndRecordTime = dTime;}

	Event<uint32_t> m_hEventDriveCheckerWarning;

protected:
	double m_dSumFrequency;
	double m_dEndRecordTime;
};

