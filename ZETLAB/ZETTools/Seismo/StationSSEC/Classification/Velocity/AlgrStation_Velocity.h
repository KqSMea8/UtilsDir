//------------------------------------------------------------------------------
//	Файл AlgrStation_Accel.h
//------------------------------------------------------------------------------
/*		Класс, реализующий в СКСВ определение ложности событий сейсмостанции по
с помощью анализа пиковых значений ускоренирй от датчиков
*/
//------------------------------------------------------------------------------
#pragma once
#include <Seismo\StationSSEC/Classification/0_Custom/CustomFalseEventStation.h>
#include <Seismo\StationSSEC/Classification/Velocity/Struct_CFG\Ssm_cfg_AlgrStation_VelocityProxy.h>
//------------------------------------------------------------------------------
namespace zet {
namespace algorithm {
namespace dsp {
namespace seismo {
//------------------------------------------------------------------------------
class CFalseEventStationVelocity : public CCustomFalseEventStation
{
private:
	Ssm_cfg_AlgrStation_Velocity m_config;

protected:
public:
	CFalseEventStationVelocity();
	CFalseEventStationVelocity(const CFalseEventStationVelocity & val);
	virtual ~CFalseEventStationVelocity() {}
	void operator = (const CFalseEventStationVelocity & val);

	Ssm_cfg_AlgrStation_Velocity_ns::Proxy m_cfgClass;

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