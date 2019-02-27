//----------------------------------------------------------------------
// Файл CustomIirFiltrDouble.h
// Версия от 03.05.2012
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
#include <ippFiltrDouble\polynom.h>
//----------------------------------------------------------------------
class CCustomIirFiltrDouble
{
private:
	void sub(double* x1, double* z, double* e, double* y, int t, int n);

	IppsIIRState_64f* pState;	// структура фильтра
	Ipp8u* pStateBuf;

protected:
	bool bInit;			// флаг инициализации фильтра
	TypeFiltr FType;	// тип фильтра
	double Freq;		// частота фильтра (если требуется), Гц
	double *pFreqADC;	// указатель на частоту дискретизации АЦП в Гц
	double FreqMinHz = 0.2;

	bool _InitFiltr(const double *pTaps, int order);
	bool _Filtration(double *pInpData, double *pOutData, int num,
						bool flag_I);

	typedef struct TransferFunction
	{
		Polynom numerator;
		Polynom denomenator;
	} TransferFunction;
	void _BilinearTransformation(TransferFunction tfFunction, double dFrequency,
		TransferFunction &tfReturn);

public:
	CCustomIirFiltrDouble();
	virtual ~CCustomIirFiltrDouble();
	virtual bool Create(double *pfreqADC);

	// методы чтения/записи свойств -----------------
	bool IsInit() { return bInit;  }
	TypeFiltr GetTypeFiltr() { return FType; }
	double GetFreq() { return Freq; }
	double* GetPointerToFreqADC() { return pFreqADC; }
	void SetFreqMinInHz(double freq);

	// методы класса --------------------------------
	// Виртуальная функция инициализации фильтра новыми параметрами.
	// В наследниках необходимо переопределять
	virtual bool InitFiltr(TypeFiltr type, double freq = 0.);

	// Функции фильтрации
	virtual bool Filtration(double *pInpData, double *pOutData, int num);
	virtual bool Filtration_I(double *pInpOutData, int num);

	// Переинициализация фильтра со старыми параметрами
	virtual bool ReStart();		// не для tfUser
	// Задание коэффициентов фильтра и порядка (только tfUser)
	bool SetUserTaps(double *ptaps, int order);
};
//----------------------------------------------------------------------

