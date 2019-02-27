//------------------------------------------------------------------------------
//	Файл CustomFDWT.h
//	Зеленоград, ЗАО "ЭТМС", Савостин Ю.М.
//------------------------------------------------------------------------------
//		Базовый класс предназначен для инициализации первичных и
// вторичных параметров для расчёта коэффициентов быстрого дискретного
// вейвлет преобразования (Fast Discrete Wavelet Transform)
// одномерных сигналов.
//		Наследники этого класса будут реализовывать конкретные алгоритмы
// расчётов. В классе используются функции библитеки IPP 7.0.
//		Первичные параметры (в порядке их установки) и определяемые ими
// вторичные параметры:
//	1. тип материнского вейвлета, определяет длину фильтров;
//	2. длина сигнала (может равняться INFINITE для программ типа
//		"Вейвлет-анализ"), совместно с длиной фильтров определяет
//		максимальный уровень разложения;
//	3. уровень разложения.
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include "math.h"
#include <Intel\ipp\ipps.h>
#include <ippFDWT\Taps_Wavelet.h>
//------------------------------------------------------------------------------
enum TypeWaveletRetValue
{// перечень битов, которые можно объединять с помощью логического ИЛИ
	wtrvError = 0x80000000,	// бит ошибки 
	wtrvNoError = 0,		// ошибок нет, заявленые параметры установлены
	wtrvNewTypeWavelet = 1,	// заявлен не существующий тип вейвлета,
							// установлен вейвлет Хаара
	wtrvNewLevel = 2,		// заявленый уровень разложения невозможен,
							// установлен максимальный из возможных
	wtrvNewLengthSignal = 4	// заявлена некорректная длина сигнала,
};							// установлена минимальная из корректных
//------------------------------------------------------------------------------
enum TypeWavelet
{// перечень типов материнских вейвлетов
	Haar,
	db1,
	db2,
	db3,
	db4,
	db5,
	db6,
	db7,
	db8,
	db9,
	db10,
	sym1,
	sym2,
	sym3,
	sym4,
	sym5,
	sym6,
	sym7,
	coif1,
	coif2,
	coif3,
	coif4,
	coif5,
	no
};
//------------------------------------------------------------------------------
class CCustomFDWT
{
private:
	int FQuantityWavelet;	// кол-во материнских вейвлетов
	long _setTypeWavelet(TypeWavelet val);
	void _setLevel(int val);
	void _setLevelMaxCur();
	void _setLengthSignal(int val);
	CString *m_pNameWavelet;

protected:
	int fwdFltOffs;
	int invFltOffs;
	bool bInitForward;		// структуры прямого преобр. проинициализированы
	bool bInitInverse;		// структуры обратного преобр. проинициализированы
	bool FbNeedInverse;		// требуется только прямое преобразование
	int FLevelOld;			// прежние
	int FLengthSignalOld;	//		значения

	int FLevel;				// уровень разложения
	int FLevelMaxCur;		// макс. уровень для данной длины сигнала
	int FLevelMaxAll;		// внешнее ограничение на максимальный уровень
	int FLengthFilter;		// длина фильтров
	int FLengthSignal;		// длина исходного сигнала
	int FLengthSignalClient;// запрошенная длина сигнала
	TypeWavelet FTypeWavelet;// код типа вейвлета (от 0 до 23)

	// 30 - максимальная длина для coif5
	float pTaps_fwdLo[30], pTaps_fwdHi[30],
		  pTaps_invLo[30], pTaps_invHi[30];

	void SetpSize();
	void SetLevelMaxCur();

	// виртуальные функции, реализующие побочные эффекты при изменении параметров,
	// в наследниках они будут переопределяться
	virtual void NewTypeWavelet() {}
	virtual void NewLevel() {}
	virtual void NewLengthSignal() {}
	virtual void SetNeedInverse(bool val) { FbNeedInverse = val; }

public:
	CCustomFDWT();
	virtual ~CCustomFDWT();
	virtual long Create(TypeWavelet typeWavelet,
		int level, bool needInverse, int lengthSignal = INFINITE);

	// массив размерностей векторов Di для классического разложения
	// или частотных полос для пакетного
	int m_pSize[32];

	// методы чтения/записи свойств -----------------
	bool GetNeedInverse() { return FbNeedInverse; }

	int GetQuantityWavelet() {return FQuantityWavelet;}
	TypeWavelet GetTypeWavelet() {return FTypeWavelet;}
	int GetLengthFiltr() {return FLengthFilter;}

	int GetLengthSignal() {return FLengthSignal;}

	int GetLevel() {return FLevel;}	
	int GetLevelMaxCur() {return FLevelMaxCur;}
	int GetLevelMaxAll() {return FLevelMaxAll;}

	CString NameWavelet(int index);

	// установка параметров извне
	long SetLevel(int val);
	void SetLevelMaxAll(int val);
	long SetLengthSignal(int val, bool bNewLevel = false);
	long SetTypeWavelet(TypeWavelet val);

	// методы класса ----------------------------
};
//------------------------------------------------------------------------------

