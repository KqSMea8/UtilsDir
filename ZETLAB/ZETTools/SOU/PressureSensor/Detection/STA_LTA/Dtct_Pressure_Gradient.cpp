//----------------------------------------------------------------------------------------------------------------------
#include <SOU\PressureSensor\Detection\STA_LTA\Dtct_Pressure_Gradient.h>
//----------------------------------------------------------------------------------------------------------------------
void Param_STA_LTA_Pressure_Gradient::SetSize(int size)
{
	Param_STA_LTA_Pressure::SetSize(size);
	m_sumYSta = m_sumY2Sta = m_sumXYSta = 
		m_sumYLta = m_sumY2Lta = m_sumXYLta = 
		m_sumYStaLta4Diff = m_sumY2StaLta4Diff = m_sumXYStaLta4Diff =
		m_sumYStaLta4StdDevMean = m_sumY2StaLta4StdDevMean = 0.;
}
//----------------------------------------------------------------------------------------------------------------------
void Param_STA_LTA_Pressure_Gradient::Zero()
{
	Param_STA_LTA_Pressure::Zero();
	m_sumYSta = m_sumY2Sta = m_sumXYSta =
		m_sumYLta = m_sumY2Lta = m_sumXYLta =
		m_sumYStaLta4Diff = m_sumY2StaLta4Diff = m_sumXYStaLta4Diff =
		m_sumYStaLta4StdDevMean = m_sumY2StaLta4StdDevMean = 0.;
}
//----------------------------------------------------------------------------------------------------------------------
//										Описание алгоритма вычисления
// В данном алгоритме вычисляется разница между наклоном прямой на коротком (STA) и длинном (LTA) окне усреднения.
// Вычисление коэффициента наклона на каждом из окон усреднения выполняется по методу наименьших квадратов (МНК) 
// однопроходным методом. Для его реализации используются следующие формулы:
//
//										n*sum(X*Y) - sum(X)*sum(Y)
//									k = --------------------------.
//										n*sum(X*X) - sum(X)*sum(X)
//
// Суммы sum(X) и sum(X*X) зависят только от количества точек и могут быть посчитаны один раз. В данном случае разбивка
// по оси абсцисс берется в соответствии с частотой дискретизации, т.е. X0 = 0, X1 = 1 / freq, X2 = 2 / freq,..., 
// XN = (N-1) / freq. Поэтому:
//
//									sum(X) = n * (n - 1) / 2 / freq,
//					sum(X*X) = (((n - 1)^3 + (n - 1)^2) / freq^2 + sum(X) / freq) / 3
//
// При реализации однопроходного метода необходим расчет всех параметров за один проход цикла, наибольшую сложность 
// представляет расчет sum(X*Y). Выведем формулу его расчета через	сумму, вычисленную на N-ном (sum(X*Y)[N]) и 
// N+1 шагах (sum(X*Y)[N+1]):
//
// sum(X*Y)[N] = sum((i-1) / freq * Y[i]) = (1-1) / freq * Y[1] + (2-1) / freq * Y[2] + ... + (N-1) / freq * Y[N],
// sum(X*Y)[N+1] = sum((i-1) / freq * Y[i+1]) = (1-1) / freq * Y[2] + (2-1) / freq * Y[3] + ... + (N-1) / freq * Y[N+1],
// sum(X*Y)[N+1] - sum(X*Y)[N] = (N-1) / freq * Y[N+1] - (sum(Y) - Y[1]) / freq
// sum(X*Y)[N+1] = sum(X*Y)[N] + (N-1) / freq * Y[N+1] - (sum(Y) - Y[1]) / freq,
// где	Y[1] - это выходящий из окна отсчет,
//		Y[N + 1] - это входящий в окно отсчет.
//
// Распишем также формулу нахождения СКО для величины наклона на каждом из окон:
//
//										u[i] = sig[i] - k*x[i]
//										Sigma[u] = D[u]^0.5
//										D[u] = M[u*u] - M[u]*M[u]
//					M[u] = sum(u) / N = (sum(sig) - sum(k*x)) / N = (sum(Y) - k * sum(X)) / N
//	M[u*u] = (sum(sig*sig) - 2*sum(sig*k*x) + sum(k*x*k*x)) / N = (sum(Y*Y) - 2 * k * sum(X*Y) + k * k * sum(X*X)) / N
//
//----------------------------------------------------------------------------------------------------------------------
bool Param_STA_LTA_Pressure_Gradient::NewData(const float *pData, const int sizePage, const int numSta,
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
		float* pDataSta = m_staData.GetFirstNewPoint();
		float* pDataSta2 = m_staData2.GetFirstNewPoint();
		float* pDataLta = m_ltaData.GetFirstNewPoint();
		float* pDataLta2 = m_ltaData2.GetFirstNewPoint();
		float* pDataStdDevSta = m_stdDevStaData.GetFirstNewPoint();
		float* pDataStdDevLta = m_stdDevLtaData.GetFirstNewPoint();
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
			int nLta1 = nLta - 1;

			double sumXLta = 0.;
			double sumX2Lta = 0.;
			double kLta = 0.;
			if (nLta > 0)
			{
				sumXLta = (double)nLta * (double)nLta1 / 2. / normFactor;
				sumX2Lta = (((double)nLta1 * (double)nLta1 * (double)nLta1 + (double)nLta1 * (double)nLta1) /
					normFactor / normFactor + sumXLta / normFactor) / 3.;
				kLta = sumX2Lta * nLta - sumXLta * sumXLta;
			}
			if (kLta == 0.)
				kLta = 1.;
			
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
				m_sumXYLta += (double)m_lastLtaNew * (double)nLta1 / normFactor;
				if ((int)numData - numSta > nLta)
					m_sumXYLta -= (m_sumYLta - m_lastLtaOld) / normFactor;
				m_sumY2Lta += m_lastLtaNew * m_lastLtaNew - m_lastLtaOld * m_lastLtaOld;
				m_sumYLta += m_lastLtaNew - m_lastLtaOld;
				m_valLta = (m_sumXYLta * nLta - sumXLta * m_sumYLta) / kLta;
				m_valLta2 = (m_sumYLta - m_valLta * sumXLta) / nLta;
				double sumLta = m_sumYLta - sumXLta * m_valLta;
				double sumLta2 = m_sumY2Lta - 2. * m_valLta * m_sumXYLta + m_valLta * m_valLta * sumX2Lta;
				m_valStdDevLta = pow(abs(sumLta2 / nLta - (sumLta / nLta) * (sumLta / nLta)), 0.5) * normFactor / nLta;
				if (nLta < numSta)
					m_valStdDevLta *= (double)numSta / (double)nLta;
			}

			// работа с STA
			int nSta(numSta);
			if (nSta > (int)numData)
				nSta = (int)numData;
			int nSta1 = nSta - 1;

			double sumXSta = (double)nSta * (double)nSta1 / 2. / normFactor;
			double sumX2Sta = (((double)nSta1 * (double)nSta1 * (double)nSta1 + (double)nSta1 * (double)nSta1) /
				normFactor / normFactor + sumXSta / normFactor) / 3.;
			double kSta = sumX2Sta * nSta - sumXSta * sumXSta;
			if (kSta == 0.)
				kSta = 1.;

			if (0 != _finite(*pStartSta))
				m_lastStaOld = *pStartSta++;
			else
				++pStartSta;
			if (0 != _finite(*pEndSta))
				m_lastStaNew = *pEndSta++;
			else
				++pEndSta;

			m_sumXYSta += (double)m_lastStaNew * (double)nSta1 / normFactor;
			if (numData > nSta)
				m_sumXYSta -= (m_sumYSta - m_lastStaOld) / normFactor;
			m_sumY2Sta += m_lastStaNew * m_lastStaNew - m_lastStaOld * m_lastStaOld;
			m_sumYSta += m_lastStaNew - m_lastStaOld;
			m_valSta = (m_sumXYSta * nSta - sumXSta * m_sumYSta) / kSta;
			m_valSta2 = (m_sumYSta - m_valSta * sumXSta) / nSta;
			double sumSta = m_sumYSta - sumXSta * m_valSta;
			double sumSta2 = m_sumY2Sta - 2. * m_valSta * m_sumXYSta + m_valSta * m_valSta * sumX2Sta;
			m_valStdDevSta = pow(abs(sumSta2 / nSta - (sumSta / nSta) * (sumSta / nSta)), 0.5) * normFactor / nSta;
			m_valStdDevSta *= (double)numSta / (double)nSta;
			
			float val = numData > numSta ? (float)((m_valSta - m_valLta) / (sqrt(m_valStdDevSta * m_valStdDevSta + m_valStdDevLta * m_valStdDevLta))) : 0.f;
			*pDataStaLta++ = val;

			/*
			auto valLta2 = m_valLta2 + m_valLta * nLta / normFactor;
			double x1 = 0.;
			double y1 = m_valLta * x1 + valLta2;
			double x2 = 0.;
			double y2 = m_valSta * x2 + m_valSta2;
			if (y2 < y1)
			{
				y1 = m_valSta * x2 + m_valSta2;
				y2 = m_valLta * x1 + valLta2;
			}
			double x3 = (double)nSta / normFactor;
			double y3 = m_valLta * x3 + valLta2;
			double x4 = (double)nSta / normFactor;
			double y4 = m_valSta * x4 + m_valSta2;
			if(y3 < y4)
			{
				y3 = m_valSta * x4 + m_valSta2;
				y4 = m_valLta * x3 + valLta2;
			}
			x3 = 1.;
			x4 = 1.;
			double a = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
			double b = sqrt((x2 - x3) * (x2 - x3) + (y2 - y3) * (y2 - y3));
			double c = sqrt((x3 - x4) * (x3 - x4) + (y3 - y4) * (y3 - y4));
			double d = sqrt((x4 - x1) * (x4 - x1) + (y4 - y1) * (y4 - y1));
			double p = (a + b + c + d) / 2.;
			val = sqrt((p - a) * (p - b) * (p - c) * (p - d));
			*pDataStaLta++ = val;
			*/

			//обработка для адаптации
			int nStaLta4StdDevMean(numSta * 2);
			if (nStaLta4StdDevMean > (int)numData)
				nStaLta4StdDevMean = (int)numData;
			double koeffStdDev = ((int)numData < numSta * 2) ? sqrt((double)numSta * 2. / (double)(numSta * 2 - (int)numData)) : 1.;

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
						
			val = /*bInit ? */(float)m_sumYStaLta4StdDevMean / nStaLta4StdDevMean / (float)koeffStdDev;/* : 0.f*/;
			*pDataStaLtaMean++ =  val;
			val = /*bInit ? */pow((float)m_sumY2StaLta4StdDevMean / nStaLta4StdDevMean - val * val, 0.5f) * (float)koeffStdDev;/* : 0.f*/;
			*pDataStaLtaStdDev++ = val == 0.f ? 1.f : val;
			val = /*bInit ? */(float)m_valSta/* : 0.f*/;
			*pDataSta++ = val;
			val = /*bInit ? */(float)m_valSta2/* : 0.f*/;
			*pDataSta2++ = val;
			val = /*bInit ? */(float)m_valLta/* : 0.f*/;
			*pDataLta++ = val;
			val = /*bInit ? */(float)m_valLta2/* : 0.f*/;
			*pDataLta2++ = val;
			val = /*bInit ? */(float)m_valStdDevSta/* : 0.f*/;
			*pDataStdDevSta++ = val;
			val = /*bInit ? */(float)m_valStdDevLta/* : 0.f*/;
			*pDataStdDevLta++ = val;

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
CDetector_Pressure_Gradient::CDetector_Pressure_Gradient() : CDetector_Pressure()
{
	m_pParamStaLtaPressure = new Param_STA_LTA_Pressure_Gradient;
}
//----------------------------------------------------------------------------------------------------------------------
CDetector_Pressure_Gradient::~CDetector_Pressure_Gradient()
{
	if (nullptr != m_pParamStaLtaPressure)
	{
		delete m_pParamStaLtaPressure;
		m_pParamStaLtaPressure = nullptr;
	}
}
//----------------------------------------------------------------------------------------------------------------------