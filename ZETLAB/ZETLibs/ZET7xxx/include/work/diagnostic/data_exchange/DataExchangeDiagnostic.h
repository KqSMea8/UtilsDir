#pragma once
#include "diagnostic\Diagnostic.h"

#include <ZETSimple\include\time\timer\difftimer\DiffTimer2.h>

class CDataExchangeDiagnostic : public CDiagnostic
{
public:
	CDataExchangeDiagnostic();
	~CDataExchangeDiagnostic();

	void addInfo(_In_ CDiagnosticInfo* pInfo, bool bCount = true) override;

protected:
	void clear() override;

private:
	std::map<uint8_t, uint64_t> m_streamDataBytesCounter;
};

