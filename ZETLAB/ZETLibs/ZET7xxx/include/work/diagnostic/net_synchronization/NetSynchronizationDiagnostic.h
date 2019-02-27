#pragma once
#include "../Diagnostic.h"

class CNetSynchronizationDiagnostic : public CDiagnostic
{
public:
	CNetSynchronizationDiagnostic();
	~CNetSynchronizationDiagnostic();

	void addInfo(_In_ CDiagnosticInfo* pInfo, bool bCount = true) override;

protected:
private:
};
