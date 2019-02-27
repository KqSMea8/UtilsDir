#include "StdAfx.h"
#include "Impedance.h"
#include "math.h"

CImpedance::CImpedance(void)
{
	m_fDistance = 0.f;
	m_fSoundSpeed = 0.f;
	m_fTubeDiametr = 0.f;
	m_fEndMicrDistance = 0.f;
	m_fcWaveNumber = NULL;
	m_fcHI = NULL;
	m_fcH12 = NULL;
	m_fcHR = NULL;
	m_lSize = 0;
}

CImpedance::~CImpedance(void)
{
	if (m_fcWaveNumber != NULL)
	{
		ippsFree(m_fcWaveNumber);
		m_fcWaveNumber = nullptr;
	}
	if (m_fcH12 != NULL)
	{
		ippsFree(m_fcH12);
		m_fcH12 = nullptr;
	}
	if (m_fcHI != NULL)
	{
		ippsFree(m_fcHI);
		m_fcHI = nullptr;
	}
	if (m_fcHR != NULL)
	{
		ippsFree(m_fcHR);
		m_fcHR = nullptr;
	}
}

bool CImpedance::CalcParameters(float* pFreq,
								float* pReH12,
								float* pImH12,
								long lSize,
								float* pSoundAbsorptionCoeff,
								float* pReflectionFactorRe,
								float* pReflectionFactorIm,
								float* pAcousticImpedanceRe,
								float* pAcousticImpedanceIm )
{
	bool bReturn(true);
	Ipp32fc fcTemp;
	IppStatus ippState = ippStsNoErr;

	if (m_lSize != lSize)
	{
		m_lSize = lSize;
		if (m_fcWaveNumber != NULL)
			ippsFree(m_fcWaveNumber);
		m_fcWaveNumber = ippsMalloc_32fc(m_lSize);
		if (m_fcH12 != NULL)
			ippsFree(m_fcH12);
		m_fcH12 = ippsMalloc_32fc(m_lSize);
		if (m_fcHI != NULL)
			ippsFree(m_fcHI);
		m_fcHI = ippsMalloc_32fc(m_lSize);
		if (m_fcHR != NULL)
			ippsFree(m_fcHR);
		m_fcHR = ippsMalloc_32fc(m_lSize);
	}

	//копирование передаточной функции 
	bReturn = bReturn && (ippStsNoErr == ippsRealToCplx_32f(pReH12, pImH12, m_fcH12, lSize));
	
	//вычисление волнового числа (k0 = k0' - jk0")
	CalcWaveNumber(pFreq, lSize, m_fcWaveNumber);
	
	//умножение на дистанцию между микрофонами (jsk0)
	fcTemp.re = 0;
	fcTemp.im = m_fDistance;
	bReturn = bReturn && (ippStsNoErr == ippsMulC_32fc_I(fcTemp, m_fcWaveNumber, lSize));
	
	//вычисление HI = exp(-jsk0) и HR = exp(jsk0)
	for (int i = 0; i < lSize; ++i)
	{
		m_fcHI[i].re = exp(-m_fcWaveNumber[i].re) * cos(-m_fcWaveNumber[i].im);
		m_fcHI[i].im = exp(-m_fcWaveNumber[i].re) * sin(-m_fcWaveNumber[i].im);
		m_fcHR[i].re = exp(m_fcWaveNumber[i].re) * cos(m_fcWaveNumber[i].im);
		m_fcHR[i].im = exp(m_fcWaveNumber[i].re) * sin(m_fcWaveNumber[i].im);
	}
	
	//m_fcHI = H12 - HI
	fcTemp.re = -1.f;
	fcTemp.im = 0.f;
	bReturn = bReturn && (ippStsNoErr == ippsMulC_32fc_I(fcTemp, m_fcHI, lSize));
	bReturn = bReturn && (ippStsNoErr == ippsAdd_32fc_I(m_fcH12, m_fcHI, lSize));
	
	//m_fcHR = HR - H12
	fcTemp.re = -1.f;
	fcTemp.im = 0.f;
	bReturn = bReturn && (ippStsNoErr == ippsMulC_32fc_I(fcTemp, m_fcH12, lSize));
	bReturn = bReturn && (ippStsNoErr == ippsAdd_32fc_I(m_fcH12, m_fcHR, lSize));
	
	//m_fcHI = m_fcHI / m_fcHR = (H12 - HI) / (HR - H12)
	bReturn = bReturn && (ippStsNoErr == ippsDiv_32fc_I(m_fcHR, m_fcHI, lSize));

	//вычисление волнового числа (k0 = k0' - jk0")
	fcTemp.re = 2.f * m_fEndMicrDistance / m_fDistance;
	fcTemp.im = 0.f;
	bReturn = bReturn && (ippStsNoErr == ippsMulC_32fc_I(fcTemp, m_fcWaveNumber, lSize));
	
	//вычисление exp(2jk0x1) (m_fcHR = exp(2jk0x1))
	for (int i = 0; i < lSize; ++i)
	{
		m_fcHR[i].re = exp(m_fcWaveNumber[i].re) * cos(m_fcWaveNumber[i].im);
		m_fcHR[i].im = exp(m_fcWaveNumber[i].re) * sin(m_fcWaveNumber[i].im);
	}
	
	//m_fcHI = m_fcHI / m_fcHR = (H12 - HI) / (HR - H12) * exp(2jk0x1) = r - 
	//вычисление комплексного коэффициента отражения
	bReturn = bReturn && (ippStsNoErr == ippsMul_32fc_I(m_fcHR, m_fcHI, lSize));
	bReturn = bReturn && (ippStsNoErr == ippsCplxToReal_32fc(m_fcHI, pReflectionFactorRe, pReflectionFactorIm, lSize));

	//вычисление коэффициента звукопоглощения (? = 1 - Re(r)^2 - Im(r)^2)
	bReturn = bReturn && (ippStsNoErr == ippsPowerSpectr_32fc(m_fcHI, pSoundAbsorptionCoeff, lSize));
	bReturn = bReturn && (ippStsNoErr == ippsMulC_32f_I(-1.f, pSoundAbsorptionCoeff, lSize));
	bReturn = bReturn && (ippStsNoErr == ippsAddC_32f_I(1.f, pSoundAbsorptionCoeff, lSize));
	
	//m_fcHR = 1 + m_fcHI = 1 + r
	fcTemp.re = 1.f;
	fcTemp.im = 0.f;
	bReturn = bReturn && (ippStsNoErr == ippsAddC_32fc(m_fcHI, fcTemp, m_fcHR, lSize));
	
	//m_fcHI = 1 - m_fcHI = 1 - r
	fcTemp.re = -1.f;
	fcTemp.im = 0.f;
	bReturn = bReturn && (ippStsNoErr == ippsMulC_32fc_I(fcTemp, m_fcHI, lSize));
	fcTemp.re = 1.f;
	fcTemp.im = 0.f;
	bReturn = bReturn && (ippStsNoErr == ippsAddC_32fc_I(fcTemp, m_fcHI, lSize));

	//m_fcHR = m_fcHR / m_fcHI = (1 + r) / (1 - r) = Z
	//вычисление комплекстного акустического импеданса
	bReturn = bReturn && (ippStsNoErr == ippsDiv_32fc_I(m_fcHI, m_fcHR, lSize));
	fcTemp.re = m_fAirDensity * m_fSoundSpeed;
	bReturn = bReturn && (ippStsNoErr == ippsMulC_32fc_I(fcTemp, m_fcHR, lSize));
	bReturn = bReturn && (ippStsNoErr == ippsCplxToReal_32fc(m_fcHR, pAcousticImpedanceRe, pAcousticImpedanceIm, lSize));

	return bReturn;
}

void CImpedance::CalcWaveNumber( float* pFreq, long lSize, Ipp32fc* pfcWaveNumber )
{
	for (int i = 0; i < lSize; ++i)
	{
		pfcWaveNumber[i].re = float(IPP_2PI) * pFreq[i] / m_fSoundSpeed;
		pfcWaveNumber[i].im = -0.0194f * sqrt(pFreq[i]) / m_fSoundSpeed / m_fTubeDiametr;
	}
}
