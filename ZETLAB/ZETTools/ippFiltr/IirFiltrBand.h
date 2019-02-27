//----------------------------------------------------------------------
// ���� IirFiltrBand.h
// ������ �� 26.02.2012
// ��� "����", ����������, �������� �.�.
//----------------------------------------------------------------------
//		����� ������������ ��� �������� ���������� ���������� ��������
// � ������� ������� �����������. ����������� ��������� � �����������
// �������. ����� � ��� � ���, �� ��� ���� �������� ����� ������������
// ����� CIirFiltrInternal.
//		� ������ ������������ ������� ��������� IIP 7.0
//		���������� ��������� �������: �� ������ ������ - ���� ������!!!
//----------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <ippFiltr\IirFiltrInternal.h>
//----------------------------------------------------------------------
class CIirFiltrBand
{
private:
	TypeFiltr FType;
	float Ffmin, Ffmax;
	float *pTemp;
	int nTemp;
	CIirFiltrInternal *pFiltrHiPass, *pFiltrLoPass;
	bool bNeedHi, bNeedLo;
	bool bInit;

	bool DefineNeedFiltr(float freq);

protected:

public:
	CIirFiltrBand();
	virtual ~CIirFiltrBand();
	virtual bool Create(float *pfreqADC);

	// ������ ������/������ ������� -----------------
	TypeFiltr GetTipFiltr() { return FType; }
	float GetFmin();
	float GetFmax();

	// ������ ������ --------------------------------
	bool Filtration(float *pInpData, float *pOutData, int num);
	bool Filtration_I(float *pInpOutData, int num);
	bool InitFiltr(TypeFiltr type, float fmin, float fmax);
	float PriorValueFmin(float freq);
	float PriorValueFmax(float freq);
	bool ReStart();
};
//----------------------------------------------------------------------

