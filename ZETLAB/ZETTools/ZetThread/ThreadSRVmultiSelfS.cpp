//------------------------------------------------------------------------------
//	Файл ThreadSRVmultiSelf.cpp
//	Зеленоград, ЗАО "ЭТМС", Савостин Ю.М.
//------------------------------------------------------------------------------
//   Глобальная функция SRV_ThreadMulti - это первичная функция потока.
// Её аргумент - это указатель на экземпляр структуры общих параметров,
// которая включает в себя указатель на структуру частных параметров.
//   В функции потока вначале проверяется время и наличие работающего сервера.
// У каждого канала своё время! По истечению времени считываются данные и для
// каждого канала вызывается вторичная глобальная функция, где должна выполняться
// обработка считанных данных канала. Эта функция возвращает код качества
// результата обработанки данных по каналу. Если бит AllData этого кода равен 0
// (все данные обработаны), то ожидается завершение отображения данных в потоке
// диалогово окна, и после этого генерится сообщение WM_NEW_DATA_SRV.
//------------------------------------------------------------------------------
//		При выполнении кода рабочего потока возможны следующие сообщения:
//	WM_END_JOB_SRV - нет сервера данных, сообщение без параметров;
//	WM_NEW_DATA_SRV - данные по одному из каналов считаны и обработаны,
//		передаётся номер канала и код качества результаов обработки.
//------------------------------------------------------------------------------
//
#include "StdAfx.h"
#include <ZetThread\ThreadSRVmultiSelfS.h>
//------------------------------------------------------------------------------
DWORD WINAPI SRV_ThreadMultiSelfS(PVOID pParam)
{// первичная глобальная функция рабочего потока
	int indexChan(0);
	CodeQuality retFunction(0);
	bool bIsSyncGUI(false);

	// Полученные параметры -----------------------------------------
	// адрес структуры параметров класса-прародителя CCustomZetThread
	ZetThreadParam0 *par0 = (ZetThreadParam0*)pParam;
	par0->statusEx = running;
	// адрес структуры параметров класса-родителя CCustomThreadSRV
	ZetThreadParamSRV0 *parSRV0 = (ZetThreadParamSRV0*)par0->pParamNext;
	// адрес структуры параметров данного класса CThreadSRVmulti
	ZetThreadParam1 *parSRV1 = (ZetThreadParam1*)parSRV0->pParamNext;
	// вектор параметров каналов
	std::vector<CHANNEL_STRUCT_READ*> *pChannels = parSRV1->pChannels;

	// параметры, передаваемые вторичной глобальной функции ----------
	ZetThreadParam2 parSend;
	//parSend.index - будет задан перед вызовом вторичной функции
	parSend.pChannels = pChannels;
	parSend.pParamNext = par0->pParamFunction;	// параметры от вольтмера или nullptr

	// НАЧИНАЕМ -------------------------------------------------------
	while ( par0->FlagJob )
	{// цикл рабочего потока -------------------------------------------
		par0->FlagViewStop = false;
		// Проверка не было-ли Pause -----------------------------------
		par0->statusEx = waitObject;
		WaitForSingleObject(parSRV0->hEventModify, par0->TimeOutSend);
	//	auto retw = WaitForSingleObject(parSRV0->hEventModify, par0->TimeOutSend);
		par0->statusEx = working;
		//---------------------------------------------------------------
		// проверка NumFileUsed всех используемых каналов
		bool flag = true;	// в конце цикла нужен NumFileUsed
		if ((pChannels != nullptr) && !pChannels->empty())
		{
			for (auto & it : *pChannels)
			{	// определяю есть-ли рабочие каналы
				auto xz = it->channel;
				auto bf = it->bFlags;
				if (it->GetFlag(Flag_Chan_OnOff) &&
					it->GetFlag(Flag_Chan_Exist) &&
					!it->GetFlag(Flag_Change_Chan))
				{// есть рабочий канал
					flag = false;
					if (parSRV0->pSRV->NumFileUsed(it->channel) == -1)
					{// сервер выгрузился
						::PostMessageW(par0->hWndParent, WM_END_JOB_SRV, 0, 0);
						par0->FlagJob = false;
						break;	// выхожу из цикла и завершаю поток
					}
				}
			}
		}
		// рабочих каналов нет, нужен NumFileUsed
		if (flag)
			parSRV0->pSRV->NumFileUsed(0);
		//---------------------------------------------------------------
		if (par0->FlagJob && (pChannels != nullptr) && !pChannels->empty())
		{// поток ещё нужен и каналы заданы
			indexChan = 0;
			for (auto & it : *pChannels)
			{// цикл по всем каналам, но чтение только с рабочих каналов
				par0->statusEx = working;
				if (par0->FlagStop)	
					break;
				// проверка статуса и изменение флага Flag_Chan_Exist
				it->DoChangeStatus();

				if (it->GetFlag(Flag_Chan_OnOff) &&
					it->GetFlag(Flag_Chan_Exist) &&
					!it->GetFlag(Flag_Change_Chan))
				{// канал рабочий
					par0->statusEx = dataTime;
					// проверяю время на ошибку -14 ------------
					double timecur = parSRV0->pSRV->CurrentTime(it->channel);
					double dtime = timecur - it->dataTime.GetTime();
					if (fabs(dtime) > MaxDeltaTimeInSec)
					{// прыгнуло время! перевожу время
						it->dataTime.SetStartTime(timecur);
					}
					// while нужен для того, чтобы успевать за поступившими новыми данными
					BYTE whileCount(0);
					while (
						par0->FlagJob &&
						!par0->FlagStop &&
							(parSRV0->pSRV->CurrentTime(it->channel) >=
								it->dataTime.GetTime()))
					{
						if(bIsSyncGUI)
						{// поток GUI выполнил свою работу
							if (!it->GetFlag(Flag_Chan_7000) ||
								(it->GetFlag(Flag_Chan_7000) && (parSRV1->bSupport7000)))
							{
								// проверка частоты дискретизации
								if (it->GetFlag(Flag_Change_Freq))
									it->SetNumberCounts();

								par0->statusEx = dataRead;
								it->retGetData = parSRV0->pSRV->GetData(
									it->channel, it->decade, it->dataTime.GetTime(),
									it->saArray.size, it->saArray.pData);
								if (it->retGetData < 0)
									ippsSet_32f(std::numeric_limits<float>::quiet_NaN(),
										it->saArray.pData, (int)it->saArray.size);
								
								parSend.index = indexChan;
								par0->statusEx = dataProcessing;
								retFunction = (*par0->Function)(&parSend);	// что-то делаю (вычисляю, ...)
								par0->statusEx = working;
								if ( retFunction.IsAllData() )
								{// сообщаю о завершении обработки данных
									bIsSyncGUI = false;
									::PostMessageW( par0->hWndParent, WM_NEW_DATA_SRV,
														(WPARAM)indexChan, (LPARAM)retFunction );
								}
								else if (retFunction.NeedSynchro)
								{
									bIsSyncGUI = false;
								}
							}
							// инкремент времени
							it->dataTime.IncrementTime();
						}//if (bIsSyncGUI)
						else
						{// поток GUI работает, надо ждать
							par0->statusEx = waitSynshroGUI;
							bIsSyncGUI = WaitForSingleObject(parSRV0->hEventSynchro, par0->TimeOutSend) == WAIT_OBJECT_0;
						}
						if (++whileCount > WhileCountMax)
							break;	// выход из while
					}	// while (par0->FlagJob ...)
				}	// if ( it->bEnabled && ...
				++indexChan;
			}	// for(auto it=pChannels->begin(); ...
		}	// if (par0->FlagJob && (pChannels != nullptr) && !pChannels->empty())
		// завершение цикла while ----------------------------------------------
		if (!par0->FlagStop)					// останавливать поток не надо, это чаще
		{
			par0->statusEx = sleeping;
			Sleep(par0->TimeSleep);			// надо отдать процессор
		}
		else
		{// надо остановить поток
			par0->FlagViewStop = true;
			par0->statusEx = suspend;
			SuspendThread(par0->hThread);// последняя команда в цикле while
		}
	}// while ( par0->FlagJob )
	par0->statusEx = ending;
	return 0;
}
//------------------------------------------------------------------------------
CThreadSRVmultiSelfS::CThreadSRVmultiSelfS() : CCustomThreadSRV()
	, m_freqADCMin(50.f)
	, m_constFreqADCminFor_0_1(10.f)
	, m_constFreqADCminFor_1_0(1.f)
	, m_pChannels(nullptr)
{
	parSRV0.pParamNext = &parSRV1;

	parSRV1.pChannels = nullptr;
	parSRV1.bSupport7000 = true;
}
//------------------------------------------------------------------------------
bool CThreadSRVmultiSelfS::Create(
	CString nameThread,			// имя потока
	HWND hwnd,					// HWND родительского окна
	CDSRV *psrv, 				// указатель на сервер
	std::vector<CHANNEL_STRUCT_READ*> * pChannels,	// указатель на параметры каналов
	FunctionThread function,	// адрес вторичной функции
	PVOID paramFunction)		// параметры вторичной функции
{
	bool ret(pChannels != nullptr);
	if (ret)
	{
		m_pChannels = pChannels;
		parSRV1.pChannels = m_pChannels;
		bool ret =
			CCustomThreadSRV::Create(nameThread, hwnd, psrv,
				(int)pChannels->size(), function, paramFunction);
		if (ret)
			ret = CreateZetThread(&SRV_ThreadMultiSelfS);
	}
	return ret;
}
//------------------------------------------------------------------------------
CThreadSRVmultiSelfS::~CThreadSRVmultiSelfS()
{// Необходимо вначале синхронно остановить поток, а затем удалить данные,
	// иначе возможна ситуация, когда поток обратится к уже удалённым объектам
	SynchroFinishThread();

	//if ((m_pChannels != nullptr) && !m_pChannels->empty())
	//{
	//	for (auto & it : *m_pChannels)
	//	{
	//		it->sArray.Clear();
	//	}
	//}
}
//------------------------------------------------------------------------------
void CThreadSRVmultiSelfS::SetMinFreqADC(float freq)
{
	if (freq > 0.f)
		*((float*)&m_freqADCMin) = freq;
}
//------------------------------------------------------------------------------
void CThreadSRVmultiSelfS::SetTime(double val)
{
	if ((m_pChannels != nullptr) && !m_pChannels->empty())
	{
		for (auto & it : *m_pChannels)
		{
			it->dataTime.SetStartTime(parSRV0.pSRV->CurrentTime(it->channel));
		}
	}
}
//------------------------------------------------------------------------------