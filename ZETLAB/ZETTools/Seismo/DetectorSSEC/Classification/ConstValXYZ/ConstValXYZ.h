//------------------------------------------------------------------------------
//	Файл AmplRatio.h
//------------------------------------------------------------------------------
/*		Класс, реализующий в СКСВ классификацию ложности событий с помощью
анализа постоянной составляющей по компонентам
*/
//------------------------------------------------------------------------------
#pragma once
#include <Seismo\FalseEvent\0_Custom\CustomFalseEvent.h>
#include <Seismo\FalseEvent\ConstValXYZ\Struct_CFG\Ssm_cfg_Algr_ConstValXYZProxy.h>
#include <Seismo\FalseEvent\ConstValXYZ\Struct_Dia\Ssm_dia_algr_ConstValXYZ.zschema1.h>
//------------------------------------------------------------------------------
namespace zet {
namespace algorithm {
namespace dsp {
namespace seismo {
//------------------------------------------------------------------------------
class CConstValXYZ : public CCustomFalseEvent
{
private:
	Ssm_cfg_Algr_ConstValXYZ m_config;
	Ssm_dia_Algr_ConstValXYZ m_diagns;
	bool CalcSignalAlgoritm(
		const float *pDataX, const float *pDataY, const float *pDataZ,
		const int size, float & signal);

protected:
public:
	CConstValXYZ();
	CConstValXYZ(const CConstValXYZ & val);
	virtual ~CConstValXYZ() {}
	void operator = (const CConstValXYZ & val);

	Ssm_cfg_Algr_ConstValXYZ_ns::Proxy m_cfgClass;

	virtual ErrorCode SetConfigFromProxy();
	virtual ErrorCode SetConfig(ZetHeader* pParam);
	virtual ErrorCode FalseEvent(const float *pDataX, const float *pDataY,
		const float *pDataZ, const DWORD number);
};
//------------------------------------------------------------------------------
}
}
}
}
//------------------------------------------------------------------------------