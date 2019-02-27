//*-------------------------------------------------
//* Класс для вычисления коэффициента корреляции
//* Автор: Бегишев Сергей, ООО "ЭТМС", 2015 год
//*-------------------------------------------------

// TODO: перенести сюда весь функционал вычисления корреляции

#pragma once


class CZetCalcCorrelation
{
	int m_lSize1;
	int m_lSize2;
	int m_lTempSize;

	float* m_fData1;
	float* m_fData2;
	float* m_fTempData;

public:
	CZetCalcCorrelation();
	~CZetCalcCorrelation();

	void InitClass(int size);

	CZetCalcCorrelation& operator=(const CZetCalcCorrelation& right)
	{
		InitClass(right.m_lSize1);

		if (this == &right) 
			return *this;
				
		memcpy_s(m_fData1, sizeof(right.m_fData1[0])*m_lSize1, right.m_fData1, sizeof(right.m_fData1[0])*m_lSize1);
		memcpy_s(m_fData2, sizeof(right.m_fData2[0])*m_lSize2, right.m_fData2, sizeof(right.m_fData2[0])*m_lSize2);
		memcpy_s(m_fTempData, sizeof(right.m_fTempData[0])*m_lTempSize, right.m_fTempData, sizeof(right.m_fTempData[0])*m_lTempSize);
	
		return *this;
	}

	float Coefficient(float* data1, float* data2);
};

