//------------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------------
#define _USE_MATH_DEFINES
#include "afx.h"
#include "math.h"
#include <vector>
#include <complex>
#include <algorithm>
#include <limits>
#include <Intel\ipp\ipps.h>	// расчёт SNR
#include <ZetErrorCodes.h>
//------------------------------------------------------------------------------
//ускорение свободного падения
#define M_g						9.80665f;
// константы для перевода градусов в радианы и обратно
#define M_180_PI				57.295779513082323
#define M_PI_180				0.017453292519943295
#define Mf_180_PI				(float)M_180_PI
#define Mf_PI_180				(float)M_PI_180
// кол-во секунд в сутках и обратная величина
#define M_NumberSecInDay		86400
#define M_SecAsPartOfDay		1.1574074074074073e-005
// золотое сечение
#define M_GoldenSection			1.6180339887498949
#define M_GoldenSection_1		0.6180339887498949
// sqrt(M_E)
#define M_SQRT_E				1.6487212707001282
//------------------------------------------------------------------------------
enum TypeExtremum
{
	typeExtr_Min = -1,			// минимум
	typeExtr_Any,				// любой
	typeExtr_Max				// максимум
};
//------------------------------------------------------------------------------
// Преобразование из радиан в градусы
template <class T>				// T - это float, double
T GetRadian(T degree)
{
	return (T)M_PI_180 * degree;
}
//------------------------------------------------------------------------------
// Преобразование из градусов в радианы
template <class T>				// T - это float, double
T GetDegree(T radian)
{
	return (T)M_180_PI * radian;
}
//------------------------------------------------------------------------------
// Вычисление синуса от аргумента в градусах
template <class T>				// T - это float, double
T sind(T degree)
{
	return sin( GetRadian(degree) );
}
//------------------------------------------------------------------------------
// Вычисление косинуса от аргумента в градусах
template <class T>				// T - это float, double
T cosd(T degree)
{
	return cos( GetRadian(degree) );
}
//------------------------------------------------------------------------------
// Возвращает знак числа
template <class T>				// T - это любые числа
T sign(T val)
{
	T ret;
	if (val > (T)0)
		ret = (T)1;
	else if (val < (T)0)
		ret = (T)-1;
	else
		ret = (T)0;
	return ret;
}
//------------------------------------------------------------------------------
// Представление широты или долготы в строковом виде
template <class T>				// T - это bool, BOOL
CString ConvertGeodeticToString(
	const double coordinate,	// значение широты или долготы
	const T bLatitude)			// широта или долгота
{
	CString str;
	int nDegree = int(coordinate);	// градусы
	double val = (coordinate - (double)nDegree) * 60;

	int nMin = int(val);			// минуты
	val = (val - (double)nMin) * 60;

	int nSec = int(val);			// секунды

	// округление до целой секунды
	val = (val - (double)nSec) + 0.5;
	if (int(val) > 0)
	{
		if (nSec != 59)
			++nSec;
		else
		{
			if (nMin != 59)
				++nMin;
			else
			{
				++nDegree;
				if (bLatitude)
				{
					if (nDegree > 90)
						nDegree = 90;
					else if (nDegree < -90)
						nDegree = -90;
				} 
				else
				{
					if (nDegree > 180)
						nDegree -= 360;
					else if (nDegree < -180)
						nDegree += 360;
				}
			}
		}
	}
	str.Format(L"%d°%02d′%02d″", nDegree, nMin, nSec);
	return str;
}
//------------------------------------------------------------------------------	
template <class T>				// T - это float или double
// Вместо if (val1 == val2),
// лучше  if ( IsEqual(val1, val2, precision) )
// precision - относительная точность (не в процентах, а в разах)
bool IsEqual(const T &val1, const T &val2, const T &precision)
{
	return fabs(val1 - val2) <=
			( precision * (T)0.5 * fabs(val1 + val2) );
}
//------------------------------------------------------------------------------	
template <class T>				// T - это float или double
// Вместо if (val1 != val2),
// лучше  if ( IsNotEqual(val1, val2, precision) )
// precision - относительная точность, (не в процентах, а в разах)
bool IsNotEqual(const T &val1, const T &val2, const T &precision)
{
	return fabs(val1 - val2) >
			( precision * (T)0.5 * fabs(val1 + val2) );
}
//------------------------------------------------------------------------------	
// Округление числа до заданного кол-ва значащих цифр.
template <class T>					// T - это float или double
CString DigitalNotation(
	T &val,							// округляемое число
	const long numDigital = 4)		// кол-во значащих цифр в результате
{
	CString ret, form;
	if (_isnan(val) == 0)
	{
		long numDig =				// кол-во значащих цифр в результате
			(numDigital >= 0) ? numDigital : 4;
		bool bMinus = val < T(0);
		val = fabs(val);
		if ( val > T(0) )			// 0 округлять не надо + log(0)
		{	// m - порядок числа:
			// 21 => m = 2; 1.2 => m = 1; 0.12 => m = 0; 0.012 => m = -1
			int m = int( floor( log10( (double)val ) ) ) + 1;
			T dx = (T)pow(10., m - numDig);
			val = T( int(val / dx + 0.5) ) * dx;
			if (bMinus)
				val = -val;

			// формирование строки ----------------------
			if (m > 0)
			{
				if (m < 6)
				{
					int mm = numDig - m;
					if (mm > 0)
						form.Format(L"%%.%df", mm);
					else
						form = L"%.0f";
				}
				else
				{
					int stp = (m - 1) / 3;
					for (int i=0; i<stp; ++i)
						val *= (T)0.001;						
					stp *= 3;
					int bef = m - stp;
					if (bef < 0)
						bef = 0;
					int aft = numDig - bef;
					if (aft < 0)
						aft = 0;
					bef += aft;
					form.Format(L"%%%d.%df*10^+%d", bef, aft, stp);
				//	form.Format(L"%%%d.%df*E+%d", bef, aft, stp);
				}
			}
			else if (m > -3)		// от -2 до 0
					form.Format(L"%%.%df", -m + numDig);
			else	// m <= -3
			{
				int stp = -m / 3 + 1;
				for (int i=0; i<stp; ++i)
					val *= 1000;
				stp *= -3;
				int aft = -m + numDig + stp;
				if (aft < 0)
					aft = 0;
				form.Format(L"%%.%df*10^%d", aft, stp);
			//	form.Format(L"%%.%df*E%d", aft, stp);
			}
		}
		else
		{// val = 0
			form.Format(L"%%.%df", numDig);
		}
		// завершение ------------------------------------------
		ret.Format(form, val);
	}
	else
		ret = L"*****";
	return ret;
}
//------------------------------------------------------------------------------	
// Округление погрешности или СКО по законам метрологии
template <class T>					// T - это float или double
CString UncertaintyNotation(T &val)
{
	CString ret;
	if (_isnan(val) == 0)
	{
		val = fabs(val);
		if ( val > T(0) )
		{// округляемое число не может равняться 0
			T dx = T(pow(10., int(floor(log10((double)val))) - 1));
			if ( (int)floor(val/dx) >= 30 )
				ret = DigitalNotation(val, 1);
			else
				ret = DigitalNotation(val, 2);
		}
	}
	return ret;
}
//------------------------------------------------------------------------------	
template <class T>					// T - это float или double
bool CalcMiddle(std::vector<T> &vect, T &middle)
{
	bool ret = !vect.empty();
	if (ret)
	{
		middle = 0;
		for (auto it = vect.cbegin(), ie = vect.cend(); it != ie; ++it)
			middle += *it;
		middle /= (T)vect.size();
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>					// T - это float или double
bool CalcSKO(std::vector<T> &vect, T &sko, T &middle)
{
	UINT size = vect.size() - 1;
	bool ret = size > 0;
	if (ret)
	{
		CalcMiddle(vect, middle);	// вернётся true, не проверяю
		sko = 0;
		for (auto it = vect.cbegin(), ie = vect.cend(); it != ie; ++it)
		{
			T a = *it - middle;
			sko += a * a;
		}
		sko = sqrt( sko / (T)size );
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>					// T - это float или double
bool RemovalEjectionsFromManyPoints(std::vector<T> &vect)
{// Удаление выбросов. Правило четырёх!
// Эффективно при большом размере вектора.
	bool ret = false;	// возвращаемое значение: true  - выбросы найдены
//  bool flag; исправлена из-за исправления ошибки найденной cppcheck 2014.09.04
	if (vect.size() > 4)
	{
		bool flag = false;
		do 
		{
			T cko, middle;
			CalcSKO(vect, cko, middle);	// вернётся true, не проверяю
			T delta = 4 * cko;			// Правило четырёх!
			for (auto it = vect.begin(), ie = vect.end(); it != ie; ++it)
			{
				if (delta < fabs(*it - middle))
				{
					vect.erase(it);
					flag = true;
					break;
				}
			}
		} while (flag);
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>					// T - это float или double
bool RemovalOneEjectionFromSmallPoints(std::vector<T> &vect)
{// Удаление одного выброса. Правило четырёх!
// Функция предназначена для векторов малой размерности
	bool ret = vect.size() > 2;
	if (ret)
	{
		std::vector<T> vectTmp = vect;
		std::sort( vectTmp.begin(), vectTmp.end() );
		std::vector<T> vectTmp1 = vectTmp;
		// работа с минимальной точкой
		auto it = vectTmp1.begin();
		T valCur = *it;
		T sko, mid;
		vectTmp1.erase(it);
		CalcSKO(vectTmp1, sko, mid);		// вернёт true, не проверяю
		ret = fabs(valCur - mid) > (4 * sko);
		if (!ret)
		{// работа с максимальной точкой
			vectTmp1 = vectTmp;
			it = vectTmp1.end();
			--it;
			valCur = *it;
			vectTmp1.erase(it);
			CalcSKO(vectTmp1, sko, mid);		// вернёт true, не проверяю
			ret = fabs(valCur - mid) > (4 * sko);
		}
		if (ret)
		{// надо удалить
			auto it = vect.begin();
			for (auto ie = vect.end(); it != ie; ++it)
			{
				if (valCur == *it)
					break;
			}
			vect.erase(it);
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
// Расчет среднеарифметического на лету
// T - это float или double
// Z - целое без знака (BYTE, WORD, DWORD, UINT) или со знаком, но > 0
// n - номер значения val (т.е. счет от 1)
// При n = 1 автоматическое обнуление предыдущего среднего
template <class T, class Z>
void CalcAverageOnFly(T val, Z n, T &average)
{
	// При n = 1 автоматическое обнуление предыдущего среднего
	// расчеты в double для увеличения точности
	average = (n > 0) ? (T)(((n - 1) * (double)average + val) / n) : val;
//	average += (val - average) / n; - так автоматическое обнуление плохо работает
}

// Расчет среднеарифметического на лету
// T - это float или double
// Z - целое без знака (BYTE, WORD, DWORD, UINT) или со знаком, но > 0
// n - номер значения val (т.е. счет от 1)
// При n = 1 автоматическое обнуление предыдущего среднего
// Данная функция для n > 0 (но без проверки этого условия)
template <class T, class Z>
void CalcAverageOnFly_Ex(T val, Z n, T &average)
{
	// расчеты в double для увеличения точности
	average = (T)(((n - 1) * (double)average + val) / n);
//	average += (val - average) / n; - так автоматическое обнуление плохо работает
}

//------------------------------------------------------------------------------
//Расчет времени начала расчета данных
//Time - текущее время по каналу
//Delta - время усреднения
template <class T>					// T - это float или double
T GetStartCalcTime(T Time, T Delta)
{
	T StartTime(T(0.));
	//Если усреднение меньше 1 секунды,
	//берем ближайшее предыдущее время, кратное секунде
	if (Delta <= T(1.))
		StartTime = floor(Time);
	else
	{
		//Если усреднение не кратно целой секунде,
		//берем ближайшее предыдущее время, кратное секунде
		if (Delta - long long(Delta) != T(0.))
			StartTime = floor(Time);
		else
		{
			//Если усреднение не кратно часу,
			//берем ближайшее предыдущее время, кратное секунде
			if (long long(T(3600.) / Delta) * Delta != T(3600.))
				StartTime = floor(Time);
			//иначе рассчитываем правильное время старта
			else
			{
				StartTime = floor(Time / Delta) * Delta;
			}
		}
	}

	TRACE(L"StartCalc = %lf\n", StartTime);
	return StartTime;
}
//------------------------------------------------------------------------------
// Функция проверяет правильность ввода чисел, имеющих некоторое ограничение
enum TypeValue
{// Типы значений, вводимых в Edit
	tv_any,				// любое значение
	tv_more,			// >  valThreshold
	tv_more_or_equal,	// >= valThreshold
	tv_less,			// <= valThreshold
	tv_less_or_equal,	// <  valThreshold
};
template <class T>	// T - это float или double
void SetTrueValueInEdit(
	CWnd *pWnd, CString format, T &valOld,
	TypeValue type, T valThreshold = 0)
{
	bool bOk(false);
	int numPoint(0);
	int numMinus(0);
	double x(0.);
	CString str;

	pWnd->GetWindowText(str);
	str.Trim(L" ");
	str.Replace(L',', L'.');
	for (int i = 0, n = str.GetLength(); i < n; ++i)
	{
		auto simb = str.GetAt(i);
		if (simb == L'.')
			bOk = ++numPoint < 2;
		else if (simb == L'-')
			bOk = ++numMinus < 2;
		else
			bOk = (L'0' <= simb) && (simb <= L'9');

		if (!bOk)
			break;
	}

	// если всё хорошо, то проверка на "-"
	if (bOk && (numMinus == 1))
		bOk = str.GetAt(0) == L'-';
	
	// если всё хорошо, проверка типа
	if (bOk)
	{
		x = _wtof(str);
		switch (type)
		{
	//	case tv_any:	ничего не надо
	//	break;
		case tv_more:
			bOk = x > (double)valThreshold;
			break;
		case tv_more_or_equal:
			bOk = x >= (double)valThreshold;
			break;
		case tv_less:
			bOk = x < (double)valThreshold;
			break;
		case tv_less_or_equal:
			bOk = x <= (double)valThreshold;
			break;
		}
	}
	// завершение --------------------------
	if (bOk)
	{
		valOld = (T)x;
	}
	else
	{
		str.Format(format, valOld);
		pWnd->SetWindowText(str);
	}
}
//------------------------------------------------------------------------------
// Функция переводит число с плавающей запятой в строку в зависимости от его
// величины
template <class T>	// T - это float или double
CString ValueToString(T value)
{
	CString sFormat(L"");
	T val = abs(value);
	
	if (val < 0.000001)
		sFormat.Format(L"%.8f", value);
	else
	{
		if (val < 0.00001)
			sFormat.Format(L"%.7f", value);
		else
		{
			if (val < 0.0001)
				sFormat.Format(L"%.6f", value);
			else
			{
				if (val < 0.001)
					sFormat.Format(L"%.5f", value);
				else
				{
					if (val < 0.01)
						sFormat.Format(L"%.4f", value);
					else
					{
						if (val < 0.1)
							sFormat.Format(L"%.3f", value);
						else
						{
							if (val < 10.)
								sFormat.Format(L"%.2f", value);
							else
							{
								if (val < 10000.)
									sFormat.Format(L"%.1f", value);
								else
								{
									sFormat.Format(L"%.0f", value);
								}
							}
						}
					}
				}
			}
		}
	}

	return sFormat;
}
//------------------------------------------------------------------------------
// Шаблонная функция, рассчитывающая гистограмму по исходному ряду.
// При sizeHistogram = 0 кол-во интервалов гистограммы определяется по
// размеру исходных данных.
// Середина первого интервала гистограммы - это минимальное значение в данных.
// Середина последнего интервала гистограммы - это максимальное значение в данных.
template <class T>			// T - это float или double, а также их производные
bool Histogram(
	const T *padd,				// исходные данные
	const int size,				// их размер
	std::vector<int> &histogram,	// гистограмма
	T &valItem0Histogram,	// величина середины 0-ого интервала гистограммы
	T &widthItemHistogram,	// ширина интервалов гистограммы
	int sizeHistogram = 0)	// желаемое кол-во интервалов гистограммы
{
	if (sizeHistogram > size)
		return false;
	// определяю граничные величины и кол-во не NaN -------------------
	T amax = std::numeric_limits<T>::lowest();
	T amin = -amax;
	int number(0);
	for (int i = 0; i < size; i++)
	{
		T a0 = padd[i];
		if (_isnan(a0) == 0)
		{
			++number;
			if (amin > a0)
				amin = a0;
			if (amax < a0)
				amax = a0;
		}
	}
	bool ret = number > 1;
	if (ret)
	{
		if (amax > amin)
		{// параметры и данные гистограммы ------------------------------------
			if (sizeHistogram == 0)
			{// эмпирическая формула из книги Новицкого
				number = RoundToInt(2 * floor(pow(number, 0.4f)));
			//	number = 2 * (int)floor(pow((float)number, 0.4f));
				number |= 1;
			}
			else
				number = sizeHistogram;
			if (number < 3)	// минимальное нечетное число больше 1
				number = 3;
			histogram.resize(number);

			widthItemHistogram = (amax - amin) / (number - 1);	// именно (num - 1)
			valItem0Histogram = amin;

			T a0 = amin - T(0.5) * widthItemHistogram;
			ZeroMemory(histogram.data(), number * sizeof(int));//обязательно!!!
			// т.к. Т не int и amax > amin, то widthItemHistogram > 0
			for (int i = 0; i < size; i++)
			{
				T val = padd[i];
				if (_isnan(val) == 0)
				{
					int m = (int)floor((val - a0) / widthItemHistogram);
					if (m < 0)
						m = 0;
					else if (m >= number)
						m = number - 1;
					histogram[m]++;
				}
			}
		}
		else if (amax == amin)
		{// исходные данные - это одно и тоже число
			histogram.resize(1);
			histogram[0] = number;
			valItem0Histogram = amin;
			widthItemHistogram = T(0);
		}
		else
		{// все данные - это NaN
			histogram.clear();
			widthItemHistogram = T(0);
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
// Шаблонная функция, рассчитывающая гистограмму по исходному ряду.
// При sizeHistogram = 0 кол-во интервалов гистограммы определяется по
// размеру исходных данных.
// Середина первого интервала гистограммы - это минимальное значение в данных.
// Середина последнего интервала гистограммы - это максимальное значение в данных.
template <class T>				// T - это float или double, а также их производные
bool HistogramInterval(
	const T *padd,				// исходные данные
	const int size,				// их размер
	const int sizeHistogram,	// кол-во интервалов гистограммы
	const T leftGran,			// левая граница гистограммы
	const T rightGran,			// правая граница гистограммы
	std::vector<int> &histogram,// гистограмма
	T &valItem0Histogram,		// величина середины 0-ого интервала гистограммы
	T &widthItemHistogram)		// ширина интервалов гистограммы	
{
	if ((sizeHistogram > size) || (rightGran <= leftGran))
		return false;
	// определяю кол-во не NaN -------------------
	int number(0);
	for (int i = 0; i < size; i++)
	{
		T a0 = padd[i];
		if (!_isnan(a0))
			++number;
	}
	bool ret = number > 1;
	if (ret)
	{
		number = sizeHistogram;
		if (number < 3)	// минимальное нечетное число больше 1
			number = 3;
		histogram.resize(number);

		widthItemHistogram = (rightGran - leftGran) / number;
		valItem0Histogram = leftGran + widthItemHistogram / 2;

		T a0 = leftGran;
		ZeroMemory(histogram.data(), number * sizeof(int));//обязательно!!!
		for (int i = 0; i < size; i++)
		{
			T val = padd[i];
			if (!_isnan(val))
			{
				if ((val < leftGran) || (val > rightGran))
					continue;

				int m = (int)floor((val - a0) / widthItemHistogram);
				if (m > number - 1)		// попали на правую границу
					m = number - 1;
				histogram[m]++;
			}
		}
	}

	return ret;
}
//------------------------------------------------------------------------------
// Возврат отношения, большего 1
template <class T>			// T - это float или double, а также мх производные
T CalcRatio2(const T val1, const T val2)
{
	T ret(0);
	if ((val1 > (T)0) && (val2 > (T)0))
	{
		if (val1 > val2)
			ret = val1 / val2;
		else
			ret = val2 / val1;
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
CString StringFromRes(T nID)
{
	CString str;
	if (str.LoadString((UINT)nID) == FALSE)
		str.Empty();
	return str;
}
//------------------------------------------------------------------------------
// Расчет отношения сигнал/шум (отношение двух СКЗ)
template <class T>
float CalcFloatSNR(
	const float *pDataSignal, const T sizeSignal,
	const float *pDataNoise, const T sizeNoise,
	float *pDataTmp, const bool dB = true)
{
	bool flag = (pDataSignal != nullptr) && (pDataNoise != nullptr) &&
		(pDataTmp != nullptr) && (sizeSignal > 0) && (sizeNoise > 0);
	float signal(0.f), noise(0.f);
	if (flag)
	{
		flag =
			(ippsSqr_32f(pDataSignal, pDataTmp, (int)sizeSignal) == ippStsNoErr) &&
			(ippsMean_32f(pDataTmp, (int)sizeSignal, &signal, ippAlgHintFast) == ippStsNoErr) &&
			(ippsSqr_32f(pDataNoise, pDataTmp, (int)sizeNoise) == ippStsNoErr) &&
			(ippsMean_32f(pDataTmp, (int)sizeNoise, &noise, ippAlgHintFast) == ippStsNoErr) &&
			(noise > 0.f);
	}
	float ret = flag ? (signal / noise) : 0.f;
	if (flag && dB)
		ret = 10.f * log10(ret);
	return ret;
}
//------------------------------------------------------------------------------
// Расчет пикового значения
template <class T>
bool GetPeakValue_32f(
//	bool GetFloatPeakValue(
	const float *pData, const T size,
	float & peakValue
	/*, const bool bAmpl = false*/)
{
	int len = (int)size;
	bool ret = (pData != nullptr) && (len > 0);
	if (ret)
	{
		ret = ippsNorm_Inf_32f(pData, len, &peakValue) == ippStsNoErr;
		//if (bAmpl)
		//{
		//	ret = ippsMax_32f(pData, len, &peakValue) == ippStsNoErr;
		//}
		//else
		//{
		//	float mi(0.f), ma(0.f);
		//	ret = ippsMinMax_32f(pData, len, &mi, &ma) == ippStsNoErr;
		//	if (ret)
		//	{
		//		mi = fabsf(mi);
		//		ma = fabsf(ma);
		//		peakValue = (ma > mi) ? ma : mi;
		//	}
		//}
	}
	if (!ret)
		peakValue = 0.f;
	return ret;
}
//------------------------------------------------------------------------------
// Расчет средне квадратичного значения
template <class T>
bool GetRMS_32f(const float *pData, const T size, float & snr)
{
	int len = (int)size;
	bool ret = (pData != nullptr) && (len > 0);
	if (ret)
	{
		ret = ippsNorm_L2_32f(pData, len, &snr) == ippStsNoErr;
		if (ret)
			snr /= sqrt((float)len);
	}
	if (!ret)
		snr = 0.f;
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
// Шаблонная функция округления чисел (положительных и отрицательных)
// до ближайшего целого числа.
int RoundToInt(const T value)		// округляемое число - float, double, ...
{
	int ret = (int)floor(fabs(value) + (T)0.5);
	if (value < (T)0)
		ret = -ret;
	return ret;
}
//------------------------------------------------------------------------------
template <class T>	// T - это float или double
// Округление числа до N значащих цифр после десятичной точки.
// valueSrc - округляемое значение;
// valueDst - округлённое значение;
// N - количество требуемых разрядов после десятичной точки.
//
// Возвращается флаг, что округление выполнено.
bool RoundToN_DigitsAfterPoint(const T valueSrc, T & valueDst, const int N)
{
	bool ret = (N > 0) && (_finite(valueSrc) != 0);
	if (ret)
	{
		CString str, format;
		format.Format(L"%%.%df", N);
		str.Format(format, valueSrc);
		valueDst = (T)_wtof(str);
	}
	else
		valueDst = valueSrc;
		return ret;
}
//------------------------------------------------------------------------------
template <class T>	// T - это float или double
// Округление числа до N значащих цифр после десятичной точки и
// преобразование округлённого значения в строку.
// valueSrc - округляемое значение;
// valueDst - округлённое значение;
// N - количество требуемых разрядов после десятичной точки;
// sValue - строка с округленным значением.
//
// Возвращается флаг, что округление выполнено.
bool RoundToN_DigitsAfterPoint(const T valueSrc, T & valueDst, const int N, CString & sValue)
{
	bool ret = (N >= 0) && (_finite(valueSrc) != 0);
	if (ret)
	{
		CString format;
		format.Format(L"%%.%df", N);
		sValue.Format(format, valueSrc);
		valueDst = (T)_wtof(sValue);
	}
	else
	{
		sValue.Format(L"%g", valueSrc);
		valueDst = valueSrc;
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>	// T - это float или double
// Расчет порядка числа, которое больше 0:
// 21 => m = 2; 1.2 => m = 1; 0.12 => m = 0; 0.012 => m = -1
int CalcOrderOfValue(T value)
{
	return (int)(floor(log10(value))) + 1;
}
//------------------------------------------------------------------------------
template <class T>	// T - это float или double
// Округление числа до N значащих цифр.
// valueSrc - округляемое значение, может быть больше, меньше или = 1;
// valueDst - округлённое значение;
// N - требуемое кол-во значащих цифр.
//
// Возвращается флаг, что округление выполнено.
bool RoundToN_SignificantDigits(const T valueSrc, T & valueDst, const int N)
{
	bool ret = (N > 0) && (_finite(valueSrc) != 0);
	if (ret)
	{
		bool bMinus = valueSrc < T(0);
		valueDst = bMinus ? fabs(valueSrc) : valueSrc;
		int m = CalcOrderOfValue(valueDst);
		double dx = pow(10., m - N);
		valueDst = (T)(((int)floor((double)valueDst / dx + 0.5)) * dx);
		if (bMinus)
			valueDst = -valueDst;
	}
	else
		valueDst = valueSrc;
	return ret;
}
//------------------------------------------------------------------------------
template <class T>	// T - это float или double
// Округление числа до N значащих цифр и
// преобразование округлённого значения в строку.
// valueSrc - округляемое значение, может быть больше, меньше или = 1;
// valueDst - округлённое значение;
// str - строка с округленным значением;
// N - требуемое кол-во значащих цифр.
//
// Возвращается флаг, что округление выполнено.
bool RoundToN_SignificantDigits(const T valueSrc, T & valueDst, const int N, CString & sValue)
{
	bool ret = (N > 0) && (_finite(valueSrc) != 0);
	if (ret)
	{
		bool bMinus = valueSrc < T(0);
		valueDst = bMinus ? fabs(valueSrc) : valueSrc;
		int nOrder = CalcOrderOfValue(valueDst);
		double dx = pow(10., nOrder - N);
		valueDst = (T)(((int)floor((double)valueDst / dx + 0.5)) * dx);

		// формирование строки
		sValue.Format(L"%f", valueDst);
		int i(0), len = sValue.GetLength();
		int numCur(0);
		bool bFindPoint(false);
		// поиск первой значащей цифры, которая не может быть нулём
		for (; i < len; ++i)
		{
			auto ch = sValue.GetAt(i);
			if (bFindPoint)
			{// точка уже была
				if (ch != L'0')
				{
					numCur = 1;
					break;
				}
			}
			else
			{// точки не было
				if (ch == L'.')
				{
					bFindPoint = true;
				}
				else
				{
					if (ch != L'0')
					{
						numCur = 1;
						break;
					}
				}
			}
		}
		// поиск остальных значащих цифр, которые могут быть нулём
		if (N > 1)
		{
			++i;
			for (; i < len; ++i)
			{
				if (bFindPoint)
				{
					if (++numCur == N)
						break;
				}
				else
				{
					if (sValue.GetAt(i) == L'.')
					{
						bFindPoint = true;
					}
					else
					{
						if (++numCur == N)
							break;
					}
				}
			}
		}
		// завершение строки
		++i;
		if (i < len)
		{
			sValue.SetAt(i, 0);
			sValue.ReleaseBuffer();
		}
		// проверка порядка, актуально только для порядка больше 0
		if (nOrder > 0)
		{
			int m = nOrder - CalcOrderOfValue(_wtof(sValue));
			if (m > 0)
			{
				for (int i = 0; i < m; ++i)
					sValue += L"0";
			}
		}
		if (bMinus)
		{
			valueDst = -valueDst;
			sValue = L"-" + sValue;
		}
	}// if (ret)
	else
	{
		valueDst = valueSrc;
		sValue.Format(L"%g", valueDst);
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>					// T - это float или double
// Округление итоговой погрешности или СКО в соответствии с ГОСТ Р 8.736-2011 (приложение Е).
// uncertSrc - округляемое значение, может быть больше, меньше или = 1;
// uncertDst - округлённое значение;
// bAccurateMeasure - точные изменения (да/нет);
//
// Возвращается флаг, что округление выполнено.
// При точных измерениях округленное значение всегда будет иметь 2 значащие цифры.
bool RoundUncertaintyOfResult(const T uncertSrc, T & uncertDst, const bool bAccurateMeasure) 
{
	bool ret = (_finite(uncertSrc) != 0) && ((T)uncertSrc > (T)0);
	if (ret)
	{
		int numSignDig(2);	// кол-во значащих цифр в uncertDst
		// проверка на finite уже выполнено
		RoundToN_SignificantDigits(uncertSrc, uncertDst, numSignDig);
		if (!bAccurateMeasure)
		{	// получено число с 2 значащими цифрами
			CString s;
			s.Format(L"%g", uncertDst);
			for (int i = 0, ie = s.GetLength(); i < ie; ++i)
			{
				WCHAR wc = s.GetAt(i);
				if ((wc == L'1') || (wc == L'2'))
				{// оставляю 2 значащих цифры
					break;
				}
				else if ((L'3' <= wc) && (wc <= L'9'))
				{// достаточно 1 значащей цифры
					numSignDig = 1;
					break;
				}
				// Если wc == L'9', то надо округлять, но не зависимо от то,
				// что будет после округления, надо брать 1 цифру,
				// это будет либо 9, либо 10
			}
		}
		if (numSignDig == 1)	// надо округлить заново
			RoundToN_SignificantDigits(uncertSrc, uncertDst, numSignDig);
	}
	else
		uncertDst = uncertSrc;
	return ret;
}
//------------------------------------------------------------------------------
template <class T>					// T - это float или double
// Округление итоговой погрешности или СКО в соответствии с ГОСТ Р 8.736-2011 (приложение Е)
// и преобразование округлённого значения в строку.
// uncertSrc - округляемое значение, может быть больше, меньше или = 1;
// uncertDst - округлённое значение;
// bAccurateMeasure - точные изменения (да/нет);
// sUncert - строка с округленным значением.
//
// Возвращается флаг, что округление выполнено.
// При точных измерениях округленное значение всегда будет иметь 2 значащие цифры.
bool RoundUncertaintyOfResult(const T uncertSrc, T & uncertDst, const bool bAccurateMeasure, CString & sUncert)
{
	bool ret = (_finite(uncertSrc) != 0) && (uncertSrc > (T)0);
	if (ret)
	{
		RoundUncertaintyOfResult(uncertSrc, uncertDst, bAccurateMeasure);
		// Округление выполнено, надо определить кол-во значащих цифр в uncertDst
		sUncert.Format(L"%g", uncertDst);
		int numSignDig(2);
		if (!bAccurateMeasure)
		{// По первой цифре определяю numSignDig
			for (int i = 0, ie = sUncert.GetLength(); i < ie; ++i)
			{
				WCHAR wc = sUncert.GetAt(i);
				if ((wc == L'1') || (wc == L'2'))
				{// оставляю 2 значащих цифры
					break;
				}
				else if ((L'3' <= wc) && (wc <= L'9'))
				{// достаточно 1 значащей цифры
					numSignDig = 1;
					break;
				}
			}
		}

		// Преобразование строки sUncert до numSignDig значащих цифр
		CString sLeft;
		CString sRight;
		int nE = sUncert.Find(L"e");
		if (nE == -1)
		{// символа "e" в строке нет
			sLeft = sUncert;
		}
		else
		{// символ "e" есть
			sLeft = sUncert.Left(nE);
			sRight = sUncert.Right(sUncert.GetLength() - nE);
		}
		// Разбор левой строки
		for (int i = 0, ie = sLeft.GetLength(); i < ie; ++i)
		{
			WCHAR wc = sLeft.GetAt(i);
			if ((L'1' <= wc) && (wc <= L'9'))
			{
				int i1 = i + 1;
				WCHAR * pwc = sLeft.GetBuffer() + i1;
				if (numSignDig == 1)
				{
					*pwc = 0;
					break;
				}
				else
				{// numSignDig == 2
					wc = *pwc;
					if (wc == 0)
					{	// bPointIsNo надо определить до Preallocate
						bool bPointIsNo = sLeft.Find(L".") == -1;
						sLeft.Preallocate(ie + 2);
						// + 2 - это точка и ноль 
						pwc = sLeft.GetBuffer() + i1;
						if (bPointIsNo)
							*pwc++ = L'.';
						*pwc++ = L'0';
						*pwc = 0;
					}
					if (wc == L'.')
					{
						++pwc;
						wc = *pwc;
					}
					break;
				}
			}
		}// for ...
		sLeft.ReleaseBuffer();

		sUncert = sLeft + sRight;
	}
	else
	{
		uncertDst = uncertSrc;
		sUncert.Format(L"%g", uncertDst);
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>					// T - это float или double
// Округление результата и его погрешности в соответствии с ГОСТ Р 8.736-2011 (приложение Е)
// и преобразование округлённых значений в строки.
// resultSrc - округляемое значение результата;
// uncertSrc - округляемое значение погрешности, может быть больше, меньше или = 1;
// resultDst - округлённое значение результата;
// uncertDst - округлённое значение погрешности;
// bAccurateMeasure - точные изменения (да/нет).
//
// Возвращается флаг, что округление выполнено.
// При точных измерениях округленное значение погрешности всегда будет иметь 2 значащие цифры.
bool RoundResultAndItsUncertainty(
	const T resultSrc, const T uncertSrc,
	T & resultDst, T & uncertDst,
	const bool bAccurateMeasure)
{
	bool ret = (_finite(resultSrc) != 0) && (_finite(uncertSrc) != 0) && (uncertSrc > (T)0);
	if (ret)
	{
		CString str;
		RoundUncertaintyOfResult(uncertSrc, uncertDst, bAccurateMeasure, str);

		T k((T)1.);
		int nE = str.Find(L"e");
		bool bIsE = nE != -1;
		if (bIsE)
		{// символ "e" есть
			k = (T)_wtof(L"1" + str.Right(str.GetLength() - nE));
			str = str.Left(nE);
		}

		// Теперь надо распарсить строку
		int len = str.GetLength();
		int m = str.Find(L".");
		if (m != -1)
		{// есть десятичная точка
			CString format;
			format.Format(L"%%.%df", len - m - 1);
			str.Format(format, resultSrc);
			resultDst = (T)_wtof(str);
		}
		else
		{// нет десятичной точки
			int numInErr(2);	// кол-во значащих цифр в погрешности
			if (!bAccurateMeasure)
			{
				if (str.GetAt(0) > L'2')
					numInErr = 1;
			}
			str.SetAt(0, L'1');
			double dx = _wtof(str);
			if (numInErr == 2)
				dx /= 10;
			resultDst = (T)(RoundToInt(resultSrc / dx) * dx);
		}
		if (bIsE)
			resultDst *= k;
	}
	else
	{		
		resultDst = resultSrc;
		uncertDst = uncertSrc;
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>					// T - это float или double
// Округление результата и его погрешности в соответствии с ГОСТ Р 8.736-2011 (приложение Е)
// и преобразование округлённых значений в строки.
// resultSrc - округляемое значение результата;
// uncertSrc - округляемое значение погрешности, может быть больше, меньше или = 1;
// resultDst - округлённое значение результата;
// uncertDst - округлённое значение погрешности;
// bAccurateMeasure - точные изменения (да/нет);
// sResult - строка с округлённым значением результата;
// sUncert - строка с округлённым значением погрешности.
//
// Возвращается флаг, что округление выполнено.
// При точных измерениях округленное значение погрешности всегда будет иметь 2 значащие цифры.
bool RoundResultAndItsUncertainty(
	const T resultSrc, const T uncertSrc,
	T & resultDst, T & uncertDst,
	const bool bAccurateMeasure,
	CString & sResult, CString & sUncert)
{
	bool ret = (_finite(resultSrc) != 0) && (_finite(uncertSrc) != 0) && (uncertSrc > (T)0);
	if (ret)
	{
		RoundUncertaintyOfResult(uncertSrc, uncertDst, bAccurateMeasure, sUncert);
		T k((T)1.);
		resultDst = resultSrc;
		int nE = sUncert.Find(L"e");
		bool bIsE = nE != -1;
		CString sLeft, sRight;
		if (bIsE)
		{// символ "e" есть
			sLeft = sUncert.Left(nE);
			sRight = sUncert.Right(sUncert.GetLength() - nE);
			k = (T)_wtof(L"1" + sRight);
			resultDst /= k;
		}
		else
			sLeft = sUncert;

		int m = sLeft.Find(L".");
		if (m != -1)
		{// есть десятичная точка
			CString format;
			format.Format(L"%%.%df", sLeft.GetLength() - m - 1);
			sResult.Format(format, resultDst);
			resultDst = (T)_wtof(sResult);
		}
		else
		{// нет десятичной точки
			sResult = sLeft;
			int numInErr(2);	// кол-во значащих цифр в погрешности
			if (!bAccurateMeasure)
			{
				if (sResult.GetAt(0) > L'2')
					numInErr = 1;
			}
			// округление результата
			sResult.SetAt(0, L'1');
			double dx = _wtof(sResult);
			if (numInErr == 2)
				dx /= 10;			
			resultDst = (T)(RoundToInt(resultDst / dx) * dx);
			sResult.Format(L"%.0f", resultDst);
		}
		resultDst = resultDst;

		if (bIsE)
		{
			resultDst *= k;
			sResult += sRight;
		}
	}
	else
	{
		resultDst = resultSrc;
		uncertDst = uncertSrc;
		sResult.Format(L"%g", resultDst);
		sUncert.Format(L"%g", uncertDst);
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>					// T - это float или double
// Определение формата для заданной точночти округления
//
bool GetFormatForRound(
	const T uncert,			// точность округления
	CString & sFormat)		// искомый формат
{
	bool ret = (_finite(uncert) != 0) && (uncert > (T)0);
	if (ret)
	{
		int nOrder = CalcOrderOfValue(fabs(uncert));
		if (nOrder < 0)
			sFormat.Format(L"%%.%df", -nOrder + 1);
		else if (nOrder == 0)
			sFormat = L"%.1f";
		else
			sFormat = L"%.0f";
	}
	else
	{
		sFormat = L"%f";
	}
	return ret;
}
//------------------------------------------------------------------------------
// Проверка и коррекция частот среза полосового фильтра
enum TypeTestFiltrBandPass : BYTE
{
	test_freq_0,		// проверка только нижней частоты
	test_freq_1,		// проверка только верхней частоты
	test_freq_both,		// проверка обеих частот
};

template <class T>	// T - это float или double
// Проверка и коррекция значений частот среза полосового фильтра.
// freq_0 - нижняя частота среза;
// freq_1 - верхняя частота среза;
// freqMin и freqMax - мин. и мах. значения частот среза, которые определяются частотой дискретизации;
// typeTest - тестируемые значения.
// true возвращается если значения частот входят в ОДЗ.
//
bool TestFiltrBandPass(
	T & freq_0, T & freq_1,
	const T freqMin, const T freqMax,
	const TypeTestFiltrBandPass typeTest)
{
	bool ret(true);
	switch (typeTest)
	{
	case test_freq_0:
		if (freq_0 < freqMin)
		{
			freq_0 = freqMin;
			ret = false;
		}
		else if (freq_0 > freq_1)
		{
			freq_0 = freq_1;
			ret = false;
		}
		break;

	case test_freq_1:
		if (freq_1 < freq_0)
		{
			freq_1 = freq_0;
			ret = false;
		}
		else if (freq_1 > freqMax)
		{
			freq_1 = freqMax;
			ret = false;
		}
		break;

	case test_freq_both:
		if (freq_0 < freqMin)
		{
			freq_0 = freqMin;
			ret = false;
		}
		else if (freq_0 > freqMax)
		{
			freq_0 = freqMax;
			ret = false;
		}

		if (freq_1 < freqMin)
		{
			freq_1 = freqMin;
			ret = false;
		}
		else if (freq_1 > freqMax)
		{
			freq_1 = freqMax;
			ret = false;
		}

		if (freq_0 > freq_1)
		{
			freq_0 = freq_1 = sqrt(freq_0 * freq_1);
			ret = false;
		}
		break;

	default:
		break;
	}
	return ret;
}
//------------------------------------------------------------------------------
// Проверка значения на попадание в ОДЗ (интервал, включающий границы).
// Возвращаемое значение: true/false - newVal входит в ОДЗ.
// bChange - изменять или нет новое значение с целью вхождения в ОДЗ.
template <class T>	// T - это float или double
bool TestAndChangeValue(
	T & newVal,
	const T minVal,
	const T maxVal,
	const bool bChange)
{
	bool bmin = minVal <= newVal;
	bool ret = bmin && (newVal <= maxVal);

	if (bChange && !ret)
	{
		if (!bmin)
			newVal = minVal;
		else
			newVal = maxVal;
	}
	return  ret;
}
//------------------------------------------------------------------------------
// Проверка значения enum на попадание в ОДЗ. У enum должен быть размер.
// Возвращаемое значение: true/false - newVal входит в ОДЗ.
// bChange - изменять или нет новое значение с целью вхождения в ОДЗ.
template <class T>	// T - это float или double
bool TestAndChangeValueOfEnum(
	T & typeEnum,
	const T sizeEnum,
	const bool bChange)
{
	bool ret = typeEnum < sizeEnum;
	if (bChange && !ret)
		typeEnum = (T)0;
	return  ret;
}
//------------------------------------------------------------------------------
// Определение эффективной частоты дискретизации с учетом декады
// Tint - целое число, тип декады (от 0 до 4)
// T - это float или double, тип частоты дискретизации
template <class Tint, class T>
bool GetFreqADCwithDecada(const Tint decada, const T freqADC, T & freqResult)
{
	bool ret = (freqADC > (T)0) && (0 <= decada) && (decada < 5);
	if (ret)
	{
		switch (decada)
		{
		case 1:		freqResult = freqADC / 10;		break;
		case 2:		freqResult = freqADC / 100;		break;
		case 3:		freqResult = freqADC / 1000;	break;
		case 4:		freqResult = freqADC / 10000;	break;
		default:	freqResult = freqADC;			break;
		}
	}
	else
		freqResult = (T)0;
	return ret;
}
//-----------------------------------------------------------------------------
// Умножение компрлексного массива на действительное число
// T - это float или double
// Tc - массив комплексных чисел типа T
template <class T, class Tc>
void MulC_ComplexMass(const T k, Tc * pSrc, Tc * pDst, const int len)
{
	if (k != (T)1)
	{
		std::complex<T> * pDataSrc = (std::complex<T> *)pSrc;
		std::complex<T> * pDataDst = (std::complex<T> *)pDst;
		for (int i = 0; i < len; ++i)
			pDataDst[i] = k * pDataSrc[i];
	}
}
//-----------------------------------------------------------------------------
// Умножение компрлексного массива на действительное число
// T - это float или double
// Tc - массив комплексных чисел типа T
template <class T, class Tc>
void MulC_ComplexMass_I(const T k, Tc * pSrcDst, const int len)
{
	if (k != (T)1)
	{
		std::complex<T> * pData = (std::complex<T> *)pSrcDst;
		for (int i = 0; i < len; ++i)
			pData[i] *= k;
	}
}
//-----------------------------------------------------------------------------
// Преобразование значения в сек в форматную строку
// L"%02d:%02d:%02d" - "час:мин:сек"
template <class T>	// T - это float или double
void ConvertBackTimerToString(const T valSec, CString & str)
{
	if (valSec < (T)1)
		str.Empty();
	else
	{
		if (valSec < (std::numeric_limits<T>::max)())
		{
			int nSec((int)valSec);
			int nMin(0);
			int nHour(0);

			if (nSec >= 60)
			{
				div_t dt = div(nSec, 60);
				nSec = dt.rem;
				nMin = dt.quot;

				if (nMin >= 60)
				{
					dt = div(nMin, 60);
					nMin = dt.rem;
					nHour = dt.quot;
				}
			}
			str.Format(L"%02d:%02d:%02d", nHour, nMin, nSec);
		}
		else
			str = L"XX:XX:XX";
	}
}
//-----------------------------------------------------------------------------
template <class T>	// T - это float или double
// Выполнение линейного усреднения массива
// counter - кол-во уже выполненных усреднений, от 0
bool Calc_Aver_line(T * pCur, T * pAver, int len, int & counter)
{
	bool ret = (pCur != nullptr) && (pAver != nullptr) &&
		(len > 0) && (counter >= 0);
	if (ret)
	{
		int n_1 = counter++;
		if (counter > 1)
		{
			for (int i = 0; i < len; ++i)
				pAver[i] = (n_1 * pAver[i] + pCur[i]) / counter;
		}
		else
		{
			rsize_t size = sizeof(T) * len;
			ret = 0 == memcpy_s(pAver, size, pCur, size);
		}
	}
	return ret;
}
//-----------------------------------------------------------------------------
template <class T>	// T - это float или double
// Выполнение EMA-усреднения массива
// counter - кол-во уже выполненных усреднений, от 0
bool Calc_Aver_EMA(T * pCur, T * pAver, int len, int & counter, T factorEMA)
{
	bool ret = (pCur != nullptr) && (pAver != nullptr) &&
		(len > 0) && (0. < factorEMA) && (factorEMA < 1.);
	if (ret)
	{
		if (counter++ > 0)
		{
			T _1_a = (T)1 - factorEMA;
			for (int i = 0; i < len; ++i)
				pAver[i] = _1_a * pAver[i] + factorEMA * pCur[i];
		}
		else
		{
			rsize_t size = sizeof(T) * len;
			ret = 0 == memcpy_s(pAver, size, pCur, size);
		}
	}
	return ret;
}
//-----------------------------------------------------------------------------
// функция для трассировки строки с заданным форматированием
inline void OutputDebugStringEx(_In_ LPCWSTR Format, ...)
{
	if (Format == nullptr)
		return;

	va_list _Arglist;
	va_start(_Arglist, Format);
	CString String;
	String.FormatV(Format, _Arglist);
	va_end(_Arglist);

	OutputDebugString(String);
}
//-----------------------------------------------------------------------------
// Поиск в отсортированном массиве индекса элемента с заданным значением
// Val - значение для поиска
// Array - исходный массив
// ArraySize - размер массива
// Index - найденный индекс
// return - true если успешно найдено число, false в случае возникновения ошибки
template<class T>
bool FindIndexOfVal(_In_ T Val, _In_ T* Array, _In_ int ArraySize, _Out_ int &Index)
{
	int TempIndex(0);
	Index = 0;
	int StartIndex(0), EndIndex(ArraySize - 1);
	int Count(0), PrevIndex(-1);
	bool bRes(true);
	// ищем в массиве какое значение соответствует заданной точке
	do
	{
		// формула: fInd = (N - 1) * (Val - Ar[0]) / (Ar[N - 1] - Ar[0])
		Index = (int)T(float(EndIndex - StartIndex) * (float(Val - Array[StartIndex]) / float(Array[EndIndex] - Array[StartIndex]))) + StartIndex;

		// проверки на выход из диапазона и зацикливание
		if ((Index < 0) || (Index >= ArraySize) || (Index == PrevIndex))
		{
			// выставляем крайние значения
			if (Index < 0)
				Index = 0;
			else if (Index >= ArraySize)
				Index = ArraySize - 1;
			bRes = false;
			break;
		}
		else if ((Index == ArraySize - 1) && (Array[Index] != Val))
		{
			bRes = false;
			break;
		}

		PrevIndex = Index;

		if (Val < Array[Index])
		{
			for (TempIndex = Index - 1; TempIndex >= StartIndex; TempIndex--)
			{
				if (Array[Index] == Array[TempIndex])
					Index--;
				else
				{
					if ((Val < Array[Index]) || (Val > Array[Index + 1]))
						Index--;
					else
						break;
					break;
				}
			}
			EndIndex = Index;
		}
		else if (Val > Array[Index])
		{
			for (TempIndex = Index + 1; TempIndex < EndIndex; TempIndex++)
			{
				if (Array[Index] == Array[TempIndex])
					Index++;
				else
				{
					if ((Val < Array[Index]) || (Val > Array[Index + 1]))
						Index++;
					else
						break;
					break;
				}
			}
			StartIndex = Index;
		}
		else
			break;	// нашли ровно наш элемент
		Count++;
	} while ((Val < Array[Index]) || (Val > Array[Index + 1]));

	return bRes;
}
//-----------------------------------------------------------------------------
// Поиск в отсортированном массиве индекса элемента с заданным значением
// Val - значение для поиска
// Array - исходный массив
// Index - найденный индекс
// return - true если успешно найдено число, false в случае возникновения ошибки
template<class T>
bool FindIndexOfVal(_In_ T Val, _In_ std::vector<T>& Array, _Out_ int &Index)
{
	return FindIndexOfVal(Val, Array.data(), Array.size(), Index);
}
//-----------------------------------------------------------------------------
template <class T>	// T - это float или double
UINT GetFilterOrder(_In_ const T fs, _In_ const T fadc)
{
	UINT order(0);
	if ((0. < (double)fs) && ((double)fs < (0.5 * fadc)))
	{
		double tmp = (double)(fs / fadc);
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
//-----------------------------------------------------------------------------
template <class T>	// T - это float или double
// Оценка положения и значения максимума массива.
// Максимум моделируется параболой, которая определяется по 3-м точкам.
// Средняя точка - это точка с максимальным значением из массива.
// Две другие точки - это ближайшие точки к средней, у которых значения
// меньше максимального.
//
// Возвращаемое значение.
// 0 - оценка выполнена успешно;
// Err_NULL_Pointer (-10) - нулевой указатель на исходный ряд;
// Err_Size_Structure (-4) - размер массива меньше 3;
// Err_Cmp_Value (-34) - все элементы массива равны друг другу;
// Err_UnSuccess_Find (-33) - максимум достигается на краях массива;
// Err_Calc (-9) - ошибка при определении параметров парабболы.
//
long FindExactMax(
	const T * pData,	// исходный ряд
	const int len,		// его размер
	T & yMax,			// оценка максимального значения
	T & xMax)			// оценка индекса максимального значения
{
	long ret = (pData != nullptr) ? 0 : Err_NULL_Pointer;
	if (ret == 0)
	{
		if (len < 3)
		{
			xMax = yMax = (T)0;
			ret = Err_Size_Structure;
		}
		if (ret == 0)
		{
			bool bFlag(true);
			int n1(0), n2(0), n3(0), nMax(0);
			T y1(pData[0]), y2(y1), y3(y1);
			yMax = y1;
			for (int i = 0; i < len; ++i)
			{
				T y2 = pData[i];
				bFlag &= y2 != y1;	// IsEqual НЕЛЬЗЯ!!!
				if (y2 > yMax)
				{
					yMax = y2;
					nMax = i;
				}
			}
			if (!bFlag)
			{
				xMax = (T)nMax;
				ret = Err_Cmp_Value;	// все значения массива одинаковые
			}
			if (ret)
			{
				if ((nMax == 0) || (nMax == (len - 1)))
				{
					xMax = (T)nMax;
					ret = Err_UnSuccess_Find;	// максимум достигается на краях интервала
				}
				if (ret)
				{// штатный режим. Определяю 3 точки
					n1 = nMax - 1;
					n2 = nMax;
					n3 = nMax + 1;
					y1 = pData[n1];
					y2 = yMax;
					y3 = pData[n3];
					// проверка на наличие плато
					while (true)
					{
						if (y3 == y2)	// IsEqual НЕЛЬЗЯ!!!
						{
							if (n3 < len)
							{
								++n3;
								y3 = pData[n3];
							}
							else
								break;
						}
						else
							break;
					}
					// есть 3 точки, строю параболу
					T a((T)0), b((T)0), c((T)0);
					TRACE(L"Build parabol for fmax: n1 = %d, n2 = %d, n3 = %d\n", n1, n2, n3);
					TRACE(L"Build parabol for fmax: y1 = %f, y2 = %f, y3 = %f\n", y1, y2, y3);
					if (SolutionLineEquation_3abc((T)n1, (T)n2, (T)n3, y1, y2, y3, &a, &b, &c) &&
						(a < (T)0) && (b >= (T)0))
					{
						xMax = -b / (2 * a);
						yMax = (a * xMax + b) * xMax + c;
					}
					else
					{
						xMax = (T)nMax;
						ret = Err_Calc;
					}
				}
			}
		}
	}
	else
	{
		xMax = yMax = (T)0;
	}
	return ret;
}
//-----------------------------------------------------------------------------
// Функции расчёта параметров резонанса
//-----------------------------------------------------------------------------
template <class T>	// T - это float или double
inline T Decrement(const T Q_factor)
{
	return IPP_PI / Q_factor;
}
//-----------------------------------------------------------------------------
template <class T>	// T - это float или double
// decrement - декремент затухания
inline T Q_factor(const T decrement)
{
	return (T)IPP_PI / decrement;
}
//-----------------------------------------------------------------------------
template <class T>	// T - это float или double
// freq_res - резонансная частота в Гц
// freq_0   - собственная частота в Гц
inline T Q_factor(const T freq_res, const T freq_0)
{
	T ret((T)0);
	if (((T)0 < freq_res) && (freq_res < freq_0))
	{
		ret = freq_res / freq_0;
		ret *= ret;
		ret = 1 / sqrt(2 * (1 - ret));
	}
	return ret;
}
//-----------------------------------------------------------------------------
template <class T>	// T - это float или double
// freq_res - резонансная частота в Гц
// Q_factor - добротность
inline T FreqRangeResonance(const T freq_res, const T Q_factor)
{
	return freq_res / Q_factor;
}
//-----------------------------------------------------------------------------
