//--------------------------------------------------------------------------
// Файл DetectorShiftAmpl.h
// Версия от 13.06.2012
// ЗАО "ЭТМС", Зеленоград, Савостин Ю.М.
//--------------------------------------------------------------------------
//		Класс, реализующий амплитудный детектор многоканального сигнала
//--------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include "Constants.h"
#include "Intel\ipp\ipps.h"
#include "Buffer\ShiftBuffer.h"
//--------------------------------------------------------------------------
class CDetectorShiftAmpl
{
private:
	int FNumberSignals;		// кол-во сигналов детектора
	int FNumberSignals_1;	// кол-во сигналов детектора - 1
	int NumPage;			// кол-во страниц в буфере
	int NumPageCur;			// текущая страница в буфере
	float FThreshold;		// порог детектирования
	float FFactorThreshold;	// множитель
	UINT wmEvent;			// wm события
	HWND hWnd;				// дескриптор окна
	bool FbAutoThreshold;	// флаг автопорога
	bool FbInit;			// флаг, что детектор проинициализирован
	bool bJob;				// флаг, что детектор должен работать
	CShiftBuffer<float> *pBufAmpl,	// сдвиговый буфер амплитуды
		*pBufSummAmpl, *pBufSummAmpl2;
	float summAmpl, summAmpl2, *p2;

	void CalcAutoThreshold();
	void ZeroVariable();

protected:

public:
	CDetectorShiftAmpl();
	virtual ~CDetectorShiftAmpl();
	bool Create(int numberSignal, int size, int sizePage,
				HWND hwnd, UINT wm_EVENT);	

	// функции свойств
	bool GetInitDetector() { return FbInit; }

	float GetThreshold() { return FThreshold; }
	void  SetThreshold(float val)
	{ if (!FbAutoThreshold && (val > 0.f) )	FThreshold = val; }

	bool GetAutoThreshold() { return FbAutoThreshold; }
	void SetAutoThreshold(bool val);

	int GetNumberSignals() { return FNumberSignals; }
	void SetNumberSignals(int newNumber)
	{ if (newNumber > 0) FNumberSignals = newNumber; }

	int GetSize() { return pBufAmpl->GetSize(); }
	bool SetSize(int size);

	int GetSizePage() { return pBufAmpl->GetSizePage(); }
	void SetSizePage(int val);

	float* GetPointerForDataAmpl()
	{ return pBufAmpl->GetGlobalAddres(); }

	// функции класса
	void Calculation(float **ppData);
	void Reset();
};
//--------------------------------------------------------------------------