//----------------------------------------------------------------------
// Файл IirFiltrRealResonatorDouble.cpp
// Версия от 15.01.2016
// ООО "ЭТМС", Зеленоград, Антонов А.Ю.
//----------------------------------------------------------------------
#include <ippFiltrDouble\IirFiltrSeismoCorrectionDouble.h>
//----------------------------------------------------------------------
CIirFiltrSeismoCorrectionDouble::CIirFiltrSeismoCorrectionDouble() : CCustomIirFiltrDouble()
, F0(10.)
, F1(1.)
, H(1.)
{
	ZeroMemory(pTapsDouble, 6 * sizeof(double));
}
//----------------------------------------------------------------------
CIirFiltrSeismoCorrectionDouble::~CIirFiltrSeismoCorrectionDouble()
{
}
//----------------------------------------------------------------------
bool CIirFiltrSeismoCorrectionDouble::InitFiltr(TypeFiltr type, double f0, double f1, double h)
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
				
		pTapsDouble[2] = omega0 * omega0 - 4. * H * omega0 + 4.;
		pTapsDouble[1] = 2. * omega0 * omega0 - 8;
		pTapsDouble[0] = omega0 * omega0 + 4. * H * omega0 + 4.;
		pTapsDouble[5] = omega1 * omega1 - 4. * H * omega1 + 4.;
		pTapsDouble[4] = 2. * omega1 * omega1 - 8;
		pTapsDouble[3] = omega1 * omega1 + 4. * H * omega1 + 4.;

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
double CIirFiltrSeismoCorrectionDouble::PriorValueF(double freq)
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
bool CIirFiltrSeismoCorrectionDouble::ReStart()
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
double CIirFiltrSeismoCorrectionDouble::PriorValueH(double h)
{
	if (h < 0.01)
		h = 0.01;
	else if (h > 100.)
		h = 100.;
	return h;
}
//----------------------------------------------------------------------
