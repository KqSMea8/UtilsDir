//----------------------------------------------------------------------
// Файл IirFiltrSmartInt.h
// Версия от 26.02.2012
// ЗАО "ЭТМС", Зеленоград, Савостин Ю.М.
//----------------------------------------------------------------------
//		Класс предназначен для цифрового интегрирования одномерных
// сигналов с удалением постоянной составляющей как перед, так и после
// фильтрации. Интегрирование первого или второго уровня.
//		В классе используются функции библитеки IPP 7.0.
//		Необходимо соблюдать правило: на каждый сигнал - свой фильтр!!!
//----------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <ippFiltr\IirFiltrSimple.h>
#include <ippFiltr\IirFiltrNoDC.h>
//----------------------------------------------------------------------
class CIirFiltrSmartInt
{
private:
	bool bInit;
	TypeFiltr FType;			// тип фильтра
	CIirFiltrSimple **ppFiltrInt;
	CIirFiltrNoDC **ppFiltrNo;
	int FNumFiltrNo;
	int FNumFiltrInt;
	float *pFreqADC;	// указатель на частоту дискретизации АЦП в Гц

	bool CreateFiltr();
	void DeleteFiltr();

protected:

public:
	CIirFiltrSmartInt();
	virtual ~CIirFiltrSmartInt();
	virtual bool Create(float *pfreqADC);

	// методы чтения/записи свойств -----------------
	TypeFiltr GetTypeFiltr() { return FType; }

	// методы класса --------------------------------
	bool InitFiltr(TypeFiltr type);

	// Функции фильтрации
	bool Filtration(float *pInpData, float *pOutData, int num);
	bool Filtration_I(float *pInpOutData, int num);

	// Переинициализация фильтра со старыми параметрами
	bool ReStart();
};
//----------------------------------------------------------------------

