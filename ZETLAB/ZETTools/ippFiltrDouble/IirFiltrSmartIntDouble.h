//----------------------------------------------------------------------
// Файл IirFiltrSmartIntDouble.h
// Версия от 03.05.2012
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
#include <ippFiltrDouble\IirFiltrSimpleDouble.h>
#include <ippFiltrDouble\IirFiltrNoDCDouble.h>
//----------------------------------------------------------------------
class CIirFiltrSmartIntDouble
{
private:
	bool bInit;
	TypeFiltr FType;			// тип фильтра
	CIirFiltrSimpleDouble **ppFiltrInt;
	CIirFiltrNoDCDouble **ppFiltrNo;
	int FNumFiltrNo;
	int FNumFiltrInt;
	double *pFreqADC;	// указатель на частоту дискретизации АЦП в Гц

	bool CreateFiltr();
	void DeleteFiltr();

protected:

public:
	CIirFiltrSmartIntDouble();
	virtual ~CIirFiltrSmartIntDouble();
	virtual bool Create(double *pfreqADC);

	// методы чтения/записи свойств -----------------
	TypeFiltr GetTypeFiltr() { return FType; }
	bool InitFiltr(TypeFiltr type);
	void SetFreqMinInHz(double freq);

	// Функции фильтрации
	bool Filtration(double *pInpData, double *pOutData, int num);
	bool Filtration_I(double *pInpOutData, int num);

	// Переинициализация фильтра со старыми параметрами
	bool ReStart();
};
//----------------------------------------------------------------------

