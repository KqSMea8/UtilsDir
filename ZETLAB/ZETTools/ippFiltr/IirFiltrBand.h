//----------------------------------------------------------------------
// Файл IirFiltrBand.h
// Версия от 26.02.2012
// ЗАО "ЭТМС", Зеленоград, Савостин Ю.М.
//----------------------------------------------------------------------
//		Класс предназначен для цифровой фильтрации одномерных сигналов
// с помощью фильтра Баттерворта. Реализованы полосовой и режекторный
// фильтры. Можно и ФНЧ и ФВЧ, но для этих фильтров лучще использовать
// класс CIirFiltrInternal.
//		В классе используются функции библитеки IIP 7.0
//		Необходимо соблюдать правило: на каждый сигнал - свой фильтр!!!
//----------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <ippFiltr\IirFiltrInternal.h>
//----------------------------------------------------------------------
class CIirFiltrBand
{
private:
	TypeFiltr FType;
	float Ffmin, Ffmax;
	float *pTemp;
	int nTemp;
	CIirFiltrInternal *pFiltrHiPass, *pFiltrLoPass;
	bool bNeedHi, bNeedLo;
	bool bInit;

	bool DefineNeedFiltr(float freq);

protected:

public:
	CIirFiltrBand();
	virtual ~CIirFiltrBand();
	virtual bool Create(float *pfreqADC);

	// методы чтения/записи свойств -----------------
	TypeFiltr GetTipFiltr() { return FType; }
	float GetFmin();
	float GetFmax();

	// методы класса --------------------------------
	bool Filtration(float *pInpData, float *pOutData, int num);
	bool Filtration_I(float *pInpOutData, int num);
	bool InitFiltr(TypeFiltr type, float fmin, float fmax);
	float PriorValueFmin(float freq);
	float PriorValueFmax(float freq);
	bool ReStart();
};
//----------------------------------------------------------------------

