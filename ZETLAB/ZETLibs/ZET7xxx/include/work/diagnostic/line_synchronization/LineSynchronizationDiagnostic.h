#pragma once
#include "../Diagnostic.h"

class CLineSynchronizationDiagnostic : public CDiagnostic
{
public:
	CLineSynchronizationDiagnostic();
	~CLineSynchronizationDiagnostic();

	void addInfo(_In_ CDiagnosticInfo* pInfo, bool bCount = true) override;

protected:
private:
};
