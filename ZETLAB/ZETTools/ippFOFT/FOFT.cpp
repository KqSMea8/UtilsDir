//------------------------------------------------------------------------------
//	Файл FOFT.cpp - Fractional Octave Frequency Transformation
//	Зеленоград, ЗАО "ЭТМС", Савостин Ю.М.
//------------------------------------------------------------------------------
#include "StdAfx.h"
#include <ippFOFT\FOFT.h>
//------------------------------------------------------------------------------
CFOFT::CFOFT()
	: m_bInit(false)
	, m_bNeedDecim(false)
	, m_freq_0(0.)
	, m_freq_N(0.)
	, m_FreqADC(0.)
	, m_FreqADC_decim(0.)
	, m_precision(0.001)
	, m_OctavePart(Number_Octave_Part)
	, m_result()
	, m_dataDouble()
	, m_dataDoubleDecim()
	, m_pDecimal(NULL)
{}
//------------------------------------------------------------------------------
CFOFT::~CFOFT()
{}
//------------------------------------------------------------------------------
bool CFOFT::Create(
	float freqMin,		// центральная частота младшей полосы
	float freqMax,		// центральная частота старшей полосы
	float freqADC,		// частота дискретизации АЦП
	OctavePart octavePart)	// часть октавы
{
	double Fmin = max(Freq_Min, (double)freqMin);
	m_freq_0 = Fmin;
	m_freq_N = freqMax;
	m_FreqADC = freqADC;
	m_FreqADC_decim = freqADC * 0.1;
	m_bInit = (m_freq_0 < m_freq_N) && (m_freq_N < m_FreqADC);
	if (m_bInit)
		m_bInit = SetOctavePart(octavePart);
	return m_bInit;
}
//------------------------------------------------------------------------------
bool CFOFT::SetOctavePart(OctavePart octPart)
{
	bool ret = m_bInit && ((BYTE)octPart < Number_Octave_Part);
	if (ret)
	{
		if (m_OctavePart != octPart)
		{
			m_OctavePart = octPart;
			m_list.clear();

			// Ищу первую частоту
			double *pFreq = (double*)g_d1_All_Octave;
			if (m_OctavePart == op1_3)
				pFreq += SizeBandInDecade_1;
			else if (m_OctavePart == op1_6)
				pFreq += SizeBandInDecade_1 + SizeBandInDecade_3;
			else if (m_OctavePart == op1_12)
				pFreq += SizeBandInDecade_1 + SizeBandInDecade_3 + SizeBandInDecade_6;
			else if (m_OctavePart == op1_24)
				pFreq += SizeBandInDecade_1 + SizeBandInDecade_3 + SizeBandInDecade_6 + SizeBandInDecade_12;

			DWORD i = 0;	// нужна внешняя переменная
			for (; i < g_sizeBandInAllDecade[m_OctavePart]; ++i)
			{
				if (m_freq_0 >= pFreq[i])
				{
					m_freq_0 = pFreq[i];
					break;
				}
			}
			PushBackInList(m_freq_0, m_OctavePart, true);

			++i;
			bool bNeedNextFreq = true;
			int factorFreq = 1;
			while (bNeedNextFreq)
			{
				for (; i < g_sizeBandInAllDecade[m_OctavePart]; ++i)
				{
					double fr = pFreq[i] * factorFreq;
					bNeedNextFreq = fr < m_freq_N;
					if (bNeedNextFreq)
						bNeedNextFreq = PushBackInList(fr, m_OctavePart, m_list.back().bDecimalData);
					else
						break;
				}
				if (bNeedNextFreq)
				{
					i = 0;
					if (m_OctavePart != op1_1)
						factorFreq *= 10;
					else
						factorFreq *= 1000;
				}
			}
		}
		UINT num = m_list.size();
		ret = num > 0;
		if (ret)
		{
			m_freq_0 = m_list.front().pFiltr->GetFreq();
			m_freq_N = m_list.back().pFiltr->GetFreq();
			m_result.ReSize(num);
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
bool CFOFT::PushBackInList(double freqCur, OctavePart octPart, bool bOpportDecimation)
{
	bool ret = m_bInit;
	static ParamOctaveBand tmp;
	if (ret)
	{
		tmp.pFiltr = new CIirOctaveDouble();
		tmp.pFiltr->Create(&m_FreqADC);
		tmp.pFiltr->InitFiltr(tfOctave, freqCur, octPart);
		if (IsEqual(freqCur, tmp.pFiltr->GetFreq(), m_precision))
		{// нашлось, децимация не нужна
			tmp.bDecimalData = false;
			m_list.push_back(tmp);
		}
		else
		{// не нашлось
			if (bOpportDecimation)
			{// требуется децимация
				if (!m_bNeedDecim)
				{
					m_pDecimal = new CDecimation();
					m_pDecimal->Create((float)m_FreqADC);
					m_bNeedDecim = true;
				}
				delete tmp.pFiltr;
				tmp.bDecimalData = true;
				tmp.pFiltr = new CIirOctaveDouble();
				tmp.pFiltr->Create(&m_FreqADC_decim);
				tmp.pFiltr->InitFiltr(tfOctave, freqCur, octPart);
				if (IsEqual(freqCur, tmp.pFiltr->GetFreq(), m_precision))
				{
					m_list.push_back(tmp);
				//	ret = true;		уже
				}
				else
					ret = false;
			}
			else// децимация не возможна
				ret = false;
		}
		delete tmp.pFiltr;
		tmp.pFiltr = NULL;
	}
	return ret;
}
//------------------------------------------------------------------------------
// Очень желательно, чтобы number было кратно 10.
// Если функция вернула true, значит массив m_result был заменён
bool CFOFT::Calculation(double time, int number, float *pData, long quality)
{
	bool ret = m_bInit && (pData != NULL) && (number > 0);
	if (ret)
	{
		bool bDataDecimOk = m_bNeedDecim;		// есть данные требуемого времени
		int numDecim = 0;
		// готовлю исходные данные типа double
		bool bDataOk = m_dataDouble.ReSize((UINT)number) &&
			(ippsConvert_32f64f(pData, m_dataDouble.pData, number) == ippStsNoErr);
		if (m_bNeedDecim)
		{
			m_pDecimal->PutData(time, number, pData, quality);
			numDecim = number / 10;
			bDataDecimOk = m_dataDoubleDecim.ReSize((UINT)numDecim) &&
				m_pDecimal->GetData(time, numDecim, m_dataDoubleDecim.pData);
		}
		// цикл по фильтрам
		ret = m_result.ReSize(m_list.size());
		if (ret)
		{
			int indx = 0;
			for (auto it = m_list.cbegin(), ie = m_list.cend();
				it != ie; ++it, ++indx)
			{
				bool flag;
				int size;
				double *padd;
				if (it->bDecimalData)
				{
					flag = bDataDecimOk;
					size = numDecim;
					padd = m_dataDoubleDecim.pData;
				}
				else
				{
					flag = bDataOk;
					size = number;
					padd = m_dataDouble.pData;
				}
				if (flag)
				{// фильтрация октавным фильтром и вычисление СКЗ
					double summ;
					flag = it->pFiltr->Filtration_I(padd, size) &&
						(ippsSqr_64f_I(padd, size) == ippStsNoErr) &&
						(ippsSum_64f(padd, size, &summ) == ippStsNoErr);
					if (flag)
						m_result.pData[indx] = (float)sqrt(summ / size);
				}
				if (!flag)
					m_result.pData[indx] = std::numeric_limits<float>::quiet_NaN();
			}
		}
	}
	return ret;
}
//------------------------------------------------------------------------------