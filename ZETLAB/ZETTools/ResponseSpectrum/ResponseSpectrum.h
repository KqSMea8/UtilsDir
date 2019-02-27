//!----------------------------------------------------------------------------
//! Класс предназначен для вычисления спектра ответа на сейсмический сигнал.
//! Также может использоваться для вычисления ударного спектра.
//! Автор Бегишев Сергей 2015 г.
//!----------------------------------------------------------------------------
#pragma once

#include "SRS.h"

//!	структура с входными параметрами алгоритма
typedef struct _SResponseIn
{
	double dSampling;			//!	частота дискретизации сигнала, Гц
	double dFreqBegin;			//!	первая частота в спектре ответа, Гц
	double dFreqEnd;			//! последняя частота в спектре ответа, Гц
	double dDecrement;			//! логарифмический декремент затухания
	long lOctaveNum;			//! количество точек в одной октаве
	long lSize;					//! размер входного массива с данными
	double *pData;				//! входной массив с данными акселерометра

	_SResponseIn()
	{
		dSampling = 0.0;
		dFreqBegin = 0.0;
		dFreqEnd = 0.0;
		dDecrement = 0.0;
		lOctaveNum = 0;
		lSize = 0;
		pData = nullptr;
	}
} SResponseIn;

//! структура с выходными параметрами алгоритма
typedef struct _SResponseOut
{
	long lSize;					//! размер выходных массивов данных
	double *pFreqs;				//! частотный ряд
	double *pReps;				//! значения ударного спектра

	_SResponseOut()
	{
		lSize = 0;
		pFreqs = nullptr;
		pReps = nullptr;
	}
} SResponseOut;

//! набор вариантов расчёта спектра
typedef enum _ESRSType
{
	Overall = 0,		//!	суммарный начальный ударный спектр
	Positive = 1,		//!	положительный начальный ударный спектр
	Negative = 2		//!	отрицательный начальный ударный спектр
} ESRSType;

class CResponseSpectrum
{
private:
	SResponseIn m_struct;		//!	структура с параметрами системы и массивом с входным сигналом
	SResponseOut m_response;	//!	структура с массивами спектра ответа

	Srs m_spectr;				//!	класс, выполняющий расчты спектра ответа

protected:
	long SetInputStruct(SResponseIn *pInData);			//!	функция задаёт структуру с параметрами колебательной системы
	long SetInputData(double *pData, long size);		//!	функция задаёт массив с данными для расчёта спектра
	long GetOutputStruct(SResponseOut *pOutData);		//!	функция получает структуру с результатами расчёта спектра ответа

	long CalculateSRSAAM(ESRSType typeSRS = Overall);	//!	функция выполняет расчёт спектра ответа

public:
	CResponseSpectrum();
	~CResponseSpectrum();

	long Calculate(SResponseIn *pInData, SResponseOut *pOutData, ESRSType type = Overall);
};

