//------------------------------------------------------------------------------
//	Файл CustomFalseEvent.h
//------------------------------------------------------------------------------
/*		Базовый класс для классов, реализующих в СКСВ детектирование
ложных событий. Создавать экземпляры этого класса нецелесообразно.
*/
//------------------------------------------------------------------------------
#pragma once
#include <Seismo\CustomAlgorithm/CustomAlgorithm.h>
#include <Seismo\CustomAlgorithm\Struct_Res\Ssm_res_algr_Custom.zschema1.h>
using namespace zet;
//namespace ssm = algorithm::dsp::seismo;
//------------------------------------------------------------------------------
namespace zet {
namespace algorithm	{
namespace dsp {
namespace seismo {
//------------------------------------------------------------------------------
class CCustomFalseEvent : public CCustomAlgorithm
{
private:

protected:
	int m_sizePreHistory;
	float m_weight;
	double m_freqADCdouble;
	Ssm_res_Algr_Custom m_result;
	long DataStamp(const DATE timeSRV = 0.);

public:
	CCustomFalseEvent();
	CCustomFalseEvent(const CCustomFalseEvent & val);
	virtual ~CCustomFalseEvent() {}
	void operator = (const CCustomFalseEvent & val);

	bool m_bOnOff;
	bool m_bOriginalSignal;

	Ssm_type_Event_Algr m_type_Event_Algr = tea_Event;

	// параметры класса
	float GetWeigth() { return m_weight; }
	long SetWeigth(float val);
	long SetFreqADC(double freqADC);
	long SetSizePreHistory(int val);

	// методы класса ----------------------------
	long Calculation(const float *pData, const DWORD number, const DATE timeSRV = 0.);
	long Calculation(const float *pDataX, const float *pDataY,
		const float *pDataZ, const DWORD number, const DATE timeSRV = 0.);
	void ClearStructResult();
	bool IsEvent();


	virtual long SetConfigFromProxy(const DATE timeSRV) { return ErrorCodes::FatalError; }
	virtual long FalseEvent(const float *pData, const DWORD number)
	{ return ErrorCodes::Err_Initialization; }
	virtual long FalseEvent(const float *pDataX, const float *pDataY,
		const float *pDataZ, const DWORD number)
	{ return ErrorCodes::Err_Initialization; }

	virtual long ReStart() { return ErrorCodes::NoError; }
};
//------------------------------------------------------------------------------
}//namespace seismo
}//namespace dsp
}//namespace algorithm
}//namespace zet
//------------------------------------------------------------------------------