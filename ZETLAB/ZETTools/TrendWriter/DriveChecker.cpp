#include "StdAfx.h"
#include "DriveChecker.h"

CDriveChecker::CDriveChecker(void)
: m_dSumFrequency(0.)
, m_dEndRecordTime(0.)
{
}

CDriveChecker::~CDriveChecker(void)
{
}

BOOL CDriveChecker::GetFreeDiskSpace(CString sSignalDirectory)
{
	unsigned long long ullFreeSpace = GetFreeDiskSize(sSignalDirectory);
		
	long long llNeedSize = long long(m_dSumFrequency * sizeof(float) * 60. * 60. + 0.5);
	double dHourQuantity = double(ullFreeSpace) / double(llNeedSize);
	if(dHourQuantity < m_dEndRecordTime)
	{
		m_hEventDriveCheckerWarning(this, uint32_t(dHourQuantity));
		return FALSE;
	}
	else
		return TRUE;
}

unsigned long long CDriveChecker::GetFreeDiskSize(CString sSignalDirectory)
{
	unsigned long long ullFreeSpace(0);
	ULARGE_INTEGER ulFreeBytesAvailable;
	ULARGE_INTEGER ulTotalNumberOfBytes;
	ULARGE_INTEGER ulTotalNumberOfFreeBytes;

	int iLength = wcslen(sSignalDirectory);
	wchar_t	wcTempPath[MAX_PATH] = { 0 };
	wcscpy_s(wcTempPath, MAX_PATH, sSignalDirectory.GetString());
	LPWSTR pPathPointer = wcTempPath;
	
	if (iLength > 3)
	{
		if (((wcTempPath[0] == '\\') && (wcTempPath[1] == '\\')) || ((wcTempPath[0] == '/') && (wcTempPath[1] == '/')))
		{
			pPathPointer = wcTempPath + 2;
			pPathPointer += wcscspn(pPathPointer, _T("\\/"));
			if (*pPathPointer != 0)
			{
				pPathPointer++;
				pPathPointer += wcscspn(pPathPointer, _T("\\/"));
				if (*pPathPointer != 0)
				{
					*(pPathPointer + 1) = 0;
				}
			}
			pPathPointer = wcTempPath;
		}
		else
			*(pPathPointer + 3) = 0;
	}
	if (iLength < 3)
	{ 
		ullFreeSpace = 1024 * 1024;
	}
	else
	{
		if (GetDiskFreeSpaceEx(pPathPointer, &ulFreeBytesAvailable, &ulTotalNumberOfBytes, &ulTotalNumberOfFreeBytes))
			ullFreeSpace = ulFreeBytesAvailable.QuadPart;
	}
	
	return ullFreeSpace;
}
