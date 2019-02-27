#pragma once

#include "ErrorCodes.h"
#include "Types.h"

#include <ZETSimple\include\debugtools\DebugTools.h>

using namespace zet7xxx;
using namespace modbus;
using namespace diagnostic;
using namespace firmware;

class CModbusBase
{
public:
	CModbusBase();
	CModbusBase(const CModbusBase &value);
	virtual ~CModbusBase();

	void operator= (const CModbusBase &value);

	virtual void getEvents(_Out_ LogEventList& eventLog);

protected:
	void addEventMessage(_In_ const std::wstring& sMessage);
	void addEventMessage(_In_ const LogEvent& message);

private:
	LogEventList m_events;
	CRITICAL_SECTION m_eventsCriticalSection;
};

