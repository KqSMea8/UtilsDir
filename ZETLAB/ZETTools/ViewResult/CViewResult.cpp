//------------------------------------------------------------------------------
#include "stdafx.h"
#include "CViewResult.h"
//------------------------------------------------------------------------------
CViewResultParam::CViewResultParam()
	: m_numAver(0)
	, pChanStructInit(nullptr)
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
	m_numAver = val.m_numAver;
	pChanStructInit = val.pChanStructInit;
	auto size = sizeof(STRUCT_VIEW_RESALT);
	memcpy_s(&m_param, size, &val.m_param, size);
	return *this;
}
//------------------------------------------------------------------------------
// Инициализация структуры отображения по внешней структуре параметров канала
bool CViewResultParam::InitParam(LPCHANNEL_STRUCT par, const DWORD numAver, bool bBindingPoint, BYTE indxPointExt)
{
	m_numAver = numAver;
	pChanStructInit = par;
	return zvInitParam(par, numAver, (BYTE)bBindingPoint, indxPointExt, &m_param) == 0;
}
//------------------------------------------------------------------------------
// Новое значение
bool CViewResultParam::NewValue(const float newVal)
{
	return 0 == zvNewValue(newVal, &m_param);
}
//------------------------------------------------------------------------------
// Перезапуск
void CViewResultParam::ReStart()
{
	long r = zvReStart(&m_param);
}
//------------------------------------------------------------------------------
// Получение типа привязки (есть/нет) положения десятичной "точки"
bool CViewResultParam::GetBindingPoint()
{
	return (m_param.bFlags & FLAG_POINT_ON) != 0;
}
//------------------------------------------------------------------------------
// Задание типа привязки (есть/нет) положения десятичной "точки"
void CViewResultParam::SetBindingPoint(const bool bOnOff)
{
	long r = zvSetBindingPoint((BYTE)bOnOff, &m_param);
}
//------------------------------------------------------------------------------
// Получение внутреннего индекса (счёт с 0) положения десятичной "точки",
// рассчитанного самой dll
BYTE CViewResultParam::GetIndxPointSelf()
{
	return m_param.nIndxPointInt;
}
//------------------------------------------------------------------------------
// Получение внешнего индекса (счёт с 0) положения десятичной "точки",
// заданного извне
BYTE CViewResultParam::GetIndxPointExtern()
{
	return m_param.nIndxPointExt;
}
//------------------------------------------------------------------------------
// Задание внешнего индекса (счёт с 0) положения десятичной "точки"
void CViewResultParam::SetIndxPointExtern(BYTE indx)
{// Задание индекса (счёт с 0) положения десятичной "точки"
	long r = zvSetIndxPoint(indx, &m_param);
}
//------------------------------------------------------------------------------
//
//
//
//
//------------------------------------------------------------------------------
CViewResultTransform::CViewResultTransform(LPCHANNEL_STRUCT pChanStruct)
	: CViewResultParam()
	, m_k(1.)
{
	m_convert.Reset();
}
//------------------------------------------------------------------------------
CViewResultTransform::CViewResultTransform(const CViewResultTransform & val)
	: CViewResultParam(val)
{
	m_k = val.m_k;
	m_convert = val.m_convert;
	m_сhanStructView = val.m_сhanStructView;
}
//------------------------------------------------------------------------------
CViewResultTransform & CViewResultTransform::operator = (const CViewResultTransform & val)
{
	*((CViewResultParam*)this) = *((CViewResultParam*)&val);

	m_k = val.m_k;
	m_convert = val.m_convert;
	m_сhanStructView = val.m_сhanStructView;

	return *this;
}
//------------------------------------------------------------------------------	
// Инициализация структуры отображения по структуре параметров канала
bool CViewResultTransform::InitParamView(
	LPCHANNEL_STRUCT par,
	const DWORD numAver,
	bool bBindingPoint,
	BYTE indxPointExt)
{
	pChanStructInit = par;
	bool b1 = 0 == zvInitParam(par, numAver, (BYTE)bBindingPoint, indxPointExt, &m_param) == 0;
	bool b2 = memcpy_s(&m_сhanStructView, SIZE_CHANNEL_STRUCT, par, SIZE_CHANNEL_STRUCT) > 0;

	return b1 && b2;
}
//------------------------------------------------------------------------------
// Задание типа преобразования единицы измерения
bool CViewResultTransform::InitTransform(
	const TypeTransformUnit ttu,	// тип преобразования единицы
	LPCWCH lpwMeasSrc,				// размерность исходного канала
	LPCWCH lpwMeasDst)				// размерность выходного канала
{
	bool b1 = 0 == zvInitTransformUnits(ttu, lpwMeasSrc, lpwMeasDst, &m_convert);
	memcpy_s(&m_сhanStructView, SIZE_CHANNEL_STRUCT, pChanStructInit, SIZE_CHANNEL_STRUCT);
	bool b2 = 0 == zvRealizationUnit(&m_convert, &m_сhanStructView, &m_param);
	m_k = 1.;

	return b1 && b2;
}
//------------------------------------------------------------------------------
// Изменение единицы измерения канала отображения
bool CViewResultTransform::ChangeUnit(
	const TypeTransformUnit ttu,	// тип преобразования единицы
	LPCWCH lpwMeasDst)				// размерность выходного канала
{
	bool b1 = 0 == zvChangeUnit(ttu, lpwMeasDst, &m_convert);
	memcpy_s(&m_сhanStructView, SIZE_CHANNEL_STRUCT, pChanStructInit, SIZE_CHANNEL_STRUCT);
	bool b2 = 0 == zvRealizationUnit(&m_convert, &m_сhanStructView, &m_param);
	m_k = 1.;

	return b1 && b2;
}
//------------------------------------------------------------------------------
void CViewResultTransform::ReStart()
{// virtual
	CViewResultParam::ReStart();
	m_convert.Reset();
}
//------------------------------------------------------------------------------
// Новое значение
bool CViewResultTransform::NewValue(const float newVal, bool & bEventPreUnit)
{// virtual
	float result(0.f);
	double val(1.);
	float val_new = (float)(newVal * m_k);
	bEventPreUnit = false;

	bool b2(true);
	bool b1 = 0 == zvTransformValue(val_new, &result, (BYTE*)&bEventPreUnit, &val, &m_convert);
	if (bEventPreUnit)
	{
		b2 = 0 == zvRealizationPreUnit(&m_convert, &m_сhanStructView, &m_param);
		m_k *= val;
	}
	
//	swprintf_s(m_param.sValueCur, L"%f", result); - для тестирования
	bool b3 = 0 == zvNewValue(result, &m_param);

	return b1 && b2 && b3;
}
//------------------------------------------------------------------------------