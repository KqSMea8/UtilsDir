//----------------------------------------------------------------------
// ����� ��������� ��������� ��������� ������
//----------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <Intel\ipp\ipps.h>
#include <FiltrDef.h>
#include <Buffer\SmartArray.h>
//----------------------------------------------------------------------
struct IppsMdaState_32f
{
	int len;		// ����� �������
	int len_1;		// ����� ������� ��� 1
	int *pDistIndx;	// ������ �������� ��������, ������ len
	int *pTmpIndx;	// ������ ��������� ��������, ������ len
	float *pTail;	// ����� ������, ������ len-1
	int IndxMda;	// ������ �������� ��������
	BOOL bOdd;		// �������� ����� �������
	bool bEnabledTail;	// ������ pTail �����

	IppsMdaState_32f();
	~IppsMdaState_32f();
	bool Create(int lenght);
	void Delete();
	void ReStart();
};
//----------------------------------------------------------------------
class CMedianFiltr
{
private:
	IppsMdaState_32f *m_pState;	// ��������� �������
	float *m_pData;				// ������������� ������ ������ � ���
	int m_sizeData;				//			������
	SmartArray<Ipp8u> m_sa_8u;

protected:
	bool m_bInit;				// ���� ������������� �������
	TypeFiltr m_FType;			// ��� �������
	float *m_pFreqADC;			// ��. �� ������� ������������� � ��

	bool _InitFiltr(int lenght);
	bool _Filtration(float *pInpData, float *pOutData, int num);

public:
	CMedianFiltr();
	virtual ~CMedianFiltr();
	bool Create(float *pfreqADC);

	// ������ ������/������ ������� -----------------
	TypeFiltr GetTypeFiltr() { return m_FType; }
	BOOL GetOdd();
	int GetLengthInPoint();
	float GetLengthInTime();

	// ������ ������ --------------------------------
	// ����������� ������� ������������� ������� ������ �����������.
	// � ����������� ���������� ��������������
	bool InitFiltrInPoint(int lenght);
	bool InitFiltrInTime(float lenTime);

	// ������� ����������
	bool Filtration(float *pInpData, float *pOutData, int num);
	bool Filtration_I(float *pInpOutData, int num);

	// ����������������� ������� �� ������� �����������
	bool ReStart();
};
//----------------------------------------------------------------------

