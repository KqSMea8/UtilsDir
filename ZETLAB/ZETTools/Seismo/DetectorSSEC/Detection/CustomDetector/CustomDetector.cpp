//------------------------------------------------------------------------------
// Файл CustomDetector.cpp
//------------------------------------------------------------------------------
//		Базовый класс детекторов сейсмиических сигналов
//--------------------------------------------------------------------------
#include "stdafx.h"
#include <Seismo\DetectorSSEC\Detection\CustomDetector\CustomDetector.h>
//------------------------------------------------------------------------------
CCustomDetector::CCustomDetector()
{
}
//------------------------------------------------------------------------------
bool CCustomDetector::Create(Ssm_cfg_Detector_STA_ns::Proxy & proxy)
{
	m_cfg = proxy;
	m_sizePreHistory =
			(int)floor(m_cfg.timePreHistory * m_cfg.freqADC);
	m_counterInitMax = 22000;	// будет переопределено в наследниках

	switch (m_cfg.typeDtct)
	{
	case 0:		// STA_LTA
		m_threshold = m_cfg.thresholdForSTA_LTA;
		break;
	case 1:		// Threshold
		m_sizeEvent = (int)floor(m_cfg.timeEventForThreshold * m_cfg.freqADC);
		m_threshold = m_cfg.thresholdForThreshold;
		break;
	case 2:		// Timer
		m_sizeEvent = (int)floor(m_cfg.timeEventForTimer * m_cfg.freqADC);
		break;
	}
	ZeroData();
	return true;
}
//------------------------------------------------------------------------------
void CCustomDetector::ZeroData()
{
	m_bInit = false;
	m_bEvent = false;
	m_bInterval = false;
	m_counterForInit = 0;
}
//------------------------------------------------------------------------------
TypeEventYesNo CCustomDetector::SetTypeEvent(bool bEvent)
{// Определение текущего состояния события
	if (m_bEvent && bEvent)
	{// событие наблюдается
		m_typeEvent = ted_Continue;
	}
	else if (!m_bEvent && bEvent)
	{// событие началось
		m_typeEvent = ted_Start;
	}
	else if (m_bEvent && !bEvent)
	{// событие завершилось
		m_typeEvent = ted_End;
	}
	else//if (!m_bEvent && !bEvent)
	{// нет события
		m_typeEvent = ted_No;
	}
	m_bEvent = bEvent;
	return m_typeEvent;
}
//------------------------------------------------------------------------------
// Выполняются необходимые проверки входных данных, рассчитывается амплитуда и
// новые порции исходных данных и амплитуды заносятся в буферы. Проверяется
// заполненность буферов детектора (его иинициализация).
bool CCustomDetector::Calculation(
	const float *pDataX, const float *pDataY, const float *pDataZ,
	const int sizePage,			// размер порции данных
	bool bNeedPeakValue)
{
	bool ret = (pDataX != nullptr) && (pDataY != nullptr) && (pDataZ != nullptr)
		&& (0 < sizePage);
	if (ret)
	{
		if (bNeedPeakValue)
		{
			GetPeakValue_32f(pDataX, sizePage, m_peakXYZA[0]);
			GetPeakValue_32f(pDataY, sizePage, m_peakXYZA[1]);
			GetPeakValue_32f(pDataZ, sizePage, m_peakXYZA[2]);
		}
		if (!m_bInit)
		{
			m_counterForInit += sizePage;
			m_bInit = m_counterForInit >= m_counterInitMax;
		}
	}
	return ret;
}
//------------------------------------------------------------------------------