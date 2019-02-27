//------------------------------------------------------------------------------
// Файл CalcFFT.cpp
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "CalcDFT_64.h"
#include <mathZet.h>
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
CCalcDFT_64f::CCalcDFT_64f(const int size) : CCustomCalcFFT()
, m_sizeDFT(0)
, m_sizeDFT_2(0)
, m_kNorma(1.)
, m_pCalcFFT(nullptr)
{
	SetParam(size);
}
//------------------------------------------------------------------------------
CCalcDFT_64f::~CCalcDFT_64f()
{
	if (m_pCalcFFT)
	{
		delete m_pCalcFFT;
		m_pCalcFFT = nullptr;
	}
}
//------------------------------------------------------------------------------
CCalcDFT_64f::CCalcDFT_64f(const CCalcDFT_64f & val) : CCustomCalcFFT(val)
, m_sizeDFT(0)
, m_sizeDFT_2(0)
, m_kNorma(1.)
, m_pCalcFFT(nullptr)
{
	*this = val;
}
//------------------------------------------------------------------------------
CCalcDFT_64f & CCalcDFT_64f::operator = (const CCalcDFT_64f & val)
{
	*((CCustomCalcFFT*)this) = *((CCustomCalcFFT*)&val);

	m_sizeDFT = val.m_sizeDFT;
	m_sizeDFT_2 = val.m_sizeDFT_2;

	m_kNorma = val.m_kNorma;

	if (val.m_pCalcFFT == nullptr)
	{
		if (m_pCalcFFT != nullptr)
			delete m_pCalcFFT;
		m_pCalcFFT = nullptr;
	}
	else
	{
		if (m_pCalcFFT == nullptr)
			m_pCalcFFT = new CCalcFFT_64f;
		*m_pCalcFFT = *val.m_pCalcFFT;
	}

	m_saSrc_d = val.m_saSrc_d;
	m_saSrc_dc = val.m_saSrc_dc;

	m_saExp1 = val.m_saExp1;
	m_saExp2 = val.m_saExp2;

	return *this;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
long CCalcDFT_64f::CreateMassiveExp()
{
	long ret = ((m_pCalcFFT != nullptr) &&
		(m_sizeDFT > 0) &&
		(m_sizeFFT > 0))
		? 0 : Err_Initialization;
	if (ret == 0)
	{
		ret = (m_saExp1.ReSize(m_sizeDFT) && m_saExp2.ReSize(m_sizeFFT))
			? 0 : Err_Create_Array;
		if (ret == 0)
		{
			double pi_size = IPP_PI / m_sizeDFT;

			// нулевые элементы вспомогательных массивов
			m_saExp1.pData[0].re = m_saExp2.pData[0].re = 1.;
			m_saExp1.pData[0].im = m_saExp2.pData[0].im = 0.;

			// не нулевые элементы вспомогательных массивов
			for (int i = 1; i <= m_sizeFFT_2; ++i)
			{
				double x = (i * pi_size) * i;
				double cosx = cos(x);
				double sinx = sin(x);

				if (i < m_sizeDFT)
				{
					m_saExp1.pData[i].re = cosx;
					m_saExp1.pData[i].im = sinx;
				}

				m_saExp2.pData[i].re = cosx;
				m_saExp2.pData[i].im = -sinx;

				int sizeFFT_i = m_sizeFFT - i;
				m_saExp2.pData[sizeFFT_i].re = m_saExp2.pData[i].re;
				m_saExp2.pData[sizeFFT_i].im = m_saExp2.pData[i].im;
			}
			ret = m_pCalcFFT->CalcFwd_CToC_I(m_saExp2.pData);
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
long CCalcDFT_64f::SetParam(const int size)
{
	long ret = (size > 0) ? 0 : InvalidParams;
	if (ret == 0)
	{
		m_sizeDFT = size;
		m_sizeDFT_2 = m_sizeDFT / 2;
		m_kNorma = log2(m_sizeDFT);		// временное использование m_kNorma
		m_nOrderFFT = (int)m_kNorma;
		if (m_kNorma > (double)m_nOrderFFT)
			++m_nOrderFFT;
		m_nOrderFFT += 2;	// получаем 4-х кратное перекрытие

		// нормировочный коэффициент
		m_kNorma = pow(2, (m_nOrderFFT * 0.5 + 1)) / m_sizeDFT;

		if ((m_nOrderFFT < OrderFFT_Min) || (m_nOrderFFT > OrderFFT_Max))
			ret = InvalidParams;
		if (ret == 0)
		{
			if (m_pCalcFFT == nullptr)
			{
				m_pCalcFFT = new CCalcFFT_64f(m_nOrderFFT);
				if (m_pCalcFFT == nullptr)
					ret = FatalError;
			}
			else
			{
				m_pCalcFFT->SetOrderFFT(m_nOrderFFT);
			}

			if (ret == 0)
			{
				m_sizeFFT = m_pCalcFFT->GetSize();
				m_sizeFFT_2 = m_pCalcFFT->GetHalfSize();
				// для данной нормировки m_kNorma определяется просто
				m_pCalcFFT->SetTypeNormalization(norm_Sqrt_N);
				m_pCalcFFT->SetCenteringSrcData(false);
				m_pCalcFFT->SetTypeWindowForFFT(win_no);

				long ret = (m_saSrc_d.ReSize(m_sizeDFT) && m_saSrc_dc.ReSize(m_sizeFFT))
					? 0 : Err_Create_Array;
				if (ret == 0)
				{
					m_saSrc_d.Zero();
					m_saSrc_dc.Zero();
					ret = CreateMassiveExp();
				}
			}
			else
			{
				m_sizeFFT = m_sizeFFT_2 = m_sizeDFT = m_sizeDFT_2 = 0;
			}
		}
	}
	return  ret;
}
//------------------------------------------------------------------------------
// Переопределенная функция базового класса.
// Получение размера данных = m_sizeDFT, рассчитанного в SetParam
// Возвращаемое значение:
// 0 - не было вызова SetParam;
// > 0 - размер.
int CCalcDFT_64f::GetSize()
{
	return (m_pCalcFFT != nullptr) ? m_sizeDFT : 0;
}
//------------------------------------------------------------------------------
// Переопределенная функция базового класса.
// Получение половины размера данных = m_sizeDFT / 2, рассчитанного в SetParam
// Возвращаемое значение:
// 0 - не было вызова SetParam;
// > 0 - размер.
int CCalcDFT_64f::GetHalfSize()
{
	return (m_pCalcFFT != nullptr) ? (m_sizeDFT / 2) : 0;
}
//------------------------------------------------------------------------------
// Прямое DFT.
// Преобразование реального сигнала в комплексный спектр.
// - pSrc - реальные входные данные с размером от 1 до m_sizeFFT;
// - pDst - комплексные выходные данные с размером m_sizeFFT / 2;
// - sizeSrc - размер входного массива.
// Если sizeSrc = 0, то размер входного массива = m_sizeFFT.
// Если sizeSrc меньше m_sizeDFT, то перед расчетами во входной массив
// добавляются нули. Если больше - то лишние данные игнорируются.
// Возвращается код ошибки ErrorCodes.
//
long CCalcDFT_64f::CalcFwd_RToC(double * pSrc, Ipp64fc * pDst, const int sizeSrc)
{
	long ret = ((pSrc != nullptr) && (pDst != nullptr)) ? 0 : Err_NULL_Pointer;
	if (ret == 0)
	{
		if (sizeSrc >= 0)
		{
			bool bFlag(false);
			if ((sizeSrc == 0) || (sizeSrc >= m_sizeDFT))
			{// полный размер исходных данных
				bFlag = ippStsNoErr == ippsCopy_64f(pSrc, m_saSrc_d.pData, m_sizeDFT);
				if (m_bCenteringSrcData)
					bFlag &= RemovingConstComponent_64f_I(m_saSrc_d.pData, m_sizeDFT);
			}
			else
			{// не полный размер исходных данных
				bFlag =
					(ippStsNoErr == ippsCopy_64f(pSrc, m_saSrc_d.pData, sizeSrc)) &&
					(ippStsNoErr == ippsZero_64f(m_saSrc_d.pData + sizeSrc, m_sizeDFT - sizeSrc));
				if (m_bCenteringSrcData)
					bFlag &= RemovingConstComponent_64f_I(m_saSrc_d.pData, sizeSrc);
			}
			if (bFlag)
			{	// копирование подготовленных исходных данных в комплексный массив размером m_sizeFFT
				m_saSrc_dc.Zero();
				for (int i = 0; i < m_sizeDFT; ++i)
					m_saSrc_dc.pData[i].re = m_saSrc_d.pData[i];

				bFlag =
					// умножение на первую мнимую экспоненту размером m_sizeDFT
					(ippStsNoErr == ippsMul_64fc_I(m_saExp1.pData, m_saSrc_dc.pData, m_sizeDFT)) &&
					// FFT
					(0 == m_pCalcFFT->CalcFwd_CToC_I(m_saSrc_dc.pData)) &&
					// умножение на вторую мнимую экспоненту размером m_sizeFFT
					(ippStsNoErr == ippsMul_64fc_I(m_saExp2.pData, m_saSrc_dc.pData, m_sizeFFT)) &&
					// FFT
					(0 == m_pCalcFFT->CalcFwd_CToC_I(m_saSrc_dc.pData));
				if (bFlag)
				{
					MulC_ComplexMass_I(m_kNorma, m_saSrc_dc.pData, m_sizeDFT_2);
					// копирование результата (комплексный спектр) размером m_sizeDFT_2
					if (ippStsNoErr != ippsCopy_64fc(m_saSrc_dc.pData, pDst, m_sizeDFT_2))
						ret = Err_Copy_Data;
				}
				else
					ret = Err_Calc;
			}
			else
				ret = Err_Copy_Data;
		}
		else
			ret = Err_Size_Less__Zero;
	}
	return ret;
}
//------------------------------------------------------------------------------