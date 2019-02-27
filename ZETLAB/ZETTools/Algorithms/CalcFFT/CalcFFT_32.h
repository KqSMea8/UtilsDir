//------------------------------------------------------------------------------
/* ���� CalcFFT_32.h

	����� CCalcFFT_32f ������������ ��� ���������� �������� ������� � ���������
��� ��� ������ ���� float. ����� �������� ����������� �������� ������ CustomCalcFFT".

	������ ������:
- ������ ��� (����������, ����������� ����������� � �������� ������������);
- ��� ������� ��� ���������� ������� ���;
- ��� ������� ��� ���������� ��������� ���.

	������������ �� �������� ������ �������:
- ������ ������� � ��������� Get � Set;
- ��� ������� ��������� ��������.

	������� ��� ���������� ������� ���
1. CalcFwd_RToC - �������� �������� ������ ������������� � ����������� ������.
������� ����������� ������ ���, ��������� �������� ������� � ������� Perm
(������ ���������� IPP 7.0). ����� ��������� ������������� � ����������� ������.
2. CalcFwd_C0ToC - �������� �������� ������ ������������� � ����������� ������.
������� �������� ����������� ������� ������, � �������� �������� ����� -
�������� ������ �������, � ������ ����� ����� 0. ����� ����������� ������ ���,
��������� �������� ����������� ������.
3. CalcFwd_CHToC - �������� �������� ������ ������������� � ����������� ������.
������� �� �������� ������� ������ � ������� ������������� ��������� ��������
����������� ������, �.�. �������� ������������� ������. ����� �����������
������ ���, ��������� �������� ����������� ������.

	������� ��� ���������� ��������� ���
1. CalcInv_CToC - �������������� ������������ ������� � ����������� ������.
2. CalcInv_CToR - �������������� ������������ ������� � �������� ������. �������
����������� ������ ������������� � ������ ������� Perm. ����� ���������� ��������
���, ����������� �������� �������� �������� ������.
3. CalcInv_CToC_I - �������������� ������������ ������� � ����������� ������.
��� ���� ��� ������������ ������� ������������ ������ � �������� ��������.
*/
//------------------------------------------------------------------------------
#pragma once
#include "CustomCalcFFT.h"
//------------------------------------------------------------------------------
class CCalcFFT_32f : public CCustomCalcFFT
{
private:
	// ������������� ���������, ��������� �� ���������� ���� � ���� ������� ������
	Ipp32f m_factorFFT;

	// ��������� ��� ������� �������������� ���������
	IppsHilbertSpec * m_pStruct_Hilb;
	SmartArray<Ipp8u> m_saStruct_Hilb;
	SmartArray<Ipp8u> m_saStruct_WorkHilb;
//	IppsHilbertSpec_32f32fc * m_pStruct_Hilb;

	// ��������� ��� ������� FFT
	IppsFFTSpec_R_32f * m_pStruct_R;
	IppsFFTSpec_C_32fc * m_pStruct_C;
	SmartArray<Ipp8u> m_saStruct_R;
	SmartArray<Ipp8u> m_saStruct_C;

	SmartArray<float> m_saWindow;		// ��������� ����

	// ������� ��� ������, size = m_sizeFFT
	SmartArray<float> m_saFwd_SrcRe;
	SmartArray<float> m_saFwd_SrcIm;
	SmartArray<Ipp32fc> m_saFwd_SrcC;
	
	// ������ � ������� � ������� Perm, size = m_sizeFFT
	SmartArray<float> m_saDataPerm;

	// ��������� ������� ------------------------------------

	// �������� ��������� ��������
	virtual void DeleteStructure_All();
	virtual void DeleteStructure_FFT();
	virtual void ReSize_saWindow(DWORD size);

	void DeleteStructure_Hilbert();
	long Create_Structure_R();
	long Create_Structure_C();
	long Create_Structure_Hilb(const int size);

	void Calc_FactorFFT();
	long Create_ReDataForFwd(float * pData, const int sizeData, int * psize);
	bool SetTimeWindow(const int sizeData);
	void CorrectZeroItem(Ipp32fc * pData);
	bool ConvertToPerm(Ipp32fc * pData);
	void CorrectConstComponent(const float re0, float * pData, const int size);
	void CorrectConstComponent(const float re0, Ipp32fc * pData, const int size);

protected:

public:
	CCalcFFT_32f(const int order = 7);
	~CCalcFFT_32f();
	CCalcFFT_32f(const CCalcFFT_32f & val);
	CCalcFFT_32f & operator = (const CCalcFFT_32f & val);

	// ������ FFT.
	// �������������� ��������� ������� � ����������� ������.
	// - pSrc - �������� ������� ������ � �������� �� 1 �� m_sizeFFT;
	// - pDst - ����������� �������� ������ � �������� m_sizeFFT;
	// - sizeSrc - ������ �������� �������.
	// ���� sizeSrc = 0, �� ������ �������� ������� = m_sizeFFT.
	// ���� sizeSrc ������ m_sizeFFT, �� ����� ��������� �� ������� ������
	// ����������� ����. ���� ������ - �� ������ ������ ������������.
	// ������������ ��� ������ ErrorCodes.
	long CalcFwd_RToC(float * pSrc, Ipp32fc * pDst, const int sizeSrc = 0);

	// ������ FFT. �������������� ������������ ������� � ������� ������ ������
	// � ����������� ������.
	// - pSrc - �������� ������� ������ Re � �������� �� 1 �� m_sizeFFT;
	// - pDst - ����������� �������� ������ � �������� m_sizeFFT;
	// - sizeSrc - ������ �������� �������.
	// ���� sizeSrc = 0, �� ������ �������� ������� = m_sizeFFT.
	// ���� sizeSrc ������ m_sizeFFT, �� ����� ��������� �� ������� ������
	// ����������� ����. ���� ������ - �� ������ ������ ������������.
	// ������������ ��� ������ ErrorCodes.
	long CalcFwd_C0ToC(float * pSrc, Ipp32fc * pDst, const int sizeSrc = 0);

	// ������ FFT. �������������� ������������ ������� � ������ ������,
	// ������������ � ������� �������������� ���������, � ����������� ������.
	// - pSrc - �������� ������� ������ Re � �������� �� 1 �� m_sizeFFT;
	// - pDst - ����������� �������� ������ � �������� m_sizeFFT;
	// - sizeSrc - ������ �������� �������.
	// ���� sizeSrc = 0, �� ������ �������� ������� = m_sizeFFT.
	// ���� sizeSrc ������ m_sizeFFT, �� ����� ��������� �� ������� ������
	// ����������� ����. ���� ������ - �� ������ ������ ������������.
	// ������������ ��� ������ ErrorCodes.
	long CalcFwd_CHToC(float * pSrc, Ipp32fc * pDst, const int sizeSrc = 0);

	// ������ ��������� FFT. �������������� ������������ ������� �
	// �������� ������.
	// - pSrc - ����������� ������� ������ �������� m_sizeFFT;
	// - pDst - �������� �������� ������ �������� m_sizeFFT.
	// ������������ ��� ������ ErrorCodes.
	long CalcInv_CToR(Ipp32fc * pSrc, float * pDst);

	// ������ ��������� FFT. �������������� ������������ ������� �
	// ����������� ������.
	// - pSrc - ����������� ������� ������ �������� m_sizeFFT;
	// - pDst - ����������� �������� ������ �������� m_sizeFFT.
	// ������������ ��� ������ ErrorCodes.
	long CalcInv_CToC(Ipp32fc * pSrc, Ipp32fc * pDst);

	// ������ ��������� FFT. �������������� ������������ ������� �
	// ����������� ������.
	// - pSrcDst - ����������� ������� � �������� ������ �������� m_sizeFFT;
	// ������������ ��� ������ ErrorCodes.
	long CalcInv_CToC_I(Ipp32fc * pSrcDst);
};
//------------------------------------------------------------------------------