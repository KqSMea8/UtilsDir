//----------------------------------------------------------------------------------------------------------------------
#include <SOU\PressureSensor\Detection\STA_LTA\Dtct_Pressure_StaLta.h>
//----------------------------------------------------------------------------------------------------------------------
void Param_STA_LTA_Pressure_StaLta::SetSize(int size)
{
	Param_STA_LTA_Pressure::SetSize(size);
	m_sumYSta = m_sumYLta = 0.;
}
//----------------------------------------------------------------------------------------------------------------------
void Param_STA_LTA_Pressure_StaLta::Zero()
{
	Param_STA_LTA_Pressure::Zero();
	m_sumYSta = m_sumYLta = 0.;
}
//----------------------------------------------------------------------------------------------------------------------
bool Param_STA_LTA_Pressure_StaLta::NewData(const float *pData, const int sizePage, const int numSta,
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
				m_lastLtaOld = abs(*pStartLta++);
			else
				++pStartLta;
			if (0 != _finite(*pEndLta))
				m_lastLtaNew = abs(*pEndLta++);
			else
				++pEndLta;

			if (nLta > 0)
			{
				m_sumYLta += m_lastLtaNew - m_lastLtaOld;
				m_valLta = m_sumYLta / nLta;
			}

			// работа с STA
			int nSta(numSta);
			if (nSta > (int)numData)
				nSta = (int)numData;
			
			if (0 != _finite(*pStartSta))
				m_lastStaOld = abs(*pStartSta++);
			else
				++pStartSta;
			if (0 != _finite(*pEndSta))
				m_lastStaNew = abs(*pEndSta++);
			else
				++pEndSta;

			m_sumYSta += m_lastStaNew - m_lastStaOld;
			m_valSta = m_sumYSta / nSta;
						
			float val = numData > numSta ? (float)(m_valSta / m_valLta) : 0.f;
			*pDataStaLta++ = val;

			*pDataStaLtaMean++ = 0.f;
			*pDataStaLtaStdDev++ = 1.f;
		}
	}
	return ret;
}
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
CDetector_Pressure_StaLta::CDetector_Pressure_StaLta() : CDetector_Pressure()
{
	m_pParamStaLtaPressure = new Param_STA_LTA_Pressure_StaLta;
}
//----------------------------------------------------------------------------------------------------------------------
CDetector_Pressure_StaLta::~CDetector_Pressure_StaLta()
{
	if (nullptr != m_pParamStaLtaPressure)
	{
		delete m_pParamStaLtaPressure;
		m_pParamStaLtaPressure = nullptr;
	}
}
//----------------------------------------------------------------------------------------------------------------------