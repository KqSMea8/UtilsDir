//----------------------------------------------------------------------
// ���� IirFiltrSimple.h
// ������ �� 26.02.2012
// ��� "����", ����������, �������� �.�.
//----------------------------------------------------------------------
//		����� ��������� ������� IIR �������, ������������ � �������
// ������� �������� � ������.
//		� ������ ������������ ������� ��������� IPP 7.0.
//		���������� ��������� �������: �� ������ ������ - ���� ������!!!
//----------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <ippFiltr\CustomIirFiltr.h>
//----------------------------------------------------------------------
class CIirFiltrSimple : public CCustomIirFiltr
{
private:
	float pTaps[6];	// 2*(2+1) = 6, 2 - ������������ �������

protected:

public:
	CIirFiltrSimple();
	virtual ~CIirFiltrSimple();

	// ������ ������/������ ������� -----------------

	// ������ ������ --------------------------------
	virtual bool InitFiltr(TypeFiltr type, float freq = 0.f);
};
//----------------------------------------------------------------------
