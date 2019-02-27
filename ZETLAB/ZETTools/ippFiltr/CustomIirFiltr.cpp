//----------------------------------------------------------------------
// ���� CustomIirFiltr.cpp
// ������ �� 27.02.2012
// ��� "����", ����������, �������� �.�.
//----------------------------------------------------------------------
#include "StdAfx.h"
#include <ippFiltr\CustomIirFiltr.h>
//----------------------------------------------------------------------
CCustomIirFiltr::CCustomIirFiltr()
	: FType(tfNo)			// ��� ����������
	, Freq(0.f)
    , bInit(false)			// ������ ��
	, pFreqADC(nullptr)
	, pState(nullptr)	//		���������������
	, pStateBuf(nullptr)
{

}
//----------------------------------------------------------------------
CCustomIirFiltr::~CCustomIirFiltr()
{
	if (pStateBuf)
	{
		ippsFree(pStateBuf);
		pStateBuf = nullptr;
	}
}
//----------------------------------------------------------------------
bool CCustomIirFiltr::Create(
	float *pfreqADC)	// ��������� �� ������� ������������� ���
{
	pFreqADC = pfreqADC;
	return *pFreqADC > 0.f;
}
//----------------------------------------------------------------------
bool CCustomIirFiltr::_InitFiltr(const float *pTaps, int order)
{// ������������� ��������� �������
	if (pStateBuf)			// ������� ������ ������ ���������
	{
		ippsFree(pStateBuf);
		pStateBuf = nullptr;
	}

	int sizeBuffer(0);
	bInit = ippsIIRGetStateSize_32f(order, &sizeBuffer) == ippStsNoErr;
	if (bInit)
		pStateBuf = ippsMalloc_8u(sizeBuffer);
	bInit = bInit & (ippsIIRInit_32f(&pState, pTaps, order, nullptr, pStateBuf) == ippStsNoErr);

	return bInit;
}
//----------------------------------------------------------------------
bool CCustomIirFiltr::_Filtration(
	float *pInpData,	// ��������� �� �������� ������
	float *pOutData,	// ��������� �� ��������������� ������
	int num,			// ���-�� ��������
	bool flag_I)		// ���� ���� �������
{// ������� ���������� �� ��� ������ �����
	bool ret;
	IppStatus status;
	if (bInit)
	{
		if (flag_I)
			status = ippsIIR_32f_I(pInpData, num, pState);
		else
			status = ippsIIR_32f(pInpData, pOutData, num, pState);													
		ret = status == ippStsNoErr;
	}
	else
		ret = false;
	return ret;
}
//----------------------------------------------------------------------
bool CCustomIirFiltr::InitFiltr(TypeFiltr type, float freq)
{
	bool ret;
	if (type == tfUser)
	{
		FType = tfUser;
		ret = true;
	}
	else
		ret = false;
	return ret;
}
//----------------------------------------------------------------------
bool CCustomIirFiltr::Filtration(
	float *pInpData,	// ��������� �� �������� ������
	float *pOutData,	// ��������� �� ��������������� ������
	int num)			// ���-�� ��������
{
	return _Filtration(pInpData, pOutData, num, false);
}
//----------------------------------------------------------------------
bool CCustomIirFiltr::Filtration_I(
	float *pInpOutData,	// ��������� �� ��������/�������� ������
	int num)			// ���-�� ��������
{
	return _Filtration(pInpOutData, pInpOutData, num, true);
}
//----------------------------------------------------------------------
bool CCustomIirFiltr::ReStart()
{// ����������������� ��������� ������� � �������� �����������.
// ��������� ��� ��������� ������� ������������� ��� ���
// ��� ����������� ���������. ��� tfUser ������� ������� SetUserTaps().
	bool ret;
	if (bInit)
	{// ��������� ��� ������
		if (FType != tfUser)
			ret = InitFiltr(FType, Freq);
		else
			ret = false;
	}
	else	// ��������� ��� �� ������
		ret = false;
	return ret;
}
//----------------------------------------------------------------------
bool CCustomIirFiltr::SetUserTaps(float *ptaps, int order)
{// ������������� ������� tfUser. � �� ����� ������
	bool ret;
	if (FType == tfUser)
		ret = _InitFiltr(ptaps, order);
	else
		ret = false;
	return ret;
}
//----------------------------------------------------------------------