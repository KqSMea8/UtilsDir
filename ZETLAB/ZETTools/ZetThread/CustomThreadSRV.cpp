//------------------------------------------------------------------------------
//	Файл CustomThreadSRV.cpp
//	Версия от 28.08.2012
//	Зеленоград, ЗАО "ЭТМС", Савостин Ю.М.
//------------------------------------------------------------------------------
#include "StdAfx.h"
#include <ZetThread\CustomThreadSRV.h>
//------------------------------------------------------------------------------
CCustomThreadSRV::CCustomThreadSRV() : CCustomZetThread()
{	// параметры по умолчанию
//	FNumberChannelsMax = 271;	// 128 - было, 271 - простое число
	parSRV0.pSRV = NULL;
	parSRV0.NumberChannels = 0;
	parSRV0.Time = 0.;
	parSRV0.TimeInterval = 0.1;
	parSRV0.hEventSynchro = NULL;
	parSRV0.hEventModify = NULL;
}
//------------------------------------------------------------------------------
bool CCustomThreadSRV::Create(
	CString nameThread,			// имя потока
	HWND hwnd,					// HWND родительского окна
	CDSRV *psrv,				// указатель на сервер
	int numberChannels,			// кол-во каналов
	FunctionThread function,	// адрес вторичной функции
	PVOID paramFunction )		// параметры вторичной функции
{
	bool ret = psrv &&	// адрес сервера должен быть
		CCustomZetThread::Create(nameThread, hwnd, function, paramFunction);
	if (ret)
	{
		zParam0.pParamNext = &parSRV0;
		parSRV0.pSRV = psrv;
		parSRV0.NumberChannels = numberChannels;
		// создаю событие с автосбросом и с сигнальным (свободным) состоянием
		parSRV0.hEventSynchro = CreateEventW(NULL, FALSE, TRUE, NULL);
		// создаю событие с ручным сбросом и с сигнальным (свободным) состоянием
		parSRV0.hEventModify = CreateEventW(NULL, TRUE, TRUE, NULL);
		ret =(parSRV0.hEventSynchro != NULL) && (parSRV0.hEventModify != NULL);
	}
	return  ret;
}
//------------------------------------------------------------------------------
CCustomThreadSRV::~CCustomThreadSRV()
{
	CloseHandle(parSRV0.hEventSynchro);
	parSRV0.hEventSynchro = NULL;		// обязательно
	CloseHandle(parSRV0.hEventModify);
	parSRV0.hEventModify = NULL;
}
//------------------------------------------------------------------------------
bool CCustomThreadSRV::TestSynchro()
{
	bool ret;
	if (parSRV0.hEventSynchro)
	{
		if ( WaitForSingleObject(parSRV0.hEventSynchro,
								zParam0.TimeOutSend) == WAIT_OBJECT_0 )
			ret = true;
		else// if ( kod == WAIT_TIMEOUT )
			ret = false;
	}
	else
		ret = false;
	return ret;
}
//------------------------------------------------------------------------------
double CCustomThreadSRV::SetCurTime(long chan, long fast)
{// задание текущего времени сервера, кратного 0,1 сек или 1 сек или 10 сек
	long ch(0);
	double dt(0.);
	switch(fast)
	{
	case 0: dt = 10.; break;
	case 1: dt = 1.; break;
	case 2: dt = 0.1; break;
	default : dt = 10.;
	}
	if ( (chan >= 0) && (chan < parSRV0.pSRV->QuanChan() ) )
		ch = chan;
	else
		ch = 0;
	parSRV0.Time =
			int( parSRV0.pSRV->CurrentTime(chan) * dt + 0.5 ) / dt;
	return parSRV0.Time;
}
//------------------------------------------------------------------------------
void CCustomThreadSRV::StartSRV(long chan, long fast)
{// запуск потока с заданием времени первого считывания, кратного fast
	SetNewTime(chan, fast);
	Start();
}
//------------------------------------------------------------------------------