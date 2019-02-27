//----------------------------------------------------------------------------------------------------------------------
#pragma once
//----------------------------------------------------------------------------------------------------------------------
// Аппроксимация для не эквидистантного ряда - поиск кривой y = a * x^2 + b * x + c,
// т.е. определение значений коэффициентов a, b и c.
template <class Tf, class Ti>	// Tf - это float, double. Ti - это целое
bool ApproximationParabola_equNo(const Tf* pX, const Tf* pSrc, const Ti len, Tf& a, Tf& b, Tf& c)
{
	a = b = c = (Tf)0;

	bool ret = (pX != nullptr) && (pSrc != nullptr) && len > 1;
	if (ret)
	{
		Tf s4(0.), s3(0.), s2(0.), s1(0.), s0(0.), sy2(0.), sy1(0.), sy0(0.);
		for (Ti i = 0; i < len; ++i)
		{
			s4 += pow(pX[i], (Tf)4);
			s3 += pow(pX[i], (Tf)3);
			s2 += pow(pX[i], (Tf)2);
			s1 += pX[i];
			s0 += (Tf)1;
			sy2 += pSrc[i] * pow(pX[i], (Tf)2);
			sy1 += pSrc[i] * pX[i];
			sy0 += pSrc[i];
		}

		Tf u2 = s3 - s2 * s4 / s3;
		Tf u1 = s2 - s1 * s4 / s3;
		Tf u0 = sy2 - sy1 * s4 / s3;
		Tf v2 = s2 - s1 * s3 / s2;
		Tf v1 = s1 - s0 * s3 / s2;
		Tf v0 = sy1 - sy0 * s3 / s2;

		c = (u0 * v2 - v0 * u2) / (u1 * v2 - v1 * u2);
		b = (u0 - c * u1) / u2;
		a = (sy2 - c * s2 - b* s3) / s4;
	}

	return ret;
}
//----------------------------------------------------------------------------------------------------------------------