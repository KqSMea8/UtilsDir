//----------------------------------------------------------------------
#include "StdAfx.h"
#include <ippFiltr\MovingAverageFiltr.h>
//----------------------------------------------------------------------
CMovingAverageFiltr::CMovingAverageFiltr()
{
	m_FType = tfMovingAverage;
	m_bInit = false;
	m_pData = NULL;
	m_sizeData =
	m_sizeFiltr = m_sizeFiltr_2 = 0;
	m_pFreqADC = NULL;
}
//----------------------------------------------------------------------
CMovingAverageFiltr::~CMovingAverageFiltr()
{
	if (m_pData)
	{ 
		delete [] m_pData;
		m_pData = nullptr;
	}
}
//----------------------------------------------------------------------
bool CMovingAverageFiltr::Create(
	float *pfreqADC)	// указатель на частоту дискретизации АЦП
{
	m_pFreqADC = pfreqADC;
	return *m_pFreqADC > 0.f;
}
//----------------------------------------------------------------------
int CMovingAverageFiltr::GetLengthInPoint()
{
	return m_sizeFiltr;
}
//----------------------------------------------------------------------
float CMovingAverageFiltr::GetLengthInTime()
{
	float ret;
	if (m_pFreqADC)
		ret = *m_pFreqADC * m_sizeFiltr;
	else
		ret = 0;
	return ret;
}
//----------------------------------------------------------------------
bool CMovingAverageFiltr::_InitFiltr(int lenght)
{
	bool ret = lenght > 2;
	if (ret)
	{
		m_sizeFiltr = lenght | 1;
		m_sizeFiltr_2 = m_sizeFiltr / 2;
		m_bInit = true;
	}
	return ret;
}
//----------------------------------------------------------------------
bool CMovingAverageFiltr::InitFiltrInPoint(int lenght)
{
	return _InitFiltr(lenght);
}
//----------------------------------------------------------------------
bool CMovingAverageFiltr::InitFiltrInTime(float lenTime)
{
	bool ret = (m_pFreqADC != NULL);
	if (ret)
		ret = _InitFiltr( (int)(*m_pFreqADC * lenTime) );
	return ret;
}
//----------------------------------------------------------------------
bool CMovingAverageFiltr::Filtration(
	float *pInpData,	// указатель на исходные данные
	float *pOutData,	// указатель на отфильтрованные данные
	int num)			// кол-во отсчётов
{
	bool ret = _Filtration(pInpData, num);
	if (ret)
		ippsCopy_32f(m_pData, pOutData, num);
	return ret;
}
//----------------------------------------------------------------------
bool CMovingAverageFiltr::Filtration_I(
	float *pInpOutData,	// указатель на исходные/выходные данные
	int num)			// кол-во отсчётов
{
	bool ret = _Filtration(pInpOutData, num);
	if (ret)
		ippsCopy_32f(m_pData, pInpOutData, num);
	return ret;
}
//----------------------------------------------------------------------
bool CMovingAverageFiltr::_Filtration(
	float *pInpData,	// указатель на исходные данные
	int num)			// кол-во отсчётов
{// Функция фильтрации на все случаи жизни. Результат в массив класса
	bool ret = m_bInit && (num > m_sizeFiltr);
	if (ret)
	{	// проверка размера промежуточного массива
		if (m_sizeData < num)
		{
			if (m_sizeData > 0)
			{ 
				delete [] m_pData;
				m_pData = nullptr;
			}
			m_sizeData = num;
			m_pData = new float [m_sizeData];
		}
		// сама фильтрация
		float mean;
		int size = m_sizeFiltr_2 + 1;
		int i = 0;
		for (; i < m_sizeFiltr_2; ++i)
		{
			ippsMean_32f(pInpData, size++, &mean, ippAlgHintFast);
			m_pData[i] = mean;
		}
		float *psrc = pInpData;
		for (; i < num - m_sizeFiltr; ++i)
		{
			ippsMean_32f(psrc++, size, &mean, ippAlgHintFast);
			m_pData[i] = mean;
		}
		for (; i < num; ++i)
		{
			ippsMean_32f(psrc, --size, &mean, ippAlgHintFast);
			m_pData[i] = mean;
		}
	}
	return ret;
}
//----------------------------------------------------------------------