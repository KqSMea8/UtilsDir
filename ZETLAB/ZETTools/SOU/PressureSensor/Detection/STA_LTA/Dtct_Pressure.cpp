//----------------------------------------------------------------------------------------------------------------------
#include <SOU\PressureSensor\Detection\STA_LTA\Dtct_Pressure.h>
#include <mathZet.h>
//----------------------------------------------------------------------------------------------------------------------
void Param_STA_LTA_Pressure::SetSize(int size)
{	
	m_valSta = m_valSta2 = m_valStdDevLta = m_valLta = m_valLta2 = m_valStdDevLta = 0.;
	m_lastLtaOld = m_lastLtaNew = m_lastStaOld = m_lastStaNew = m_lastStaLtaOld4Diff = m_lastStaLtaNew4Diff = 0.f;
	m_valStaLtaDiff = 0.;
	m_lastStaLtaOld4StdDevMean = m_lastStaLtaNew4StdDevMean = 0.f;
	m_numData = 0;
	m_sourceData.SetSize(size);
	m_staLtaData.SetSize(size);
	m_staLtaDiffData.SetSize(size);
	m_staLtaStdDevData.SetSize(size);
	m_staLtaMeanData.SetSize(size);
	m_staData.SetSize(size);
	m_staData2.SetSize(size);
	m_ltaData.SetSize(size);
	m_ltaData2.SetSize(size);
	m_stdDevStaData.SetSize(size);
	m_stdDevLtaData.SetSize(size);
}
//----------------------------------------------------------------------------------------------------------------------
void Param_STA_LTA_Pressure::Zero()
{
	m_valSta = m_valSta2 = m_valStdDevLta = m_valLta = m_valLta2 = m_valStdDevLta = 0.;
	m_lastLtaOld = m_lastLtaNew = m_lastStaOld = m_lastStaNew = m_lastStaLtaOld4Diff = m_lastStaLtaNew4Diff = 0.f;
	m_valStaLtaDiff = 0.;
	m_lastStaLtaOld4StdDevMean = m_lastStaLtaNew4StdDevMean = 0.f;
	m_numData = 0;
	m_sourceData.Zero();
	m_staLtaData.Zero();
	m_staLtaDiffData.Zero();
	m_staLtaStdDevData.Zero();
	m_staLtaMeanData.Zero();
	m_staData.Zero();
	m_staData2.Zero();
	m_ltaData.Zero();
	m_ltaData2.Zero();
	m_stdDevStaData.Zero();
	m_stdDevLtaData.Zero();
}
//----------------------------------------------------------------------------------------------------------------------
bool Param_STA_LTA_Pressure::NewData(const float *pData, const int sizePage, const int numSTA, const int numLTA,
	const double normFactor)
{
	bool ret =	(pData != nullptr) &&
				(sizePage < m_sourceData.GetSize()) &&
				(sizePage < m_staLtaData.GetSize()) &&
				(sizePage < m_staLtaDiffData.GetSize()) &&
				(sizePage < m_staLtaStdDevData.GetSize()) &&
				(sizePage < m_staLtaMeanData.GetSize()) &&
				(sizePage < m_staData.GetSize()) &&
				(sizePage < m_staData2.GetSize()) &&
				(sizePage < m_ltaData.GetSize()) &&
				(sizePage < m_ltaData2.GetSize()) &&
				(sizePage < m_stdDevStaData.GetSize()) &&
				(sizePage < m_stdDevLtaData.GetSize()) &&
				m_sourceData.ShiftData(sizePage) &&
				/*m_staLtaData.ShiftData(sizePage, numSTA) &&
				m_staData.ShiftData(sizePage, numSTA) &&
				m_ltaData.ShiftData(sizePage, numSTA) &&
				m_stdDevStaData.ShiftData(sizePage, numSTA) &&
				m_stdDevLtaData.ShiftData(sizePage, numSTA) &&*/
				m_staLtaData.ShiftData(sizePage) &&
				m_staLtaDiffData.ShiftData(sizePage) &&
				m_staLtaStdDevData.ShiftData(sizePage) &&
				m_staLtaMeanData.ShiftData(sizePage) &&
				m_staData.ShiftData(sizePage) &&
				m_staData2.ShiftData(sizePage) &&
				m_ltaData.ShiftData(sizePage) &&
				m_ltaData2.ShiftData(sizePage) &&
				m_stdDevStaData.ShiftData(sizePage) &&
				m_stdDevLtaData.ShiftData(sizePage) &&
				(ippsCopy_32f(pData, m_sourceData.GetFirstNewPoint(), sizePage) == ippStsNoErr);

	m_numData += sizePage;

	return ret;
}
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
CDetector_Pressure::CDetector_Pressure() : CCustomDetectorPressure()
{}
//----------------------------------------------------------------------------------------------------------------------
CDetector_Pressure::~CDetector_Pressure()
{}
//----------------------------------------------------------------------------------------------------------------------
bool CDetector_Pressure::Create(Lds_cfg_Detector_Pressure_ns::Proxy & proxy)
{
	bool ret = CCustomDetectorPressure::Create(proxy);
	if (ret)
	{
		m_numSTA = (int)floor(m_cfg.timeSTA * m_cfg.freqADC);
		m_numLTA = (int)floor(m_cfg.timeLTA * m_cfg.freqADC);
		m_counterInitMax = m_sizePreHistory + m_numLTA;
		m_sizeEventMin = (int)floor(m_cfg.minDurationEventSTA_LTA * m_cfg.freqADC);
		m_normFactor = m_cfg.freqADC;
		m_alfa = 2.f / (m_numLTA + 1);
		m_1_alfa = 1 - m_alfa;
		m_envelope = 0.f;

		int sizeBuff = 2 * (m_sizePreHistory +  m_numSTA) + m_numLTA;
		if (nullptr != m_pParamStaLtaPressure)
			m_pParamStaLtaPressure->SetSize(sizeBuff);

		m_buff_StdDev.SetSize(sizeBuff);

		ret = ztOpenStructure((ZetHeader*)&m_resSTA,
			ZETLAB_LDS_RES_DETECTOR_PRESSURE_TYPE, ZETLAB_LDS_RES_DETECTOR_PRESSURE_SIZE) == 0;

		DWORD num = (m_numLTA + 1) / 2;
		m_maStDev.Init(num);
	}
	return ret;
}
//----------------------------------------------------------------------------------------------------------------------
void CDetector_Pressure::ZeroData()
{
	CCustomDetectorPressure::ZeroData();
	m_envelope = 0.f;

	if (nullptr != m_pParamStaLtaPressure)
		m_pParamStaLtaPressure->Zero();
	
	m_buff_StdDev.Zero();

	m_maStDev.ReStart();
}
//----------------------------------------------------------------------------------------------------------------------
float* CDetector_Pressure::GetPointerForDataStaLta()
{
	return (/*GetFlagStartMode() || */(nullptr == m_pParamStaLtaPressure)) ? nullptr 
		: m_pParamStaLtaPressure->m_staLtaData.GetGlobalAddres();
}
//----------------------------------------------------------------------------------------------------------------------
float* CDetector_Pressure::GetPointerForDataStaLtaDiff()
{
	return (/*GetFlagStartMode() || */(nullptr == m_pParamStaLtaPressure)) ? nullptr
		: m_pParamStaLtaPressure->m_staLtaDiffData.GetGlobalAddres();
}
//----------------------------------------------------------------------------------------------------------------------
float* CDetector_Pressure::GetPointerForDataStaLtaStdDev()
{
	return (/*GetFlagStartMode() || */(nullptr == m_pParamStaLtaPressure)) ? nullptr
		: m_pParamStaLtaPressure->m_staLtaStdDevData.GetGlobalAddres();
}
//----------------------------------------------------------------------------------------------------------------------
float* CDetector_Pressure::GetPointerForDataStaLtaMean()
{
	return (/*GetFlagStartMode() || */(nullptr == m_pParamStaLtaPressure)) ? nullptr
		: m_pParamStaLtaPressure->m_staLtaMeanData.GetGlobalAddres();
}
//----------------------------------------------------------------------------------------------------------------------
int CDetector_Pressure::GetSizeForDataStaLta()
{
	return (/*GetFlagStartMode() || */(nullptr == m_pParamStaLtaPressure)) ? 0
		: m_pParamStaLtaPressure->m_staLtaData.GetSize();
}
//----------------------------------------------------------------------------------------------------------------------
float* CDetector_Pressure::GetPointerForDataAmpl()
{
	return (/*GetFlagStartMode() || */(nullptr == m_pParamStaLtaPressure)) ? nullptr
		: m_pParamStaLtaPressure->m_sourceData.GetGlobalAddres();
}
//----------------------------------------------------------------------------------------------------------------------
float* CDetector_Pressure::GetPointerForDataStdDev()
{
	return /*GetFlagStartMode() ? nullptr : */m_buff_StdDev.GetGlobalAddres();
}
//----------------------------------------------------------------------------------------------------------------------
unsigned long long CDetector_Pressure::GetNumData()
{
	return (/*GetFlagStartMode() || */(nullptr == m_pParamStaLtaPressure)) ? 0
		: m_pParamStaLtaPressure->m_numData;
}
//----------------------------------------------------------------------------------------------------------------------
bool CDetector_Pressure::Calculation(const float *pData, const int sizePage)
{	
	bool ret = CCustomDetectorPressure::Calculation(pData, sizePage) && nullptr != m_pParamStaLtaPressure;
	if (ret)
	{
		//ret = m_buff_StdDev.ShiftData(sizePage, m_numSTA);
		ret = m_buff_StdDev.ShiftData(sizePage);
		if (ret)
		{
			ret = m_pParamStaLtaPressure->NewData(pData, sizePage, m_numSTA, m_numLTA, m_normFactor);
			if (ret)
			{
				float* pStaLtaVal = m_pParamStaLtaPressure->m_staLtaData.GetFirstNewPoint();
				float* pStaLtaDiffVal = m_pParamStaLtaPressure->m_staLtaDiffData.GetFirstNewPoint();
				float* pStaLtaStdDevVal = m_pParamStaLtaPressure->m_staLtaStdDevData.GetFirstNewPoint();
				float* pStaLtaMeanVal = m_pParamStaLtaPressure->m_staLtaMeanData.GetFirstNewPoint();
				float* pStaVal = m_pParamStaLtaPressure->m_staData.GetFirstNewPoint();
				float* pSta2Val = m_pParamStaLtaPressure->m_staData2.GetFirstNewPoint();
				float* pLtaVal = m_pParamStaLtaPressure->m_ltaData.GetFirstNewPoint();
				float* pLta2Val = m_pParamStaLtaPressure->m_ltaData2.GetFirstNewPoint();
				float* pStdDevStaVal = m_pParamStaLtaPressure->m_stdDevStaData.GetFirstNewPoint();
				float* pStdDevLtaVal = m_pParamStaLtaPressure->m_stdDevLtaData.GetFirstNewPoint();
				float *pStdDev = m_buff_StdDev.GetFirstNewPoint() - 1;

				// работа с событиями по обобщенной огибающей
				bool bEvent = m_bEvent;
				float maxStaLtaVal(NAN), maxStaLtaDiffVal(NAN),
					  maxStaVal(NAN), maxLtaVal(NAN), 
					  maxStdDevStaVal(NAN), maxStdDevLtaVal(NAN), 
					  maxStdDev(NAN), maxEnvelope(0.f);
				for (int i = 0; i < sizePage; ++i)
				{	
					float valSta = *pStaVal++;
					if (0 == _finite(maxStaVal) || abs(maxStaVal) < abs(valSta))
						maxStaVal = valSta;
					float valLta = *pLtaVal++;
					if (0 == _finite(maxLtaVal) || abs(maxLtaVal) < abs(valLta))
						maxLtaVal = valLta;
					float valStdDevSta = *pStdDevStaVal++;
					if (0 == _finite(maxStdDevStaVal) || maxStdDevStaVal < valStdDevSta)
						maxStdDevStaVal = valStdDevSta;
					float valStdDevLta = *pStdDevLtaVal++;
					if (0 == _finite(maxStdDevLtaVal) || maxStdDevLtaVal < valStdDevLta)
						maxStdDevLtaVal = valStdDevLta;
					
					float valStaLtaDiff = *pStaLtaDiffVal++;
					if (0 == _finite(maxStaLtaDiffVal) || maxStaLtaDiffVal < valStaLtaDiff)
						maxStaLtaDiffVal = valStaLtaDiff;

					float valStaLta = (*pStaLtaVal++ - *pStaLtaMeanVal++) / *pStaLtaStdDevVal;
					//float valStaLta = *pStaLtaVal++;
					float signValSta = sign(valStaLta);
					if (0 == _finite(maxStaLtaVal) || abs(maxStaLtaVal) < abs(valStaLta))
						maxStaLtaVal = valStaLta;
					valStaLta = abs(valStaLta);

#ifdef DEBUG
					if (valStaLta > 1)
					{
					//	TRACE(L"valSta %f valLta %f valStdDevSta %f valStdDevLta %f valStaLta %f\n", valSta, valLta, valStdDevSta, valStdDevLta, valStaLta);
					}
#endif // DEBUG
					
					float stdDev = 1.f - valStaLta;
					stdDev = m_maStDev.Calculation(stdDev * stdDev);
					*(++pStdDev) = stdDev;

					stdDev = *pStaLtaStdDevVal++;
					if (0 == _finite(maxStdDev) || maxStdDev < stdDev)
						maxStdDev = stdDev;

					// событие наблюдается
					if (bEvent)
					{
						//if (m_bExtremum)
						m_envelope += log10(valStaLta * m_cfg.factorSTA_LTA);
						if (0 == _finite(maxEnvelope) || maxEnvelope < m_envelope)
							maxEnvelope = m_envelope;
						// событие завершилось
						if (m_envelope <= 0.f)
						{
							bEvent = false;
							m_envelope = 0.f;
						}
					}
					// нет события
					else
					{
						// событие началось
						if (valStaLta >= m_threshold && signValSta < 0.)
						{
							bEvent = true;
							m_envelope = 0.f;
							m_sizeEvent = 0;
						}
					}
				}

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
							if (num <= m_pParamStaLtaPressure->m_staLtaData.GetSize())
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
					}
				}
				m_resSTA.peakSignal = maxStaLtaVal;
				m_resSTA.peakStDev = maxStdDev;
				m_resSTA.peakEnvelope = maxEnvelope;
				m_resSTA.valueLta = maxLtaVal;
				m_resSTA.valueSta = maxStaVal;
				m_resSTA.stdDevLta = maxStdDevLtaVal;
				m_resSTA.stdDevSta = maxStdDevStaVal;
				m_resSTA.peakStaLtaDiff = maxStaLtaDiffVal;
				//TRACE(L"%I64d) valSta %f valLta %f valStdDevSta %f valStdDevLta %f valStaLta %f\n", m_pParamStaLtaPressure->m_numData, maxStaVal, maxLtaVal, maxStdDevStaVal, maxStdDevLtaVal, maxStaLtaVal);
			}
		}
	}

	return ret;
}
//----------------------------------------------------------------------------------------------------------------------