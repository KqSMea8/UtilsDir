//----------------------------------------------------------------------
// ����� �������, ������������ ���������� �������
//----------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <Intel\ipp\ipps.h>
#include <FiltrDef.h>
//----------------------------------------------------------------------
class CMovingAverageFiltr
{
private:
	float *m_pData;				// ������������� ������ ������ � ���
	int m_sizeData;				//			������
	int m_sizeFiltr;			// ����� �������, ����� �� ������ �����
	int m_sizeFiltr_2;			// �������� ����� �������

protected:
	bool m_bInit;				// ���� ������������� �������
	TypeFiltr m_FType;			// ��� �������
	float *m_pFreqADC;			// ��. �� ������� ������������� � ��

	bool _InitFiltr(int lenght);
	bool _Filtration(float *pInpData, int num);

public:
	CMovingAverageFiltr();
	virtual ~CMovingAverageFiltr();
	bool Create(float *pfreqADC);

	// ������ ������/������ ������� -----------------
	TypeFiltr GetTypeFiltr() { return m_FType; }
	int GetLengthInPoint();
	float GetLengthInTime();

	// ������ ������ --------------------------------
	bool InitFiltrInPoint(int lenght);
	bool InitFiltrInTime(float lenTime);

	// ������� ����������
	bool Filtration(float *pInpData, float *pOutData, int num);
	bool Filtration_I(float *pInpOutData, int num);
};
//----------------------------------------------------------------------

