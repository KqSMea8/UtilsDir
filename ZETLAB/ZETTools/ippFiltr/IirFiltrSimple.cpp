//----------------------------------------------------------------------
// Файл IirFiltrSimple.cpp
// Версия от 28.02.2012
// ЗАО "ЭТМС", Зеленоград, Савостин Ю.М.
//----------------------------------------------------------------------
#include "StdAfx.h"
#include <ippFiltr\IirFiltrSimple.h>
//----------------------------------------------------------------------
// без фильтрации
const float pTapsNo[4]     = {1.f, 0.f,				// коэфф. Bn
								1.f, 0.f};			// коэфф. An
// несмещённая разностная схема для дифференцирования
const float pTapsDiff1[6]  = {0.5f, 0.f, -0.5f,		// коэфф. Bn
								1.f, 0.f, 0.f};		// коэфф. An
// разностная схема для двойного дифференцирования
const float pTapsDiff2[6]  = {1.f, -2.f, 1.f,		// коэфф. Bn
								1.f, 0.f, 0.f};		// коэфф. An
// интегрирование безразмерной величины
const float pTapsInt1[4]   = {0.5f, 0.5f,			// коэфф. Bn
								1.f, -1.f,};			// коэфф. An
// интегрирование величины размерности "g"
// с переводом результата в "мм/c"
const float pTapsInt1g[4]  = {500.f*g, 500.f*g,		// коэфф. Bn
								1.f, -1.f,};			// коэфф. An
// интегрирование величины размерности "м/с^2"
// с переводом результата в "мм/c"
const float pTapsInt1m[4]  = {500.f, 500.f,			// коэфф. Bn
								1.f, -1.f,};			// коэфф. An
//----------------------------------------------------------------------
CIirFiltrSimple::CIirFiltrSimple() : CCustomIirFiltr()
{
	ZeroMemory(pTaps, sizeof(float) * 6);
}
//----------------------------------------------------------------------
CIirFiltrSimple::~CIirFiltrSimple()
{
}
//----------------------------------------------------------------------
bool CIirFiltrSimple::InitFiltr(TypeFiltr type, float freq)
{	// новый тип фильтра
	int order;
	float a = *pFreqADC;
	if ( (type == tfNo)    ||						// if должен быть
		 (type == tfDiff1) || (type == tfDiff2) ||	// именно таким
		 (type == tfInt1)  || (type == tfInt1g) || (type == tfInt1m) )
	{
		FType = type;
		switch (FType)
		{
		case tfNo :
			order = 1;
			ippsCopy_32f(pTapsNo, pTaps, 4);
			break;
		case tfDiff1 :
			order = 2;
			ippsCopy_32f(pTapsDiff1, pTaps, 6);
			pTaps[0] *= a;
			pTaps[2] *= a;
			break;
		case tfDiff2 :
			order = 2;
			ippsCopy_32f(pTapsDiff2, pTaps, 6);
			a *= a;
			pTaps[0] *= a;
			pTaps[1] *= a;
			pTaps[2] *= a;
			break;
		case tfInt1:
			order = 1;
			ippsCopy_32f(pTapsInt1, pTaps, 4);
			pTaps[0] /= a;
			pTaps[1] /= a;
			break;
		case tfInt1g :
			order = 1;
			ippsCopy_32f(pTapsInt1g, pTaps, 4);
			pTaps[0] /= a;
			pTaps[1] /= a;
			break;
		case tfInt1m :
			order = 1;
			ippsCopy_32f(pTapsInt1m, pTaps, 4);
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