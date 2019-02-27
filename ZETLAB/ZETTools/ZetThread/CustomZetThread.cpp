//------------------------------------------------------------------------------
//	Файл CustomZetThread.cpp
//	Версия от 21.08.2012
//	Зеленоград, ЗАО "ЭТМС", Савостин Ю.М.
//------------------------------------------------------------------------------
#include "stdafx.h"
#include <ZetThread\CustomZetThread.h>
//------------------------------------------------------------------------------
CCustomZetThread::CCustomZetThread()	// Конструктор класса
{
	zParam0.TimeOut = 3000;
	zParam0.TimeOutSend = 500;
	zParam0.ztId = 0;
	zParam0.NameThread.Empty();
	zParam0.FlagJob = true;
	zParam0.FlagStop = false;
	zParam0.FlagViewStop = true;
	zParam0.statusEx = notStart;
	zParam0.hThread = NULL;
	zParam0.hWndParent = 0;
	zParam0.TimeSleep = 20;
	zParam0.Function = NULL;
	zParam0.pParamFunction = NULL;
	zParam0.pParamNext = NULL;

}
//------------------------------------------------------------------------------
bool CCustomZetThread::Create(
	CString nameThread,			// имя потока
	HWND hwnd,					// hwnd родителя (ему будут идти сообщения)
	FunctionThread function,	// исполняемая в потоке вторичная глобальная функция
	PVOID paramFunction )		// указатель на её аргумент
{	// function может быть NULL 
	zParam0.NameThread = nameThread;
	zParam0.hWndParent = hwnd;
	zParam0.Function = function;
	zParam0.pParamFunction = paramFunction;
	return zParam0.Function != NULL;
}
//------------------------------------------------------------------------------
CCustomZetThread::~CCustomZetThread()	// Деструктор класса
{// Поток ДОЛЖЕН завершаться в деструкторах классов-наследников!!!
// Иначе возможна ситуация, когда поступает команда удалить наследника
// при выполняющимся потоке. В этом случае последовательность операций
// может быть следующей. В потоке интерфейса вызывается команда синхронно
// завершить работу, т.е. сбрасывается флаг zParam0.FlagJob и поток
// интерфейса переходит в ожидание. При этом вызывается деструктор
// наследника, где уничтожается некий объект. Поток в это время выполняется
// (до проверки флага FlagJob ещё не дошло), и происходит обращение к
// удалённому объекту! Поток в ауте!
// Поэтому: в деструкторе наледника вначале НЕОБХОДИМО вызывать функцию
// данного класса SynchroFinishThread.
	if (zParam0.hThread)
		CloseHandle(zParam0.hThread);
}
//------------------------------------------------------------------------------
bool CCustomZetThread::CreateZetThread(
	_Thread function)	// первичная глобальная функция потока
{//	Создание приостановленного потока
	bool ret;
	zParam0.hThread = (HANDLE)_beginthreadex(NULL, 0, (PTHREAD_START)function,
									&zParam0, CREATE_SUSPENDED, &zParam0.ztId);
	if (zParam0.hThread)
		ret = true;
	else
		ret = false;
	return ret;
}
//------------------------------------------------------------------------------
void CCustomZetThread::SynchroFinishThread()
{	// синхронное завершение работы потока
	if ( EndAndWait() < 0 )
	{// функция, выполняемая в потоке зависла или были ошибки
		#pragma warning(suppress: 6258)
		TerminateThread(zParam0.hThread, -1);			// убиваю поток
		WaitForSingleObject(zParam0.hThread, zParam0.TimeOut);	// обязательно
	}													// надо подождать
}
//------------------------------------------------------------------------------
long CCustomZetThread::_MyLastError()
{
	long ret = GetLastError();
	if (ret > 0)
		ret = -ret;
	return ret;
}
//------------------------------------------------------------------------------
// Статус потока:
//	0 - выполняется;
//	1 - остановлен;
//	2 - завершен;
// <0 - код ошибки - проблемы с потоком; 
long CCustomZetThread::GetStatus()
{
	long ret;
	switch ( WaitForSingleObject(zParam0.hThread, 0) )
	{
	case WAIT_TIMEOUT:	// поток работает
		ret = (long)zParam0.FlagViewStop;
		break;

	case WAIT_OBJECT_0:	// поток уже завершил работу
		ret = 2;
		break;

	default:			// WAIT_FAILED ошибка при работе потока
		ret = _MyLastError();		// или его дескриптора
	}
	return ret;
}
//------------------------------------------------------------------------------
long CCustomZetThread::_End(
	bool flagWait)	// тип команды: асинхронный - false, синхронный - true
{//  Приватный метод завершения работы потока (на все случаи)
//		Коды возврата:
//  1 - поток уже завершился;
//  0 - поток завершён (или команда передана)
// -1 - поток не завершился в течении FTimeOut мсек (завис?)
// <0 - код ошибки - проблемы с событием или с потоком;
//----------------------------------
	long ret;
	// проверяю состояние потока
	switch ( WaitForSingleObject(zParam0.hThread, 0) )
	{
	case WAIT_TIMEOUT:	// поток не завершён
		zParam0.FlagJob = false;	// команда завершить поток
		// проверяю стоит поток или нет (по событию)
		if (zParam0.FlagViewStop)
		// поток стоит, его надо запустить чтобы он сам завершился
			while ( ResumeThread(zParam0.hThread) > 1 );
		// если поток работает, то делать ничего не надо
		ret = 0;
		// если команда синхронная
		if (flagWait)
		{	// жду остановки
			switch ( WaitForSingleObject(zParam0.hThread, zParam0.TimeOut) )
			{
			case WAIT_OBJECT_0: //	поток завершился
				//ret = 0;		// поток остановлен, команда выполнена
				break;
			case WAIT_TIMEOUT:	// поток не завершился в течении FTimeOut сек
				ret = -1;		// скорее всего завис
				break;
			default:			// WAIT_FAILED ошибка потока
				ret = _MyLastError();	// или его дескриптора
			}
		}
		break;	// case WAIT_TIMEOUT:	// поток не завершён

	case WAIT_OBJECT_0:	// работа потока уже завершена
		ret = 1;
		break;

	default:			// WAIT_FAILED ошибка при работе потока
		ret = _MyLastError();	// или его дескриптора
	}
	return ret;
}
//------------------------------------------------------------------------------
long CCustomZetThread::_Stop(
	bool flagWait)	// тип команды: асинхронный - false, синхронный - true
{//		Приватный метод остановки работы потока (на все случаи)
//		Коды возврата:
//  1 - поток стоит
//	0 - поток остановлен (или передана команда становиться)
// -1 - поток уже завершил свою работу
// -2 - поток не остановился в течении FTimeOut мсек
// <0 - код ошибки - проблемы с событием или с потоком;
//----------------------------------
	long ret;
	// проверяю состояние потока
	switch ( WaitForSingleObject(zParam0.hThread, 0) )
	{
	case WAIT_TIMEOUT:	// поток работает
		if (!zParam0.FlagViewStop)
		{// поток не стоит
			zParam0.FlagStop = true;	// команда остановить поток
			ret = 0;
			if (flagWait)	// если команда синхронная жду остановки
			{
				for(DWORD n=0; n<zParam0.TimeOut/20; ++n)
				{// Жду когда поток остановится, проверка через каждые 20 мсек
					Sleep(20);
					if (zParam0.FlagViewStop)
						break;
				}
				if (!zParam0.FlagViewStop)
					ret = -2;
			}
		}
		else
			ret = 1;
		break;
	case WAIT_OBJECT_0:	// работа потока завершена
		ret = -1;
		break;
	default:			// WAIT_FAILED ошибка потока
		ret = _MyLastError();	// или его дескриптора
	}
	return ret;
}
//------------------------------------------------------------------------------
// Асинхронная (без ожидания выполнения) команда потоку "Конец"
//--------------------
long CCustomZetThread::End()
{
	return _End(false);
}
//------------------------------------------------------------------------------
// Синхронная (с ожиданием выполнения) команда потоку "Конец"
//---------------------------
long CCustomZetThread::EndAndWait()
{
	return _End(true);
}
//------------------------------------------------------------------------------
long CCustomZetThread::Start()
{//	Запуск потока (если он стоит)
//		Коды возврата:
//	0 - поток запущен
// -1 - поток уже завершил свою работу
// <0 - код ошибки - проблемы с потоком;
	long ret;
	switch( WaitForSingleObject(zParam0.hThread, 0) )
	{
	case WAIT_OBJECT_0:	// работа потока завершена
		ret = -1;
		break;
	case WAIT_TIMEOUT:	// поток не завершён, запускаю его
		zParam0.FlagStop = false;
		if (zParam0.FlagViewStop)
		{//запуск c учётом счётчика остановок
			while (true)
			{
				if (ResumeThread(zParam0.hThread) <= 1)
					break;
			}
		}
		ret = 0;
		break;
	default:			//  WAIT_FAILED ошибка потока
		ret = _MyLastError();	// или его дискриптора
	}
	return ret;
}
//------------------------------------------------------------------------------
long CCustomZetThread::Stop()			// асинхронная команда потоку "Стоп"
{
	return _Stop(false);
}
//------------------------------------------------------------------------------
long CCustomZetThread::StopAndWait()	// синхронная команда потоку "Стоп"
{
	return _Stop(true);
}
//------------------------------------------------------------------------------