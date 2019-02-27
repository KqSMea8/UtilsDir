//----------------------------------------------------------------------------------------------------------------------
#pragma once
#include <SOU\PressureSensor\Detection\STA_LTA\Dtct_Pressure.h>
//----------------------------------------------------------------------------------------------------------------------
struct Param_STA_LTA_Pressure_StaLta : public Param_STA_LTA_Pressure
{
	double m_sumYLta = 0.;
	double m_sumYSta = 0.;
	
	Param_STA_LTA_Pressure_StaLta()
		: m_sumYSta(0.)
		, m_sumYLta(0.)
	{}

	bool NewData(const float *pData, const int sizePage, const int numSTA, const int numLTA, const double normFactor) override;
	void SetSize(int size) override;
	void Zero() override;
};
//----------------------------------------------------------------------------------------------------------------------
class CDetector_Pressure_StaLta : public CDetector_Pressure
{
private:
		
protected:

public:
	CDetector_Pressure_StaLta();
	virtual ~CDetector_Pressure_StaLta();
};
//----------------------------------------------------------------------------------------------------------------------