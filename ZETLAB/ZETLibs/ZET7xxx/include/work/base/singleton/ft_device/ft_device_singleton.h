#pragma once

#ifndef ft_devices_singleton_h_
#define ft_devices_singleton_h_

#include <ZET7xxx\include\base\ZET7xxxDefines.h>
#include "iohelper\ftdi\libFTDI.h"

#include <ZETSimple\include\singleton\singleton.h>

#include <boost\noncopyable.hpp>

class CFtDevicesSingletonImpl : private boost::noncopyable
{
public:
	CFtDevicesSingletonImpl();
	~CFtDevicesSingletonImpl();

	void update(_In_opt_ FT_DEVICE_LIST_INFO_NODE* pInfo, _In_ const DWORD dwDeviceQuantity);
	void getDeviceList(_Outref_result_maybenull_ FT_DEVICE_LIST_INFO_NODE*& pInfo, _Out_ DWORD& dwDeviceQuantity);

private:
	void clear();

	DWORD m_dwDeviceQuantity;
	FT_DEVICE_LIST_INFO_NODE* m_pInfo;

	CRITICAL_SECTION m_csData;
};

typedef CSingleton<CFtDevicesSingletonImpl> CFtDevicesSingleton;

#endif //ft_devices_singleton_h_
