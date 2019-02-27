#pragma once
#include "base\Types.h"
#include "base\ErrorCodes.h"
#include "diagnostic\DiagnosticInfo.h"

#include <ZETSimple\include\time\timer\difftimer\DiffTimer.h>

using namespace zet7xxx;
using namespace modbus;
using namespace errors;

class CExchangeInfo : public CDiagnosticInfo
{
public:
	CExchangeInfo();
	~CExchangeInfo();

	void begin(_In_ const MODBUS_PACKAGE_STRUCT& request, _In_ const uint32_t requestSize);
	void response(uint32_t responseSize);
	void end(_In_ const MODBUS_PACKAGE_STRUCT& response,
		_In_ const ResponseErrorCode recCode, _In_ const ExchangeErrorCode eecCode);
	
	void setEecCode(ExchangeErrorCode eecCode) { m_exchangeErrorCode = eecCode; }

	uint8_t getAddress() const { return m_request.node; }
	ModbusCommand getCommand() const { return m_request.command; }
	ExchangeErrorCode getExchangeErrorCode() const { return m_exchangeErrorCode; }
	ResponseErrorCode getResponseErrorCode() const { return m_responseErrorCode; }
	double getResponseTime() const { return m_responseTime; }
	uint32_t getExchangeByteSize() const { return m_requestSize + m_responseSize; }
	uint16_t getDataSize() const { return m_response.size; }
	uint32_t getResponseSize() const { return m_responseSize; }
	uint32_t getRequestSize() const { return m_requestSize; }

protected:
	MODBUS_PACKAGE_STRUCT m_request;
	MODBUS_PACKAGE_STRUCT m_response;
	ResponseErrorCode m_responseErrorCode;
	ExchangeErrorCode m_exchangeErrorCode;
	double m_exchangeTime;
	double m_responseTime;
	uint32_t m_requestSize;
	uint32_t m_responseSize;

private:
	CDiffTimer m_timer;
	bool m_bResponse;
};
