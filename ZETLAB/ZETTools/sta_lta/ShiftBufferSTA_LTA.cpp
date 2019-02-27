//------------------------------------------------------------------------------
// Файл ShiftBufferSTA_LTA.cpp
// Версия от 05.03.2012
// ЗАО "ЭТМС", Зеленоград, Савостин Ю.М.
//------------------------------------------------------------------------------
//	Класс предназначен для создания сдвиговых буферов, ориентированных,
// на работу с детектором STA_LTA
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "math.h"
#include <sta_lta\ShiftBufferSTA_LTA.h>
//------------------------------------------------------------------------------
CShiftBufferSTA_LTA::CShiftBufferSTA_LTA()
{
	m_pSTA_LTA = nullptr;
	m_pSignal = nullptr;
//	FbNeedDelMiddle = _bNeedDelMiddle = false;
}
//------------------------------------------------------------------------------
CShiftBufferSTA_LTA::~CShiftBufferSTA_LTA()
{
	if (m_pSTA_LTA)
	{
		delete [] m_pSTA_LTA;
		m_pSTA_LTA = nullptr;
	}
	if (m_pSignal)
	{
		delete m_pSignal;
		m_pSignal = nullptr;
	}
}
//------------------------------------------------------------------------------
bool CShiftBufferSTA_LTA::Create(int size, int sizePage,
							int numberSTA, int numberLTA)
{
	div_t dt1 = div(size, sizePage);// ,
//		dt2 = div(numberSTA, sizePage);// ,
//		dt3 = div(numberLTA, sizePage);
	
	bool ret = (dt1.rem == 0) && //(dt2.rem == 0) &&// (dt3.rem == 0) && 
		(sizePage < size) &&
		((numberSTA + numberLTA) < size);
	if (ret)
	{
		m_pSignal = new CShiftBuffer<float>();
		ret = m_pSignal->Create(size, sizePage);
		if (ret)
		{
			m_NumberSTA = numberSTA;
			m_NumberLTA = numberLTA;
			m_NumberPoint = m_NumberSTA + m_NumberLTA - 1;
			m_normFactor = (double)m_NumberLTA / (double)m_NumberSTA;

			// номер точки в которой посчитано последнее STA/LTA
			m_nCur_STA_LTA = m_pSignal->GetSize() - m_pSignal->GetSizePage() - numberSTA;
			try
			{
				m_pSTA_LTA = new float[m_pSignal->GetSizePage()];
				InitVariable();
			}
			catch (CMemoryException*)
			{
				ret = false;
			}
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
/*void CShiftBufferSTA_LTA::DefineMiddle()
{// Первоначальное определение постоянной составляющей, bEnableSTA_LTA = false.
	// Уже получено curNumPage страниц данных, т.е. n отсчётов =
	int i, n = curNumPage * pSignal->GetSizePage();
	float *add = pSignal->GetPointerLastElement() - n + 1;
	float *addCur = add;

	// рассчитываю пост. сост.
	middle = 0.0;					// на всякий случай
	for(i=0; i<n; i++)
		middle += (double)*addCur++;
	middle /= (double)n;

	// удаляю пост. сост. из всех уже полученных данных
	addCur = add;
	for(i=0; i<n; i++)
		*addCur++ -= (float)middle;
	bEnableMiddle = true;

	// теперь нужно пересчитать стартовые значения amplSTA и amplLTA
	amplLTA = 0.0;
	for(i=0; i<FNumberLTA; i++)
		amplLTA += fabs( double(*add++) );
	add--;		// общая точка обоих интервалов
	amplSTA = 0.0;
	for(i=0; i<FNumberSTA; i++)
		amplSTA += fabs( double(*add++) );
}*/
//------------------------------------------------------------------------------
void CShiftBufferSTA_LTA::InitVariable()
{
//	FbNeedDelMiddle = _bNeedDelMiddle;
//	bEnableSTA_LTA = bEnableMiddle = false;
//	amplSTA = amplLTA = middle = 0.0;
	m_bEnableSTA_LTA = false;
	m_amplSTA = m_amplLTA = 0.0;
	m_curNum = m_curNumPage = 0;
	ippsZero_32f( m_pSTA_LTA, m_pSignal->GetSizePage() );
}
//------------------------------------------------------------------------------
/*void CShiftBufferSTA_LTA::SubtractMiddleInPage()
{	// вычитание пост. сост. из данных последней порции
	float *addCur = pSignal->GetPointerLastPage();
	for(int i=0; i<pSignal->GetSizePage(); i++)
		*addCur++ -= (float)middle;
}*/
//------------------------------------------------------------------------------
bool CShiftBufferSTA_LTA::SetSize(int size)
{
	bool ret;
	if ( m_pSignal->GetSize() != size )
	{
		ret = m_pSignal->SetSize(size);		
		m_nCur_STA_LTA = m_pSignal->GetSize() - m_pSignal->GetSizePage() - m_NumberSTA;
		InitVariable();
	}
	else
		ret = true;
	return ret;
}
//------------------------------------------------------------------------------
void CShiftBufferSTA_LTA::SetSizePage(int val)
{
	if ( m_pSignal->GetSizePage() != val )
	{
		m_pSignal->SetSizePage(val);
		m_nCur_STA_LTA = m_pSignal->GetSize() - m_pSignal->GetSizePage() - m_NumberSTA;
		delete [] m_pSTA_LTA;
		m_pSTA_LTA = nullptr;
		m_pSTA_LTA = new float [ m_pSignal->GetSizePage() ];
		InitVariable();
	}
}
//------------------------------------------------------------------------------
void CShiftBufferSTA_LTA::SetNumberLTA(int val)
{
	if ( ( m_NumberLTA != val ) && ( val > 0 ) )
	{
		m_NumberLTA = val;
		m_NumberPoint = m_NumberLTA + m_NumberSTA - 1;
		m_normFactor = (double)m_NumberLTA / (double)m_NumberSTA;
		Clear();
	}
}
//------------------------------------------------------------------------------
void CShiftBufferSTA_LTA::SetNumberSTA(int val)
{
	if ( ( m_NumberSTA != val ) && ( val > 0 ) )
	{
		m_NumberSTA = val;
		m_NumberPoint = m_NumberLTA + m_NumberSTA - 1;
		m_normFactor = (double)m_NumberLTA / (double)m_NumberSTA;
		m_nCur_STA_LTA = m_pSignal->GetSize() - m_pSignal->GetSizePage() -
															m_NumberSTA;
		Clear();
	}
}
//------------------------------------------------------------------------------
//		Расчёты отношения средних амплитуд STA / LTA.
// Предполагается, что перед расчётами в буфере были обновлены данные
// последней страницы
//-------------------------------------
void CShiftBufferSTA_LTA::Calculation()
{
	double val;
	if ( !m_bEnableSTA_LTA )
		m_curNumPage++;
	float *p_Val = m_pSignal->GetGlobalAddres() + m_nCur_STA_LTA;
	float *pStart_LTAold = p_Val - m_NumberLTA + 1;
	float *pStart_STAold = p_Val;
	float *pEnd_LTAnew = pStart_STAold + 1;
	float *pEnd_STAnew = p_Val + m_NumberSTA;
	p_Val = m_pSTA_LTA;

	int nnn = m_pSignal->GetSizePage();
	for(int i=0; i<m_pSignal->GetSizePage(); i++)
	{
		// расчёты сумм амплитуд ---------------
		m_amplLTA -= fabs( double(*pStart_LTAold++) );
		m_amplLTA += fabs( double(*pEnd_LTAnew++) );
		
		m_amplSTA -= fabs( double(*pStart_STAold++) );
		m_amplSTA += fabs( double(*pEnd_STAnew++) );

		if (m_bEnableSTA_LTA)
		{	// детектор проинициализирован
			// расчеты отношения амплитуд ----------
			if ( m_amplLTA )
				val = m_amplSTA / m_amplLTA * m_normFactor;
			else
				val = 1.0;
		}
		else
		{	// детектор не проинициализирован
			val = 1.0;
			if ( m_curNum < m_NumberPoint )
				m_curNum++;
			else
				m_bEnableSTA_LTA = true;
		}
		*p_Val++ = float(val);
	}// for(i=0; i<pSignal->GetSizePage(); i++)
}
//------------------------------------------------------------------------------
void CShiftBufferSTA_LTA::Clear()
{	// обнуление данных в буфере и инициализация расчётов STA/LTA
	m_pSignal->Clear();
	InitVariable();
}
//------------------------------------------------------------------------------
void CShiftBufferSTA_LTA::CopyNewData(float *pDataPage)
{	// копирование новых данных в последнюю страницу, после удаление уже
	// определённой постоянной составляющей в данных последней страницы
	// считаю, что сдвиг данных уже выполнен
	m_pSignal->CopyNewData(pDataPage);
//	if ( FbNeedDelMiddle && bEnableMiddle )	// удаление пост. сост.
//		SubtractMiddleInPage();				// в последней порции
}
//------------------------------------------------------------------------------
void CShiftBufferSTA_LTA::NewData(float *pDataPage)
{	// вначале сдвиг данных, затем поток копирование новых данных в последнюю
	// страницу, и, наконец, удаление пост. сост. в последней порции
	m_pSignal->NewData(pDataPage);
//	if ( FbNeedDelMiddle && bEnableMiddle )	// удаление пост. сост.
//		SubtractMiddleInPage();				// в последней порции
}
//------------------------------------------------------------------------------