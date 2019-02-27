//------------------------------------------------------------------------------
// Файл CDetectorShiftSTA_LTA.cpp
// Версия от 05.03.2012
// ЗАО "ЭТМС", Зеленоград, Савостин Ю.М.
//------------------------------------------------------------------------------
//	Класс предназначен для создания детектора STA_LTA, основанного на
// многоканальном (N >= 1) временном сигнале
//------------------------------------------------------------------------------
#include "stdafx.h"
#include <sta_lta\DetectorShiftSTA_LTA.h>
//------------------------------------------------------------------------------
CDetectorShiftSTA_LTA::CDetectorShiftSTA_LTA()
{
	bEvent = false;
	m_bRemovalOneComponentEvent = false;
	m_Threshold = 3.0;
	Factor = 0.8f;
	m_pData_in = nullptr;
	m_ppBufAll = NULL;
	m_pSTA_LTA = NULL;
	pBufDate = NULL;
	T0 = T1 = 0.;
}
//------------------------------------------------------------------------------
CDetectorShiftSTA_LTA::~CDetectorShiftSTA_LTA()
{
	if (m_pData_in)
	{
		delete[] m_pData_in;
		m_pData_in = nullptr;
	}
	if (m_pSTA_LTA)
	{
		delete m_pSTA_LTA;
		m_pSTA_LTA = nullptr;
	}
	DeleteAllBufSTA_LTA();
}
//------------------------------------------------------------------------------
bool CDetectorShiftSTA_LTA::Create(int numberSignal, int size, int sizePage,
				int numberSTA, int numberLTA,
				HWND hwnd, UINT wm_EVENT_START, UINT wm_EVENT_END,
				CShiftBufferDATE *pbufDate)
{
	m_pSTA_LTA = new CShiftBuffer<float>();
	bool ret = m_pSTA_LTA->Create(size, sizePage);
	if (ret)
	{
		ret = CreateAllBufSTA_LTA(numberSignal, size, sizePage,
										numberSTA, numberLTA);
		if (ret)
		{
			FNumberSignals = FNumberSignals_1 = numberSignal;
			FNumberSignals_1--;
			FNumberSTA = numberSTA;
			FNumberLTA = numberLTA;
			FAllNumber = FNumberLTA + FNumberSTA;
			FCountInEvent = 0;
			wmEventStart = wm_EVENT_START;
			wmEventEnd = wm_EVENT_END;
			hWnd = hwnd;
			pBufDate = pbufDate;
			m_pData_in = new float[numberSignal];
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
bool CDetectorShiftSTA_LTA::CreateAllBufSTA_LTA(int numSignal,
	int size, int sizePage, int numberSTA, int numberLTA)
{
	bool ret = true;
	m_ppBufAll = new CShiftBufferSTA_LTA* [numSignal];
	for(int i=0; i<numSignal; i++)
	{
		m_ppBufAll[i] = new CShiftBufferSTA_LTA();
		ret &= m_ppBufAll[i]->Create(size, sizePage, numberSTA, numberLTA);
	}
	return ret;
}
//------------------------------------------------------------------------------
void CDetectorShiftSTA_LTA::DeleteAllBufSTA_LTA()
{
	if (m_ppBufAll)
	{
		for(int i=0; i<FNumberSignals; i++)
		{ 
			delete m_ppBufAll[i];
			m_ppBufAll[i] = nullptr;
		}
		delete [] m_ppBufAll;
		m_ppBufAll = nullptr;
	}
}
//------------------------------------------------------------------------------
bool CDetectorShiftSTA_LTA::SetNumberSignals(int newNumber)
{
	bool ret = true;
	if ( FNumberSignals != newNumber )
	{
		DeleteAllBufSTA_LTA();
		ret = CreateAllBufSTA_LTA( newNumber,
					m_pSTA_LTA->GetSize(), m_pSTA_LTA->GetSizePage(),
					FNumberSTA, FNumberLTA );
		m_pSTA_LTA->Clear();
		FNumberSignals_1 = FNumberSignals = newNumber;
		FNumberSignals_1--;

		if (m_pData_in)
		{ 
			delete[] m_pData_in;
			m_pData_in = nullptr;
		}
		m_pData_in = new float[FNumberSignals];
	}
	return ret;
}
//------------------------------------------------------------------------------
void CDetectorShiftSTA_LTA::SetNumberSTA(int val)
{
	for(int i=0; i<FNumberSignals; i++)
		m_ppBufAll[i]->SetNumberSTA(val);
	FNumberSTA = val;
	FAllNumber = FNumberLTA + FNumberSTA;
}
//------------------------------------------------------------------------------
void CDetectorShiftSTA_LTA::SetNumberLTA(int val)
{
	for(int i=0; i<FNumberSignals; i++)
		m_ppBufAll[i]->SetNumberLTA(val);
	FNumberLTA = val;
	FAllNumber = FNumberLTA + FNumberSTA;
}
//------------------------------------------------------------------------------
bool CDetectorShiftSTA_LTA::SetSize(int size)
{
	bool ret = true;
	if ( m_pSTA_LTA->GetSize() != size )
	{
		if ( m_pSTA_LTA->SetSize(size) )
		{
			for(int i=0; i<FNumberSignals; i++)
			{
				if ( !m_ppBufAll[i]->SetSize(size) )
					ret = false;
			}
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
void CDetectorShiftSTA_LTA::SetSizePage(int val)
{
	if ( m_pSTA_LTA->GetSizePage() != val )
	{
		m_pSTA_LTA->SetSizePage(val);
		for(int i=0; i<FNumberSignals; i++)
			m_ppBufAll[i]->SetSizePage(val);
	}
}
//------------------------------------------------------------------------------
Calculation_Ret_Value CDetectorShiftSTA_LTA::Calculation(
	float **ppData, int *pIndex)
{	//	Расчеты значений детектора для данных последнего буфера.
	// Данные из сервера копирую в буферы детектора и в каждом буфере
	// рассчитываю STA/LTA.
	Calculation_Ret_Value ret = crv_NoEvent;
	int i, n;
	for(i=0; i<FNumberSignals; i++)
	{
		m_ppBufAll[i]->NewData( ppData[i] );
		m_ppBufAll[i]->Calculation();
	}

	// Сдвигаю данные в буфере детектора.
	m_pSTA_LTA->ShiftData();

	// Рассчитываю новые данные детектора и их складываю в буфер детектора.
	float *addCur = m_pSTA_LTA->GetGlobalAddres() +
						m_ppBufAll[0]->m_nCur_STA_LTA;

	// цикл по отсчётам последней порции
	for(n=0; n<m_pSTA_LTA->GetSizePage(); n++)
	{
		float val = 0.f;	// текущее значение сигнала детектора
		for (i = 0; i < FNumberSignals; i++)
			m_pData_in[i] = m_ppBufAll[i]->GetPointerSTA_LTA()[n];

		bool flag = m_bRemovalOneComponentEvent;
		if (flag)
		{
			int num(0);
			for (i = 0; i < FNumberSignals; i++)
			{
				if (m_pData_in[i] > m_Threshold)
					++num;
			}
			flag = num == 1;
		}
		for (i = 0; i < FNumberSignals; i++)
		{
			float x_in = m_pData_in[i];
			if (flag && (x_in > m_Threshold))
				val += 1.f;
			else
				val += x_in;
		}	
		val -= (float)FNumberSignals_1;
		*addCur++ = val;
		
		// работа с событиями --------------------------
		if (bEvent)
		{	// событие наблюдается
			if ( val < 0.f )
				val = 0.001f;
			factorAmpl += log( Factor * val );
			if ( factorAmpl < 0.0 )
			{	// событие завершилось
				bEvent = false;
				ret = crv_EndEvent;
				i = n + m_pSTA_LTA->GetSize() -
								m_pSTA_LTA->GetSizePage();
				if (pBufDate)	// T1 - обязательно
					T1 = pBufDate->GetGlobalAddres()[i];
				if (pIndex)
					*pIndex = i;
				if ( hWnd )
					::PostMessageW( hWnd, wmEventEnd, i, (LPARAM)&T1 );
			}
			if (++FCountInEvent == FAllNumber)
			{// длительность события превысила допустимый размер
				bEvent = false;
				ret = crv_EndEvent;
				if (hWnd)
					::PostMessageW(hWnd, wmEventEnd, 0, 0);
			}
		}
		else
		{	// события нет
			if ( val > m_Threshold )
			{	// детектор сработал, началось событие
				bEvent = true;
				factorAmpl = 0.0;
				FCountInEvent = 0;
				ret = crv_StartEvent;
				i = n + m_pSTA_LTA->GetSize() -
								m_pSTA_LTA->GetSizePage();
				if (pBufDate)	// T0 - обязательно
					T0 = pBufDate->GetGlobalAddres()[i];
				if (pIndex)
					*pIndex = i;
				if ( hWnd )
					::PostMessageW( hWnd, wmEventStart, i, (LPARAM)&T0 );
			}
		}
	} // for(n=0; n<m_pSTA_LTA->GetSizePage(); n++)
	return ret;
}
//------------------------------------------------------------------------------
void CDetectorShiftSTA_LTA::Clear()
{
	bEvent = false;
	m_pSTA_LTA->NaN();
	for(int i=0; i<FNumberSignals; i++)
		m_ppBufAll[i]->Clear();
}
//------------------------------------------------------------------------------