//------------------------------------------------------------------------------
//	Файл Decimation.h
//	Зеленоград, ЗАО "ЭТМС", Савостин Ю.М.
//------------------------------------------------------------------------------
/*		Класс предназначен для формирования, хранения и выдачи децимированных
данных. Первый отчет, который уходит в буфер децимированных данных - это
первый отчёт с временем, у которого целое число секунд. Далее - каждый 10-тый.
		Перед децимацией исходные данные подвергаюся фильтрации с помощью ФНЧ,
у которого частота среза равна 5% от частоты дискретизации исходных данных.
		Класс не поддерживает работу, при которой меняется частота дискретизации
исходных данных.
*/
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <Buffer\SmartArray.h>
#include <Decimation\BufferForDataWithTime.h>
#include <ippFiltrDouble\IirFiltrInternalDouble.h>
//------------------------------------------------------------------------------
class CDecimation
{
private:
	bool m_bInit;
	bool m_bEnableFirst;
	BYTE m_index;
	const BYTE m_indexMax;
	double m_FreqADC;
	double m_dt;
	const double m_precision;
	double m_lastTime;
	CBufferForDataWithTime<double> *m_pBufferDecimal;
	CIirFiltrInternalDouble *m_pFiltr;
	SmartArray<double> m_dataDouble;

protected:

public:
	CDecimation();
	virtual ~CDecimation();
	bool Create(float freqADC, DWORD sizeDecimalBuffer = 0);

	// методы чтения/записи свойств -----------------
	bool GetIsInit() { return m_bInit; }
	// Размеры данных после Create
	DWORD GetSizeDataDecimal() {
		return m_bInit ? m_pBufferDecimal->GetSize() : 0; }
	
	// указатель на результат

	// методы класса ----------------------------
	bool PutData(double time, int number, float *pData, long quality);
	long GetData(double time, int number, double *pData, long *pQuality = NULL);
	long GetData(double time, int number, float *pData, long *pQuality = NULL);
};
//------------------------------------------------------------------------------

