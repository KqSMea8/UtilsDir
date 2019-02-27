//------------------------------------------------------------------------------
#include "StdAfx.h"
#include <Algorithms\LdsEnergy.h>
//------------------------------------------------------------------------------
CLdsDataEnergyBand::CLdsDataEnergyBand()
	: m_bInit(false)
	, m_bFull(false)
	, m_bThresholdsEnabled(false)
	, m_Thresholds(0.f)
	, m_alfa(2.f)
	, m_saAbsData()
	, m_CurIndex(0)
{}
//------------------------------------------------------------------------------
CLdsDataEnergyBand::CLdsDataEnergyBand(const CLdsDataEnergyBand &val)
	: m_saAbsData()
{
	*this = val;
}
//------------------------------------------------------------------------------
CLdsDataEnergyBand::~CLdsDataEnergyBand()
{}
//------------------------------------------------------------------------------
void CLdsDataEnergyBand::operator = (const CLdsDataEnergyBand &val)
{
	m_bInit = val.m_bInit;
	m_bFull = val.m_bFull;
	m_bThresholdsEnabled = val.m_bThresholdsEnabled;
	m_CurIndex = val.m_CurIndex;
	m_Thresholds = val.m_Thresholds;
	m_alfa = val.m_alfa;
//	m_saAbsData = val.m_saAbsData; лучше так:
	if (m_saAbsData.ReSize(val.m_saAbsData.size) && (m_saAbsData.size > 0))
		ippsCopy_32f(val.m_saAbsData.pData, m_saAbsData.pData, (int)m_saAbsData.size);
}
//------------------------------------------------------------------------------
bool CLdsDataEnergyBand::Create(DWORD size, float alfa)
{
	bool ret = !m_bInit;
	if (ret)
	{
		m_bInit = (size > 0);
		ret = m_bInit && m_saAbsData.ReSize(size);
		SetAlfa(alfa);
	}
	return ret;
}
//------------------------------------------------------------------------------
bool CLdsDataEnergyBand::SetAlfa(float val)
{
	bool ret = (0.f <= val) && (val <= 2.f);
	if (ret)
		m_alfa = val;
	return ret;
}
//------------------------------------------------------------------------------
float CLdsDataEnergyBand::GetThreshold()
{
	float ret;
	if (m_bThresholdsEnabled)
		ret = m_Thresholds;
	else
		ret = 0.f;
	return ret;
}
//------------------------------------------------------------------------------
float CLdsDataEnergyBand::GetThreshold2()
{
	float ret;
	if (m_bThresholdsEnabled)
		ret = m_Thresholds2;
	else
		ret = 0.f;
	return ret;
}
//------------------------------------------------------------------------------
bool CLdsDataEnergyBand::AddAbsValue(float val, float val2,
	bool &bEvent, float &energyEvent2)
{
	bool ret = m_bInit;
	bEvent = false;
	energyEvent2 = 0.f;
	if (ret)
	{
		if (m_bThresholdsEnabled)
		{
			bEvent = val > m_Thresholds;
			if (bEvent)
			{
				float x = val2 - m_Thresholds2;
				if (x < 0.f)
					x = 0.f;
				energyEvent2 = x;
			}
		}

		m_saAbsData.pData[m_CurIndex] = val;
		if (++m_CurIndex == m_saAbsData.size)
		{
			m_CurIndex = 0;
			m_bFull = true;
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
bool CLdsDataEnergyBand::CalcThresholds()
{
	bool ret = m_bInit && m_bFull;
	if (ret)
	{
		float stdDef, ma;
		ippsMax_32f(m_saAbsData.pData, (int)m_saAbsData.size, &ma);
		ippsStdDev_32f(m_saAbsData.pData, (int)m_saAbsData.size,
							&stdDef, ippAlgHintFast);
		m_Thresholds = ma + m_alfa * stdDef;
		m_Thresholds2 = m_Thresholds * m_Thresholds;
		m_bThresholdsEnabled = true;
	}
	else
		m_bThresholdsEnabled = false;
	return ret;
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
CLdsEnergy::CLdsEnergy() :
m_bInit(false),
m_numBandForEvent(0),
m_EnergyNoise2(0.f),
m_numBandForEventInPercentage(NumEventInPercentage_MinValue),
m_sapBand()
{}
//------------------------------------------------------------------------------
CLdsEnergy::CLdsEnergy(const CLdsEnergy &val)
	: m_sapBand()
{
	*this = val;
}
//------------------------------------------------------------------------------
void CLdsEnergy::operator = (const CLdsEnergy &val)
{
	m_bInit = val.m_bInit;
	m_EnergyNoise2 = val.m_EnergyNoise2;
	m_numBandForEventInPercentage = val.m_numBandForEventInPercentage;
	m_numBandForEvent = val.m_numBandForEvent;

	if (val.m_sapBand.size > 0)
	{
		if (m_sapBand.size != val.m_sapBand.size)
		{
			Delete();
			m_sapBand.ReSize(val.m_sapBand.size);
		}
		for (DWORD i = 0; i < m_sapBand.size; ++i)
			m_sapBand.pData[i] = val.m_sapBand.pData[i];
	}
	else
	{//val.m_sapBand.size = 0
		Delete();
		m_sapBand.ReSize(0);
	}
}
//------------------------------------------------------------------------------
CLdsEnergy::~CLdsEnergy()
{
	Delete();
}
//------------------------------------------------------------------------------
void CLdsEnergy::Delete()
{
	if (m_sapBand.size > 0)
	{
		for (DWORD i = 0; i < m_sapBand.size; ++i)
		{
			if (m_sapBand.pData[i] != NULL)
			{
				delete m_sapBand.pData[i];
				m_sapBand.pData[i] = nullptr;
			}
		}
	}
}
//------------------------------------------------------------------------------
bool CLdsEnergy::Create(DWORD numBand, DWORD sizeBand, float alfa)
{
	bool ret = !m_bInit;
	if (ret)
	{
		m_bInit = (numBand > 0) && m_sapBand.ReSize(numBand);
		if (m_bInit)
		{
			for (DWORD i = 0; i < m_sapBand.size; ++i)
			{
				m_sapBand.pData[i] = new CLdsDataEnergyBand();
				m_sapBand.pData[i]->Create(sizeBand, alfa);
			}
			SetNumBandForEventInPercentage(NumEventInPercentage_MinValue);
		}
		ret = m_bInit;
	}
	return ret;
}
//------------------------------------------------------------------------------
// функции свойств
//------------------------------------------------------------------------------
bool CLdsEnergy::SetNumBandForEventInPercentage(float val)
{
	bool ret = (0 <= val) && (val <= 100.f);
	if (ret)
	{
		m_numBandForEventInPercentage = val;
		m_numBandForEvent = (DWORD)(int)floor(0.01f * val * m_sapBand.size);
	}
	return ret;
}
//------------------------------------------------------------------------------
// функции класса
//------------------------------------------------------------------------------
float CLdsEnergy::GetEnergyNoise()
{
	return m_EnergyNoise2;
}
//------------------------------------------------------------------------------
// pData - массив новых значений по полосам. На каждую полосу одно значение
bool CLdsEnergy::Add(float *pData, bool &bEvent,
	float &sign_noise, float &numBand_float,
	DWORD &indxBandWithEventMin,  DWORD &indxBandWithEventMax)
{
	bEvent = false;
	sign_noise = 0.f;
	numBand_float = 0.f;
	indxBandWithEventMin = DWORD_MAX;
	indxBandWithEventMax = DWORD_MAX;
	bool ret = m_bInit && (pData != NULL);
	if (ret)
	{
		DWORD _numBandWithEvent = 0;
		float _energuFull2 = 0.f;
		float _energyNoise2 = 0.f;

		for (DWORD i = 0; i < m_sapBand.size; ++i)
		{
			float absVal = fabs(pData[i]);
			float val2 = absVal * absVal;
			bool flag(false);
			float _evergyEventBand2 = 0.f;
			m_sapBand.pData[i]->AddAbsValue(absVal, val2, flag, _evergyEventBand2);
			if (flag)
			{
				_energuFull2 += val2;
				_energyNoise2 += m_sapBand.pData[i]->GetThreshold2();
				if (indxBandWithEventMin == DWORD_MAX)
					indxBandWithEventMin = i;
				indxBandWithEventMax = i;
				++_numBandWithEvent;
			}
		}
		if (_energyNoise2 > 0.f)
			sign_noise = 10 * log10(_energuFull2 / _energyNoise2);	// в дБ
		numBand_float = (float)_numBandWithEvent / m_sapBand.size;
		bEvent = (_numBandWithEvent > 0) && (_numBandWithEvent >= m_numBandForEvent);
	}
	return ret;
}
//------------------------------------------------------------------------------
void CLdsEnergy::CalcThresholds()
{
	m_EnergyNoise2 = 0.f;
	if (m_bInit)
	{
		for (DWORD i = 0; i < m_sapBand.size; ++i)
		{
			if (m_sapBand.pData[i]->CalcThresholds())
				m_EnergyNoise2 += m_sapBand.pData[i]->GetThreshold2();
		}
	}
}
//------------------------------------------------------------------------------
void CLdsEnergy::ResetThreshold()
{
	if (m_bInit)
	{
		for (DWORD i = 0; i < m_sapBand.size; ++i)
			m_sapBand.pData[i]->ResetThreshold();
	}
}
//------------------------------------------------------------------------------