//----------------------------------------------------------------------
// Файл IirFiltrInternal.cpp
// Версия от 05.03.2012
// ЗАО "ЭТМС", Зеленоград, Савостин Ю.М.
//----------------------------------------------------------------------
#include "StdAfx.h"
#include <ippFiltr\IirFiltrInternal.h>
//----------------------------------------------------------------------
CIirFiltrInternal::CIirFiltrInternal() : CCustomIirFiltr()
, autoOrder(true)
, order(0)
{
	ZeroMemory(pTapsFloat, sizeof(float) * 22);
	ZeroMemory(pTapsDouble, sizeof(double) * 22);
}
//----------------------------------------------------------------------
CIirFiltrInternal::~CIirFiltrInternal()
{
}
//----------------------------------------------------------------------
bool CIirFiltrInternal::InitFiltr(TypeFiltr type, float freq)
{	// новый тип фильтра
	IppStatus curStatus;
	if ( (type == tfLowPass) || (type == tfHighPass) )
	{
		FType = type;
		Freq = PriorValue(freq);

		float tmp, FreqCatOff;
		tmp = FreqCatOff = Freq / *pFreqADC;
		if (autoOrder)
		{
			if (tmp > 0.25f)
				tmp = 0.5f - tmp;
			if      (tmp > 0.2f ) order = 10;
			else if (tmp > 0.1f ) order = 8;
			else if (tmp > 0.05f) order = 6;
			else if (tmp > 0.01f) order = 4;
			else				  order = 2;
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
		if (bInit)
			ippsConvert_64f32f( pTapsDouble, pTapsFloat,
								2 * ( order + 1) );
	}
	else
		bInit = false;
	// Инициализация внутренней структуры фильтра
	if (bInit)
		bInit = _InitFiltr(pTapsFloat, order);
	return bInit;
}
//----------------------------------------------------------------------
float CIirFiltrInternal::PriorValue(float freq)
{
	float temp, f = freq;
	if ( f > 0.f )
	{	// ограничение на минимальное значение
		temp = max( *pFreqADC / 25000.f, FreqMinHz / *pFreqADC );
		if (f < temp)
			f = temp;	
		else
		{	// ограничение Найквиста
			temp = *pFreqADC * 0.5f - temp;
			if (f > temp)
				f = temp;
		}
	}
	else
		f = 0.f;
	return f;
}
//----------------------------------------------------------------------
void CIirFiltrInternal::SetAutoOrder( bool bAutoOrder )
{
	autoOrder = bAutoOrder;
	if (bInit)
		InitFiltr(FType, Freq);
}
//----------------------------------------------------------------------
