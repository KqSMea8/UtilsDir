//----------------------------------------------------------------------
// Файл IirFiltrSimpleDouble.cpp
// Версия от 03.05.2012
// ЗАО "ЭТМС", Зеленоград, Савостин Ю.М.
//----------------------------------------------------------------------
#include "StdAfx.h"
#include <ippFiltrDouble\IirFiltrSimpleDouble.h>
//----------------------------------------------------------------------
// без фильтрации
const double pTapsNo[4]     = {1., 0.,				// коэфф. Bn
								1., 0.};			// коэфф. An
// несмещённая разностная схема для дифференцирования
const double pTapsDiff1[6]  = {0.5, 0., -0.5,		// коэфф. Bn
								1., 0., 0.};		// коэфф. An
// разностная схема для двойного дифференцирования
const double pTapsDiff2[6]  = {1., -2., 1.,			// коэфф. Bn
								1., 0., 0.};		// коэфф. An
// коэффициенты интеграторов получены эмпирически (формула прямоугольнков
// задирает верхние частоты, формула трапеций опускает верхние частоты)
// интегрирование безразмерной величины
const double pTapsInt1[4]   = {0.875, 0.125,		// коэфф. Bn
								1., -1.,};			// коэфф. An
// интегрирование величины размерности "g"
// с переводом результата в "мм/c"
const double pTapsInt1g[4]  = {875.*g, 125.*g,		// коэфф. Bn
								1., -1.,};			// коэфф. An
// интегрирование величины размерности "м/с^2"
// с переводом результата в "мм/c"
const double pTapsInt1m[4]  = {875., 125.,			// коэфф. Bn
								1., -1.,};			// коэфф. An
//----------------------------------------------------------------------
CIirFiltrSimpleDouble::CIirFiltrSimpleDouble() : CCustomIirFiltrDouble()
{
	ZeroMemory(pTaps, 6*sizeof(double));
}
//----------------------------------------------------------------------
CIirFiltrSimpleDouble::~CIirFiltrSimpleDouble()
{
}
//----------------------------------------------------------------------
bool CIirFiltrSimpleDouble::InitFiltr(TypeFiltr type, double freq)
{	// новый тип фильтра
	int order;
	double a = *pFreqADC;
	if ( (type == tfNo)    ||						// if должен быть
		 (type == tfDiff1) || (type == tfDiff2) ||	// именно таким
		 (type == tfInt1)  || (type == tfInt1g) || (type == tfInt1m) )
	{
		FType = type;
		switch (FType)
		{
		case tfNo :
			order = 1;
			ippsCopy_64f(pTapsNo, pTaps, 4);
			break;
		case tfDiff1 :
			order = 2;
			ippsCopy_64f(pTapsDiff1, pTaps, 6);
			pTaps[0] *= a;
			pTaps[2] *= a;
			break;
		case tfDiff2 :
			order = 2;
			ippsCopy_64f(pTapsDiff2, pTaps, 6);
			a *= a;
			pTaps[0] *= a;
			pTaps[1] *= a;
			pTaps[2] *= a;
			break;
		case tfInt1:
			order = 1;
			ippsCopy_64f(pTapsInt1, pTaps, 4);
			pTaps[0] /= a;
			pTaps[1] /= a;
			break;
		case tfInt1g :
			order = 1;
			ippsCopy_64f(pTapsInt1g, pTaps, 4);
			pTaps[0] /= a;
			pTaps[1] /= a;
			break;
		case tfInt1m :
			order = 1;
			ippsCopy_64f(pTapsInt1m, pTaps, 4);
			pTaps[0] /= a;
			pTaps[1] /= a;
			//break;
		}
		bInit = true;
	}
	else
		bInit = false;
	// Инициализация структуры фильтра
	if (bInit)
		bInit = _InitFiltr(pTaps, order);
	return bInit;
}
//----------------------------------------------------------------------