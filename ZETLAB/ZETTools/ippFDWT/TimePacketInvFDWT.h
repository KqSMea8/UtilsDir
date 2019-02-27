//--------------------------------------------------------------------------
//	������ CPacketDWT.h
//	������ �� 16.12.2011
//--------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include "ippFDWT\CustomFDWT.h"
//--------------------------------------------------------------------------
class CPacketFDWT : public CCustomFDWT
{
private:
	void DeleteAll(int level);
	bool CreateAll();
	IppsWTFwdState_32f ***pppFwdState;
	CShiftBuffer <float> ***pppBuf;
	//IppsWTInvState_32f ***pppInvState;
	bool _InitForward();
	void SetNumBuf();		// ������� ������� m_pNumBuf[]
	int m_pSizePage[32];	// ������ �������� ������� ������� ������
	int m_pNumBuf[32];		// ������ ���-� ����� � ������

protected:

public:
	CPacketFDWT(void);
	virtual ~CPacketFDWT(void);
	virtual bool Create(TypeWavelet typeWavelet, int level);

	// ������ ������/������ ������� -----------------
	bool SetSizePageSignal(int size = 0);

	// ������ ������ ----------------------------
	bool InitForward(int sizePageSignal);
	bool InitForwardOne(int size);
	bool Forward(float *pData);
	bool ForwardOne(float *pData);
	bool ReStartInitForward();

/*	bool InitInverse();
	bool InitInverseOne();
	bool Inverse(float *pSignal);
	bool InverseOne(float *pSignal);
	bool ReStartInitInverse();*/

	CShiftBuffer <float> *m_pSignal;
	float **m_ppBandData;
	//float* BandPointerData(int band);

	float BandWidth(float *pFreqADC)
		{ return *pFreqADC / m_pNumBuf[FLevel] * 0.5f; }
	float BandWidthHalf(float *pFreqADC)
		{ return BandWidth(pFreqADC) * 0.5f; }
	float BandMiddleFreq(float *pFreqADC, int band);
	void  BandMiddleFreqAll(float *pFreqADC, float *pFreqBand);
};
//--------------------------------------------------------------------------

