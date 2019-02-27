//------------------------------------------------------------------------------
//	Файл FalseEventWavelet.h
//------------------------------------------------------------------------------
/*	Класс, реализующий в СКСВ алгоритм определения ложности событий с помощью
вейвлет-анализа.
Определяется с помощью корреляции
	Алгоритм применяется однократно на выделенном массиве.
	Удельный вес решения алгоритма = 1
*/
//------------------------------------------------------------------------------
#pragma once
#include <Buffer\SmartArray.h>
#include <Seismo\DetectorSSEC/Classification\0_Custom\CustomFalseEvent.h>
#include <Seismo\DetectorSSEC/Classification\Wavelet\Struct_CFG\Ssm_cfg_Algr_WaveletProxy.h>
//------------------------------------------------------------------------------
namespace zet {
namespace algorithm {
namespace dsp {
namespace seismo {
//------------------------------------------------------------------------------
class CFalseEventWavelet : public CCustomFalseEvent
{
private:
	int nTp;
	SmartArray<float> m_saXYZ[3];
	Ssm_cfg_Algr_Wavelet m_config;
	
protected:
public:
	CFalseEventWavelet();
	CFalseEventWavelet(const CFalseEventWavelet &val);
	virtual ~CFalseEventWavelet();
	void operator = (const CFalseEventWavelet &val);
	
	Ssm_cfg_Algr_Wavelet_ns::Proxy m_cfgClass;

	int GetnTp() { return nTp; }
	float* GetPointerDataX() { return m_saXYZ[0].pData; }
	float* GetPointerDataY() { return m_saXYZ[1].pData; }
	float* GetPointerDataZ() { return m_saXYZ[2].pData; }
	bool FiltrFDWT(
		const float *pDataX, const float *pDataY, const float *pDataZ,
		int size, int sizePreHistory, int * pnT_start);

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