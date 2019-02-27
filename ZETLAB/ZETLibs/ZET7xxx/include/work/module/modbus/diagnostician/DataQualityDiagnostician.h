#pragma once
#include "Diagnostician.h"

#pragma warning( disable : 4290 )

class CDataQualityDiagnostician : public CDiagnostician
{
public:
	CDataQualityDiagnostician(_In_ const std::vector<CModbusSlave*> &devices, _In_ CDiagnostic* pDiagnostic) 
		throw (std::exception);
	~CDataQualityDiagnostician();

	void setResponsePackage(MODBUS_PACKAGE_STRUCT& package);

protected:
	void prepareSlave() override;

	bool isDiagnosticEnable() override;
	
private:
	int32_t m_stage;
	
	enum Stage
	{
		stageReadDiagnost = 0,
	};

	void prepare4ReadDiagnost();
};

