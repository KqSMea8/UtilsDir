//----------------------------------------------------------------------
// Файл IirFiltrSeismoCorrectionDouble.h
// Версия от 08.09.2016
// ООО "ЭТМС", Зеленоград, Антонов А.Ю.
//----------------------------------------------------------------------
//		Класс реализует фильтр коррекции сейсмометра
//		Необходимо соблюдать правило: на каждый сигнал - свой фильтр!!!
//----------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <ippFiltrDouble\CustomIirFiltrDouble.h>
//----------------------------------------------------------------------
class CIirFiltrSeismoCorrectionDouble : public CCustomIirFiltrDouble
{
private:
	// порядок 2 => размер
	double pTapsDouble[6];	//	массивов = 2 * (2 + 1) = 6
protected:
	double F0;
	double F1;
	double H;
public:
	CIirFiltrSeismoCorrectionDouble();
	virtual ~CIirFiltrSeismoCorrectionDouble();

	// методы чтения/записи свойств -----------------
	
	// методы класса --------------------------------
	bool InitFiltr(TypeFiltr type, double f1, double f2, double h);
	double PriorValueF(double freq);
	double PriorValueH(double h);

	bool ReStart();

	double GetF0() { return F0; }
	double GetF1() { return F1; }
	double GetH() { return H; }
};
//----------------------------------------------------------------------

