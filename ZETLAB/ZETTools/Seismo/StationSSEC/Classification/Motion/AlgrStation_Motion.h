//------------------------------------------------------------------------------
//	Файл AlgrStation_Motion.h
//------------------------------------------------------------------------------
/*		Класс, реализующий в СКСВ определение ложности событий сейсмостанции по
с помощью анализа пиковых значений перемещения от датчиков
*/
//------------------------------------------------------------------------------
#pragma once
#include <Seismo\StationSSEC/Classification/0_Custom/CustomFalseEventStation.h>
#include <Seismo\StationSSEC/Classification/Motion/Struct_CFG\Ssm_cfg_AlgrStation_MotionProxy.h>
//------------------------------------------------------------------------------
namespace zet {
namespace algorithm {
namespace dsp {
namespace seismo {
//------------------------------------------------------------------------------
class CFalseEventStationMotion : public CCustomFalseEventStation
{
private:
	Ssm_cfg_AlgrStation_Motion m_config;

protected:
public:
	CFalseEventStationMotion();
	CFalseEventStationMotion(const CFalseEventStationMotion & val);
	virtual ~CFalseEventStationMotion() {}
	void operator = (const CFalseEventStationMotion & val);

	Ssm_cfg_AlgrStation_Motion_ns::Proxy m_cfgClass;

	virtual long SetConfigFromProxy(const DATE timeSRV);
	virtual long SetConfig(ZetHeader* pParam, const DATE timeSRV = 0);
	virtual long Calculation(std::list<Ssm_str_EventParam> & listEvent);
};
//------------------------------------------------------------------------------
}
}
}
}
//------------------------------------------------------------------------------