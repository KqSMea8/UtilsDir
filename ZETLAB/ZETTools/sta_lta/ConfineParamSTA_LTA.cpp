//------------------------------------------------------------------------------
// Файл ConfineParamSTA_LTA.cpp
// Версия от 24.09.2012
// ЗАО "ЭТМС", Зеленоград, Савостин Ю.М.
//------------------------------------------------------------------------------
#include "stdafx.h"
#include <sta_lta\ConfineParamSTA_LTA.h>
//------------------------------------------------------------------------------
CConfineParamSTA_LTA::CConfineParamSTA_LTA()
{
	m_min_Div_LTA_STA = 10.f;
	m_max_Div_Sum_Tall = 0.9f;
	m_min_Div_Sta_TimeADC = 10.f;
	m_minThreshold = 1.5f;
	m_maxThreshold = 60.f;
	pFreqADC = NULL;
	pFiltr = NULL;
	bFiltrInternal = true;
	m_minCompressRatio = 400;
	m_maxCompressRatio = 4000;
}
//------------------------------------------------------------------------------
CConfineParamSTA_LTA::~CConfineParamSTA_LTA()
{
	if (bFiltrInternal && pFiltr )
	{
		delete pFiltr;
		pFiltr = nullptr;
	}
}
//------------------------------------------------------------------------------
bool CConfineParamSTA_LTA::Create(
	float *freqADC,
	int sizeMax,
	CIirFiltrBand *pfiltr)
{
	bool ret;
	pFreqADC = freqADC;
	SizeMax = sizeMax;
	ret = pFreqADC != NULL;
	if (ret)
	{
		if (pfiltr)
		{
			bFiltrInternal = false;
			pFiltr = pfiltr;
		}
		else
		{
			bFiltrInternal = true;
			pFiltr = new CIirFiltrBand();
			ret = pFiltr->Create(pFreqADC);
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
float CConfineParamSTA_LTA::PriorValueSTA(float newVal, float valLTA)
{
	float ret;
	float valMin = GetMinSTA();
	float valMax = min(GetMaxSTA(), valLTA / m_min_Div_LTA_STA);
	if (newVal < valMin)
		ret = valMin;
	else if (newVal > valMax)
		ret = valMax;
	else
		ret = newVal;
	return ret;
}
//------------------------------------------------------------------------------
float CConfineParamSTA_LTA::PriorValueLTA(float newVal, float valSTA)
{
	float ret;
	float valMin = max(GetMinLTA(), valSTA * m_min_Div_LTA_STA);
	float valMax = GetMaxLTA();
	if (newVal < valMin)
		ret = valMin;
	else if (newVal > valMax)
		ret = valMax;
	else
		ret = newVal;
	return ret;
}
//------------------------------------------------------------------------------
void CConfineParamSTA_LTA::NewFreqBand(
	float *pFmin,		// значение Fmin
	float *pFmax,		// значение Fmax
	float newFmin,		// новое значение Fmin
	float newFmax,		// новое значение Fmax
	short bNewParam)	// 0 - изменились обе частоты
{						// 1 - изменилась Fmin
	float fmin, fmax;	// 2 - изменилась Fmax
	switch (bNewParam)
	{
	case 1 :
		if (newFmin <= 0.f)
			fmin = 1e-20f;
		else
			fmin = newFmin;
		fmin = pFiltr->PriorValueFmin(fmin);
		if (fmin < *pFmax)
			*pFmin = fmin;
		else
			*pFmin = *pFmax;
		break;

	case 2 :
		if (newFmax <= 0.f)
			fmax = 1e-20f;
		else
			fmax = newFmax;
		fmax = pFiltr->PriorValueFmax(fmax);
		if (fmax > *pFmin)
			*pFmax = fmax;
		else
			*pFmax = *pFmin;
		break;

	default :
		if (newFmin <= 0.f)
			fmin = 1e-20f;
		else
			fmin = newFmin;
		if (newFmax <= 0.f)
			fmax = 1e-20f;
		else
			fmax = newFmax;
		fmin = pFiltr->PriorValueFmin(fmin);
		fmax = pFiltr->PriorValueFmax(fmax);
		if (fmax > fmin)
		{
			*pFmin = fmin;
			*pFmax = fmax;
		}
		else
		{
			*pFmin = fmax;
			*pFmax = fmin;
		}
	}
}
//------------------------------------------------------------------------------
void CConfineParamSTA_LTA::NewDuration(
	float *pSta,		// значение дл. Sta
	float *pLta,		// значение дл. Lta
	float newSta,		// новое значение дл. Sta
	float newLta)		// новое значение дл. Lta
{
	float sta, lta, x1, x2;		// новые значения, которые будут установлены
	if (newLta > newSta)
	{// Длительность Sta более важный параметр, поэтому начинаю с него
		x1 = GetMinSTA(); x2 = GetMaxSTA();
		if (newSta < x1)
			sta = x1;
		else if (newSta > x2)
			sta = x2;
		else
			sta = newSta;

		x1 = PriorValueLTA(newLta, sta); x2 = GetMaxLTA();
		if (newLta < x1)
			lta = x1;
		else if (newLta > x2)
			lta = x2;
		else
			lta = newLta;
	}
	else
	{
		sta = sqrt( GetMinSTA() * GetMaxSTA() );
		lta = sqrt( GetMinLTA() * GetMaxLTA() );
	}
	*pSta = sta;
	*pLta = lta;
}
//------------------------------------------------------------------------------
void CConfineParamSTA_LTA::NewThreshold(
	float *pThreshold,
	float newThreshold)
{
	float val;
	if (newThreshold < m_minThreshold)
		val = m_minThreshold;
	else if (newThreshold > m_maxThreshold)
		val = m_maxThreshold;
	else
		val = newThreshold;
	*pThreshold = val;
}
//------------------------------------------------------------------------------
void CConfineParamSTA_LTA::NewCompressRatio(
	DWORD *pCompressRatio,
	DWORD newVal)
{
	DWORD val, mi, ma;
	mi = max( m_minCompressRatio, DWORD(*pFreqADC) );
	ma = min( 10*mi, m_maxCompressRatio );
	if (ma <= mi)
		ma = 10 * mi;

	if (newVal < mi)
		val = mi;
	else if (newVal > ma)
		val = ma;
	else
		val = newVal;
	*pCompressRatio = val;
}
//------------------------------------------------------------------------------
float CConfineParamSTA_LTA::GetMinSTA()
{
	return m_min_Div_Sta_TimeADC / *pFreqADC;
}
//------------------------------------------------------------------------------
float CConfineParamSTA_LTA::GetMaxSTA()
{
	float Tall = SizeMax / *pFreqADC;
	float Tsum = Tall * m_max_Div_Sum_Tall;
	return Tsum / (1 + m_min_Div_LTA_STA);
}
//------------------------------------------------------------------------------
float CConfineParamSTA_LTA::GetMinLTA()
{
	float Tall = SizeMax / *pFreqADC;
	return m_min_Div_LTA_STA * GetMinSTA();
}
//------------------------------------------------------------------------------
float CConfineParamSTA_LTA::GetMaxLTA()
{
	float Tall = SizeMax / *pFreqADC;
	float Tsum = Tall * m_max_Div_Sum_Tall;
	return Tsum * m_min_Div_LTA_STA / (1 + m_min_Div_LTA_STA);
}
//------------------------------------------------------------------------------