//------------------------------------------------------------------------------
// Швблонный класс CSystemEquations_2 предназначен для решения системы
// двух линейных уравнений:
//		m_a11 * x + m_a12 * y = m_b1
//		m_a21 * x + m_a22 * y = m_b2
//------------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------------
template <class T>				// T - это float, double
double CalcDet_2(
	const T a11, const T a12,
	const T a21, const T a22)
{
	return (double)a11 * (double)a22 - (double)a12 * (double)a21;
}
//------------------------------------------------------------------------------
// Швблонныая функция для решения системы из двух линейных уравнений:
//		a11 * x + a12 * y = b1
//		a21 * x + a22 * y = b2
//------------------------------------------------------------------------------
template <class T>				// T - это float, double
bool SolutionLineEquation_2(
	const T a11, const T a12,
	const T a21, const T a22,
	const T  b1, const T  b2,
	T * pX, T *pY)
{
	bool bX = pX != nullptr;
	bool bY = pY != nullptr;	
	bool ret = bX || bY;
	if (ret)
	{
		double det = CalcDet_2(a11, a12, a21, a22);
		if (det != 0.)		// IsNotEqual здесь нельзя
		{
			if (bX)
			{
				*pX = (T)(CalcDet_2(b1, a12, b2, a22) / det);
			}
			if (bY)
			{
				*pY = (T)(CalcDet_2(a11, b1, a21, b2) / det);
			}
		}
		else
		{
			ret = false;
			if (bX)
				*pX = (T)0;
			if (bY)
				*pY = (T)0;
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>				// T - это float, double
class CSystemEquations_2
{
public:
	T	m_a11, m_a12,
		m_a21, m_a22,
		m_b1, m_b2;
	
	CSystemEquations_2()
		: m_a11 (0)
		, m_a12 (0)
		, m_a21 (0)
		, m_a22 (0)
		, m_b1 (0)
		, m_b2 (0)
	{}

	bool Solution(T &x, T &y)
	{
		double det = (double)m_a11 * (double)m_a22 - (double)m_a12 * (double)m_a21;
		bool ret = det != T(0);				// IsNotEqual здесь нельзя
		if (ret)
		{
			x = (T)(((double)m_b1 * (double)m_a22 - (double)m_b2 * (double)m_a12) / det);
			y = (T)(((double)m_b2 * (double)m_a11 - (double)m_b1 * (double)m_a21) / det);
		}
		return ret;
	}
};
//------------------------------------------------------------------------------
// Швблонныая функция для расчёта детерминанта матрицы 3 на 3
//		a11 * x + a12 * y + a13 * z = b1
//		a21 * x + a22 * y + a23 * z = b2
//		a31 * x + a32 * y + a33 * z = b3
//------------------------------------------------------------------------------
template <class T>				// T - это float, double
double CalcDet(
	const T a11, const T a12, const T a13,
	const T a21, const T a22, const T a23,
	const T a31, const T a32, const T a33)
{
	return
		(double)a11 * (double)a22 * (double)a33 +
		(double)a12 * (double)a23 * (double)a31 +
		(double)a13 * (double)a21 * (double)a32 -
		(double)a13 * (double)a22 * (double)a31 -
		(double)a12 * (double)a21 * (double)a33 -
		(double)a11 * (double)a23 * (double)a32;
}
//------------------------------------------------------------------------------
// Швблонныая функция для решения системы из трёх линейных уравнений:
//		a11 * x + a12 * y + a13 * z = b1
//		a21 * x + a22 * y + a23 * z = b2
//		a31 * x + a32 * y + a33 * z = b3
//------------------------------------------------------------------------------
template <class T>				// T - это float, double
bool SolutionLineEquation_3(
	const T a11, const T a12, const T a13,
	const T a21, const T a22, const T a23,
	const T a31, const T a32, const T a33,
	const T  b1, const T  b2, const T  b3,
	T * pX, T *pY, T * pZ)
{
	bool bX = pX != nullptr;
	bool bY = pY != nullptr;
	bool bZ = pZ != nullptr;
	bool ret = bX || bY || bZ;
	if (ret)
	{
		double det = CalcDet(a11, a12, a13, a21, a22, a23, a31, a32, a33);
		if (det != 0.)		// IsNotEqual здесь нельзя
		{
			if (bX)
				*pX = (T)(CalcDet(b1, a12, a13, b2, a22, a23, b3, a32, a33) / det);
			if (bY)
				*pY = (T)(CalcDet(a11, b1, a13, a21, b2, a23, a31, b3, a33) / det);
			if (bZ)
				*pZ = (T)(CalcDet(a11, a12, b1, a21, a22, b2, a31, a32, b3) / det);
		}
		else
		{
			ret = false;
			if (bX)
				*pX = (T)0;
			if (bY)
				*pY = (T)0;
			if (bZ)
				*pZ = (T)0;
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>				// T - это float, double
bool SolutionLineEquation_3abc(
	const T x1, const T x2, const T x3,
	const T y1, const T y2, const T y3,
	T * pa, T *pb, T * pc)
{
	return SolutionLineEquation_3(
		x1*x1, x1, (T)1,
		x2*x2, x2, (T)1,
		x3*x3, x3, (T)1,
		y1, y2, y3,
		pa, pb, pc);
}
//------------------------------------------------------------------------------