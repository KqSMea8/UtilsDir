//------------------------------------------------------------------------------
// ���� IirFiltrNoDCDouble.h
// ������ �� 03.05.2012
// ��� "����", ����������, �������� �.�.
//------------------------------------------------------------------------------
//		����� ��������� ������, ��������������� ��� ��������
// ���������� ������������.
//		� ������ ������������ ������� ��������� IPP 7.0.
//		���������� ��������� �������: �� ������ ������ - ���� ������!!!
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <ippFiltrDouble\CustomIirFiltrDouble.h>
//------------------------------------------------------------------------------
class CIirFiltrNoDCDouble : public CCustomIirFiltrDouble
{
private:
	double pTapsD[6];
	IppsIIRState_64f *pState[2];
	Ipp8u* pStateBuf[2];

	bool _Init();

protected:

public:
	CIirFiltrNoDCDouble();
	virtual ~CIirFiltrNoDCDouble();
	// ������ ������/������ ������� -----------------

	// ������ ������ --------------------------------
	virtual bool InitFiltr(TypeFiltr type);

	// ������� ����������
	virtual bool Filtration(double *pInpData, double *pOutData, int num);
	virtual bool Filtration_I(double *pInpOutData, int num);

	// ����������������� ������� �� ������� �����������
	virtual bool ReStart();
};
//------------------------------------------------------------------------------

