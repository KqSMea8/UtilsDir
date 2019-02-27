//----------------------------------------------------------------------
// ���� CustomIirFiltr.h
// ������ �� 04.03.2012
// ��� "����", ����������, �������� �.�.
//----------------------------------------------------------------------
//		������� ����� ��� �������, ����������� ������� IIR.
// ����� ����� ��������� ���������������� ������� (tfUser).
//----------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <Constants.h>
#include <Intel\ipp\ipps.h>
#include <FiltrDef.h>
//----------------------------------------------------------------------
const float FreqMinHz = 0.2f;
//----------------------------------------------------------------------
class CCustomIirFiltr
{
private:
	IppsIIRState_32f *pState;	// ��������� �������
	Ipp8u* pStateBuf;

protected:
	bool bInit;			// ���� ������������� �������
	TypeFiltr FType;	// ��� �������
	float Freq;			// ������� ������� (���� ���������), ��
	float *pFreqADC;	// ��������� �� ������� ������������� ��� � ��

	bool _InitFiltr(const float *pTaps, int order);
	bool _Filtration(float *pInpData, float *pOutData, int num,
						bool flag_I);

public:
	CCustomIirFiltr();
	virtual ~CCustomIirFiltr();
	virtual bool Create(float *pfreqADC);

	// ������ ������/������ ������� -----------------
	TypeFiltr GetTypeFiltr() { return FType; }
	float GetFreq() { return Freq; }

	// ������ ������ --------------------------------
	// ����������� ������� ������������� ������� ������ �����������.
	// � ����������� ���������� ��������������
	virtual bool InitFiltr(TypeFiltr type, float freq = 0.f);

	// ������� ����������
	virtual bool Filtration(float *pInpData, float *pOutData, int num);
	virtual bool Filtration_I(float *pInpOutData, int num);

	// ����������������� ������� �� ������� �����������
	virtual bool ReStart();		// �� ��� tfUser
	// ������� ������������� ������� � ������� (������ tfUser)
	bool SetUserTaps(float *ptaps, int order);
};
//----------------------------------------------------------------------

