//--------------------------------------------------------------------------
//	Модуль CShiftBufferDATE.cpp
//	Версия 1.2 от 05.05.2011
//	Зеленоград, ЗАО "ЭТМС", Савостин Ю.М.
//--------------------------------------------------------------------------
//	Класс CShiftBufferDATE предназначен для реализации сдвигового буфера
// временных данных типа DATE.
//--------------------------------------------------------------------------
#include "stdafx.h"
#include <Buffer\ShiftBufferDATE.h>
//--------------------------------------------------------------------------
CShiftBufferDATE::CShiftBufferDATE()
{
	pbuf = NULL;
}
//------------------------------------------------------------------------------
CShiftBufferDATE::~CShiftBufferDATE()
{
	if (pbuf)
	{
		delete pbuf;
		pbuf = nullptr;
	}
}
//------------------------------------------------------------------------------
bool CShiftBufferDATE::Create(int size, int sizePage, double dtSec, DATE date)
{
	bool ret = true;
	try
	{
		pbuf = new CShiftBuffer<DATE> ();
		if ( pbuf->Create(size, sizePage) )
		{
			SetdtSec(dtSec);
			if ( date != 0.0 )
				NewTime(date);
		}
	}
	catch (CMemoryException*)
	{
		ret = false;
	}
	return ret;
}
//------------------------------------------------------------------------------
void CShiftBufferDATE::SetdtSec(double val)
{
	FdtSec = val;
	FdtDate = FdtSec * SecAsPartOfDay;
}
//------------------------------------------------------------------------------
void CShiftBufferDATE::NewData()
{	// сдвиг и расчёт времен новой порции
	DATE curDate = *pbuf->GetPointerLastElement();
	ShiftData();
	DATE *padd = pbuf->GetPointerLastPage();

	int n = pbuf->GetSizePage() - 1;
	padd[n] = curDate + FdtDate * pbuf->GetSizePage();	// последняя точка
	for(int i=0; i<n; i++)				// остальные точки
	{
		curDate += FdtDate;
		*padd++ = curDate;
	}
}
//------------------------------------------------------------------------------
void CShiftBufferDATE::NewTime(DATE date)
{	// задание времени последнего элемента буфера
	DATE *padd = pbuf->GetPointerLastElement();
	DATE curDate = date;
	for(int i=0; i<pbuf->GetSize(); i++)
	{
		*padd-- = curDate;
		curDate -= FdtDate;
	}
}
//------------------------------------------------------------------------------