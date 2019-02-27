//----------------------------------------------------------------------
// Файл IirFiltrRealResonatorDouble.h
// Версия от 15.01.2016
// ООО "ЭТМС", Зеленоград, Антонов А.Ю.
//----------------------------------------------------------------------
//		Класс реализует фильтр типа действительный резонатор
//		Необходимо соблюдать правило: на каждый сигнал - свой фильтр!!!
//----------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <ippFiltrDouble\CustomIirFiltrDouble.h>
//----------------------------------------------------------------------
class CIirFiltrRealResonatorDouble : public CCustomIirFiltrDouble
{
private:
	// порядок 2 => размер
	double pTapsDouble[6];	//	массивов = 2 * (2 + 1) = 6
	
protected:
	double Decrement;

public:
	CIirFiltrRealResonatorDouble();
	virtual ~CIirFiltrRealResonatorDouble();

	// методы чтения/записи свойств -----------------
	
	// методы класса --------------------------------
	bool InitFiltr(TypeFiltr type, double freq, double decrement);
	double PriorValue(double freq);
	double GetFres() const;
	double GetQ() const;

	bool ReStart();
};
//----------------------------------------------------------------------

