#include "Modbus\ExternalDevices\Tools\Device finder\devicefinder.h"

CDeviceFinder::CDeviceFinder(CString path_)
{
	m_sDevicesPath = path_;
	m_vDevices.clear();

	WIN32_FIND_DATA findFileData;
	HANDLE findHandle;

	findHandle = FindFirstFile(path_ + L"\\*", &findFileData);

	if (findHandle != INVALID_HANDLE_VALUE)
	{
		do
		{
			CString deviceName = CString(findFileData.cFileName);

			if (deviceName != L"." && deviceName != L"..")
			{
				if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					m_vDevices.push_back(deviceName);
				}
			}
		} while (FindNextFile(findHandle, &findFileData) != 0);
	}

	FindClose(findHandle);
}

CDeviceFinder::~CDeviceFinder()
{

}

unsigned long CDeviceFinder::getDevicesCount()
{
	return m_vDevices.size();
}

CString CDeviceFinder::getDeviceName(unsigned long numDevice_)
{
	CString retVal = L"";

	if (numDevice_ < m_vDevices.size())
		retVal = m_vDevices[numDevice_];

	return retVal;
}

CString CDeviceFinder::getDevicePath(unsigned long numDevice_)
{
	CString retVal = L"";

	if (numDevice_ < m_vDevices.size())
		retVal = m_sDevicesPath + L"\\" + m_vDevices[numDevice_];

	return retVal;
}

CString CDeviceFinder::getDeviceCfgAdr(unsigned long numDevice_)
{
	CString retVal = L"";

	if (numDevice_ < m_vDevices.size())
		retVal = m_sDevicesPath + L"\\" + m_vDevices[numDevice_] + L"\\" + m_vDevices[numDevice_] + L".cfg";

	return retVal;
}