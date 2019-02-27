//--------------------------------------------------------------------------
// Файл ShiftBufferSTA_LTA.h
// Версия от 05.03.2012
// ЗАО "ЭТМС", Зеленоград, Савостин Ю.М.
//--------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <Buffer\ShiftBuffer.h>
#include <Intel\ipp\ipps.h>
//--------------------------------------------------------------------------
class CShiftBufferSTA_LTA
{
private:
	int m_NumberSTA;		// кол-во точек в интервале STA
	int m_NumberLTA;		// кол-во точек в интервале LTA
	int m_NumberPoint;		// суммарное кол-во точек в обоих интервалах
	int m_curNum;			// текущее кол-во точек при инициализации
	int m_curNumPage;		// текущее кол-во страниц при инициализации
	float* m_pSTA_LTA;		// массив последних значений (размер FSizePage)
	bool m_bEnableSTA_LTA;	// кол-во текущих точек больше FNumberPoint

	double m_amplSTA;			// амплитуда в интервале STA
	double m_amplLTA;			// амплитуда в интервале LTA
	double m_normFactor;		// нормировочное значение = NumberLTA/NumberSTA
	CShiftBuffer<float> *m_pSignal;

	void InitVariable();	// инициализация переменных

protected:

public:
	CShiftBufferSTA_LTA();
	virtual ~CShiftBufferSTA_LTA();
	bool Create(int size, int sizePage, int numberSTA, int numberLTA);

	int m_nCur_STA_LTA;		// номер точки где посчитано последнее STA/LTA

	// функции свойств	
	float* GetGlobalAddres() { return m_pSignal->GetGlobalAddres(); }
	float* GetPointerLastPage() { return m_pSignal->GetPointerLastPage(); }
	float* GetPointerLastElement() { return m_pSignal->GetPointerLastElement(); }

	bool GetEnableSTA_LTA() { return m_bEnableSTA_LTA; }

	int GetSize() { return m_pSignal->GetSize(); }
	bool SetSize(int size);			// новый размер буфера

	int GetSizePage() { return m_pSignal->GetSizePage(); }
	void SetSizePage(int val);		// расчет всех переменных класса
	
	int GetNumberSTA() { return m_NumberSTA; }
	void SetNumberSTA(int val);

	int GetNumberLTA() { return m_NumberLTA; }
	void SetNumberLTA(int val);

	// функции класса
	void Calculation();		// расчёты по данным последней страницы
	void Clear();			// обнуление и приведение к исходу
	void CopyNewData(float *pDataPage);	// копирование данных новой порции
	float* GetPointerSTA_LTA() { return m_pSTA_LTA; }
	void NewData(float *pDataPage);	// сдвиг и копирование новых данных
	void ShiftData() { m_pSignal->ShiftData(); }	// только сдвиг данных
};
//--------------------------------------------------------------------------