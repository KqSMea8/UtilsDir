//----------------------------------------------------------------------
// Файл SplineInterpolation.h
// Версия от 28.03.2013
// ЗАО "ЭТМС", Зеленоград, Антонов А.Ю.
//----------------------------------------------------------------------
//	Класс реализует интерполяцию сплайнами
//----------------------------------------------------------------------
#pragma once
#include <Buffer\SmartArray.h>
#include <Interpolation\CustomInterpolation.h>
//----------------------------------------------------------------------
template <class T>
class CSplineInterpolation: public CCustomInterpolation<T>
{
private:
	struct spline_tuple
	{
		float a, b, c, d, x;
		spline_tuple()
			: a(0.f)
			, b(0.f)
			, c(0.f)
			, d(0.f)
			, x(0.f)
		{}
	};

	SmartArray<float> m_saAlfa;
	SmartArray<float> m_saBeta;
	SmartArray<spline_tuple> m_saSplines;
	
	bool _BuildSpline(const float * x, const T * y, long n)
	{
		bool ret = (n > 1) &&
			m_saSplines.ReSize((DWORD)n) &&
			m_saAlfa.ReSize((DWORD)(n - 1)) &&
			m_saBeta.ReSize((DWORD)(n - 1));

		if (ret)
		{
			for (long i = 0; i < n; ++i)
			{
				m_saSplines.pData[i].x = x[i];
				m_saSplines.pData[i].a = float(y[i]);
			}
			m_saSplines.pData[0].c = m_saSplines.pData[n - 1].c = 0.f;

			m_saAlfa.pData[0] = m_saBeta.pData[0] = 0.f;
			for (long i = 1; i < n - 1; ++i)
			{
				float h_i = x[i] - x[i - 1],
					h_i1 = x[i + 1] - x[i];
				float A = h_i;
				float C = 2.f * (h_i + h_i1);
				float B = h_i1;
				float F = 6.f * (float(y[i + 1] - y[i]) / h_i1 - float(y[i] - y[i - 1]) / h_i);
				float z = (A * m_saAlfa.pData[i - 1] + C);
				m_saAlfa.pData[i] = -B / z;
				m_saBeta.pData[i] = (F - A * m_saBeta.pData[i - 1]) / z;
			}

			for (long i = n - 2; i > 0; --i)
				m_saSplines.pData[i].c = m_saAlfa.pData[i] * m_saSplines.pData[i + 1].c + m_saBeta.pData[i];

			for (long i = n - 1; i > 0; --i)
			{
				float h_i = x[i] - x[i - 1];
				m_saSplines.pData[i].d = (m_saSplines.pData[i].c - m_saSplines.pData[i - 1].c) / h_i;
				m_saSplines.pData[i].b = h_i * (2.f * m_saSplines.pData[i].c + m_saSplines.pData[i - 1].c) / 6.f + float(y[i] - y[i - 1]) / h_i;
			}
		}
		return ret;
	}

	T GetValue(float x) const
	{
		if (m_saSplines.size == 0)
			return T(0);

		spline_tuple *pst;
		int n = (int)m_saSplines.size;
		if (x <= m_saSplines.pData[0].x)
			pst = m_saSplines.pData + 1;
		else if (x >= m_saSplines.pData[n - 1].x)
			pst = m_saSplines.pData + n - 1;
		else
		{
			long i = 0, j = n - 1;
			while (i + 1 < j)
			{
				long k = i + (j - i) / 2;
				if (x <= m_saSplines.pData[k].x)
					j = k;
				else
					i = k;
			}
			pst = m_saSplines.pData + j;
		}

		float dx = (x - pst->x);
		return T(pst->a + (pst->b + (pst->c / 2.f + pst->d * dx / 6.f) * dx) * dx);
	}

public:
	CSplineInterpolation()
	{}
	~CSplineInterpolation() 
	{}

	// методы чтения/записи свойств -----------------

	// методы класса --------------------------------
	bool InitInterpolation(TypeInterpolation type) override
	{
		if (type == tiSpline)
		{
			IType = tiSpline;
			bInit = true;
		}
		return bInit;
	}

	// Функция интерполяции
	bool Interpolation(float* pInpXData, T* pInpYData, int iInpSize, float* pOutXData, T* pOutYData, int iOutSize) override
	{
		bool ret = bInit;
		if (ret)
		{
			ret = _BuildSpline(pInpXData, pInpYData, iInpSize);
			if (ret)
			{
				for (int i = 0; i < iOutSize; ++i)
					pOutYData[i] = GetValue(pOutXData[i]);
			}
		}
		return ret;
	}
};
