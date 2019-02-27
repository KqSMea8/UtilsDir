//------------------------------------------------------------------------------
#pragma once
#include "Buffer\SmartArray.h"
#include <mathZet.h>
#include "Seismo\FalseEvent\0_Custom\CustomFalseEvent.h"
#include "Seismo\FalseEvent\UnipolarSignal\Struct_CFG\Ssm_cfg_Algr_UnipolarSignalProxy.h"
#include "Seismo\FalseEvent\UnipolarSignal\Struct_Dia\Ssm_dia_Algr_UnipolarSignal.zschema1.h"
//------------------------------------------------------------------------------
/*	Алгоритм отсеивания однополярных сигналов

*/
//------------------------------------------------------------------------------
namespace zet {
namespace algorithm {
namespace dsp {
namespace seismo {
//------------------------------------------------------------------------------
class CUnipolarSignal : public CCustomFalseEvent
{
private:
	SmartArray<float> m_saHist;		/**< гистограмма */
	Ssm_cfg_Algr_UnipolarSignal m_config;	/**< структура параметров */
	Ssm_dia_Algr_UnipolarSignal m_diagns;	/**< структура диагностики */
	bool CalcOneComponent(const float *pData, const int size,
		float &mean, float &deltaMean,
		float &valueDoun, float &valueUp, float &ratio);

protected:

public:
	CUnipolarSignal();
	CUnipolarSignal(const CUnipolarSignal & val);
	~CUnipolarSignal();
	void operator = (const CUnipolarSignal & val);

	Ssm_cfg_Algr_UnipolarSignal_ns::Proxy m_cfgClass;

	virtual ErrorCode SetConfigFromProxy();
	virtual ErrorCode SetConfig(ZetHeader* pParam);
	virtual ErrorCode FalseEvent(const float *pDataX, const float *pDataY,
		const float *pDataZ, const DWORD number);
//	virtual ErrorCode ReStart();

};
//------------------------------------------------------------------------------
}//namespace seismo
}//namespace dsp
}//namespace algorithm
}//namespace zet
//------------------------------------------------------------------------------