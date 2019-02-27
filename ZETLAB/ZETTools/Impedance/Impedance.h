#pragma once
#include <ipp.h>

class CImpedance
{
private:
	float m_fDistance;
	float m_fSoundSpeed;
	float m_fTubeDiametr;
	float m_fEndMicrDistance;
	float m_fAirDensity;

	Ipp32fc* m_fcWaveNumber;
	Ipp32fc* m_fcH12;
	Ipp32fc* m_fcHI;
	Ipp32fc* m_fcHR;
	long m_lSize;

	void CalcWaveNumber(float* pFreq, long lSize, Ipp32fc* pfcWaveNumber);

public:
	CImpedance(void);
	~CImpedance(void);

	float GetDistance(){return m_fDistance;};
	void SetDistance(float fDistance){m_fDistance = fDistance;};

	float GetSoundSpeed(){return m_fSoundSpeed;};
	void SetSoundSpeed(float fSoundSpeed){m_fSoundSpeed = fSoundSpeed;};

	float GetTubeDiametr(){return m_fTubeDiametr;};
	void SetTubeDiametr(float fTubeDiametr){m_fTubeDiametr = fTubeDiametr;};

	float GetEndMicrDistance(){return m_fEndMicrDistance;};
	void SetEndMicrDistance(float fEndMicrDistance){m_fEndMicrDistance = fEndMicrDistance;};

	float GetAirDensity(){return m_fAirDensity;};
	void SetAirDensity(float fAirDensity){m_fAirDensity = fAirDensity;};

	bool CalcParameters(float* pFreq, float* pReH12, float* pImH12, long lSize, float* pSoundAbsorptionCoeff, float* pReflectionFactorRe, float* pReflectionFactorIm, float* pAcousticImpedanceRe, float* pAcousticImpedanceIm);
};

