#include "stdafx.h"
#include "Goertzel.h"
#include "resource.h"
#include <Intel\ipp\ipp.h>
#include <mathZet.h>


#include <Intel\nspvec.h>
#include <Intel\nspfft.h>
#include <Intel\nsparith.h>
#include <Intel\nspcvrt.h>
#include <Intel\nspnorm.h>
#include <Intel\nspgrtzl.h>
#include <Intel\nspwin.h>

#include <Analysis\window_funcs.h>

#define _USE_MATH_DEFINES
#include <math.h>

const long MaxHarmonics = 32;


CGoertzel::CGoertzel()
	: m_data(nullptr)
	, m_sin(nullptr)
	, m_cos(nullptr)
	, m_outdata(nullptr)
	, m_outpogr(nullptr)
	, m_dTHD(0.0)
	, m_dTHDN(0.0)
	, m_dSNR(0.0)
	, m_dSFDR(0.0)
	, m_dSINAD(0.0)
	, m_dENOB(0.0)
	, m_dFS(0.0)
	, m_dENOB_FS(0.0)
	, m_dRate(0.0)
	, m_size(0)
	, m_dResolution(0)
	, m_order(0)
{
	m_dMin = 6e-8;
	m_dMax = 1.;
	m_dMaxEnob = 32.;
}

CGoertzel::~CGoertzel()
{
	if (m_data != NULL)
	{ 
		ippsFree(m_data); 
		m_data = nullptr;
	}
	if (m_sin != NULL)
	{
		ippsFree(m_sin);
		m_sin = nullptr;
	}
	if (m_cos != NULL)
	{ 
		ippsFree(m_cos);
		m_cos = nullptr;
	}
	if (m_outdata != NULL)
	{ 
		ippsFree(m_outdata);
		m_outdata = nullptr;
	}
	if (m_outpogr != NULL)
	{ 
		ippsFree(m_outpogr);
		m_outpogr = nullptr;
	}
}

void CGoertzel::CheckSize(long size)
{
	if (m_size == size)
		return;
	else
	{
		if (m_data != NULL)
		{
			ippsFree(m_data);
			m_data = nullptr;
			ippsFree(m_sin);
			m_sin = nullptr;
			ippsFree(m_cos);
			m_cos = nullptr;
			ippsFree(m_outdata);
			m_outdata = nullptr;
			ippsFree(m_outpogr);
			m_outpogr = nullptr;
		}
		m_data = ippsMalloc_64f(size);
		m_sin = ippsMalloc_64f(size);
		m_cos = ippsMalloc_64f(size);
		m_outdata = ippsMalloc_64f(size);
		m_outpogr = ippsMalloc_64f(size);
		m_size = size;

		m_order = (long)(log((double)m_size) / log((double)2.) + 0.5) - 1;
		long spsize = 1 << m_order;
		spectr.resize(spsize);
		win.resize(spsize);
	}
}

void CGoertzel::CheckMinimum()
{
	double minLevel = m_dResolution / m_dMax / 2.;
	double maxRatio = 1 / (minLevel * minLevel);
	// характеристики не должны быть меньше чем минимальный уровень
	if (m_dTHD < minLevel)
		m_dTHD = minLevel;

	if (m_dTHDN < minLevel)
		m_dTHDN = minLevel;

	if (m_dSNR > maxRatio)
		m_dSNR = maxRatio;

	if (m_dSFDR > maxRatio)
		m_dSFDR = maxRatio;

	if (m_dSINAD > maxRatio)
		m_dSINAD = maxRatio;

	// не могут характеристики быть меньше чем заданна€ константа
	double dMax = 1 / (m_dMin * m_dMin);
	if (m_dTHD < m_dMin)
		m_dTHD = m_dMin;

	if (m_dTHDN < m_dMin)
		m_dTHDN = m_dMin;

	if (m_dSNR > dMax)
		m_dSNR = dMax;

	if (m_dSFDR > dMax)
		m_dSFDR = dMax;

	if (m_dSINAD > dMax)
		m_dSINAD = dMax;

	// разр€дность не должна быть больше чем позвол€ет это диапазон
	double maxENOB = log(sqrt(2) * power[0] / m_dResolution) / log(2.);
	double maxENOBFS = log(2 * m_dMax / m_dResolution) / log(2.);

	if (m_dENOB > maxENOB)
		m_dENOB = maxENOB;

	if (m_dENOB_FS > maxENOBFS)
		m_dENOB_FS = maxENOBFS;

	// разр€дность устройства не можеть быть больше чем заданна€ константа
	if (m_dENOB > m_dMaxEnob)
		m_dENOB = m_dMaxEnob;

	if (m_dENOB_FS > m_dMaxEnob)
		m_dENOB_FS = m_dMaxEnob;
}

void CGoertzel::Calculate(float* data, long size)
{
	CheckSize(size);
	ippsConvert_32f64f(data, m_data, m_size);
	Calculate();
}

void CGoertzel::Calculate(double* data, long size)
{
	CheckSize(size);
	ippsCopy_64f(data, m_data, m_size);
	Calculate();
}

void CGoertzel::Calculate()
{
	if (m_data == NULL)
		return;

	freqs.resize(MaxHarmonics);
	phase.resize(MaxHarmonics);
	power.resize(MaxHarmonics);

	double mean = nspdMean(m_data, m_size);
	nspdbSub1(mean, m_data, m_size);

	freqs[0] = Frequency();
	for (long i = 1; i < MaxHarmonics; i++)
		freqs[i] = freqs[0] * (i + 1);

	ippsCopy_64f(m_data, m_outdata, m_size);

	double dev = nspdStdDev(m_data, m_size);
	for (long i = 0; i < MaxHarmonics; i++)
	{
		SinusCosinus(freqs[i], 1.0, 0.0);
		power[i] = Convolution(phase[i]);
		SinusCosinus(freqs[i], power[i], phase[i]);
		ippsSub_64f_I(m_sin, m_data, m_size);
		dev = nspdStdDev(m_data, m_size);
		if (i == 0)
			ippsCopy_64f(m_data, m_outpogr, m_size);
	}

	CalculateCharacteristic();
}

void CGoertzel::CalculateCharacteristic()
{
	CString str =  L"";
	double sum2 = 0.;
	double distort = 0.;
	for (long i = 1; i < MaxHarmonics; i++)
		if (freqs[i] < m_dRate / 2)
			sum2 += power[i] * power[i];
	distort = sqrt(sum2);
	m_dTHD = distort / power[0];
	str.Format(L"THD %f ;", m_dTHD);

	double enb(1), wdt(1), enj(1);
	nspzbZero(spectr.data(), spectr.size());
	//RifeVincent4_64f(win.data(), win.size());
	//BlackmanHarris_4_94_64f(win.data(), win.size());
	if (win.size() > 2)
	{
		nspdbSet(1.0, win.data(), win.size());
		nspdWinHamming(win.data(), win.size());
		enb = EffectiveNoiseBandwidth_64f(win.data(), win.size());
		wdt = WindowWidth_64f(win.data(), win.size());
		enj = WindowEnergy_64f(win.data(), win.size());
		nspdbMpy2(win.data(), m_data, win.size());
	}
	nspdRealFftNip(m_data, spectr.data(), m_order, NSP_Inv);

	//long index = 0;The scope of the variable 'index' can be reduced.
	long w = 5;
	nspzbPowerSpectr(spectr.data(), win.data(), spectr.size());
	long zeros = 0;
	double noise = 0.;
	long index0 = 0;
	long nsize = 0;
	// обнул€ем области вокруг гармоник чтобы не мешали подсчЄтам
	for (long i = 0, size = freqs.size(); i < size; i++)
	{
		long index = (long)(freqs[i] / m_dRate * spectr.size());
		for (long j = index - w; j <= index + w; j++)
			if (j >= 0 && j < (long)spectr.size() / 2)
				win[j] = 0;
		nsize += 2 * w + 1;
	}
	nsize = win.size() - nsize;
	// счтаем шум
	noise = nspdSum(win.data(), spectr.size());
	if (nsize != 0)
		noise *= spectr.size() / nsize;// * enb * 2;// / 2 * enb * 2;
	else
		noise = 1e-6;
	noise = sqrt(noise / wdt) * 1.64 * sqrt(2.0);
	
	m_dTHDN = sqrt((distort*distort + noise*noise) / (power[0]*power[0]));
	str.AppendFormat(L"   THDN %f ;", m_dTHDN);

	double max2 = 0;
	for (long i = 1, size = power.size(); i < size; i++)
		if (max2 < power[i] && freqs[i] < m_dRate / 2)
			max2 = power[i];
	m_dSFDR = power[0] / max2;
	str.AppendFormat(L"   SFDR %f ;", m_dSFDR);

	m_dSNR = (power[0]*power[0]) / (noise*noise);
	str.AppendFormat(L"   SNR %f ;", m_dSNR);

	m_dSINAD = (power[0]*power[0] + distort*distort + noise*noise) / (distort*distort + noise*noise);
	str.AppendFormat(L"   SINAD %f ;", m_dSINAD);

	m_dENOB = ((10.*log10(m_dSINAD) - 1.76) / 6.02);
	str.AppendFormat(L"   ENOB %f ;\n", m_dENOB);

	m_dFS = m_dMax / power[0];
	str.AppendFormat(L"   DBFS %f ;\n", m_dFS);

	m_dENOB_FS = ((10.*log10(m_dSINAD) - 20.*log10(1/m_dFS) - 1.76) / 6.02);
	str.AppendFormat(L"   ENOB_FS %f ;\n", m_dENOB_FS);

	CheckMinimum();

	//OutputDebugString(str);
}

double CGoertzel::Frequency()
{
	double freq = 0.0;
	double mmin = 0.0;
	double mmax = 0.0;
	double mean;//не нужно инициализировать, так как дальше идет присвоение
	double delta;//не нужно инициализировать, так как дальше идет присвоение
	double element;//не нужно инициализировать, так как дальше идет присвоение
	long index1 = -1;
	long index2 = -1;
	long counter = 0;
	bool curved = true;

	ippsMinMax_64f(m_data, m_size, &mmin, &mmax);
	//ippsMean_64f(m_data, m_size, &mean);
	delta = (mmax - mmin) / 6.;
	element = m_data[2];
	mean = (mmax + mmin) / 2.0;

	if (m_data[1] > mean && m_data[0] > mean)
		curved = true;			// выгнутый полупериод
	else
		curved = false;			// вогнутый полупериод

	// считаем количество переходов через точку гистерезиса
	for (long i = 1; i < m_size-1; i++)
	{
		if (!curved)
		{
			if (m_data[i] - mean > 0)
			{
				if (index1 < 0)
					index1 = i;
				index2 = i;
				counter++;
				curved = true;
			}
		}
		if (m_data[i] - mean < -delta)
			curved = false;
	}
	counter--;

	// уточн€ем среднее значение сигнала на целом числе периодов
	ippsMean_64f(&m_data[index1], index2 - index1, &mean);

	if (m_data[1] > mean && m_data[0] > mean)
		curved = true;			// выгнутый полупериод
	else
		curved = false;			// вогнутый полупериод
	index1 = -1;
	index2 = -1;
	counter = 0;
	std::vector<double> ftemp;
	// считаем количество переходов через точку гистерезиса
	for (long i = 1; i < m_size - 1; i++)
	{
		if (!curved)
		{
			if (m_data[i] - mean > 0)
			{
				if (index1 < 0)
					index1 = i;
				index2 = i;
				counter++;
				curved = true;
				ftemp.push_back((asin(m_data[index2]) - asin(m_data[index2 - 1])) / (2.0 * M_PI) * m_dRate);
			}
		}
		if (m_data[i] - mean < -delta)
			curved = false;
	}
	counter--;

	// используем линейную интерпол€цию дл€ точного вычислени€ длинны периода
	double dindex1 = index1 - (mean - m_data[index1]) / (m_data[index1 - 1] - m_data[index1]);
	double dindex2 = index2 - (mean - m_data[index2]) / (m_data[index2 - 1] - m_data[index2]);

	if (counter > 0)
		if (index2 > index1)
			freq = (double)counter / (dindex2 - dindex1) * m_dRate;

	// тестовый алгоритм
	double w = asin(m_data[index1]) - asin(m_data[index1-1]);
	double p = asin(m_data[index1]) - w * index1;
	double f = w / (2.0 * M_PI) * m_dRate;


	return freq;
}

double CGoertzel::Convolution(double &phase)
{
	double dpsin(0.);
	double dpcos(0.);
	ippsDotProd_64f(m_data, m_sin, m_size, &dpsin);
	ippsDotProd_64f(m_data, m_cos, m_size, &dpcos);
	double amplitude = 2 * sqrt(dpsin * dpsin + dpcos * dpcos) / m_size;
	phase = atan2(dpcos, dpsin);
	return amplitude;
}

void CGoertzel::SinusCosinus(double freq, double amplitude, double phase)
{
	double alpha = phase;
	double dfreq = freq / m_dRate * 2. * M_PI;
	for (long i = 0; i < m_size; i++)
	{
		m_sin[i] = amplitude * sin(alpha);
		m_cos[i] = amplitude * cos(alpha);
		alpha += dfreq;
		if (alpha > 2. * M_PI)
			alpha -= 2. * M_PI;
	}
}

void CGoertzel::SetSamplingRate(double rate)
{
	m_dRate = rate;
}

void CGoertzel::SetMaxChannelValue(double dmax)
{
	m_dMax = dmax;
	int r = (int)(log10(m_dMax) + 0.5);
	double v = round(m_dMax / pow(10.0, r - 1)) * pow(10., r - 1);
	m_sMax.Format(L"%g", v);
}

void CGoertzel::SetChannelUnits(CString unit)
{
	m_sUnit = unit;
	int pos = m_sUnit.Find(L"%");
	if (pos != -1)
		m_sUnit.Insert(pos, L"%");
}

void CGoertzel::SetResolution(double resol)
{
	m_dResolution = resol;
}

double CGoertzel::GetHarmonic(long index)
{
	if (index >= 0 && index < (long)power.size())
		return power[index];
	else
		return -1;
}

double CGoertzel::GetFrequency(long index)
{
	if (index >= 0 && index < (long)freqs.size())
		return freqs[index];
	else
		return -1;
}

/*void CGoertzel::PrintResultOnListControl(CToolTipListCtrl* pList, bool bLog)
{
	long num = 0;
	long count = 0;
	double val = 0.;
	CString str = L"";
	CString sdb;//не нужно инициализировать, так как дальше идет присвоение
	sdb = TranslateResourse(IDS_DB);
	for (long i = 0; i < MaxHarmonics; i++)
	{
		if (freqs[i] > m_dRate/2)
		{
			str.Empty();
			pList->SetItemText(count + num, 0, str);
			pList->SetItemText(count + num, 1, str);
			pList->SetItemText(count + num, 2, str);
			count++;
		}
		else
		{
			val = power[i] / power[0];
			if (val > 10e-9)
			{
				str.Format(L"%d", i + 1);
				pList->SetItemText(count + num, 0, str);
				str.Format(L"%.2f", freqs[i]);
				pList->SetItemText(count + num, 1, str);
				if (bLog)
					str.Format(L"%.2f " + sdb, 20. * log10(val));
				else
					str.Format(L"%5.3f %%", val * 100.);
				pList->SetItemText(count + num, 2, str);
				count++;
			}
		}
		if (count > 8)
			break;
	}

	str.Format(L"ЧЧЧЧЧ");
	pList->SetItemText(8 + num, 0, str);
	str.Format(L"ЧЧЧЧЧЧ");
	pList->SetItemText(8 + num, 1, str);
	str.Format(L"ЧЧЧЧЧЧЧ");
	pList->SetItemText(8 + num, 2, str);

	pList->SetItemText(9 + num, 0, TranslateResourse(IDS_THD));
	pList->SetItemToolTipText(9 + num, 0, TranslateResourse(IDS_DESCRIPTION_THD));
	str.Format(L"%.2f", freqs[0]);
	pList->SetItemText(9 + num, 1, str);
	if (bLog)
		str.Format(L"%.2f " + sdb, 20. * log10(1 / m_dTHD));
	else
		str.Format(L"%5.3f %%", m_dTHD * 100.);
	pList->SetItemText(9 + num, 2, str);

	pList->SetItemText(10 + num, 0, TranslateResourse(IDS_THDN));
	pList->SetItemToolTipText(10 + num, 0, TranslateResourse(IDS_DESCRIPTION_THDN));
	str.Format(L"%.2f", freqs[0]);
	pList->SetItemText(10 + num, 1, str);
	if (bLog)
		str.Format(L"%.2f " + sdb, 20. * log10(1 / m_dTHDN));
	else
		str.Format(L"%5.3f %%", m_dTHDN * 100.);
	pList->SetItemText(10 + num, 2, str);

	pList->SetItemText(11 + num, 0, TranslateResourse(IDS_SFDR));
	pList->SetItemToolTipText(11 + num, 0, TranslateResourse(IDS_DESCRIPTION_SFDR));
	str.Format(L"%.2f", freqs[0]);
	pList->SetItemText(11 + num, 1, str);
	if (bLog)
		str.Format(L"%.2f " + sdb, 20. * log10(m_dSFDR));
	else
		str.Format(L"%5.1f", m_dSFDR);
	pList->SetItemText(11 + num, 2, str);

	pList->SetItemText(12 + num, 0, TranslateResourse(IDS_SNR));
	pList->SetItemToolTipText(12 + num, 0, TranslateResourse(IDS_DESCRIPTION_SNR));
	str.Format(L"%.2f", freqs[0]);
	pList->SetItemText(12 + num, 1, str);
	if (bLog)
		str.Format(L"%.2f " + sdb, 10. * log10(m_dSNR));
	else
		str.Format(L"%5.1f", m_dSNR);
	pList->SetItemText(12 + num, 2, str);

	pList->SetItemText(13 + num, 0, TranslateResourse(IDS_SINAD));
	pList->SetItemToolTipText(13 + num, 0, TranslateResourse(IDS_DESCRIPTION_SINAD));
	str.Format(L"%.2f", freqs[0]);
	pList->SetItemText(13 + num, 1, str);
	if (bLog)
		str.Format(L"%.2f " + sdb, 10. * log10(m_dSINAD));
	else
		str.Format(L"%5.1f", m_dSINAD);
	pList->SetItemText(13 + num, 2, str);

	pList->SetItemText(14 + num, 0, TranslateResourse(IDS_ENOB));
	pList->SetItemToolTipText(14 + num, 0, TranslateResourse(IDS_DESCRIPTION_ENOB));
	str.Format(L"%.2f", freqs[0]);
	pList->SetItemText(14 + num, 1, str);
	str.Format(L"%5.1f", m_dENOB);
	pList->SetItemText(14 + num, 2, str);

	sdb = TranslateResourse(IDS_DB);
	sdb += L" (" + m_sMax + m_sUnit + L")";
	pList->SetItemText(15 + num, 0, TranslateResourse(IDS_FS));
	pList->SetItemToolTipText(15 + num, 0, TranslateResourse(IDS_DESCRIPTION_FS));
	str.Format(L"%.2f", freqs[0]);
	pList->SetItemText(15 + num, 1, str);
	if (bLog)
		str.Format(L"%.2f " + sdb, 20. * log10(1/m_dFS));
	else
		str.Format(L"%5.1f", m_dFS);
	pList->SetItemText(15 + num, 2, str);

	pList->SetItemText(16 + num, 0, TranslateResourse(IDS_ENOB_FS));
	pList->SetItemToolTipText(16 + num, 0, TranslateResourse(IDS_DESCRIPTION_ENOB_FS));
	str.Format(L"%.2f", freqs[0]);
	pList->SetItemText(16 + num, 1, str);
	str.Format(L"%5.1f", m_dENOB_FS);
	pList->SetItemText(16 + num, 2, str);
}*/

long CGoertzel::GenerateOutputData(float* signal, float* distortion, long size)
{
	double *temp1 = new double[size];
	double *temp2 = new double[size];

	GenerateOutputData(temp1, temp2, size);
	ippsConvert_64f32f(temp1, signal, size);
	ippsConvert_64f32f(temp2, distortion, size);

	delete[] temp1;
	temp1 = nullptr;
	delete[] temp2;
	temp2 = nullptr;

	return 0;
}

long CGoertzel::GenerateOutputData(double* signal, double* distortion, long size)
{
	long order = (long)(log((double)size) / log((double)2.) + 0.5);
	long realsize = 1 << order;
	//long index; The scope of the variable 'index' can be reduced.
	_DCplx *spectr = new _DCplx[realsize];
	double *spower = new double[realsize];

	// выравнивание фазы
	size = phase.size();
	for (long i = 1; i < size; i++)
	{
		phase[i] -= phase[0] + M_PI / 2;
		if (phase[i] < -M_PI)
			phase[i] += 2 * M_PI;
		if (phase[i] > M_PI)
			phase[i] -= 2 * M_PI;
	}
	phase[0] = - M_PI / 2;

	// генераци€ сигнала из р€да ‘урье по гармоническим частотам
	nspzbZero(spectr, realsize);
	size = freqs.size();
	for (long i = 0; i < size; i++)
	{
		long index = (long)(freqs[i] / m_dRate * realsize + 0.5);
		if (index > realsize / 2)
			break;
		if (realsize > 1)
		{
		spectr[i + 1].re = power[i] * cos(phase[i]);
		spectr[i + 1].im = power[i] * sin(phase[i]);
		}
	}

	nspdCcsFftNip(spectr, signal, order, NSP_Inv);
	nspdbMpy1(realsize / 2.f, signal, realsize);

	if (realsize > 1)
	{
		spectr[1].re = 0.0;
		spectr[1].im = 0.0;
	}
	nspdCcsFftNip(spectr, distortion, order, NSP_Inv);
	nspdbMpy1(realsize / 2.f, distortion, realsize);

	delete[] spectr;
	spectr = nullptr;
	delete[] spower;
	spower = nullptr;

	return 0;
}
