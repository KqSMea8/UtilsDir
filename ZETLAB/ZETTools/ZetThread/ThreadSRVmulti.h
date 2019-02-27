//------------------------------------------------------------------------------
//	Файл ThreadSRVmulti.h
//	Версия от 08.11.2012
//	Зеленоград, ЗАО "ЭТМС", Савостин Ю.М.
//------------------------------------------------------------------------------
//		Класс предназначен для считывания данных и их обработки с независимых
// каналов сервера. Т.е. каналов, возможно от разных устройств,
// с разными частотами дискретизации, разным кол-вом кадров и т.д.
//		Считается, что данные каналов не требуют совместной обработки!!!
//		Интервалы считывания данных всех каналов должны быть кратны некоторой
// величине, которая и задаётся интервалом времени считывания данных,
// например: 0,1 сек.
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <vector>
#include <Intel\ipp\ipps.h>
#include <ZetThread\CustomThreadSRV.h>
//------------------------------------------------------------------------------
struct MultiChannel
{// Структура параметров каналов
	long Channel;	// номер канала
	long ChannelId;	// идентификатор канала
	bool bEnabled;	// канал задан оператором, т.е. у канала еcть ID
	bool bJob;		// канал требует чтения и обработки данных
	bool bExist;	// канал существует в системе и не отключен
	bool b7000;		// флаг, что канал устройства серии 7000 (низкая частота АЦП)
	bool bRead7000;	// флаг, что можно читать данные (только для серии 7000)
	int NumberKadrs;// кол-во считываемых кадров
	long Decade;	// декада
	long qualityCode;// код качества считанных данных
	float *pData;	// указатель для считанных данных
	double TimeData;// время чтения данных
	MultiChannel()
	{
		Channel =
		ChannelId = -1;
		bEnabled =
		bJob =
		bExist =
		b7000 =
		bRead7000 = false;
		NumberKadrs = 0;
		Decade = 0;
		qualityCode = 0;
		pData = NULL;
		TimeData = 0.;
	}
};
//------------------------------------------------------------------------------
struct SendToSecGlobFunction
{// Структура параметров, передаваемых во вторичную глобальную функцию
	MultiChannel *pMChan;
	int index;
	PVOID pParamNext;
};
//------------------------------------------------------------------------------
struct ZetThreadParamSRVmulti
{// Параметры класса
	bool Support7000;		// поддрержка устройств серии 7000
	double Time7000;		// локальное время сервера (от 0)
	double TimeInterval7000;// интервал инкрементации времени сервера
	std::vector<MultiChannel> *pChannels;
};
//------------------------------------------------------------------------------
class CThreadSRVmulti : public CCustomThreadSRV
{
private:
	int FIndexCur;
	float FFreqADCMin;	// минимальная частота дискретизации нормального канала
	std::vector<MultiChannel> m_Channels;
	ZetThreadParamSRVmulti par1;
	bool CalcNumKadrs(int index, int *pnum, bool testIndex = true);
	double SetCurTime7000(long chan, long fast);

protected:

public:
	CThreadSRVmulti();
	virtual bool Create( CString nameThread, HWND hwnd, CDSRV *psrv,
		int numberChannels,
		FunctionThread function, PVOID paramFunction = NULL );
	virtual ~CThreadSRVmulti(void);


	// методы свойств
	std::vector<MultiChannel>::iterator GetIteratorBegin() {
		return m_Channels.begin(); }
	std::vector<MultiChannel>::iterator GetIteratorEnd() {
		return m_Channels.end(); }	
	MultiChannel* GetPointerMultiChannelCur() {
		return &m_Channels[FIndexCur]; }

	bool GetSupport7000() {
		return par1.Support7000; }
	void SetSupport7000(bool val) {
		par1.Support7000 = val; }

	bool GetChannel7000() {
		return m_Channels[FIndexCur].b7000; }

	virtual void SetTime(double val);

	long GetChannelNumber() {
		return m_Channels[FIndexCur].Channel; }
	bool SetChannelNumber(long num);

	int GetChannelNumberKadrs() {
		return m_Channels[FIndexCur].NumberKadrs; }

	int GetIndexCur() {
		return FIndexCur; }
	bool SetIndexCur(int index, bool test = true);
	
	double GetTimeInterval7000() { return par1.TimeInterval7000; }
	virtual void SetTimeInterval(double val);
	void SetTimeInterval7000(double val);

	// члены класса

	// методы класса
	long AddChannel(long chan);
	virtual float* GetDataPointer(int index);
	void Modify();
	bool StartChannel(int index);
	bool StopChannel(int index);
	bool TestIndex(int index) {
		return ( index >= 0 ) && ( index < int(m_Channels.size()) ); }

	virtual void SetNewTime(long chan, long fast);
	virtual void ZeroTime();
};
//------------------------------------------------------------------------------