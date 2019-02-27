//----------------------------------------------------------------------
// Файл IirFiltrSeismoCorrection.cpp
// Версия от 18.01.2018
// ООО "ЗЭТЛАБ", Зеленоград, Антонов А.Ю.
//----------------------------------------------------------------------
#include <ippFiltr\IirFiltrSeismoCorrection.h>
//----------------------------------------------------------------------
CIirFiltrSeismoCorrection::CIirFiltrSeismoCorrection() : CCustomIirFiltr()
, F0(10.)
, F1(1.)
, H(1.)
{
	ippsZero_32f(pTaps, 6);
}
//----------------------------------------------------------------------
CIirFiltrSeismoCorrection::~CIirFiltrSeismoCorrection()
{
}
//----------------------------------------------------------------------
bool CIirFiltrSeismoCorrection::InitFiltr(TypeFiltr type, double f0, double f1, double h)
{
	const int order = 2;
	if (type == tfSeismoCorrection)
	{
		FType = type;
		F0 = PriorValueF(f0);
		F1 = PriorValueF(f1);
		H = h;

		double omega0 = IPP_2PI * F0 / (*pFreqADC);
		double omega1 = IPP_2PI * F1 / (*pFreqADC);
				
		pTaps[2] = (float)(omega0 * omega0 - 4. * H * omega0 + 4.);
		pTaps[1] = (float)(2. * omega0 * omega0 - 8);
		pTaps[0] = (float)(omega0 * omega0 + 4. * H * omega0 + 4.);
		pTaps[5] = (float)(omega1 * omega1 - 4. * H * omega1 + 4.);
		pTaps[4] = (float)(2. * omega1 * omega1 - 8);
		pTaps[3] = (float)(omega1 * omega1 + 4. * H * omega1 + 4.);

		bInit = true;
	}
	else
		bInit = false;
	// Инициализация внутренней структуры фильтра
	if (bInit)
		bInit = _InitFiltr(pTaps, order);
	return bInit;
}
//----------------------------------------------------------------------
double CIirFiltrSeismoCorrection::PriorValueF(double freq)
{
	double temp, f = freq;
	if ( f > 0. )
	{	// ограничение на минимальное значение
		temp = *pFreqADC / 10000.;
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
bool CIirFiltrSeismoCorrection::ReStart()
{
	bool ret;
	if (bInit)
	{// параметры уже заданы
		if (FType != tfUser)
			ret = InitFiltr(FType, F0, F1, H);
		else
			ret = false;
	}
	else	// параметры ещё не заданы
		ret = false;
	return ret;
}
//----------------------------------------------------------------------
double CIirFiltrSeismoCorrection::PriorValueH(double h)
{
	if (h < 0.01)
		h = 0.01;
	else if (h > 100.)
		h = 100.;
	return h;
}
//----------------------------------------------------------------------
