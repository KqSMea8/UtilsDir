//----------------------------------------------------------------------
// Файл LogarithmInterpolation.h
// Версия от 06.06.2013
// ЗАО "ЭТМС", Зеленоград, Антонов А.Ю.
//----------------------------------------------------------------------
//	Класс реализует линейную интерполяцию в логарифмическом масштабе
//----------------------------------------------------------------------
#pragma once
#include <Interpolation\CustomInterpolation.h>
#include <mathZet.h>
//----------------------------------------------------------------------
template <class T>
class CLogarithmInterpolation: public CCustomInterpolation<T>
{
private:

public:
	CLogarithmInterpolation(){};
	virtual ~CLogarithmInterpolation(){};

	// методы чтения/записи свойств -----------------

	// методы класса --------------------------------
	bool InitInterpolation(TypeInterpolation type) override
	{
		if (type == tiLogarithm)
		{
			IType = tiLogarithm;
			bInit = true;
		}
		return bInit;
	}

	// Функция интерполяции
	bool Interpolation(float* pInpXData, T* pInpYData, int iInpSize, float* pOutXData, T* pOutYData, int iOutSize) override
	{
		if (bInit)
		{
			for (int i = 0; i < iOutSize; ++i)
			{
				float fLevel[2] = { pOutXData[i], pOutXData[i] };
				float* pFound = std::search(pInpXData, pInpXData + iInpSize, fLevel, fLevel + 1, predicateMore);
				int position = pFound - pInpXData;
				const float precision = pOutXData[i] * 0.001f;
				if (IsEqual(pOutXData[i], *pFound, precision))
					pOutYData[i] = pInpYData[position];
				else if (position >= iInpSize)
					pOutYData[i] = pInpYData[iInpSize - 1];
				else if (position <= 0)
					pOutYData[i] = pInpYData[0];
				else
				{
					float fK = log10(pInpYData[position] / pInpYData[position - 1]) / log10f(pInpXData[position] / pInpXData[position - 1]);
					float fB = log10(pInpYData[position - 1]) - fK * log10f(pInpXData[position - 1]);
					pOutYData[i] = pow(10.f, fK * log10f(pOutXData[i]) + fB);
				}
			}
		}
		return bInit;
	}
};

