//------------------------------------------------------------------------------
// ���� CalcFFT.cpp
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "CalcFFT_64.h"
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
CCalcFFT_64f::CCalcFFT_64f(const int order) : CCustomCalcFFT(order)
	, m_pStruct_R(nullptr)
	, m_pStruct_C(nullptr)
{// ��������� ����� ������ ����� ����������
	m_factorFFT = m_factorTypeSrc / m_factorWindow;
}
//------------------------------------------------------------------------------
CCalcFFT_64f::~CCalcFFT_64f()
{
}
//------------------------------------------------------------------------------
CCalcFFT_64f::CCalcFFT_64f(const CCalcFFT_64f & val) : CCustomCalcFFT(val)
	, m_pStruct_R(nullptr)
	, m_pStruct_C(nullptr)
{
	*this = val;
}
//------------------------------------------------------------------------------
CCalcFFT_64f & CCalcFFT_64f::operator = (const CCalcFFT_64f & val)
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
void CCalcFFT_64f::DeleteStructure_FFT()
{// ��������� �������
	m_pStruct_R = nullptr;
	m_pStruct_C = nullptr;
}
//------------------------------------------------------------------------------
void CCalcFFT_64f::DeleteStructure_All()
{
	DeleteStructure_FFT();
}
//------------------------------------------------------------------------------
void CCalcFFT_64f::ReSize_saWindow(DWORD size)
{
	m_saWindow.ReSize(size);
}
//------------------------------------------------------------------------------
long CCalcFFT_64f::Create_Structure_R()
{// �������� ��������� ��� ������� �������������� Real => Compl. ��������� �������
	long ret = (m_pStruct_R != nullptr) ? 0 : Err_Initialization;
	if (ret != 0)
	{
		int flag = GetFlagForFwd();
		int sizeFFTSpec(0), sizeFFTInitBuf(0), sizeFFTWorkBuf(0);
		if ((ippStsNoErr == ippsFFTGetSize_R_64f(m_nOrderFFT, flag, ippAlgHintAccurate, &sizeFFTSpec, &sizeFFTInitBuf, &sizeFFTWorkBuf)) &&
			m_saStruct_R.ReSize(sizeFFTSpec) &&
			m_saBuffer_8u.ReSize(sizeFFTInitBuf) &&
			(ippStsNoErr == ippsFFTInit_R_64f(&m_pStruct_R, m_nOrderFFT, flag, ippAlgHintAccurate, m_saStruct_R.pData, m_saBuffer_8u.pData)) &&
			m_saBuffer_8u.ReSize(sizeFFTWorkBuf))
			ret = 0;
	}
	return ret;
}
//------------------------------------------------------------------------------
long CCalcFFT_64f::Create_Structure_C()
{// �������� ��������� ��� ������� �������������� Compl => Compl. ��������� �������
	long ret = (m_pStruct_C != nullptr) ? 0 : Err_Initialization;
	if (ret != 0)
	{
		int flag = GetFlagForFwd();
		int sizeFFTSpec(0), sizeFFTInitBuf(0), sizeFFTWorkBuf(0);
		if ((ippStsNoErr == ippsFFTGetSize_C_64fc(m_nOrderFFT, flag, ippAlgHintAccurate, &sizeFFTSpec, &sizeFFTInitBuf, &sizeFFTWorkBuf)) &&
			m_saStruct_C.ReSize(sizeFFTSpec) &&
			m_saBuffer_8u.ReSize(sizeFFTInitBuf) &&
			(ippStsNoErr == ippsFFTInit_C_64fc(&m_pStruct_C, m_nOrderFFT, flag, ippAlgHintAccurate, m_saStruct_C.pData, m_saBuffer_8u.pData)) &&
			m_saBuffer_8u.ReSize(sizeFFTWorkBuf))
			ret = 0;
	}
	return ret;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------	
//------------------------------------------------------------------------------
void CCalcFFT_64f::Calc_FactorFFT()
{
	m_factorFFT = m_factorTypeSrc / m_factorWindow;
}
//------------------------------------------------------------------------------
void CCalcFFT_64f::CorrectZeroItem(Ipp64fc * pData)
{
	if (m_factorTypeSrc > 1)
	{
		pData[0].re /= 2;
		pData[m_sizeFFT_2].re /= 2;
	}
}
//------------------------------------------------------------------------------
bool CCalcFFT_64f::ConvertToPerm(Ipp64fc * pData)
{
	bool ret = m_saDataPerm.ReSize(m_sizeFFT) &&
		(ippStsNoErr == ippsCopy_64f((double*)pData, m_saDataPerm.pData, m_sizeFFT));
	if (ret)
	{
		m_saDataPerm.pData[1] = pData[m_sizeFFT_2].re;		
	}
	return ret;
}
//------------------------------------------------------------------------------
// ��������� �������. ��������� ������ �� ����
void CCalcFFT_64f::CorrectConstComponent(const double re0, double * pData, const int size)
{
	double x0(0.f);
	if (m_typeNorm == norm_N)
		x0 = 0.5 * re0;
	else if (m_typeNorm == norm_Sqrt_N)
		x0 = 0.5 * re0 * m_sqrt_N;

	if (x0 != 0.f)		// IsNotEqual - ������!!!
		ippsAddC_64f_I(x0, pData, size);
}
//------------------------------------------------------------------------------
// ��������� �������. ��������� ������ �� ����
void CCalcFFT_64f::CorrectConstComponent(const double re0, Ipp64fc * pData, const int size)
{
	double x0(0.f);
	if (m_typeNorm == norm_N)
		x0 = 0.5 * re0;
	else if (m_typeNorm == norm_Sqrt_N)
		x0 = 0.5 * re0 * m_sqrt_N;
	
	if (x0 != 0.f)		// IsNotEqual - ������!!!
	{
		for (int i = 0; i < size; ++i)
			pData[i].re += x0;
	}
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// ��������� ������� ��������� ���������� ����. ��������� �������. ������
// ��������� �� ����
bool CCalcFFT_64f::SetTimeWindow(const int sizeData)
{
	bool ret(true);
	if ((int)m_saWindow.size != sizeData)
	{
		m_saWindow.ReSize(sizeData);
		ippsSet_64f(1., m_saWindow.pData, sizeData);
		m_factorWindow = 1.;
		m_factorFFT = 1.;

		if (m_typeWin != win_no)
		{
			switch (m_typeWin)
			{
			case win_Bartlett:
				ret = ippStsNoErr == ippsWinBartlett_64f_I(m_saWindow.pData, sizeData);
				break;

			case win_Blackman:
				ret = ippStsNoErr == ippsWinBlackmanStd_64f_I(m_saWindow.pData, sizeData);
				break;

			case win_Hamming:
				ret = ippStsNoErr == ippsWinHamming_64f_I(m_saWindow.pData, sizeData);
				break;

			case win_Hann:
				ret = ippStsNoErr == ippsWinHann_64f_I(m_saWindow.pData, sizeData);
				break;

			case win_Kaiser:
				ret = ippStsNoErr == ippsWinKaiser_64f_I(m_saWindow.pData, sizeData, 1.f);
				break;

			case win_Hanning:			// ���� ��������
				WinHanning_I(m_saWindow.pData, sizeData);
				break;

			case win_RifeVincent_4:		// ���� ����-�������� (4)
				WinRifeVincent_4_I(m_saWindow.pData, sizeData);
				break;

			case win_BlackmanHarris_3:	// ���� ��������-������� (3)
				WinBlackmanHarris_N_I(m_saWindow.pData, sizeData, 3);
				break;

			case win_BlackmanHarris_4:	// ���� ��������-������� (4)
				WinBlackmanHarris_N_I(m_saWindow.pData, sizeData, 4);
				break;

			case win_Nuttall:			// ���� �������
				WinNuttall_I(m_saWindow.pData, sizeData);
				break;

			case win_BlackmanNuttall:	// ���� ��������-�������
				WinBlackmanNuttall_I(m_saWindow.pData, sizeData);
				break;

			case win_Flattop:			// ���� � ������� ��������
				WinFlattop_I(m_saWindow.pData, sizeData);
				break;

			default:
				break;
			}
			// ������ ������������ ������� ���������� ����
			bool bFlag = ippStsNoErr == ippsMean_64f(m_saWindow.pData, sizeData, &m_factorWindow);
			ret &= bFlag;
			// ������ ��������������� ������� ���������� ����
			SmartArray<double> saTmp(sizeData);
			bFlag = (ippStsNoErr == ippsSqr_64f(m_saWindow.pData, saTmp.pData, sizeData)) &&
				(ippStsNoErr == ippsMean_64f(saTmp.pData, sizeData, &m_factorEnergyWindow));
			ret &= bFlag;
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
// ��������� �������. 
// �������� ������: pData, ������ = sizeData.
// �������� ������: m_saSrcFwd_Re.pData, ������ = m_sizeFFT
// ���� sizeData < m_sizeFFT, �� �������� ����.
// ����������� ��������, �����������, ��������� ������ � ����.
long CCalcFFT_64f::Create_ReDataForFwd(double * pData, const int sizeData, int * psize)
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

		bool bIppOk = ippStsNoErr == ippsCopy_64f(pData, m_saFwd_SrcRe.pData, size);
		if (bSizeSmall)
		{
			bIppOk &= ippStsNoErr == ippsZero_64f(m_saFwd_SrcRe.pData + size, m_sizeFFT - size);
		}
		if (m_bCenteringSrcData)
		{
			bIppOk &= RemovingConstComponent_64f_I(m_saFwd_SrcRe.pData, size);
		}
		if (m_typeWin != win_no)
		{
			bIppOk &= SetTimeWindow(size) &&
				(ippStsNoErr == ippsMul_64f_I(m_saWindow.pData, m_saFwd_SrcRe.pData, size));
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
// ������ FFT.
// �������������� ��������� ������� � ����������� ������.
// - pSrc - �������� ������� ������ � �������� �� 1 �� m_sizeFFT;
// - pDst - ����������� �������� ������ � �������� m_sizeFFT;
// - sizeSrc - ������ �������� �������.
// ���� sizeSrc = 0, �� ������ �������� ������� = m_sizeFFT.
// ���� sizeSrc ������ m_sizeFFT, �� ����� ��������� �� ������� ������
// ����������� ����. ���� ������ - �� ������ ������ ������������.
// ������������ ��� ������ ErrorCodes.
//
long CCalcFFT_64f::CalcFwd_RToC(double * pSrc, Ipp64fc * pDst, const int sizeSrc)
{
	long ret = Create_ReDataForFwd(pSrc, sizeSrc, nullptr);
	if (ret == 0)
	{
		ret = Create_Structure_R();
		if (ret == 0)
		{
			if (m_saDataPerm.ReSize(m_sizeFFT))
			{
				m_factorTypeSrc = 2;	// ����������������� ����
				Calc_FactorFFT();

				if ((ippStsNoErr == ippsFFTFwd_RToPerm_64f(
										m_saFwd_SrcRe.pData, m_saDataPerm.pData,
										m_pStruct_R, m_saBuffer_8u.pData)) &&
					(ippStsNoErr == ippsConjPerm_64fc(
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
		ippsZero_64fc(pDst, m_sizeFFT);
	return ret;
}
//------------------------------------------------------------------------------
// ������ FFT. �������������� ������������ ������� � ������� ������ ������
// � ����������� ������.
// - pSrc - �������� ������� ������ Re � �������� �� 1 �� m_sizeFFT;
// - pDst - ����������� �������� ������ � �������� m_sizeFFT;
// - sizeSrc - ������ �������� �������.
// ���� sizeSrc = 0, �� ������ �������� ������� = m_sizeFFT.
// ���� sizeSrc ������ m_sizeFFT, �� ����� ��������� �� ������� ������
// ����������� ����. ���� ������ - �� ������ ������ ������������.
// ������������ ��� ������ ErrorCodes.
//
long CCalcFFT_64f::CalcFwd_C0ToC(double * pSrc, Ipp64fc * pDst, const int sizeSrc)
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
				m_factorTypeSrc = 2;	// ����������������� ����
				Calc_FactorFFT();
				if ((ippStsNoErr == ippsZero_64f(m_saFwd_SrcIm.pData, m_sizeFFT)) &&
					(ippStsNoErr == ippsRealToCplx_64f(m_saFwd_SrcRe.pData, m_saFwd_SrcIm.pData,
					m_saFwd_SrcC.pData, m_sizeFFT)) &&
					(ippStsNoErr == ippsFFTFwd_CToC_64fc(m_saFwd_SrcC.pData, pDst,
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
		ippsZero_64fc(pDst, m_sizeFFT);
	return ret;
}
//------------------------------------------------------------------------------
// ������ FFT. �������������� ������������ ������� � ����������� ������.
// - pSrc - ����������� ������� ������ �������� m_sizeFFT;
// - pDst - ����������� �������� ������ � �������� m_sizeFFT.
// ��� ���������� ���� ������� ������������ �������� �������������
// ��������� ������� � ���������� ���������� ����!
// ������������ ��� ������ ErrorCodes.
long CCalcFFT_64f::CalcFwd_CToC(Ipp64fc * pSrc, Ipp64fc * pDst)
{
	long ret = Create_Structure_C();
	if (ret == 0)
	{	// �.�. � ���� ������ m_factorTypeSrc = 1 �
		// m_factorWindow = 1. (��� ����),
		// �� ������� Calc_FactorFFT, MulToConst � CorrectZeroItem
		// ������ �� ������ � �� ����� �� ��������, ����� �� �������
		// m_factorWindow ��� ������ ������� ������� CalcFwd.
//		m_factorTypeSrc = 1;	// ����������������� ����
//  	m_factorWindow = 1.;	// ��� ����
//  	Calc_FactorFFT();
//  	if (ippStsNoErr == ippsFFTFwd_CToC_64fc(pSrc, pDst, m_pStruct_C, m_saBuffer_8u.pData))
//  	{
//  		MulC_ComplexMass_I(m_factorFFT, pDst, m_sizeFFT);
//  		CorrectZeroItem(pDst);
//  	}
//  	else
//  		ret = Err_Calc;

		if (ippStsNoErr != ippsFFTFwd_CToC_64fc(pSrc, pDst, m_pStruct_C, m_saBuffer_8u.pData))
			ret = Err_Calc;
	}
	return ret;
}

// ������ FFT. �������������� ������������ ������� � ����������� ������.
// - pSrcDst - ����������� ������� � �������� ������ �������� m_sizeFFT.
// ��� ���������� ���� ������� ������������ �������� �������������
// ��������� ������� � ���������� ���������� ����!
// ������������ ��� ������ ErrorCodes.
long CCalcFFT_64f::CalcFwd_CToC_I(Ipp64fc * pSrcDst)
{
	long ret = Create_Structure_C();
	if (ret == 0)
	{	// �.�. � ���� ������ m_factorTypeSrc = 1 �
		// m_factorWindow = 1. (��� ����),
		// �� ������� Calc_FactorFFT, MulToConst � CorrectZeroItem
		// ������ �� ������ � �� ����� �� ��������, ����� �� �������
		// m_factorWindow ��� ������ ������� ������� CalcFwd.
// 		m_factorTypeSrc = 1;	// ����������������� ����
// 		m_factorWindow = 1.;	// ��� ����
// 		Calc_FactorFFT();
// 		if (ippStsNoErr == ippsFFTFwd_CToC_64fc_I(pSrcDst, m_pStruct_C, m_saBuffer_8u.pData))
// 		{
// 			MulC_ComplexMass_I(m_factorFFT, pSrcDst, m_sizeFFT);
// 			CorrectZeroItem(pSrcDst);
// 		}
// 		else
// 			ret = Err_Calc;

		if (ippStsNoErr != ippsFFTFwd_CToC_64fc_I(pSrcDst, m_pStruct_C, m_saBuffer_8u.pData))
			ret = Err_Calc;
	}
	return ret;
}
//------------------------------------------------------------------------------
// ������ ��������� FFT. �������������� ������������ ������� �
// �������� ������.
// - pSrc - ����������� ������� ������ �������� m_sizeFFT;
// - pDst - �������� �������� ������ �������� m_sizeFFT.
// ������������ ��� ������ ErrorCodes.
//
long CCalcFFT_64f::CalcInv_CToR(Ipp64fc * pSrc, double * pDst)
{
	if ((pSrc == nullptr) || (pDst == nullptr))
		return Err_NULL_Pointer;

	long ret = Create_Structure_R();
	if (ret == 0)
	{
		if (ConvertToPerm(pSrc))
		{
			if (ippStsNoErr == ippsFFTInv_PermToR_64f(
				(Ipp64f*)m_saDataPerm.pData, pDst, m_pStruct_R, m_saBuffer_8u.pData))
			{
 				ippsMulC_64f_I(0.5, pDst, m_sizeFFT);
				CorrectConstComponent(m_saDataPerm.pData[0], pDst, m_sizeFFT);
			}
			else
				ret = Err_Calc;
		}
	}
	if ((ret != 0) && (pDst != nullptr))
		ippsZero_64f(pDst, m_sizeFFT);
	return ret;
}
//------------------------------------------------------------------------------
// ������ ��������� FFT. �������������� ������������ ������� �
// ����������� ������.
// - pSrc - ����������� ������� ������ �������� m_sizeFFT;
// - pDst - ����������� �������� ������ �������� m_sizeFFT.
// ������������ ��� ������ ErrorCodes.
//
long CCalcFFT_64f::CalcInv_CToC(Ipp64fc * pSrc, Ipp64fc * pDst)
{
	if ((pSrc == nullptr) || (pDst == nullptr))
		return Err_NULL_Pointer;

	long ret = Create_Structure_C();
	if (ret == 0)
	{
		if (ippStsNoErr == ippsFFTInv_CToC_64fc(pSrc, pDst, m_pStruct_C, m_saBuffer_8u.pData))
		{
			MulC_ComplexMass_I(0.5, pDst, m_sizeFFT);
			CorrectConstComponent(pSrc[0].re, pDst, m_sizeFFT);
		}
		else
			ret = Err_Calc;
	}
	if ((ret != 0) && (pDst != nullptr))
		ippsZero_64fc(pDst, m_sizeFFT);
	return ret;
}
//------------------------------------------------------------------------------
// ������ ��������� FFT. �������������� ������������ ������� �
// ����������� ������.
// - pSrcDst - ����������� ������� � �������� ������ �������� m_sizeFFT;
// ������������ ��� ������ ErrorCodes.
long CCalcFFT_64f::CalcInv_CToC_I(Ipp64fc * pSrcDst)
{
	if (pSrcDst == nullptr)
		return Err_NULL_Pointer;

	long ret = Create_Structure_C();
	if (ret == 0)
	{
		double re0 = pSrcDst[0].re;
		if (ippStsNoErr == ippsFFTInv_CToC_64fc_I(pSrcDst, m_pStruct_C, m_saBuffer_8u.pData))
		{
			MulC_ComplexMass_I(0.5, pSrcDst, m_sizeFFT);
			CorrectConstComponent(re0, pSrcDst, m_sizeFFT);
		}
		else
			ret = Err_Calc;
	}
	if ((ret != 0) && (pSrcDst != nullptr))
		ippsZero_64fc(pSrcDst, m_sizeFFT);
	return ret;
}
//------------------------------------------------------------------------------