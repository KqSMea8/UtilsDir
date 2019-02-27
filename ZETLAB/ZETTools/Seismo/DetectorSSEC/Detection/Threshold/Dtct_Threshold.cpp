//------------------------------------------------------------------------------
// Файл Threshold.cpp
//------------------------------------------------------------------------------
//		Пороговый детектор.
//--------------------------------------------------------------------------
#include "stdafx.h"
#include <Seismo\DetectorSSEC\Detection\Threshold\Dtct_Threshold.h>
//------------------------------------------------------------------------------
CDetector_Threshold::CDetector_Threshold() : CCustomDetector()
{}
//------------------------------------------------------------------------------
bool CDetector_Threshold::Create(Ssm_cfg_Detector_STA_ns::Proxy & proxy)
{
	bool ret = CCustomDetector::Create(proxy);
	if (ret)
	{
		m_counterInitMax = m_sizePreHistory + m_sizeEvent;
	}
	return ret;
}
//------------------------------------------------------------------------------
void CDetector_Threshold::ZeroDataBuff()
{
	CCustomDetector::ZeroData();
	m_numPageInEventCur = -1;
}
//------------------------------------------------------------------------------
bool CDetector_Threshold::Calculation(
	const float *pDataX, const float *pDataY,
	const float *pDataZ, const int sizePage,
	bool bNeedPeakValue)
{
	bool ret = m_saAmpl.ReSize(sizePage);
	ret &= bNeedPeakValue ? CCustomDetector::Calculation(
		pDataX, pDataY, pDataZ, sizePage) : true;

	if (ret && m_bInit)
	{
		if (bNeedPeakValue)
		{
			ret =
				CalcAmpl_32f(pDataX, pDataY, pDataZ, m_saAmpl.pData, sizePage, true,
				m_saTmpXYZ[0].pData, m_saTmpXYZ[1].pData, m_saTmpXYZ[2].pData) &&
				(ippsMax_32f(m_saAmpl.pData, sizePage, m_peakXYZA + 3) == ippStsNoErr);
		}
		// Определение текущего состояния события
		SetTypeEvent(m_peakXYZA[3] >= m_threshold);

		// счётчик m_numPageInEventCur
		if (m_numPageInEventCur == -1)
		{
			if (m_typeEvent == ted_Start)
			{
				m_numPageInEventMax = m_sizeEvent / sizePage;
				m_numPageInEventCur = 0;
			}
		}
		else
		{
			++m_numPageInEventCur;
		}

		// По m_numPageInEventCur определяю m_bInterval
		if (m_numPageInEventCur >= 0)
		{
			m_bInterval = m_numPageInEventCur == m_numPageInEventMax;
			if (m_bInterval && m_bInit)
			{// Можно копировать данные в интервал
				m_result.sizePage = sizePage;
				m_result.sizePreHistory = m_sizePreHistory;
				m_result.sizeArrays = sizePage + m_sizePreHistory + m_sizeEvent;
				m_result.timeSRV_lastKadr = 0.;		// позже
				m_numPageInEventCur = -1;
			}
		}
		else
		{
			m_bInterval = false;
			m_numPageInEventCur = -1;
		}
	}
	return ret;
}
//------------------------------------------------------------------------------