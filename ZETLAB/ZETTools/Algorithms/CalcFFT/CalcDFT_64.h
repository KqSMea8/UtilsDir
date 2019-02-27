//------------------------------------------------------------------------------
/* ���� CalcDFT_64.h

����� CCalcDFT_64f ������������ ��� ���������� �������� ������� ��� ���
������ ���� double. ����� �������� ����������� �������� ������ CustomCalcFFT".

������ ������:
- ������ ��� (����������, ����������� ����������� � �������� ������������);
- ��� ������� ��� ���������� ������� ���.

������������ �� �������� ������ �������:
- ������ ������� � ��������� Get � Set;
- ��� ������� ��������� ��������.

*/
//------------------------------------------------------------------------------
#pragma once
#include "CalcFFT_64.h"
//------------------------------------------------------------------------------
class CCalcDFT_64f : public CCustomCalcFFT
{
private:
	int m_sizeDFT;
	int m_sizeDFT_2;
	double m_kNorma;

	CCalcFFT_64f * m_pCalcFFT;

	SmartArray<double> m_saSrc_d;	// ������ m_sizeDFT
	SmartArray<Ipp64fc> m_saSrc_dc;	// ������ m_sizeFFT

	SmartArray<Ipp64fc> m_saExp1;	// ������ m_sizeDFT
	SmartArray<Ipp64fc> m_saExp2;	// ������ m_sizeFFT

	// ��������� ������� ------------------------------------
	long CreateMassiveExp();

protected:

public:
	CCalcDFT_64f(const int size = 0);
	~CCalcDFT_64f();
	CCalcDFT_64f(const CCalcDFT_64f & val);
	CCalcDFT_64f & operator = (const CCalcDFT_64f & val);

	// ������� ���������� DFT.
	// - size - ������ ��������� ������ ������.
	// � ������� �������� ��������� ������, ����������� ���
	// ���������� �������������� ��������� CalcFwd
	long SetParam(const int size);
	
	// ���������������� ������� �������� ������.
	// ��������� ������� ������ = m_sizeDFT, ����������� � SetParam
	// ������������ ��������:
	// 0 - �� ���� ������ SetParam;
	// > 0 - ������.
	virtual int GetSize();

	// ���������������� ������� �������� ������.
	// ��������� �������� ������� ������ = m_sizeDFT / 2, ����������� � SetParam
	// ������������ ��������:
	// 0 - �� ���� ������ SetParam;
	// > 0 - ������.
	virtual int GetHalfSize();

	// ������ DFT.
	// �������������� ��������� ������� � ����������� ������.
	// - pSrc - �������� ������� ������ � �������� �� 1 �� m_sizeFFT;
	// - pDst - ����������� �������� ������ � �������� m_sizeFFT / 2;
	// - sizeSrc - ������ �������� �������.
	// ���� sizeSrc = 0, �� ������ �������� ������� = m_sizeFFT.
	// ���� sizeSrc ������ m_sizeDFT, �� ����� ��������� �� ������� ������
	// ����������� ����. ���� ������ - �� ������ ������ ������������.
	// ������������ ��� ������ ErrorCodes.
	long CalcFwd_RToC(double * pSrc, Ipp64fc * pDst, const int sizeSrc = 0);
};
//------------------------------------------------------------------------------