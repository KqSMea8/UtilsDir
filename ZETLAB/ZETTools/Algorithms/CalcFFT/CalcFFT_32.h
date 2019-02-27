//------------------------------------------------------------------------------
/* Файл CalcFFT_32.h

	Класс CCalcFFT_32f ПРЕДНАЗНАЧЕН для выполнения расчетов прямого и обратного
БПФ для данных типа float. Класс является насоедником базового класса CustomCalcFFT".

	СОСТАВ класса:
- тройка ООП (деструктор, конструктор копирования и оператор присваивания);
- три функции для выполнения прямого БПФ;
- три функции для выполнения обратного БПФ.

	Наследуюемын от базового класса функции:
- четыре свойста с функциями Get и Set;
- две функции получения размеров.

	Функции для выполнения ПРЯМОГО БПФ
1. CalcFwd_RToC - реальные исходные данные преобразуются в комплексный спектр.
Вначале выполняется прямое БПФ, результат которого выдаётся в формате Perm
(формат библиотеки IPP 7.0). Затем результат преобразуется в комплексный спектр.
2. CalcFwd_C0ToC - реальные исходные данные преобразуются в комплексный спектр.
Вначале создаётся комплексный входной сигнал, у которого реальная часть -
исходные данные функции, а мнимая часть равна 0. Затем выполняется прямое БПФ,
результат которого комплексный спектр.
3. CalcFwd_CHToC - реальные исходные данные преобразуются в комплексный спектр.
Вначале из реальных входных данных с помощью пробразования Гильберта создаётся
комплексные данные, т.е. создаётся аналитический сигнал. Затем выполняется
прямое БПФ, результат которого комплексный спектр.

	Функции для выполнения ОБРАТНОГО БПФ
1. CalcInv_CToC - преобразование комплексного спектра в комплексный сигнал.
2. CalcInv_CToR - преобразование комплексного спектра в реальный сигнал. Вначале
комплексный спектр преобразуется в данные формата Perm. Затем выполнется обратное
БПФ, результатом которого являются реальные данные.
3. CalcInv_CToC_I - преобразование комплексного спектра в комплексный сигнал.
При этом для клмплексного сигнала используется массив с исходным спектром.
*/
//------------------------------------------------------------------------------
#pragma once
#include "CustomCalcFFT.h"
//------------------------------------------------------------------------------
class CCalcFFT_32f : public CCustomCalcFFT
{
private:
	// нормировочный множитель, зависящий от временного окна и типа входных данных
	Ipp32f m_factorFFT;

	// Структура для расчета преобразования Гильберта
	IppsHilbertSpec * m_pStruct_Hilb;
	SmartArray<Ipp8u> m_saStruct_Hilb;
	SmartArray<Ipp8u> m_saStruct_WorkHilb;
//	IppsHilbertSpec_32f32fc * m_pStruct_Hilb;

	// Структуры для расчета FFT
	IppsFFTSpec_R_32f * m_pStruct_R;
	IppsFFTSpec_C_32fc * m_pStruct_C;
	SmartArray<Ipp8u> m_saStruct_R;
	SmartArray<Ipp8u> m_saStruct_C;

	SmartArray<float> m_saWindow;		// временное окно

	// Массивы для данных, size = m_sizeFFT
	SmartArray<float> m_saFwd_SrcRe;
	SmartArray<float> m_saFwd_SrcIm;
	SmartArray<Ipp32fc> m_saFwd_SrcC;
	
	// Массив с данными в формате Perm, size = m_sizeFFT
	SmartArray<float> m_saDataPerm;

	// Приватные функции ------------------------------------

	// Удаление служебных структур
	virtual void DeleteStructure_All();
	virtual void DeleteStructure_FFT();
	virtual void ReSize_saWindow(DWORD size);

	void DeleteStructure_Hilbert();
	long Create_Structure_R();
	long Create_Structure_C();
	long Create_Structure_Hilb(const int size);

	void Calc_FactorFFT();
	long Create_ReDataForFwd(float * pData, const int sizeData, int * psize);
	bool SetTimeWindow(const int sizeData);
	void CorrectZeroItem(Ipp32fc * pData);
	bool ConvertToPerm(Ipp32fc * pData);
	void CorrectConstComponent(const float re0, float * pData, const int size);
	void CorrectConstComponent(const float re0, Ipp32fc * pData, const int size);

protected:

public:
	CCalcFFT_32f(const int order = 7);
	~CCalcFFT_32f();
	CCalcFFT_32f(const CCalcFFT_32f & val);
	CCalcFFT_32f & operator = (const CCalcFFT_32f & val);

	// Прямое FFT.
	// Преобразование реального сигнала в комплексный спектр.
	// - pSrc - реальные входные данные с размером от 1 до m_sizeFFT;
	// - pDst - комплексные выходные данные с размером m_sizeFFT;
	// - sizeSrc - размер входного массива.
	// Если sizeSrc = 0, то размер входного массива = m_sizeFFT.
	// Если sizeSrc меньше m_sizeFFT, то перед расчетами во входной массив
	// добавляются нули. Если больше - то лишние данные игнорируются.
	// Возвращается код ошибки ErrorCodes.
	long CalcFwd_RToC(float * pSrc, Ipp32fc * pDst, const int sizeSrc = 0);

	// Прямое FFT. Преобразование комплексного сигнала с нулевой мнимой частью
	// в комплексный спектр.
	// - pSrc - реальные входные данные Re с размером от 1 до m_sizeFFT;
	// - pDst - комплексные выходные данные с размером m_sizeFFT;
	// - sizeSrc - размер входного массива.
	// Если sizeSrc = 0, то размер входного массива = m_sizeFFT.
	// Если sizeSrc меньше m_sizeFFT, то перед расчетами во входной массив
	// добавляются нули. Если больше - то лишние данные игнорируются.
	// Возвращается код ошибки ErrorCodes.
	long CalcFwd_C0ToC(float * pSrc, Ipp32fc * pDst, const int sizeSrc = 0);

	// Прямое FFT. Преобразование комплексного сигнала с мнимой частью,
	// определяемой с помощью преобразования Гильберта, в комплексный спектр.
	// - pSrc - реальные входные данные Re с размером от 1 до m_sizeFFT;
	// - pDst - комплексные выходные данные с размером m_sizeFFT;
	// - sizeSrc - размер входного массива.
	// Если sizeSrc = 0, то размер входного массива = m_sizeFFT.
	// Если sizeSrc меньше m_sizeFFT, то перед расчетами во входной массив
	// добавляются нули. Если больше - то лишние данные игнорируются.
	// Возвращается код ошибки ErrorCodes.
	long CalcFwd_CHToC(float * pSrc, Ipp32fc * pDst, const int sizeSrc = 0);

	// Расчёт обратного FFT. Преобразование комплексного спектра в
	// реальный сигнал.
	// - pSrc - комплексные входные данные размером m_sizeFFT;
	// - pDst - реальные выходные данные размером m_sizeFFT.
	// Возвращается код ошибки ErrorCodes.
	long CalcInv_CToR(Ipp32fc * pSrc, float * pDst);

	// Расчёт обратного FFT. Преобразование комплексного спектра в
	// комплексный сигнал.
	// - pSrc - комплексные входные данные размером m_sizeFFT;
	// - pDst - комплексные выходные данные размером m_sizeFFT.
	// Возвращается код ошибки ErrorCodes.
	long CalcInv_CToC(Ipp32fc * pSrc, Ipp32fc * pDst);

	// Расчёт обратного FFT. Преобразование комплексного спектра в
	// комплексный сигнал.
	// - pSrcDst - комплексные входные и выходные данные размером m_sizeFFT;
	// Возвращается код ошибки ErrorCodes.
	long CalcInv_CToC_I(Ipp32fc * pSrcDst);
};
//------------------------------------------------------------------------------