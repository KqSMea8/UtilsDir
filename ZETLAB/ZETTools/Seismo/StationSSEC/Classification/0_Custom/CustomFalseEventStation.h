//------------------------------------------------------------------------------
//	Файл CustomFalseEventStation.h
//------------------------------------------------------------------------------
/*		Базовый класс для классов, реализующих в СКСВ детектирование
ложных событий сейсмостанции. Создавать экземпляры этого класса нецелесообразно.
*/
//------------------------------------------------------------------------------
#pragma once
#include <list>
#include <vector>
#include <Seismo\Seismo.h>
#include <Seismo\DetectorSSEC\Struct_EventParam\Ssm_str_EventParam.zschema1.h>
#include <Seismo\CustomAlgorithm/CustomAlgorithm.h>
#include <Seismo\CustomAlgorithm\Struct_Res\Ssm_res_algr_Custom.zschema1.h>
using namespace zet;
//------------------------------------------------------------------------------
namespace zet {
namespace algorithm	{
namespace dsp {
namespace seismo {
//------------------------------------------------------------------------------
class CCustomFalseEventStation : public CCustomAlgorithm
{
private:

protected:
	Ssm_res_Algr_Custom m_result;

	long CalcRatios();
	std::vector<float> m_vectData;

public:
	CCustomFalseEventStation();
	CCustomFalseEventStation(const CCustomFalseEventStation & val);
	virtual ~CCustomFalseEventStation() {}
	void operator = (const CCustomFalseEventStation & val);

	bool m_bOnOff = true;
	Ssm_type_Event_Algr m_type_Event_Algr = tea_Event;

	// параметры класса

	// методы класса ----------------------------
	virtual long Calculation(std::list<Ssm_str_EventParam> & listEvent)
	{ return ErrorCodes::FatalError; }
	void ClearStructResult();
	bool IsEvent() {
		return m_result.result != 0; }

	virtual long SetConfigFromProxy(const DATE timeSRV) { return ErrorCodes::FatalError; }
//	virtual long ReStart() { return ErrorCodes::NoError; }
};
//------------------------------------------------------------------------------
}//namespace seismo
}//namespace dsp
}//namespace algorithm
}//namespace zet
//------------------------------------------------------------------------------