//!----------------------------------------------------------------------------
//! ����� ������������ ��� ���������� ������� ������ �� ������������ ������.
//! ����� ����� �������������� ��� ���������� �������� �������.
//! ����� ������� ������ 2015 �.
//!----------------------------------------------------------------------------
#pragma once

#include "SRS.h"

//!	��������� � �������� ����������� ���������
typedef struct _SResponseIn
{
	double dSampling;			//!	������� ������������� �������, ��
	double dFreqBegin;			//!	������ ������� � ������� ������, ��
	double dFreqEnd;			//! ��������� ������� � ������� ������, ��
	double dDecrement;			//! ��������������� ��������� ���������
	long lOctaveNum;			//! ���������� ����� � ����� ������
	long lSize;					//! ������ �������� ������� � �������
	double *pData;				//! ������� ������ � ������� �������������

	_SResponseIn()
	{
		dSampling = 0.0;
		dFreqBegin = 0.0;
		dFreqEnd = 0.0;
		dDecrement = 0.0;
		lOctaveNum = 0;
		lSize = 0;
		pData = nullptr;
	}
} SResponseIn;

//! ��������� � ��������� ����������� ���������
typedef struct _SResponseOut
{
	long lSize;					//! ������ �������� �������� ������
	double *pFreqs;				//! ��������� ���
	double *pReps;				//! �������� �������� �������

	_SResponseOut()
	{
		lSize = 0;
		pFreqs = nullptr;
		pReps = nullptr;
	}
} SResponseOut;

//! ����� ��������� ������� �������
typedef enum _ESRSType
{
	Overall = 0,		//!	��������� ��������� ������� ������
	Positive = 1,		//!	������������� ��������� ������� ������
	Negative = 2		//!	������������� ��������� ������� ������
} ESRSType;

class CResponseSpectrum
{
private:
	SResponseIn m_struct;		//!	��������� � ����������� ������� � �������� � ������� ��������
	SResponseOut m_response;	//!	��������� � ��������� ������� ������

	Srs m_spectr;				//!	�����, ����������� ������ ������� ������

protected:
	long SetInputStruct(SResponseIn *pInData);			//!	������� ����� ��������� � ����������� ������������� �������
	long SetInputData(double *pData, long size);		//!	������� ����� ������ � ������� ��� ������� �������
	long GetOutputStruct(SResponseOut *pOutData);		//!	������� �������� ��������� � ������������ ������� ������� ������

	long CalculateSRSAAM(ESRSType typeSRS = Overall);	//!	������� ��������� ������ ������� ������

public:
	CResponseSpectrum();
	~CResponseSpectrum();

	long Calculate(SResponseIn *pInData, SResponseOut *pOutData, ESRSType type = Overall);
};

