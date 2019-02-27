//----------------------------------------------------------------------
// ���� CustomIirFiltrDouble.h
// ������ �� 03.05.2012
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
#include <ippFiltrDouble\polynom.h>
//----------------------------------------------------------------------
class CCustomIirFiltrDouble
{
private:
	void sub(double* x1, double* z, double* e, double* y, int t, int n);

	IppsIIRState_64f* pState;	// ��������� �������
	Ipp8u* pStateBuf;

protected:
	bool bInit;			// ���� ������������� �������
	TypeFiltr FType;	// ��� �������
	double Freq;		// ������� ������� (���� ���������), ��
	double *pFreqADC;	// ��������� �� ������� ������������� ��� � ��
	double FreqMinHz = 0.2;

	bool _InitFiltr(const double *pTaps, int order);
	bool _Filtration(double *pInpData, double *pOutData, int num,
						bool flag_I);

	typedef struct TransferFunction
	{
		Polynom numerator;
		Polynom denomenator;
	} TransferFunction;
	void _BilinearTransformation(TransferFunction tfFunction, double dFrequency,
		TransferFunction &tfReturn);

public:
	CCustomIirFiltrDouble();
	virtual ~CCustomIirFiltrDouble();
	virtual bool Create(double *pfreqADC);

	// ������ ������/������ ������� -----------------
	bool IsInit() { return bInit;  }
	TypeFiltr GetTypeFiltr() { return FType; }
	double GetFreq() { return Freq; }
	double* GetPointerToFreqADC() { return pFreqADC; }
	void SetFreqMinInHz(double freq);

	// ������ ������ --------------------------------
	// ����������� ������� ������������� ������� ������ �����������.
	// � ����������� ���������� ��������������
	virtual bool InitFiltr(TypeFiltr type, double freq = 0.);

	// ������� ����������
	virtual bool Filtration(double *pInpData, double *pOutData, int num);
	virtual bool Filtration_I(double *pInpOutData, int num);

	// ����������������� ������� �� ������� �����������
	virtual bool ReStart();		// �� ��� tfUser
	// ������� ������������� ������� � ������� (������ tfUser)
	bool SetUserTaps(double *ptaps, int order);
};
//----------------------------------------------------------------------

