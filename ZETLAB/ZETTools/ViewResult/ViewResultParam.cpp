//------------------------------------------------------------------------------
#include "stdafx.h"
#include <ViewResult\ViewResultParam.h>
//------------------------------------------------------------------------------
CViewResultParam::CViewResultParam()
{
	ZeroMemory(&m_param, sizeof(STRUCT_VIEW_RESALT));
}
//------------------------------------------------------------------------------
CViewResultParam::CViewResultParam(const CViewResultParam & val)
{
	*this = val;
}
//------------------------------------------------------------------------------
CViewResultParam & CViewResultParam::operator = (const CViewResultParam & val)
{
	auto size = sizeof(STRUCT_VIEW_RESALT);
	memcpy_s(&m_param, size, &val.m_param, size);
	return *this;
}
//------------------------------------------------------------------------------
bool CViewResultParam::Init(LPCHANNEL_STRUCT par, const DWORD numAver, bool bBindingPoint)
{// Инициализация по структуре параметров канала
	return zvInitParam(par, numAver, (BYTE)bBindingPoint, 0, &m_param) == 0;
}
//------------------------------------------------------------------------------
bool CViewResultParam::Init(PVOID par, const DWORD numAver, bool bBindingPoint)
{// Инициализация по структуре, являющейся наследником структуры параметров канала
	return zvInitParam((LPCHANNEL_STRUCT)par, numAver, (BYTE)bBindingPoint, 0, &m_param) == 0;
}
//------------------------------------------------------------------------------
void CViewResultParam::NewValue(const float newVal)
{// Новое значение
	long r = zvNewValue(newVal, &m_param);
}
//------------------------------------------------------------------------------
void CViewResultParam::ReStart()
{
	long r = zvReStart(&m_param);
}
//------------------------------------------------------------------------------
void CViewResultParam::SetIndxPoint(BYTE indx)
{// Задание индекса (счёт с 0) положения десятичной "точки"
	long r = zvSetIndxPoint(indx, &m_param);
}
//------------------------------------------------------------------------------
BYTE CViewResultParam::GetIndxPointSelf()
{
	return m_param.nIndxPointInt;
}
//------------------------------------------------------------------------------
BYTE CViewResultParam::GetIndxPointExtern()
{
	return m_param.nIndxPointExt;
}
//------------------------------------------------------------------------------