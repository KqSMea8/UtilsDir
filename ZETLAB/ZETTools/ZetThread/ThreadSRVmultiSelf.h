//------------------------------------------------------------------------------
//	Файл ThreadSRVmultiSelf.h
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
#include <limits>
#include <Intel\ipp\ipps.h>
#include <CodeQuality.h>
#include <Dialog_ZET\DataTime.h>
#include <ZetThread\CustomThreadSRV.h>
//------------------------------------------------------------------------------
#define WhileCountMax		100
#define MaxDeltaTimeInSec	60.
//------------------------------------------------------------------------------
struct MultiChannelSelf
{// Структура параметров каналов
	bool bEnabled;	// канал задан оператором, т.е. у канала еcть ID
	bool bJob;		// канал требует чтения и обработки данных
	bool bExist;	// канал существует в системе и не отключен
	bool b7000;		// флаг, что канал устройства серии 7000 (низкая частота АЦП)
	bool bAutoNumberCounts;	// кол-во кадров определяется m_dt в DataTime
	long Channel;	// номер канала
	long ChannelId;	// идентификатор канала
	DWORD NumberCounts;// кол-во считываемых отсчетов
	DWORD NumberBufferSRV;	// размер буфера канала данной декады
	long Decade;	// декада
	long RetGetData;// код качества считанных данных
	DWORD DataSize;	// размер массива pData
	float *pData;	// указатель для считанных данных
	float freqADC;	// частота дискретизации канала
	CDataTime DataTime;	// время чтения данных

	MultiChannelSelf()
		: Channel(-1)
		, ChannelId(-1)
		, bEnabled(false)
		, bJob(false)
		, bExist(false)
		, b7000(false)
		, bAutoNumberCounts(true)
		, NumberCounts(0)
		, NumberBufferSRV(0)
		, Decade(0)
		, RetGetData(0)
		, DataSize(0)
		, freqADC(0)
		, pData(NULL)
	//	, DataTime сработает конструктор класса
	{}
	MultiChannelSelf(const MultiChannelSelf &val)
	{
		Channel = val.Channel;
		ChannelId = val.ChannelId;
		bEnabled = val.bEnabled;
		bJob = val.bJob;
		bExist = val.bExist;
		b7000 = val.b7000;
		bAutoNumberCounts = val.bAutoNumberCounts;
		NumberCounts = val.NumberCounts;
		NumberBufferSRV = val.NumberBufferSRV;
		Decade = val.Decade;
		RetGetData = val.RetGetData;
		freqADC = val.freqADC;
		DataTime = val.DataTime;
		if (val.DataSize > 0)
		{
			DataSize = val.DataSize;
			pData = new float [DataSize];
			ippsCopy_32f(val.pData, pData, DataSize);
		}
		else
		{
			DataSize = 0;
			pData = nullptr;
		}
	}
	~MultiChannelSelf()
	{
		if (DataSize > 0)
		{
			if (nullptr != pData)
			{
				delete[] pData;
				pData = nullptr;
			}
			DataSize = 0;
		}
	}

	void operator = (const MultiChannelSelf &val)
	{
		bEnabled = val.bEnabled;
		bJob = val.bJob;
		bExist = val.bExist;
		b7000 = val.b7000;
		bAutoNumberCounts = val.bAutoNumberCounts;
		Channel = val.Channel;
		ChannelId = val.ChannelId;
		NumberCounts = val.NumberCounts;
		NumberBufferSRV = val.NumberBufferSRV;
		Decade = val.Decade;
		RetGetData = val.RetGetData;
		freqADC = val.freqADC;
		DataTime = val.DataTime;

		if (val.DataSize > 0)
		{
			ReSize(val.DataSize);
			ippsCopy_32f(val.pData, pData, DataSize);
		}
	}

	bool ReSize(DWORD size)
	{
		bool ret = DataSize >= size;
		if (!ret)
		{
			if (DataSize > 0)
			{
				delete[] pData;
				pData = nullptr;
			}
			try
			{
				pData = new float [size];
				DataSize = size;
				ret = true;
			}
			catch (CMemoryException*)
			{
				pData = nullptr;
				DataSize = 0;
			//	ret = false;	уже
			}
		}
		return ret;
	}

	bool SetNumberCounts(DWORD val = 0)
	{// 0 - значит автоматический режим в соответствии с Data.m_dt,
	//	иначе ручной режим и задаю.
		// Эту функцию есть смысл вызывать а автоматическом режиме только после
		// задания канала (т.е. после задания частоты дискретизации и декады) и
		// после задания параметров элементу DataTime.
		DWORD num(0);
		bAutoNumberCounts = val == 0;
		bool ret(true);
		if (bAutoNumberCounts)
		{
			double x = DataTime.GetTimeInterval() * freqADC;
			if (b7000)
			{// маленькая частота дискретизации, лучше взять ещё отсчёт,
				// чем рано или позно потерять отсчёт
				num = (DWORD)(int)floor(x);
				if (x > (double)num)
					++num;
			}
			else
				num = (DWORD)(int)floor(x + 0.5);
		}
		else
			num = val;

		// проверка на превышение размера буфера
		if (NumberBufferSRV > 0)
		{
			if (num > NumberBufferSRV)
				num = NumberBufferSRV;
		}
		// проверка размера массива
		if (num > 0)
			ret = ReSize(num);

		if (ret)
			NumberCounts = num;
		else
			NumberCounts = 0;
		return ret;
	}
};
//------------------------------------------------------------------------------
struct SendToSecGlobFunction
{// Структура параметров, передаваемых во вторичную глобальную функцию
	MultiChannelSelf *pMChan;
	int index;
	bool isCurrentChannel;
	PVOID pParamNext;
};
//------------------------------------------------------------------------------
struct ZetThreadParamSRVmultiSelf
{// Параметры класса
	bool Support7000;		// поддрержка устройств серии 7000
	long* pFiIndexCur;
	std::vector<MultiChannelSelf> *pChannels;
};
//------------------------------------------------------------------------------
class CThreadSRVmultiSelf : public CCustomThreadSRV
{
private:
	long FIndexCur;
	// минимальная частота дискретизации нормального канала
	float FFreqADCMin;
	// мин частота дискретизации для возможности работы при усреднении в 0,1 сек
	const float m_constFreqADCminFor_0_1;
	// мин частота дискретизации для возможности работы при усреднении в 1 сек
	const float m_constFreqADCminFor_1_0;
	ZetThreadParamSRVmultiSelf par1;
	std::vector<MultiChannelSelf> m_Channels;
//	bool CalcNumKadrs(int index, bool testIndex);

protected:

public:
	CThreadSRVmultiSelf();
	virtual bool Create( CString nameThread, HWND hwnd, CDSRV *psrv,
		int numberChannels,
		FunctionThread function, PVOID paramFunction = NULL );
	virtual ~CThreadSRVmultiSelf(void);


	// методы свойств
	std::vector<MultiChannelSelf>::iterator GetIteratorBegin() {
		return m_Channels.begin(); }
	std::vector<MultiChannelSelf>::iterator GetIteratorEnd() {
		return m_Channels.end(); }	
	MultiChannelSelf* GetPointerMultiChannelCur() {
		return &m_Channels[FIndexCur]; }

	float GetMinFreqADCforAverage_0_1sec() { return m_constFreqADCminFor_0_1; }
	float GetMinFreqADCforAverage_1sec() { return m_constFreqADCminFor_1_0; }

	bool GetSupport7000() {
		return par1.Support7000; }
	void SetSupport7000(bool val) {
		par1.Support7000 = val; }

	long GetChannelDecade(int index);
	void SetChannelDecade(int index, long val);

	bool GetChannel7000() {
		return m_Channels[FIndexCur].b7000; }

	virtual void SetTime(double val);

	long GetChannelNumber() {
		return m_Channels[FIndexCur].Channel; }
	bool SetChannelNumber(long num);

	int GetChannelNumberCounts() {
		return m_Channels[FIndexCur].NumberCounts; }
	bool SetChannelNumberCounts(DWORD val);
	bool SetChannelNumberCountsAuto();

	long GetIndexCur() { return FIndexCur; }
	bool SetIndexCur(int index, bool test = true);

	void SetMinFreqADC(float freq) { if (freq > 0.f) FFreqADCMin = freq; };
	float GetMinFreqADC() { return FFreqADCMin; };
	
	// члены класса

	// методы класса
	long AddChannel(long chan);
	virtual float* GetDataPointer(int index);
	bool Modify();
	bool StartChannel(int index);
	bool StopChannel(int index);
	bool TestIndex(int index) {
		return ( index >= 0 ) && ( index < int(m_Channels.size()) ); }
};
//------------------------------------------------------------------------------