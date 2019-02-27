//----------------------------------------------------------------------------------------------------------------------
#pragma once
#include <SOU\PressureSensor\Detection\CustomDetector\CustomDetector.h>
#include <SOU\PressureSensor\Detection\STA_LTA\Struct_Res\Lds_res_Detector_Pressure.zschema1.h>
#include <Algorithms\MovingAverage.h>
//----------------------------------------------------------------------------------------------------------------------
struct Param_STA_LTA_Pressure
{
	double m_valSta = 0.;
	double m_valSta2 = 0.;
	double m_valStdDevSta = 0.;
	double m_valLta = 0.;
	double m_valLta2 = 0.;
	double m_valStdDevLta = 0.;
	double m_valStaLtaDiff = 0.;

	float m_lastStaOld = 0.f;
	float m_lastStaNew = 0.f;
	float m_lastLtaOld = 0.f;
	float m_lastLtaNew = 0.f;
	float m_lastStaLtaOld4Diff = 0.f;
	float m_lastStaLtaNew4Diff = 0.f;
	float m_lastStaLtaOld4StdDevMean = 0.f;
	float m_lastStaLtaNew4StdDevMean = 0.f;

	unsigned long long m_numData = 0;

	CShiftBufferArbitraryPage<float> m_sourceData;
	CShiftBufferArbitraryPage<float> m_staData;
	CShiftBufferArbitraryPage<float> m_staData2;
	CShiftBufferArbitraryPage<float> m_ltaData;
	CShiftBufferArbitraryPage<float> m_ltaData2;
	CShiftBufferArbitraryPage<float> m_stdDevStaData;
	CShiftBufferArbitraryPage<float> m_stdDevLtaData;
	CShiftBufferArbitraryPage<float> m_staLtaData;
	CShiftBufferArbitraryPage<float> m_staLtaDiffData;
	CShiftBufferArbitraryPage<float> m_staLtaStdDevData;
	CShiftBufferArbitraryPage<float> m_staLtaMeanData;

	Param_STA_LTA_Pressure()
		: m_valSta(0.)
		, m_valSta2(0.)
		, m_valStdDevSta(0.)
		, m_valLta(0.)
		, m_valLta2(0.)
		, m_valStdDevLta(0.)
		, m_valStaLtaDiff(0.)
		, m_lastStaOld(0.f)
		, m_lastStaNew(0.f)
		, m_lastLtaOld(0.f)
		, m_lastLtaNew(0.f)
		, m_lastStaLtaOld4Diff(0.f)
		, m_lastStaLtaNew4Diff(0.f)
		, m_lastStaLtaOld4StdDevMean(0.f)
		, m_lastStaLtaNew4StdDevMean(0.f)
	{}

	virtual bool NewData(const float* pData, const int sizePage, const int numSTA, const int numLTA, const double normFactor);
	virtual void SetSize(int size);
	virtual void Zero();
};
//----------------------------------------------------------------------------------------------------------------------
class CDetector_Pressure : public CCustomDetectorPressure
{
private:
	int m_numSTA = 1000;
	int m_numLTA = 19000;
	int m_sizeEventMin = 1000;
	float m_envelope = 0.f;
	float m_alfa;
	float m_1_alfa;
	double m_normFactor = 100.;
		
	CMovingAverage<float> m_maStDev;
	CShiftBufferArbitraryPage<float> m_buff_StdDev;
	
protected:
	Param_STA_LTA_Pressure* m_pParamStaLtaPressure = nullptr;

public:
	CDetector_Pressure();
	virtual ~CDetector_Pressure();
	virtual bool Create(Lds_cfg_Detector_Pressure_ns::Proxy & proxy);

	Lds_res_Detector_Pressure m_resSTA;

	float* GetPointerForDataStaLta();
	float* GetPointerForDataStaLtaDiff();
	float* GetPointerForDataStaLtaStdDev();
	float* GetPointerForDataStaLtaMean();
	int GetSizeForDataStaLta();
	float* GetPointerForDataAmpl();
	float* GetPointerForDataStdDev();
	unsigned long long GetNumData();

	virtual void ZeroData();
	virtual bool Calculation(const float* pData, const int sizePage);
};
//----------------------------------------------------------------------------------------------------------------------