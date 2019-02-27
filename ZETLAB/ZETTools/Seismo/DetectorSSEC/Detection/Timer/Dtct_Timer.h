//--------------------------------------------------------------------------
// Файл Timer.h
//--------------------------------------------------------------------------
/*		Детектор по таймеру.
*/
//--------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <Seismo\DetectorSSEC\Detection\CustomDetector\CustomDetector.h>
//--------------------------------------------------------------------------
class CDetector_Timer : public CCustomDetector
{
private:
	SmartArray<float> m_saAmpl;

protected:

public:
	CDetector_Timer();
	virtual ~CDetector_Timer() {}
	virtual bool Create(Ssm_cfg_Detector_STA_ns::Proxy & proxy);

	// функции свойств

	// функции класса

	virtual bool Calculation(
		const float *pDataX, const float *pDataY,
		const float *pDataZ, const int sizePage,
		bool bNeedPeakValue = true);
};
//--------------------------------------------------------------------------