//------------------------------------------------------------------------------
//	���� RealFFT.h
//	����������, ��� "����", �������� �.�.
//------------------------------------------------------------------------------
/*		����� ������������ ��� ������� ������������� �����-��������������
�������� ������� ��������. ����� ���������� ������� �������������� �����,
������������� ��������� ���������� IPP 7.0. �������������� ������� ������ �����
��������������� � ������� ��������� ������� ������� �������.
		������ �������� ������ = 2^order. ������ �������� ������ ����� ������.
������� ����� ������ � ����� ����� � ������� ������� SetOrder.
		� �������� �������� ������ ������� ������� ������������� FFT �����
������������ ���������� ������� ������.
*/
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include "math.h"
#include <Intel\ipp\ipps.h>
//------------------------------------------------------------------------------
enum TypeWeightFunction
{// �������� ����� ������� �������
	twf_no,
	twf_Hamming,
	twf_Hann,
	twf_Bartlett,
//	twf_Blackman,
//	twf_Kaiser,
};
#define Number_Type_Weight_Function 3	// ���-�� ������������ �����
//------------------------------------------------------------------------------
class CRealFFT
{
private:
	bool m_bInit;
	TypeWeightFunction m_WeightFunc;
	float *m_pFreqADC;
	int m_order;
	int m_sizeDataOut;
	// ���������� ������ ��� ���������� �������� ����������
	int m_sizeBuffer_8u;
	Ipp8u *m_pBuffer_8u;
	int m_sizeBuffer_Inp;
	float *m_pBuffer_Inp;
	int m_sizeBuffer_Out;
	float *m_pBuffer_Out;
	// ����������� ���������-���������
	IppsFFTSpec_R_32f *m_pFFTSpec;

	void CreateAllBuffer(int sizeBuffer_8u,
		int sizeBuffer_Inp_32f, int sizeBuffer_Out_32f);
	void DeleteAllBuffer();
	bool WeightFunction(float *pDataInp, float *pDataOut);

protected:

public:
	CRealFFT();
	virtual ~CRealFFT();
	bool Create(float *pFreqADC, DWORD order,
		TypeWeightFunction weightFunc = twf_Hamming);

	// ������ ������/������ ������� -----------------
	TypeWeightFunction GetWeightFunction() { return m_WeightFunc; }
	bool SetWeightFunction(TypeWeightFunction val);

	float GetFrequencyResolution();

	float BandMiddleFreq(int indxBand);

	DWORD GetOrder() { return m_order; }
	bool SetOrder(DWORD val);

	// ������� ������ ����� Create ��� SetOrder
	DWORD GetSizeForInputData() { return (DWORD)m_sizeBuffer_Inp; }
	DWORD GetSizeForOutputData() { return (DWORD)m_sizeDataOut; }
	
	// ��������� �� ��������� ��� pDataOut = NULL
	float* GetPointerToResultMagn() { return m_pBuffer_Inp; }
	Ipp32fc* GetPointerToResultCplx() { return (Ipp32fc*)m_pBuffer_Out; }

	// ������ ������ ----------------------------
	// ������ ��������������, ��������� ������� � ����������� ������.
	// pDataOut ����� = NULL, ����� ��������� �� ������ GetPointerToResultCplx
	bool Forward_ToCplx(float *pDataInp, Ipp32fc *pDataOut);
	// ������ ��������������, ��������� - ��������� (������ ���������� ��
	// ����� ��������� Re � Im). ������ pDataInp ����� ������ ������� pDataOut.
	// pDataOut ����� ��������� pDataInp.
	// pDataOut ����� = NULL, ����� ��������� �� ������ GetPointerToResultMagn
	bool Forward_ToMagn(float *pDataInp, float *pDataOut);
};
//------------------------------------------------------------------------------

