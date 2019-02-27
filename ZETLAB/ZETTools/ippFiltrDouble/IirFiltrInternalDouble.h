//----------------------------------------------------------------------
// ���� IirFiltrInternal.h
// ������ �� 03.05.2012
// ��� "����", ����������, �������� �.�.
//----------------------------------------------------------------------
//		����� ��������� �������, ������ ������������� �������
// ���������� � ���������� IPP 7.0. ��� ��� � ��� ��� ��������
// ����������� � �������� (������� �������� ������ � ������).
//		���������� ��������� �������: �� ������ ������ - ���� ������!!!
//----------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <ippFiltrDouble\CustomIirFiltrDouble.h>
//----------------------------------------------------------------------
class CIirFiltrInternalDouble : public CCustomIirFiltrDouble
{
private:
	int order;
	bool autoOrder;
	// ������������ ������� 10 => ������
	double pTapsDouble[26];	//	�������� = 2 * (10 + 1) = 22
protected:

public:
	CIirFiltrInternalDouble();
	virtual ~CIirFiltrInternalDouble();

	// ������ ������/������ ������� -----------------
	void SetAutoOrder(bool bAutoOrder);
	int GetOrder();

	// ������ ������ --------------------------------
	virtual bool InitFiltr(TypeFiltr type, double freq = 0.);
	bool InitFiltr(TypeFiltr type, int ord = 0, double freq = 0.);
	double PriorValue(double freq);

	const double* getTapsDouble() { return pTapsDouble; }
};
//----------------------------------------------------------------------

