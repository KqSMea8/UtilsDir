//----------------------------------------------------------------------
// ���� IirFiltrSimpleDouble.cpp
// ������ �� 03.05.2012
// ��� "����", ����������, �������� �.�.
//----------------------------------------------------------------------
#include "StdAfx.h"
#include <ippFiltrDouble\IirFiltrSimpleDouble.h>
//----------------------------------------------------------------------
// ��� ����������
const double pTapsNo[4]     = {1., 0.,				// �����. Bn
								1., 0.};			// �����. An
// ����������� ���������� ����� ��� �����������������
const double pTapsDiff1[6]  = {0.5, 0., -0.5,		// �����. Bn
								1., 0., 0.};		// �����. An
// ���������� ����� ��� �������� �����������������
const double pTapsDiff2[6]  = {1., -2., 1.,			// �����. Bn
								1., 0., 0.};		// �����. An
// ������������ ������������ �������� ����������� (������� ��������������
// �������� ������� �������, ������� �������� �������� ������� �������)
// �������������� ������������ ��������
const double pTapsInt1[4]   = {0.875, 0.125,		// �����. Bn
								1., -1.,};			// �����. An
// �������������� �������� ����������� "g"
// � ��������� ���������� � "��/c"
const double pTapsInt1g[4]  = {875.*g, 125.*g,		// �����. Bn
								1., -1.,};			// �����. An
// �������������� �������� ����������� "�/�^2"
// � ��������� ���������� � "��/c"
const double pTapsInt1m[4]  = {875., 125.,			// �����. Bn
								1., -1.,};			// �����. An
//----------------------------------------------------------------------
CIirFiltrSimpleDouble::CIirFiltrSimpleDouble() : CCustomIirFiltrDouble()
{
	ZeroMemory(pTaps, 6*sizeof(double));
}
//----------------------------------------------------------------------
CIirFiltrSimpleDouble::~CIirFiltrSimpleDouble()
{
}
//----------------------------------------------------------------------
bool CIirFiltrSimpleDouble::InitFiltr(TypeFiltr type, double freq)
{	// ����� ��� �������
	int order;
	double a = *pFreqADC;
	if ( (type == tfNo)    ||						// if ������ ����
		 (type == tfDiff1) || (type == tfDiff2) ||	// ������ �����
		 (type == tfInt1)  || (type == tfInt1g) || (type == tfInt1m) )
	{
		FType = type;
		switch (FType)
		{
		case tfNo :
			order = 1;
			ippsCopy_64f(pTapsNo, pTaps, 4);
			break;
		case tfDiff1 :
			order = 2;
			ippsCopy_64f(pTapsDiff1, pTaps, 6);
			pTaps[0] *= a;
			pTaps[2] *= a;
			break;
		case tfDiff2 :
			order = 2;
			ippsCopy_64f(pTapsDiff2, pTaps, 6);
			a *= a;
			pTaps[0] *= a;
			pTaps[1] *= a;
			pTaps[2] *= a;
			break;
		case tfInt1:
			order = 1;
			ippsCopy_64f(pTapsInt1, pTaps, 4);
			pTaps[0] /= a;
			pTaps[1] /= a;
			break;
		case tfInt1g :
			order = 1;
			ippsCopy_64f(pTapsInt1g, pTaps, 4);
			pTaps[0] /= a;
			pTaps[1] /= a;
			break;
		case tfInt1m :
			order = 1;
			ippsCopy_64f(pTapsInt1m, pTaps, 4);
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