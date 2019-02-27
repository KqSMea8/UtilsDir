//------------------------------------------------------------------------------
// Файл IirFiltrNoDCDouble.h
// Версия от 03.05.2012
// ЗАО "ЭТМС", Зеленоград, Савостин Ю.М.
//------------------------------------------------------------------------------
//		Класс реализует фильтр, предназначенный для удаления
// постоянной составляющей.
//		В классе используются функции библитеки IPP 7.0.
//		Необходимо соблюдать правило: на каждый сигнал - свой фильтр!!!
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <ippFiltrDouble\CustomIirFiltrDouble.h>
//------------------------------------------------------------------------------
class CIirFiltrNoDCDouble : public CCustomIirFiltrDouble
{
private:
	double pTapsD[6];
	IppsIIRState_64f *pState[2];
	Ipp8u* pStateBuf[2];

	bool _Init();

protected:

public:
	CIirFiltrNoDCDouble();
	virtual ~CIirFiltrNoDCDouble();
	// методы чтения/записи свойств -----------------

	// методы класса --------------------------------
	virtual bool InitFiltr(TypeFiltr type);

	// Функции фильтрации
	virtual bool Filtration(double *pInpData, double *pOutData, int num);
	virtual bool Filtration_I(double *pInpOutData, int num);

	// Переинициализация фильтра со старыми параметрами
	virtual bool ReStart();
};
//------------------------------------------------------------------------------

