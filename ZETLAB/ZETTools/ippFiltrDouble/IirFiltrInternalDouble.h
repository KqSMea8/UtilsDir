//----------------------------------------------------------------------
// Файл IirFiltrInternal.h
// Версия от 03.05.2012
// ЗАО "ЭТМС", Зеленоград, Савостин Ю.М.
//----------------------------------------------------------------------
//		Класс реализует фильтры, расчёт коэффициентов которых
// реализован в библиотеке IPP 7.0. Это ФНЧ и ФВЧ для фильтров
// Баттерворта и Чебышева (фильтры Чебышева только в планах).
//		Необходимо соблюдать правило: на каждый сигнал - свой фильтр!!!
//----------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <ippFiltrDouble\CustomIirFiltrDouble.h>
//----------------------------------------------------------------------
class CIirFiltrInternalDouble : public CCustomIirFiltrDouble
{
private:
	int order;
	bool autoOrder;
	// максимальный порядок 10 => размер
	double pTapsDouble[26];	//	массивов = 2 * (10 + 1) = 22
protected:

public:
	CIirFiltrInternalDouble();
	virtual ~CIirFiltrInternalDouble();

	// методы чтения/записи свойств -----------------
	void SetAutoOrder(bool bAutoOrder);
	int GetOrder();

	// методы класса --------------------------------
	virtual bool InitFiltr(TypeFiltr type, double freq = 0.);
	bool InitFiltr(TypeFiltr type, int ord = 0, double freq = 0.);
	double PriorValue(double freq);

	const double* getTapsDouble() { return pTapsDouble; }
};
//----------------------------------------------------------------------

