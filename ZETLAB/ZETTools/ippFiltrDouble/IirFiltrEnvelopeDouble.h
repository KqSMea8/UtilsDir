//----------------------------------------------------------------------
// Файл IirFiltrEnvelopeDouble.h
// Версия от 03.05.2012
// ЗАО "ЭТМС", Зеленоград, Антонов А.Ю.
//----------------------------------------------------------------------
//		Класс реализует фильтр для расчёта огибающей, он реализован
// как два последовательных ФНЧ.
//		Исполльзуются функции библиотеки IPP 7.0.
//		Необходимо соблюдать правило: на каждый сигнал - свой фильтр!!!
//----------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <ippFiltrDouble\IirFiltrInternalDouble.h>
//----------------------------------------------------------------------
class CIirFiltrEnvelopeDouble
{
private:
	bool bInit;
	bool bNeed;
	TypeFiltr FType;			// тип фильтра
	double time;
	CIirFiltrInternalDouble *pFiltrInternal1, *pFiltrInternal2;
	double *pFreqADC;	// указатель на частоту дискретизации АЦП в Гц

	bool DefineNeedFiltr(double time);
	bool _Filtration(double *pInpOutData, int num);
protected:

public:
	CIirFiltrEnvelopeDouble();
	virtual ~CIirFiltrEnvelopeDouble();
	virtual bool Create(double *pfreqADC);

	// методы чтения/записи свойств -----------------
	TypeFiltr GetTypeFiltr() { return FType; }
	double GetTime() {return time;}

	// методы класса --------------------------------
	bool InitFiltr(TypeFiltr type, double fTime);

	// Функции фильтрации
	bool Filtration(double *pInpData, double *pOutData, int num);
	bool Filtration_I(double *pInpOutData, int num);
	double PriorValue(double time);
	// Переинициализация фильтра со старыми параметрами
	bool ReStart();
};
//----------------------------------------------------------------------

