//------------------------------------------------------------------------------
//	Файл AmplRatio.h
//------------------------------------------------------------------------------
/*		Класс, реализующий в СКСВ определение ложности событий с помощью
анализа отношения амплитуд пиковых значений по компонентам
*/
//------------------------------------------------------------------------------
#pragma once
#include <Buffer\SmartArray.h>
#include <Seismo\DetectorSSEC/Classification/0_Custom/CustomFalseEvent.h>
#include <Seismo\DetectorSSEC/Classification/AmplRatio\Struct_CFG\Ssm_cfg_Algr_AmplRatioProxy.h>
//------------------------------------------------------------------------------
namespace zet {
namespace algorithm {
namespace dsp {
namespace seismo {
//------------------------------------------------------------------------------
class CAmplRatio : public CCustomFalseEvent
{
private:
	SmartArray<float> m_saTmp;
	Ssm_cfg_Algr_AmplRatio m_config;
	float CalcMaxRatio(const float val1, const float val2, const float val3);

protected:
public:
	CAmplRatio();
	CAmplRatio(const CAmplRatio & val);
	virtual ~CAmplRatio() {}
	void operator = (const CAmplRatio & val);

	Ssm_cfg_Algr_AmplRatio_ns::Proxy m_cfgClass;

	virtual long SetConfigFromProxy(const DATE timeSRV);
	virtual long SetConfig(ZetHeader* pParam, const DATE timeSRV);
	virtual long FalseEvent(const float *pDataX, const float *pDataY,
		const float *pDataZ, const DWORD number);
};
//------------------------------------------------------------------------------
}
}
}
}
//------------------------------------------------------------------------------