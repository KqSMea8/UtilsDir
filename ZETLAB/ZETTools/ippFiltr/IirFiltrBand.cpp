//----------------------------------------------------------------------
// ���� CIirFiltrBand.cpp
// ������ �� 26.02.2012
// ��� "����", ����������, �������� �.�.
//----------------------------------------------------------------------
#include "StdAfx.h"
#include <ippFiltr\IirFiltrBand.h>
//----------------------------------------------------------------------
CIirFiltrBand::CIirFiltrBand()
	: nTemp(0)
	, pTemp(NULL)
	, FType(tfNo)
	, bNeedHi(false)
	, bNeedLo(false)
	, bInit(false)
{
	pFiltrLoPass = pFiltrHiPass = NULL;
	Ffmin = Ffmax = 0.f;

}
//----------------------------------------------------------------------
CIirFiltrBand::~CIirFiltrBand()
{
	if (pFiltrLoPass)
	{
		delete pFiltrLoPass;
		pFiltrLoPass = nullptr;

	}

	if (pFiltrHiPass)
	{
		delete pFiltrHiPass;
		pFiltrHiPass = nullptr;

	}
	if (pTemp)
	{
		delete[] pTemp;
		pTemp = nullptr;
	}
}
//----------------------------------------------------------------------
bool CIirFiltrBand::Create(
	float *pfreqADC)	// ��������� �� ������� ������������� ���
{
	bool flag;
	pFiltrLoPass = new CIirFiltrInternal();
	pFiltrHiPass = new CIirFiltrInternal();
	flag = pFiltrLoPass->Create(pfreqADC) &&
			pFiltrHiPass->Create(pfreqADC);
	if (!flag)
	{
		delete pFiltrLoPass;
		pFiltrLoPass = nullptr;
		delete pFiltrHiPass;
		pFiltrLoPass = nullptr;
	}
	return flag;
}
//----------------------------------------------------------------------
// ��������� - ����� ������ ��� ���
// ������������ ��������: true - ������ �����, false - ������ �� �����
bool CIirFiltrBand::DefineNeedFiltr(
	float freq)			// ����������� ������� ����� ������� ��� 0
{
	bool ret;
	if ( freq > 0.f )
		ret = true;		// ������ �����
	else
		ret = false;	// ������ �� �����
	return ret;
}
//----------------------------------------------------------------------
float CIirFiltrBand::GetFmin()
{
	float f_ret;
	if (FType == tfBandPass)
		f_ret = pFiltrHiPass->GetFreq();
	else
		f_ret = pFiltrLoPass->GetFreq();
	return f_ret;
}
//----------------------------------------------------------------------
float CIirFiltrBand::GetFmax()
{
	float f_ret;
	if (FType == tfBandPass)
		f_ret = pFiltrLoPass->GetFreq();
	else
		f_ret = pFiltrHiPass->GetFreq();
	return f_ret;
}
//----------------------------------------------------------------------
// ������� ����������. ���������� true, ���� ��������� (���� ��� ���)
// ������� ������������������� � ��������� ���� ����������
bool CIirFiltrBand::Filtration(
	float *pInpData,		// ��������� �� �������� ������
	float *pOutData,		// ��������� �� ��������������� ������
	int num)				// ���-�� ��������
{
	bool ret;
	if (bInit)
	{
		if ( bNeedHi && bNeedLo )
		{// ��� � ��� ����� ���
			if ( FType == tfBandPass)
				ret = pFiltrHiPass->Filtration(pInpData, pOutData, num) &&
					  pFiltrLoPass->Filtration_I(pOutData, num);
			else
			{// FType = tfBandStop
				if (num > nTemp)
				{
					delete [] pTemp;
					pTemp = nullptr;
					pTemp = new float [num];
					nTemp = num;
				}
				ret = pFiltrLoPass->Filtration(pInpData, pTemp, num) &&
					  pFiltrHiPass->Filtration(pInpData, pOutData, num) &&
					  ( ippsAdd_32f_I(pTemp, pOutData, num) == ippStsNoErr );
			}
		}
		else if (bNeedHi)	// ����� ������ ���
			ret = pFiltrHiPass->Filtration(pInpData, pOutData, num);

		else if (bNeedLo)	// ����� ������ ���
			ret = pFiltrLoPass->Filtration(pInpData, pOutData, num);

		else
			ret = ippsCopy_32f(pInpData, pOutData, num) == ippStsNoErr;
	}
	else
		ret = false;
	return ret;
}
//----------------------------------------------------------------------
bool CIirFiltrBand::Filtration_I(float *pInpOutData, int num)
{
	bool ret;
	if (bInit)
	{
		if ( bNeedHi && bNeedLo )
		{// ��� � ��� ����� ���
			if ( FType == tfBandPass)
				ret = pFiltrHiPass->Filtration_I(pInpOutData, num) &&
					  pFiltrLoPass->Filtration_I(pInpOutData, num);
			else
			{// FType = tfBandStop
				if (num > nTemp)
				{
					delete [] pTemp;
					pTemp = nullptr;
					pTemp = new float [num];
					nTemp = num;
				}
				ret = pFiltrHiPass->Filtration(pInpOutData, pTemp, num) &&
					  pFiltrLoPass->Filtration_I(pInpOutData, num) &&
					  ( ippsAdd_32f_I(pTemp, pInpOutData, num) == ippStsNoErr );
			}
		}
		else if (bNeedHi)	// ����� ������ ���
			ret = pFiltrHiPass->Filtration_I(pInpOutData, num);

		else if (bNeedLo)	// ����� ������ ���
			ret = pFiltrLoPass->Filtration_I(pInpOutData, num);

		else
			ret = true;
	}
	else
		ret = false;
	return ret;
}
//----------------------------------------------------------------------
bool CIirFiltrBand::InitFiltr(
	TypeFiltr type,		// ��� �������
	float fmin,			// ������� ����� ��� 0
	float fmax)			// ������� ����� ��� 0
{// ������������� �������� ��� � ���
	bool retHi, retLo;
	if ( type == tfBandPass )
	{
		FType = tfBandPass;
		// ��������� ����� ������������� ��������, ���
		// ������������ � ������ ��������
		bNeedHi = DefineNeedFiltr(fmin);
		bNeedLo = DefineNeedFiltr(fmax);

		if ( bNeedHi )
			retHi = pFiltrHiPass->InitFiltr(tfHighPass, fmin);
		else
			retHi = true;
		if ( bNeedLo )
			retLo = pFiltrLoPass->InitFiltr(tfLowPass,  fmax);
		else
			retLo = true;
		bInit = retHi && retLo;
	}
	else if ( type == tfBandStop )
	{
		FType = tfBandStop;
		bNeedHi = DefineNeedFiltr(fmax);
		bNeedLo = DefineNeedFiltr(fmin);

		if ( bNeedHi )
			retHi = pFiltrHiPass->InitFiltr(tfHighPass, fmax);
		else
			retHi = true;
		if ( bNeedLo )
			retLo = pFiltrLoPass->InitFiltr(tfLowPass,  fmin);
		else
			retLo = true;
		bInit = retHi && retLo;
	}
	else
		bInit = false;
	return bInit;
}
//----------------------------------------------------------------------
float CIirFiltrBand::PriorValueFmin(float freq)
{
	float f_ret;
	if ( FType == tfBandPass )
		f_ret = pFiltrHiPass->PriorValue(freq);
	else
		f_ret = pFiltrLoPass->PriorValue(freq);
	return f_ret;
}
//----------------------------------------------------------------------
float CIirFiltrBand::PriorValueFmax(float freq)
{
	float f_ret;
	if ( FType == tfBandPass )
		f_ret = pFiltrLoPass->PriorValue(freq);
	else
		f_ret = pFiltrHiPass->PriorValue(freq);
	return f_ret;
}
//----------------------------------------------------------------------
bool CIirFiltrBand::ReStart()
{
	return bInit &&
		pFiltrHiPass->ReStart() && pFiltrLoPass->ReStart();
}
//----------------------------------------------------------------------