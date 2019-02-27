//----------------------------------------------------------------------
// Файл IirFiltrEnvelope.h
// Версия от 05.03.2012
// ЗАО "ЭТМС", Зеленоград, Антонов А.Ю.
//----------------------------------------------------------------------
//		Класс реализует фильтр для расчёта огибающей, он реализован
// как два последовательных ФНЧ.
//		Исполльзуются функции библиотеки IPP 7.0.
//		Необходимо соблюдать правило: на каждый сигнал - свой фильтр!!!
//----------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <ippFiltr\IirFiltrInternal.h>
//----------------------------------------------------------------------
class CIirFiltrEnvelope
{
private:
	bool bInit;
	bool bNeed;
	TypeFiltr FType;			// тип фильтра
	float time;
	CIirFiltrInternal *pFiltrInternal1, *pFiltrInternal2;
	float *pFreqADC;	// указатель на частоту дискретизации АЦП в Гц

	bool DefineNeedFiltr(float time);
	bool _Filtration(float *pInpOutData, int num);
protected:

public:
	CIirFiltrEnvelope();
	virtual ~CIirFiltrEnvelope();
	virtual bool Create(float *pfreqADC);

	// методы чтения/записи свойств -----------------
	TypeFiltr GetTypeFiltr() { return FType; }
	float GetTime() {return time;}

	// методы класса --------------------------------
	bool InitFiltr(TypeFiltr type, float fTime);

	// Функции фильтрации
	bool Filtration(float *pInpData, float *pOutData, int num);
	bool Filtration_I(float *pInpOutData, int num);
	float PriorValue(float time);
	// Переинициализация фильтра со старыми параметрами
	bool ReStart();
};
//----------------------------------------------------------------------

