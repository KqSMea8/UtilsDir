#pragma once

#include <ZETSimple\include\singleton\singleton_auto_link.h>
#include <ZETSimple\include\singleton\singleton.h>

#include <boost\noncopyable.hpp>

#include <stdint.h>
#include <string>

class CDriveCheckerImpl : private boost::noncopyable
{
public:
	CDriveCheckerImpl(void);
	~CDriveCheckerImpl(void);

	bool isFreeDiskSpaceExists(_In_ const std::wstring& sSignalDirectory);
	uint64_t getFreeDiskSize(_In_ const std::wstring& sSignalDirectory);
	double getSumFrequency() const {return m_sumFrequency;}
	void setSumFrequency(_In_ const double dFrequency){m_sumFrequency = dFrequency;}
	void setEndRecordTime(_In_ const double dTime){ m_endRecordTime = dTime; }

	//Event<CString> m_hEventDriveCheckerWarning;

protected:
	double m_sumFrequency;
	double m_endRecordTime;
};

typedef CSingleton<CDriveCheckerImpl> CDriveChecker;

