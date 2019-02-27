//----------------------------------------------------------------------
// Файл IirFiltrInternal.cpp
// Версия от 03.05.2012
// ЗАО "ЭТМС", Зеленоград, Савостин Ю.М.
//----------------------------------------------------------------------
#include "StdAfx.h"
#include <ippFiltrDouble\IirFiltrInternalDouble.h>
//----------------------------------------------------------------------
CIirFiltrInternalDouble::CIirFiltrInternalDouble() : CCustomIirFiltrDouble()
, autoOrder(true)
, order(0)
{
	ZeroMemory(pTapsDouble, 26 * sizeof(double));
}
//----------------------------------------------------------------------
CIirFiltrInternalDouble::~CIirFiltrInternalDouble()
{
}
//----------------------------------------------------------------------
bool CIirFiltrInternalDouble::InitFiltr(TypeFiltr type, double freq)
{	
	return InitFiltr(type, 0, freq);
}
//----------------------------------------------------------------------
bool CIirFiltrInternalDouble::InitFiltr(TypeFiltr type, int ord, double freq)
{	// новый тип фильтра
	IppStatus curStatus;
	if ((type == tfLowPass) || (type == tfHighPass))
	{
		FType = type;
		Freq = PriorValue(freq);

		double tmp, FreqCatOff;
		tmp = FreqCatOff = Freq / *pFreqADC;
		if (ord > 0 && ord <= 12)
			order = ord;
		else if (autoOrder)
		{
			if (tmp > 0.25)
				tmp = 0.5 - tmp;
			if      (tmp > 0.1 ) order = 12;
			else if (tmp > 0.05 ) order = 10;
			else if (tmp > 0.02) order = 8;
			else if (tmp > 0.005) order = 6;
			else if (tmp > 0.001) order = 4;
			else				 order = 2;
		}
		else
			order = 2;

		if (FType == tfLowPass)
		{
			int sizeBuffer(0);
			curStatus = ippsIIRGenGetBufferSize(order, &sizeBuffer);
			if (curStatus == ippStsNoErr)
			{
				Ipp8u* pBuffer = ippsMalloc_8u(sizeBuffer);
				curStatus = ippsIIRGenLowpass_64f(FreqCatOff, 0., order,
					pTapsDouble, ippButterworth, pBuffer);
				ippsFree(pBuffer);
			}
		}
		else	//== tfHighPass
		{
			int sizeBuffer(0);
			curStatus = ippsIIRGenGetBufferSize(order, &sizeBuffer);
			if (curStatus == ippStsNoErr)
			{
				Ipp8u* pBuffer = ippsMalloc_8u(sizeBuffer);
				curStatus = ippsIIRGenHighpass_64f(FreqCatOff, 0., order,
					pTapsDouble, ippButterworth, pBuffer);
				ippsFree(pBuffer);
			}
		}
		bInit = curStatus == ippStsNoErr;
	}
	else
		bInit = false;
	// Инициализация внутренней структуры фильтра
	if (bInit)
		bInit = _InitFiltr(pTapsDouble, order);
	return bInit;
}
//----------------------------------------------------------------------
double CIirFiltrInternalDouble::PriorValue(double freq)
{
	double temp, f = freq;
	if ( f > 0. )
	{	// ограничение на минимальное значение
		temp = *pFreqADC / 500000.;
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
void CIirFiltrInternalDouble::SetAutoOrder( bool bAutoOrder )
{
	autoOrder = bAutoOrder;
	if (bInit)
		InitFiltr(FType, Freq);
}
//----------------------------------------------------------------------
int CIirFiltrInternalDouble::GetOrder()
{
	return order;
}
//----------------------------------------------------------------------
