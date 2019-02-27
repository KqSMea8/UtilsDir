#ifndef invoice_parser_h_
#define invoice_parser_h_

#include "invoice_types.h"

#include <string>
#include <tchar.h>
#include <comdef.h>

class CInvoiceParser
{
public:
	CInvoiceParser(_In_ const std::wstring& sFileName);
	~CInvoiceParser();

	uint32_t getOrderNumber() const { return m_orderNumber; }
	uint32_t getOrderYear() const { return m_orderYear; }
	uint64_t getClientInn() const { return m_clientInn; }
	OrderList getOrderList() const { return m_orderList; }

protected:
	uint32_t parse();

	std::wstring m_sFileName;

private:
	uint32_t getOrder(_In_ const IDispatchPtr& pInterface, _Out_ uint32_t& orderNumber, _Out_ uint32_t& orderYear);
	uint32_t getClient(_In_ const IDispatchPtr& pInterface, _Out_ uint64_t& clientInn, _Out_ std::wstring& clientName);
	uint32_t getOrderList(_In_ const IDispatchPtr& pInterface, _Out_ OrderList& orderList);

	uint32_t m_orderNumber;
	uint32_t m_orderYear;
	uint64_t m_clientInn;
	std::wstring m_clientName;
	OrderList m_orderList;
};

#endif // invoice_parser_h_