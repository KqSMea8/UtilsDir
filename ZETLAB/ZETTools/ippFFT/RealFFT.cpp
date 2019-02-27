//------------------------------------------------------------------------------
//	Файл RealFFT.cpp
//	Зеленоград, ЗАО "ЭТМС", Савостин Ю.М.
//------------------------------------------------------------------------------
#include "StdAfx.h"
#include <ippFFT\RealFFT.h>
//------------------------------------------------------------------------------
CRealFFT::CRealFFT()
	: m_bInit(false)
	, m_WeightFunc(twf_Hamming)
	, m_pFreqADC(NULL)
	, m_order(0)
	, m_sizeDataOut(0)
	, m_sizeBuffer_8u(0)
	, m_sizeBuffer_Inp(0)
	, m_sizeBuffer_Out(0)
	, m_pBuffer_8u(NULL)
	, m_pBuffer_Inp(NULL)
	, m_pBuffer_Out(NULL)
	, m_pFFTSpec(NULL)
{}
//------------------------------------------------------------------------------
CRealFFT::~CRealFFT()
{
	DeleteAllBuffer();
	ippsFFTFree_R_32f(m_pFFTSpec);
}
//------------------------------------------------------------------------------
void CRealFFT::CreateAllBuffer(int sizeBuffer_8u,
	int sizeBuffer_Inp_32f, int sizeBuffer_Out_32f)
{
	if (sizeBuffer_8u > 0)
	{
		m_sizeBuffer_8u = sizeBuffer_8u;
		try
		{
			m_pBuffer_8u = new Ipp8u[m_sizeBuffer_8u];
			ZeroMemory(m_pBuffer_8u, m_sizeBuffer_8u);	// sizeof(Ipp8u) = 1
		//	ippsZero_8u(m_pBuffer_8u, m_sizeBuffer_8u);
		}
		catch (...)
		{
			m_pBuffer_8u = NULL;
			m_sizeBuffer_8u = 0;
		}
	}
	if (sizeBuffer_Inp_32f > 0)
	{
		m_sizeBuffer_Inp = sizeBuffer_Inp_32f;
		try
		{
			m_pBuffer_Inp = new float[m_sizeBuffer_Inp];
			ippsZero_32f(m_pBuffer_Inp, m_sizeBuffer_Inp);
		}
		catch (...)
		{
			m_pBuffer_Inp = NULL;
			m_sizeBuffer_Inp = 0;
		}
	}
	if (sizeBuffer_Out_32f > 0)
	{
		m_sizeBuffer_Out = sizeBuffer_Out_32f;
		try
		{
			m_pBuffer_Out = new float[m_sizeBuffer_Out];
			ippsZero_32f(m_pBuffer_Out, m_sizeBuffer_Out);
		}
		catch (...)
		{
			m_pBuffer_Out = NULL;
			m_sizeBuffer_Out = 0;
		}
	}
}
//------------------------------------------------------------------------------
void CRealFFT::DeleteAllBuffer()
{
	if (m_sizeBuffer_8u > 0)
	{
		delete[] m_pBuffer_8u;
		m_pBuffer_8u = NULL;
		m_sizeBuffer_8u = 0;
	}
	if (m_sizeBuffer_Inp > 0)
	{
		delete[] m_pBuffer_Inp;
		m_pBuffer_Inp = NULL;
		m_sizeBuffer_Inp = 0;
	}
	if (m_sizeBuffer_Out > 0)
	{
		delete[] m_pBuffer_Out;
		m_pBuffer_Out = NULL;
		m_sizeBuffer_Out = 0;
	}
}
//------------------------------------------------------------------------------
bool CRealFFT::WeightFunction(float *pDataInp, float *pDataOut)
{// Функция приватная, перед её вызовом выполняются необходимые проверки
	bool ret = false;
	switch (m_WeightFunc)
	{
	case twf_no:
		ret = ippsCopy_32f(pDataInp, pDataOut, m_sizeBuffer_Inp) == ippStsNoErr;
		break;
	case twf_Hamming:
		ret = ippsWinHamming_32f(pDataInp, pDataOut, m_sizeBuffer_Inp) == ippStsNoErr;		
		break;
	case twf_Hann:
		ret = ippsWinHann_32f(pDataInp, pDataOut, m_sizeBuffer_Inp) == ippStsNoErr;	
		break;
	case twf_Bartlett:
		ret = ippsWinBartlett_32f(pDataInp, pDataOut, m_sizeBuffer_Inp) == ippStsNoErr;
	//	break;
	//case twf_Kaiser:
	//	ret = ippsWinKaiser_32f(pDataInp, pDataOut, m_sizeBuffer_Inp_32f, 1.f) == ippStsNoErr;
	//	break;
	}
	return ret;
}
//------------------------------------------------------------------------------
bool CRealFFT::Create(float *pFreqADC, DWORD order,
	TypeWeightFunction weightFunc)
{
	m_bInit = (pFreqADC != NULL) && (order < 32);
	if (m_bInit)
	{
		m_pFreqADC = pFreqADC;
		m_bInit = SetOrder(order) && SetWeightFunction(weightFunc);
	}
	return m_bInit;
}
//------------------------------------------------------------------------------
bool CRealFFT::SetOrder(DWORD val)
{
	bool ret = (m_order != val) && (val > 0);
	if (ret)
	{
		if (m_pFFTSpec != NULL)
			ippsFFTFree_R_32f(m_pFFTSpec);
		DeleteAllBuffer();

		ret = ippsFFTInitAlloc_R_32f(
			&m_pFFTSpec, val, IPP_FFT_DIV_INV_BY_N, ippAlgHintNone)	//ippAlgHintFast
			== ippStsNoErr;
		if (ret)
		{
			m_order = val;
			m_sizeBuffer_Inp = 2;
			for (int i = 1; i < m_order; ++i)
				m_sizeBuffer_Inp *= 2;
			m_sizeDataOut = m_sizeBuffer_Inp / 2;
			m_sizeBuffer_Out = m_sizeBuffer_Inp + 2;
			ret = ippsFFTGetBufSize_R_32f(m_pFFTSpec, &m_sizeBuffer_8u)
				== ippStsNoErr;
			if (ret)
			{
				CreateAllBuffer(m_sizeBuffer_8u,
					m_sizeBuffer_Inp, m_sizeBuffer_Out);
				// На шустрых Intel-овских ЦП m_sizeBuffer_8u = 0, поэтому так:
				ret = (m_sizeBuffer_Inp > 0) && (m_sizeBuffer_Out > 0);
			}
		}
		else
			m_pFFTSpec = NULL;
		m_bInit = ret;
	}
	else
		ret = m_bInit;
	return ret;
}
//------------------------------------------------------------------------------
bool CRealFFT::SetWeightFunction(TypeWeightFunction val)
{
	bool ret = (DWORD)(int)val <= Number_Type_Weight_Function;
	if (ret)
		m_WeightFunc = val;
	return ret;
}
//------------------------------------------------------------------------------
float CRealFFT::GetFrequencyResolution()
{
	float ret;
	if (m_bInit)
		ret = *m_pFreqADC / m_sizeBuffer_Inp;
	else
		ret = 0.f;
	return ret;
}
//------------------------------------------------------------------------------
float CRealFFT::BandMiddleFreq(int indxBand)
{
	float ret;
	if (m_bInit && (0 <= indxBand) && (indxBand < m_sizeDataOut))
		ret = *m_pFreqADC / m_sizeBuffer_Inp * (indxBand + 0.5f);
	else
		ret = 0.f;
	return ret;
}
//------------------------------------------------------------------------------
bool CRealFFT::Forward_ToCplx(float *pDataInp, Ipp32fc *pDataOut)
{// копировать надо, т.к. размер массива m_pBuffer_Out_32f больше
// размера массива pDataOut.
	bool ret = m_bInit && (pDataInp != NULL) &&
		WeightFunction(pDataInp, m_pBuffer_Inp) &&
		(ippsFFTFwd_RToCCS_32f(m_pBuffer_Inp, m_pBuffer_Out,
								m_pFFTSpec, m_pBuffer_8u) == ippStsNoErr);
	if (ret && (pDataOut != NULL))
		ret = ippsCopy_32fc((Ipp32fc*)m_pBuffer_Out, pDataOut, m_sizeDataOut)
															== ippStsNoErr;
	return ret;
}
//------------------------------------------------------------------------------
bool CRealFFT::Forward_ToMagn(float *pDataInp, float *pDataOut)
{
	bool ret = m_bInit && (pDataInp != NULL) &&
		WeightFunction(pDataInp, m_pBuffer_Inp) &&
		(ippsFFTFwd_RToCCS_32f(m_pBuffer_Inp, m_pBuffer_Out,
								m_pFFTSpec, m_pBuffer_8u) == ippStsNoErr);
	if (ret)
	{
		if (pDataOut != NULL)
			ret = ippsMagnitude_32fc((Ipp32fc*)m_pBuffer_Out, pDataOut,
									m_sizeDataOut) == ippStsNoErr;
		else
			ret = ippsMagnitude_32fc((Ipp32fc*)m_pBuffer_Out, m_pBuffer_Inp,
									m_sizeDataOut) == ippStsNoErr;
	}
	return ret;
}
//------------------------------------------------------------------------------