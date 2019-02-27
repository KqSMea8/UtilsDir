//----------------------------------------------------------------------
// Класс фильтра, реализующего скользящее среднее
//----------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <Intel\ipp\ipps.h>
#include <FiltrDef.h>
//----------------------------------------------------------------------
class CMovingAverageFiltr
{
private:
	float *m_pData;				// промежуточный массив данных и его
	int m_sizeData;				//			размер
	int m_sizeFiltr;			// длина фильтра, всега не чётное число
	int m_sizeFiltr_2;			// половина длины фильтра

protected:
	bool m_bInit;				// флаг инициализации фильтра
	TypeFiltr m_FType;			// тип фильтра
	float *m_pFreqADC;			// ук. на частоту дискретизации в Гц

	bool _InitFiltr(int lenght);
	bool _Filtration(float *pInpData, int num);

public:
	CMovingAverageFiltr();
	virtual ~CMovingAverageFiltr();
	bool Create(float *pfreqADC);

	// методы чтения/записи свойств -----------------
	TypeFiltr GetTypeFiltr() { return m_FType; }
	int GetLengthInPoint();
	float GetLengthInTime();

	// методы класса --------------------------------
	bool InitFiltrInPoint(int lenght);
	bool InitFiltrInTime(float lenTime);

	// Функции фильтрации
	bool Filtration(float *pInpData, float *pOutData, int num);
	bool Filtration_I(float *pInpOutData, int num);
};
//----------------------------------------------------------------------

