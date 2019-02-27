//------------------------------------------------------------------------------
//	Файл CustomZetThread.h
//	Версия от 21.08.2012
//	Зеленоград, ЗАО "ЭТМС", Савостин Ю.М.
//------------------------------------------------------------------------------
//		Базовый класс всех классов, создающих потоки. 
//   Класс не создает потоков, а является хранилищем общих параметров и функций
// для классов-наследников (остановить, запустить, определить статус потока).
//	Коды возврата функций класса:
//	 0 - поданная команда выполнена
//	 1 - поданная команда была выполнена до подачи команды
//	-1 - поток завершил свою работу до подачи команды
//	-2 - время ожидания выполнения поданной команды превысило установленное
//		 значение TimeOut, которое по умолчанию равно 3 сек.
//	-3 - не задана функция для выполнения в потоке
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include "TypeStatusThread.h"
#include <process.h>
//------------------------------------------------------------------------------
typedef DWORD (__stdcall *_Thread) (void *);
typedef unsigned int (__stdcall *PTHREAD_START) (void *);
typedef long (*FunctionThread)(void *);
//------------------------------------------------------------------------------
#include <DefineWM.h>
//------------------------------------------------------------------------------
//   Структура, содержащая все параметры потока. Указатель на неё - это
// параметр первичной глобальной функции.
//   Структура содержит общие параметры и указатель на структуру частных
// параметров, которые определяются в классах-наследниках.
struct ZetThreadParam0
{
	bool FlagJob;				// флаг необходимости работы потока
	bool FlagStop;				// флаг необходимости остановки потока
	bool FlagViewStop;			// флаг состояния потока (1-стоит или 0-нет)
	TypeStatusThread statusEx;		// расширенный статус потока
	HANDLE hThread;				// дескриптор потока
	HWND hWndParent;			// дескриптор диалогового окна
	CString NameThread;			// имя потока
	unsigned int ztId;			// идентификатор потока
	DWORD TimeOut;				// максимальное время выполнения функции в потоке
	DWORD TimeOutSend;			// максимальное время ожидания выполнения SendMessage
	DWORD TimeSleep;			// время ожидания потока
	FunctionThread Function;	// адрес вторичной глобальной функции
	PVOID pParamFunction;		// адрес параметров вторичной глобальной функции
	PVOID pParamNext;			// адрес структуры частных параметров потока
};
//------------------------------------------------------------------------------
class CCustomZetThread
{
private:
	long _MyLastError();
	long _End(bool flagWait);
	long _Stop(bool flagWait);

protected:
	ZetThreadParam0 zParam0;	// параметры первичной глобальной функции
	bool CreateZetThread(_Thread function);	// создание приостановленного потока
	void SynchroFinishThread();	// синхронное завершение работы потока

public:
	CCustomZetThread();
	virtual ~CCustomZetThread();
	virtual bool Create( CString nameThread, HWND hwnd,
						FunctionThread function, PVOID paramFunction = NULL );

	// методы свойств
	unsigned int GetId() { return zParam0.ztId; }
	CString GetNameThread()	{ return zParam0.NameThread; }
	HWND GetHWNDParent() { return zParam0.hWndParent; }
	TypeStatusThread GetStatusThread() { return zParam0.statusEx; }

	void SetFunction(FunctionThread function) { zParam0.Function = function; }

	long GetStatus();
	bool IsError() { return GetStatus() < 0; }
	bool IsRunning() { return GetStatus() == 0; }
	bool IsStopped() { return GetStatus() == 1; }
	bool IsCompleted() { return GetStatus() == 2; }
	
	DWORD GetTimeOut() { return zParam0.TimeOut; }
	void SetTimeOut(DWORD val) { zParam0.TimeOut = val; }
	
	DWORD GetTimeOutSend() { return zParam0.TimeOutSend; }
	void SetTimeOutSend(DWORD val) { zParam0.TimeOutSend = val; }
	
	DWORD GetTimeSleep() { return zParam0.TimeSleep; }
	void SetTimeSleep(DWORD val) { zParam0.TimeSleep = val; }	

	// методы класса ----------------------------
	virtual long End();				// асинхронная команда потоку "Конец"
	virtual long EndAndWait();		// синхронная команда потоку "Конец"
	long Start();			// запуск потока
	virtual long Stop();			// асинхронная команда потоку "Стоп"
	virtual long StopAndWait();		// синхронная команда потоку "Стоп"
};
//------------------------------------------------------------------------------