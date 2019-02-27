//----------------------------------------------------------------------
// ���� IirFiltrSeismoCorrectionDouble.h
// ������ �� 08.09.2016
// ��� "����", ����������, ������� �.�.
//----------------------------------------------------------------------
//		����� ��������� ������ ��������� �����������
//		���������� ��������� �������: �� ������ ������ - ���� ������!!!
//----------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <ippFiltrDouble\CustomIirFiltrDouble.h>
//----------------------------------------------------------------------
class CIirFiltrSeismoCorrectionDouble : public CCustomIirFiltrDouble
{
private:
	// ������� 2 => ������
	double pTapsDouble[6];	//	�������� = 2 * (2 + 1) = 6
protected:
	double F0;
	double F1;
	double H;
public:
	CIirFiltrSeismoCorrectionDouble();
	virtual ~CIirFiltrSeismoCorrectionDouble();

	// ������ ������/������ ������� -----------------
	
	// ������ ������ --------------------------------
	bool InitFiltr(TypeFiltr type, double f1, double f2, double h);
	double PriorValueF(double freq);
	double PriorValueH(double h);

	bool ReStart();

	double GetF0() { return F0; }
	double GetF1() { return F1; }
	double GetH() { return H; }
};
//----------------------------------------------------------------------

