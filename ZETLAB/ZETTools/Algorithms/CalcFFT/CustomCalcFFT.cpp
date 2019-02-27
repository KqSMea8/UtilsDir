//------------------------------------------------------------------------------
// Файл CustomCalcFFT.cpp
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "CustomCalcFFT.h"
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
CCustomCalcFFT::CCustomCalcFFT(const int order) 
	: m_bCenteringSrcData(false)
	, m_typeNorm(norm_N)
	, m_typeWin(win_no)
	, m_nOrderFFT(0)
	, m_sizeHilbert(0)
	, m_factorTypeSrc(2)
	, m_factorWindow(1.)
	, m_factorEnergyWindow(1.)
{// остальные члены класса будут рассчитаны
	SetOrderFFT(order);
}
//------------------------------------------------------------------------------
CCustomCalcFFT::~CCustomCalcFFT()
{
	DeleteStructure_All();
}
//------------------------------------------------------------------------------
CCustomCalcFFT::CCustomCalcFFT(const CCustomCalcFFT & val)
	: m_bCenteringSrcData(false)
	, m_typeNorm(norm_N)
	, m_typeWin(win_no)
	, m_nOrderFFT(0)
{
	*this = val;
}
//------------------------------------------------------------------------------
CCustomCalcFFT & CCustomCalcFFT::operator = (const CCustomCalcFFT & val)
{
	m_bCenteringSrcData = val.m_bCenteringSrcData;
	m_typeNorm = val.m_typeNorm;
	m_typeWin = val.m_typeWin;

	m_factorTypeSrc = val.m_factorTypeSrc;
	m_factorWindow = val.m_factorWindow;
	m_factorEnergyWindow = val.m_factorEnergyWindow;
	SetOrderFFT(val.m_nOrderFFT);

	return *this;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int CCustomCalcFFT::GetFlagForFwd()
{// Получение флага для прямого преобразования. Приватная функция
	int ret(0);
	switch (m_typeNorm)
	{
	case norm_no:
		ret = IPP_FFT_NODIV_BY_ANY;
		break;

	case norm_N:
		ret = IPP_FFT_DIV_FWD_BY_N;
		break;

	case norm_Sqrt_N:
		ret = IPP_FFT_DIV_BY_SQRTN;
		break;

	case norm_N_inv:
		ret = IPP_FFT_DIV_INV_BY_N;
		break;

	default:
		ret = IPP_FFT_DIV_FWD_BY_N;
		break;
	}
	return ret;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------	
//------------------------------------------------------------------------------
// Получение флага центрирования исходного сигнала при Fwd.
// По умолчанию - true.
bool CCustomCalcFFT::GetCenteringSrcData()
{
	return m_bCenteringSrcData;
}

// Задание флага центрирования исходного сигнала при Fwd
void CCustomCalcFFT::SetCenteringSrcData(const bool val)
{
	m_bCenteringSrcData = val;
}
//------------------------------------------------------------------------------
// Получение типа нормировки результатов.
// По умолчанию - 1/N.
TypeNormalization CCustomCalcFFT::GetTypeNormalization()
{
	return m_typeNorm;
}

// Задание типа нормировки результатов
void CCustomCalcFFT::SetTypeNormalization(const TypeNormalization type)
{
	if (m_typeNorm != type)
	{
		m_typeNorm = type;
		DeleteStructure_FFT();
	}
}
//------------------------------------------------------------------------------
// Получение типа временного окна.
// По умолчанию - без окна.
TypeWindow CCustomCalcFFT::GetTypeWindowForFFT()
{
	return m_typeWin;
}

// Задание типа временного окна
void CCustomCalcFFT::SetTypeWindowForFFT(const TypeWindow type)
{
	if (m_typeWin!= type)
	{
		m_typeWin = type;
		ReSize_saWindow(0);
	}
}
//------------------------------------------------------------------------------
// Получение порядка FFT.
// По умолчанию - 7.
int CCustomCalcFFT::GetOrderFFT()
{
	return m_nOrderFFT;
}

// Задание порядка FFT
void CCustomCalcFFT::SetOrderFFT(const int order)
{
	int nOrder(0);
	if (order < OrderFFT_Min)
		nOrder = OrderFFT_Min;
	else if (order > OrderFFT_Max)
		nOrder = OrderFFT_Max;
	else
		nOrder = order;

	if (m_nOrderFFT != nOrder)
	{
		m_nOrderFFT = nOrder;
		m_sizeFFT = (int)pow(2, nOrder);
		m_sizeFFT_2 = m_sizeFFT / 2;
		m_sqrt_N = 1. / sqrt((double)m_sizeFFT);
		DeleteStructure_All();
	}
}
//------------------------------------------------------------------------------
// Получение размера исходных данных для прямого FFT = 2 ^ orderFFT
int CCustomCalcFFT::GetSize()
{
	return m_sizeFFT;
}

// Получение половины размера данных = (2 ^ orderFFT) / 2
int CCustomCalcFFT::GetHalfSize()
{
	return m_sizeFFT_2;
}
//------------------------------------------------------------------------------