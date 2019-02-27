//------------------------------------------------------------------------------
//	Файл KitOfChannels.cpp
//	Зеленоград, ЗАО "ЭТМС", Савостин Ю.М.
//------------------------------------------------------------------------------
//
#include "StdAfx.h"
#include "KitOfChannels.h"
#include <mathZet.h>
#include <stdarg.h>
//------------------------------------------------------------------------------
STRUCT_KIT_OF_CHANNEL::STRUCT_KIT_OF_CHANNEL()
	: m_vectChannels(1)
	, m_bFlags(0)
	, m_numberReadOk(0)
	, m_numberChannels(0)
	, m_numberChannelsWork(0)
	, m_hEventSynchro(NULL)
	, m_cTimeRead()
	, m_zts_timeDataCur()
{	// создаю событие с автосбросом и с сигнальным (свободным) состоянием
	m_hEventSynchro = CreateEventW(NULL, FALSE, TRUE, NULL);
	PlaceFlag(Flag_Kit_Read);
	m_zt_lastTimeReadComp.SetCurrentTime();
	InitializeCriticalSection(&m_crSectinKit);
}
//------------------------------------------------------------------------------
STRUCT_KIT_OF_CHANNEL::~STRUCT_KIT_OF_CHANNEL()
{
	if (m_hEventSynchro)
		CloseHandle(m_hEventSynchro);
	m_hEventSynchro = NULL;
	DeleteCriticalSection(&m_crSectinKit);
}
//------------------------------------------------------------------------------
STRUCT_KIT_OF_CHANNEL::STRUCT_KIT_OF_CHANNEL(const STRUCT_KIT_OF_CHANNEL & val)
{
	m_bFlags = val.m_bFlags;
	m_vectChannels = val.m_vectChannels;
	m_numberChannels = m_vectChannels.size();
	m_numberReadOk = val.m_numberReadOk;
	m_numberChannelsWork = val.m_numberChannelsWork;
	m_numberChannels = (DWORD)m_vectChannels.size();
	m_zts_timeDataCur = val.m_zts_timeDataCur;
	m_cTimeRead = val.m_cTimeRead;

	// создаю событие с автосбросом и с сигнальным (свободным) состоянием
	m_hEventSynchro = CreateEventW(NULL, FALSE, TRUE, NULL);
	m_zt_lastTimeReadComp.SetCurrentTime();
	InitializeCriticalSection(&m_crSectinKit);
}
//------------------------------------------------------------------------------
STRUCT_KIT_OF_CHANNEL & STRUCT_KIT_OF_CHANNEL::operator = (const STRUCT_KIT_OF_CHANNEL & val)
{
	m_bFlags = val.m_bFlags;
	m_vectChannels = val.m_vectChannels;
	m_numberChannels = m_vectChannels.size();
	m_numberReadOk = val.m_numberReadOk;
	m_numberChannelsWork = val.m_numberChannelsWork;
	m_numberChannels = (DWORD)m_vectChannels.size();
	m_zts_timeDataCur = val.m_zts_timeDataCur;
	m_cTimeRead = val.m_cTimeRead;
	m_zt_lastTimeReadComp = val.m_zt_lastTimeReadComp;

	// создаю событие с автосбросом и с сигнальным (свободным) состоянием
	m_hEventSynchro = CreateEventW(NULL, FALSE, TRUE, NULL);

	return *this;
}
//------------------------------------------------------------------------------
// Чтение флага
bool STRUCT_KIT_OF_CHANNEL::GetFlag(uint32_t flag)
{
	return m_bFlags.GetFlag(flag);
}
//------------------------------------------------------------------------------
// Установка флага булевским значением
void STRUCT_KIT_OF_CHANNEL::SetFlag(uint32_t flag, bool val)
{
	m_bFlags.SetFlag(flag, val);
}
//------------------------------------------------------------------------------
// Установка флага в 1
void STRUCT_KIT_OF_CHANNEL::PlaceFlag(uint32_t flag)
{
	m_bFlags.PlaceFlag(flag);
}
//------------------------------------------------------------------------------
// Сброс флага в 0
void STRUCT_KIT_OF_CHANNEL::ResetFlag(uint32_t flag)
{
	m_bFlags.ResetFlag(flag);
}
//------------------------------------------------------------------------------
// Получение кол-ва рабочих каналов в комплекте
DWORD STRUCT_KIT_OF_CHANNEL::GetNumChannelsWork()
{// Кр. секция контейнера вызывается до вызова этой функции
	m_numberChannelsWork = 0;
	if (!m_vectChannels.empty())
	{
		for (auto & it : m_vectChannels)
		{
			if (it.GetFlag(Flag_Chan_OnOff) &&
				it.GetFlag(Flag_Chan_Exist) &&
				!it.GetFlag(Flag_Chan_Disabled))
				++m_numberChannelsWork;
		}
	}
	return m_numberChannelsWork;
}
//------------------------------------------------------------------------------
// Задание размера буфера для чтения данных в соответствии с интервалом времени из CDataTimeZT.
// Выполняется проверка на min и max значения.
// Возвращаемое значение - новый размер (с учетом проверки) установлен.
bool STRUCT_KIT_OF_CHANNEL::ReSize()
{// Кр. секция контейнера вызывается до вызова этой функции
	bool ret = !m_vectChannels.empty();
	if (ret)
	{
		for (auto & it : m_vectChannels)
		{
			ret &= it.ReSize(m_cTimeRead.GetIntervalBuffer());
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
// Получение минимального SRV-времени по комплекту
double STRUCT_KIT_OF_CHANNEL::GetTimeMin(CDSRV * psrv, const bool bNeed7000)
{// Кр. секция контейнера вызывается до вызова этой функции
	double ret(DBL_MAX);
	if (!m_vectChannels.empty())
	{
		for (auto & it : m_vectChannels)
		{	// Цикл по каналам контейнера, it - структура одного канала
			psrv->NumFileUsed(it.channel);
			bool bFlag =
				it.GetFlag(Flag_Chan_OnOff) &&
				it.GetFlag(Flag_Chan_Exist) &&
				!it.GetFlag(Flag_Chan_Disabled);
			if (!bNeed7000)
				bFlag &= !it.GetFlag(Flag_Chan_7000);
			
			// Проверка, что время по каналу идёт ----------------------
			// Сброс флага (как-бы значение по умолчанию), именно здесь!
			it.ResetFlag(Flag_Chan_Stop_Time);
			if (bFlag)
			{	// Только для рабочих каналов
				double curTime = psrv->CurrentTime(it.channel);
				if (ret > curTime)
					ret = curTime;
				
				if (it.ChannelIsOk())
				{	// Проверка распрространяется только на рабочие каналы
					it.SetFlag(Flag_Chan_Stop_Time, (it.m_lastTimeRead >= curTime));
					if (it.GetFlag(Flag_Chan_Stop_Time))
					{// Время по каналу не растёт
						// время по компу, прошедшее с момента последнего чтения
						CZetTime zt;
						zt.SetCurrentTime();
						CZetTimeSpan zts = zt - m_zt_lastTimeReadComp;
						double pastTime = (double)zts;
						// ---------------------------
						double dt1 = Factor_Chan_Stop_Time * (double)m_cTimeRead.GetIntervalRead();
						double dt2 = Factor_Chan_Stop_Time / it.frequency;
						// ---------------------------
						it.SetFlag(Flag_Chan_Stop_Time, (pastTime > max(dt1, dt2)));
					}
				}
			}// if (bFlag)				
		}// for (auto & it : m_vectChannels)
	}// if (!m_vectChannels.empty())
	return ret;
}
//------------------------------------------------------------------------------
bool STRUCT_KIT_OF_CHANNEL::SetTimeStart(CDSRV * psrv, const bool bNeed7000)
{
	double time = GetTimeMin(psrv, bNeed7000);
	bool ret = time < DBL_MAX;
	if (ret)
		m_cTimeRead.SetTimeReadStart(time);
	return ret;
}
//------------------------------------------------------------------------------
bool STRUCT_KIT_OF_CHANNEL::SetTimeStart(const double time)
{
	bool ret = _finite(time) != 0;
	if (ret)
		m_cTimeRead.SetTimeReadStart(time);
	return ret;
}
//------------------------------------------------------------------------------
// Проверка готовности синхронного чтения данных
bool STRUCT_KIT_OF_CHANNEL::ReadinessData(CDSRV * psrv, const bool bNeed7000)
{// Кр. секция контейнера вызывается до вызова этой функции
	double time = GetTimeMin(psrv, bNeed7000);
	return time >= m_cTimeRead.GetTime();
}
//------------------------------------------------------------------------------
// Синхронное чтение данных с каналов комплекта.
// Если было чтение, то в m_zts_timeDataCur записывается время
// прочитанных данных и инкрементируется время чтения
// Возвращает флаг - чтение было/не было.
//
bool STRUCT_KIT_OF_CHANNEL::GetData(CDSRV * psrv, const bool bNeed7000)
{// Кр. секция контейнера вызывается до вызова этой функции
	bool ret(false);
	if (!m_vectChannels.empty())
	{
		double timeRead = m_cTimeRead.GetTime();
		m_numberReadOk = 0; // кол-во каналов, у которых успешно прочитаны данные.
		for (auto & it : m_vectChannels)
		{
			EnterCriticalSection(&it.m_crSectionChan);
			m_numberReadOk = 0;
			bool bFlag =
				it.GetFlag(Flag_Chan_OnOff) &&
				it.GetFlag(Flag_Chan_Exist) &&
				!it.GetFlag(Flag_Chan_Disabled);
			if (!bNeed7000)
				bFlag &= !it.GetFlag(Flag_Chan_7000);
			if (bFlag)
			{	// чтение данных если канал задан и он существует
 				if (it.GetFlag(Flag_Change_Freq))
					it.ReSize(m_cTimeRead.GetIntervalBuffer());

				if (it.saArray.pData != nullptr)
				{
					it.retGetData = psrv->GetData(it.channel, it.m_decade, timeRead,
						it.saArray.size, it.saArray.pData);
					it.m_lastTimeRead = timeRead;
// 					CString s;
// 					s.Format(L"SRV::ReadData: ret = %d, chan = %d, decada = %d, time = %f, size = %u\n",
// 						it.retGetData, it.channel, it.m_decade, timeRead, it.saArray.size);
// 					OutputDebugStringW(s);
				}
				else
					it.retGetData = -13;		// нулевой указатель у буфера

				if (it.retGetData >= 0)
				{
					++m_numberReadOk;
				}
				ret = true;
			}
			else
			{	// канал не задан, не существует, или
				// типа 7ххх при  отсутствии поддержки 7ххх
				it.retGetData = -24;	
				if (it.saArray.size > 0)
					it.saArray.NaN();
			}

			LeaveCriticalSection(&it.m_crSectionChan);
		}// for (auto & it : m_vectChannels)
		m_zt_lastTimeReadComp.SetCurrentTime();
		if (ret)
		{
			m_zts_timeDataCur = m_cTimeRead.GetTimeRead();
			m_cTimeRead.IncrementTime();
		}
	}// if (!m_vectChannels.empty())
	return ret;
}
//------------------------------------------------------------------------------.
// Если что-то поменялось, то возрашается true.
bool STRUCT_KIT_OF_CHANNEL::Modify(CDSRV * psrv, CHANNEL_STRUCT & tmpChanStr, const double FriqMin)
{	// новые параметры каналов
	bool ret = !m_vectChannels.empty();
	if (ret)
	{
		ret = false;
// 		EnterCriticalSection(&m_crSectinKit);
		bool bChangeStatus(false);
		m_numberChannelsWork = 0;
		for (auto & it : m_vectChannels)
		{	// обновление данных если канал задан
			if (it.GetFlag(Flag_Chan_OnOff))
			{	// канал мог не существовать или
				// номер канала мог поменяться
				EnterCriticalSection(&it.m_crSectionChan);
				it.channel = psrv->detectChanNum(it.id);
				it.SetFlag(Flag_Chan_Exist, it.channel >= 0);

				if (it.GetFlag(Flag_Chan_Exist))
				{
					tmpChanStr = it;
					psrv->GetChannelParEx(it.channel, &it);
					it.SetFlag(Flag_Chan_7000, tmpChanStr.frequency < FriqMin);
					if (it.TestChangeStruct(tmpChanStr))
						ret = true;
					bChangeStatus |= it.GetFlag(Flag_Change_Status);
					it.DoChangeStatus();	// статус мог поменяться
				}
				LeaveCriticalSection(&it.m_crSectionChan);
			}
			if (it.GetFlag(Flag_Chan_OnOff) &&
				it.GetFlag(Flag_Chan_Exist) &&
				!it.GetFlag(Flag_Chan_Disabled))
				++m_numberChannelsWork;
		}// for (auto & it : m_vectChannels)
		SetFlag(Flag_Kit_Change_Status, bChangeStatus);
		SetFlag(Flag_Kit_Channes_IsOk, m_numberChannelsWork > 0);
		SetFlag(Flag_Kit_Channes_AllOk, m_numberChannelsWork == m_numberChannels);
// 		LeaveCriticalSection(&m_crSectinKit);
	}
	DefineFlagsOfKit();
	return ret;
}
//------------------------------------------------------------------------------
void STRUCT_KIT_OF_CHANNEL::SetSynchro()
{
	if (m_hEventSynchro)
		SetEvent(m_hEventSynchro);
}
//------------------------------------------------------------------------------
void STRUCT_KIT_OF_CHANNEL::DefineFlagsOfKit()
{
	GetNumChannelsWork();
	SetFlag(Flag_Kit_Channes_IsOk, m_numberChannelsWork > 0);
	SetFlag(Flag_Kit_Channes_AllOk, m_numberChannelsWork == m_numberChannels);
}
//------------------------------------------------------------------------------
void STRUCT_KIT_OF_CHANNEL::SetTimeParam(
	const CZetTimeSpan & timeInternalRead, const CZetTimeSpan & timeInternalBuffer)
{
	EnterCriticalSection(&m_crSectinKit);
	m_cTimeRead.SetIntervalRead(timeInternalRead);
	m_cTimeRead.SetIntervalBuffer(timeInternalBuffer);
	LeaveCriticalSection(&m_crSectinKit);
}
//------------------------------------------------------------------------------
void STRUCT_KIT_OF_CHANNEL::ResetAllFlagChangeFreq()
{// Кр. секция контейнера вызывается до вызова этой функции
	if (!m_vectChannels.empty())
	{
		for (auto & it : m_vectChannels)
		{
			it.ResetFlag(Flag_Change_Freq);
		}
	}
}
//------------------------------------------------------------------------------
void STRUCT_KIT_OF_CHANNEL::SetAllFlagChangeFreq(bool val)
{// Кр. секция контейнера вызывается до вызова этой функции
	if (!m_vectChannels.empty())
	{
		for (auto & it : m_vectChannels)
		{
			it.SetFlag(Flag_Change_Freq, val || it.GetFlag(Flag_Change_Freq));
		}
	}
}
//------------------------------------------------------------------------------
// Время, оставшееся до очередного усреднения, сек
double STRUCT_KIT_OF_CHANNEL::GetTimeBeforeNextAver(CDSRV * psrv, const bool bNeed7000)
{
	double ret = GetTimeMin(psrv, bNeed7000);
	if (ret < DBL_MAX)
		ret = (double)m_cTimeRead.GetTimeAver() - ret;
	return ret;
}
//------------------------------------------------------------------------------
// Время, оставшееся до очередного чтения, сек
double STRUCT_KIT_OF_CHANNEL::GetTimeBeforeNextRead(CDSRV * psrv, const bool bNeed7000)
{
	double ret = GetTimeMin(psrv, bNeed7000);
	if (ret < DBL_MAX)
		ret = m_cTimeRead.GetTime() - ret;
	return ret;
}
//------------------------------------------------------------------------------