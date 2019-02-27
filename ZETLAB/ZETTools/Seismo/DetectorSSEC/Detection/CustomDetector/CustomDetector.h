//--------------------------------------------------------------------------
// Файл CustomDetector.h
//--------------------------------------------------------------------------
/*		Базовый класс детекторов для сейсмических зачач. Поэтому детектор
3-х компонентный (x, y, z), а буферов - 4 (+ амплитуда для порогового
детектора). При этом индекс 0 - это x, 1 - y, 2 - z, 3 - амплитуда.
		Буферы реализованы на классе, в основе которого "умный" массив,
поэтому создаю их статически.
		Детектор предназначен для выдачи временного интервала с некоторым
событием для его классификации по данным этого интервала.
*/
#define MaxSizeBuffer	100000	// 1 000 сек при Fadc = 100 Гц
//--------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <Intel\ipp\ipps.h>	// расчёт SNR
#include <Algorithms\SomeFeatures.h>	// CalcAmpl_32f
#include <Buffer\ShiftBufferArbitraryPage.h>
#include <Seismo/DetectorSSEC/Detection/Struct_CFG/Ssm_cfg_Detector_STAProxy.h>
//--------------------------------------------------------------------------
using namespace zet::configuration::schema;
//------------------------------------------------------------------------------
enum TypeEventYesNo
{
	ted_No,			// события нет
	ted_Start,		// событие началось
	ted_Continue,	// событие наблюдается
	ted_End,		// событие закончилось
	ted_End_SmallTime,	// событие закрыто
	ted_End_LargeTime,	// событие закрыто
};
//--------------------------------------------------------------------------
struct DetectorResult
{
	int sizePage;
	int sizePreHistory;
	int sizeArrays;
//	float SNR;	// SNR - отношение сигнал/шум (СКЗ сигнала/СКЗ шума), дБ
	double timeSRV_lastKadr;
};
//--------------------------------------------------------------------------
class CCustomDetector
{
private:

protected:
	bool m_bInit = false;		// детектор проинициализирован
	bool m_bEvent = false;		// наблюдается событие
	bool m_bInterval = false;	// интервал с событием определён
	int m_counterForInit = 0;	// счётчик инициадизации
	int m_counterInitMax = 0;
	int m_sizePreHistory = 2000;// предыстория: 20 сек при 100 Гц
	int m_sizeEvent = 500;		// размер события: 5 сек при 100 Гц
	float m_threshold = 0.03f;	// порог детектирования
	SmartArray<float> m_saTmpXYZ[3];	// для расчёта амплитуды и др.

	TypeEventYesNo SetTypeEvent(bool bEvent);

public:
	CCustomDetector();
	virtual ~CCustomDetector() {}
	virtual bool Create(Ssm_cfg_Detector_STA_ns::Proxy & proxy);

	TypeEventYesNo m_typeEvent = ted_No;
	float m_peakXYZA[4];
	DetectorResult m_result;
	Ssm_cfg_Detector_STA m_cfg;

	// функции свойств
	bool GetFlagStartMode() { return !m_bInit; }
	bool GetFlagEvent() { return m_bInit && m_bEvent; }
	bool GetFlagReadyInterval() { return m_bInit && m_bInterval; }

	// функции класса
	virtual void ZeroData();
	virtual bool Calculation(
		const float *pDataX, const float *pDataY,
		const float *pDataZ, const int sizePage,
		bool bNeedPeakValue = true);
};
//--------------------------------------------------------------------------