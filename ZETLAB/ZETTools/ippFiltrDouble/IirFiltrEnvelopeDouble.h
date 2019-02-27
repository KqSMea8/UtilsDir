//----------------------------------------------------------------------
// ���� IirFiltrEnvelopeDouble.h
// ������ �� 03.05.2012
// ��� "����", ����������, ������� �.�.
//----------------------------------------------------------------------
//		����� ��������� ������ ��� ������� ���������, �� ����������
// ��� ��� ���������������� ���.
//		������������� ������� ���������� IPP 7.0.
//		���������� ��������� �������: �� ������ ������ - ���� ������!!!
//----------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <ippFiltrDouble\IirFiltrInternalDouble.h>
//----------------------------------------------------------------------
class CIirFiltrEnvelopeDouble
{
private:
	bool bInit;
	bool bNeed;
	TypeFiltr FType;			// ��� �������
	double time;
	CIirFiltrInternalDouble *pFiltrInternal1, *pFiltrInternal2;
	double *pFreqADC;	// ��������� �� ������� ������������� ��� � ��

	bool DefineNeedFiltr(double time);
	bool _Filtration(double *pInpOutData, int num);
protected:

public:
	CIirFiltrEnvelopeDouble();
	virtual ~CIirFiltrEnvelopeDouble();
	virtual bool Create(double *pfreqADC);

	// ������ ������/������ ������� -----------------
	TypeFiltr GetTypeFiltr() { return FType; }
	double GetTime() {return time;}

	// ������ ������ --------------------------------
	bool InitFiltr(TypeFiltr type, double fTime);

	// ������� ����������
	bool Filtration(double *pInpData, double *pOutData, int num);
	bool Filtration_I(double *pInpOutData, int num);
	double PriorValue(double time);
	// ����������������� ������� �� ������� �����������
	bool ReStart();
};
//----------------------------------------------------------------------

