//------------------------------------------------------------------------------
/* Файл TemplateWindow.h
 Функции расчёта нестандартных окон.
 Т.к. функции шаблонные, то пришлось их определять в h-файле, иначе
 компилятор выдаёт ошибку.*/
//------------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------------
// Общая функция для некоторых окон
template <class T>
void WinWithNconst_I(T * pSrcDst, int len, const double * pConst, int N)
{
	double val(IPP_2PI / (len - 1));

	for (int i = 0; i < len; ++i)
	{
		double ak = 0.;
		for (int k = 0; k < N; ++k)
			ak += pConst[k] * cos((k * i) * val);
		pSrcDst[i] *= (T)ak;
	}
}
//------------------------------------------------------------------------------
// окно Хэннинга
template <class T>
void WinHanning_I(T * pSrcDst, int len)
{
	double val(IPP_2PI / (len - 1));

	for (int i = 0; i < len; ++i)
		pSrcDst[i] *= (T)(0.5 * (1. - cos(val * i)));
}
//------------------------------------------------------------------------------
// окно Рифа-Винсента (4)
template <class T>
void WinRifeVincent_4_I(T * pSrcDst, int len)
{
	double val(IPP_2PI / (len - 1));
	const double valRifeVincent4[5] = { 35.0, -56.0, 28.0, -8.0, 1.0 };

	for (int i = 0; i < len; i++)
	{
		double ak = 0.;
		for (int k = 0; k < 5; k++)
			ak += valRifeVincent4[k] * cos((k * i) * val) / 128.0;
		pSrcDst[i] *= (T)ak;
	}
}
//------------------------------------------------------------------------------
// окно Блэкмана-Харриса (N)
template <class T>
void WinBlackmanHarris_N_I(T * pSrcDst, int len, int N)
{
	static const double valBlackmanHarris_3[3] = { 0.42323, -0.49364, 0.05677 };
	static const double valBlackmanHarris_4[4] = { 0.35875, -0.48829, 0.14128, -0.01168 };

	WinWithNconst_I(pSrcDst, len,
		((N == 3) ? valBlackmanHarris_3 : valBlackmanHarris_4), N);
}
//------------------------------------------------------------------------------
// окно Наталла
template <class T>
void WinNuttall_I(T * pSrcDst, int len)
{
	static const double valNuttal[4] = { 0.355768, -0.487396, 0.144232, -0.012604 };
	WinWithNconst_I(pSrcDst, len, valNuttal, 4);
}
//------------------------------------------------------------------------------
// окно Блэкмана-Наталла
template <class T>
void WinBlackmanNuttall_I(T * pSrcDst, int len)
{
	static const double valBlackmanNuttal[4] = { 0.3635819, -0.4891755, 0.1365995, -0.0106411 };
	WinWithNconst_I(pSrcDst, len, valBlackmanNuttal, 4);
}
//------------------------------------------------------------------------------
// окно с плоской вершиной
template <class T>
void WinFlattop_I(T * pSrcDst, int len)
{
	static const double valFlattop[5] = { 1.0, -1.93, 1.29, -0.388, 0.028 };
	WinWithNconst_I(pSrcDst, len, valFlattop, 5);
}
//------------------------------------------------------------------------------