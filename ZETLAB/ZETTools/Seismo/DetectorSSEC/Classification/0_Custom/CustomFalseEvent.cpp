//------------------------------------------------------------------------------
//	Файл CustomFalseEvent.cpp
//------------------------------------------------------------------------------
//#ifndef _WIN32_WINNT
//#define _WIN32_WINNT _WIN32_WINNT_MAXVER
//#endif
#include "StdAfx.h"
//------------------------------------------------------------------------------
#include <Seismo\DetectorSSEC/Classification/0_Custom/CustomFalseEvent.h>
//------------------------------------------------------------------------------
namespace zet {
namespace algorithm {
namespace dsp {
namespace seismo {
//------------------------------------------------------------------------------
CCustomFalseEvent::CCustomFalseEvent() : CCustomAlgorithm()
	, m_bOnOff(true)
	, m_bOriginalSignal(false)
	, m_weight(1.f)
	, m_sizePreHistory(2000)
	, m_freqADCdouble(100.)
{
	m_pResult = &m_result.zet_header;
	long ret = ztOpenStructure(m_pResult,
		ZETLAB_SSM_RES_ALGR_CUSTOM_TYPE, ZETLAB_SSM_RES_ALGR_CUSTOM_SIZE);
}
//------------------------------------------------------------------------------
CCustomFalseEvent::CCustomFalseEvent(const CCustomFalseEvent & val) :
	CCustomAlgorithm(val)
{
	m_bOnOff = val.m_bOnOff;
	m_weight = val.m_weight;
	m_freqADCdouble = val.m_freqADCdouble;
	m_bOriginalSignal = val.m_bOriginalSignal;
	m_sizePreHistory = val.m_sizePreHistory;
	m_type_Event_Algr = val.m_type_Event_Algr;

	m_result = val.m_result;
}
//------------------------------------------------------------------------------
void CCustomFalseEvent::operator = (const CCustomFalseEvent & val)
{
	*((CCustomAlgorithm*)this) = *((CCustomAlgorithm*)&val);
	m_bOnOff = val.m_bOnOff;
	m_weight = val.m_weight;
	m_freqADCdouble = val.m_freqADCdouble;
	m_bOriginalSignal = val.m_bOriginalSignal;
	m_sizePreHistory = val.m_sizePreHistory;
	m_type_Event_Algr = val.m_type_Event_Algr;

	m_result = val.m_result;
}
//------------------------------------------------------------------------------
long CCustomFalseEvent::SetFreqADC(double freqADC)
{
	long ret = ((40. <= freqADC) && (freqADC <= 2500.)) ?
		0 : ErrorCodes::InvalidParams;
	if (ret)
		m_freqADCdouble = freqADC;
	return ret;
}
//------------------------------------------------------------------------------
long CCustomFalseEvent::SetWeigth(float val)
{
	long ret(0);
	if ((0.f < val) && (val <= 1.f))
		m_weight = val;
	else
		ret = ErrorCodes::InvalidParams;
	return ret;
}
//------------------------------------------------------------------------------
long CCustomFalseEvent::SetSizePreHistory(int val)
{
	bool flag = val > 100;
	if (flag)
		m_sizePreHistory = val;
	return flag ? ErrorCodes::NoError : ErrorCodes::InvalidParams;
}
//------------------------------------------------------------------------------
void CCustomFalseEvent::ClearStructResult()
{
	m_result.result = 0;
	m_result.value = 0.f;
	m_result.valueSNR = 0.f;
	m_result.valueThreshold = 0.f;
}
//------------------------------------------------------------------------------
bool CCustomFalseEvent::IsEvent()
{
	return m_result.result != 0;
}
//------------------------------------------------------------------------------
long CCustomFalseEvent::DataStamp(const DATE timeSRV)
{// Если timeSRV = 0, значит оно не задано
//	m_pResult всегда не nullptr - см. конструктор
	return ztSetStructureTimeLocal(m_pResult, timeSRV);
}
//------------------------------------------------------------------------------
long CCustomFalseEvent::Calculation(
	const float *pData, const DWORD number, const DATE timeSRV)
{// именно в этом порядке
	long ret = FalseEvent(pData, number);
	DataStamp(timeSRV);
	return ret;
}
//------------------------------------------------------------------------------
long CCustomFalseEvent::Calculation(
	const float *pDataX, const float *pDataY, const float *pDataZ,
	const DWORD number, const DATE timeSRV)
{// именно в этом порядке
	long ret = FalseEvent(pDataX, pDataY, pDataZ, number);
	DataStamp(timeSRV);
	return ret;
}
//------------------------------------------------------------------------------
}//namespace seismo
}//namespace dsp
}//namespace algorithm
}//namespace zet
//------------------------------------------------------------------------------