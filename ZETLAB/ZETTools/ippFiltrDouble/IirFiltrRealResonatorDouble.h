//----------------------------------------------------------------------
// ���� IirFiltrRealResonatorDouble.h
// ������ �� 15.01.2016
// ��� "����", ����������, ������� �.�.
//----------------------------------------------------------------------
//		����� ��������� ������ ���� �������������� ���������
//		���������� ��������� �������: �� ������ ������ - ���� ������!!!
//----------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <ippFiltrDouble\CustomIirFiltrDouble.h>
//----------------------------------------------------------------------
class CIirFiltrRealResonatorDouble : public CCustomIirFiltrDouble
{
private:
	// ������� 2 => ������
	double pTapsDouble[6];	//	�������� = 2 * (2 + 1) = 6
	
protected:
	double Decrement;

public:
	CIirFiltrRealResonatorDouble();
	virtual ~CIirFiltrRealResonatorDouble();

	// ������ ������/������ ������� -----------------
	
	// ������ ������ --------------------------------
	bool InitFiltr(TypeFiltr type, double freq, double decrement);
	double PriorValue(double freq);
	double GetFres() const;
	double GetQ() const;

	bool ReStart();
};
//----------------------------------------------------------------------

