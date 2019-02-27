//----------------------------------------------------------------------
// Файл CustomIirFiltr.h
// Версия от 04.03.2012
// ЗАО "ЭТМС", Зеленоград, Савостин Ю.М.
//----------------------------------------------------------------------
//		Базовый класс для классов, реализующих фильтры IIR.
// Класс также реализует пользовательские фильтры (tfUser).
//----------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <Constants.h>
#include <Intel\ipp\ipps.h>
#include <FiltrDef.h>
//----------------------------------------------------------------------
const float FreqMinHz = 0.2f;
//----------------------------------------------------------------------
class CCustomIirFiltr
{
private:
	IppsIIRState_32f *pState;	// структура фильтра
	Ipp8u* pStateBuf;

protected:
	bool bInit;			// флаг инициализации фильтра
	TypeFiltr FType;	// тип фильтра
	float Freq;			// частота фильтра (если требуется), Гц
	float *pFreqADC;	// указатель на частоту дискретизации АЦП в Гц

	bool _InitFiltr(const float *pTaps, int order);
	bool _Filtration(float *pInpData, float *pOutData, int num,
						bool flag_I);

public:
	CCustomIirFiltr();
	virtual ~CCustomIirFiltr();
	virtual bool Create(float *pfreqADC);

	// методы чтения/записи свойств -----------------
	TypeFiltr GetTypeFiltr() { return FType; }
	float GetFreq() { return Freq; }

	// методы класса --------------------------------
	// Виртуальная функция инициализации фильтра новыми параметрами.
	// В наследниках необходимо переопределять
	virtual bool InitFiltr(TypeFiltr type, float freq = 0.f);

	// Функции фильтрации
	virtual bool Filtration(float *pInpData, float *pOutData, int num);
	virtual bool Filtration_I(float *pInpOutData, int num);

	// Переинициализация фильтра со старыми параметрами
	virtual bool ReStart();		// не для tfUser
	// Задание коэффициентов фильтра и порядка (только tfUser)
	bool SetUserTaps(float *ptaps, int order);
};
//----------------------------------------------------------------------

