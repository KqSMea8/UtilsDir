//------------------------------------------------------------------------------
// ���� IirFiltrNoDC.h
// ������ �� 04.03.2012
// ��� "����", ����������, �������� �.�.
//------------------------------------------------------------------------------
//		����� ��������� ������, ��������������� ��� ��������
// ���������� ������������.
//		� ������ ������������ ������� ��������� IPP 7.0.
//		���������� ��������� �������: �� ������ ������ - ���� ������!!!
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <ippFiltr\CustomIirFiltr.h>
//------------------------------------------------------------------------------
class CIirFiltrNoDC : public CCustomIirFiltr
{
private:
	double pTapsD[6];
	IppsIIRState_64f* pState[2];
	Ipp8u* pStateBuf[2];
	double *pData;
	int numCur;
	bool _Init();
	void NewNum(int num);

protected:

public:
	CIirFiltrNoDC();
	virtual ~CIirFiltrNoDC();
	// ������ ������/������ ������� -----------------

	// ������ ������ --------------------------------
	virtual bool InitFiltr(TypeFiltr type);

	// ������� ����������
	virtual bool Filtration(float *pInpData, float *pOutData, int num);
	virtual bool Filtration_I(float *pInpOutData, int num);

	// ����������������� ������� �� ������� �����������
	virtual bool ReStart();
};
//------------------------------------------------------------------------------

