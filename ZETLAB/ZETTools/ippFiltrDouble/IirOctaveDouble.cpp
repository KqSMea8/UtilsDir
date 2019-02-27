//------------------------------------------------------------------------------
// Файл CIirOctaveDouble.cpp
// Версия от 26.06.2012
// ЗАО "ЭТМС", Зеленоград, Антонов А.Ю.
//------------------------------------------------------------------------------
#include "StdAfx.h"
#include <ippFiltrDouble\IirOctaveDouble.h>
#include <ippFiltrDouble\EllipticFunctions.h>
//------------------------------------------------------------------------------
const double *d1_1Octave(g_d1_All_Octave);
const double *d1_3Octave(g_d1_All_Octave + SizeBandInDecade_1);
//------------------------------------------------------------------------------
const double k3db = 1.035129226666;
//------------------------------------------------------------------------------
CIirOctaveDouble::CIirOctaveDouble() : CCustomIirFiltrDouble()
{
	m_pStateLPF = m_pStateHPF = nullptr;
	m_pStateLpfBuf = m_pStateHpfBuf = nullptr;
	ZeroMemory(m_pTapsD, ((MAX_ELLIPTIC_ORDER + 1) * 2)*sizeof(double));
}
//------------------------------------------------------------------------------
CIirOctaveDouble::~CIirOctaveDouble()
{
	if (m_pStateLpfBuf)
	{
		ippsFree(m_pStateLpfBuf);
		m_pStateLpfBuf = nullptr;
	}
	if (m_pStateHpfBuf)
	{
		ippsFree(m_pStateHpfBuf);
		m_pStateHpfBuf = nullptr;
	}
}
//------------------------------------------------------------------------------
bool CIirOctaveDouble::_Init()
{
	bool bStatus;
	TransferFunction tfLPF, tfHPF;

	// надо задать эмпирические константы - неравномерность в полосе пропускания и
	// максимальное значение в полосе запирания
	bStatus =	_InitEllipticFilter(teLPF, g_factor[m_OctavePart], g_factor[m_OctavePart] * IPP_SQRT2, 0.1, 80., &tfLPF) &&
				_InitEllipticFilter(teHPF, g_factor[m_OctavePart], g_factor[m_OctavePart] * IPP_SQRT2, 0.1, 80., &tfHPF);		
	
	// приподнимаем характеристики на 0.05 дБ, так как задали неравномерность 0.1 дБ
	tfLPF.numerator *= pow(10., 0.0025);
	tfHPF.numerator *= pow(10., 0.0025);
	
	for (int i = 0; i < LimitPartsCount; ++i)
	{
		m_limitFreqs[LimitPartsCount - 1 + i] = 1. + (g_factor[m_OctavePart] - 1.) / (pow(2., 1. / 2.) - 1.) * (pow(2., g_partPoints[i]) - 1.);
		m_limitFreqs[LimitPartsCount - 1 - i] = 1. / m_limitFreqs[LimitPartsCount - 1 + i];
	}

	// tfLPF -------------------------------------------
	if (bStatus)
	{
		double fNorm(Freq / *pFreqADC);
		double freq = fNorm * g_factor[m_OctavePart];
		double kLPF = freq * freq * 0.0828 + freq * 0.0319 + 1.;
		_BilinearTransformation(tfLPF, freq / k3db * kLPF, tfLPF);

		int iOrder(max(tfLPF.numerator.getOrder(), tfLPF.denomenator.getOrder()));
		for (int i = 0; i < iOrder + 1; ++i)
		{
			m_pTapsD[i] = tfLPF.numerator.getCoeff(i).real();
			m_pTapsD[i + iOrder + 1] = tfLPF.denomenator.getCoeff(i).real();
		}

		if (m_pStateLpfBuf)	
		{
			ippsFree(m_pStateLpfBuf);
			m_pStateLpfBuf = nullptr;
		}
		int sizeBuffer(0);
		bStatus = ippsIIRGetStateSize_64f(iOrder, &sizeBuffer) == ippStsNoErr;
		if (bStatus)
			m_pStateLpfBuf = ippsMalloc_8u(sizeBuffer);
		bStatus = bStatus & (ippsIIRInit_64f(&m_pStateLPF, m_pTapsD, iOrder, nullptr, m_pStateLpfBuf) == ippStsNoErr);
				
		// tfHPF --------------------------------------------------
		if (bStatus)
		{
			freq = fNorm / g_factor[m_OctavePart];
			double kHPF = freq * freq * 0.0828 + freq * 0.0319 + 1.;
			_BilinearTransformation(tfHPF, freq * k3db / kHPF, tfHPF);

			iOrder = max(tfHPF.numerator.getOrder(), tfHPF.denomenator.getOrder());
			for (int i = 0; i < iOrder + 1; ++i)
			{
				m_pTapsD[i] = tfHPF.numerator.getCoeff(i).real();
				m_pTapsD[i + iOrder + 1] = tfHPF.denomenator.getCoeff(i).real();
			}

			if (m_pStateHpfBuf)
			{
				ippsFree(m_pStateHpfBuf);
				m_pStateHpfBuf = nullptr;
			}
			int sizeBuffer(0);
			bStatus = ippsIIRGetStateSize_64f(iOrder, &sizeBuffer) == ippStsNoErr;
			if (bStatus)
				m_pStateHpfBuf = ippsMalloc_8u(sizeBuffer);
			bStatus = bStatus & (ippsIIRInit_64f(&m_pStateHPF, m_pTapsD, iOrder, nullptr, m_pStateHpfBuf) == ippStsNoErr);
		}
	}
	return bStatus;
}
//------------------------------------------------------------------------------
bool CIirOctaveDouble::InitFiltr(TypeFiltr type, double freq, OctavePart part)
{	// новый тип фильтра
	if (type == tfOctave)
	{
		FType = tfOctave;
		if ((BYTE)part >= Number_Octave_Part)
			m_OctavePart = (OctavePart)(Number_Octave_Part - 1);
		else
			m_OctavePart = part;
		Freq = PriorValue(freq, part);
		bInit = _Init();
	}
	else
		bInit = false;
	return bInit;
}
//------------------------------------------------------------------------------
bool CIirOctaveDouble::Filtration(double *pInpData, double *pOutData, int num)
{
	bool ret;
	if (bInit)
	{
		ret = (ippsIIR_64f(pInpData, pOutData, num, m_pStateLPF) == ippStsNoErr) &&
				(ippsIIR_64f_I(pOutData, num, m_pStateHPF) == ippStsNoErr);
	}
	else
		ret = false;
	return ret;
}
//------------------------------------------------------------------------------
bool CIirOctaveDouble::Filtration_I(double *pInpOutData, int num)
{
	bool ret;
	if (bInit)
	{		
		ret = (ippsIIR_64f_I(pInpOutData, num, m_pStateLPF) == ippStsNoErr) &&
				(ippsIIR_64f_I(pInpOutData, num, m_pStateHPF) == ippStsNoErr);
	}
	else
		ret = false;
	return ret;
}
//------------------------------------------------------------------------------
bool CIirOctaveDouble::ReStart()
{
	bool ret;
	if (bInit)
		ret = InitFiltr(FType, Freq, OctavePart(m_OctavePart));
	else
		ret = false;
	return ret;
}
//------------------------------------------------------------------------------
bool CIirOctaveDouble::_InitEllipticFilter(
	TypeElliptic teType, double dFreq1, double dFreq2,
	double dBandPassRipple, double dBandStopDrop,
	TransferFunction* tfFunction )
{
	double dBandPassEpsilon(sqrt(pow(10., dBandPassRipple * 0.1) - 1.));
	double dBandStopEpsilon(sqrt(pow(10., dBandStopDrop * 0.1) - 1.));
	double k(dFreq1 / dFreq2);
	double k1(dBandPassEpsilon / dBandStopEpsilon);

	double Kk(EllipticIntegral(k));
	double Kk1(EllipticIntegral(k1));
	double Kk_(EllipticIntegral(sqrt(1 - pow(k, 2))));
	double Kk1_(EllipticIntegral(sqrt(1 - pow(k1, 2))));

	double dN((Kk1_ * Kk) / (Kk1 * Kk_));
	long lN(long(dN + 1.));

	if (lN > MAX_ELLIPTIC_ORDER)
		return false;

	std::vector <double> vU;
	for (int i = 0; i < lN / 2; ++i)
		vU.push_back((2. * (double(i) + 1.) - 1.) / double(lN));

	double k1p(sqrt(1 - pow(k1, 2)));
	double kp(pow (k1p, lN));
	for (int i = 0; i < int (vU.size()); ++i)
		kp *= pow (EllipticSinus(vU[i], k1p), 4);

	k = sqrt(1 - pow(kp, 2));

	std::complex<double> j(0., 1.);

	//нули
	std::vector<std::complex<double>> vzeros;
	std::complex<double> zero;

	for (UINT i = 0; i < vU.size(); ++i)
	{
		zero = j / k / EllipticCosinus(vU[i], k);
		vzeros.push_back(zero);
	}

	vector<Polynom> vNumerator;
	Polynom numerator(1.);
	for (UINT i = 0; i < vzeros.size(); ++i)
	{
		vNumerator.push_back(Polynom(1., 0., - pow(vzeros[i], 2)));
		numerator *= vNumerator[i];
	}

	//полюса
	std::complex<double> v0;
	v0 = -j / double(lN) * EllipticArcsinus(j / dBandPassEpsilon, k1);

	std::vector<std::complex<double>> vpoles;
	std::complex<double> pole;
	for (UINT i = 0; i < vU.size(); ++i)
	{
		pole = j * EllipticCosinus(vU[i] - j * v0, k);
		vpoles.push_back(pole);
	}

	vector<Polynom> vDenomenator;
	Polynom denomenator(1.);
	for (UINT i = 0; i < vpoles.size(); ++i)
	{
		vDenomenator.push_back(Polynom(1., -2. * vpoles[i].real(),
			pow(vpoles[i].real(), 2) + pow(vpoles[i].imag(), 2)));
		denomenator *= vDenomenator[i];
	}

	if (lN % 2 != 0)
	{
		pole = j * EllipticSinus(j * v0, k);
		vDenomenator.push_back(Polynom(1., -pole));
		denomenator *= vDenomenator[vDenomenator.size() - 1];
	}

	complex<double>numerator0 = numerator.getCoeff(0);
	complex<double>denomenator0 = denomenator.getCoeff(0);
	double H0 = numerator0.real() / denomenator0.real();

	if (lN % 2 == 0)
		numerator *= (1. / sqrt(1. + pow(dBandPassEpsilon, 2)));
	numerator /= H0;

	TransferFunction tfAnalogFunction, tfDigitalFunction;
	tfAnalogFunction.numerator = numerator;
	tfAnalogFunction.denomenator = denomenator;

	if (teType == teHPF)
	{
		int iOrder(max(tfAnalogFunction.numerator.getOrder(), tfAnalogFunction.denomenator.getOrder()));
		tfAnalogFunction.numerator.inverse();
		tfAnalogFunction.numerator *= (Polynom(1, 0)).pow(iOrder - tfAnalogFunction.numerator.getOrder());
		tfAnalogFunction.denomenator.inverse();
	}
	
	*tfFunction = tfAnalogFunction;

	return true;
}
//------------------------------------------------------------------------------
double CIirOctaveDouble::PriorValue( double freq, OctavePart part )
{// Возвращаем частоту из допустимых (см. массивы d1_*Octave),
// максимально близкую к freq
	double f(freq), f1, f2, temp, temp_;
	if (f > 0.)
	{
		f1 = f / g_factor[part];
		f2 = f * g_factor[part];

		// ограничение на минимальное значение
		temp_ = *pFreqADC / 90.;
		temp = temp_ / g_factor[part];
		if (f1 < temp)
		{
			f = temp_;
		}
		else
		{// ограничение Найквиста
			temp = *pFreqADC * 0.5 - temp;
			if (f2 > temp)
			{
				f = temp / g_factor[part];
			}
		}
		double dMin(temp_);		//*pFreqADC / 90.
		double dMax(*pFreqADC * 0.5 / g_factor[part]);

		vector <double> dFrequencies;
		int size(0);
		double *pAddOctal = (double*)g_d1_All_Octave;
		switch (part)
		{
		case op1_1:
			size = SizeBandInDecade_1;
		//	pAddOctal += 0;
			break;
		case op1_3:
			size = SizeBandInDecade_3;
			pAddOctal += SizeBandInDecade_1;
			break;
		case op1_6:
			size = SizeBandInDecade_6;
			pAddOctal += SizeBandInDecade_1 + SizeBandInDecade_3;
			break;
		case op1_12:
			size = SizeBandInDecade_12;
			pAddOctal += SizeBandInDecade_1 + SizeBandInDecade_3 + SizeBandInDecade_6;
			break;
		case op1_24:
			size = SizeBandInDecade_24;
			pAddOctal += SizeBandInDecade_1 + SizeBandInDecade_3 + SizeBandInDecade_6 + SizeBandInDecade_12;
			break;
		}
		if (part == op1_1)
		{
			for (int i = -2; i < 2; ++i)
			{
				for (int j = 0; j < size; ++j)
				{
					temp = pAddOctal[j] * pow(10, i * 3);
					if (temp >= dMin && temp <= dMax)
					{
						dFrequencies.push_back(temp);
					}
				}
			}
		}
		else
		{
			for (int i = -3; i < 9; ++i)
			{
				for (int j = 0; j < size; ++j)
				{
					temp = pAddOctal[j] * pow(10, i);
					if (temp >= dMin && temp <= dMax)
					{
						dFrequencies.push_back(temp);
					}
				}
			}
		}
		
		int iIndex(0);
		for (UINT i = 0; i < dFrequencies.size(); ++i)
		{
			if (f >= dFrequencies[i])
				iIndex = i;
		}
		f = dFrequencies[iIndex];
	}
	else
		f = 0.;
	return f;
}
//----------------------------------------------------------------------------------------------------------------------
const double CIirOctaveDouble::getDownLimit(const double f, const int filtrClass) const
{
	auto cl = filtrClass;
	if (cl < 0)
		cl = 0;
	if (cl > 2)
		cl = 2;
	return getLimit(f, g_partDownLimitStart[cl], g_partDownLimitEnd[cl]);
}
//----------------------------------------------------------------------------------------------------------------------
const double CIirOctaveDouble::getUpLimit(const double f, const int filtrClass) const
{
	auto cl = filtrClass;
	if (cl < 0)
		cl = 0;
	if (cl > 2)
		cl = 2;
	return getLimit(f, g_partUpLimitStart[cl], g_partUpLimitEnd[cl]);
}
//----------------------------------------------------------------------------------------------------------------------
const double CIirOctaveDouble::getLimit(const double f, const double* pLimitStart, const double* pLimitEnd) const
{
	double k = f / Freq;
	int zone(0);
	for (int i = 0; i < LimitPointsCount; ++i)
	{
		if (k < m_limitFreqs[i])
			break;
		++zone;
	}

	double ret(NAN);

	if (zone == 0)
		ret = pLimitStart[0];
	else if (zone == LimitPointsCount)
		ret = pLimitEnd[LimitPointsCount - 2];
	else
	{
		--zone;
		auto startY = pLimitStart[zone];
		auto endY = pLimitEnd[zone];
		auto startX = log10(m_limitFreqs[zone]);
		auto endX = log10(m_limitFreqs[zone + 1]);

		ret = startY + (endY - startY) / (endX - startX) * (log10(k) - startX);
	}

	return ret;
}
//----------------------------------------------------------------------------------------------------------------------