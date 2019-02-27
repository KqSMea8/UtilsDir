//------------------------------------------------------------------------------
//	Файл ChannelForKit.h
//------------------------------------------------------------------------------
//		Класс предназначен для считывания данных и их обработки с независимых
// каналов сервера. Т.е. каналов, возможно от разных устройств,
// с разными частотами дискретизации, разным кол-вом кадров и т.д.
//		Считается, что данные каналов не требуют совместной обработки!!!
//		Интервалы считывания данных у каждого канала может быть своим.
//------------------------------------------------------------------------------
#pragma once
#include "afx.h" 
#include <limits>
#include <chaninfo.h>
#include <Flags.h>
#include <Buffer\SmartArray.h>
#include <ZetTools\include\ZetTools.h>
//------------------------------------------------------------------------------
#ifndef WhileCountMax
#define WhileCountMax		100
#endif

#ifndef MaxDeltaTimeInSec
#define MaxDeltaTimeInSec	(60.)
#endif
//------------------------------------------------------------------------------
// Флаги канала. Всего флагов 32, из них используются 13.

#define Flag_Chan_OnOff			0x1		// 1 - работа с каналом
#define Flag_Chan_Exist			0x2		// 2 - такой канал у сервера есть
#define Flag_Chan_7000			0x4		// 3 - канал с маленькой freqADC
#define Flag_Chan_Disabled		0x8		// 4 - канал отключен

#define Flag_Change_Chan		0x10	// 5 - другой канал (ID другой)
#define Flag_Change_Status		0x20	// 6 - изменился статус канала

#define Flag_Change_Freq		0x40	// 7 - изм. частота
#define Flag_Change_Level		0x80	// 8 - изм. амплитудные значения
#define Flag_Change_Level_DC	0x100	// 9 - изм. пост. составляющая
#define Flag_Change_Level_dB	0x200	// 10 - изм. опорное знач. для dB
#define Flag_Change_Name		0x400	// 11 - изм. имя канала
#define Flag_Change_Measure		0x800	// 12 - изм. размерность канала

#define Flag_Chan_Stop_Time		0x1000	// 13 - остановилось время по каналу
//------------------------------------------------------------------------------
// Структура параметров канала сервера, предназначенная для работы в составе
// комплекта каналов. Содержит параметры, необходимые для чтения данных.
// Наследник CHANNEL_STRUCT.
//
struct CHANNEL_FOR_KIT : public CHANNEL_STRUCT
{
	long channel;				// номер канала
	long m_decade;				// декада

	DWORD m_numKadrMin;			// минимальный размер массива
	DWORD m_numKadrMax;			// максимальный размер массива

	long retGetData;			// код, возвращаемый функцией GetData
	Flags<uint32_t> bFlags;		// 32 флага канала

	double m_lastTimeRead;		// последнее время чтения данных по серверу
	SmartArray<float> saArray;	// массив для данных канала

	CRITICAL_SECTION m_crSectionChan;	// доступ к каналу

	CHANNEL_FOR_KIT();
	~CHANNEL_FOR_KIT();
	CHANNEL_FOR_KIT(const CHANNEL_FOR_KIT & val);
	CHANNEL_FOR_KIT & operator = (const CHANNEL_FOR_KIT & val);
	CHANNEL_FOR_KIT & operator = (const CHANNEL_STRUCT & val);
	
	// Задание размера буфера чтения по интервалу времени.
	// Выполняется проверка на min и max значения.
	bool ReSize(const DWORD sizeBuffer);
	bool ReSize(const double dIntervalBuffer);
	bool ReSize(const CZetTimeSpan & ztsIntervalBuffer);

	// Первоначальная установка флагов после задания канала
	void InitFlags();

	// Чтение флага
	bool GetFlag(uint32_t flag);

	// Установка флага булевским значением
	void SetFlag(uint32_t flag, bool val);

	// Установка флага в 1
	void PlaceFlag(uint32_t flag);

	// Сброс флага в 0
	void ResetFlag(uint32_t flag);
	
	// Проверка значений новой структуры и установка соответствующих флагов
	// Возвращает true если структуры разные
	bool TestChangeStruct(CHANNEL_STRUCT & pstr);

	// Если установлен флаг изменения статуса, то переопределяется флаг
	// существования канала и флаг изменения статуса сбрасывается
	void DoChangeStatus();

	// Анализ статуса канала на предмет отключения
	// true если канал отключен
	bool IsDisabled();

	// Анализ статуса канала на предмет работоспособности канала
	bool ChannelIsOk();
};
//------------------------------------------------------------------------------