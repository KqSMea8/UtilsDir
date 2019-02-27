//------------------------------------------------------------------------------
#pragma once
#include "Buffer\SmartArray.h"
#include <mathZet.h>
#include "ippFiltrDouble\IirFiltrSimpleDouble.h"
#include <Seismo\DetectorSSEC/Classification/0_Custom/CustomFalseEvent.h>
#include "Seismo\DetectorSSEC/Classification\UnipolarClick\Struct_CFG\Ssm_cfg_Algr_UnipolarClickProxy.h"
//------------------------------------------------------------------------------
/*	Алгоритм отсеивания однополярных щелчков

*/
//------------------------------------------------------------------------------
namespace zet {
namespace algorithm {
namespace dsp {
namespace seismo {
//------------------------------------------------------------------------------
class CUnipolarClick : public CCustomFalseEvent
{
private:
	SmartArray<double> m_saDouble;		// массив для фильтра
	SmartArray<float> m_saXYZE[3];		// отфильтрованные данные X, Y, Z
	CIirFiltrSimpleDouble* m_pFiltrDiff[3];	// фильтры для X, Y, Z

	Ssm_cfg_Algr_UnipolarClick m_config;	/**< структура параметров */
	bool CalcOneComponent(float *pData, const int size,
		float &peakPreHistory, float &ratio, CIirFiltrSimpleDouble* pFiltr);

	void Equal(const CUnipolarClick & val);
	bool CalcSignalAlgoritm(
		const float *pDataX, const float *pDataY, const float *pDataZ,
		const int size, float & signal, bool & bFalseEvent);

protected:

public:
	CUnipolarClick();
	CUnipolarClick(const CUnipolarClick & val);
	~CUnipolarClick();
	void operator = (const CUnipolarClick & val);

	Ssm_cfg_Algr_UnipolarClick_ns::Proxy m_cfgClass;

	void SetFreqADC(double *pFreqADC);
	virtual long SetConfigFromProxy(const DATE timeSRV);
	virtual long SetConfig(ZetHeader* pParam, const DATE timeSRV);
	virtual long FalseEvent(const float *pDataX, const float *pDataY,
		const float *pDataZ, const DWORD number);
	virtual long ReStart();
};
//------------------------------------------------------------------------------
}//namespace seismo
}//namespace dsp
}//namespace algorithm
}//namespace zet
//------------------------------------------------------------------------------