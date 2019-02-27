//------------------------------------------------------------------------------
// Файл STA_LTA.cpp
//------------------------------------------------------------------------------
//		Детектор STA_LTA.
//------------------------------------------------------------------------------
#include "stdafx.h"
#include <Seismo\DetectorSSEC\Detection\STA_LTA\Dtct_STA_LTA.h>
//------------------------------------------------------------------------------
void Param_STA_LTA_XYZA::SetSize(int size)
{	// Zero вызываетчся в SetSize
	m_valSTA = m_valLTA = 0.;
	m_lastLtaOld = m_lastLtaNew = m_lastStaOld = m_lastStaNew = 0.f;
	m_buff_data.SetSize(size);
	m_buff_StaLta.SetSize(size);
}
//------------------------------------------------------------------------------
void Param_STA_LTA_XYZA::Zero()
{
	m_valSTA = m_valLTA = 0.;
	m_lastLtaOld = m_lastLtaNew = m_lastStaOld = m_lastStaNew = 0.f;
	m_buff_data.Zero();
	m_buff_StaLta.Zero();
}
//------------------------------------------------------------------------------
bool Param_STA_LTA_XYZA::NewData(const float *pData, const int sizePage,
	const int numSTA, const int numLTA,
	const double normFactor, const bool bInit)
{
	bool ret =	// проверки ------------------------------------------------
		(pData != nullptr) &&
		(sizePage < m_buff_data.GetSize()) &&
		(sizePage < m_buff_StaLta.GetSize()) &&
		// сдвиг данных в буферах, определение текущего значения m_pFirstNewPoint
		m_buff_data.ShiftData(sizePage) &&
		m_buff_StaLta.ShiftData(sizePage, numSTA) &&
		// копирование в буфер исходных данных (которые хранятся по модулю)
		(ippsCopy_32f(pData, m_buff_data.GetFirstNewPoint(), sizePage) == ippStsNoErr) &&
		(ippsAbs_32f_I(m_buff_data.GetFirstNewPoint(), sizePage) == ippStsNoErr);

	if (ret)
	{	// расчет значений сигнала детектора STA/OLTA
		float *pointer = m_buff_data.GetFirstNewPoint() - numSTA;
		float *pStart_LTAold = pointer - numLTA + 1;
		float *pStart_STAold = pointer;
		float *pEnd_LTAnew   = pointer + 1;
		float *pEnd_STAnew   = pointer + numSTA;

		pointer = m_buff_StaLta.GetFirstNewPoint();

		for (int i = 0; i < sizePage; i++)
		{	// расчёты сумм амплитуд ---------------
			if (0 != _finite(*pStart_LTAold))
				m_lastLtaOld = *pStart_LTAold++;
			if (0 != _finite(*pEnd_LTAnew))
				m_lastLtaNew = *pEnd_LTAnew++;
			m_valLTA += m_lastLtaNew - m_lastLtaOld;
			if (0 != _finite(*pStart_STAold))
				m_lastStaOld = *pStart_STAold++;
			if (0 != _finite(*pEnd_STAnew))
				m_lastStaNew = *pEnd_STAnew++;
			m_valSTA += m_lastStaNew - m_lastStaOld;
			float val = ((m_valLTA > 0.) && bInit) ?
				(float)(m_valSTA / m_valLTA * normFactor) : 1.f;
			*pointer++ = val;
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
CDetector_STA_LTA::CDetector_STA_LTA() : CCustomDetector()
{}
//------------------------------------------------------------------------------
CDetector_STA_LTA::~CDetector_STA_LTA()
{}
//------------------------------------------------------------------------------
bool CDetector_STA_LTA::Create(Ssm_cfg_Detector_STA_ns::Proxy & proxy)
{
	bool ret = CCustomDetector::Create(proxy);
	if (ret)
	{
		m_numSTA = (int)floor(m_cfg.timeSTA * m_cfg.freqADC);
		m_numLTA = (int)floor(m_cfg.timeLTA * m_cfg.freqADC);
		m_counterInitMax = m_sizePreHistory +  m_numSTA + m_numLTA;
		m_sizeEventMin = (int)floor(m_cfg.minDurationEventSTA_LTA * m_cfg.freqADC);
		m_normFactor = (double)m_numLTA / (double)m_numSTA;
		m_alfa = 2.f / (m_numLTA + 1);
		m_1_alfa = 1 - m_alfa;
		m_envelope = 0.f;

		int sizeBuff = 2 * (m_sizePreHistory +  m_numSTA) + m_numLTA;
		m_buffXYZA[0].SetSize(sizeBuff);
		m_buffXYZA[1].SetSize(sizeBuff);
		m_buffXYZA[2].SetSize(sizeBuff);
		m_buffXYZA[3].SetSize(sizeBuff);
		m_buff_StDev.SetSize(sizeBuff);

		ret = ztOpenStructure((ZetHeader*)&m_resSTA,
			ZETLAB_SSM_RES_DETECTOR_STA_TYPE, ZETLAB_SSM_RES_DETECTOR_STA_SIZE) == 0;

		DWORD num = (m_numLTA + 1) / 2;
		m_maStDev.Init(num);
	}
	return ret;
}
//------------------------------------------------------------------------------
void CDetector_STA_LTA::ZeroData()
{
	CCustomDetector::ZeroData();
	m_envelope = 0.f;

	m_buffXYZA[0].Zero();
	m_buffXYZA[1].Zero();
	m_buffXYZA[2].Zero();
	m_buffXYZA[3].Zero();
	m_buff_StDev.Zero();

	m_maStDev.ReStart();
}
//------------------------------------------------------------------------------
float* CDetector_STA_LTA::GetPointerForDataStaLta()
{
	return GetFlagStartMode() ? nullptr :
		m_buffXYZA[3].m_buff_StaLta.GetGlobalAddres();
}
//------------------------------------------------------------------------------
float* CDetector_STA_LTA::GetPointerForDataAmpl()
{
	return GetFlagStartMode() ? nullptr :
		m_buffXYZA[3].m_buff_data.GetGlobalAddres();
}
//------------------------------------------------------------------------------
float* CDetector_STA_LTA::GetPointerForDataStDev()
{
	return GetFlagStartMode() ? nullptr :
		m_buff_StDev.GetGlobalAddres();
}
//----------------------------------------------------------------------------------------------------------------------
int CDetector_STA_LTA::GetSizeForDataStaLta()
{
	return GetFlagStartMode() ? 0 : 
		m_buffXYZA[3].m_buff_StaLta.GetSize();
}
//------------------------------------------------------------------------------
bool CDetector_STA_LTA::Calculation(
	const float *pDataX, const float *pDataY,
	const float *pDataZ, const int sizePage,
	bool bNeedPeakValue)
{	// одноимённая функция базового класса:
	// проверка указателей, расчёт пиковых значений по компонентам и
	// определение m_bInit 
	bool ret = (m_bChanX || m_bChanY || m_bChanZ) &&
		CCustomDetector::Calculation(
		pDataX, pDataY, pDataZ, sizePage, bNeedPeakValue);
	if (ret)
	{// вычисления
		ret =	// сдвиг в буферах амплитуды и общего детектора
			m_buffXYZA[3].m_buff_data.ShiftData(sizePage) &&
			m_buffXYZA[3].m_buff_StaLta.ShiftData(sizePage, m_numSTA) &&
			m_buff_StDev.ShiftData(sizePage, m_numSTA) &&
			// вспомогательные массивы при расчёте амплитуды
			m_saTmpXYZ[0].ReSize(sizePage) &&
			m_saTmpXYZ[1].ReSize(sizePage) &&
			m_saTmpXYZ[2].ReSize(sizePage);
			// расчет амплитуды и заполнение его буфера
		if (bNeedPeakValue)
		{
			ret &= CalcAmpl_32f(pDataX, pDataY, pDataZ,
				m_buffXYZA[3].m_buff_data.GetFirstNewPoint(), sizePage, true,
				m_saTmpXYZ[0].pData, m_saTmpXYZ[1].pData, m_saTmpXYZ[2].pData) &&
				// пиковое значение амплитуды
				(ippsMax_32f(m_buffXYZA[3].m_buff_data.
				GetFirstNewPoint(), sizePage, m_peakXYZA + 3) == ippStsNoErr);
		}
		if (ret)
		{
			ret =// расчёты в канальных детекторах
				m_buffXYZA[0].NewData(pDataX, sizePage, m_numSTA, m_numLTA, m_normFactor, m_bInit) &&
				m_buffXYZA[1].NewData(pDataY, sizePage, m_numSTA, m_numLTA, m_normFactor, m_bInit) &&
				m_buffXYZA[2].NewData(pDataZ, sizePage, m_numSTA, m_numLTA, m_normFactor, m_bInit);

			if (ret)
			{// общий детектор
				float *px = m_buffXYZA[0].m_buff_StaLta.GetFirstNewPoint();
				float *py = m_buffXYZA[1].m_buff_StaLta.GetFirstNewPoint();
				float *pz = m_buffXYZA[2].m_buff_StaLta.GetFirstNewPoint();
				float *pSta = m_buffXYZA[3].m_buff_StaLta.GetFirstNewPoint();
				float *pStDev = m_buff_StDev.GetFirstNewPoint() - 1;

				// работа с событиями по обобщенной огибающей
				bool bEvent = m_bEvent;
				UINT numChanOk = (m_bChanX ? 1 : 0) + (m_bChanY ? 1 : 0) + (m_bChanZ ? 1 : 0);
				float maxVal(0.f);
				float maxStD(0.f);
				float maxEnv(0.f);
				for (int i = 0; i < sizePage; i++)
				{	// текущее значение сигнала общего детектора
					float val = 0.f;
					if (m_bChanX)
						val += *px++;
					if (m_bChanY)
						val += *py++;
					if (m_bChanZ)
						val += *pz++;
						
					val /= numChanOk;//V609 Divide by zero. Denominator range [0..3]. dtct_sta_lta.cpp 207 не прав, проверка есть
					*pSta++ = val;
					if (maxVal < val)
						maxVal = val;

					// сигнал размаха. храним дисперсию, в структуру пишем СКО
					float stDev = 1.f - val;
					stDev = m_maStDev.Calculation(stDev * stDev);
					*(++pStDev) = stDev;

					if (maxStD < stDev)
						maxStD = stDev;

					if (bEvent)
					{// событие наблюдается
						m_envelope += log10(val * m_cfg.factorSTA_LTA);
						if (maxEnv < m_envelope)
							maxEnv = m_envelope;
						if (m_envelope <= 0.f)
						{// событие завершилось
							bEvent = false;
							m_envelope = 0.f;
						//	m_maStDev.ReStart();
						}
					}
					else
					{// нет события
						if (val >= m_threshold)
						{// событие началось
							bEvent = true;
							m_envelope = 0.f;
							m_sizeEvent = 0;
						}
					}
				}// for (int i = 0; i < sizePage; i++)

				// Что делать дальше?
				SetTypeEvent(bEvent);
				m_bInterval = false;
				if (m_typeEvent != ted_No)
				{
					m_sizeEvent += sizePage;
					if (m_typeEvent == ted_End)
					{// действия при завершении события
						if (m_sizeEvent >= m_sizeEventMin)
						{
							int num = m_sizeEvent + m_sizePreHistory + m_numSTA;
							if (num <= m_buffXYZA[0].m_buff_StaLta.GetSize())
							{// Можно копировать данные в интервал
								m_bInterval = true;
								m_result.sizePage = sizePage;
								m_result.sizePreHistory = m_sizePreHistory;
								m_result.sizeArrays = num;
								m_result.timeSRV_lastKadr = 0.;		// позже
							}
							else
							{
								m_typeEvent = ted_End_LargeTime;
								m_bEvent = false;
								m_envelope = 0.f;
								m_sizeEvent = 0;
							}
						}
						else
						{
							m_typeEvent = ted_End_SmallTime;
							m_bEvent = false;
							m_envelope = 0.f;
							m_sizeEvent = 0;
						}
					}// if (m_typeEvent == ted_End)
				}// if (m_typeEvent != ted_No) 			
				m_resSTA.peakSignal = maxVal;
				m_resSTA.peakStDev = sqrt(maxStD);
				m_resSTA.peakEnvelope = maxEnv;
			}// if (ret)
		}// if (ret)
	}// if (ret)
	return ret;
}
//------------------------------------------------------------------------------