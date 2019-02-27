//----------------------------------------------------------------------
// ���� IirFiltrSmartIntDouble.h
// ������ �� 03.05.2012
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
#include <ippFiltrDouble\IirFiltrSimpleDouble.h>
#include <ippFiltrDouble\IirFiltrNoDCDouble.h>
//----------------------------------------------------------------------
class CIirFiltrSmartIntDouble
{
private:
	bool bInit;
	TypeFiltr FType;			// ��� �������
	CIirFiltrSimpleDouble **ppFiltrInt;
	CIirFiltrNoDCDouble **ppFiltrNo;
	int FNumFiltrNo;
	int FNumFiltrInt;
	double *pFreqADC;	// ��������� �� ������� ������������� ��� � ��

	bool CreateFiltr();
	void DeleteFiltr();

protected:

public:
	CIirFiltrSmartIntDouble();
	virtual ~CIirFiltrSmartIntDouble();
	virtual bool Create(double *pfreqADC);

	// ������ ������/������ ������� -----------------
	TypeFiltr GetTypeFiltr() { return FType; }
	bool InitFiltr(TypeFiltr type);
	void SetFreqMinInHz(double freq);

	// ������� ����������
	bool Filtration(double *pInpData, double *pOutData, int num);
	bool Filtration_I(double *pInpOutData, int num);

	// ����������������� ������� �� ������� �����������
	bool ReStart();
};
//----------------------------------------------------------------------

