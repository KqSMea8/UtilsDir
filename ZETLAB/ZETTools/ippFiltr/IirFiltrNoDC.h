//------------------------------------------------------------------------------
// Файл IirFiltrNoDC.h
// Версия от 04.03.2012
// ЗАО "ЭТМС", Зеленоград, Савостин Ю.М.
//------------------------------------------------------------------------------
//		Класс реализует фильтр, предназначенный для удаления
// постоянной составляющей.
//		В классе используются функции библитеки IPP 7.0.
//		Необходимо соблюдать правило: на каждый сигнал - свой фильтр!!!
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <ippFiltr\CustomIirFiltr.h>
//------------------------------------------------------------------------------
class CIirFiltrNoDC : public CCustomIirFiltr
{
private:
	double pTapsD[6];
	IppsIIRState_64f* pState[2];
	Ipp8u* pStateBuf[2];
	double *pData;
	int numCur;
	bool _Init();
	void NewNum(int num);

protected:

public:
	CIirFiltrNoDC();
	virtual ~CIirFiltrNoDC();
	// методы чтения/записи свойств -----------------

	// методы класса --------------------------------
	virtual bool InitFiltr(TypeFiltr type);

	// Функции фильтрации
	virtual bool Filtration(float *pInpData, float *pOutData, int num);
	virtual bool Filtration_I(float *pInpOutData, int num);

	// Переинициализация фильтра со старыми параметрами
	virtual bool ReStart();
};
//------------------------------------------------------------------------------

