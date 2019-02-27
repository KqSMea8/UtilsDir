//------------------------------------------------------------------------------
#include "StdAfx.h"
#include <Algorithms\FunctionsUsingIPP.h>
#include <Approximation\Approximation_Line.h>
#include <Approximation\Approximation_Parabola.h>
#include <complex>
#include <vector>
//------------------------------------------------------------------------------
const double g_10_log10_E_64f(10 * log10(IPP_E));
const double g_20_log10_E_64f(2 * g_10_log10_E_64f);

const float  g_10_log10_E_32f((float)g_10_log10_E_64f);
const float  g_20_log10_E_32f((float)g_20_log10_E_64f);
//------------------------------------------------------------------------------
// Удаление постоянной составляющей сигнала
bool RemovingConstComponent_32f(const float *pSrc, float *pDst, int len, float *mean)
{
	float x(0.f);
	bool ret = (pSrc != nullptr) && (pDst != nullptr) && (len > 1);
	if (ret)
	{
		ret = ippStsNoErr == ippsMean_32f(pSrc, len, &x, ippAlgHintFast);
		if (ret && (x != 0.f))		// IsNotEqual - нельзя !!
		{// пост. сост. успешно определена и != 0
			ret = ippStsNoErr == ippsSubC_32f(pSrc, x, pDst, len);
		}
		else
			ret = ippStsNoErr == ippsCopy_32f(pSrc, pDst, len);
	}
	if (mean != nullptr)
		*mean = x;
	return ret;
}
//------------------------------------------------------------------------------
// Удаление постоянной составляющей сигнала
bool RemovingConstComponent_32f_I(float *pSrcDst, int len, float *mean)
{
	float x(0.f);
	bool ret = (pSrcDst != nullptr) && (len > 1);
	if (ret)
	{
		ret = ippStsNoErr == ippsMean_32f(pSrcDst, len, &x, ippAlgHintFast);
		if (ret && (x != 0.f))		// IsNotEqual - нельзя !!
			ret = ippStsNoErr == ippsSubC_32f_I(x, pSrcDst, len);
	}
	if (mean != nullptr)
		*mean = x;
	return ret;
}
//------------------------------------------------------------------------------
// Удаление постоянной составляющей сигнала
bool RemovingConstComponent_64f(const double *pSrc, double *pDst, int len, double *mean)
{
	double x(0.);
	bool ret = (pSrc != nullptr) && (pDst != nullptr) && (len > 1);
	if (ret)
	{
		ret = ippStsNoErr == ippsMean_64f(pSrc, len, &x);
		if (ret && (x != 0.))		// IsNotEqual - нельзя !!
		{// пост. сост. успешно определена и != 0
			ret = ippStsNoErr == ippsSubC_64f(pSrc, x, pDst, len);
		}
		else
			ret = ippStsNoErr == ippsCopy_64f(pSrc, pDst, len);
	}
	if (mean != nullptr)
		*mean = x;
	return ret;
}
//------------------------------------------------------------------------------
// Удаление постоянной составляющей сигнала
bool RemovingConstComponent_64f_I(double *pSrcDst, int len, double *mean)
{
	double x(0.);
	bool ret = (pSrcDst != nullptr) && (len > 1);
	if (ret)
	{
		ret = ippStsNoErr == ippsMean_64f(pSrcDst, len, &x);
		if (ret && (x != 0.))		// IsNotEqual - нельзя !!
			ret = ippStsNoErr == ippsSubC_64f_I(x, pSrcDst, len);
	}
	if (mean != nullptr)
		*mean = x;
	return ret;
}
//------------------------------------------------------------------------------
bool ConvertAmplToDB_32f(const float referAmpl, const float *pSrc, float *pDst, int len, const bool bSrcNegative)
{
	bool ret =
		(referAmpl > 0.f) &&
		(pSrc != nullptr) &&
		(pDst != nullptr) &&
		(len > 0);
	if (ret)
	{
		if (bSrcNegative)
			ret =
			(ippStsNoErr == ippsAbs_32f(pSrc, pDst, len)) &&
			(ippStsNoErr == ippsMulC_32f_I((1.f / referAmpl), pDst, len));
		else
			ret = ippStsNoErr == ippsMulC_32f(pSrc, (1.f / referAmpl), pDst, len);

		if (ret)
			ret =
			(ippStsNoErr == ippsLn_32f_I(pDst, len)) &&
			(ippStsNoErr == ippsMulC_32f_I(g_20_log10_E_32f, pDst, len));
	}
	return ret;
}

bool ConvertAmplToDB_32f_I(const float referAmpl, float *pSrcDst, int len, const bool bSrcNegative)
{
	bool ret =
		(referAmpl > 0.f) &&
		(pSrcDst != nullptr) &&
		(len > 0);
	if (ret)
	{
		if (bSrcNegative)
			ret = ippStsNoErr == ippsAbs_32f_I(pSrcDst, len);

		if (ret)
			ret =
			(ippStsNoErr == ippsMulC_32f_I((1.f / referAmpl), pSrcDst, len)) &&
			(ippStsNoErr == ippsLn_32f_I(pSrcDst, len)) &&
			(ippStsNoErr == ippsMulC_32f_I(g_20_log10_E_32f, pSrcDst, len));
	}
	return ret;
}

bool ConvertPowerToDB_32f(const float referAmpl, const float *pSrc, float *pDst, int len, const bool bSrcNegative)
{
	bool ret =
		(referAmpl > 0.f) &&
		(pSrc != nullptr) &&
		(pDst != nullptr) &&
		(len > 0);
	if (ret)
	{
		if (bSrcNegative)
			ret =
			(ippStsNoErr == ippsAbs_32f(pSrc, pDst, len)) &&
			(ippStsNoErr == ippsMulC_32f_I((1.f / (referAmpl * referAmpl)), pDst, len));
		else
			ret = ippStsNoErr == ippsMulC_32f(pSrc, (1.f / (referAmpl * referAmpl)), pDst, len);

		if (ret)
			ret =
			(ippStsNoErr == ippsLn_32f_I(pDst, len)) &&
			(ippStsNoErr == ippsMulC_32f_I(g_10_log10_E_32f, pDst, len));
	}
	return ret;

}

bool ConvertPowerToDB_32f_I(const float referAmpl, float *pSrcDst, int len, const bool bSrcNegative)
{
	bool ret =
		(referAmpl > 0.f) &&
		(pSrcDst != nullptr) &&
		(len > 0);
	if (ret)
	{
		if (bSrcNegative)
			ret = ippStsNoErr == ippsAbs_32f_I(pSrcDst, len);

		if (ret)
			ret =
			(ippStsNoErr == ippsMulC_32f_I((1.f / (referAmpl * referAmpl)), pSrcDst, len)) &&
			(ippStsNoErr == ippsLn_32f_I(pSrcDst, len)) &&
			(ippStsNoErr == ippsMulC_32f_I(g_10_log10_E_32f, pSrcDst, len));
	}
	return ret;
}

bool ConvertAmplToDB_32f(const float referAmpl, const float subLevelAmpl, const float *pSrc, float *pDst, int len, const bool bSrcNegative)
{
	bool ret =
		(referAmpl > 0.f) &&
		(subLevelAmpl > 0.f) &&
		(pSrc != nullptr) &&
		(pDst != nullptr) &&
		(len > 0);
	if (ret)
	{
		if (bSrcNegative)
			ret =
			(ippStsNoErr == ippsAbs_32f(pSrc, pDst, len)) &&
			(ippStsNoErr == ippsThreshold_32f_I(pDst, len, subLevelAmpl, ippCmpLess));
		else
			ret = ippStsNoErr == ippsThreshold_32f(pSrc, pDst, len, subLevelAmpl, ippCmpLess);

		if (ret)
			ret =
			(ippStsNoErr == ippsMulC_32f_I((1.f / referAmpl), pDst, len)) &&
			(ippStsNoErr == ippsLn_32f_I(pDst, len)) &&
			(ippStsNoErr == ippsMulC_32f_I(g_20_log10_E_32f, pDst, len));
	}
	return ret;
}

bool ConvertAmplToDB_32f_I(const float referAmpl, const float subLevelAmpl, float *pSrcDst, int len, const bool bSrcNegative)
{
	bool ret =
		(referAmpl > 0.f) &&
		(subLevelAmpl > 0.f) &&
		(pSrcDst != nullptr) &&
		(len > 0);
	if (ret)
	{
		if (bSrcNegative)
			ret = ippStsNoErr == ippsAbs_32f_I(pSrcDst, len);

		if (ret)
			ret =
			(ippStsNoErr == ippsThreshold_32f_I(pSrcDst, len, subLevelAmpl, ippCmpLess)) &&
			(ippStsNoErr == ippsMulC_32f_I((1.f / referAmpl), pSrcDst, len)) &&
			(ippStsNoErr == ippsLn_32f_I(pSrcDst, len)) &&
			(ippStsNoErr == ippsMulC_32f_I(g_20_log10_E_32f, pSrcDst, len));
	}
	return ret;
}
//------------------------------------------------------------------------------
bool ConvertPowerToDB_32f(const float referAmpl, const float subLevelPower, const float *pSrc, float *pDst, int len, const bool bSrcNegative)
{
	bool ret =
		(referAmpl > 0.f) &&
		(subLevelPower > 0.f) &&
		(pSrc != nullptr) &&
		(pDst != nullptr) &&
		(len > 0);
	if (ret)
	{
		if (bSrcNegative)
			ret =
			(ippStsNoErr == ippsAbs_32f(pSrc, pDst, len)) &&
			(ippStsNoErr == ippsThreshold_32f_I(pDst, len, subLevelPower, ippCmpLess));
		else
			ret = ippStsNoErr == ippsThreshold_32f(pSrc, pDst, len, subLevelPower, ippCmpLess);

		ret =
			(ippStsNoErr == ippsMulC_32f_I((1.f / (referAmpl * referAmpl)), pDst, len)) &&
			(ippStsNoErr == ippsLn_32f_I(pDst, len)) &&
			(ippStsNoErr == ippsMulC_32f_I(g_10_log10_E_32f, pDst, len));
	}
	return ret;
}

bool ConvertPowerToDB_32f_I(const float referAmpl, const float subLevelPower, float *pSrcDst, int len, const bool bSrcNegative)
{
	bool ret =
		(referAmpl > 0.f) &&
		(subLevelPower > 0.f) &&
		(pSrcDst != nullptr) &&
		(len > 0);
	if (ret)
	{
		if (bSrcNegative)
			ret = ippStsNoErr == ippsAbs_32f_I(pSrcDst, len);

		ret =
			(ippStsNoErr == ippsThreshold_32f_I(pSrcDst, len, subLevelPower, ippCmpLess)) &&
			(ippStsNoErr == ippsMulC_32f_I((1.f / (referAmpl * referAmpl)), pSrcDst, len)) &&
			(ippStsNoErr == ippsLn_32f_I(pSrcDst, len)) &&
			(ippStsNoErr == ippsMulC_32f_I(g_10_log10_E_32f, pSrcDst, len));
	}
	return ret;
}

bool ConvertAmplToDB_64f(const double referAmpl, const double *pSrc, double *pDst, int len, const bool bSrcNegative)
{
	bool ret =
		(referAmpl > 0.f) &&
		(pSrc != nullptr) &&
		(pDst != nullptr) &&
		(len > 0);
	if (ret)
	{
		if (bSrcNegative)
			ret =
			(ippStsNoErr == ippsAbs_64f(pSrc, pDst, len)) &&
			(ippStsNoErr == ippsMulC_64f_I((1. / referAmpl), pDst, len));
		else
			ret = ippStsNoErr == ippsMulC_64f(pSrc, (1. / referAmpl), pDst, len);

		if (ret)
			ret =
			(ippStsNoErr == ippsLn_64f_I(pDst, len)) &&
			(ippStsNoErr == ippsMulC_64f_I(g_20_log10_E_64f, pDst, len));
	}
	return ret;
}

bool ConvertAmplToDB_64f_I(const double referAmpl, double *pSrcDst, int len, const bool bSrcNegative)
{
	bool ret =
		(referAmpl > 0.f) &&
		(pSrcDst != nullptr) &&
		(len > 0);
	if (ret)
	{
		if (bSrcNegative)
			ret = ippStsNoErr == ippsAbs_64f_I(pSrcDst, len);

		if (ret)
			ret =
			(ippStsNoErr == ippsMulC_64f_I((1. / referAmpl), pSrcDst, len)) &&
			(ippStsNoErr == ippsLn_64f_I(pSrcDst, len)) &&
			(ippStsNoErr == ippsMulC_64f_I(g_20_log10_E_64f, pSrcDst, len));
	}
	return ret;
}

bool ConvertPowerToDB_64f(const double referAmpl, const double *pSrc, double *pDst, int len, const bool bSrcNegative)
{
	bool ret =
		(referAmpl > 0.f) &&
		(pSrc != nullptr) &&
		(pDst != nullptr) &&
		(len > 0);
	if (ret)
	{
		if (bSrcNegative)
			ret =
			(ippStsNoErr == ippsAbs_64f(pSrc, pDst, len)) &&
			(ippStsNoErr == ippsMulC_64f_I((1. / (referAmpl * referAmpl)), pDst, len));
		else
			ret = ippStsNoErr == ippsMulC_64f(pSrc, (1. / (referAmpl * referAmpl)), pDst, len);

		if (ret)
			ret =
			(ippStsNoErr == ippsLn_64f_I(pDst, len)) &&
			(ippStsNoErr == ippsMulC_64f_I(g_10_log10_E_64f, pDst, len));
	}
	return ret;

}

bool ConvertPowerToDB_64f_I(const double referAmpl, double *pSrcDst, int len, const bool bSrcNegative)
{
	bool ret =
		(referAmpl > 0.f) &&
		(pSrcDst != nullptr) &&
		(len > 0);
	if (ret)
	{
		if (bSrcNegative)
			ret = ippStsNoErr == ippsAbs_64f_I(pSrcDst, len);

		if (ret)
			ret =
			(ippStsNoErr == ippsMulC_64f_I((1. / (referAmpl * referAmpl)), pSrcDst, len)) &&
			(ippStsNoErr == ippsLn_64f_I(pSrcDst, len)) &&
			(ippStsNoErr == ippsMulC_64f_I(g_10_log10_E_64f, pSrcDst, len));
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>	// T - это float, double
bool ValueOverNois(
	const int nMin, const T valMin,
	const int nMax, const T valMax,
	const T factor, const T rms,
	int & xExt, T & yExt)
{
	bool ret(false);
	bool bExtIsMin(false);
	bool bExtIsMax(false);

	T val = fabs(valMin);
	if (valMax > val)
		bExtIsMax = true;
	else if (valMax < val)
		bExtIsMin = true;
	else
		bExtIsMax = nMin < nMax;

	if (bExtIsMin)
	{
		ret = (val * factor) >= rms;
		if (ret)
		{
			xExt = nMin;
			yExt = valMin;
		}
	}
	else if (bExtIsMax)
	{
		ret = (valMax * factor) >= rms;
		if (ret)
		{
			xExt = nMax;
			yExt = valMax;
		}
	}
	return ret;
}

// Поиск значимого экстремума. Исходный ряд должен быть отцентрирован.
// Значимый - это такой, значение которого больше произведения СКО на factor
bool ExtremumOverNose_32f(const float *pSrc, const int len, int & xExt, float & yExt, const float factor)
{
	bool ret = (pSrc != nullptr) && (len > 0) && (factor > 0.f);
	if (ret)
	{
		float rms(0.f), valMin(0.f), valMax(0.f);
		int nMin(0), nMax(0);
		ret =
			(ippStsNoErr == ippsStdDev_32f(pSrc, len, &rms, ippAlgHintAccurate)) &&
			(ippStsNoErr == ippsMinMaxIndx_32f(pSrc,len, &valMin, &nMin, &valMax, &nMax)) &&
			ValueOverNois(nMin, valMin, nMax, valMax, factor, rms, xExt, yExt);
	}
	if (!ret)
	{
		xExt = 0;
		yExt = 0.f;
	}
	return ret;
}

bool ExtremumOverNose_64f(const double *pSrc, const int len, int & xExt, double & yExt, const double factor)
{
	bool ret = (pSrc != nullptr) && (len > 0) && (factor > 0.f);
	if (ret)
	{
		double rms(0.f), valMin(0.f), valMax(0.f);
		int nMin(0), nMax(0);
		ret =
			(ippStsNoErr == ippsStdDev_64f(pSrc, len, &rms)) &&
			(ippStsNoErr == ippsMinMaxIndx_64f(pSrc, len, &valMin, &nMin, &valMax, &nMax)) &&
			ValueOverNois(nMin, valMin, nMax, valMax, factor, rms, xExt, yExt);
	}
	if (!ret)
	{
		xExt = 0;
		yExt = 0.;
	}
	return ret;
}
//------------------------------------------------------------------------------
// Поиск пикового значения. Ряд должен быть отцентрированным
bool FindPeak_32f(const float *pSrc, const int len, int & xPeak)
{
	bool ret = (pSrc != nullptr) && (len > 1);
	if (ret)
	{
		int xMin(0), xMax(0);
		float yMin(0.f), yMax(0.f);
		ret = ippStsNoErr == ippsMinMaxIndx_32f(pSrc, len, &yMin, &xMin, &yMax, &xMax);
		if (ret)
		{
			if (yMin != yMax)
			{
				if ((yMin < 0.f) && (yMax > 0.f))
				{
					yMin = -yMin;
					if (yMin < yMax)
						xPeak = xMax;
					else if (yMin > yMax)
						xPeak = xMin;
					else
						xPeak = min(xMin, xMax);
				}
				else if ((yMin > 0.f) && (yMax > 0.f))
				{
					xPeak = xMax;
				}
				else// (yMin < 0.f) && (yMax < 0.f)
				{
					xPeak = xMin;
				}
			}
			else
				xPeak = min(xMin, xMax);
		}
	}
	return ret;
}
//----------------------------------------------------------------------------------------------------------------------
int calcPossibleResonancesByPhase(_In_ const double* pSrcPhase, _In_ const int size, _In_ const double freq,
	_In_ const double phase, _In_ const double deltaPhase, _In_ const double f1, _In_ const double f2,
	_Out_ double** ppFreq, _Out_ double** ppQ, _Out_ int& count)
{
	*ppFreq = nullptr;
	*ppQ = nullptr;
	count = 0;

	if (nullptr == pSrcPhase)
		return -1;
	if (size <= 0)
		return -2;
	
	std::vector<std::pair<double, double>> possibleResonances;

	// границы коридора определения добротности по фазовой характеристике
	const double ph1 = phase - deltaPhase;
	const double ph2 = phase + deltaPhase;

	// pos: -1 - внизу, -2 - зашел в середину снизу, 1 - вверху, 2 - зашел в середину сверху
	int pos(-1);
	if (pSrcPhase[0] > ph2)
		pos = 1;
	else if (pSrcPhase[0] >= ph1)
		pos = -2;
	int firstInd(-1), lastInd(-1);
	for (int i = 1; i < size; ++i)
	{
		if (pos == -1)
		{
			if (pSrcPhase[i] > ph2)
			{
				pos = 1;
				firstInd = -1;
				lastInd = -1;
			}
			else if (pSrcPhase[i] >= ph1)
			{
				pos = -2;
				firstInd = i - 1;
				lastInd = -1;
			}
		}
		else if (pos == -2)
		{
			if (pSrcPhase[i] > ph2)
			{
				pos = 1;
				lastInd = i;
			}
			else if (pSrcPhase[i] < ph1)
			{
				pos = -1;
				firstInd = -1;
				lastInd = -1;
			}
		}
		else if (pos == 1)
		{
			if (pSrcPhase[i] < ph1)
			{
				pos = -1;
				firstInd = -1;
				lastInd = -1;
			}
			else if (pSrcPhase[i] <= ph2)
			{
				pos = 2;
				firstInd = i - 1;
				lastInd = -1;
			}
		}
		else if (pos == 2)
		{
			if (pSrcPhase[i] < ph1)
			{
				pos = -1;
				lastInd = i;
			}
			else if (pSrcPhase[i] > ph2)
			{
				pos = 1;
				firstInd = -1;
				lastInd = -1;
			}
		}
		
		// если нашли переход фазы, то приступаем к расчету параметров резонанса
		if (firstInd >= 0 && lastInd >= 0)
		{
			if (lastInd > firstInd)
			{
				//длина коридора в отсчетах по оси частот
				const auto len = lastInd - firstInd + 1;
				if (len > 2 && abs(pSrcPhase[lastInd] - pSrcPhase[firstInd]) < 90.)
				{
					//TRACE(L"firstInd %d, lastInd %d, %lf\n", firstInd, lastInd, pSrcPhase[lastInd] - pSrcPhase[firstInd]);
					//индекс центрального отсчета
					const auto ind = len / 2 + firstInd;
					//грубая оценка резонансной частоты
					double f = (double)ind / (double)size * freq / 2.;
					if (f1 < f && f < f2)
					{
						//частотное разрешение
						double deltaF = freq / 2. / (double)size;

						// апрроксимируем фазовую характеристику прямой в месте пересечения заданного значения фазы по точкам, 
						// попавшим в коридор, т. е. от индекса firstInd до индекса lastInd
						double b(0.), k(0.);
						if (ApproximationLine_equ(pSrcPhase + firstInd, len, k, b) && _finite(k) != 0 && k > 0)
						{
							//оценка добротности
							double q = k / deltaF * IPP_PI180 * f / 2.;
							possibleResonances.push_back(std::make_pair(f, q));
						}
					}
				}
			}
			firstInd = -1;
			lastInd = -1;
		}
	}
	count = possibleResonances.size();
	if (count > 0)
	{
		*ppFreq = new double[count];
		*ppQ = new double[count];
		for (int i = 0; i < count; ++i)
		{
			(*ppFreq)[i] = possibleResonances[i].first;
			(*ppQ)[i] = possibleResonances[i].second;
		}
	}

	return 0;
}
//----------------------------------------------------------------------------------------------------------------------
// Расчет точных резонансной и собственной частоты по оценочному значению резонансной частоты и добротности
// Параметры:
//		pSrcAmpl	- массив данных амплитудной характеристики
//		size		- размер массива данных амплитудной характеристики
//		freqADC		- частота дискретизации
//		fResSrc		- оценка значения резонансной частоты
//		q			- оценка значения добротности
//		f0			- точное значение собственной частоты
//		fResDst		- точное значение резонансной частоты
//		amplRes		- точное значение максимума амплитудной характеристики на резонансной частоте
long calcResonanceFreqs(_In_ const double* pSrcAmpl, _In_ const int size, _In_ const double freqADC, 
	_In_ const double fResSrc, _In_ const double q, _Out_ double& f0, _Out_ double& fResDst, _Out_ double& amplRes)
{
	f0 = fResDst = amplRes = NAN;

	if (nullptr == pSrcAmpl)
		return Err_NULL_Pointer;

	if (size <= 0)
		return Err_Size_Zero;

	if ((fResSrc <= 0.) || (q <= 0.))
		return Err_Value_Is_Negative;

	if (fResSrc >= (0.5 * freqADC))
		return InvalidParams;

	//частотное разрешение
	double deltaF = freqADC / 2. / (double)size;
	//оценка частотной полосы резонанса
	double df = fResSrc / q;
	// индексы начала и окончания частотной полосы
	int firstInd = (int)((fResSrc - df) / deltaF), lastInd = (int)((fResSrc + df) / deltaF);
	if (firstInd >= lastInd || 
		firstInd < 0 || 
		lastInd < 0 || 
		firstInd >= size || 
		lastInd >= size)
		return InvalidParams;

	//длина коридора в отсчетах по оси частот
	const auto len = lastInd - firstInd;
	//массивы амплитуд и фаз
	double* pAmpl = new double[len];
	double* pFreq = new double[len];
	// заполняем массивы и возводим в квадрат, чтобы получить параболу
	double valSF = 0.5 * freqADC / size;
	for (int i = 0; i < len; ++i)
	{
		int n = firstInd + i;
		double val = pSrcAmpl[n];
		pAmpl[i] = 1 / (val * val);

		val = n * valSF;
		pFreq[i] = val * val;

// 		pAmpl[i] = 1 / pSrcAmpl[firstInd + i];
// 		pAmpl[i] *= pAmpl[i];
// 		pFreq[i] = (double)(firstInd + i) / (double)size * freqADC / 2.;
// 		pFreq[i] *= pFreq[i];
	}
	// рассчитываем параболу
	double a0(0.), a1(0.), a2(0.);
	if (!ApproximationParabola_equNo(pFreq, pAmpl, len, a2, a1, a0) ||
		_finite(a2) == 0 ||
		_finite(a1) == 0 ||
		_finite(a0) == 0 ||
		a2 <= 0.)
	{
		delete[]pAmpl;
		delete[]pFreq;
		return Err_Calc;
	}
		
	//точная резонансная частота
	double f2 = -0.5 * a1 / a2;
	if (f2 <= 0.)
	{
		delete[]pAmpl;
		delete[]pFreq;
		return Err_Calc;
	}

	fResDst = sqrt(f2);
//	fResDst = sqrt(-0.5 * a1 / a2);
	if (_finite(fResDst) == 0)
	{
		delete[]pAmpl;
		delete[]pFreq;
		return Err_Calc;
	}

	amplRes = (a2 * f2 + a1) * f2 + a0;
//	amplRes = a2 * fResDst * fResDst * fResDst * fResDst + a1 * fResDst * fResDst + a0;
	amplRes = sqrt(1 / amplRes);
	if (_finite(amplRes) == 0)
	{
		delete[]pAmpl;
		delete[]pFreq;
		return Err_Calc;
	}

	double corrK = amplRes / q;
	corrK *= corrK;
	for (int i = 0; i < len; ++i)
		pAmpl[i] = (pAmpl[i] * corrK - 1.) / pFreq[i];

// 	double corrK = amplRes / q;
// 	for (int i = 0; i < len; ++i)
// 	{
// 		pAmpl[i] *= corrK * corrK;
// 		pAmpl[i] -= 1.;
// 		pAmpl[i] /= pFreq[i];
// 	}

	double b(0.), k(0.);
	if (!ApproximationLine_equNo(pFreq, pAmpl, len, k, b) ||
		_finite(k) == 0 ||
		_finite(b) == 0)
	{
		delete[]pAmpl;
		delete[]pFreq;
		return Err_Calc;
	}
	delete[]pAmpl;
	delete[]pFreq;

	f0 = 1. / sqrt(sqrt(k));
	if (_finite(f0) == 0)
		return Err_Calc;
		
	return 0;
}
//----------------------------------------------------------------------------------------------------------------------
inline const UINT getFilterOrder(_In_ const double fs, _In_ const double fadc)
{
	UINT order(0);
	if ((0. < fs) && (fs < (0.5 * fadc)))
	{
		double tmp = fs / fadc;
		if (tmp > 0.25)
			tmp = 0.5 - tmp;
		if (tmp > 0.1)
			order = 12;
		else if (tmp > 0.05)
			order = 10;
		else if (tmp > 0.02)
			order = 8;
		else if (tmp > 0.005)
			order = 6;
		else if (tmp > 0.001)
			order = 4;
		else
			order = 2;
	}
	return order;
}
//----------------------------------------------------------------------------------------------------------------------
int CalcImpRespByCrossSpectrum(_In_ const СalcImpRespState& param, _In_ const Ipp64fc* pCrossSpectrum,
	_In_ const double* pAmp2Spectr1, _In_ Ipp64fc* pMemory, _In_ const int size, _In_ const Ipp64fc gain, _Out_writes_all_(size * 2) double* pImpResp)
{
	if (nullptr == pCrossSpectrum ||
		nullptr == pAmp2Spectr1 ||
		nullptr == pMemory ||
		nullptr == pImpResp)
		return 1;
	if (size <= 1)
		return 2;

	auto tapsLen = size * 2;
	auto orderLpf = getFilterOrder(param.lpfFreq, param.freq);
	auto orderHpf = getFilterOrder(param.hpfFreq, param.freq);

	for (int i = 0; i < size; ++i)
	{
		auto i2 = tapsLen - i;
		if (i2 >= tapsLen)
			i2 = tapsLen - 1;
		double f = (double)i / (double)tapsLen * param.freq;
		if (i == 0)
			f = (double)(i + 1) / (double)tapsLen * param.freq;
		std::complex<double> value(0., 0.);
		std::complex<double> lpfCoeff(1., 0.), hpfCoeff(1., 0.);
		std::complex<double> jwLpf(0., f / param.lpfFreq), jwHpf(0., f / param.hpfFreq);
		jwHpf = 1. / jwHpf;

		if (param.needLpf && orderLpf > 0)
		{
			for (unsigned int j = 1; j <= orderLpf / 2; ++j)
			{
				std::complex<double> p = jwLpf * jwLpf - 2. * jwLpf * cos(IPP_PI * (2 * j + orderLpf - 1) / 2 / orderLpf) + 1.;
				lpfCoeff *= p;
			}
		}
		if (param.needHpf && orderHpf > 0)
		{
			for (unsigned int j = 1; j <= orderHpf / 2; ++j)
			{
				std::complex<double> p = jwHpf * jwHpf - 2. * jwHpf * cos(IPP_PI * (2 * j + orderHpf - 1) / 2 / orderHpf) + 1.;
				hpfCoeff *= p;
			}
		}

		//коэффициент передачи
		if (i == 0)
		{
			//прямая импульсная характеристика
			if (param.direction == 0)
				value.real(gain.re);
			//обратная импульсная 
			else
			{
				if (gain.re != 0.)
					value.real(1. / gain.re);
			}
		}
		else
		{
			//прямая импульсная характеристика
			if (param.direction == 0)
			{
				if (pAmp2Spectr1[i] != 0.)
				{
					if (param.noPhase)
						value.real(sqrt(pCrossSpectrum[i].re * pCrossSpectrum[i].re + pCrossSpectrum[i].im * pCrossSpectrum[i].im) / pAmp2Spectr1[i]);
					else
					{
						value.real(pCrossSpectrum[i].re / pAmp2Spectr1[i]);
						value.imag(-pCrossSpectrum[i].im / pAmp2Spectr1[i]);
					}
				}
			}
			//обратная импульсная 
			else
			{
				if (pAmp2Spectr1[i] != 0.)
				{
					auto mod = sqrt(pCrossSpectrum[i].re * pCrossSpectrum[i].re + pCrossSpectrum[i].im * pCrossSpectrum[i].im);
					if (param.noPhase)
						value.real(1. / (mod / pAmp2Spectr1[i]));
					else
					{
						value.real(pCrossSpectrum[i].re * pAmp2Spectr1[i] / mod / mod);
						value.imag(pCrossSpectrum[i].im * pAmp2Spectr1[i] / mod / mod);
					}
				}
			}
		}
		value /= hpfCoeff;
		value /= lpfCoeff;
		if (i == 0)
		{
			pMemory[i].re = value.real();
			pMemory[i].im = -value.imag();
			pMemory[i2].re = value.real();
			pMemory[i2].im = value.imag();
		}
		else
		{
			pMemory[i].re = 0.;
			pMemory[i].im = 0.;
			pMemory[i2].re = value.real() * 2.;
			pMemory[i2].im = value.imag() * 2.;
		}
	}

	auto order = int(log2(tapsLen) + 0.5);
	if (pow(2, order) == tapsLen)
	{
		int sizeFFTSpec(0), sizeFFTInitBuf(0), sizeFFTWorkBuf(0);
		ippsFFTGetSize_C_64fc(order, IPP_DIV_INV_BY_N, ippAlgHintNone, &sizeFFTSpec, &sizeFFTInitBuf, &sizeFFTWorkBuf);
		Ipp8u* pFFTSpecBuf = ippsMalloc_8u(sizeFFTSpec);
		Ipp8u* pFFTWorkBuf = ippsMalloc_8u(sizeFFTWorkBuf);
		Ipp8u* pFFTInitBuf = ippsMalloc_8u(sizeFFTInitBuf);
		IppsFFTSpec_C_64fc* pFftSpec(nullptr);
		ippsFFTInit_C_64fc(&pFftSpec, order, IPP_DIV_INV_BY_N, ippAlgHintNone, pFFTSpecBuf, pFFTInitBuf);
		if (pFFTInitBuf)
			ippsFree(pFFTInitBuf);
		ippsFFTInv_CToC_64fc_I(pMemory, pFftSpec, pFFTWorkBuf);
		if (pFFTSpecBuf)
			ippsFree(pFFTSpecBuf);
		if (pFFTWorkBuf)
			ippsFree(pFFTWorkBuf);
	}
	else
	{
		int sizeDFTSpec(0), sizeDFTInitBuf(0), sizeDFTWorkBuf(0);
		ippsDFTGetSize_C_64fc(tapsLen, IPP_DIV_INV_BY_N, ippAlgHintNone, &sizeDFTSpec, &sizeDFTInitBuf, &sizeDFTWorkBuf);
		IppsDFTSpec_C_64fc* pDftSpec = (IppsDFTSpec_C_64fc*)ippsMalloc_8u(sizeDFTSpec);
		Ipp8u* pDftInitBuf = ippsMalloc_8u(sizeDFTInitBuf);
		Ipp8u* pDftWorkBuf = ippsMalloc_8u(sizeDFTWorkBuf);
		ippsDFTInit_C_64fc(tapsLen, IPP_DIV_INV_BY_N, ippAlgHintNone, pDftSpec, pDftInitBuf);
		if (pDftInitBuf)
			ippsFree(pDftInitBuf);
		ippsDFTInv_CToC_64fc(pMemory, pMemory, pDftSpec, pDftWorkBuf);
		if (pDftWorkBuf)
			ippsFree(pDftWorkBuf);
		if (pDftSpec)
			ippsFree(pDftSpec);
	}

	if (param.symm)
	{
		for (int i = 0; i < size; ++i)
			pImpResp[i] = pMemory[size - i - 1].re;
		for (int i = 1; i < size; ++i)
			pImpResp[i + size - 1] = pMemory[tapsLen - i].re;
	}
	else
	{
		for (int i = 0; i < size; ++i)
			pImpResp[i] = pMemory[size - i - 1].im;
		for (int i = 1; i < size; ++i)
			pImpResp[i + size - 1] = pMemory[tapsLen - i].im;
	}

	return 0;
}
//----------------------------------------------------------------------------------------------------------------------
int CalcImpRespByCrossSpectrum(_In_ const СalcImpRespState& param, _In_ const double* pH12, const double* pPhase,
	_In_ Ipp64fc* pMemory, _In_ const int size, _Out_writes_all_(size * 2) double* pImpResp)
{
	if (nullptr == pH12 ||
		nullptr == pPhase ||
		nullptr == pMemory ||
		nullptr == pImpResp)
		return 1;
	if (size <= 1)
		return 2;

	auto tapsLen = size * 2;
	auto orderLpf = getFilterOrder(param.lpfFreq, param.freq);
	auto orderHpf = getFilterOrder(param.hpfFreq, param.freq);

	for (int i = 0; i < size; ++i)
	{
		auto i2 = tapsLen - i;
		if (i2 >= tapsLen)
			i2 = tapsLen - 1;
		double f = (double)i / (double)tapsLen * param.freq;
		if (i == 0)
			f = (double)(i + 1) / (double)tapsLen * param.freq;
		std::complex<double> value(0., 0.);
		std::complex<double> lpfCoeff(1., 0.), hpfCoeff(1., 0.);
		std::complex<double> jwLpf(0., f / param.lpfFreq), jwHpf(0., f / param.hpfFreq);
		jwHpf = 1. / jwHpf;

		if (param.needLpf && orderLpf > 0)
		{
			for (unsigned int j = 1; j <= orderLpf / 2; ++j)
			{
				std::complex<double> p = jwLpf * jwLpf - 2. * jwLpf * cos(IPP_PI * (2 * j + orderLpf - 1) / 2 / orderLpf) + 1.;
				lpfCoeff *= p;
			}
		}
		if (param.needHpf && orderHpf > 0)
		{
			for (unsigned int j = 1; j <= orderHpf / 2; ++j)
			{
				std::complex<double> p = jwHpf * jwHpf - 2. * jwHpf * cos(IPP_PI * (2 * j + orderHpf - 1) / 2 / orderHpf) + 1.;
				hpfCoeff *= p;
			}
		}

		//коэффициент передачи
		if (i == 0)
		{
			//прямая импульсная характеристика
			if (param.direction == 0)
				value.real(pH12[i]);
			//обратная импульсная 
			else
			{
				if (pH12[i] != 0.)
					value.real(1. / pH12[i]);
			}
		}
		else
		{
			//прямая импульсная характеристика
			if (param.direction == 0)
			{
				if (param.noPhase)
					value.real(pH12[i]);
				else
				{
					value.real(pH12[i] * cos(pPhase[i] * IPP_PI180));
					value.imag(-pH12[i] * sin(pPhase[i] * IPP_PI180));
				}
			}
			//обратная импульсная 
			else
			{
				if (param.noPhase)
					value.real(1. / pH12[i]);
				else
				{
					value.real(pH12[i] * cos(pPhase[i] * IPP_PI180) / pH12[i] / pH12[i]);
					value.imag(pH12[i] * sin(pPhase[i] * IPP_PI180) / pH12[i] / pH12[i]);
				}
			}
		}
		value /= hpfCoeff;
		value /= lpfCoeff;
		if (i == 0)
		{
			pMemory[i].re = value.real();
			pMemory[i].im = -value.imag();
			pMemory[i2].re = value.real();
			pMemory[i2].im = value.imag();
		}
		else
		{
			pMemory[i].re = 0.;
			pMemory[i].im = 0.;
			pMemory[i2].re = value.real() * 2.;
			pMemory[i2].im = value.imag() * 2.;
		}
	}

	auto order = int(log2(tapsLen) + 0.5);
	if (pow(2, order) == tapsLen)
	{
		int sizeFFTSpec(0), sizeFFTInitBuf(0), sizeFFTWorkBuf(0);
		ippsFFTGetSize_C_64fc(order, IPP_DIV_INV_BY_N, ippAlgHintNone, &sizeFFTSpec, &sizeFFTInitBuf, &sizeFFTWorkBuf);
		Ipp8u* pFFTSpecBuf = ippsMalloc_8u(sizeFFTSpec);
		Ipp8u* pFFTWorkBuf = ippsMalloc_8u(sizeFFTWorkBuf);
		Ipp8u* pFFTInitBuf = ippsMalloc_8u(sizeFFTInitBuf);
		IppsFFTSpec_C_64fc* pFftSpec(nullptr);
		ippsFFTInit_C_64fc(&pFftSpec, order, IPP_DIV_INV_BY_N, ippAlgHintNone, pFFTSpecBuf, pFFTInitBuf);
		if (pFFTInitBuf)
			ippsFree(pFFTInitBuf);
		ippsFFTInv_CToC_64fc_I(pMemory, pFftSpec, pFFTWorkBuf);
		if (pFFTSpecBuf)
			ippsFree(pFFTSpecBuf);
		if (pFFTWorkBuf)
			ippsFree(pFFTWorkBuf);
	}
	else
	{
		int sizeDFTSpec(0), sizeDFTInitBuf(0), sizeDFTWorkBuf(0);
		ippsDFTGetSize_C_64fc(tapsLen, IPP_DIV_INV_BY_N, ippAlgHintNone, &sizeDFTSpec, &sizeDFTInitBuf, &sizeDFTWorkBuf);
		IppsDFTSpec_C_64fc* pDftSpec = (IppsDFTSpec_C_64fc*)ippsMalloc_8u(sizeDFTSpec);
		Ipp8u* pDftInitBuf = ippsMalloc_8u(sizeDFTInitBuf);
		Ipp8u* pDftWorkBuf = ippsMalloc_8u(sizeDFTWorkBuf);
		ippsDFTInit_C_64fc(tapsLen, IPP_DIV_INV_BY_N, ippAlgHintAccurate, pDftSpec, pDftInitBuf);
		if (pDftInitBuf)
			ippsFree(pDftInitBuf);
		ippsDFTInv_CToC_64fc(pMemory, pMemory, pDftSpec, pDftWorkBuf);
		if (pDftWorkBuf)
			ippsFree(pDftWorkBuf);
		if (pDftSpec)
			ippsFree(pDftSpec);
	}

	if (param.symm)
	{
		for (int i = 0; i < size; ++i)
			pImpResp[i] = pMemory[size - i - 1].re;
		for (int i = 1; i < size; ++i)
			pImpResp[i + size - 1] = pMemory[tapsLen - i].re;
	}
	else
	{
		for (int i = 0; i < size; ++i)
			pImpResp[i] = pMemory[size - i - 1].im;
		for (int i = 1; i < size; ++i)
			pImpResp[i + size - 1] = pMemory[tapsLen - i].im;
	}

	return 0;
}
//----------------------------------------------------------------------------------------------------------------------