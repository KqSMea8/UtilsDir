//--------------------------------------------------------------------------
// Файл STA_LTA.h
//--------------------------------------------------------------------------
/*		Детектор STA_LTA 3-х компонентного датчика.
Состоит из 3-х детекторов STA_LTA (на каждую компоненту) и общего
детектора.
*/
//--------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <Algorithms\MovingAverage.h>
#include <Seismo\DetectorSSEC\Detection\CustomDetector\CustomDetector.h>
#include <Seismo\DetectorSSEC\Detection\STA_LTA\Struct_Res\Ssm_res_Detector_STA.zschema1.h>
//--------------------------------------------------------------------------
struct Param_STA_LTA_XYZA
{
	double m_valSTA = 0.;	// сумма значений в STA
	double m_valLTA = 0.;	// сумма значений в LTA

	float m_lastStaOld = 0.f;
	float m_lastStaNew = 0.f;
	float m_lastLtaOld = 0.f;
	float m_lastLtaNew = 0.f;

	// сдвиговый буфер для хранения исходных данных компоненты по модулю
	CShiftBufferArbitraryPage<float> m_buff_data;
	// сдвиговый буфер для хранения сигнала детектора STA/LTA компоненты
	CShiftBufferArbitraryPage<float> m_buff_StaLta;

	Param_STA_LTA_XYZA()
		: m_valSTA(0.)
		, m_valLTA(0.)
	{}

	bool NewData(const float *pData, const int sizePage,
		const int numSTA, const int numLTA,
		const double normFactor, const bool bInit);
	void SetSize(int size);
	void Zero();
};
//--------------------------------------------------------------------------

class CDetector_STA_LTA : public CCustomDetector
{
private:
	int m_numSTA = 1000;	// 10 сек при Fadc = 100 Гц
	int m_numLTA = 19000;	// 190 сек при Fadc = 100 Гц
	int m_sizeEventMin = 1000;
	float m_envelope = 0.f;	// величина кумулятивной огибающей
	float m_alfa;
	float m_1_alfa;
	double m_normFactor = 19.;// = m_numLTA / m_numSTA
	CMovingAverage<float> m_maStDev;
	//	Массив Param_STA_LTA_XYZA[4]:
	// 0, 1, 2: компоненты X, Y, Z
	// 3: buff_data - буфер амплитуды, buff_StaLta - буфер самого детектора
	Param_STA_LTA_XYZA m_buffXYZA[4];
	CShiftBufferArbitraryPage<float> m_buff_StDev;
	
protected:

public:
	CDetector_STA_LTA();
	virtual ~CDetector_STA_LTA();
	virtual bool Create(Ssm_cfg_Detector_STA_ns::Proxy & proxy);

	bool m_bChanX = true;
	bool m_bChanY = true;
	bool m_bChanZ = true;
	Ssm_res_Detector_STA m_resSTA;

	// функции свойств

	// функции класса
	float* GetPointerForDataStaLta();
	int GetSizeForDataStaLta();
	float* GetPointerForDataAmpl();
	float* GetPointerForDataStDev();

	virtual void ZeroData();
	virtual bool Calculation(
		const float *pDataX, const float *pDataY,
		const float *pDataZ, const int sizePage,
		bool bNeedPeakValue = true);
};
//--------------------------------------------------------------------------