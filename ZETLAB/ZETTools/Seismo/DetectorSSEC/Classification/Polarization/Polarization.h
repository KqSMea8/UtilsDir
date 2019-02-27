//------------------------------------------------------------------------------
//	‘айл FalseEventPolarization.h
//------------------------------------------------------------------------------
/*		 ласс, реализующий в — —¬ определение ложности событий с помощью
пол€ризационного анализа
*/
//------------------------------------------------------------------------------
#pragma once
#include <Seismo\DetectorSSEC/Classification/0_Custom/CustomFalseEvent.h>
#include "Seismo\DetectorSSEC/Classification\Polarization\Struct_CFG\Ssm_cfg_Algr_PolarProxy.h"
#include <Buffer\SmartArray.h>
//------------------------------------------------------------------------------
namespace zet {
namespace algorithm {
namespace dsp {
namespace seismo {
//------------------------------------------------------------------------------
class CFalseEventPolarization : public CCustomFalseEvent
{
private:
	int m_numAverageArray;
	int m_numAverageSignal;

	Ssm_cfg_Algr_Polar m_config;

	// промежуточные массивы дл€ расчета пол€ризационной матрицы, размер size
	SmartArray<double> m_saDouble[3];
	SmartArray<float> m_saMaxAxis;		// размер массива number
	SmartArray<float> m_saTmp;			// размер массива number

	bool CalcOnePoint(const float *pDataX, const float *pDataY, const float *pDataZ,
		const int size, float &maxAxis);

protected:
public:
	CFalseEventPolarization();
	CFalseEventPolarization(const CFalseEventPolarization &val);
	virtual ~CFalseEventPolarization();
	void operator = (const CFalseEventPolarization &val);

	Ssm_cfg_Algr_Polar_ns::Proxy m_cfgClass;

	float* GetPointerForMaxAxis() { return m_saMaxAxis.pData; }
	//int GetNumZero() { return m_numZero; }
	float m_numZeroPersent;

	bool FiltrationSignal();

	virtual long SetConfigFromProxy(const DATE timeSRV);
	virtual long SetConfig(ZetHeader* pParam, const DATE timeSRV);
	virtual long FalseEvent(const float *pDataX, const float *pDataY,
		const float *pDataZ, const DWORD number);
};
//------------------------------------------------------------------------------
}//namespace seismo
}//namespace dsp
}//namespace algorithm
}//namespace zet
//------------------------------------------------------------------------------