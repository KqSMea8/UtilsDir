//------------------------------------------------------------------------------
//	Файл FalseEventCorrelation.h
//------------------------------------------------------------------------------
/*	Класс, реализующий в СКСВ алгоритм определения ложности событий вызванных 
электирческими или физическими факторами и дающих синхронных сигнал по каналам
Определяется с помощью корреляции
	Алгоритм применяется однократно на выделенном массиве
	Удельный вес решения алгоритма = 0.5
*/
//------------------------------------------------------------------------------
#pragma once
#include <Buffer\SmartArray.h>
#include <Seismo\DetectorSSEC/Classification/0_Custom/CustomFalseEvent.h>
#include <Seismo\DetectorSSEC/Classification\Correlation\Struct_CFG\Ssm_cfg_Algr_CorrelationProxy.h>
//------------------------------------------------------------------------------
namespace zet {
namespace algorithm {
namespace dsp {
namespace seismo {
//------------------------------------------------------------------------------
class CFalseEventCorrelation : public CCustomFalseEvent
{
private:
	SmartArray<float> m_sa1;
	SmartArray<float> m_sa2;
	SmartArray<float> m_sa3;
	Ssm_cfg_Algr_Correlation m_config;

	bool Correlation(const float* data1, const float* data2,
		float &corr, const int size);
	bool Calculation(const float *pDataX, const float *pDataY,
		const float *pDataZ, const int size, float & corr);

protected:
public:
	CFalseEventCorrelation();
	CFalseEventCorrelation(const CFalseEventCorrelation &val);
	virtual ~CFalseEventCorrelation();
	void operator = (const CFalseEventCorrelation &val);
	
	Ssm_cfg_Algr_Correlation_ns::Proxy m_cfgClass;

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