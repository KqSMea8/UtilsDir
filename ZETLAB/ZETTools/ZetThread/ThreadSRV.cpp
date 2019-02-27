//------------------------------------------------------------------------------
//	Файл ThreadSRV.cpp
//	Зеленоград, ЗАО "ЭТМС", Савостин Ю.М.
//------------------------------------------------------------------------------
//   Глобальная функция SRV_Thread - это первичная функция потока. Её аргумент -
// это указатель на экземпляр структуры общих параметров, которая включает в
// себя указатель на структуру частных параметров (см. функцию CreateZetThread
// класса-родителя).
//   В функции потока после считывания данных вызывается вторичная глобальная
// функция, где должна выполняться обработка считанных данных. Адрес вторичной
// глобальной функции и её аргумент - это соответственно переменные function и
// paramFunction функции Create данного класса.
//		При выполнении кода класса возможны следующие сообщения родительскому окну:
// 1. WM_NEW_DATA_SRV - данные считаны и обработаны без ошибок,
//		сообщение без параметров;
// 2. WM_END_JOB_SRV - нет сервера данных, сообщение без параметров.
//		В трёх последних случаях поток завершает свою работу.
//------------------------------------------------------------------------------
#include "StdAfx.h"
#include <ZetThread\ThreadSRV.h>
//------------------------------------------------------------------------------
DWORD WINAPI SRV_Thread(PVOID pParam)
{// функция рабочего потока сервера
	bool bWait;
	// адрес структуры параметров класса-прародителя CCustomZetThread
	ZetThreadParam0 *par0 = (ZetThreadParam0*)pParam;
	// адрес структуры параметров класса-родителя CCustomThreadSRV
	ZetThreadParamSRV0 *parSRV0 = (ZetThreadParamSRV0*)par0->pParamNext;
	// адрес структуры параметров данного класса CThreadSRV
	ZetThreadParamSRV1 *par1 = (ZetThreadParamSRV1*)parSRV0->pParamNext;

	while ( par0->FlagJob )
	{// цикл потока -----------------------------------------------------
		par0->FlagViewStop = false;
		bWait = true;
		if (par1->bNoWaitMissingChannel)
		{// штатная работа, пропавшего канала нет
			// проверяю время всех каналов ----------------
			// Перед этим по всем каналам надо вызвать NumFileUsed
			for (auto & it : par1->vectChan)
			{
				if ( parSRV0->pSRV->NumFileUsed(it.channel) == -1 )
				{// сервер выгрузился
					if (par0->hWndParent)
						::PostMessageW( par0->hWndParent, WM_END_JOB_SRV, 0, 0 );
					bWait = false;
					par0->FlagJob = false;
					break;	// заканчиваю работу потока
				}
				bWait &= parSRV0->pSRV->CurrentTime(it.channel) >= parSRV0->Time;
			}
		}
		else	// будем ждать (либо время, либо пропавший канал)
		{// ждём пропавший канал, проверяю время 0-канала, который есть всегда
			if ( parSRV0->pSRV->NumFileUsed(0) == -1 )
			{// сервер выгрузился
				if (par0->hWndParent)
					::PostMessageW( par0->hWndParent, WM_END_JOB_SRV, 0, 0 );
				bWait = false;
				par0->FlagJob = false;
				break;	// заканчиваю работу потока
			}
			bWait = bWait && ( parSRV0->pSRV->CurrentTime(0) >=
							parSRV0->Time );
		}

		if (bWait)
		{// время ожидания вышло, читаю данные, если надо ---------------
			// если требуется чтение данных
			if (par1->bNeedReadData && par1->bNoWaitMissingChannel)
			{
				par1->QualityCode = 0;
				for (auto & it : par1->vectChan)
				{// читаю данные по всем каналам
					it.saArray.ReSize(par1->NumberKadrs);
					par1->QualityCode |= parSRV0->pSRV->GetData(it.channel,
						par1->Decade, parSRV0->Time,
						par1->NumberKadrs, it.saArray.pData);
				}
				par1->dTimeData = parSRV0->Time;
				// функцию обработки надо вызывать всегда, обработка ошибок в ней!
				if (par0->Function)
				{// если функция обработки данных задана
					long kod = (*par0->Function)(par0->pParamFunction);
					Quality *pqc = (Quality*)&kod;
					if ( pqc->IsAllData() &&		// данные все
							parSRV0->hEventSynchro &&	// проверка SynchroView
							( WaitForSingleObject(parSRV0->hEventSynchro,
								par0->TimeOutSend) == WAIT_OBJECT_0 ) )
						::PostMessageW( par0->hWndParent, WM_NEW_DATA_SRV, 0, (LPARAM)kod );
				}
			}
			parSRV0->Time += parSRV0->TimeInterval;	// инкремент времени
		}
		else // if (bWait) надо ждать дальше ------------------
			Sleep(par0->TimeSleep);	// сервер работает, время не вышло, жду

		// если надо останавливаю поток ----------------------------------
		if (par0->FlagStop)
		{
			par0->FlagViewStop = true;
			SuspendThread(par0->hThread);	// последняя команда в цикле while
		}
	}// while ( par0->FlagJob )
	return 0;
}
//------------------------------------------------------------------------------
// Конструктор класса
//----------------------
CThreadSRV::CThreadSRV() : CCustomThreadSRV()
	, FAutoInterval(false)
{
	zParam0.pParamNext = &parSRV0;			// частные параметры потока
	parSRV0.pParamNext = &par1;

	// параметры по умолчанию
	par1.bNeedReadData = true;
	par1.Decade = 0;
	par1.NumberKadrs = 0;
	par1.bNoWaitMissingChannel = true;
}
//------------------------------------------------------------------------------
bool CThreadSRV::Create( CString nameThread, HWND hwnd, CDSRV *psrv,
						int numberChannels, int numberKadr, bool bautoInterval,
						FunctionThread function, PVOID paramFunction )
{
	bool ret = CCustomThreadSRV::Create(nameThread, hwnd, psrv,
			numberChannels, function, paramFunction ) && (numberChannels > 0);
	if (ret)
	{
		FAutoInterval = bautoInterval;
		SetNumberChannels(numberChannels);
		SetNumberKadrs(numberKadr);
		ret = CreateZetThread(&SRV_Thread);	// Создаю приостановленный поток
	}
	return ret;
}
//------------------------------------------------------------------------------
//		Деструктор класса
//-----------------------
CThreadSRV::~CThreadSRV()
{// Необходимо вначале синхронно остановить поток, а затем удалить массивы,
	// иначе возможна ситуация, когда поток обратится к уже удалённым объектам
	SynchroFinishThread();
// 	DeleteMassivAll();
}
//------------------------------------------------------------------------------
void CThreadSRV::CreateMassivChan(int numChan)
{
	int size = par1.vectChan.size();
	if ((numChan >= 0) && (numChan != size))
	{
		par1.vectChan.resize(numChan);

		if (numChan > size)
		{
			auto it = par1.vectChan.data() + size;
			for (int i = size; i < numChan; ++i, ++it)
			{
				it->channel = i;
				parSRV0.pSRV->GetChannelParEx(i, it);
			}
		}
	}
}
//------------------------------------------------------------------------------
bool CThreadSRV::SetNumberChannels(int val)
{
	bool ret = (0 < val) && (val < parSRV0.pSRV->QuanChan());
	int n = val, num = parSRV0.pSRV->QuanChan();
	if (n < 1)
	{
		n = 1;
		ret = false;
	}
	else if (n > num)
	{
		n = num;
		ret = false;
	}
	if ((int)par1.vectChan.size() != n)
		CreateMassivChan(n);
	parSRV0.NumberChannels = n;		// именно в конце!!!
	
	return ret;
}
//------------------------------------------------------------------------------
int CThreadSRV::GetChannels(int iChannels)
{
	return ((0 <= iChannels) &&
			(iChannels < (int)par1.vectChan.size())) ?
		par1.vectChan[iChannels].channel : -1;
}
//------------------------------------------------------------------------------
bool CThreadSRV::SetChannels(int iChannels, int val)
{	// Проверка индекса iChannels
	bool ret = (0 <= iChannels) && (iChannels < (int)par1.vectChan.size());
	if (ret)
	{
		ret = (0 <= val) && (val < parSRV0.pSRV->QuanChan());
		long chan = ret ? val : -1;
		par1.vectChan[iChannels].channel = chan;
		parSRV0.pSRV->GetChannelParEx(chan, par1.vectChan.data() + iChannels);
	}
	return ret;
}
//------------------------------------------------------------------------------
bool CThreadSRV::SetNumberKadrs(int val)
{
	bool ret = val > 0;
	if (ret)
	{
		par1.NumberKadrs = parSRV0.pSRV->BufferSize() / 2;
		ret = val <= par1.NumberKadrs;
		if (ret)
			par1.NumberKadrs = val;

		if (FAutoInterval)
			parSRV0.TimeInterval = par1.NumberKadrs / par1.vectChan[0].frequency;
	}
	return ret;
}
//------------------------------------------------------------------------------
float * CThreadSRV::GetDataPointer(int iChannel)
{
	return
		((0 <= iChannel) && (iChannel < (int)par1.vectChan.size())) ?
		(par1.vectChan.data() + iChannel)->saArray.pData :
		nullptr;
}
//------------------------------------------------------------------------------
bool CThreadSRV::Modify()
{
	bool ret(false);
	for (auto & it : par1.vectChan)
	{
		CHANNEL_FOR_KIT cfk;
		parSRV0.pSRV->GetChannelParEx(it.channel, &cfk);
		ret |= it.TestChangeStruct(cfk);
	}
	return ret;
}
//------------------------------------------------------------------------------