//------------------------------------------------------------------------------
//	Файл FalseEventPolarization.cpp
//------------------------------------------------------------------------------
#include "StdAfx.h"
#include <Seismo/DetectorSSEC/Classification/Polarization/Polarization.h>
//------------------------------------------------------------------------------
namespace zet {
namespace algorithm {
namespace dsp {
namespace seismo {
//------------------------------------------------------------------------------
CFalseEventPolarization::CFalseEventPolarization() : CCustomFalseEvent()
	, m_numAverageArray(20)     // как-бы FreqADC = 100 Гц и 0,2 сек
	, m_numAverageSignal(1000)  // как-бы FreqADC = 100 Гц и 10,0 сек
{
	m_bOriginalSignal = false;

	m_type_Event_Algr = tea_ActualEvent;
	m_pConfig = &m_config.zet_header;

	// задание конфигурационных параметров значениями по умолчанию
	long r = ztOpenStructure(m_pConfig,
		ZETLAB_SSM_CFG_ALGR_POLAR_TYPE, ZETLAB_SSM_CFG_ALGR_POLAR_SIZE);

	// структура результата см. базовый класс
}
//------------------------------------------------------------------------------
CFalseEventPolarization::~CFalseEventPolarization()
{}
//------------------------------------------------------------------------------
CFalseEventPolarization::CFalseEventPolarization(
	const CFalseEventPolarization &val) : CCustomFalseEvent(val)
{
	m_cfgClass = val.m_cfgClass;
	m_config = val.m_config;

	m_numAverageArray = val.m_numAverageArray;
	m_numAverageSignal = val.m_numAverageSignal;
	m_saMaxAxis = val.m_saMaxAxis;
}
//------------------------------------------------------------------------------
void CFalseEventPolarization::operator = (const CFalseEventPolarization &val)
{
	*((CCustomFalseEvent*)this) = *((CCustomFalseEvent*)&val);

	m_cfgClass = val.m_cfgClass;
	m_config = val.m_config;
	
	m_numAverageArray = val.m_numAverageArray;
	m_numAverageSignal = val.m_numAverageSignal;
	m_saMaxAxis = val.m_saMaxAxis;
}
//------------------------------------------------------------------------------
long CFalseEventPolarization::SetConfigFromProxy(const DATE timeSRV)
{// копирование cgf-параметров из Proxy в структуру класса
	m_config = m_cfgClass;

	m_numAverageArray = (int)floor((float)m_freqADCdouble * m_config.TimeAverArray);
	m_numAverageSignal = (int)floor((float)m_freqADCdouble * m_config.TimeAverSignal);

	return ztSetStructureTimeLocal(m_pConfig, timeSRV);
}
//------------------------------------------------------------------------------
long CFalseEventPolarization::SetConfig(ZetHeader* pParam, const DATE timeSRV)
{	// проверка пришедшей структуры
	long ret = CCustomAlgorithm::SetConfig(pParam, timeSRV);
	if (ret == NoError)
	{	// копирование этой структуры в Proxy с целью проверки полей структуры
		m_cfgClass = *((Ssm_cfg_Algr_Polar*)pParam);
		// копирование параметрров из Proxy в структуру класса
		ret = SetConfigFromProxy(timeSRV);
	}
	return ret;
}
//------------------------------------------------------------------------------
//void CFalseEventPolarization::SetPointerForFreqADC(float *pFreq)
//{
//	if (pFreq != nullptr)
//	{
//		m_pFreqADC = pFreq;
//		int num = (int)floor(*m_pFreqADC * m_config.TimeAverArray);
//		if ((DWORD)num < m_saMaxAxis.size)
//			m_numAverageArray = num;
//		num = (int)floor(*m_pFreqADC * m_config.TimePreHistory);
//		if ((DWORD)num < m_saMaxAxis.size)
//			m_numTimePreHistory = num;
//	}
//}
//------------------------------------------------------------------------------
// Возвращаемое значение true - значит можно доверять результатам.
// maxAxis = 0, значит не сейсмическое событие
// Все расчёты необходимо выполнять в double, результат - float.
bool CFalseEventPolarization::CalcOnePoint(
	const float *pDataX, const float *pDataY, const float *pDataZ,
	const int size, float &maxAxis)
{
	double a, b, c, Q, R, S, x, y, ns[3] = { 0. };
	double m11, m12, m13, m22, m23, m33;

	maxAxis = 0.f;
	
	bool flag = (ippsConvert_32f64f(pDataX, m_saDouble[0].pData, size) == ippStsNoErr)
		&& (ippsConvert_32f64f(pDataY, m_saDouble[1].pData, size) == ippStsNoErr)
		&& (ippsConvert_32f64f(pDataZ, m_saDouble[2].pData, size) == ippStsNoErr);
	// расчёт элементов ковариационной матрицы
	if (flag)
	{// удаление постоянных составляющих - обязательно!!!		
		for (DWORD i = 0; i < 3; ++i)
		{
			double *add = m_saDouble[i].pData;
			flag &= (ippsMean_64f(add, size, &a) == ippStsNoErr) &&
					(ippsSubC_64f_I(a, add, size) == ippStsNoErr);
		}
		flag &=
			(ippsDotProd_64f(m_saDouble[0].pData, m_saDouble[0].pData, size, &m11) == ippStsNoErr) &&
			(ippsDotProd_64f(m_saDouble[0].pData, m_saDouble[1].pData, size, &m12) == ippStsNoErr) &&
			(ippsDotProd_64f(m_saDouble[0].pData, m_saDouble[2].pData, size, &m13) == ippStsNoErr) &&
			(ippsDotProd_64f(m_saDouble[1].pData, m_saDouble[1].pData, size, &m22) == ippStsNoErr) &&
			(ippsDotProd_64f(m_saDouble[1].pData, m_saDouble[2].pData, size, &m23) == ippStsNoErr) &&
			(ippsDotProd_64f(m_saDouble[2].pData, m_saDouble[2].pData, size, &m33) == ippStsNoErr);
	}

	bool ret = flag;
	if (flag)
	{// Уравнение 3-ей степени. Тригонометрическое решение Виета
		a = 1. / size;
		m11 *= a;
		m12 *= a;
		m13 *= a;
		m22 *= a;
		m23 *= a;
		m33 *= a;

		a = -m11 - m22 - m33;
		b = m11 * (m22 + m33) + m22 * m33 - m12 * m12 - m23 * m23 - m13 *m13;
		c = m13 * m13 * m22 + m12 * m12 * m33 + m11 * m23 * m23 -
			m11 * m22 * m33 - 2 * m13 *m12 * m23;

		Q = (a*a - 3 * b) / 9.;
		R = (2 * a*a*a - 9 * a*b + 27 * c) / 54.;
		S = Q*Q*Q - R*R;

		a /= 3.;
		if (S > 0.)
		{
			if (Q > 0)
			{
				x = sqrt(Q);
				y = acos(R / (Q * x)) / 3.;
				double p_3 = IPP_2PI / 3.;
				ns[0] = cos(y);
				ns[1] = cos(y + p_3);
				ns[2] = cos(y - p_3);

				x *= -2;
				ns[0] *= x;
				ns[1] *= x;
				ns[2] *= x;

				ns[0] -= a;
				ns[1] -= a;
				ns[2] -= a;
			}
			else
			{
				ns[0] = ns[1] = ns[2] = -a;
			}
		}
		else if (S == 0.)
		{
			y = abs(R);
			y = R / y * sqrt(y);
			ns[0] = -2 * y - a;
			ns[1] = ns[2] = y - a;
		}
		else	// комплексные корни, не наш случай
			flag = false;
		// проверка собственных чисел на положительность,
		// иначе квадратичная форма не может представлять эллипсоид
		flag &= (ns[0] > 0.) && (ns[1] > 0.) && (ns[2] > 0.);
		if (flag)
		{// собственные числа все положительные
			maxAxis = (float)std::max(std::max(ns[0], ns[1]), ns[2]);
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
long CFalseEventPolarization::FalseEvent(
	const float *pDataX, const float *pDataY,
	const float *pDataZ, const DWORD number)
{
	long ret(0);

	m_result.result = 0;
	m_result.value = 0.f;
	m_result.valueSNR = 0;
	m_result.valueThreshold = 0.f;

	m_saMaxAxis.Zero();

//	SetConfigFromProxy();
	if ((number > 0) && (m_numAverageArray > 0))	// размер усреднения для одной точки
	{
		if (m_saMaxAxis.ReSize(number) &&
			m_saDouble[0].ReSize((DWORD)m_numAverageArray) &&
			m_saDouble[1].ReSize((DWORD)m_numAverageArray) &&
			m_saDouble[2].ReSize((DWORD)m_numAverageArray))
		{
			float maxAxis(0);
			int ie = (int)number - m_numAverageArray;
			if (ie > 0)
			{
				for (int i = 0; i < ie; ++i)
				{// основная часть массива
					if (CalcOnePoint(pDataX + i, pDataY + i, pDataZ + i,
						m_numAverageArray, maxAxis))
						maxAxis *= 1000000.f;
					else
						maxAxis = 0.f;
					m_saMaxAxis.pData[i] = maxAxis;
					//m_saMaxAxis.pData[i] = CalcOnePoint(pDataX + i, pDataY + i, pDataZ + i,
					//	m_numAverageArray, maxAxis) ? maxAxis : 0.f;
				}
				for (DWORD i = ie; i < number; ++i)
				{// хвостик массива заполняю последним значением
					m_saMaxAxis.pData[i] = maxAxis;
				}

				// Если максимальное значение за весь интервал не превышает
				// максимального значения + 2 * СКО за предысторию,
				// то это ложное событие
				float maxAll(0.f), maxPre(0.f), stdPre(0.f);
				if (
					(ippsMax_32f(m_saMaxAxis.pData + m_sizePreHistory,
					(int)number - m_sizePreHistory, &maxAll) == ippStsNoErr) &&
					(ippsMax_32f(m_saMaxAxis.pData, m_sizePreHistory, &maxPre) == ippStsNoErr) &&
					(ippsStdDev_32f(m_saMaxAxis.pData, m_sizePreHistory,
									&stdPre, ippAlgHintFast) == ippStsNoErr) )
				{	// пороговое значение по предыстории
					m_result.valueThreshold = maxPre + 2 * stdPre;
					// сигнал алгоритма привожу к стандартному виду
					if ((maxAll > 0.f) && (maxPre > 0.f) && (stdPre > 0.f))
					{
						m_result.result = (maxAll > m_result.valueThreshold) ? 1 : 0;
						m_result.value = maxAll;
						m_result.valueSNR = maxAll / maxPre;
					}
				}
			}
		}
	}// if (number > 0)
	else
		ret = ErrorCodes::Err_Copy_Data;
	return ret;
}
//------------------------------------------------------------------------------
bool CFalseEventPolarization::FiltrationSignal()
{
	int size = m_saMaxAxis.size;
	bool ret = (m_numAverageSignal < size) && (m_saTmp = m_saMaxAxis);
	if (ret)
	{
		float val(0.f);
		ret = ippsMean_32f(m_saTmp.pData, m_numAverageSignal, &val,
							ippAlgHintFast) == ippStsNoErr;
		if (ret)
		{
			ippsSet_32f(val, m_saMaxAxis.pData, m_numAverageSignal);
			//float *pointer = m_saTmp.pData;
			//for (int i = m_numAverageSignal; i < size; ++i)
			//{
			//	if (ippsMean_32f(++pointer, m_numAverageSignal, &val,
			//					ippAlgHintFast) != ippStsNoErr)
			//			val = 0.f;				
			//	m_saMaxAxis.pData[i] = val;
			//}
			for (int i = m_numAverageSignal, j = 1; i < size; ++i, ++j)
			{
				if (ippsMean_32f(m_saTmp.pData + j, m_numAverageSignal, &val,
					ippAlgHintFast) != ippStsNoErr)
					val = 0.f;
				m_saMaxAxis.pData[i] = val;
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