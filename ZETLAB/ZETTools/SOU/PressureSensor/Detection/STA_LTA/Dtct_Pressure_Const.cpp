//----------------------------------------------------------------------------------------------------------------------
#include <SOU\PressureSensor\Detection\STA_LTA\Dtct_Pressure_Const.h>
//----------------------------------------------------------------------------------------------------------------------
void Param_STA_LTA_Pressure_Const::SetSize(int size)
{
	Param_STA_LTA_Pressure::SetSize(size);
	m_sumYSta = m_sumY2Sta = m_sumYLta = m_sumY2Lta =
		m_sumYStaLta4StdDevMean = m_sumY2StaLta4StdDevMean =
		m_sumYStaLta4Diff = m_sumY2StaLta4Diff = m_sumXYStaLta4Diff = 0.;
}
//----------------------------------------------------------------------------------------------------------------------
void Param_STA_LTA_Pressure_Const::Zero()
{
	Param_STA_LTA_Pressure::Zero();
	m_sumYSta = m_sumY2Sta = m_sumYLta = m_sumY2Lta =
		m_sumYStaLta4StdDevMean = m_sumY2StaLta4StdDevMean =
		m_sumYStaLta4Diff = m_sumY2StaLta4Diff = m_sumXYStaLta4Diff = 0.;
}
//----------------------------------------------------------------------------------------------------------------------
bool Param_STA_LTA_Pressure_Const::NewData(const float *pData, const int sizePage, const int numSta,
	const int numLta, const double normFactor)
{
	bool ret = Param_STA_LTA_Pressure::NewData(pData, sizePage, numSta, numLta, normFactor);

	if (ret)
	{	
		float* p = m_sourceData.GetFirstNewPoint();
		float* pStartLta = p - numLta - numSta;
		float* pStartSta = p - numSta;
		float* pEndLta = p - numSta;
		float* pEndSta = p;

		float* pDataStaLta = m_staLtaData.GetFirstNewPoint();
		float* pStartStaLta4StdDevMean = pDataStaLta - numSta * 2;
		float* pEndStaLta4StdDevMean = pDataStaLta;
		float* pStartStaLta4Diff = pDataStaLta - numSta / 4;
		float* pEndStaLta4Diff = pDataStaLta;
		float* pDataStaLtaDiff = m_staLtaDiffData.GetFirstNewPoint();
		float* pDataStaLtaStdDev = m_staLtaStdDevData.GetFirstNewPoint();
		float* pDataStaLtaMean = m_staLtaMeanData.GetFirstNewPoint();

		unsigned long long numData = m_numData - sizePage;
		if (numData > numSta + numLta + 1)
			numData = numSta + numLta + 1;

		for (int i = 0; i < sizePage; ++i)
		{	
			if (numData < numSta + numLta + 1)
				++numData;
			// работа с LTA
			int nLta(numLta);
			if (nLta > (int)numData - numSta)
				nLta = (int)numData - numSta;
			
			if (0 != _finite(*pStartLta))
				m_lastLtaOld = *pStartLta++;
			else
				++pStartLta;
			if (0 != _finite(*pEndLta))
				m_lastLtaNew = *pEndLta++;
			else
				++pEndLta;

			if (nLta > 0)
			{
				m_sumYLta += m_lastLtaNew - m_lastLtaOld;
				m_sumY2Lta += m_lastLtaNew * m_lastLtaNew - m_lastLtaOld * m_lastLtaOld;
				m_valLta = m_sumYLta / nLta;
				m_valStdDevLta = pow(abs(m_sumY2Lta / nLta - m_valLta * m_valLta), 0.5);
			}

			// работа с STA
			int nSta(numSta);
			if (nSta > (int)numData)
				nSta = (int)numData;
			
			if (0 != _finite(*pStartSta))
				m_lastStaOld = *pStartSta++;
			else
				++pStartSta;
			if (0 != _finite(*pEndSta))
				m_lastStaNew = *pEndSta++;
			else
				++pEndSta;

			m_sumYSta += m_lastStaNew - m_lastStaOld;
			m_sumY2Sta += m_lastStaNew * m_lastStaNew - m_lastStaOld * m_lastStaOld;
			m_valSta = m_sumYSta / nSta;
			m_valStdDevSta = pow(abs(m_sumY2Sta / nSta - m_valSta * m_valSta), 0.5);
			
			float val = numData > numSta ? (float)((m_valSta - m_valLta) / (sqrt(m_valStdDevSta * m_valStdDevSta + m_valStdDevLta * m_valStdDevLta))) : 0.f;
			*pDataStaLta++ = val;

			//обработка для адаптации
			int nStaLta4StdDevMean(numLta);
			if (nStaLta4StdDevMean > (int)numData)
				nStaLta4StdDevMean = (int)numData;
			double koeffStdDev = ((int)numData < numLta) ? sqrt((double)numLta / (double)(numLta - (int)numData)) : 1.;

			if (0 != _finite(*pStartStaLta4StdDevMean))
				m_lastStaLtaOld4StdDevMean = *pStartStaLta4StdDevMean++;
			else
				++pStartStaLta4StdDevMean;
			if (0 != _finite(*pEndStaLta4StdDevMean))
				m_lastStaLtaNew4StdDevMean = *pEndStaLta4StdDevMean++;
			else
				++pEndStaLta4StdDevMean;

			m_sumY2StaLta4StdDevMean += m_lastStaLtaNew4StdDevMean * m_lastStaLtaNew4StdDevMean -
				m_lastStaLtaOld4StdDevMean * m_lastStaLtaOld4StdDevMean;
			m_sumYStaLta4StdDevMean += m_lastStaLtaNew4StdDevMean - m_lastStaLtaOld4StdDevMean;

			val = (float)m_sumYStaLta4StdDevMean / nStaLta4StdDevMean / (float)koeffStdDev;
			*pDataStaLtaMean++ = val;
			val = /*bInit ? */pow((float)m_sumY2StaLta4StdDevMean / nStaLta4StdDevMean - val * val, 0.5f) * (float)koeffStdDev;/* : 0.f*/;
			*pDataStaLtaStdDev++ = val == 0.f ? 1.f : val;

			//обработка для поиска производной
			int nSta4(nSta / 4);
			if (nSta4 < 1)
				nSta4 = 1;
			int nSta41 = nSta4 - 1;

			double sumXStaLta = (double)nSta4 * (double)nSta41 / 2. / normFactor;
			double sumX2StaLta = (((double)nSta41 * (double)nSta41 * (double)nSta41 + (double)nSta41 * (double)nSta41) /
				normFactor / normFactor + sumXStaLta / normFactor) / 3.;
			double kStaLta = sumX2StaLta * nSta4 - sumXStaLta * sumXStaLta;

			if (0 != _finite(*pStartStaLta4Diff))
				m_lastStaLtaOld4Diff = *pStartStaLta4Diff++;
			else
				++pStartStaLta4Diff;
			if (0 != _finite(*pEndStaLta4Diff))
				m_lastStaLtaNew4Diff = *pEndStaLta4Diff++;
			else
				++pEndStaLta4Diff;

			m_sumXYStaLta4Diff += (double)m_lastStaLtaNew4Diff * (double)nSta41 / normFactor;
			if (numData > nSta4)
				m_sumXYStaLta4Diff -= (m_sumYStaLta4Diff - m_lastStaLtaOld4Diff) / normFactor;
			m_sumY2StaLta4Diff += m_lastStaLtaNew4Diff * m_lastStaLtaNew4Diff -
				m_lastStaLtaOld4Diff * m_lastStaLtaOld4Diff;
			m_sumYStaLta4Diff += m_lastStaLtaNew4Diff - m_lastStaLtaOld4Diff;
			m_valStaLtaDiff = (m_sumXYStaLta4Diff * nSta4 - sumXStaLta * m_sumYStaLta4Diff) / kStaLta;
			val = /*bInit ? */(float)m_valStaLtaDiff/* : 0.f*/;
			*pDataStaLtaDiff++ = val;

		}
	}
	return ret;
}
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
CDetector_Pressure_Const::CDetector_Pressure_Const() : CDetector_Pressure()
{
	m_pParamStaLtaPressure = new Param_STA_LTA_Pressure_Const;
}
//----------------------------------------------------------------------------------------------------------------------
CDetector_Pressure_Const::~CDetector_Pressure_Const()
{
	if (nullptr != m_pParamStaLtaPressure)
	{
		delete m_pParamStaLtaPressure;
		m_pParamStaLtaPressure = nullptr;
	}
}
//----------------------------------------------------------------------------------------------------------------------