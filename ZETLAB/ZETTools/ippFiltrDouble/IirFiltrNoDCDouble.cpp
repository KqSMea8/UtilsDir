//------------------------------------------------------------------------------
// Файл IirFiltrNoDCDouble.cpp
// Версия от 03.05.2012
// ЗАО "ЭТМС", Зеленоград, Савостин Ю.М.
//------------------------------------------------------------------------------
#include "StdAfx.h"
#include <ippFiltrDouble\IirFiltrNoDCDouble.h>
//------------------------------------------------------------------------------
CIirFiltrNoDCDouble::CIirFiltrNoDCDouble() : CCustomIirFiltrDouble()
{
	pState[0] = pState[1] = nullptr;
	pStateBuf[0] = pStateBuf[1] = nullptr;
	ZeroMemory(pTapsD, 6 * sizeof(double));
}
//------------------------------------------------------------------------------
CIirFiltrNoDCDouble::~CIirFiltrNoDCDouble()
{
	for(int i = 0; i < 2; ++i)
	{ 
		if (pStateBuf[i])
		{
			ippsFree(pStateBuf[i]);
			pStateBuf[i] = nullptr;
		}
	}
}
//------------------------------------------------------------------------------
bool CIirFiltrNoDCDouble::_Init()
{
	// Расчет коэффициентов фильтров (предоставлен Антоновым А.Ю.)
	// Расчет взят с сайта http://model.exponenta.ru/audio_eq.html
	
	// Дискретная передаточная функция фильтра второго порядка имеет вид:

	//         (B0/A0) + (B1/A0)*z^-1 + (B2/A0)*z^-2     b0 + b1*z^-1 + b2*z^-2
	// H(z) = --------------------------------------- = ------------------------
	//            1 + (A1/A0)*z^-1 + (A2/A0)*z^-2        1 + a1*z^-1 + a2*z^-2

	// Для расчета фильтра, убирающего постоянную составляющую, воспользуемся ФВЧ с малой частотой среза.
	// Тогда передаточная функция его аналогового фильтра-прототипа

	//             s^2
	// H(s) = ------------- , где Q - добротность
	//        s^2 + s/Q + 1

	// Для перехода от передаточной функции аналогового фильтра к дискретной передаточной функции цифрового фильтра
	// воспользуемся билинейным преобразованием, которое выполняется посредством подстановки:

	//         1       1 - z^-1           2 * PI * frequency
	// s =  ------- * ----------, где w = ------------------, где frequency - частота среза ФВЧ, sampleRate - частота дискретизации
	//      tg(w/2)    1 + z^-1               sampleRate

	// Используя билинейное преобразование, получаем дискретную передаточную функцию цифрового фильтра с коэффициентами:
	
	// B0 = (1 + cos(w)) / 2                 B0 = (1 + cos) / 2
	// B1 = - (1 + cos(w))                   B1 = - (1 + cos)            
	// B2 = (1 + cos(w)) / 2         или     B2 = (1 + cos) / 2,   где  cos = cos(w), alpha = sin(w) / (2 * Q)
	// A0 = 1 + sin(w) / (2 * Q)             A0 = 1 + alpha             
	// A1 = - 2 * cos(w)                     A1 = - 2 * cos
	// A2 = 1 - sin(w) / (2 * Q)             A2 = 1 - alpha

	// Для рассчитываемого фильтра примем:
	//		- добротность (Q), равную 1 / sqrt(2) - примерно 3 дБ/октаву
	//		- соотношение частот (frequency / sampleRate), равное 0.00001 (0.5 Гц при 50 кГц)

	/*double SAMPLE_RATE =	50000.;
	double FREQUENCY =		1.;
	double OMEGA =			IPP_2PI * FREQUENCY / SAMPLE_RATE;
	double ALPHA =			sin(OMEGA) / sqrt(2.);
	double COS =			cos(OMEGA);
	double B0 =				(1. + COS) / 2.;
	double B1 =				- (1. + COS);
	double B2 =				(1. + COS) / 2.;
	double A0 =				1 + ALPHA;
	double A1 =				-2. * COS;
	double A2 =				1 - ALPHA;
	double b0 =				B0 / A0;
	double b1 =				B1 / A0;
	double b2 =				B2 / A0;
	double a0 =				A0 / A0;
	double a1 =				A1 / A0;
	double a2 =				A2 / A0;

	pTapsD[0] = b0;
	pTapsD[1] = b1;
	pTapsD[2] = b2;
	pTapsD[3] = a0;
	pTapsD[4] = a1;
	pTapsD[5] = a2;*/

	double OMEGA = IPP_2PI *
		max( 0.5 / 50000., double(FreqMinHz) / *pFreqADC );
	double ALPHA = sin(OMEGA) / IPP_SQRT2;
	OMEGA = cos(OMEGA);
	double _A0 = 1. / ( 1. + ALPHA );

	pTapsD[0] =									// b0
	pTapsD[2] = ( 1. + OMEGA ) * _A0 * 0.5;		// b2
	pTapsD[1] =-( 1. + OMEGA ) * _A0;			// b1
	pTapsD[3] = 1.;								// a0
	pTapsD[4] = -2. * OMEGA * _A0;				// a1
	pTapsD[5] = ( 1. - ALPHA ) * _A0;			// a2

	bInit = true;
	for (int i = 0; i < 2; ++i)
	{
		if (pStateBuf[i])
		{
			ippsFree(pStateBuf[i]);
			pStateBuf[i] = nullptr;
		}
		int sizeBuffer(0);
		bInit = bInit & (ippsIIRGetStateSize_64f(2, &sizeBuffer) == ippStsNoErr);
		if (bInit)
			pStateBuf[i] = ippsMalloc_8u(sizeBuffer);
		bInit = bInit & (ippsIIRInit_64f(&pState[i], pTapsD, 2, nullptr, pStateBuf[i]) == ippStsNoErr);
	}

	return bInit;
}
//------------------------------------------------------------------------------
bool CIirFiltrNoDCDouble::InitFiltr(TypeFiltr type)
{	// новый тип фильтра
	if (type == tfNoDC)
	{
		FType = tfNoDC;
		bInit = _Init();				
	}
	else
		bInit = false;
	return bInit;
}
//------------------------------------------------------------------------------
bool CIirFiltrNoDCDouble::Filtration(double *pInpData, double *pOutData, int num)
{
	bool ret;
	if (bInit)
	{
		ret = ( ippsIIR_64f(pInpData, pOutData, num, pState[0]) == ippStsNoErr ) &&
			  ( ippsIIR_64f_I(pOutData, num, pState[1]) == ippStsNoErr );
	}
	else
		ret = false;
	return ret;
}
//------------------------------------------------------------------------------
bool CIirFiltrNoDCDouble::Filtration_I(double *pInpOutData, int num)
{
	bool ret;
	if (bInit)
	{		
		ret = ( ippsIIR_64f_I(pInpOutData, num, pState[0]) == ippStsNoErr ) && 
			  ( ippsIIR_64f_I(pInpOutData, num, pState[1]) == ippStsNoErr );
	}
	else
		ret = false;
	return ret;
}
//------------------------------------------------------------------------------
bool CIirFiltrNoDCDouble::ReStart()
{
	bool ret;
	if (bInit)
		ret = _Init();
	else
		ret = false;
	return ret;
}
//------------------------------------------------------------------------------