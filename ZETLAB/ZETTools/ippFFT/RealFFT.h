//------------------------------------------------------------------------------
//	Файл RealFFT.h
//	Зеленоград, ЗАО "ЭТМС", Савостин Ю.М.
//------------------------------------------------------------------------------
/*		Класс предназначен для расчёта коэффициентов Фурье-преобразования
реальных входных сигналов. Класс использует быстрое преобразование Фурье,
реализованное функциями библиотеки IPP 7.0. Предварительно входные данные можно
преобразовывать с помощью различных оконных весовых функций.
		Размер исходных данных = 2^order. Размер выходных данных вдвое меньше.
Порядок можно менять в любое время с помощью функции SetOrder.
		В качестве выходных данных функция расчёта коъффициентов FFT можно
использовать внутренние массивы класса.
*/
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include "math.h"
#include <Intel\ipp\ipps.h>
//------------------------------------------------------------------------------
enum TypeWeightFunction
{// перечень типов весовых функций
	twf_no,
	twf_Hamming,
	twf_Hann,
	twf_Bartlett,
//	twf_Blackman,
//	twf_Kaiser,
};
#define Number_Type_Weight_Function 3	// кол-во используемых типов
//------------------------------------------------------------------------------
class CRealFFT
{
private:
	bool m_bInit;
	TypeWeightFunction m_WeightFunc;
	float *m_pFreqADC;
	int m_order;
	int m_sizeDataOut;
	// внутренние буферы для увеличения скорости вычислений
	int m_sizeBuffer_8u;
	Ipp8u *m_pBuffer_8u;
	int m_sizeBuffer_Inp;
	float *m_pBuffer_Inp;
	int m_sizeBuffer_Out;
	float *m_pBuffer_Out;
	// специальная структура-описатель
	IppsFFTSpec_R_32f *m_pFFTSpec;

	void CreateAllBuffer(int sizeBuffer_8u,
		int sizeBuffer_Inp_32f, int sizeBuffer_Out_32f);
	void DeleteAllBuffer();
	bool WeightFunction(float *pDataInp, float *pDataOut);

protected:

public:
	CRealFFT();
	virtual ~CRealFFT();
	bool Create(float *pFreqADC, DWORD order,
		TypeWeightFunction weightFunc = twf_Hamming);

	// методы чтения/записи свойств -----------------
	TypeWeightFunction GetWeightFunction() { return m_WeightFunc; }
	bool SetWeightFunction(TypeWeightFunction val);

	float GetFrequencyResolution();

	float BandMiddleFreq(int indxBand);

	DWORD GetOrder() { return m_order; }
	bool SetOrder(DWORD val);

	// Размеры данных после Create или SetOrder
	DWORD GetSizeForInputData() { return (DWORD)m_sizeBuffer_Inp; }
	DWORD GetSizeForOutputData() { return (DWORD)m_sizeDataOut; }
	
	// указатели на результат при pDataOut = NULL
	float* GetPointerToResultMagn() { return m_pBuffer_Inp; }
	Ipp32fc* GetPointerToResultCplx() { return (Ipp32fc*)m_pBuffer_Out; }

	// методы класса ----------------------------
	// Прямое преобразование, результат выдаётся в комплексный массив.
	// pDataOut может = NULL, тогда результат по адресу GetPointerToResultCplx
	bool Forward_ToCplx(float *pDataInp, Ipp32fc *pDataOut);
	// Прямое преобразование, результат - магнитуда (корень квадратный из
	// суммы квадратов Re и Im). Размер pDataInp вдвое больше размера pDataOut.
	// pDataOut может равняться pDataInp.
	// pDataOut может = NULL, тогда результат по адресу GetPointerToResultMagn
	bool Forward_ToMagn(float *pDataInp, float *pDataOut);
};
//------------------------------------------------------------------------------

