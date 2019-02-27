//----------------------------------------------------------------------
// ���� IirFiltrSimple.cpp
// ������ �� 28.02.2012
// ��� "����", ����������, �������� �.�.
//----------------------------------------------------------------------
#include "StdAfx.h"
#include <ippFiltr\IirFiltrSimple.h>
//----------------------------------------------------------------------
// ��� ����������
const float pTapsNo[4]     = {1.f, 0.f,				// �����. Bn
								1.f, 0.f};			// �����. An
// ����������� ���������� ����� ��� �����������������
const float pTapsDiff1[6]  = {0.5f, 0.f, -0.5f,		// �����. Bn
								1.f, 0.f, 0.f};		// �����. An
// ���������� ����� ��� �������� �����������������
const float pTapsDiff2[6]  = {1.f, -2.f, 1.f,		// �����. Bn
								1.f, 0.f, 0.f};		// �����. An
// �������������� ������������ ��������
const float pTapsInt1[4]   = {0.5f, 0.5f,			// �����. Bn
								1.f, -1.f,};			// �����. An
// �������������� �������� ����������� "g"
// � ��������� ���������� � "��/c"
const float pTapsInt1g[4]  = {500.f*g, 500.f*g,		// �����. Bn
								1.f, -1.f,};			// �����. An
// �������������� �������� ����������� "�/�^2"
// � ��������� ���������� � "��/c"
const float pTapsInt1m[4]  = {500.f, 500.f,			// �����. Bn
								1.f, -1.f,};			// �����. An
//----------------------------------------------------------------------
CIirFiltrSimple::CIirFiltrSimple() : CCustomIirFiltr()
{
	ZeroMemory(pTaps, sizeof(float) * 6);
}
//----------------------------------------------------------------------
CIirFiltrSimple::~CIirFiltrSimple()
{
}
//----------------------------------------------------------------------
bool CIirFiltrSimple::InitFiltr(TypeFiltr type, float freq)
{	// ����� ��� �������
	int order;
	float a = *pFreqADC;
	if ( (type == tfNo)    ||						// if ������ ����
		 (type == tfDiff1) || (type == tfDiff2) ||	// ������ �����
		 (type == tfInt1)  || (type == tfInt1g) || (type == tfInt1m) )
	{
		FType = type;
		switch (FType)
		{
		case tfNo :
			order = 1;
			ippsCopy_32f(pTapsNo, pTaps, 4);
			break;
		case tfDiff1 :
			order = 2;
			ippsCopy_32f(pTapsDiff1, pTaps, 6);
			pTaps[0] *= a;
			pTaps[2] *= a;
			break;
		case tfDiff2 :
			order = 2;
			ippsCopy_32f(pTapsDiff2, pTaps, 6);
			a *= a;
			pTaps[0] *= a;
			pTaps[1] *= a;
			pTaps[2] *= a;
			break;
		case tfInt1:
			order = 1;
			ippsCopy_32f(pTapsInt1, pTaps, 4);
			pTaps[0] /= a;
			pTaps[1] /= a;
			break;
		case tfInt1g :
			order = 1;
			ippsCopy_32f(pTapsInt1g, pTaps, 4);
			pTaps[0] /= a;
			pTaps[1] /= a;
			break;
		case tfInt1m :
			order = 1;
			ippsCopy_32f(pTapsInt1m, pTaps, 4);
			pTaps[0] /= a;
			pTaps[1] /= a;
			//break;
		}
		bInit = true;
	}
	else
		bInit = false;
	// ������������� ��������� �������
	if (bInit)
		bInit = _InitFiltr(pTaps, order);
	return bInit;
}
//----------------------------------------------------------------------