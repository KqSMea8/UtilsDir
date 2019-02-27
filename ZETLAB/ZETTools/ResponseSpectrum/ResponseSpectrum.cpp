#include "ResponseSpectrum.h"


CResponseSpectrum::CResponseSpectrum()
{

}

CResponseSpectrum::~CResponseSpectrum()
{
	if (m_struct.pData != nullptr)
	{
		delete[] m_struct.pData;
		m_struct.pData = nullptr;
	}
	if (m_response.pFreqs != nullptr)
	{
		delete[] m_response.pFreqs;
		m_response.pFreqs = nullptr;
	}
	if (m_response.pReps != nullptr)
	{
		delete[] m_response.pReps;
		m_response.pReps = nullptr;
	}

	m_spectr.Clear();
}

long CResponseSpectrum::SetInputStruct(SResponseIn *pInData)
{
	if (pInData->dSampling <= 0)
		return -102;

	if (pInData->dFreqBegin <= 0)
		return -103;

	if (pInData->dFreqEnd > pInData->dSampling / 2)
		pInData->dFreqEnd = pInData->dSampling / 2;

	if (pInData->dFreqBegin > pInData->dFreqEnd)
		return -104;

	if (pInData->dDecrement < 0)
		pInData->dDecrement = 0;

	if (pInData->lOctaveNum < 2)
		pInData->lOctaveNum = 2;

	m_struct.dSampling = pInData->dSampling;
	m_struct.dFreqBegin = pInData->dFreqBegin;
	m_struct.dFreqEnd = pInData->dFreqEnd;
	m_struct.dDecrement = pInData->dDecrement;
	m_struct.lOctaveNum = pInData->lOctaveNum;
	
	return SetInputData(pInData->pData, pInData->lSize);
}

long CResponseSpectrum::SetInputData(double *data, long size)
{
	if (size > 0)
	{
		if (m_struct.lSize != size)
		{
			m_struct.lSize = size;
			if (m_struct.pData != nullptr)
				delete[] m_struct.pData;
			m_struct.pData = new double[m_struct.lSize];
		}
		memcpy_s(m_struct.pData, m_struct.lSize * sizeof(m_struct.pData[0]),
			data, m_struct.lSize * sizeof(m_struct.pData[0]));

		m_spectr = Srs(m_struct.dSampling, m_struct.lSize, m_struct.pData,
			m_struct.lOctaveNum, m_struct.dFreqBegin, m_struct.dFreqEnd, m_struct.dDecrement);

		return 0;
	}
	else
		return -101;
}

long CResponseSpectrum::GetOutputStruct(SResponseOut *pOutData)
{
	if (m_spectr.Length_srs > 0)
	{
		if (pOutData->lSize != m_spectr.Length_srs)
		{
			pOutData->lSize = m_spectr.Length_srs;
			if (pOutData->pFreqs != nullptr)
				delete[] pOutData->pFreqs;
			if (pOutData->pReps != nullptr)
				delete[] pOutData->pReps;
			pOutData->pFreqs = new double[pOutData->lSize];
			pOutData->pReps = new double[pOutData->lSize];
		}
		for (long i = 0; i < pOutData->lSize; i++)
			pOutData->pFreqs[i] = m_spectr.FreqVec[i];
		for (long i = 0; i < pOutData->lSize; i++)
			pOutData->pReps[i] = m_spectr.ShockResponseSpectrum[i];
		return 0;
	}
	else
		return -201;
}

long CResponseSpectrum::CalculateSRSAAM(ESRSType typeSRS)
{
	switch (typeSRS)
	{
	case Overall:  m_spectr.CompSmallwoodSRSAAM(0); break;
	case Positive: m_spectr.CompSmallwoodSRSAAM(1); break;
	case Negative: m_spectr.CompSmallwoodSRSAAM(-1); break;
	}

	return 0;
}

long CResponseSpectrum::Calculate(SResponseIn *pInData, SResponseOut *pOutData, ESRSType type /* = Overall */)
{
	m_spectr.Clear();

	long ret = SetInputStruct(pInData);
	if (ret != 0)
		return ret;

	ret = CalculateSRSAAM(type);
	if (ret != 0)
		return ret;

	ret = GetOutputStruct(pOutData);
	return ret;
}