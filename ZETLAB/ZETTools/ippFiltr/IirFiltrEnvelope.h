//----------------------------------------------------------------------
// ���� IirFiltrEnvelope.h
// ������ �� 05.03.2012
// ��� "����", ����������, ������� �.�.
//----------------------------------------------------------------------
//		����� ��������� ������ ��� ������� ���������, �� ����������
// ��� ��� ���������������� ���.
//		������������� ������� ���������� IPP 7.0.
//		���������� ��������� �������: �� ������ ������ - ���� ������!!!
//----------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <ippFiltr\IirFiltrInternal.h>
//----------------------------------------------------------------------
class CIirFiltrEnvelope
{
private:
	bool bInit;
	bool bNeed;
	TypeFiltr FType;			// ��� �������
	float time;
	CIirFiltrInternal *pFiltrInternal1, *pFiltrInternal2;
	float *pFreqADC;	// ��������� �� ������� ������������� ��� � ��

	bool DefineNeedFiltr(float time);
	bool _Filtration(float *pInpOutData, int num);
protected:

public:
	CIirFiltrEnvelope();
	virtual ~CIirFiltrEnvelope();
	virtual bool Create(float *pfreqADC);

	// ������ ������/������ ������� -----------------
	TypeFiltr GetTypeFiltr() { return FType; }
	float GetTime() {return time;}

	// ������ ������ --------------------------------
	bool InitFiltr(TypeFiltr type, float fTime);

	// ������� ����������
	bool Filtration(float *pInpData, float *pOutData, int num);
	bool Filtration_I(float *pInpOutData, int num);
	float PriorValue(float time);
	// ����������������� ������� �� ������� �����������
	bool ReStart();
};
//----------------------------------------------------------------------

