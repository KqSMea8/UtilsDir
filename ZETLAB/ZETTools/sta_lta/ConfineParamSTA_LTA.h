//--------------------------------------------------------------------------
// Файл ConfineParamSTA_LTA.h
// Версия от 24.09.2012
// ЗАО "ЭТМС", Зеленоград, Савостин Ю.М.
//--------------------------------------------------------------------------
//	Класс предназначен для учёта ограничений, накладываемых на параметры
// детектора STA_LTA. Экземпляры класса используются в программе
// "Детектор STA_LTA" и в соответствующем компоненте SCADA
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <ippFiltr\IirFiltrBand.h>
//--------------------------------------------------------------------------
class CConfineParamSTA_LTA
{
private:
	float *pFreqADC;
	int SizeMax;
	CIirFiltrBand *pFiltr;
	bool bFiltrInternal;
	float PriorValueSTA(float newVal, float valLTA);
	float PriorValueLTA(float newVal, float valSTA);
protected:

public:
	CConfineParamSTA_LTA();
	virtual ~CConfineParamSTA_LTA();
	bool Create(float *freqADC, int sizeMax, CIirFiltrBand *pfiltr = NULL);

	// мин. значение отношения LTA к STA (=10)
	float m_min_Div_LTA_STA;
	// макс. значение отношения суммы LTA и STA к длит. буфера (=0.9)
	float m_max_Div_Sum_Tall;
	// мин. значение STA в отсчётах АЦП (=10)
	float m_min_Div_Sta_TimeADC;
	// мин. порог детектирования (=1.5)
	float m_minThreshold;
	// макс. порог детектирования (=60)
	float m_maxThreshold;
	// мин. и  макс. значения коэффицента сжатия
	DWORD m_minCompressRatio;		// = 400
	DWORD m_maxCompressRatio;		// = 4000

	// функции класса
	float GetMinFmin() { return pFiltr->PriorValueFmin(1e-7f); }
	void NewFreqBand(float *pFmin, float *pFmax,
		float newFmin, float newFmax, short bNewParam);

	float GetMinSTA();
	float GetMaxSTA();
	float GetMinLTA();
	float GetMaxLTA();
	void NewDuration(float *pSta,  float *pLta,
					 float newSta, float newLta);

	void NewThreshold(float *pThreshold, float newThreshold);
	void NewCompressRatio(DWORD *pCompressRatio, DWORD newVal);
};
//--------------------------------------------------------------------------