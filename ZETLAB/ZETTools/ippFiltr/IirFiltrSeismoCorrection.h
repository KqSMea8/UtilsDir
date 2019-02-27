//----------------------------------------------------------------------
// ���� IirFiltrSeismoCorrection.h
// ������ �� 18.01.2018
// ��� "������", ����������, ������� �.�.
//----------------------------------------------------------------------
//		����� ��������� ������ ��������� �����������
//		���������� ��������� �������: �� ������ ������ - ���� ������!!!
//----------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <ippFiltr\CustomIirFiltr.h>
//----------------------------------------------------------------------
class CIirFiltrSeismoCorrection : public CCustomIirFiltr
{
private:
	// ������� 2 => ������
	float pTaps[6];	//	�������� = 2 * (2 + 1) = 6

protected:
	double F0;
	double F1;
	double H;

public:
	CIirFiltrSeismoCorrection();
	virtual ~CIirFiltrSeismoCorrection();

	// ������ ������/������ ������� -----------------
	
	// ������ ������ --------------------------------
	bool InitFiltr(TypeFiltr type, double f1, double f2, double h);
	double PriorValueF(double freq);
	double PriorValueH(double h);

	bool ReStart();

	const double GetF0() const { return F0; }
	const double GetF1() const { return F1; }
	const double GetH() const { return H; }
};
//----------------------------------------------------------------------

