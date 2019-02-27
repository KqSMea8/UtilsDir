//------------------------------------------------------------------------------
//	Файл BroadBandNoise.h
//------------------------------------------------------------------------------
/*		Класс, реализующий в СКСВ определение ложности событий с помощью
анализа отношения энергии сигнала на низких и высоких частотах
*/
//------------------------------------------------------------------------------
#pragma once
#include <Buffer\SmartArray.h>
#include <Seismo\DetectorSSEC/Classification/0_Custom/CustomFalseEvent.h>
#include <Seismo\DetectorSSEC/Classification\BroadBandNoise\Struct_CFG\Ssm_cfg_Algr_BroadBandNoiseProxy.h>
#include <ippFiltrDouble\IirFiltrBandDouble.h>
#include <Algorithms\SomeFeatures.h>
//------------------------------------------------------------------------------
namespace zet {
namespace algorithm {
namespace dsp {
namespace seismo {
//------------------------------------------------------------------------------
class CBroadBandNoise : public CCustomFalseEvent
{
private:
	SmartArray<double> m_saDouble;		// массив для фильтра
	SmartArray<float> m_saXYZE[4];		// отфильтрованные данные X, Y, Z, Ampl
	CIirFiltrInternalDouble *m_pFiltrHighPass[3];	// фильтры для X, Y, Z
	CIirFiltrBandDouble *m_pFiltrBandPass[3];

	Ssm_cfg_Algr_BroadBandNoise m_config;

	void Equal(const CBroadBandNoise & val);
	bool CalcSignalAlgoritm(
		const float *pDataX, const float *pDataY, const float *pDataZ,
		const int size, float & signal);

protected:
public:
	CBroadBandNoise();
	CBroadBandNoise(const CBroadBandNoise & val);
	virtual ~CBroadBandNoise();
	void operator = (const CBroadBandNoise & val);

	Ssm_cfg_Algr_BroadBandNoise_ns::Proxy m_cfgClass;

	virtual long SetConfigFromProxy(const DATE timeSRV);
	virtual long SetConfig(ZetHeader* pParam, const DATE timeSRV);
	virtual long FalseEvent(const float *pDataX, const float *pDataY,
		const float *pDataZ, const DWORD number);
	virtual long ReStart();
};
//------------------------------------------------------------------------------
}
}
}
}
//------------------------------------------------------------------------------