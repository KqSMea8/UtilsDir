//------------------------------------------------------------------------------
//	Файл CustomThreadSRV.h
//	Версия от 28.08.2012
//	Зеленоград, ЗАО "ЭТМС", Савостин Ю.М.
//------------------------------------------------------------------------------
//		Базовый класс для классов, создающих потоки для чтения данных с
// сервера и их обработки. Наследник класса CCustomZetThread.
//		Класс не создает потоков, а является хранилищем общих параметров и
// функций, имеющих отношение к серверу данных.
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include "ATLComTime.h"
#include <Constants.h>
#include <ZetThread\CustomZetThread.h>
#include <ZETView_basic_class\CDSRV.h>
#include <chaninfo.h>
//------------------------------------------------------------------------------
struct ZetThreadParamSRV0
{// Параметры сервера. Для удобства их передачи в поток объединены в структуру.
	CDSRV *pSRV;			// указатель на экземпляр класса сервера
	int NumberChannels;		// кол-во используемых каналов
	HANDLE hEventSynchro;	// синхронизирующее событие отображения результатов
	HANDLE hEventModify;	// синхронизирующее событие для Modify SRV
	PVOID pParamNext;		// адрес структуры параметров дочернего класса
	double Time;			// локальное время сервера (от 0)
	double TimeInterval;	// интервал инкрементации времени сервера
};
//------------------------------------------------------------------------------
class CCustomThreadSRV : public CCustomZetThread
{// виртуальные функции в классах-наследниках будут переопределяться
private:

protected:
//	unsigned int FNumberChannelsMax;
	ZetThreadParamSRV0 parSRV0;
	double SetCurTime(long chan, long fast);
	bool TestSynchro();

public:
	CCustomThreadSRV();
	virtual bool Create( CString nameThread, HWND hwnd, CDSRV *psrv,
		int numberChannels,
		FunctionThread function, PVOID paramFunction = NULL );
	virtual ~CCustomThreadSRV(void);

	// методы свойств
	int GetNumberChannels() { return parSRV0.NumberChannels; }
	int GetNumberChannelsMax() { return MAXCHAN; }
	
	CDSRV* GetSRV() { return parSRV0.pSRV; }
	void SetSRV(CDSRV *val) { parSRV0.pSRV = val; }

	double GetTime() { return parSRV0.Time; }
	virtual void SetTime(double val) { parSRV0.Time = val; }

	double GetTimeInterval() { return parSRV0.TimeInterval; }
	virtual void SetTimeInterval(double val) { parSRV0.TimeInterval = val; }

	DATE GetTimeInDATE() { return
		parSRV0.pSRV->GetTimeSRVasDATE(parSRV0.Time); }

	DATE GetStartTimeInDATE() { return
		parSRV0.pSRV->GetTimeSRVasDATE(0); }

	// методы класса
	virtual float* GetDataPointer(int index) {return NULL;}
	void StartSRV(long chan, long fast);
	virtual void SetNewTime(long chan, long fast) {
		parSRV0.Time = parSRV0.TimeInterval + SetCurTime(chan, fast); }
	virtual void ZeroTime() { parSRV0.Time = 0; }
	
	// функция, разрешающая потоку дальнейшие вычисления
	// до вызова необходимо скопировать данные для отображения
	// вызывать функцию нужно только один раз
	void SetSynchro() {
		if (parSRV0.hEventSynchro) SetEvent(parSRV0.hEventSynchro); }

	void StopThreadForModifySRV() {
		if (parSRV0.hEventSynchro) ResetEvent(parSRV0.hEventSynchro); }
	void GoOnThreadAfterModifySRV() {
		if (parSRV0.hEventSynchro) SetEvent(parSRV0.hEventSynchro); }
};
//------------------------------------------------------------------------------