//------------------------------------------------------------------------------
#pragma once
#include "Buffer\SmartArray.h"
#include <mathZet.h>
#include "Seismo\FalseEvent\0_Custom\CustomFalseEvent.h"
#include "Seismo\FalseEvent\Spurt\Struct_CFG\Ssm_cfg_Algr_SpurtProxy.h"
#include "Seismo\FalseEvent\Spurt\Struct_Dia\Ssm_dia_Algr_Spurt.zschema1.h"
//------------------------------------------------------------------------------
/*	Алгоритм отсеивания однополярных щелчков

*/
//------------------------------------------------------------------------------
namespace zet {
namespace algorithm {
namespace dsp {
namespace seismo {
//------------------------------------------------------------------------------
class CSpurt : public CCustomFalseEvent
{
private:
	Ssm_cfg_Algr_Spurt m_config;	/**< структура параметров */
	Ssm_dia_Algr_Spurt m_diagns;	/**< структура диагностики */
	void CalcOneComponent(const float *pData, const int size,
		float &peakPreHistory, float &maxAll, float &ratio);

protected:

public:
	CSpurt();
	CSpurt(const CSpurt & val);
	~CSpurt();
	void operator = (const CSpurt & val);

	Ssm_cfg_Algr_Spurt_ns::Proxy m_cfgClass;

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