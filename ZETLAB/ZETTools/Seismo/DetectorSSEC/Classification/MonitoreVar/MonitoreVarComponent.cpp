#include "stdafx.h"
#include "MonitoreVarComponent.h"
namespace zet
{
namespace algorithm
{
namespace dsp
{
//--------------------------------------------------------------
CMonitoreVarComponent::CMonitoreVarComponent()
{
	m_pP = nullptr;
	m_size = 0;
	m_min = m_max = 0.f;
}
//--------------------------------------------------------------
CMonitoreVarComponent::CMonitoreVarComponent(const CMonitoreVarComponent* pMVC)
{
	m_min = pMVC->m_min;
	m_max = pMVC->m_max;

	m_size = pMVC->m_size;


	if (pMVC->m_pP)
	{
		if (m_pP)
		{
			delete[] m_pP;
			m_pP = nullptr;
		}
		m_pP = new float[m_size];
		ippsCopy_32f(m_pP, pMVC->m_pP, m_size);
	}
	memcpy_s(&m_Prm, sizeof(m_Prm), &pMVC->m_Prm, sizeof(m_Prm));
}
//--------------------------------------------------------------
CMonitoreVarComponent::~CMonitoreVarComponent()
{
	if (m_pP)
	{
		delete[] m_pP;
		m_pP = nullptr;
	}
}
//--------------------------------------------------------------
void CMonitoreVarComponent::SetMemory(const PVOID pD, const DWORD size)
{
	if (m_size < size)
	{	// если размер имеющихся массивов меньше
		m_pP = new float[size];
		m_size = size;
	}
	// обнуляем
	ippsZero_32f(m_pP, m_size);
	memcpy_s(m_pP, size * sizeof(float), (float*)pD, size * sizeof(float));
}
//--------------------------------------------------------------
void CMonitoreVarComponent::SetMinThreshold(const float min)
{
	m_min = min;
}
//--------------------------------------------------------------
void CMonitoreVarComponent::SetMaxThreshold(const float max)
{
	m_max = max;
}
//--------------------------------------------------------------
ErrorCode CMonitoreVarComponent::ReStart()
{
	m_bEventFlag = false;
	return ErrorCodes::Err_Initialization;
}
//--------------------------------------------------------------
long CMonitoreVarComponent::SetParam(const PVOID pParam)
{
	return Err_Initialization;
}
//--------------------------------------------------------------
PVOID CMonitoreVarComponent::GetParam()
{
	return nullptr;
}
//--------------------------------------------------------------
long CMonitoreVarComponent::FalseEvent(const PVOID pData, const DWORD number, bool &bFalseEvent)
{
	// выделяем память под массивы
	SetMemory(pData, number);
	// вычисляем срднее значение
	ippsStdDev_32f(m_pP, number, &m_MiddleVal, ippAlgHintAccurate);
	if (m_min > m_MiddleVal || m_max < m_MiddleVal)
	{
		bFalseEvent = true;
		return 0;
	}
	bFalseEvent = false;
	return 0;
}
//--------------------------------------------------------------
}
}
}
