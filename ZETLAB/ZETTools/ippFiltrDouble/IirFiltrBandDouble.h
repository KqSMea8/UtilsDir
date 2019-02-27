//----------------------------------------------------------------------
// ���� IirFiltrBandDouble.h
// ������ �� 03.05.2012
// ��� "����", ����������, �������� �.�.
//----------------------------------------------------------------------
//		����� ������������ ��� �������� ���������� ���������� ��������
// � ������� ������� �����������. ����������� ��������� � �����������
// �������. ����� � ��� � ���, �� ��� ���� �������� ����� ������������
// ����� CIirFiltrInternalDouble.
//		� ������ ������������ ������� ��������� IIP 7.0
//		���������� ��������� �������: �� ������ ������ - ���� ������!!!
//----------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <ippFiltrDouble\IirFiltrInternalDouble.h>
//----------------------------------------------------------------------
class CIirFiltrBandDouble
{
private:
	TypeFiltr FType;
	double Ffmin, Ffmax;
	double *pTemp;
	int nTemp;
	CIirFiltrInternalDouble *pFiltrHiPass, *pFiltrLoPass;
	bool bNeedHi, bNeedLo;
	bool bInit;
	int m_countLpf;
	int m_countHpf;

	bool DefineNeedFiltr(double freq);
	inline const double freqCorrection(_In_ const int order, _In_ const int count) const
	{
		return pow(pow(2., 1. / (double)count) - 1., 1. / 2. / (double)order);
	}

protected:

public:
	CIirFiltrBandDouble();
	virtual ~CIirFiltrBandDouble();
	virtual bool Create(double *pfreqADC);
	bool Create(double* pfreqADC, int countLpf, int countHpf);

	// ������ ������/������ ������� -----------------
	TypeFiltr GetTipFiltr() { return FType; }
	double GetFmin();
	double GetFmax();
	int GetOrderLPF();
	int GetOrderHPF();
	const double* getTapsDoubleLPF();
	const double* getTapsDoubleHPF();
	bool IsInit() { return bInit; }

	// ������ ������ --------------------------------
	bool Filtration(double *pInpData, double *pOutData, int num);
	bool Filtration_I(double *pInpOutData, int num);
	bool InitFiltr(TypeFiltr type, double fmin, double fmax);
	double PriorValueFmin(double freq);
	double PriorValueFmax(double freq);
	bool ReStart();
	void SetFreqMinInHz(double freq);
};
//----------------------------------------------------------------------

