#pragma once
#include "../Diagnostic.h"

class CDataQualityDiagnostic : public CDiagnostic
{
public:
	CDataQualityDiagnostic();
	~CDataQualityDiagnostic();

	void addInfo(_In_ CDiagnosticInfo* pInfo, bool bCount = true) override;

protected:
private:
};

