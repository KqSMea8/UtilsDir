//------------------------------------------------------------------------------
//	���� CustomPacketDWT.h
//	����������, ��� "����", �������� �.�.
//------------------------------------------------------------------------------
//		������� ����� ������������ ��� ������� ������������� �����������
// ������� �������������� (Fast Discrete Wavelet Transform) ����������
// �������� � ������������ � �������� ����������. � ������ ��������
// ��������� ������� �������� ��� ������� � ��������� FDWT.
//		pppFwdState[i][n] - ��� ��������� �� ��������� ������� FDWT
// (i+1)-��� ������ ����������, n-�� ������.
//		����� �������� ����������� �������� ������ CCustomFDWT.
//		� ������ ������������ ������� ��������� IPP 7.0.
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <Buffer\SmartArray.h>
#include <ippFDWT\CustomFDWT.h>
//------------------------------------------------------------------------------
template <class T>				// T - ��� IppsWTFwdState_32f ��� IppsWTInvState_32f
struct fdwtState
{
	T * pState;
	SmartArray<Ipp8u> sa;

	fdwtState(const int sizeInByte = 0)
		: pState(nullptr)
		, sa()
	{
		ReSize(sizeInByte);
	}

	bool ReSize(const int sizeInByte)
	{
		bool ret = sizeInByte > 0;
		if (ret)
		{
			ret = sa.ReSize((DWORD)sizeInByte);
			pState = ret ? ((T*)sa.pData) : nullptr;
		}
		return ret;
	}
};
//------------------------------------------------------------------------------
class CCustomPacketFDWT : public CCustomFDWT
{
private:
	int nLevelForState;		// �������� ����������� �������� �������� FDWT

	bool CreateFwdState();	// �������� �������� ������� FDWT
	bool CreateInvState();	// �������� �������� ��������� FDWT
	void DeleteFwdState();	// �������� �������� ������� FDWT
	void DeleteInvState();	// �������� �������� ��������� FDWT
	bool _InitForward();	// ������������� �������� ������� FDWT
	bool _InitInverse();	// ������������� �������� ��������� FDWT

protected:
	int nSizeMassThreshold;	// ������ ����� ������� �������
	fdwtState<IppsWTFwdState_32f> ***pppFwdState;	// ��. �� ���������� �������
	fdwtState<IppsWTInvState_32f> ***pppInvState;	//		�������� FDWT
// 	IppsWTFwdState_32f ***pppFwdState;	// ��. �� ���������� �������
// 	IppsWTInvState_32f ***pppInvState;	//		�������� FDWT
	int nLevelForBuf;		// ����������� ������� ������������� FDWT
	int FQuantityBand;		// ���-�� ����� ������� ������ ����������
	int m_pNumBand[32];		// ������ ���-� ����� � ������
	bool _Init();			// ������������� ���� ��������
	bool CreateState();		// �������� ���� ��������
	void DeleteState();		// �������� ���� ��������
	
	// ����������� �������, ����������� �������� ��������, �.�.
	virtual void NewTypeWavelet() { _Init(); }	// ���-�� ��������
	virtual void NewLevel();					//		FDWT

public:
	CCustomPacketFDWT(void);
	virtual ~CCustomPacketFDWT(void);
	virtual long Create(TypeWavelet typeWavelet, int level, bool bneedInverse,
		int lengthSignal);

	// ������ ���������� �� ������������ FDWT ���������� ������
	float **m_ppBandData;

	// ������ ������/������ ������� -----------------
	int GetQuantityBand() { return FQuantityBand; }

	// ������ ������ ----------------------------
	float* GetPointerToTopLevelFactors(int band);
	int GetMaxFactorInBand(int band, float *pValMax = NULL);

	float BandWidth(float *pFreqADC)	// ������ �����
		{ return *pFreqADC / m_pNumBand[FLevel] * 0.5f; }
	float BandWidthHalf(float *pFreqADC)	// �������� ������
		{ return BandWidth(pFreqADC) * 0.5f; }
	float BandMiddleFreq(float *pFreqADC, int band);
	void  BandMiddleFreqAll(float *pFreqADC, float *pFreqBand);
};
//------------------------------------------------------------------------------

