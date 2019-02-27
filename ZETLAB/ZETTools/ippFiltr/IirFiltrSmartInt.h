//----------------------------------------------------------------------
// ���� IirFiltrSmartInt.h
// ������ �� 26.02.2012
// ��� "����", ����������, �������� �.�.
//----------------------------------------------------------------------
//		����� ������������ ��� ��������� �������������� ����������
// �������� � ��������� ���������� ������������ ��� �����, ��� � �����
// ����������. �������������� ������� ��� ������� ������.
//		� ������ ������������ ������� ��������� IPP 7.0.
//		���������� ��������� �������: �� ������ ������ - ���� ������!!!
//----------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <ippFiltr\IirFiltrSimple.h>
#include <ippFiltr\IirFiltrNoDC.h>
//----------------------------------------------------------------------
class CIirFiltrSmartInt
{
private:
	bool bInit;
	TypeFiltr FType;			// ��� �������
	CIirFiltrSimple **ppFiltrInt;
	CIirFiltrNoDC **ppFiltrNo;
	int FNumFiltrNo;
	int FNumFiltrInt;
	float *pFreqADC;	// ��������� �� ������� ������������� ��� � ��

	bool CreateFiltr();
	void DeleteFiltr();

protected:

public:
	CIirFiltrSmartInt();
	virtual ~CIirFiltrSmartInt();
	virtual bool Create(float *pfreqADC);

	// ������ ������/������ ������� -----------------
	TypeFiltr GetTypeFiltr() { return FType; }

	// ������ ������ --------------------------------
	bool InitFiltr(TypeFiltr type);

	// ������� ����������
	bool Filtration(float *pInpData, float *pOutData, int num);
	bool Filtration_I(float *pInpOutData, int num);

	// ����������������� ������� �� ������� �����������
	bool ReStart();
};
//----------------------------------------------------------------------

