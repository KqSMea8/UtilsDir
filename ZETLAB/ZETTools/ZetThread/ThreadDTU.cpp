//------------------------------------------------------------------------------
//	Модуль ZetWriteDTU.cpp
//	Версия от 15.11.2011
//	Зеленоград, ЗАО "ЭТМС", Савостин Ю.М.
//------------------------------------------------------------------------------
//		Класс предназначен для записи данных в dtu-файл. 
//		Файл будет записан в формате UTF-8.
//------------------------------------------------------------------------------
//		Коды сообщений об ошибках в родительское окно (WPARAM в PostMessage)
// 1 - ошибка при открытии файла
//------------------------------------------------------------------------------
#include "StdAfx.h"
#include "ThreadDTU.h"
//------------------------------------------------------------------------------
DWORD WINAPI WriteDtuThread(PVOID pParam)
{
	//CString str; не используется
	const int maxSize = 16000;
	int i, n, n0, n1, numKadr = 0;
	DATE TimeFirstPoint;	// время первого кадра в файле
	DATE TimeEndPoint;		// время последного кадра в файле
	DATE TimeEvent;			// длительность события
	DATE TimeFirstCur;		// время первой точки текущей выборки
	DATE TimeEndCur;		// время последней точки текущей выборки

	ZetThreadParam0   *par0 = (ZetThreadParam0*)pParam;
	ZetThreadParamDTU *par1 = (ZetThreadParamDTU*)par0->pParamNext;
	ParamPrivat *parPri = par1->pPrivat;
	ParamTotal  *parTot = par1->pTotal;

	// время ожидания потока = четверть времени сбора одной порции данных
	DWORD sleepTime = DWORD( parTot->pBufDate->GetSizePage() /
								parPri->pWriter->m_FreqADC * 250 );

	/////////////////////////////////////////////////////////////////////
	// считаю временные координаты начала и конца
	double a = 2.;
	double a3 = 0.8 * parTot->pBufDate->GetSize() /
						parPri->pWriter->m_FreqADC * SecAsPartOfDay;
	TimeEvent = parPri->T1 - parPri->T0;
	if ( TimeEvent < 0. )
	{// ошибка синхронизации данных
		par0->FlagViewStop = true;
		return 0;
	}
	TimeFirstPoint = parPri->T0 - a * TimeEvent;
	TimeEndPoint   = parPri->T1 + a * TimeEvent;
	if ( ( TimeEndPoint - TimeFirstPoint ) > a3 )
	{
		if ( TimeEvent > a3 )
		{// Начало события важнее
			TimeFirstPoint = parPri->T0;
			TimeEndPoint   = TimeFirstPoint + a3;
		}
		else
		{// подбираю величину "a"
			a = 0.5 * ( a3 / TimeEvent - 1. );
			TimeFirstPoint = parPri->T0 - a * TimeEvent;
			TimeEndPoint   = parPri->T1 + a * TimeEvent;
		}
	}
	double dt = SecAsPartOfDay / parPri->pWriter->m_FreqADC;
	numKadr = int( ( TimeEndPoint - TimeFirstPoint ) / dt  );

	if ( numKadr > maxSize )
		numKadr = maxSize;

	/////////////////////////////////////////////////////////////////////
	// жду когда наступит время писать файл ---------------------------
	bool flag = true;
	do
	{
		TimeFirstCur = *parTot->pBufDate->GetGlobalAddres();
		TimeEndCur = *parTot->pBufDate->GetPointerLastElement();

		n0 = int( (TimeFirstPoint - TimeFirstCur) / dt );
		n1 = int( (TimeEndPoint   - TimeFirstCur) / dt );

		if ( n0 < 0 )
		{// начало события уже вышло из интервала, пишу что осталось
			n0 = 0;
			if ( n1 > parTot->pBufDate->GetSize() )
				n1 = parTot->pBufDate->GetSize();
			flag = false;
		}
		else if ( n0 <= parTot->pBufDate->GetSizePage() )
		{// начало события выйдет за интервал при следующей порции,
		// пишу что есть
			if ( n1 > parTot->pBufDate->GetSize() )
				n1 = parTot->pBufDate->GetSize();
			flag = false;
		}
		else
		{// следующая порция возможна по Т0
			if ( n1 < parTot->pBufDate->GetSize() )
				flag = false;
		}

		if ( !flag )
		{
			n = n1 - n0;
			if ( numKadr < n )
				numKadr = n;
		//	parPri->pWriter->SetNumberSignals();
			parPri->pWriter->SetNumberKadrs(numKadr);
			for(i=0; i<parPri->pWriter->GetNumberSignals(); i++)
				parPri->pWriter->CopyData(i, parPri->ppData[i] + n0);
			parPri->pWriter->SetFirstTimeDATE( parTot->pBufDate->GetGlobalAddres()[n0] );
		}
		Sleep(sleepTime);
	}
	while( flag && par0->FlagJob && !par0->FlagStop );

	// Если из цикла do-while вышли по FlagJob/FlagStop, то файл не нужен
	if ( par0->FlagJob && !par0->FlagStop )
	{
		if ( parPri->pWriter->Write() && par0->hWndParent )
		{
			::PostMessageW( par0->hWndParent, WM_WRITE_DTU, (WPARAM)par0->ztId,
					(LPARAM)parPri->pWriter->m_NameFileDTU.GetBuffer() );
			parPri->pWriter->m_NameFileDTU.ReleaseBuffer();
		}// если файл записан
		else
		{	// файл не записан --------------------------------------------
			if ( par0->hWndParent )
				::PostMessageW( par0->hWndParent, WM_ERROR_THREAD_DTU, 1, 0 );
		}
	}	// if ( par0->FlagJob && !par0->FlagStop )	
	// завершение работы потока
	par0->FlagViewStop = true;
	par0->FlagJob = false;
	return 0;
}
//------------------------------------------------------------------------------
CThreadDTU::CThreadDTU() : CCustomZetThread()
{
	zParam0.pParamNext = &zParam1;
}
//------------------------------------------------------------------------------
CThreadDTU::~CThreadDTU(void)
{// Необходимо вначале синхронно остановить поток, а затем удалить массивы,
	// иначе возможна ситуация, когда поток обратится к уже удалённым объектам
	SynchroFinishThread();
}
//------------------------------------------------------------------------------
bool CThreadDTU::Create( CString nameThread, HWND hwnd, ParamTotal *parTot )
{	// т.к. NULL, то CCustomZetThread::Create возвращает всегда false, поэтому так:
	CCustomZetThread::Create(nameThread, hwnd, NULL);
	zParam1.pTotal = parTot;
	zParam1.pPrivat = &m_ParamPrivat;
	m_ParamPrivat.pWriter = &m_WriterDTU;
	return CreateZetThread(&WriteDtuThread);
}
//------------------------------------------------------------------------------