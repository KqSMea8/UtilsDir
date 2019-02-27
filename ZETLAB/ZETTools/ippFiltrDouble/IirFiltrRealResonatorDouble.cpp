//----------------------------------------------------------------------
// Файл IirFiltrRealResonatorDouble.cpp
// Версия от 15.01.2016
// ООО "ЭТМС", Зеленоград, Антонов А.Ю.
//----------------------------------------------------------------------
#include "StdAfx.h"
#include <ippFiltrDouble\IirFiltrRealResonatorDouble.h>
//----------------------------------------------------------------------
CIirFiltrRealResonatorDouble::CIirFiltrRealResonatorDouble() : CCustomIirFiltrDouble()
, Decrement(0.)
{
	ZeroMemory(pTapsDouble, 6 * sizeof(double));
}
//----------------------------------------------------------------------
CIirFiltrRealResonatorDouble::~CIirFiltrRealResonatorDouble()
{
}
//----------------------------------------------------------------------
bool CIirFiltrRealResonatorDouble::InitFiltr(TypeFiltr type, double freq, double decrement)
{
	const int order = 2;
	if (type == tfRealResonator)
	{
		FType = type;
		Freq = PriorValue(freq);
		Decrement = decrement;

		/*double omega = IPP_2PI * Freq / (*pFreqADC);
		double cosinus = cos(omega);
		// r - параметр затухания, формула связи с декрементом следующая
		// r = exp(-beta * f0 / fd), где
		// beta - логарифмический декремент затухания
		// f0 - резонансная частота
		// fd - частота дискретизации
		double r = exp(-1. * Decrement * Freq / (*pFreqADC));
		
		pTapsDouble[0] = 0.;
		pTapsDouble[1] = 1. * omega * omega;
		pTapsDouble[2] = 0.;
		pTapsDouble[3] = 1.;
		pTapsDouble[4] = -2. * r * cosinus;
		pTapsDouble[5] = r * r ;*/
		
		TransferFunction tfIn, tfOut;
		tfIn.numerator.setCoeff(0, 1.);
		tfIn.numerator.setCoeff(1, 0.);
		tfIn.numerator.setCoeff(2, 0.);
		tfIn.denomenator.setCoeff(0, 1.);
		tfIn.denomenator.setCoeff(1, 2. * Decrement / IPP_2PI);
		tfIn.denomenator.setCoeff(2, 1.);
		_BilinearTransformation(tfIn, Freq / (*pFreqADC), tfOut);

		pTapsDouble[0] = tfOut.numerator[0].real();
		pTapsDouble[1] = tfOut.numerator[1].real();
		pTapsDouble[2] = tfOut.numerator[2].real();
		pTapsDouble[3] = tfOut.denomenator[0].real();
		pTapsDouble[4] = tfOut.denomenator[1].real();
		pTapsDouble[5] = tfOut.denomenator[2].real();

		//коррекция коэффициента усиления
		double sumA(0.), sumB(0.);
		ippsSum_64f(pTapsDouble, order + 1, &sumA);
		ippsSum_64f(pTapsDouble + order + 1, order + 1, &sumB);
		if (sumB != 0. && sumA != 0.)
			pTapsDouble[1] /= (sumA / sumB);
		
		bInit = true;
	}
	else
		bInit = false;
	// Инициализация внутренней структуры фильтра
	if (bInit)
		bInit = _InitFiltr(pTapsDouble, order);
	return bInit;
}
//----------------------------------------------------------------------
double CIirFiltrRealResonatorDouble::PriorValue(double freq)
{
	double temp, f = freq;
	if ( f > 0. )
	{	// ограничение на минимальное значение
		temp  = *pFreqADC / 10000.;
		if (f < temp)
			f = temp;	
		else
		{	// ограничение Найквиста
			temp = *pFreqADC * 0.5 - temp;
			if (f > temp)
				f = temp;
		}
	}
	else
		f = 0.;
	return f;
}
//----------------------------------------------------------------------
bool CIirFiltrRealResonatorDouble::ReStart()
{
	bool ret;
	if (bInit)
	{// параметры уже заданы
		if (FType != tfUser)
			ret = InitFiltr(FType, Freq, Decrement);
		else
			ret = false;
	}
	else	// параметры ещё не заданы
		ret = false;
	return ret;
}
//----------------------------------------------------------------------
double CIirFiltrRealResonatorDouble::GetFres() const
{
	//return sqrt(Freq * Freq - 2. * Decrement * Decrement);
	return Freq * sqrt(1. - 2. * Decrement * Decrement / IPP_2PI / IPP_2PI);
}
//----------------------------------------------------------------------
double CIirFiltrRealResonatorDouble::GetQ() const
{
	return IPP_PI / Decrement;
}
//----------------------------------------------------------------------
