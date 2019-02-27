//----------------------------------------------------------------------------------------------------------------------
#pragma once
#include <SOU\PressureSensor\Detection\STA_LTA\Dtct_Pressure.h>
//----------------------------------------------------------------------------------------------------------------------
struct Param_STA_LTA_Pressure_Gradient : public Param_STA_LTA_Pressure
{
	double m_sumYSta = 0.;
	double m_sumY2Sta = 0.;
	double m_sumXYSta = 0.;
	double m_sumYLta = 0.;
	double m_sumY2Lta = 0.;
	double m_sumXYLta = 0.;
	double m_sumYStaLta4Diff = 0.;
	double m_sumY2StaLta4Diff = 0.;
	double m_sumXYStaLta4Diff = 0.;
	double m_sumYStaLta4StdDevMean = 0;
	double m_sumY2StaLta4StdDevMean = 0;

	Param_STA_LTA_Pressure_Gradient()
		: m_sumYSta(0.)
		, m_sumY2Sta(0.)
		, m_sumXYSta(0.)
		, m_sumYLta(0.)
		, m_sumY2Lta(0.)
		, m_sumXYLta(0.)
		, m_sumYStaLta4Diff(0.)
		, m_sumY2StaLta4Diff(0.)
		, m_sumXYStaLta4Diff(0.)
		, m_sumYStaLta4StdDevMean(0.)
		, m_sumY2StaLta4StdDevMean(0.)
	{}

	bool NewData(const float *pData, const int sizePage, const int numSTA, const int numLTA, const double normFactor) override;
	void SetSize(int size) override;
	void Zero() override;
};
//----------------------------------------------------------------------------------------------------------------------
class CDetector_Pressure_Gradient : public CDetector_Pressure
{
private:
		
protected:

public:
	CDetector_Pressure_Gradient();
	virtual ~CDetector_Pressure_Gradient();
};
//----------------------------------------------------------------------------------------------------------------------