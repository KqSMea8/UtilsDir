//------------------------------------------------------------------------------
#pragma once
#include <Algebra\SystemEquations_2.h>
//------------------------------------------------------------------------------
// Линнейная аппроксимация для эквидистантного ряда - поиск кривой y = k * x + b,
// т.е. определение значений коэффициентов k и b.
// Используется метод наименьших квадратов
template <class Tf, class Ti>	// Tf - это float, double. Ti - это целое
bool ApproximationLine_equ(const Tf * pSrc, const Ti len, Tf & k, Tf & b)
{
	bool ret = (pSrc != nullptr) && ((int)len > 1);
	if (ret)
	{
		double x(0.), xx(0.), y(0.), xy(0.), kd(0.), bd(0.);
		for (int i = 0; i < (int)len; ++i)
		{
			double xd = (double)i;
			double yd = (double)pSrc[i];
			x += xd;
			y += yd;
			xx += xd * xd;
			xy += xd * yd;
		}
		ret = SolutionLineEquation_2(xx, x, x, (double)(int)len, xy, y, &kd, &bd);
		if (ret)
		{
			k = (Tf)kd;
			b = (Tf)bd;
		}
	}
	
	if (!ret)
	{
		k = b = (Tf)0;
	}
	return ret;
}
//------------------------------------------------------------------------------
// Линнейная аппроксимация для не эквидистантного ряда - поиск кривой y = k * x + b,
// т.е. определение значений коэффициентов k и b.
// Используется метод наименьших квадратов
template <class Tf, class Ti>	// Tf - это float, double. Ti - это целое
bool ApproximationLine_equNo(const Tf * pX, const Tf * pSrc, const Ti len, Tf & k, Tf & b)
{
	bool ret = (pX != nullptr) && (pSrc != nullptr) && ((int)len > 1);
	if (ret)
	{
		double x(0.), xx(0.), y(0.), xy(0.), kd(0.), bd(0.);
		for (int i = 0; i < (int)len; ++i)
		{
			double xd = (double)pX[i];
			double yd = (double)pSrc[i];
			x += xd;
			y += yd;
			xx += xd * xd;
			xy += xd * yd;
		}
		ret = SolutionLineEquation_2(xx, x, x, (double)(int)len, xy, y, &kd, &bd);
		if (ret)
		{
			k = (Tf)kd;
			b = (Tf)bd;
		}
	}

	if (!ret)
	{
		k = b = (Tf)0;
	}
	return ret;
}
//------------------------------------------------------------------------------
// Определение линейного тренда и удаление его из эквидистантного ряда.
template <class Tf, class Ti>	// Tf - это float, double. Ti - это целое
bool RemoveApproximationLine_equ(const Tf * pSrc, Tf * pDst, const Ti len, Tf * pk = nullptr, Tf * pb = nullptr)
{
	bool ret = (pSrc != nullptr) && (pDst != nullptr);
	if (ret)
	{
		ret = (int)len > 1;
		if (ret)
		{
			Tf k(0), b(0);
			ret = ApproximationLine_equ(pSrc, len, k, b);
			if (ret)
			{
				for (int i = 0; i < (int)len; ++i)
					pDst[i] = pSrc[i] - (k * i + b);
				if (pk != nullptr)
					*pk = k;
				if (pb != nullptr)
					*pb = b;
			}
		}
		if (!ret)
			CopyMemory(pDst, pSrc, len * sizeof(Tf));
	}
	if (!ret)
	{
		if (pk != nullptr)
			*pk = 0;
		if (pb != nullptr)
			*pb = 0;
	}
	return ret;
}

// Определение линейного тренда и удаление его из эквидистантного ряда.
template <class Tf, class Ti>	// Tf - это float, double. Ti - это целое
bool RemoveApproximationLine_equ(Tf * pSrcDst, const Ti len, Tf * pk = nullptr, Tf * pb = nullptr)
{
	bool ret = (pSrcDst != nullptr) && ((int)len > 1);
	if (ret)
	{
		Tf k(0), b(0);
		ret = ApproximationLine_equ(pSrcDst, len, k, b);
		if (ret)
		{
			for (int i = 0; i < (int)len; ++i)
				pSrcDst[i] -= k * i + b;
			if (pk != nullptr)
				*pk = k;
			if (pb != nullptr)
				*pb = b;
		}
	}
	if (!ret)
	{
		if (pk != nullptr)
			*pk = 0;
		if (pb != nullptr)
			*pb = 0;
	}
	return ret;
}
//------------------------------------------------------------------------------
// Определение линейного тренда и удаление его из не эквидистантного ряда.
template <class Tf, class Ti>	// Tf - это float, double. Ti - это целое
bool RemoveApproximationLine_equNo(const Tf * pX, const Tf * pSrc, Tf * pDst, const Ti len, Tf * pk = nullptr, Tf * pb = nullptr)
{
	bool ret = (pX != nullptr) && (pSrc != nullptr) && (pDst != nullptr) &&
		((int)len > 1);
	if (ret)
	{
		Tf k(0), b(0);
		ret = ApproximationLine_equNo(pX, pSrc, len, k, b);
		if (ret)
		{
			for (int i = 0; i < (int)len; ++i)
				pDst[i] = pSrc[i] - (k * pX[i] + b);
			if (pk != nullptr)
				*pk = k;
			if (pb != nullptr)
				*pb = b;
		}
		if (!ret)
			CopyMemory(pDst, pSrc, len * sizeof(Tf));
	}
	if (!ret)
	{
		if (pk != nullptr)
			*pk = 0;
		if (pb != nullptr)
			*pb = 0;
	}
	return ret;
}

// Определение линейного тренда и удаление его из не эквидистантного ряда.
template <class Tf, class Ti>	// Tf - это float, double. Ti - это целое
bool RemoveApproximationLine_equNo(const Tf * pX, Tf * pSrcDst, const Ti len, Tf * pk = nullptr, Tf * pb = nullptr)
{
	bool ret = (pX != nullptr) && (pSrcDst != nullptr) && ((int)len > 1);
	if (ret)
	{
		Tf k(0), b(0);
		ret = ApproximationLine_equNo(pX, pSrcDst, len, k, b);
		if (ret)
		{
			for (int i = 0; i < (int)len; ++i)
				pSrcDst[i] -= k * pX[i] + b;
			if (pk != nullptr)
				*pk = k;
			if (pb != nullptr)
				*pb = b;
		}
	}
	if (!ret)
	{
		if (pk != nullptr)
			*pk = 0;
		if (pb != nullptr)
			*pb = 0;
	}
	return ret;
}
//------------------------------------------------------------------------------