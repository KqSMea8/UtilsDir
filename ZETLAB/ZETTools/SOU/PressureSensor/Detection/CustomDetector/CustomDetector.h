//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------  Файл CustomDetector.h  -----------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
/*	Базовый класс детекторов для задач СОУ. Детектор однокомпонентный, один буфер. Буфер реализованы на классе, в основе 
которого "умный" массив, поэтому создаю их статически.
	Детектор предназначен для выдачи временного интервала с некоторым событием для его классификации по данным этого 
	интервала.
*/
#define MaxSizeBuffer	100000	// 1 000 сек при Fadc = 100 Гц
//----------------------------------------------------------------------------------------------------------------------
#pragma once
#include "Intel\ipp\ipps.h"	// расчёт SNR
#include <Buffer\ShiftBufferArbitraryPage.h>
#include <SOU\PressureSensor\Detection\Struct_CFG\Lds_cfg_Detector_PressureProxy.h>
//----------------------------------------------------------------------------------------------------------------------
using namespace zet::configuration::schema;
//----------------------------------------------------------------------------------------------------------------------
enum TypeEventYesNo
{
	ted_No,			// события нет
	ted_Start,		// событие началось
	ted_Continue,	// событие наблюдается
	ted_End,		// событие закончилось
	ted_End_SmallTime,	// событие закрыто
	ted_End_LargeTime,	// событие закрыто
};
//----------------------------------------------------------------------------------------------------------------------
struct DetectorResult
{
	int sizePage;
	int sizePreHistory;
	int sizeArrays;
	double timeSRV_lastKadr;
};
//--------------------------------------------------------------------------
class CCustomDetectorPressure
{
private:

protected:
	bool m_bInit = false;		// детектор проинициализирован
	bool m_bEvent = false;		// наблюдается событие
	bool m_bInterval = false;	// интервал с событием определён
	//bool m_bExtremum = false;
	int m_counterForInit = 0;	// счётчик инициадизации
	int m_counterInitMax = 0;
	int m_sizePreHistory = 2000;// предыстория: 20 сек при 100 Гц
	int m_sizeEvent = 500;		// размер события: 5 сек при 100 Гц
	float m_threshold = 0.03f;	// порог детектирования
	
	TypeEventYesNo SetTypeEvent(bool bEvent);

public:
	CCustomDetectorPressure();
	virtual ~CCustomDetectorPressure() {}
	virtual bool Create(Lds_cfg_Detector_Pressure_ns::Proxy & proxy);

	TypeEventYesNo m_typeEvent = ted_No;
	DetectorResult m_result;
	Lds_cfg_Detector_Pressure m_cfg;

	// функции свойств
	bool GetFlagStartMode() { return !m_bInit; }
	bool GetFlagEvent() { return m_bInit && m_bEvent; }
	bool GetFlagReadyInterval() { return m_bInit && m_bInterval; }

	// функции класса
	virtual void ZeroData();
	virtual bool Calculation(const float* pData, const int sizePage);
};
//--------------------------------------------------------------------------