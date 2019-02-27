#include "stdafx.h"
#include "ZetCalcCorrelation.h"
#include <ipp.h>
// #include <Intel\ipp\ipps.h>
// #include <Intel\ipp\ippvm.h>
// #include <Intel\ipp\ippcore.h>


CZetCalcCorrelation::CZetCalcCorrelation()
	: m_lSize1(0)
	, m_lSize2(0)
	, m_lTempSize(0)
	, m_fData1(nullptr)
	, m_fData2(nullptr)
	, m_fTempData(nullptr)
{}

CZetCalcCorrelation::~CZetCalcCorrelation()
{

	if (m_fData1 != nullptr)
		delete[] m_fData1;
	if (m_fData2 != nullptr)
		delete[] m_fData2;
	if (m_fTempData != nullptr)
		delete[] m_fTempData;
	m_fData1 = nullptr;
	m_fData2 = nullptr;
	m_fTempData = nullptr;
}


void CZetCalcCorrelation::InitClass(int size)
{
	if (size <= 0)
		return;

	if (size != m_lSize1)
	{
		m_lSize1 = size;
		m_lSize2 = size;
		m_lTempSize = size * 2 + 1;

		if (m_fData1 != nullptr)
			delete[] m_fData1;
		m_fData1 = new float[m_lSize1];

		if (m_fData2 != nullptr)
			delete[] m_fData2;
		m_fData2 = new float[m_lSize2];

		if (m_fTempData != nullptr)
			delete[] m_fTempData;
		m_fTempData = new float[m_lTempSize];
	}
}

float CZetCalcCorrelation::Coefficient(float* data1, float* data2)
{
	IppStatus status;
	float mean1;
	float mean2;
	float sum1;
	float sum21;
	float sum22;
	float coef;

	status = ippsCopy_32f(data1, m_fData1, m_lSize1);
	status = ippsCopy_32f(data2, m_fData2, m_lSize2);
	
	status = ippsMean_32f(m_fData1, m_lSize1, &mean1, ippAlgHintAccurate);
	status = ippsMean_32f(m_fData2, m_lSize2, &mean2, ippAlgHintAccurate);

	status = ippsSubC_32f_I(mean1, m_fData1, m_lSize1);
	status = ippsSubC_32f_I(mean2, m_fData2, m_lSize2);

	status = ippsMul_32f(m_fData1, m_fData2, m_fTempData, m_lSize1);
	status = ippsSum_32f(m_fTempData, m_lSize1, &sum1, ippAlgHintAccurate);

	status = ippsSqr_32f_I(m_fData1, m_lSize1);
	status = ippsSqr_32f_I(m_fData2, m_lSize2);

	status = ippsSum_32f(m_fData1, m_lSize1, &sum21, ippAlgHintAccurate);
	status = ippsSum_32f(m_fData2, m_lSize2, &sum22, ippAlgHintAccurate);

	coef = sum1 / sqrtf(sum21 * sum22);

	return coef;
}