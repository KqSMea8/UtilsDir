#pragma once
#include "afx.h"
#include <ippFiltr\CustomIirFiltr.h>

// �������� IIR ��������, ����������� ������-���� ��������
enum FreqCorrection
{
	fcWb,
	fcWc,
	fcWd,
	fcWe,
	fcWf,
	fcWh,
	fcWj,
	fcWk,
	fcWm,
	fcA,
	fcB,
	fcC,
	fcD,
};

class CIirFreqCorrection :public CCustomIirFiltr
{
private:
	double pTapsD[6];
	IppsIIRState_64f *pStateHPF;
	Ipp8u* pStateHPFBuf;
	IppsIIRState_64f *pStateLPF;
	Ipp8u* pStateLPFBuf;
	IppsIIRState_64f *pStateTF;
	Ipp8u* pStateTFBuf;
	IppsIIRState_64f *pStateSF1;
	Ipp8u* pStateSF1Buf;
	IppsIIRState_64f *pStateSF2;
	Ipp8u* pStateSF2Buf;
	double *pData;
	int numCur;
	bool _Init();
	void NewNum(int num);
	bool InitVibroCorrection();
	bool InitAcousticCorrection();

protected:
	FreqCorrection FCorr;

public:
	CIirFreqCorrection();
	virtual ~CIirFreqCorrection();
	// ������ ������/������ ������� -----------------

	// ������ ������ --------------------------------
	bool InitFiltr(TypeFiltr type, FreqCorrection corr);

	// ������� ����������
	virtual bool Filtration(float *pInpData, float *pOutData, int num);
	virtual bool Filtration_I(float *pInpOutData, int num);

	// ����������������� ������� �� ������� �����������
	virtual bool ReStart();
};

