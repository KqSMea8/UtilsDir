//------------------------------------------------------------------------------
//	���� CustomPacketDWT.cpp
//	����������, ��� "����", �������� �.�.
//------------------------------------------------------------------------------
#include "StdAfx.h"
#include "CustomPacketFDWT.h"
//------------------------------------------------------------------------------
CCustomPacketFDWT::CCustomPacketFDWT(void) : CCustomFDWT()
{// ������������� ������� ���������� �� ������ ������� ������ - m_ppBandData
//	������� ������� m_pNumBand - ���-�� ��������� �����, ���������������
//	������������ ������. 0 - �������� ������, ���� ������;
//	������� 1 - ��� ������ (���������������� � �������������� �������)
	pppFwdState = NULL;
	pppInvState = NULL;
	m_ppBandData = NULL;
	int n = 1;
	for(int i=0; i<31; ++i)
	{
		m_pNumBand[i] = n;
		n *= 2;
	}
	nLevelForBuf = 0;
	m_pNumBand[31] = 0;
	nLevelForState = 0;
	nSizeMassThreshold = 0;
}
//------------------------------------------------------------------------------
CCustomPacketFDWT::~CCustomPacketFDWT(void)
{// ������ ��������� ������� ��������������
	DeleteFwdState();
	DeleteInvState();
}
//------------------------------------------------------------------------------
long CCustomPacketFDWT::Create(TypeWavelet typeWavelet, int level,
	bool needInverse, int lengthSignal)
{// �������� ����������� ������� �������� ������� �������������� FDWT
	long ret = CCustomFDWT::Create(typeWavelet, level,
		needInverse, lengthSignal);
	FQuantityBand = m_pNumBand[FLevel];
	if ( !CreateState() )
		ret |= wtrvError;
	return ret;
}
//------------------------------------------------------------------------------
bool CCustomPacketFDWT::CreateState()
{
	nLevelForState = FLevel;
	bool ret = CreateFwdState();
	if (FbNeedInverse)
		ret = ret && CreateInvState();
	return ret;
}
//------------------------------------------------------------------------------
bool CCustomPacketFDWT::CreateFwdState()
{// �������� ���������� ������� �������� ������� FDWT
								// � �������������� ��������
	if (!pppFwdState)
	{// ���� ������� �������� ���, ������ ���
		int sizeState(0);
		pppFwdState = new fdwtState<IppsWTFwdState_32f> **[nLevelForState];
		bInitForward = pppFwdState != nullptr;
		if (bInitForward)
		{
			for (int i = 0; i < nLevelForState; ++i)
			{
				pppFwdState[i] = new fdwtState<IppsWTFwdState_32f> *[m_pNumBand[i]];
				bInitForward &= pppFwdState[i] != nullptr;
				for (int n = 0; n < m_pNumBand[i]; ++n)
				{
					pppFwdState[i][n] = new fdwtState<IppsWTFwdState_32f>;
					bInitForward &=
						(pppFwdState[i][n] != nullptr) &&
						(ippStsNoErr == ippsWTFwdGetSize(ipp32f, FLengthFilter, fwdFltOffs, FLengthFilter, fwdFltOffs, &sizeState)) &&
						pppFwdState[i][n]->ReSize(sizeState) &&
						(ippStsNoErr == ippsWTFwdInit_32f(pppFwdState[i][n]->pState, pTaps_fwdLo, FLengthFilter, fwdFltOffs, pTaps_fwdHi, FLengthFilter, fwdFltOffs));
				}
			}
		}
		if (!bInitForward)
			DeleteFwdState();
	}
	return bInitForward;
}
//------------------------------------------------------------------------------
bool CCustomPacketFDWT::CreateInvState()
{// �������� ���������� ������� �������� ��������� FDWT
							// � �������������� ��������
	if (!pppInvState)
	{// ���� ������� �������� ���, ������ ���
		int sizeState(0);
		pppInvState = new fdwtState<IppsWTInvState_32f> ** [nLevelForState];
		bInitInverse = pppInvState != nullptr;
		for (int i = 0; i<nLevelForState; ++i)
		{
			pppInvState[i] = new fdwtState<IppsWTInvState_32f> * [m_pNumBand[i]];
			bInitInverse &= pppInvState[i] != nullptr;
			for (int n = 0; n < m_pNumBand[i]; ++n)
			{
				pppInvState[i][n] = new fdwtState<IppsWTInvState_32f>;
				bInitInverse &=
					(pppInvState[i][n] != nullptr) &&
					(ippStsNoErr == ippsWTInvGetSize(ipp32f, FLengthFilter, fwdFltOffs, FLengthFilter, fwdFltOffs, &sizeState)) &&
					pppInvState[i][n]->ReSize(sizeState) &&
					(ippStsNoErr == ippsWTInvInit_32f(pppInvState[i][n]->pState, pTaps_fwdLo, FLengthFilter, fwdFltOffs, pTaps_fwdHi, FLengthFilter, fwdFltOffs));
			}
		}
		if (!bInitInverse)
			DeleteInvState();
	}
	return bInitInverse;
}
//------------------------------------------------------------------------------
void CCustomPacketFDWT::DeleteState()
{// �������� ��������� ������� � ��������� FDWT
	DeleteFwdState();
	if (FbNeedInverse)
		DeleteInvState();
}
//------------------------------------------------------------------------------
void CCustomPacketFDWT::DeleteFwdState()
{// �������� �������� ������� FDWT
	if (pppFwdState)
	{
		for(int i=0; i<nLevelForState; ++i)
		{
			for (int n = 0; n < m_pNumBand[i]; ++n)
				delete pppFwdState[i][n];
			delete [] pppFwdState[i];
			pppFwdState[i] = nullptr;
		}
		delete [] pppFwdState;
		pppFwdState = nullptr;

		bInitForward = false;
	}
}
//------------------------------------------------------------------------------
void CCustomPacketFDWT::DeleteInvState()
{// �������� �������� ��������� FDWT
	if (pppInvState)
	{
		for(int i=0; i<nLevelForState; ++i)
		{
			for(int n=0; n<m_pNumBand[i]; ++n)
				delete pppInvState[i][n];
			delete [] pppInvState[i];
		}
		delete[] pppInvState;
		pppInvState = nullptr;

		bInitInverse = false;
	}
}
//------------------------------------------------------------------------------
void CCustomPacketFDWT::NewLevel()
{// ��������������� ����������� �������, ���������� ��� ��������� ������
// �������� �������: �������� ������� � �������� ������ ����������� �������
// �������� ������� FDWT (� �� ��������������)
	FQuantityBand = m_pNumBand[FLevel];
	DeleteState();
	CreateState();
}
//------------------------------------------------------------------------------
bool CCustomPacketFDWT::_Init()
{
	bool ret = bInitForward = _InitForward();
	if (FbNeedInverse)
	{
		bInitInverse = _InitInverse();
		ret = ret && bInitInverse;
	}
	return ret;
}
//------------------------------------------------------------------------------
bool CCustomPacketFDWT::_InitForward()
{// ����������������� �������� ��� ������� ��������������
	DeleteFwdState();
	CreateFwdState();
	return bInitForward;
}
//------------------------------------------------------------------------------
bool CCustomPacketFDWT::_InitInverse()
{// ����������������� �������� ��� ��������� ��������������
	DeleteInvState();
	CreateInvState();
	return bInitInverse;
}
//------------------------------------------------------------------------------
// ������� ���������� ��������� �� ������������ ����������
// ����������� ������ ������� ������ ����������
float* CCustomPacketFDWT::GetPointerToTopLevelFactors(int band)
{
	float *ret(nullptr);
	if ( m_ppBandData && (0 < band) && (band < FQuantityBand) )
		ret = m_ppBandData[band];
	else
		ret = nullptr;
	return ret;
}
//------------------------------------------------------------------------------
int CCustomPacketFDWT::GetMaxFactorInBand(int band, float *pValMax)
{
	/**/
	float _max = 0, *point;
	int ret = -1;
	if ( m_ppBandData && (0 < band) && (band < FQuantityBand) )
	{
		point = m_ppBandData[band];
		for(int i=0; i<m_pSize[FLevel]; ++i)
		{
			float x = fabs(point[i]);
			if ( _max < x )
			{
				_max = x;
				ret = i;
			}
		}
		if (pValMax)
		*pValMax = _max;
	}
	return ret;
	
/*
	int ret = -1;
	if ( m_ppBandData && (0 < band) && (band < FQuantityBand) )
	{
		float valMin, valMax;
		int indMin, indMax;
		if (ippsMinMaxIndx_32f(m_ppBandData[band], m_pSize[FLevel],
				&valMin, &indMin, &valMax, &indMax) == ippStsNoErr)
		{
			valMin = fabs(valMin);
			valMax = fabs(valMax);
			if (valMax > valMin)
			{
				if (pValMax)
					*pValMax = valMax;
				ret = indMax;
			}
			else
			{
				if (pValMax)
					*pValMax = valMin;
				ret = indMin;
			}
		}
	}
	return ret;*/
}
//------------------------------------------------------------------------------
// ������ ������� ������� ������ � ������� band
float CCustomPacketFDWT::BandMiddleFreq(
	float *pFreqADC,	// ��������� �� ������� ������������� ���
	int band)			// ����� ������ (�� 0 �� FQuantityBand-1)
{// ������� ������� ������ � ������� band
	float freq;
	if ( (band >= 0) && (band < FQuantityBand) )
		freq = BandWidth(pFreqADC) * ( (float)band + 0.5f );
	else
		freq = -1.f;
	return freq;
}
//------------------------------------------------------------------------------
// ������ ������� ������ ���� �����
void CCustomPacketFDWT::BandMiddleFreqAll(
	float *pFreqADC,	// ��������� �� ������� ������������� ���
	float *pFreqBand)	// ��������� �� ������ ������, ������ FLevel
{
	float df = BandWidth(pFreqADC);
	float f = df * 0.5f;
	for(int i=0; i<FQuantityBand; ++i)
	{
		pFreqBand[i] = f;
		f += df;
	}
}
//------------------------------------------------------------------------------