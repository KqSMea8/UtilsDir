//----------------------------------------------------------------------
// Файл IirFiltrSeismoCorrection.h
// Версия от 18.01.2018
// ООО "ЗЭТЛАБ", Зеленоград, Антонов А.Ю.
//----------------------------------------------------------------------
//		Класс реализует фильтр коррекции сейсмометра
//		Необходимо соблюдать правило: на каждый сигнал - свой фильтр!!!
//----------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <ippFiltr\CustomIirFiltr.h>
//----------------------------------------------------------------------
class CIirFiltrSeismoCorrection : public CCustomIirFiltr
{
private:
	// порядок 2 => размер
	float pTaps[6];	//	массивов = 2 * (2 + 1) = 6

protected:
	double F0;
	double F1;
	double H;

public:
	CIirFiltrSeismoCorrection();
	virtual ~CIirFiltrSeismoCorrection();

	// методы чтения/записи свойств -----------------
	
	// методы класса --------------------------------
	bool InitFiltr(TypeFiltr type, double f1, double f2, double h);
	double PriorValueF(double freq);
	double PriorValueH(double h);

	bool ReStart();

	const double GetF0() const { return F0; }
	const double GetF1() const { return F1; }
	const double GetH() const { return H; }
};
//----------------------------------------------------------------------

