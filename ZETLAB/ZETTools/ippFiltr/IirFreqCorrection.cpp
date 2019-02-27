//------------------------------------------------------------------------------
// Файл IirFiltrNoDC.cpp
// Версия от 05.03.2012
// ЗАО "ЭТМС", Зеленоград, Савостин Ю.М.
//------------------------------------------------------------------------------
#include "StdAfx.h"
#include <ippFiltr\IirFreqCorrection.h>
//------------------------------------------------------------------------------
//							   f1						Q1				 f2					 Q2				f3						f4						Q4			f5			Q5		f6		 Q6		  K
const double fc[9][12] =	{{0.4,					1. / IPP_SQRT2,		100.,			1. / IPP_SQRT2,		16.,					16.,					0.55,		2.5,		0.9,	4.,		0.95,	1.024	},		//Wb
							 {0.4,					1. / IPP_SQRT2,		100.,			1. / IPP_SQRT2,		8.,						8.,						0.63,		0.,			1.	,	0.,		1.,		1.		},		//Wc
							 {0.4,					1. / IPP_SQRT2,		100.,			1. / IPP_SQRT2,		2.,						2.,						0.63,		0.,			1.	,	0.,		1.,		1.		},		//Wd
							 {0.4,					1. / IPP_SQRT2,		100.,			1. / IPP_SQRT2,		1.,						1.,						0.63,		0.,			1.	,	0.,		1.,		1.		},		//We
							 {0.08,					1. / IPP_SQRT2,		0.63,			1. / IPP_SQRT2,		0.,						0.25,					0.86,		0.0625,		0.8	,	0.1,	0.8,	1.		},		//Wf
							 {pow(10., 0.8),		1. / IPP_SQRT2,		pow(10., 3.1),	1. / IPP_SQRT2,		100. / IPP_2PI,			100. / IPP_2PI,			0.64,		0.,			1.	,	0.,		0.,		1.		},		//Wh
							 {0.4,					1. / IPP_SQRT2,		100.,			1. / IPP_SQRT2,		0.,						0.,						1.,			3.75,		0.91,	5.32,	0.91,	1.		},		//Wj
							 {0.4,					1. / IPP_SQRT2,		100.,			1. / IPP_SQRT2,		12.5,					12.5,					0.63,		2.37,		0.91,	3.35,	0.91,	1.		},		//Wk
							 {pow(10., -0.1),		1. / IPP_SQRT2,		100.,			1. / IPP_SQRT2,		1. / 0.028 / IPP_2PI,	1. / 0.028 / IPP_2PI,	0.5,		0.,			1.,		0.,		1.,		1.		}};		//Wm

CIirFreqCorrection::CIirFreqCorrection() : CCustomIirFiltr()
{
	pStateHPF = pStateLPF = pStateTF = pStateSF1 = pStateSF2 = nullptr;
	pStateHPFBuf = pStateLPFBuf = pStateTFBuf = pStateSF1Buf = pStateSF2Buf = nullptr;
	pData = NULL;
	numCur = 0;
	FCorr = fcWb;
}
//------------------------------------------------------------------------------
CIirFreqCorrection::~CIirFreqCorrection()
{
	if (pData)
	{
		delete[] pData;
		pData = nullptr;
	}
	
	if (pStateHPFBuf)
	{
		ippsFree(pStateHPFBuf);
		pStateHPFBuf = nullptr;
	}
	if (pStateLPFBuf)
	{
		ippsFree(pStateLPFBuf);
		pStateLPFBuf = nullptr;
	}
	if (pStateTFBuf)
	{
		ippsFree(pStateTFBuf);
		pStateTFBuf = nullptr;
	}
	if (pStateSF1Buf)
	{
		ippsFree(pStateSF1Buf);
		pStateSF1Buf = nullptr;
	}
	if (pStateSF2Buf)
	{
		ippsFree(pStateSF2Buf);
		pStateSF2Buf = nullptr;
	}
}
//------------------------------------------------------------------------------
bool CIirFreqCorrection::_Init()
{
	if (FCorr == fcA || FCorr == fcB || FCorr == fcC || FCorr == fcD)
		return InitAcousticCorrection();
	else
		return InitVibroCorrection();
}
//------------------------------------------------------------------------------
void CIirFreqCorrection::NewNum(int num)
{
	if ( (numCur != num) || (numCur == 0) )
	{
		if (numCur)
		{ 
			delete [] pData;
			pData = nullptr;
		}
		numCur = num;
		pData = new double [numCur];
	}
}
//------------------------------------------------------------------------------
bool CIirFreqCorrection::InitFiltr(TypeFiltr type, FreqCorrection corr)
{	// новый тип фильтра
	if (type == tfFreqCorrection)
	{
		FType = tfFreqCorrection;
		FCorr = corr;
		bInit = _Init();				
	}
	else
		bInit = false;
	return bInit;
}
//------------------------------------------------------------------------------
bool CIirFreqCorrection::Filtration(float *pInpData, float *pOutData, int num)
{
	bool ret;
	if (bInit)
	{
		NewNum(num);
		ret = ippsConvert_32f64f(pInpData, pData, num) == ippStsNoErr;
		if (FCorr == fcA || FCorr == fcB || FCorr == fcC || FCorr == fcD)
		{
			ret = ret && (ippsIIR_64f_I(pData, num, pStateHPF) == ippStsNoErr);
			ret = ret && (ippsIIR_64f_I(pData, num, pStateLPF) == ippStsNoErr);
			if (FCorr == fcA || FCorr == fcB || FCorr == fcD)
			{
				ret = ret && (ippsIIR_64f_I(pData, num, pStateSF1) == ippStsNoErr);
				if (FCorr == fcA || FCorr == fcD)
					ret = ret && (ippsIIR_64f_I(pData, num, pStateSF2) == ippStsNoErr);
			}
		}
		else
		{
			ret = ret && (ippsIIR_64f_I(pData, num, pStateHPF) == ippStsNoErr);
			ret = ret && (ippsIIR_64f_I(pData, num, pStateLPF) == ippStsNoErr);
			if (fc[FCorr][4] == 0. && fc[FCorr][5] == 0.)
			{
			}
			else
				ret = ret && (ippsIIR_64f_I(pData, num, pStateTF) == ippStsNoErr);
			if (fc[FCorr][9] == 0. && fc[FCorr][7] == 0.)
			{	
			}
			else
			{
				ret = ret && (ippsIIR_64f_I(pData, num, pStateSF1) == ippStsNoErr);
				ret = ret && (ippsIIR_64f_I(pData, num, pStateSF2) == ippStsNoErr);
			}
		}
		ret = ret && ( ippsConvert_64f32f(pData, pOutData, num) == ippStsNoErr );
	}
	else
		ret = false;
	return ret;
}
//------------------------------------------------------------------------------
bool CIirFreqCorrection::Filtration_I(float *pInpOutData, int num)
{
	bool ret;
	if (bInit)
	{		
		NewNum(num);
		ret = ippsConvert_32f64f(pInpOutData, pData, num) == ippStsNoErr;

		if (FCorr == fcA || FCorr == fcB || FCorr == fcC || FCorr == fcD)
		{
			ret = ret && (ippsIIR_64f_I(pData, num, pStateHPF) == ippStsNoErr);
			ret = ret && (ippsIIR_64f_I(pData, num, pStateLPF) == ippStsNoErr);
			if (FCorr == fcA || FCorr == fcB || FCorr == fcD)
			{
				ret = ret && (ippsIIR_64f_I(pData, num, pStateSF1) == ippStsNoErr);
				if (FCorr == fcA || FCorr == fcD)
					ret = ret && (ippsIIR_64f_I(pData, num, pStateSF2) == ippStsNoErr);
			}
		}
		else
		{
			ret = ret && (ippsIIR_64f_I(pData, num, pStateHPF) == ippStsNoErr);
			ret = ret && (ippsIIR_64f_I(pData, num, pStateLPF) == ippStsNoErr);
			if (fc[FCorr][4] == 0. && fc[FCorr][5] == 0.)
			{
			}
			else
				ret = ret && (ippsIIR_64f_I(pData, num, pStateTF) == ippStsNoErr);
			if (fc[FCorr][9] == 0. && fc[FCorr][7] == 0.)
			{	
			}
			else
			{
				ret = ret && (ippsIIR_64f_I(pData, num, pStateSF1) == ippStsNoErr);
				ret = ret && (ippsIIR_64f_I(pData, num, pStateSF2) == ippStsNoErr);
			}
		}
		
		ret = ret && (ippsConvert_64f32f(pData, pInpOutData, num) == ippStsNoErr);
	}
	else
		ret = false;

	return ret;
}
//------------------------------------------------------------------------------
bool CIirFreqCorrection::ReStart()
{
	bool ret;
	if (bInit)
		ret = _Init();
	else
		ret = false;
	return ret;
}

bool CIirFreqCorrection::InitVibroCorrection()
{
	//Функция частотной коррекции представляет собой произведение передаточных функций трех фильтров: полосового, переходного и ступенчатого.
	double SAMPLE_RATE(*pFreqADC);							
	double FREQUENCY(0.);								
	double Q(0.);									
	double OMEGA(0.);	
	double ALPHA(0.);
	double COS(0.);
	double SIN(0.);
	double B0(0.);
	double B1(0.);
	double B2(0.);
	double A0(0.);
	double A1(0.);
	double A2(0.);
	double b0(0.);
	double b1(0.);
	double b2(0.);
	double a0(0.);
	double a1(0.);
	double a2(0.);

	//Расчет полосового фильтра
	//Расчет ФВЧ полосового фильтра
	FREQUENCY = fc[FCorr][0];								
	Q = fc[FCorr][1];									
	OMEGA = IPP_2PI * FREQUENCY / SAMPLE_RATE;	
	ALPHA = sin(OMEGA) / (2. * Q);
	COS = cos(OMEGA);

	B0 = (1. + COS) / 2.;
	B1 = - (1. + COS);
	B2 = (1. + COS) / 2.;
	A0 = 1. + ALPHA;
	A1 = -2. * COS;
	A2 = 1. - ALPHA;

	b0 = B0 / A0;
	b1 = B1 / A0;
	b2 = B2 / A0;
	a0 = 1.;
	a1 = A1 / A0;
	a2 = A2 / A0;

	pTapsD[0] = b0;
	pTapsD[1] = b1;
	pTapsD[2] = b2;
	pTapsD[3] = a0;
	pTapsD[4] = a1;
	pTapsD[5] = a2;

	if (pStateHPFBuf)
	{
		ippsFree(pStateHPFBuf);
		pStateHPFBuf = nullptr;
	}
	int sizeBuffer(0);
	IppStatus ippStatus1 = ippsIIRGetStateSize_64f(2, &sizeBuffer);
	if (ippStatus1 == ippStsNoErr)
	{
		pStateHPFBuf = ippsMalloc_8u(sizeBuffer);
		ippStatus1 = ippsIIRInit_64f(&pStateHPF, pTapsD, 2, nullptr, pStateHPFBuf);
	}
	
	//Расчет ФНЧ полосового фильтра
	FREQUENCY = fc[FCorr][2];
	Q = fc[FCorr][3];
	OMEGA = IPP_2PI * FREQUENCY / SAMPLE_RATE;
	ALPHA = sin(OMEGA) / (2. * Q);
	COS = cos(OMEGA);

	B0 = (1. - COS) / 2.;
	B1 = (1. - COS);
	B2 = (1. - COS) / 2.;
	A0 = 1. + ALPHA;
	A1 = - 2. * COS;
	A2 = 1. - ALPHA;

	b0 = B0 / A0;
	b1 = B1 / A0;
	b2 = B2 / A0;
	a0 = 1.;
	a1 = A1 / A0;
	a2 = A2 / A0;

	pTapsD[0] = b0;
	pTapsD[1] = b1;
	pTapsD[2] = b2;
	pTapsD[3] = a0;
	pTapsD[4] = a1;
	pTapsD[5] = a2;

	if (pStateLPFBuf)
	{
		ippsFree(pStateLPFBuf);
		pStateLPFBuf = nullptr;
	}
	sizeBuffer = 0;
	IppStatus ippStatus2 = ippsIIRGetStateSize_64f(2, &sizeBuffer);
	if (ippStatus2 == ippStsNoErr)
	{
		pStateLPFBuf = ippsMalloc_8u(sizeBuffer);
		ippStatus2 = ippsIIRInit_64f(&pStateLPF, pTapsD, 2, nullptr, pStateLPFBuf);
	}

	//Расчет переходного фильтра
	IppStatus ippStatus3 = ippStsNoErr;
	if (fc[FCorr][4] == 0. && fc[FCorr][5] == 0.)
	{
	}
	else
	{
		if (fc[FCorr][4] == 0.)
		{
			FREQUENCY = fc[FCorr][5];
			Q = fc[FCorr][6];
			OMEGA = IPP_2PI * FREQUENCY / SAMPLE_RATE;
			ALPHA = sin(OMEGA) / (2. * Q);
			COS = cos(OMEGA);

			B0 = (1. - COS) / 2.;
			B1 = (1. - COS);
			B2 = (1. - COS) / 2.;
			A0 = 1. + ALPHA;
			A1 = - 2. * COS;
			A2 = 1. - ALPHA;
		}
		else
		{
			double FREQUENCY1 = fc[FCorr][4];
			double FREQUENCY2 = fc[FCorr][5];
			Q = fc[FCorr][6];
			double K = fc[FCorr][11];
			double OMEGA1 = IPP_2PI * FREQUENCY1 / SAMPLE_RATE;
			double OMEGA2 = IPP_2PI * FREQUENCY2 / SAMPLE_RATE;
			double SIN1 = sin(OMEGA1);
			double COS1 = cos(OMEGA1);
			double SIN2 = sin(OMEGA2);
			double COS2 = cos(OMEGA2);

			B0 = (1. + (1. + COS1) / SIN1) * K;
			B1 = 2. * K;
			B2 = (1. - (1. + COS1) / SIN1) * K;
			A0 = 1. + (1. + COS2) / (Q * SIN2) + (1. + COS2) / (1. - COS2);
			A1 = - (4. * COS2) / (1. - COS2);
			A2 = 1. - (1. + COS2) / (Q * SIN2) + (1. + COS2) / (1. - COS2);
		}

		b0 = B0 / A0;
		b1 = B1 / A0;
		b2 = B2 / A0;
		a0 = 1.;
		a1 = A1 / A0;
		a2 = A2 / A0;

		pTapsD[0] = b0;
		pTapsD[1] = b1;
		pTapsD[2] = b2;
		pTapsD[3] = a0;
		pTapsD[4] = a1;
		pTapsD[5] = a2;

		if (pStateTFBuf)
		{
			ippsFree(pStateTFBuf);
			pStateTFBuf = nullptr;
		}
		sizeBuffer = 0;
		ippStatus3 = ippsIIRGetStateSize_64f(2, &sizeBuffer);
		if (ippStatus3 == ippStsNoErr)
		{
			pStateTFBuf = ippsMalloc_8u(sizeBuffer);
			ippStatus3 = ippsIIRInit_64f(&pStateTF, pTapsD, 2, nullptr, pStateTFBuf);
		}
	}

	//Расчет ступенчатого фильтра
	IppStatus ippStatus4 = ippStsNoErr;
	IppStatus ippStatus5 = ippStsNoErr;
	if (fc[FCorr][9] == 0. && fc[FCorr][7] == 0.)
	{
	}
	else
	{
		FREQUENCY = fc[FCorr][9];
		Q = fc[FCorr][10];
		double K = fc[FCorr][7]* fc[FCorr][7] / fc[FCorr][9] / fc[FCorr][9];
		OMEGA = IPP_2PI * FREQUENCY / SAMPLE_RATE;
		ALPHA = 	sin(OMEGA) / (2. * Q);
		COS = cos(OMEGA);
		
		B0 = (1. + COS) / 2.;
		B1 = - (1. + COS);
		B2 = (1. + COS) / 2.;
		A0 = 1. + ALPHA;
		A1 = - 2. * COS;
		A2 = 1. - ALPHA;

		b0 = B0 / A0 * K;
		b1 = B1 / A0 * K;
		b2 = B2 / A0 * K;
		a0 = 1.;
		a1 = A1 / A0;
		a2 = A2 / A0;

		pTapsD[0] = b0;
		pTapsD[1] = b1;
		pTapsD[2] = b2;
		pTapsD[3] = a0;
		pTapsD[4] = a1;
		pTapsD[5] = a2;

		if (pStateSF1Buf)
		{
			ippsFree(pStateSF1Buf);
			pStateSF1Buf = nullptr;
		}
		sizeBuffer = 0;
		ippStatus4 = ippsIIRGetStateSize_64f(2, &sizeBuffer);
		if (ippStatus4 == ippStsNoErr)
		{
			pStateSF1Buf = ippsMalloc_8u(sizeBuffer);
			ippStatus4 = ippsIIRInit_64f(&pStateSF1, pTapsD, 2, nullptr, pStateSF1Buf);
		}
		
		FREQUENCY = fc[FCorr][7];
		Q = fc[FCorr][8];
		OMEGA = IPP_2PI * FREQUENCY / SAMPLE_RATE;
		ALPHA = sin(OMEGA) / (2. * Q);
		COS = cos(OMEGA);
		
		A0 = (1. + COS) / 2.;
		A1 = - (1. + COS);
		A2 = (1. + COS) / 2.;
		B0 = 1. + ALPHA;
		B1 = - 2. * COS;
		B2 = 1. - ALPHA;
		
		b0 = B0 / A0;
		b1 = B1 / A0;
		b2 = B2 / A0;
		a0 = 1.;
		a1 = A1 / A0;
		a2 = A2 / A0;

		pTapsD[0] = b0;
		pTapsD[1] = b1;
		pTapsD[2] = b2;
		pTapsD[3] = a0;
		pTapsD[4] = a1;
		pTapsD[5] = a2;

		if (pStateSF2Buf)
		{
			ippsFree(pStateSF2Buf);
			pStateSF2Buf = nullptr;
		}
		sizeBuffer = 0;
		ippStatus5 = ippsIIRGetStateSize_64f(2, &sizeBuffer);
		if (ippStatus5 == ippStsNoErr)
		{
			pStateSF2Buf = ippsMalloc_8u(sizeBuffer);
			ippStatus5 = ippsIIRInit_64f(&pStateSF2, pTapsD, 2, nullptr, pStateSF2Buf);
		}
	}

	return	(ippStatus1 == ippStsNoErr) && (ippStatus2 == ippStsNoErr) && 
			(ippStatus3 == ippStsNoErr) && (ippStatus4 == ippStsNoErr) && 
			(ippStatus5 == ippStsNoErr);
}

bool CIirFreqCorrection::InitAcousticCorrection()
{
	double SAMPLE_RATE(*pFreqADC);							
	double FREQUENCY(0.);								
	double Q(0.);									
	double OMEGA(0.);	
	double ALPHA(0.);
	double COS(0.);
	double SIN(0.);
	double B0(0.);
	double B1(0.);
	double B2(0.);
	double A0(0.);
	double A1(0.);
	double A2(0.);
	double b0(0.);
	double b1(0.);
	double b2(0.);
	double a0(0.);
	double a1(0.);
	double a2(0.);
	IppStatus ippStatus1(ippStsNoErr), ippStatus2(ippStsNoErr), ippStatus3(ippStsNoErr), ippStatus4(ippStsNoErr);

	if (FCorr == fcA || FCorr == fcB || FCorr == fcC)
	{
		//Расчет полосового фильтра
		//Расчет ФВЧ полосового фильтра
		FREQUENCY = 20.6;								
		Q = 0.5;									
		OMEGA = IPP_2PI * FREQUENCY / SAMPLE_RATE;	
		ALPHA = sin(OMEGA) / (2. * Q);
		COS = cos(OMEGA);
		SIN = sin(OMEGA);

		B0 = (1. + COS) / 2.;
		B1 = - (1. + COS);
		B2 = (1. + COS) / 2.;
		A0 = 1. + ALPHA;
		A1 = - 2. * COS;
		A2 = 1. - ALPHA;

		b0 = B0 / A0;
		b1 = B1 / A0;
		b2 = B2 / A0;
		a0 = 1.;
		a1 = A1 / A0;
		a2 = A2 / A0;

		pTapsD[0] = b0;
		pTapsD[1] = b1;
		pTapsD[2] = b2;
		pTapsD[3] = a0;
		pTapsD[4] = a1;
		pTapsD[5] = a2;

		if (pStateHPFBuf)
		{
			ippsFree(pStateHPFBuf);
			pStateHPFBuf = nullptr;
		}
		int sizeBuffer(0);
		ippStatus1 = ippsIIRGetStateSize_64f(2, &sizeBuffer);
		if (ippStatus1 == ippStsNoErr)
		{
			pStateHPFBuf = ippsMalloc_8u(sizeBuffer);
			ippStatus1 = ippsIIRInit_64f(&pStateHPF, pTapsD, 2, nullptr, pStateHPFBuf);
		}

		//Расчет ФНЧ полосового фильтра
		FREQUENCY =	12200.;
		Q = 0.5;
		OMEGA = IPP_2PI * FREQUENCY / SAMPLE_RATE;
		ALPHA = sin(OMEGA) / (2. * Q);
		COS = cos(OMEGA);

		B0 = (1. - COS) / 2.;
		B1 = (1. - COS);
		B2 = (1. - COS) / 2.;
		A0 = 1. + ALPHA;
		A1 = - 2. * COS;
		A2 = 1. - ALPHA;

		b0 = B0 / A0;
		b1 = B1 / A0;
		b2 = B2 / A0;
		a0 = 1.;
		a1 = A1 / A0;
		a2 = A2 / A0;

		pTapsD[0] = b0;
		pTapsD[1] = b1;
		pTapsD[2] = b2;
		pTapsD[3] = a0;
		pTapsD[4] = a1;
		pTapsD[5] = a2;

		if (pStateLPFBuf)
		{
			ippsFree(pStateLPFBuf);
			pStateLPFBuf = nullptr;
		}
		sizeBuffer = 0;
		ippStatus2 = ippsIIRGetStateSize_64f(2, &sizeBuffer);
		if (ippStatus2 == ippStsNoErr)
		{
			pStateLPFBuf = ippsMalloc_8u(sizeBuffer);
			ippStatus2 = ippsIIRInit_64f(&pStateLPF, pTapsD, 2, nullptr, pStateLPFBuf);
		}

		if (FCorr == fcA)
		{
			FREQUENCY = 107.7;
			OMEGA = IPP_2PI * FREQUENCY / SAMPLE_RATE;
			ALPHA = sin(OMEGA) / (1. + cos(OMEGA));
			
			B0 = 1.;
			B1 = - 1.;
			A0 = ALPHA + 1.;
			A1 = ALPHA - 1.;
			
			b0 = B0 / A0;
			b1 = B1 / A0;
			a0 = 1.;
			a1 = A1 / A0;

			pTapsD[0] = b0;
			pTapsD[1] = b1;
			pTapsD[2] = a0;
			pTapsD[3] = a1;

			if (pStateSF1Buf)
			{
				ippsFree(pStateSF1Buf);
				pStateSF1Buf = nullptr;
			}
			sizeBuffer = 0;
			ippStatus3 = ippsIIRGetStateSize_64f(1, &sizeBuffer);
			if (ippStatus3 == ippStsNoErr)
			{
				pStateSF1Buf = ippsMalloc_8u(sizeBuffer);
				ippStatus3 = ippsIIRInit_64f(&pStateSF1, pTapsD, 1, nullptr, pStateSF1Buf);
			}

			FREQUENCY = 737.9;
			OMEGA = IPP_2PI * FREQUENCY / SAMPLE_RATE;
			ALPHA = sin(OMEGA) / (1. + cos(OMEGA));
			
			B0 = 1.;
			B1 = - 1.;
			A0 = ALPHA + 1.;
			A1 = ALPHA - 1.;
			
			b0 = B0 / A0;
			b1 = B1 / A0;
			a0 = 1.;
			a1 = A1 / A0;

			pTapsD[0] = b0;
			pTapsD[1] = b1;
			pTapsD[2] = a0;
			pTapsD[3] = a1;

			if (pStateSF2Buf)
			{
				ippsFree(pStateSF2Buf);
				pStateSF2Buf = nullptr;
			}
			sizeBuffer = 0;
			ippStatus4 = ippsIIRGetStateSize_64f(1, &sizeBuffer);
			if (ippStatus4 == ippStsNoErr)
			{
				pStateSF2Buf = ippsMalloc_8u(sizeBuffer);
				ippStatus4 = ippsIIRInit_64f(&pStateSF2, pTapsD, 1, nullptr, pStateSF2Buf);
			}
		}

		if (FCorr == fcB)
		{
			FREQUENCY = 158.5;
			OMEGA = IPP_2PI * FREQUENCY / SAMPLE_RATE;
			ALPHA = sin(OMEGA) / (1. + cos(OMEGA));
			
			B0 = 1.;
			B1 = - 1.;
			A0 = ALPHA + 1.;
			A1 = ALPHA - 1.;
			
			b0 = B0 / A0;
			b1 = B1 / A0;
			a0 = 1.;
			a1 = A1 / A0;

			pTapsD[0] = b0;
			pTapsD[1] = b1;
			pTapsD[2] = a0;
			pTapsD[3] = a1;

			if (pStateSF1Buf)
			{
				ippsFree(pStateSF1Buf);
				pStateSF1Buf = nullptr;
			}
			sizeBuffer = 0;
			ippStatus3 = ippsIIRGetStateSize_64f(1, &sizeBuffer);
			if (ippStatus3 == ippStsNoErr)
			{
				pStateSF1Buf = ippsMalloc_8u(sizeBuffer);
				ippStatus3 = ippsIIRInit_64f(&pStateSF1, pTapsD, 1, nullptr, pStateSF1Buf);
			}
		}
	}
	else
	{
		FREQUENCY = 282.7;
		OMEGA = IPP_2PI * FREQUENCY / SAMPLE_RATE;
		ALPHA = sin(OMEGA) / (1. + cos(OMEGA));

		B0 = 1.;
		B1 = - 1.;
		A0 = ALPHA + 1.;
		A1 = ALPHA - 1.;

		b0 = B0 / A0;
		b1 = B1 / A0;
		a0 = 1.;
		a1 = A1 / A0;

		pTapsD[0] = b0;
		pTapsD[1] = b1;
		pTapsD[2] = a0;
		pTapsD[3] = a1;

		if (pStateHPFBuf)
		{
			ippsFree(pStateHPFBuf);
			pStateHPFBuf = nullptr;
		}
		int sizeBuffer(0);
		ippStatus1 = ippsIIRGetStateSize_64f(1, &sizeBuffer);
		if (ippStatus1 == ippStsNoErr)
		{
			pStateHPFBuf = ippsMalloc_8u(sizeBuffer);
			ippStatus1 = ippsIIRInit_64f(&pStateHPF, pTapsD, 1, nullptr, pStateHPFBuf);
		}

		FREQUENCY = 1160.;
		OMEGA = IPP_2PI * FREQUENCY / SAMPLE_RATE;
		ALPHA = sin(OMEGA) / (1. + cos(OMEGA));

		B0 = ALPHA;
		B1 = ALPHA;
		A0 = ALPHA + 1.;
		A1 = ALPHA - 1.;

		b0 = B0 / A0;
		b1 = B1 / A0;
		a0 = 1.;
		a1 = A1 / A0;

		pTapsD[0] = b0;
		pTapsD[1] = b1;
		pTapsD[2] = a0;
		pTapsD[3] = a1;

		if (pStateLPFBuf)
		{
			ippsFree(pStateLPFBuf);
			pStateLPFBuf = nullptr;
		}
		sizeBuffer = 0;
		ippStatus2 = ippsIIRGetStateSize_64f(1, &sizeBuffer);
		if (ippStatus2 == ippStsNoErr)
		{
			pStateLPFBuf = ippsMalloc_8u(sizeBuffer);
			ippStatus2 = ippsIIRInit_64f(&pStateLPF, pTapsD, 1, nullptr, pStateLPFBuf);
		}

		FREQUENCY = sqrt(9837328.);
		Q = FREQUENCY / 3424.;
		OMEGA = IPP_2PI * FREQUENCY / SAMPLE_RATE;
		ALPHA = sin(OMEGA) / (2. * Q);
		COS = cos(OMEGA);

		B0 = (1. - COS) / 2.;
		B1 = 1. - COS;
		B2 = (1. - COS) / 2.;
		A0 = 1. + ALPHA;
		A1 = - 2. * COS;
		A2 = 1. - ALPHA;

		b0 = B0 / A0;
		b1 = B1 / A0;
		b2 = B2 / A0;
		a0 = 1.;
		a1 = A1 / A0;
		a2 = A2 / A0;

		pTapsD[0] = b0;
		pTapsD[1] = b1;
		pTapsD[2] = b2;
		pTapsD[3] = a0;
		pTapsD[4] = a1;
		pTapsD[5] = a2;

		if (pStateSF1Buf)
		{
			ippsFree(pStateSF1Buf);
			pStateSF1Buf = nullptr;
		}
		sizeBuffer = 0;
		ippStatus3 = ippsIIRGetStateSize_64f(2, &sizeBuffer);
		if (ippStatus3 == ippStsNoErr)
		{
			pStateSF1Buf = ippsMalloc_8u(sizeBuffer);
			ippStatus3 = ippsIIRInit_64f(&pStateSF1, pTapsD, 2, nullptr, pStateSF1Buf);
		}

		FREQUENCY = sqrt(1037918.48);
		Q = FREQUENCY / 1039.6;
		OMEGA = IPP_2PI * FREQUENCY / SAMPLE_RATE;
		ALPHA = sin(OMEGA) / (2. * Q);
		COS = cos(OMEGA);

		A0 = (1. - COS) / 2.;
		A1 = 1. - COS;
		A2 = (1. - COS) / 2.;
		B0 = 1. + ALPHA;
		B1 = - 2. * COS;
		B2 = 1. - ALPHA;

		b0 = B0 / A0;
		b1 = B1 / A0;
		b2 = B2 / A0;
		a0 = 1.;
		a1 = A1 / A0;
		a2 = A2 / A0;

		pTapsD[0] = b0;
		pTapsD[1] = b1;
		pTapsD[2] = b2;
		pTapsD[3] = a0;
		pTapsD[4] = a1;
		pTapsD[5] = a2;

		if (pStateSF2Buf)
		{
			ippsFree(pStateSF2Buf);
			pStateSF2Buf = nullptr;
		}
		sizeBuffer = 0;
		ippStatus4 = ippsIIRGetStateSize_64f(2, &sizeBuffer);
		if (ippStatus4 == ippStsNoErr)
		{
			pStateSF2Buf = ippsMalloc_8u(sizeBuffer);
			ippStatus4 = ippsIIRInit_64f(&pStateSF2, pTapsD, 2, nullptr, pStateSF2Buf);
		}
	}
	
	return (ippStatus1 == ippStsNoErr) && (ippStatus2 == ippStsNoErr) && (ippStatus3 == ippStsNoErr) && (ippStatus4 == ippStsNoErr);
}

//------------------------------------------------------------------------------