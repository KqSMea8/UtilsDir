//------------------------------------------------------------------------------
//	Файл KitOfChannels.h
//	Зеленоград, ООО "ЭТМС", Савостин Ю.М.
//------------------------------------------------------------------------------
//		Класс предназначен для чтения и обработки данных
// коллекции комплектов каналов.
//		Любой комплект может содержать произвольное кол-во каналов. Внутри
// каждого комплекта выполняется синхронное чтение данных с каналов этого
// комплекта. Время чтения данных в каждом комплекте своё.
//		Коллекция может содержать произвольное кол-во комплектов.
//------------------------------------------------------------------------------
#pragma once

#include "ChannelForKit.h"
#include <vector>
#include "DataTimeZT.h"
#include <ZETView_basic_class\CDSRV.h>
//------------------------------------------------------------------------------
// Флаги комплекта каналов. Всего флагов 32, из них используются 4.

#define Flag_Kit_Channes_IsOk	0x1		// 1 - в комплекте есть рабочие каналы
#define Flag_Kit_Channes_AllOk	0x2		// 2 - в комплекте все каналы рабочие
#define Flag_Kit_Read 			0x4		// 3 - разрешение чтения данных
#define Flag_Kit_Change_Status	0x8		// 4 - изменился статус какого- то канала
// #define Flag_Kit_			0x10	// 5 - 
// #define Flag_Kit_			0x20	// 6 - 
// #define Flag_Kit_			0x40	// 7 - 
// #define Flag_Kit_			0x80	// 8 - 
// #define Flag_Kit_			0x100	// 9 - 
// #define Flag_Kit_			0x200	// 10 - 
// #define Flag_Kit_			0x400	// 11 - 
//------------------------------------------------------------------------------
#define Factor_Chan_Stop_Time	10.		// число типа double
//------------------------------------------------------------------------------
// Структура параметров комплекта каналов.
struct STRUCT_KIT_OF_CHANNEL
{ 
	Flags<uint32_t> m_bFlags;	// 32 флага комплекта каналов
	HANDLE m_hEventSynchro;		// синхронизирующее событие с автосбросом и с сигнальным (свободным) состоянием

	DWORD m_numberChannels;     // кол-во каналов в контейнере (размер вектора каналов)
	DWORD m_numberChannelsWork;	// кол-во рабочих каналов в контейнере
	DWORD m_numberReadOk;		// кол-во каналов с успешным чтением
	DWORD dw1;

	std::vector<CHANNEL_FOR_KIT> m_vectChannels;
	CZetTime m_zt_lastTimeReadComp;		// последнее время чтения данных по компьютеру
	CZetTimeSpan m_zts_timeDataCur;		// время текущих прочитанных данных
	CDataTimeZT m_cTimeRead;			// время чтения
	CRITICAL_SECTION m_crSectinKit;		// доступ к контейнеру

	STRUCT_KIT_OF_CHANNEL();
	~STRUCT_KIT_OF_CHANNEL();
	STRUCT_KIT_OF_CHANNEL(const STRUCT_KIT_OF_CHANNEL & val);
	STRUCT_KIT_OF_CHANNEL & operator = (const STRUCT_KIT_OF_CHANNEL & val);

	// Чтение флага
	bool GetFlag(uint32_t flag);

	// Установка флага булевским значением
	void SetFlag(uint32_t flag, bool val);

	// Установка флага в 1
	void PlaceFlag(uint32_t flag);

	// Сброс флага в 0
	void ResetFlag(uint32_t flag);

	// Получение кол-ва рабочих каналов в комплекте
	DWORD GetNumChannelsWork();

	// Задание размера буферов для чтения данных в соответствии с интервалом
	// времени из CDataTimeZT. Выполняется проверка на min и max значения.
	// Возвращаемое значение - новые размеры (с учетом проверки) установлены.
	bool ReSize();

	// получение минимального SRV-времени по комплекту и
	// проверка движения времени по каналам
	double GetTimeMin(CDSRV * psrv, const bool bNeed7000);

	// Задание времени первого чтения
	bool SetTimeStart(CDSRV * psrv, const bool bNeed7000);
	bool SetTimeStart(const double time);

	// Проверка готовности синхронного чтения данных с
	// проверкой движения времени по каналам
	bool ReadinessData(CDSRV * psrv, const bool bNeed7000);

	// Синхронное чтение данных с каналов комплекта.
    // Если было чтение, то в m_zts_timeDataCur записывается время
    // прочитанных данных и инкрементируется время чтения
	// Возвращает флаг - чтение было/не было.
	bool GetData(CDSRV * psrv, const bool bNeed7000);

	// Проверка параметров каналов на предмет их изменения.
	// Если что-то поменялось, то возрашается true.
	bool Modify(CDSRV * psrv, CHANNEL_STRUCT & tmp, const double FriqMin);

	// Установка синхронизирующего события контейнера
	void SetSynchro();

	// Определение флагов контейнера
	void DefineFlagsOfKit();
	
	// Установка параметров времени контейнера
	void SetTimeParam(const CZetTimeSpan & timeInternalRead, const CZetTimeSpan & timeInternalBuffer);
    
	// Сброс флагов Flag_Change_Freq у всех каналов контейнера
	void ResetAllFlagChangeFreq();

	// Задание флагов Flag_Change_Freq у всех каналов контейнера
	void SetAllFlagChangeFreq(bool val);

	// Время, оставшееся до очередного усреднения, сек
	double GetTimeBeforeNextAver(CDSRV * psrv, const bool bNeed7000);
	// Время, оставшееся до очередного чтения, сек
	double GetTimeBeforeNextRead(CDSRV * psrv, const bool bNeed7000);
};
//------------------------------------------------------------------------------