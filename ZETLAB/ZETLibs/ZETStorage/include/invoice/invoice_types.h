#ifndef invoice_types_h_
#define invoice_types_h_

#include <stdint.h>
#include <list>
#include <tuple>

typedef std::pair<std::wstring, uint32_t> OrderPosition;
typedef std::list<OrderPosition> OrderList;

struct DeviceOrderPosition
{
	uint32_t uType_Index;
	std::wstring sType_Name;
	uint32_t uCount;
	uint32_t uCount_Verification;
	bool operator == (const uint32_t &_uType_Index)
	{
		return uType_Index == _uType_Index;
	}

	bool operator == (const std::wstring &_sName)
	{
		return sType_Name.find(_sName) != -1;
	}
};
//typedef std::tuple<uint32_t, uint32_t, uint32_t> DeviceOrderPosition;
typedef std::list<DeviceOrderPosition> DeviceOrderList;

#endif // invoice_types_h_