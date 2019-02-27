//------------------------------------------------------------------------------
// Файл CalcFFT_32.cpp
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "CalcFFT_32.h"
#include <Algorithms\FunctionsUsingIPP.h>
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
CCalcFFT_32f::CCalcFFT_32f(const int order) : CCustomCalcFFT(order)
	, m_pStruct_R(nullptr)
	, m_pStruct_C(nullptr)
	, m_pStruct_Hilb(nullptr)
{// остальные члены класса будут рассчитаны
	m_factorFFT = (float)(m_factorTypeSrc / m_factorWindow);
}
//------------------------------------------------------------------------------
CCalcFFT_32f::~CCalcFFT_32f()
{
}
//------------------------------------------------------------------------------
CCalcFFT_32f::CCalcFFT_32f(const CCalcFFT_32f & val) : CCustomCalcFFT(val)
	, m_pStruct_R(nullptr)
	, m_pStruct_C(nullptr)
	, m_pStruct_Hilb(nullptr)
{
	*this = val;
}
//------------------------------------------------------------------------------
CCalcFFT_32f & CCalcFFT_32f::operator = (const CCalcFFT_32f & val)
{
	*((CCustomCalcFFT*)this) = *((CCustomCalcFFT*)&val);

	m_factorFFT = val.m_factorFFT;
	m_saWindow = val.m_saWindow;
	SetOrderFFT(val.m_nOrderFFT);

	return *this;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------	
void CCalcFFT_32f::DeleteStructure_FFT()
{// Приватная функция
	m_pStruct_R = nullptr;
	m_pStruct_C = nullptr;
}
//------------------------------------------------------------------------------
void CCalcFFT_32f::DeleteStructure_Hilbert()
{
	m_pStruct_Hilb = nullptr;
}
//------------------------------------------------------------------------------
void CCalcFFT_32f::DeleteStructure_All()
{
	DeleteStructure_FFT();
	DeleteStructure_Hilbert();
}
//------------------------------------------------------------------------------
void CCalcFFT_32f::ReSize_saWindow(DWORD size)
{
	m_saWindow.ReSize(size);
}
//------------------------------------------------------------------------------
long CCalcFFT_32f::Create_Structure_R()
{// Создание структуры для прямого преобразования Real => Compl. Приватная функция
	long ret = (m_pStruct_R != nullptr) ? 0 : Err_Initialization;
	if (ret != 0)
	{
		int flag = GetFlagForFwd();
		int sizeFFTSpec(0), sizeFFTInitBuf(0), sizeFFTWorkBuf(0);
		if ((ippStsNoErr == ippsFFTGetSize_R_32f(m_nOrderFFT, flag, ippAlgHintAccurate, &sizeFFTSpec, &sizeFFTInitBuf, &sizeFFTWorkBuf)) &&
			m_saStruct_R.ReSize(sizeFFTSpec) &&
			m_saBuffer_8u.ReSize(sizeFFTInitBuf) &&
			(ippStsNoErr == ippsFFTInit_R_32f(&m_pStruct_R, m_nOrderFFT, flag, ippAlgHintAccurate, m_saStruct_R.pData, m_saBuffer_8u.pData)) &&
			m_saBuffer_8u.ReSize(sizeFFTWorkBuf))
			ret = 0;			
	}
	return ret;
}
//------------------------------------------------------------------------------
long CCalcFFT_32f::Create_Structure_C()
{// Создание структуры для прямого преобразования Compl => Compl. Приватная функция
	long ret = (m_pStruct_C != nullptr) ? 0 : Err_Initialization;
	if (ret != 0)
	{
		int flag = GetFlagForFwd();
		int sizeFFTSpec(0), sizeFFTInitBuf(0), sizeFFTWorkBuf(0);
		if ((ippStsNoErr == ippsFFTGetSize_C_32fc(m_nOrderFFT, flag, ippAlgHintAccurate, &sizeFFTSpec, &sizeFFTInitBuf, &sizeFFTWorkBuf)) &&
			m_saStruct_C.ReSize(sizeFFTSpec) &&
			m_saBuffer_8u.ReSize(sizeFFTInitBuf) &&
			(ippStsNoErr == ippsFFTInit_C_32fc(&m_pStruct_C, m_nOrderFFT, flag, ippAlgHintAccurate, m_saStruct_C.pData, m_saBuffer_8u.pData)) &&
			m_saBuffer_8u.ReSize(sizeFFTWorkBuf))
			ret = 0;
	}
	return ret;
}
//------------------------------------------------------------------------------
long CCalcFFT_32f::Create_Structure_Hilb(const int size)
{// Создание структуры для преобразования Гильберта. Приватная функция
	long ret = ((m_pStruct_Hilb != nullptr) && (m_sizeHilbert == size)) ? 0 : Err_Initialization;
	if (ret != 0)
	{
		int sizeSpec(0), sizeBuffer(0);
		if ((ippStsNoErr == ippsHilbertGetSize_32f32fc(size, ippAlgHintAccurate, &sizeSpec, &sizeBuffer)) &&
			m_saStruct_Hilb.ReSize(sizeSpec) &&
			m_saStruct_WorkHilb.ReSize(sizeBuffer))
		{
			m_sizeHilbert = size;
			m_pStruct_Hilb = (IppsHilbertSpec*)m_saStruct_Hilb.pData;
			ret = 0;
		}
		else
			m_pStruct_Hilb = nullptr;
	}
	return ret;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------	
//------------------------------------------------------------------------------
void CCalcFFT_32f::Calc_FactorFFT()
{
	m_factorFFT = (float)(m_factorTypeSrc / m_factorWindow);
}
//------------------------------------------------------------------------------
void CCalcFFT_32f::CorrectZeroItem(Ipp32fc * pData)
{
	if (m_factorTypeSrc > 1)
	{
		pData[0].re /= 2;
		pData[m_sizeFFT_2].re /= 2;
	}
}
//------------------------------------------------------------------------------
bool CCalcFFT_32f::ConvertToPerm(Ipp32fc * pData)
{
	bool ret = m_saDataPerm.ReSize(m_sizeFFT) &&
		(ippStsNoErr == ippsCopy_32f((float*)pData, m_saDataPerm.pData, m_sizeFFT));
	if (ret)
	{
		m_saDataPerm.pData[1] = pData[m_sizeFFT_2].re;		
	}
	return ret;
}
//------------------------------------------------------------------------------
// Приватная функция. Проверять ничего не надо
void CCalcFFT_32f::CorrectConstComponent(const float re0, float * pData, const int size)
{
	float x0(0.f);
	if (m_typeNorm == norm_N)
		x0 = 0.5f * re0;
	else if (m_typeNorm == norm_Sqrt_N)
		x0 = (float)(0.5 * re0 * m_sqrt_N);

	if (x0 != 0.f)		// IsNotEqual - нельзя!!!
		ippsAddC_32f_I(x0, pData, size);
}
//------------------------------------------------------------------------------
// Приватная функция. Проверять ничего не надо
void CCalcFFT_32f::CorrectConstComponent(const float re0, Ipp32fc * pData, const int size)
{
	float x0(0.f);
	if (m_typeNorm == norm_N)
		x0 = 0.5f * re0;
	else if (m_typeNorm == norm_Sqrt_N)
		x0 = (float)(0.5 * re0 * m_sqrt_N);
	
	if (x0 != 0.f)		// IsNotEqual - нельзя!!!
	{
		for (int i = 0; i < size; ++i)
			pData[i].re += x0;
	}
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Приватная функция наложения временного окна. Приватная функция. Ничего
// проверять не надо
bool CCalcFFT_32f::SetTimeWindow(const int sizeData)
{
	bool ret(true);
	if ((int)m_saWindow.size != sizeData)
	{
		float factorWindow = 1.f;
		m_saWindow.ReSize(sizeData);
		ippsSet_32f(1.f, m_saWindow.pData, sizeData);

		if (m_typeWin != win_no)
		{
			switch (m_typeWin)
			{
			case win_Bartlett:
				ret = ippStsNoErr == ippsWinBartlett_32f_I(m_saWindow.pData, sizeData);
				break;

			case win_Blackman:
				ret = ippStsNoErr == ippsWinBlackmanStd_32f_I(m_saWindow.pData, sizeData);
				break;

			case win_Hamming:
				ret = ippStsNoErr == ippsWinHamming_32f_I(m_saWindow.pData, sizeData);
				break;

			case win_Hann:
				ret = ippStsNoErr == ippsWinHann_32f_I(m_saWindow.pData, sizeData);
				break;

			case win_Kaiser:
				ret = ippStsNoErr == ippsWinKaiser_32f_I(m_saWindow.pData, sizeData, 1.f);
				break;

			case win_Hanning:			// окно Хэннинга
				WinHanning_I(m_saWindow.pData, sizeData);
				break;

			case win_RifeVincent_4:		// окно Рифа-Винсента (4)
				WinRifeVincent_4_I(m_saWindow.pData, sizeData);
				break;

			case win_BlackmanHarris_3:	// окно Блэкмана-Харриса (3)
				WinBlackmanHarris_N_I(m_saWindow.pData, sizeData, 3);
				break;

			case win_BlackmanHarris_4:	// окно Блэкмана-Харриса (4)
				WinBlackmanHarris_N_I(m_saWindow.pData, sizeData, 4);
				break;

			case win_Nuttall:			// окно Наталла
				WinNuttall_I(m_saWindow.pData, sizeData);
				break;

			case win_BlackmanNuttall:	// окно Блэкмана-Наталла
				WinBlackmanNuttall_I(m_saWindow.pData, sizeData);
				break;

			case win_Flattop:			// окно с плоской вершиной
				WinFlattop_I(m_saWindow.pData, sizeData);
				break;

			default:
				break;
			}
			ret &= ippStsNoErr == ippsMean_32f(m_saWindow.pData, sizeData, &factorWindow, ippAlgHintAccurate);
		}
		m_factorWindow = factorWindow;
	}
	return ret;
}
//------------------------------------------------------------------------------
// Приватная функция. 
// Исходные данные: pData, размер = sizeData.
// Выходные данные: m_saSrcFwd_Re.pData, размер = m_sizeFFT
// Если sizeData < m_sizeFFT, то добавляю нули.
// Выполняется проверка, копирование, центровка данных и наложение временного окна.
long CCalcFFT_32f::Create_ReDataForFwd(float * pData, const int sizeData, int * psize)
{
	if (pData == nullptr)
		return Err_NULL_Pointer;
	if (sizeData < 0)
		return Err_Size_Zero;

	long ret = m_saFwd_SrcRe.ReSize(m_sizeFFT) ? 0 : Err_Allocate_Memory;
	if (ret == 0)
	{
		int size = (sizeData == 0) ? m_sizeFFT : sizeData;
		bool bSizeSmall = size < m_sizeFFT;
		if (!bSizeSmall)
			size = m_sizeFFT;

		bool bIppOk = ippStsNoErr == ippsCopy_32f(pData, m_saFwd_SrcRe.pData, size);
		if (bSizeSmall)
		{
			bIppOk &= ippStsNoErr == ippsZero_32f(m_saFwd_SrcRe.pData + size, m_sizeFFT - size);
		}
		if (m_bCenteringSrcData)
		{
			bIppOk &= RemovingConstComponent_32f_I(m_saFwd_SrcRe.pData, size);
		}
		if (m_typeWin != win_no)
		{
			bIppOk &= SetTimeWindow(size) &&
				(ippStsNoErr == ippsMul_32f_I(m_saWindow.pData, m_saFwd_SrcRe.pData, size));
		}
		// -----------------------------------------
		if (bIppOk)
		{
			if (psize != nullptr)
				*psize = size;
		}
		else
		{
			if (psize != nullptr)
				*psize = 0;
			ret = Err_Calc;
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
// Прямое FFT.
// Преобразование реального сигнала в комплексный спектр.
// - pSrc - реальные входные данные с размером от 1 до m_sizeFFT;
// - pDst - комплексные выходные данные с размером m_sizeFFT;
// - sizeSrc - размер входного массива.
// Если sizeSrc = 0, то размер входного массива = m_sizeFFT.
// Если sizeSrc меньше m_sizeFFT, то перед расчетами во входной массив
// добавляются нули. Если больше - то лишние данные игнорируются.
// Возвращается код ошибки ErrorCodes.
//
long CCalcFFT_32f::CalcFwd_RToC(float * pSrc, Ipp32fc * pDst, const int sizeSrc)
{
	long ret = Create_ReDataForFwd(pSrc, sizeSrc, nullptr);
	if (ret == 0)
	{
		ret = Create_Structure_R();
		if (ret == 0)
		{
			if (m_saDataPerm.ReSize(m_sizeFFT))
			{
				m_factorTypeSrc = 2;	// экспериментальный факт
				Calc_FactorFFT();

				if ((ippStsNoErr == ippsFFTFwd_RToPerm_32f(
										m_saFwd_SrcRe.pData, m_saDataPerm.pData,
										m_pStruct_R, m_saBuffer_8u.pData)) &&
					(ippStsNoErr == ippsConjPerm_32fc(
										m_saDataPerm.pData, pDst, m_sizeFFT)))
				{
					MulC_ComplexMass_I(m_factorFFT, pDst, m_sizeFFT);
					CorrectZeroItem(pDst);
				}
				else
					ret = Err_Calc;
			}
			else
				ret = Err_Allocate_Memory;
		}
	}
	if (ret != 0)
		ippsZero_32fc(pDst, m_sizeFFT);
	return ret;
}
//------------------------------------------------------------------------------
// Прямое FFT. Преобразование комплексного сигнала с нулевой мнимой частью
// в комплексный спектр.
// - pSrc - реальные входные данные Re с размером от 1 до m_sizeFFT;
// - pDst - комплексные выходные данные с размером m_sizeFFT;
// - sizeSrc - размер входного массива.
// Если sizeSrc = 0, то размер входного массива = m_sizeFFT.
// Если sizeSrc меньше m_sizeFFT, то перед расчетами во входной массив
// добавляются нули. Если больше - то лишние данные игнорируются.
// Возвращается код ошибки ErrorCodes.
//
long CCalcFFT_32f::CalcFwd_C0ToC(float * pSrc, Ipp32fc * pDst, const int sizeSrc)
{
	long ret = Create_ReDataForFwd(pSrc, sizeSrc, nullptr);
	if (ret == 0)
	{
		ret = Create_Structure_C();
		if (ret == 0)
		{
			if (m_saFwd_SrcIm.ReSize(m_sizeFFT) &&
				m_saFwd_SrcC.ReSize(m_sizeFFT))
			{
				m_factorTypeSrc = 2;	// экспериментальный факт
				Calc_FactorFFT();
				if ((ippStsNoErr == ippsZero_32f(m_saFwd_SrcIm.pData, m_sizeFFT)) &&
					(ippStsNoErr == ippsRealToCplx_32f(m_saFwd_SrcRe.pData, m_saFwd_SrcIm.pData,
					m_saFwd_SrcC.pData, m_sizeFFT)) &&
					(ippStsNoErr == ippsFFTFwd_CToC_32fc(m_saFwd_SrcC.pData, pDst,
					m_pStruct_C, m_saBuffer_8u.pData)))
				{
					MulC_ComplexMass_I(m_factorFFT, pDst, m_sizeFFT);
					CorrectZeroItem(pDst);
				}
				else
					ret = Err_Calc;
			}
			else
				ret = Err_Allocate_Memory;
		}
	}
	if (ret != 0)
		ippsZero_32fc(pDst, m_sizeFFT);
	return ret;
}
//------------------------------------------------------------------------------
// Прямое FFT. Преобразование комплексного сигнала с мнимой частью,
// определяемой с помощью преобразования Гильберта, в комплексный спектр.
// - pSrc - реальные входные данные Re с размером от 1 до m_sizeFFT;
// - pDst - комплексные выходные данные с размером m_sizeFFT;
// - sizeSrc - размер входного массива.
// Если sizeSrc = 0, то размер входного массива = m_sizeFFT.
// Если sizeSrc меньше m_sizeFFT, то перед расчетами во входной массив
// добавляются нули. Если больше - то лишние данные игнорируются.
// Возвращается код ошибки ErrorCodes.
//
long CalcFwd_CHToC(float * pSrc, Ipp32fc * pDst, const int sizeSrc = 0);
//
long CCalcFFT_32f::CalcFwd_CHToC(float * pSrc, Ipp32fc * pDst, const int sizeSrc)
{
	int size(0);
	long ret = Create_ReDataForFwd(pSrc, sizeSrc, &size);
	if (ret == 0)
	{
		ret = Create_Structure_C();
		if (ret == 0)
		{
			ret = Create_Structure_Hilb(size);
			if (ret == 0)
			{
				if (m_saFwd_SrcC.ReSize(m_sizeFFT))
				{
					m_factorTypeSrc = 1;	// экспериментальный факт
					Calc_FactorFFT();
					bool bOk = (ippStsNoErr == ippsHilbert_32f32fc(m_saFwd_SrcRe.pData, m_saFwd_SrcC.pData,
															m_pStruct_Hilb, m_saStruct_WorkHilb.pData));
					if (size < m_sizeFFT)
						bOk &= ippStsNoErr == ippsZero_32fc(m_saFwd_SrcC.pData + size, m_sizeFFT - size);
					bOk &= (ippStsNoErr == ippsFFTFwd_CToC_32fc(m_saFwd_SrcC.pData, pDst,
											m_pStruct_C, m_saBuffer_8u.pData));
					if (bOk)
					{
						MulC_ComplexMass_I(m_factorFFT, pDst, m_sizeFFT);
						CorrectZeroItem(pDst);
					}
					else
						ret = Err_Calc;
				}
				else
					ret = Err_Allocate_Memory;
			}
		}
	}
	if (ret != 0)
		ippsZero_32fc(pDst, m_sizeFFT);
	return ret;
}
//------------------------------------------------------------------------------// Расчёт обратного FFT. Преобразование комплексного спектра в
// реальный сигнал.
// - pSrc - комплексные входные данные размером m_sizeFFT;
// - pDst - реальные выходные данные размером m_sizeFFT.
// Возвращается код ошибки ErrorCodes.
//
long CCalcFFT_32f::CalcInv_CToR(Ipp32fc * pSrc, float * pDst)
{
	if ((pSrc == nullptr) || (pDst == nullptr))
		return Err_NULL_Pointer;

	long ret = Create_Structure_R();
	if (ret == 0)
	{
		if (ConvertToPerm(pSrc))
		{
			if (ippStsNoErr == ippsFFTInv_PermToR_32f(
				(Ipp32f*)m_saDataPerm.pData, pDst, m_pStruct_R, m_saBuffer_8u.pData))
			{
				ippsMulC_32f_I(0.5f, pDst, m_sizeFFT);
				CorrectConstComponent(m_saDataPerm.pData[0], pDst, m_sizeFFT);
			}
			else
				ret = Err_Calc;
		}
	}
	if ((ret != 0) && (pDst != nullptr))
		ippsZero_32f(pDst, m_sizeFFT);
	return ret;
}
//------------------------------------------------------------------------------
// Расчёт обратного FFT. Преобразование комплексного спектра в
// комплексный сигнал.
// - pSrc - комплексные входные данные размером m_sizeFFT;
// - pDst - комплексные выходные данные размером m_sizeFFT.
// Возвращается код ошибки ErrorCodes.
//
long CCalcFFT_32f::CalcInv_CToC(Ipp32fc * pSrc, Ipp32fc * pDst)
{
	if ((pSrc == nullptr) || (pDst == nullptr))
		return Err_NULL_Pointer;

	long ret = Create_Structure_C();
	if (ret == 0)
	{
		if (ippStsNoErr == ippsFFTInv_CToC_32fc(pSrc, pDst, m_pStruct_C, m_saBuffer_8u.pData))
		{
			MulC_ComplexMass_I(0.5f, pDst, m_sizeFFT);
			CorrectConstComponent(pSrc[0].re, pDst, m_sizeFFT);
		}
		else
			ret = Err_Calc;
	}
	if ((ret != 0) && (pDst != nullptr))
		ippsZero_32fc(pDst, m_sizeFFT);
	return ret;
}
//------------------------------------------------------------------------------
// Расчёт обратного FFT. Преобразование комплексного спектра в
// комплексный сигнал.
// - pSrcDst - комплексные входные и выходные данные размером m_sizeFFT;
// Возвращается код ошибки ErrorCodes.
long CCalcFFT_32f::CalcInv_CToC_I(Ipp32fc * pSrcDst)
{
	if (pSrcDst == nullptr)
		return Err_NULL_Pointer;

	long ret = Create_Structure_C();
	if (ret == 0)
	{
		float re0 = pSrcDst[0].re;
		if (ippStsNoErr == ippsFFTInv_CToC_32fc_I(pSrcDst, m_pStruct_C, m_saBuffer_8u.pData))
		{
			MulC_ComplexMass_I(0.5f, pSrcDst, m_sizeFFT);
			CorrectConstComponent(re0, pSrcDst, m_sizeFFT);
		}
		else
			ret = Err_Calc;
	}
	if ((ret != 0) && (pSrcDst != nullptr))
		ippsZero_32fc(pSrcDst, m_sizeFFT);
	return ret;
}
//------------------------------------------------------------------------------