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
#include <ZetThread\ThreadSRVmultiSelf.h>
//------------------------------------------------------------------------------
DWORD WINAPI SRV_ThreadMultiSelf(PVOID pParam)
{// первичная глобальная функция рабочего потока
	int indexChan;
	CodeQuality retFunction;
	bool bIsSyncGUI(false);
	SendToSecGlobFunction parSend;
	// канал рабочий, если у него все три флага true
	//  (bEnabled, bJob и bExist)

	// адрес структуры параметров класса-прародителя CCustomZetThread
	ZetThreadParam0 *par0 = (ZetThreadParam0*)pParam;
	// адрес структуры параметров класса-родителя CCustomThreadSRV
	ZetThreadParamSRV0 *parSRV0 = (ZetThreadParamSRV0*)par0->pParamNext;
	// адрес структуры параметров данного класса CThreadSRVmulti
	ZetThreadParamSRVmultiSelf *par1 = (ZetThreadParamSRVmultiSelf*)parSRV0->pParamNext;
	// вектор мультиканалов
	std::vector<MultiChannelSelf> *pChannels = par1->pChannels;
	// адрес структуры - параметр вторичной глобальной функции
	parSend.pParamNext = par0->pParamFunction;

	while ( par0->FlagJob )
	{// цикл рабочего потока -------------------------------------------
		par0->FlagViewStop = false;
		// Проверка не было-ли Modify -----------------------------------
		WaitForSingleObject(parSRV0->hEventModify, par0->TimeOutSend);
		//---------------------------------------------------------------
		// проверка NumFileUsed всех используемых каналов
		for(auto it=pChannels->begin(), itEnd=pChannels->end(); it!=itEnd; ++it)
		{// определяю есть-ли рабочие каналы
			bool flag = true;
			if ( it->bEnabled && it->bJob && it->bExist)
			{// есть рабочий канал
				flag = false;
				if ( parSRV0->pSRV->NumFileUsed(it->Channel) == -1 )
				{// сервер выгрузился
					::PostMessageW( par0->hWndParent, WM_END_JOB_SRV, 0, 0 );
					par0->FlagJob = false;
					break;	// выхожу из цикла и завершаю поток
				}
			}
			if (flag)
				parSRV0->pSRV->NumFileUsed(0);
		}
		//---------------------------------------------------------------
		if (par0->FlagJob)
		{// если завершения нет
			indexChan = 0;
			for(auto it=pChannels->begin(), itEnd = pChannels->end(); it!=itEnd; ++it,++indexChan)
			{// цикл по всем каналам
				if (par0->FlagStop)	
					break;
				if ( it->bEnabled && it->bJob && it->bExist)
				{// есть рабочий канал
					// проверяю время на ошибку -14 ------------
					double timecur = parSRV0->pSRV->CurrentTime(it->Channel);
					double dtime = timecur - it->DataTime.GetTime();
					if (fabs(dtime) > MaxDeltaTimeInSec)
					{// прыгнуло время! перевожу время
						it->DataTime.SetStartTime(timecur);
					}
					
					// while нужен для того, чтобы успевать за поступившими новыми данными
				//	if (parSRV0->pSRV->CurrentTime(it->Channel) >= it->DataTime.GetTime() ) )
					BYTE whileCount = 0;
					while ( par0->FlagJob && !par0->FlagStop &&
							(parSRV0->pSRV->CurrentTime(it->Channel) >=
													it->DataTime.GetTime() ) )
					{
						if(bIsSyncGUI)
						{// поток GUI выполнил свою работу
							if ( !it->b7000 || (it->b7000 && par1->Support7000) )
							{
								it->RetGetData = parSRV0->pSRV->GetData(
									it->Channel, it->Decade, it->DataTime.GetTime(),
									it->NumberCounts, it->pData );
								if (it->RetGetData < 0)
									ippsSet_32f(std::numeric_limits<float>::quiet_NaN(), it->pData,
												(int)it->NumberCounts);
								parSend.index = indexChan;
								parSend.isCurrentChannel = indexChan == *par1->pFiIndexCur;
								parSend.pMChan = it._Ptr;
								retFunction = (*par0->Function)(&parSend);	// что-то делаю (вычисляю, ...)
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
							it->DataTime.IncrementTime();
						}
						else
						{// поток GUI работает, надо ждать
							bIsSyncGUI = WaitForSingleObject(parSRV0->hEventSynchro, par0->TimeOutSend) == WAIT_OBJECT_0;
						}
						if (++whileCount > WhileCountMax)
							break;	// выход из while
					}	// while
				}	// if ( it->bEnabled && ...
			}	// for(auto it=pChannels->begin(); ...
			// завершение цикла ----------------------------------------------
			if (!par0->FlagStop)					// останавливать поток не надо
				Sleep(par0->TimeSleep);			// надо отдать процессор
			else
			{// надо остановить поток
				par0->FlagViewStop = true;
				SuspendThread(par0->hThread);// последняя команда в цикле while
			}
		}	// if (par0->FlagJob)
	}// while ( par0->FlagJob )
	return 0;
}
//------------------------------------------------------------------------------
CThreadSRVmultiSelf::CThreadSRVmultiSelf() : CCustomThreadSRV()
, m_constFreqADCminFor_0_1(10.f)
, m_constFreqADCminFor_1_0(1.f)
{
	FIndexCur = 0;
	FFreqADCMin = 50;
	m_Channels.clear();
	par1.pChannels = &m_Channels;
	par1.Support7000 = true;
	par1.pFiIndexCur = &FIndexCur;
	parSRV0.pParamNext = &par1;
}
//------------------------------------------------------------------------------
bool CThreadSRVmultiSelf::Create(
	CString nameThread,			// имя потока
	HWND hwnd,					// HWND родительского окна
	CDSRV *psrv, 				// указатель на сервер
	int numberChannels,			// кол-во каналов
	FunctionThread function,	// адрес вторичной функции
	PVOID paramFunction )		// параметры вторичной функции
{
	bool ret = (numberChannels > 0) &&
		CCustomThreadSRV::Create(nameThread, hwnd, psrv, numberChannels,
									function, paramFunction);
	if (ret)
	{
		parSRV0.NumberChannels = numberChannels;
		try
		{
			m_Channels.resize( (size_t)parSRV0.NumberChannels );
		}
		catch (CMemoryException*)
		{
			parSRV0.NumberChannels = 0;
			m_Channels.clear();
			ret = false;	
		}
		if (ret)	// Создаю приостановленный поток
			ret = CreateZetThread(&SRV_ThreadMultiSelf);
	}
	return ret;
}
//------------------------------------------------------------------------------
CThreadSRVmultiSelf::~CThreadSRVmultiSelf()
{// Необходимо вначале синхронно остановить поток, а затем удалить данные,
	// иначе возможна ситуация, когда поток обратится к уже удалённым объектам
	SynchroFinishThread();
}
//------------------------------------------------------------------------------
//bool CThreadSRVmultiSelf::CalcNumKadrs(int index, bool testIndex)
//{// Расчет кол-ва кадров для канала
//	DWORD num = 0;
//	bool ret = false;
//	if (testIndex)
//		ret = TestIndex(index);
//	else
//		ret = true;
//
//	if (ret)
//	{
//		MultiChannelSelf *pMChan = &m_Channels[index];
//		if (pMChan->bEnabled)
//		{// канал задан правильно
//			if (pMChan->bAutoNumberCounts)
//			{
//				double x = pMChan->DataTime.GetTimeInterval() *
//					pMChan->freqADC;
//				if (pMChan->b7000)
//				{// маленькая частота дискретизации, лучше взять ещё отсчёт,
//					// чем рано или позно потерять отсчёт
//					num = (DWORD)(int)floor(x);
//					if (x > (double)num)
//						++num;
//				}
//				else
//					num = (DWORD)(int)floor(x + 0.5);
//			}
//			else
//				num = pMChan->NumberCounts;
//
//			// проверка на превышение размера буфера
//			if (num > pMChan->NumberBufferSRV)
//				num = pMChan->NumberBufferSRV;
//
//			// проверка размера массива
//			if (num > 0)
//				ret = pMChan->ReSize(num);
//
//			if (ret)
//				pMChan->NumberCounts = num;
//			else
//				pMChan->NumberCounts = 0;
//		}
//	}
//	return ret;
//}
//------------------------------------------------------------------------------
long CThreadSRVmultiSelf::GetChannelDecade(int index)
{
	return TestIndex(index) ? m_Channels[index].Decade : -1;
}
//------------------------------------------------------------------------------
void CThreadSRVmultiSelf::SetChannelDecade(int index, long val)
{
	if( TestIndex(index) )
	{
		if (val < 0)
		{
			val = 0;
		}
		else if (val >= MAXDECADE)
		{
			val = MAXDECADE - 1;
		}
		m_Channels[index].Decade = val;
		m_Channels[index].NumberBufferSRV = parSRV0.pSRV->DecadeBufferSize(val) / 2;
	}
}
//------------------------------------------------------------------------------
bool CThreadSRVmultiSelf::SetChannelNumber(long numChan)
{// Установка номера канала сервера текущему каналу
	MultiChannelSelf *pMChan = &m_Channels[FIndexCur];
	bool ret = ( numChan >= 0 ) && ( numChan < parSRV0.pSRV->QuanChan() );
	pMChan->Channel = numChan;
	if (ret)
	{//канал есть в сервере
		pMChan->bJob = pMChan->bEnabled = ret =
			parSRV0.pSRV->IdChan(numChan, &pMChan->ChannelId) == 0;
		if (ret)
		{// ID у канала определён правильно;
			long status = parSRV0.pSRV->GetStatus(numChan);
			pMChan->bExist = (status != 4) && (status != 5) && (status != 7);
			// 4 - отключенный канал АЦП; 5 - канал отключенного АЦП
			// 7 - отключенный канал интеллектуального датчика
			pMChan->freqADC = parSRV0.pSRV->Frequency(numChan);
			pMChan->b7000 = pMChan->freqADC < FFreqADCMin;
			pMChan->NumberBufferSRV = parSRV0.pSRV->DecadeBufferSize(pMChan->Decade) / 2;
			pMChan->SetNumberCounts();
		//	CalcNumKadrs(FIndexCur, false);
		}
		else
		{
			pMChan->bExist = false;
			pMChan->ChannelId = -1;
		}
	}
	else
	{
		pMChan->bJob = pMChan->bExist = pMChan->bEnabled = false;
		pMChan->ChannelId = -1;
	}
	return ret;	// заданный канал существует или нет
}
//------------------------------------------------------------------------------
bool CThreadSRVmultiSelf::SetChannelNumberCounts(DWORD val)
{// Задание текущему каналу кол-во считываемых кадров "руками"
	return m_Channels[FIndexCur].SetNumberCounts(val);
}
//------------------------------------------------------------------------------
bool CThreadSRVmultiSelf::SetChannelNumberCountsAuto()
{// Задание текущему каналу кол-во считываемых кадров после задания параметров
//	в CDataTime
	return m_Channels[FIndexCur].SetNumberCounts();
}
//------------------------------------------------------------------------------
bool CThreadSRVmultiSelf::SetIndexCur(int index, bool test)
{// задание текущего индекса
	bool ret;
	if (test)
		ret = TestIndex(index);
	else
		ret = true;
	if (ret)
		FIndexCur = index;
	return ret;
}
//------------------------------------------------------------------------------
bool CThreadSRVmultiSelf::Modify()
{
	bool ret = false, flag;	// были изменения или нет
	int i = 0;
	long sta;
	StopThreadForModifySRV();
	for(auto it=m_Channels.begin(), ie=m_Channels.end(); it!=ie; ++it, ++i)
	{
		if (it->bEnabled)	// цикл по заданным каналам
		{
			parSRV0.pSRV->IdChan(it->Channel, &sta);
			flag = sta != it->ChannelId;		// номер канала измениля?
			ret = ret || flag;
			if (flag)
				it->Channel = parSRV0.pSRV->detectChanNum(it->ChannelId);
			if (it->Channel != -1)
			{
				sta = parSRV0.pSRV->GetStatus(it->Channel);
				it->bExist = (sta != 4) && (sta != 5) && (sta != 7);
				// 4 - отключенный канал АЦП; 5 - канал отключенного АЦП
				// 7 - отключенный канал интеллектуального датчика
			}
			else
				it->bExist = false;
			if (it->bExist)
			{// проверяю изменилась-ли частота дискретизации по этому канала
				float freq = parSRV0.pSRV->Frequency(it->Channel);
				flag = it->freqADC != freq;
				ret = ret || flag;
				if (flag)
				{
					it->freqADC = freq;
					if (it->bAutoNumberCounts)
						it->SetNumberCounts();
				}
			}
		}
	}
	GoOnThreadAfterModifySRV();
	return ret;
}
//------------------------------------------------------------------------------
long CThreadSRVmultiSelf::AddChannel(long chan)
{// Добавление канала с номером chan. Возвращаемое значение:
	// добавленный канал становится текущим
	//  0 - канал добавлен успешно
	// -1 - уже есть максимальное число
	// -2 - канал добавлен, но номер chan - это несуществующий канал
	long ret;
	MultiChannelSelf mc;
	if (parSRV0.NumberChannels < MAXCHAN)
	{// если каналов меньше 128
		m_Channels.push_back(mc);
		FIndexCur = parSRV0.NumberChannels++;
		if ( SetChannelNumber(chan) )
			ret = 0;
		else
			ret = -2;
	}
	else
		ret = -1;
	return ret;
}
//------------------------------------------------------------------------------
float* CThreadSRVmultiSelf::GetDataPointer(int index)
{
	float *pret(nullptr);
	if ( TestIndex(index) )
		pret = m_Channels[index].pData;
	else
		pret = NULL;
	return pret;
}
//------------------------------------------------------------------------------
bool CThreadSRVmultiSelf::StartChannel(int index)
{// Если индекс рабочий, то добавление канала
	bool ret = TestIndex(index);
	if (ret)	// если индекс рабочий
	{
		MultiChannelSelf *pMChan = &m_Channels[index];
		pMChan->bJob = true;
		pMChan->DataTime.SetStartTime( parSRV0.pSRV->CurrentTime(pMChan->Channel) );
	}
	return ret;	// true если индекс и номер канала сервера рабочие
}
//------------------------------------------------------------------------------
bool CThreadSRVmultiSelf::StopChannel(int index)
{// Если индекс рабочий, то в любом случае - сброс флага bJob
	bool ret = TestIndex(index);
	if (ret)
		m_Channels[index].bJob = false;
	return ret;
}
//------------------------------------------------------------------------------
void CThreadSRVmultiSelf::SetTime(double val)
{
	for(auto it=m_Channels.begin(); it!=m_Channels.end(); ++it)
		it->DataTime.SetStartTime( parSRV0.pSRV->CurrentTime(it->Channel) );
}
//------------------------------------------------------------------------------