//------------------------------------------------------------------------------
//	CShiftBufferDATE.h
//	Версия от 05.05.2011
//	Зеленоград, ЗАО "ЭТМС", Савостин Ю.М.
//------------------------------------------------------------------------------
//	Класс CShiftBufferDATE предназначен для реализации сдвигового буфера
// временных данных типа DATE.
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <Constants.h>
#include <Buffer\ShiftBuffer.h>
//------------------------------------------------------------------------------
class CShiftBufferDATE
{
private:
	CShiftBuffer<DATE> *pbuf;
	DATE FdtDate;
	double FdtSec;

protected:

public:
	CShiftBufferDATE();
	virtual ~CShiftBufferDATE();
	bool Create(int size, int sizePage, double dtSec, DATE date = 0.0);

	// функции свойств
	DATE GetdtDate() { return FdtDate; }

	double GetdtSec() { return FdtSec; }
	void SetdtSec(double val);

	DATE* GetGlobalAddres() { return pbuf->GetGlobalAddres(); }
	DATE* GetPointerLastPage() { return pbuf->GetPointerLastPage(); }
	DATE* GetPointerLastElement() { return pbuf->GetPointerLastElement(); }
	int GetSize() { return pbuf->GetSize(); }
	bool SetSize(int size) { return pbuf->SetSize(size); }
	int GetSizePage() { return pbuf->GetSizePage(); }
	void SetSizePage(int val) { pbuf->SetSizePage(val); }

	// функции класса
	void NewData();				// сдвиг и задание времён новой порции
	void NewTime(DATE date);	// задание времён по последнему элементу
	void ShiftData() { pbuf->ShiftData(); }
};
//------------------------------------------------------------------------------