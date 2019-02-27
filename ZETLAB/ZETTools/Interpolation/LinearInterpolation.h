//----------------------------------------------------------------------
// Файл LinearInterpolation.h
// Версия от 28.03.2013
// ЗАО "ЭТМС", Зеленоград, Антонов А.Ю.
//----------------------------------------------------------------------
//	Класс реализует линейную интерполяцию
//----------------------------------------------------------------------
#pragma once
#include <Interpolation\CustomInterpolation.h>
#include <algorithm>
#include <mathZet.h>
//----------------------------------------------------------------------
template <class T>
class CLinearInterpolation: public CCustomInterpolation<T>
{
private:

public:
	CLinearInterpolation(){};
	virtual ~CLinearInterpolation(){};

	// методы чтения/записи свойств -----------------

	// методы класса --------------------------------
	bool InitInterpolation(TypeInterpolation type) override
	{
		if (type == tiLinear)
		{
			IType = tiLinear;
			bInit = true;
		}
		return bInit;
	}

	// Функция интерполяции
	bool Interpolation(float* pInpXData, T* pInpYData, int iInpSize, float* pOutXData, T* pOutYData, int iOutSize) override
	{
		if (iInpSize <= 1 || iOutSize <= 1)
			return false;
		if (bInit)
		{
			float* pFound(NULL);
			float fLevel[2];
			int iPosition(0);
			float fK, fB;
			for (int i = 0; i < iOutSize; ++i)
			{
				fLevel[0] = fLevel[1] = pOutXData[i];
				pFound = std::search(pInpXData, pInpXData + iInpSize, fLevel, fLevel + 1, predicateMore);
				iPosition = pFound - pInpXData;
				if (iPosition >= iInpSize)
					pOutYData[i] = pInpYData[iInpSize - 1];
				else
				{
					if (iPosition <= 0)
					{
						pOutYData[i] = pInpYData[0];
					}
					else
					{
						fK = (pInpYData[iPosition] - pInpYData[iPosition - 1]) / (pInpXData[iPosition] - pInpXData[iPosition - 1]);
						fB = pInpYData[iPosition - 1] - fK * pInpXData[iPosition - 1];
						pOutYData[i] = fK * pOutXData[i] + fB;
					}
				}
			}
		}
		return bInit;
	}
	// Функция интерполяции
	bool InterpolationDouble(double* pInpXData, T* pInpYData, int iInpSize, double* pOutXData, T* pOutYData, int iOutSize)
	{
		if (iInpSize <= 1 || iOutSize <= 1)
			return false;

		if (bInit)
		{
			int iPosition(0);
			double fK, fB;
			for (int i = 0; i < iOutSize; ++i)
			{
				FindIndexOfVal(pOutXData[i], pInpXData, iInpSize, iPosition);

				if (iPosition >= iInpSize)
					pOutYData[i] = pInpYData[iInpSize - 1];
				else
				{
					if (iPosition <= 0)
					{
						pOutYData[i] = pInpYData[0];
					}
					else
					{
						fK = (pInpYData[iPosition] - pInpYData[iPosition - 1]) / (pInpXData[iPosition] - pInpXData[iPosition - 1]);
						fB = pInpYData[iPosition - 1] - fK * pInpXData[iPosition - 1];
						pOutYData[i] = (T)(fK * pOutXData[i] + fB);
					}
				}
			}
		}
		return bInit;
	}
};


