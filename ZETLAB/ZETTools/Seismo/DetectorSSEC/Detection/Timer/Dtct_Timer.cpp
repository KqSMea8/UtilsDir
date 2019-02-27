//------------------------------------------------------------------------------
// Файл Timer.cpp
//------------------------------------------------------------------------------
//		Детектор по таймеру.
//--------------------------------------------------------------------------
#include "stdafx.h"
#include <Seismo\DetectorSSEC\Detection\Timer\Dtct_Timer.h>
//------------------------------------------------------------------------------
CDetector_Timer::CDetector_Timer() : CCustomDetector()
{}
//------------------------------------------------------------------------------
bool CDetector_Timer::Create(Ssm_cfg_Detector_STA_ns::Proxy & proxy)
{
	bool ret = CCustomDetector::Create(proxy);
	if (ret)
	{
		m_counterInitMax = m_sizePreHistory + m_sizeEvent;
	}
	return ret;
}
//------------------------------------------------------------------------------
bool CDetector_Timer::Calculation(
	const float *pDataX, const float *pDataY,
	const float *pDataZ, const int sizePage,
	bool bNeedPeakValue)
{
	m_bEvent = false;
	bool ret = m_saAmpl.ReSize(sizePage);
	ret &= bNeedPeakValue ? CCustomDetector::Calculation(
		pDataX, pDataY, pDataZ, sizePage) : true;

	if (ret)
	{
		if (bNeedPeakValue)
		{
			ret =
				CalcAmpl_32f(pDataX, pDataY, pDataZ, m_saAmpl.pData, sizePage, true,
				m_saTmpXYZ[0].pData, m_saTmpXYZ[1].pData, m_saTmpXYZ[2].pData) &&
				(ippsMax_32f(m_saAmpl.pData, sizePage, m_peakXYZA + 3) == ippStsNoErr);
		}
		if (ret && m_bInit)
		{
			m_bInterval = true;
			m_result.sizePage = sizePage;
			m_result.sizePreHistory = m_sizePreHistory;
			m_result.sizeArrays = m_sizePreHistory + m_sizeEvent;
			m_result.timeSRV_lastKadr = 0.;		// позже
		}
	}
	return ret;
}
//------------------------------------------------------------------------------