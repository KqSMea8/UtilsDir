//------------------------------------------------------------------------------
//	Τΰιλ CustomFalseEventStation.cpp
//------------------------------------------------------------------------------
#ifndef _WIN32_WINNT
#ifndef _WIN32_WINNT_MAXVER
#include <winsdkver.h>
#endif
#define _WIN32_WINNT _WIN32_WINNT_MAXVER
#endif
//------------------------------------------------------------------------------
#include <mathZet.h>
#include <Seismo/StationSSEC/Classification/0_Custom/CustomFalseEventStation.h>
//------------------------------------------------------------------------------
namespace zet {
namespace algorithm {
namespace dsp {
namespace seismo {
//------------------------------------------------------------------------------
CCustomFalseEventStation::CCustomFalseEventStation() : CCustomAlgorithm()
	, m_bOnOff(true)
	, m_type_Event_Algr(tea_Event)
{
	m_pResult = &m_result.zet_header;
	long ret = ztOpenStructure(m_pResult,
		ZETLAB_SSM_RES_ALGR_CUSTOM_TYPE, ZETLAB_SSM_RES_ALGR_CUSTOM_SIZE);
}
//------------------------------------------------------------------------------
CCustomFalseEventStation::CCustomFalseEventStation(const CCustomFalseEventStation & val) :
	CCustomAlgorithm(val)
{
	m_bOnOff = val.m_bOnOff;
	m_type_Event_Algr = val.m_type_Event_Algr;

	m_result = val.m_result;
	m_pResult = &m_result.zet_header;
}
//------------------------------------------------------------------------------
void CCustomFalseEventStation::operator = (const CCustomFalseEventStation & val)
{
	*((CCustomAlgorithm*)this) = *((CCustomAlgorithm*)&val);
	m_bOnOff = val.m_bOnOff;
	m_type_Event_Algr = val.m_type_Event_Algr;

	m_result = val.m_result;
	m_pResult = &m_result.zet_header;
}
//------------------------------------------------------------------------------
void CCustomFalseEventStation::ClearStructResult()
{
	m_result.result = 0;
	m_result.value = 0.f;
	m_result.valueSNR = 0.f;
	m_result.valueThreshold = 0.f;
}
//------------------------------------------------------------------------------
long CCustomFalseEventStation::CalcRatios()
{
	long ret = (m_vectData.size() > 1) ? 0 : FatalError;
	m_result.value = 0.f;
	if (ret == 0)
	{
		auto it0 = m_vectData.cbegin();
		auto it1 = it0; ++it1;
		for (auto it0 = m_vectData.cbegin(), ie = m_vectData.cend(); it0 != ie; ++it0)
		{
			for (auto it1 = it0 + 1; it1 != ie; ++it1)
			{
				float ratio = CalcRatio2(*it0, *it1);
				if ((ratio > 0.f) && (m_result.value < ratio))
					m_result.value = ratio;
			}
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
}//namespace seismo
}//namespace dsp
}//namespace algorithm
}//namespace zet
//------------------------------------------------------------------------------