#ifndef invoice_converter_h_
#define invoice_converter_h_

#include "invoice_types.h"

class CInvoiceConverter
{
public:
	CInvoiceConverter(_In_ const OrderList& orderList);
	~CInvoiceConverter();

	DeviceOrderList get7xxxList() const { return m_7xxxList; }
	DeviceOrderList getDeviceList() const { return m_deviceList; }
	DeviceOrderList getAnalogList() const { return m_analog_list; }

protected:

	void get7xxxFromOrderList(_In_ const OrderList& orderList);

private:	
	DeviceOrderList m_7xxxList;
	DeviceOrderList m_deviceList;
	DeviceOrderList m_analog_list;
};

#endif // invoice_converter_h_