//------------------------------------------------------------------------------
//	Файл ChannelForKit.cpp
//------------------------------------------------------------------------------
//		Класс предназначен для считывания данных и их обработки с независимых
// каналов сервера. Т.е. каналов, возможно от разных устройств,
// с разными частотами дискретизации, разным кол-вом кадров и т.д.
//		Считается, что данные каналов не требуют совместной обработки!!!
//		Интервалы считывания данных у каждого канала может быть своим.
//------------------------------------------------------------------------------
#pragma once
#include "StdAfx.h"
#include <mathZet.h>
#include "ChannelForKit.h"
//------------------------------------------------------------------------------
CHANNEL_FOR_KIT::CHANNEL_FOR_KIT() : CHANNEL_STRUCT()
	, channel(-1)
	, retGetData(-1)
	, bFlags()
	, saArray()
	, m_decade(0)
	, m_numKadrMin(1)
	, m_numKadrMax(100000)	// = m_psrv->DecadeBufferSize(0) * 0.8;
	, m_lastTimeRead(-DBL_MAX)
{
	InitializeCriticalSection(&m_crSectionChan);
}
//------------------------------------------------------------------------------
CHANNEL_FOR_KIT::~CHANNEL_FOR_KIT()
{
	DeleteCriticalSection(&m_crSectionChan);
}
//------------------------------------------------------------------------------
CHANNEL_FOR_KIT::CHANNEL_FOR_KIT(const CHANNEL_FOR_KIT & val) :
	CHANNEL_STRUCT(val)
{
	channel = val.channel;
	retGetData = val.retGetData;
	bFlags = val.bFlags;
	saArray = val.saArray;
	m_decade = val.m_decade;
	m_numKadrMin = val.m_numKadrMin;
	m_numKadrMax = val.m_numKadrMax;
	m_lastTimeRead = val.m_lastTimeRead;

	InitializeCriticalSection(&m_crSectionChan);
}
//------------------------------------------------------------------------------
CHANNEL_FOR_KIT & CHANNEL_FOR_KIT::operator = (const CHANNEL_FOR_KIT & val)
{
	*this = *((CHANNEL_STRUCT*)&val);

	channel = val.channel;
	retGetData = val.retGetData;
	bFlags = val.bFlags;
	saArray = val.saArray;
	m_decade = val.m_decade;
	m_numKadrMin = val.m_numKadrMin;
	m_numKadrMax = val.m_numKadrMax;

// 	InitializeCriticalSection(&m_crSectinChan);

	return *this;
}
//------------------------------------------------------------------------------
CHANNEL_FOR_KIT & CHANNEL_FOR_KIT::operator = (const CHANNEL_STRUCT & val)
{
	*((CHANNEL_STRUCT*)this) = val;
	return *this;
}
//------------------------------------------------------------------------------
// Чтение флага
bool CHANNEL_FOR_KIT::GetFlag(uint32_t flag)
{
	return bFlags.GetFlag(flag);
}
//------------------------------------------------------------------------------
// Установка флага булевским значением
void CHANNEL_FOR_KIT::SetFlag(uint32_t flag, bool val)
{
	bFlags.SetFlag(flag, val);
}
//------------------------------------------------------------------------------
// Установка флага в 1
void CHANNEL_FOR_KIT::PlaceFlag(uint32_t flag)
{
	bFlags.PlaceFlag(flag);
}
//------------------------------------------------------------------------------
// Сброс флага в 0
void CHANNEL_FOR_KIT::ResetFlag(uint32_t flag)
{
	bFlags.ResetFlag(flag);
}
//------------------------------------------------------------------------------
// Если установлен флаг изменения статуса, то переопределяется флаг
// существования канала и флаг изменения статуса сбрасывается
void CHANNEL_FOR_KIT::DoChangeStatus()
{
	if (GetFlag(Flag_Change_Status))
	{
		SetFlag(Flag_Chan_Exist, channel != -1);
		SetFlag(Flag_Chan_Disabled, IsDisabled());
		ResetFlag(Flag_Change_Status);
	}
}
//------------------------------------------------------------------------------
// Анализ статуса канала на предмет отключения
bool CHANNEL_FOR_KIT::IsDisabled()
{
	return
		(status == 4) ||	// отключенный канал АЦП
		(status == 5) ||	// канал отключенного АЦП
		(status == 7) ||	// отключенный интеллектуальный датчик
		(status == 9);		// неактивный виртуальный канал
}
//------------------------------------------------------------------------------
// Анализ статуса канала на предмет работоспособности канала
bool CHANNEL_FOR_KIT::ChannelIsOk()
{
	return
		(status != 4) &&	// не отключенный канал АЦП
		(status != 5) &&	// канал не отключенного АЦП
		(status != 7) &&	// не отключенный интеллектуальный датчик
		(status != 9);		// не неактивный виртуальный канал

}
//------------------------------------------------------------------------------
// Проверка значений новой структуры и установка соответствующих флагов
// Возвращает true если структуры разные
bool CHANNEL_FOR_KIT::TestChangeStruct(CHANNEL_STRUCT & pstr)
{
	bool ret(false);
	if (pstr != *(LPCHANNEL_STRUCT)this)
	{
		if (pstr.id == id)
		{
			if (pstr.status != status)
			{
				ret = true;
				PlaceFlag(Flag_Change_Status);
			}
			if (pstr.frequency != frequency)
			{
				ret = true;
				PlaceFlag(Flag_Change_Freq);
			}
			if (wcscmp(pstr.name, name) != 0)
			{
				ret = true;
				PlaceFlag(Flag_Change_Name);
			}
			if (wcscmp(pstr.measure, measure) != 0)
			{
				ret = true;
				PlaceFlag(Flag_Change_Measure);
			}
			if ((pstr.subLevel != subLevel) ||
				(pstr.minLevel != minLevel) ||
				(pstr.maxLevel != maxLevel) ||
				(pstr.sense != sense))
			{
				ret = true;
				PlaceFlag(Flag_Change_Level);
			}
			if (pstr.dc != dc)
			{
				ret = true;
				PlaceFlag(Flag_Change_Level_DC);
			}
			if (pstr.referense != referense)
			{
				ret = true;
				PlaceFlag(Flag_Change_Level_dB);
			}
		}
		else
		{
			ret = true;
			PlaceFlag(Flag_Change_Chan);
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
void CHANNEL_FOR_KIT::InitFlags()
{
 	SetFlag(Flag_Chan_Exist, channel != -1);
	SetFlag(Flag_Chan_Disabled, IsDisabled());

	PlaceFlag(Flag_Change_Chan);
	PlaceFlag(Flag_Change_Status);
	PlaceFlag(Flag_Change_Freq);
	PlaceFlag(Flag_Change_Level);
	PlaceFlag(Flag_Change_Level_DC);
	PlaceFlag(Flag_Change_Level_dB);
	PlaceFlag(Flag_Change_Name);
	PlaceFlag(Flag_Change_Measure);

	ResetFlag(Flag_Chan_Stop_Time);
}
//------------------------------------------------------------------------------
// Задание размера буфера чтения по интервалу времени
bool CHANNEL_FOR_KIT::ReSize(const DWORD sizeBuffer)
{
	DWORD len(sizeBuffer);
	if (len < m_numKadrMin)
		len = m_numKadrMin;
	else if (len > m_numKadrMax)
		len = m_numKadrMax;

	return saArray.ReSize(sizeBuffer);
}
//------------------------------------------------------------------------------
bool CHANNEL_FOR_KIT::ReSize(const double dIntervalBuffer)
{
	return ReSize((DWORD)RoundToInt(frequency * dIntervalBuffer));
}
//------------------------------------------------------------------------------
bool CHANNEL_FOR_KIT::ReSize(const CZetTimeSpan & ztsIntervalBuffer)
{
	return ReSize((double)(CZetTimeSpan)ztsIntervalBuffer);
}
//------------------------------------------------------------------------------