//--------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <vector>
#include "ATLComTime.h"
#include <mathZet.h>
#include <Quality.h>
//--------------------------------------------------------------------------
const COLORREF ColorFont  = RGB(1, 203, 247);
const COLORREF ColorOk    = RGB(23, 23, 23);
const COLORREF ColorBad   = RGB(120, 120, 120);
const COLORREF ColorError = RGB(200, 200, 200);
const COLORREF ColorCalibr = RGB(250, 80, 80);
//------------------------------------------------------------------------------
// Константы, используемые во многих программах
//----------------------------------------------------------------------
// Текстовые константы
//---------------------
const CString Str_n = L"\n";
const CString Str_t = L"\t";
const CString StrStars = L"*****";
const CString FormatForDTU = L"%.6f";
const CString g_sCreate(L"-createprocess");
const CString g_sCreateFormat(g_sCreate + L"_%u_%d");
//----------------------------------------------------------------------
// Численные константы
//---------------------
const float fOne = 1.f;
const float fZero = 0.f;
const float g = 9.807f;
const double constD_180_pi = 180. / M_PI;
const double constD_pi_180 = M_PI / 180.;
const float const_180_pi = float(constD_180_pi);
const float const_pi_180 = float(constD_pi_180);
const float REarthInkm = 6371.3f; // средний радиус Земли

// Секунда как часть суток
const double SecAsPartOfDay = 1. / (24. * 60. * 60.);
//----------------------------------------------------------------------
// Цвета, используемые в графиках
//--------------------------------
const unsigned long ColorCrs = RGB(32,  32,  192);	// курсор
const unsigned long ColorDig = RGB(64,  64,    0);	// цифры на осях
const unsigned long ColorFon = RGB(240, 240, 240);	// фон
const unsigned long ColorGrd = RGB(192, 192,   0);	// сетка
const unsigned long ColorLeg = RGB(128,   0,   0);	// легенда
const unsigned long ColorGrf = RGB(172,  22,  39);	// график
//------------------------------------------------------------------------------
// Шаблонная функция, вычисляющая факториал
template <class T>		// T - это любые целые числа.
T Factorial(T n)		// НО могут быть переполнения
{
	T ret = 1;			// считаю, что 1!, равный 1, тоже допустим
	if ( n > 1 )
	{
		for(T i=2; i<=n; i++)
			ret *= i;
	}
	return ret;
}
//------------------------------------------------------------------------------
// Шаблонная функция, вычисляющая функцию "С из n по k", равную n!/[k!*(n-k)!]
template <class T>		// T - это любые целые числа.
T CfromNbyK(T n, T k)	// НО могут быть переполнения
{
	T n1, n2, ret;
	if ( (k > 0) && (n > k) )
	{	// вначале вычисляю n!/n1!, где n1 = max(k, (n-k))
		n2 = n - k;
		if ( k > n2 )
			n1 = k;
		else
		{
			n1 = n2;
			n2 = k;
		}
		ret = ++n1;
		for(T i=++n1; i<=n; i++)
			ret *= i;
		// осталось результат поделить на n2!, где n2 = min(k, (n-k))
		ret /= Factorial(n2);
	}
	else
		ret = 1;
	return ret;
}
//------------------------------------------------------------------------------
// Шаблонная функция, вычисляющая азимут - угол, отсчитываемый от направления на
// ось Y по часовой стрелке. Угол рассчитывается в градусах, он должен быть
// в интервале [0, 360)
template <class T>			// T - это float, double, long double
T Azimuth(					// азимут в град.
	T x,					// компонента X
	T y,					// компонента Y
	bool bInvers = false)	// требуется противоположное направление
{	// вначале считаю обычный (в смысле математики) угол в град.
	T ret = atan2(y, x) * T(constD_180_pi);		// интервал (-180, +180]
	if (bInvers)
	{	// противоположное направление, интервал (-180, +180]
		if ( ret > T(0) )
			ret -= T(180);
		else
			ret += T(180);
	}
	// перевод угла из интервала (-180, +180] в азимут
	ret = T(90) - ret;
	// если надо, то сдвиг в интервал [0, 360)
	if ( ret < T(0) )
		ret += T(360);
	return ret;
}
//------------------------------------------------------------------------------
// Шаблонная функция, вычисляющая угол места, рассчитываемый в градусах
template <class T>			// T - это float, double, long double
T AngleOfElevation(T x, T y, T z)	// угол места в град.
{
	return atan( z / sqrt(x*x + y*y) ) * T(constD_180_pi);
}
//------------------------------------------------------------------------------
// Шаблонная функция, округляющая число до заданного кол-ва значащих цифр
template <class T>				// T - это int, float, double
T Rounding(	const T val,		// округляемое число
			const int num = 0)	// кол-во оставляемых значащих цифр,
// возвращаемое число - округлённое число
{// если num = 0, то округляем погрешность (одна или две значащих цифры)
	int numDig;	// кол-во значащих цифр в результате
	T x, ret, dx;
	bool bMinus = val < T(0);
	if ( ( num >= 0 ) && ( val != T(0) ) )	// отрицательным кол-во цифр
	{// не может быть, как и округляемое число не может равняться 0
		if (bMinus)	// проверка на отрицательность
			x = -val;
		else
			x = val;
		// определяю сколько требуется значащих цифр
		if (num != 0)
			numDig = num;	// кол-во задано
		else
		{// округление погрешности
			ret = Rounding(x, 2);
			dx = T(pow(10., int(floor(log10((double)ret))) - 1));
			if ( int(ret/dx) < 20 )
				numDig = 2;
			else
				numDig = 1;
		}
		// само округление ( floor(-2.8) = -3 и  floor(2.8) = 2 )
		dx = T(pow(10., int(floor(log10((double)x))) - numDig + 1));
		ret = T( int(x / dx + 0.5) ) * dx;
		// проверка на отрицательность
		if (bMinus)
			ret = -ret;
	}
	else
		ret = val;	// аварийный случай
	return ret;
}
//------------------------------------------------------------------------------
// Шаблонная функция, сортирующая массив некоторых элементов.
// Возможна сортировка по возрастанию или по убыванию
template <class T>			// T - это любые элементы, у которых определены
void SortYM(				//		операторы ">" и "="
	T *pT,					// указатель на массив
	int size,				// размер массива
	bool flagUp = true)		// сортировка по возрастанию
{
	T x0 = *pT, x1;
	bool flag;
	for(int i=0; i<size-1; i++)
	{
		x1 = pT[i+1];
		flag = x1 > x0;
		if (flagUp)
			flag = !flag;
		if (flag)
		{
			pT[i] = x1;
			pT[i+1] = x0;
		}
		else
			x0 = x1;
	}
	if (size > 2)
		SortYM(pT, size-1, flagUp);
}
//------------------------------------------------------------------------------
// Шаблонная функция, определяющая количество экстремумов некоторой
// таблично заданной функции. Границы не считаю экстремумами.
template <class T>			// T - это любые числа
int CounterExtremum(// возвр. значение - кол-во экстремумов
	T *padd,	// исходный ряд
	int size,	// его размер
	bool bMax,	// нужны: 1 - максимумы, 0 - минимумы
	int zzX,	// зиг-заг по Х
	T zzY,		// зиг-заг по Y
	PVOID pval,	// вектор значений экстремумов, если pval != NULL
	std::vector<int> * pInd)// вектор индексов экстремумов, или NULL
{
	if (size < 3)
		return 0;
	int indRetStart, ret = 0, i = 2;
	bool flag,			// y2 > y1
		bGoUp,			// генеральное движение вверх
		bGoRet = false;	// обратное движение
	T dy, valRetStart;
	std::vector<T> *pVal = (std::vector<T>*)pval;
	if (pVal)
		pVal->clear();
	if (pInd)
		pInd->clear();

	T y1 = padd[0];
	T y2 = padd[1];
	while ( y2 == y1 )
		y2 = padd[i++];
	flag = bGoUp = y2 > y1;

	for(; i<size; i++)
	{
		y1 = y2;
		y2 = padd[i];
		flag = y2 > y1;

		if ( ( ( flag && !bGoUp && !bGoRet) ||
			   (!flag &&  bGoUp && !bGoRet) ) && (y2 != y1) )
		{// начало обратного хода
			if ( (zzX == 0) && ( (y1 > y2) == bMax ) )
			{
				dy = y2 - y1;
				if ( dy < 0 )
					dy = -dy;
				if ( dy > zzY )
				{// был экстремум, который мы ищем
					if (pVal)
						pVal->push_back(y1);
					if (pInd)
						pInd->push_back(i-1);
					ret++;
					bGoRet = false;
					bGoUp = !bGoUp;
				}
			}
			else
			{
				indRetStart = i - 1;
				valRetStart = y1;
				bGoRet = true;
			}
		}
		else if ( ( ( flag &&  bGoUp && bGoRet) ||
				    (!flag && !bGoUp && bGoRet) ) && (y2 != y1) )
		{// конец обратного хода
			if ( zzX > 0 )
			{// был экстремум, который мы ищем
				if ( ( bGoUp == bMax ) && (i - indRetStart - 1) > zzX )
				{// и значимый по Х, здесь (i - indRetStart - 1), т.к.
					dy = y2 - valRetStart;	// конец обратного хода
					if ( dy < 0 )
						dy = -dy;
					if ( dy > zzY )
					{// и значимый по Y
						if (pVal)
							pVal->push_back(valRetStart);
						if (pInd)
							pInd->push_back(indRetStart);
						ret++;	// направление остаётся прежним!!!
					}
				}
			}
			else
			{// есть три точки
				if ( (y1 > y2) == bMax )
				{// был экстремум, который мы ищем
					dy = y2 - y1;
					if ( dy < 0 )
						dy = -dy;
					if ( dy > zzY )
					{// и этот экстремум значимый
						if (pVal)
							pVal->push_back(y1);
						if (pInd)
							pInd->push_back(i-1);
						ret++;	// направление остаётся прежним!!!
					}
				}
			}
			bGoRet = false;
		}
		else if ( ( ( ( flag && !bGoUp && bGoRet) ||
				  (!flag &&  bGoUp && bGoRet) ) ) || ( (y2 == y1) && bGoRet ) )
		{// монотонный обратный ход
			if ( (i - indRetStart) > zzX )
			{// вышли за порог по Х
				dy = y2 - valRetStart;
				if ( dy < 0 )
					dy = -dy;
				if ( dy > zzY )
				{// вышли за порог по Y - экстремум
					if (bGoUp == bMax)
					{// наш экстремум
						if (pVal)
							pVal->push_back(valRetStart);
						if (pInd)
							pInd->push_back(indRetStart);
						ret++;
					}
					bGoUp = !bGoUp;
					bGoRet = false;
				}
			}
		//	else не вышли за пределы по Х, ничего не делаю
		}
	//	else монотонный прямой ход, ничего не делаем
	}//	for(; i<size; i++)
	return ret;
}
//------------------------------------------------------------------------------
// Шаблонная функция, рассчитывающая гистограмму по исходному ряду.
// При sizeHistogram = 0 кол-во интервалов гистограммы определяется по
// размеру исходных данных
template <class T>			// T - это float или double, а также мх производные 
int* Histogram(
	T *padd,				// исходные данные
	int size,				// их размер
	int *pValHistogram,		// ук. на данные гистограммы или NULL
	int *pSizeHistogram,	// кол-во интервалов гистограммы
	T *pValItem0Histogram,	// величина середины 0-ого интервала гистограммы
	T *pWidthItemHistogram,	// ширина интервалов гистограммы
	int NumMaxHistogram = 1,// кол-во ожидаемых максимумов
	int sizeHistogram = 0)	// желаемое кол-во интервалов гистограммы
{
	int i, num;
	T a0, da, amin, amax;
	// определяю кол-во интервалов -----------------------------------
	if (sizeHistogram == 0)
	{
		num = int(pow((float)size, 0.4f));
		if (NumMaxHistogram < 2)
			num = int(1.25f * num);
		else
			num *= 2;
		num |= 1;
	}
	else
		num = sizeHistogram;
	if (num < 3)	// меньше нельзя, т.к. далее в знаменате
		num = 3;	// будет num-2

	// определяю ширину интервалов -----------------------------------
	amin = min(padd[0], padd[1]);
	amax = max(padd[0], padd[1]);
	for (i = 2; i < size; i++)
	{
		a0 = padd[i];
		if (amin > a0)
			amin = a0;
		if (amax < a0)
			amax = a0;
	}

	if (amax == amin)
	{// исходные данные - это одно и тоже число
		*pSizeHistogram = 0;
		*pValItem0Histogram = amin;
		*pWidthItemHistogram = T(0);
		if (pValHistogram)
		{
			delete[] pValHistogram;
			pValHistogram = nullptr;
		}
		return NULL;
	}

	da = (amax - amin) / (num - 2);
	a0 = amin - T(0.5) * da;

	// создаю массив для гистограммы ------------------------------------
	if (pValHistogram)
	{
		delete[] pValHistogram;
		pValHistogram = nullptr;
	}

		pValHistogram = new int[num];
		ZeroMemory(pValHistogram, num * sizeof(int));	// обязательно !!!


	// параметры и данные гистограммы ------------------------------------
	*pSizeHistogram = num;
	*pValItem0Histogram = amin;
	*pWidthItemHistogram = da;
	// т.к. Т не int и amax != amin, то da != 0
	for(i=0; i<size; i++)
		pValHistogram[ int( (padd[i] - a0) / da ) ]++;	
	return pValHistogram;

}
//------------------------------------------------------------------------------