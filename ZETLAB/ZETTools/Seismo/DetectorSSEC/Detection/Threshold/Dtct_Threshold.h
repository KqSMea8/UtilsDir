//--------------------------------------------------------------------------
// Файл Threshold.h
//--------------------------------------------------------------------------
/*		Пороговый детектор.
*/
//--------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <Intel\ipp\ipps.h>
#include <Seismo\DetectorSSEC\Detection\CustomDetector\CustomDetector.h>
//--------------------------------------------------------------------------
class CDetector_Threshold : public CCustomDetector
{
private:
	int m_numPageInEventMax = 5;
	int m_numPageInEventCur = -1;
	SmartArray<float> m_saAmpl;

protected:

public:
	CDetector_Threshold();
	virtual ~CDetector_Threshold() {}
	virtual bool Create(Ssm_cfg_Detector_STA_ns::Proxy & proxy);

	// функции свойств

	// функции класса
	virtual void ZeroDataBuff();

	virtual bool Calculation(
		const float *pDataX, const float *pDataY,
		const float *pDataZ, const int sizePage,
		bool bNeedPeakValue = true);
};
//--------------------------------------------------------------------------