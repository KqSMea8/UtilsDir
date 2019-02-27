//------------------------------------------------------------------------------
// ���� IirFiltrNoDC.cpp
// ������ �� 05.03.2012
// ��� "����", ����������, �������� �.�.
//------------------------------------------------------------------------------
#include "StdAfx.h"
#include <ippFiltr\IirFiltrNoDC.h>
//------------------------------------------------------------------------------
CIirFiltrNoDC::CIirFiltrNoDC() : CCustomIirFiltr()
{
	pState[0] = pState[1] = nullptr;
	pStateBuf[0] = pStateBuf[1] = nullptr;
	pData = NULL;
	numCur = 0;
	ZeroMemory(pTapsD, sizeof(double) * 6);
}
//------------------------------------------------------------------------------
CIirFiltrNoDC::~CIirFiltrNoDC()
{
	if (pData)
	{
		delete[] pData;
		pData = nullptr;
	}
	for(int i = 0; i < 2; ++i)
	{ 
		if (pStateBuf[i])
		{
			ippsFree(pStateBuf[i]);
			pStateBuf[i] = nullptr;
		}
	}
}
//------------------------------------------------------------------------------
bool CIirFiltrNoDC::_Init()
{
	// ������ ������������� �������� (������������ ��������� �.�.)
	// ������ ���� � ����� http://model.exponenta.ru/audio_eq.html
	
	// ���������� ������������ ������� ������� ������� ������� ����� ���:

	//         (B0/A0) + (B1/A0)*z^-1 + (B2/A0)*z^-2     b0 + b1*z^-1 + b2*z^-2
	// H(z) = --------------------------------------- = ------------------------
	//            1 + (A1/A0)*z^-1 + (A2/A0)*z^-2        1 + a1*z^-1 + a2*z^-2

	// ��� ������� �������, ���������� ���������� ������������, ������������� ��� � ����� �������� �����.
	// ����� ������������ ������� ��� ����������� �������-���������

	//             s^2
	// H(s) = ------------- , ��� Q - �����������
	//        s^2 + s/Q + 1

	// ��� �������� �� ������������ ������� ����������� ������� � ���������� ������������ ������� ��������� �������
	// ������������� ���������� ���������������, ������� ����������� ����������� �����������:

	//         1       1 - z^-1           2 * PI * frequency
	// s =  ------- * ----------, ��� w = ------------------, ��� frequency - ������� ����� ���, sampleRate - ������� �������������
	//      tg(w/2)    1 + z^-1               sampleRate

	// ��������� ���������� ��������������, �������� ���������� ������������ ������� ��������� ������� � ��������������:
	
	// B0 = (1 + cos(w)) / 2                 B0 = (1 + cos) / 2
	// B1 = - (1 + cos(w))                   B1 = - (1 + cos)            
	// B2 = (1 + cos(w)) / 2         ���     B2 = (1 + cos) / 2,   ���  cos = cos(w), alpha = sin(w) / (2 * Q)
	// A0 = 1 + sin(w) / (2 * Q)             A0 = 1 + alpha             
	// A1 = - 2 * cos(w)                     A1 = - 2 * cos
	// A2 = 1 - sin(w) / (2 * Q)             A2 = 1 - alpha

	// ��� ��������������� ������� ������:
	//		- ����������� (Q), ������ 1 / sqrt(2) - �������� 3 ��/������
	//		- ����������� ������ (frequency / sampleRate), ������ 0.00001 (0.5 �� ��� 50 ���)

	/*double SAMPLE_RATE =	50000.;
	double FREQUENCY =		1.;
	double OMEGA =			IPP_2PI * FREQUENCY / SAMPLE_RATE;
	double ALPHA =			sin(OMEGA) / sqrt(2.);
	double COS =			cos(OMEGA);
	double B0 =				(1. + COS) / 2.;
	double B1 =				- (1. + COS);
	double B2 =				(1. + COS) / 2.;
	double A0 =				1 + ALPHA;
	double A1 =				-2. * COS;
	double A2 =				1 - ALPHA;
	double b0 =				B0 / A0;
	double b1 =				B1 / A0;
	double b2 =				B2 / A0;
	double a0 =				A0 / A0;
	double a1 =				A1 / A0;
	double a2 =				A2 / A0;

	pTapsD[0] = b0;
	pTapsD[1] = b1;
	pTapsD[2] = b2;
	pTapsD[3] = a0;
	pTapsD[4] = a1;
	pTapsD[5] = a2;*/

	double OMEGA = IPP_2PI *
		max( 0.5 / 50000., double(FreqMinHz) / *pFreqADC );
	double ALPHA = sin(OMEGA) / IPP_SQRT2;
	OMEGA = cos(OMEGA);
	double _A0 = 1. / ( 1. + ALPHA );

	pTapsD[0] =									// b0
	pTapsD[2] = ( 1. + OMEGA ) * _A0 * 0.5;		// b2
	pTapsD[1] =-( 1. + OMEGA ) * _A0;			// b1
	pTapsD[3] = 1.;								// a0
	pTapsD[4] = -2. * OMEGA * _A0;				// a1
	pTapsD[5] = ( 1. - ALPHA ) * _A0;			// a2

	bInit = true;
	for (int i = 0; i < 2; ++i)
	{
		if (pStateBuf[i])
		{
			ippsFree(pStateBuf[i]);
			pStateBuf[i] = nullptr;
		}
		int sizeBuffer(0);
		bInit = bInit & (ippsIIRGetStateSize_64f(2, &sizeBuffer) == ippStsNoErr);
		if (bInit)
			pStateBuf[i] = ippsMalloc_8u(sizeBuffer);
		bInit = bInit & (ippsIIRInit_64f(&pState[i], pTapsD, 2, nullptr, pStateBuf[i]) == ippStsNoErr);
	}

	return bInit;
}
//------------------------------------------------------------------------------
void CIirFiltrNoDC::NewNum(int num)
{
	if ((numCur != num) || (numCur == 0))
	{
		if (numCur)
		{
			delete[] pData;
			pData = nullptr;
		}
		numCur = num;
		pData = new double [numCur];
	}
}
//------------------------------------------------------------------------------
bool CIirFiltrNoDC::InitFiltr(TypeFiltr type)
{	// ����� ��� �������
	if (type == tfNoDC)
	{
		FType = tfNoDC;
		bInit = _Init();				
	}
	else
		bInit = false;
	return bInit;
}
//------------------------------------------------------------------------------
bool CIirFiltrNoDC::Filtration(float *pInpData, float *pOutData, int num)
{
	bool ret;
	if (bInit)
	{
		NewNum(num);
		ret = ippsConvert_32f64f(pInpData, pData, num) == ippStsNoErr;
		ret = ret && ( ippsIIR_64f_I(pData, num, pState[0]) == ippStsNoErr );
		ret = ret && ( ippsIIR_64f_I(pData, num, pState[1]) == ippStsNoErr );
		ret = ret && ( ippsConvert_64f32f(pData, pOutData, num) == ippStsNoErr );
	}
	else
		ret = false;
	return ret;
}
//------------------------------------------------------------------------------
bool CIirFiltrNoDC::Filtration_I(float *pInpOutData, int num)
{
	bool ret;
	if (bInit)
	{		
		NewNum(num);
		ret = ippsConvert_32f64f(pInpOutData, pData, num) == ippStsNoErr;
		ret = ret && ( ippsIIR_64f_I(pData, num, pState[0]) == ippStsNoErr );
		ret = ret && ( ippsIIR_64f_I(pData, num, pState[1]) == ippStsNoErr );
		ret = ret && ( ippsConvert_64f32f(pData, pInpOutData, num) ==
						ippStsNoErr );
	}
	else
		ret = false;
	return ret;
}
//------------------------------------------------------------------------------
bool CIirFiltrNoDC::ReStart()
{
	bool ret;
	if (bInit)
		ret = _Init();
	else
		ret = false;
	return ret;
}
//------------------------------------------------------------------------------