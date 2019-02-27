//------------------------------------------------------------------------------
//	Файл AlgrStation_Accel.h
//------------------------------------------------------------------------------
/*		Класс, реализующий в СКСВ определение ложности событий сейсмостанции по
с помощью анализа пиковых значений ускоренирй от датчиков
*/
//------------------------------------------------------------------------------
#pragma once
#include <Seismo\StationSSEC/Classification/0_Custom/CustomFalseEventStation.h>
#include <Seismo\StationSSEC/Classification/Accel/Struct_CFG\Ssm_cfg_AlgrStation_AccelProxy.h>
//------------------------------------------------------------------------------
namespace zet {
namespace algorithm {
namespace dsp {
namespace seismo {
//------------------------------------------------------------------------------
class CFalseEventStationAccel : public CCustomFalseEventStation
{
private:
	Ssm_cfg_AlgrStation_Accel m_config;

protected:
public:
	CFalseEventStationAccel();
	CFalseEventStationAccel(const CFalseEventStationAccel & val);
	virtual ~CFalseEventStationAccel() {}
	void operator = (const CFalseEventStationAccel & val);

	Ssm_cfg_AlgrStation_Accel_ns::Proxy m_cfgClass;

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