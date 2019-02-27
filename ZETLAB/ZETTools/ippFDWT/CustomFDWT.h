//------------------------------------------------------------------------------
//	���� CustomFDWT.h
//	����������, ��� "����", �������� �.�.
//------------------------------------------------------------------------------
//		������� ����� ������������ ��� ������������� ��������� �
// ��������� ���������� ��� ������� ������������� �������� �����������
// ������� �������������� (Fast Discrete Wavelet Transform)
// ���������� ��������.
//		���������� ����� ������ ����� ������������� ���������� ���������
// ��������. � ������ ������������ ������� ��������� IPP 7.0.
//		��������� ��������� (� ������� �� ���������) � ������������ ���
// ��������� ���������:
//	1. ��� ������������ ��������, ���������� ����� ��������;
//	2. ����� ������� (����� ��������� INFINITE ��� �������� ����
//		"�������-������"), ��������� � ������ �������� ����������
//		������������ ������� ����������;
//	3. ������� ����������.
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include "math.h"
#include <Intel\ipp\ipps.h>
#include <ippFDWT\Taps_Wavelet.h>
//------------------------------------------------------------------------------
enum TypeWaveletRetValue
{// �������� �����, ������� ����� ���������� � ������� ����������� ���
	wtrvError = 0x80000000,	// ��� ������ 
	wtrvNoError = 0,		// ������ ���, ��������� ��������� �����������
	wtrvNewTypeWavelet = 1,	// ������� �� ������������ ��� ��������,
							// ���������� ������� �����
	wtrvNewLevel = 2,		// ��������� ������� ���������� ����������,
							// ���������� ������������ �� ���������
	wtrvNewLengthSignal = 4	// �������� ������������ ����� �������,
};							// ����������� ����������� �� ����������
//------------------------------------------------------------------------------
enum TypeWavelet
{// �������� ����� ����������� ���������
	Haar,
	db1,
	db2,
	db3,
	db4,
	db5,
	db6,
	db7,
	db8,
	db9,
	db10,
	sym1,
	sym2,
	sym3,
	sym4,
	sym5,
	sym6,
	sym7,
	coif1,
	coif2,
	coif3,
	coif4,
	coif5,
	no
};
//------------------------------------------------------------------------------
class CCustomFDWT
{
private:
	int FQuantityWavelet;	// ���-�� ����������� ���������
	long _setTypeWavelet(TypeWavelet val);
	void _setLevel(int val);
	void _setLevelMaxCur();
	void _setLengthSignal(int val);
	CString *m_pNameWavelet;

protected:
	int fwdFltOffs;
	int invFltOffs;
	bool bInitForward;		// ��������� ������� ������. �������������������
	bool bInitInverse;		// ��������� ��������� ������. �������������������
	bool FbNeedInverse;		// ��������� ������ ������ ��������������
	int FLevelOld;			// �������
	int FLengthSignalOld;	//		��������

	int FLevel;				// ������� ����������
	int FLevelMaxCur;		// ����. ������� ��� ������ ����� �������
	int FLevelMaxAll;		// ������� ����������� �� ������������ �������
	int FLengthFilter;		// ����� ��������
	int FLengthSignal;		// ����� ��������� �������
	int FLengthSignalClient;// ����������� ����� �������
	TypeWavelet FTypeWavelet;// ��� ���� �������� (�� 0 �� 23)

	// 30 - ������������ ����� ��� coif5
	float pTaps_fwdLo[30], pTaps_fwdHi[30],
		  pTaps_invLo[30], pTaps_invHi[30];

	void SetpSize();
	void SetLevelMaxCur();

	// ����������� �������, ����������� �������� ������� ��� ��������� ����������,
	// � ����������� ��� ����� ����������������
	virtual void NewTypeWavelet() {}
	virtual void NewLevel() {}
	virtual void NewLengthSignal() {}
	virtual void SetNeedInverse(bool val) { FbNeedInverse = val; }

public:
	CCustomFDWT();
	virtual ~CCustomFDWT();
	virtual long Create(TypeWavelet typeWavelet,
		int level, bool needInverse, int lengthSignal = INFINITE);

	// ������ ������������ �������� Di ��� ������������� ����������
	// ��� ��������� ����� ��� ���������
	int m_pSize[32];

	// ������ ������/������ ������� -----------------
	bool GetNeedInverse() { return FbNeedInverse; }

	int GetQuantityWavelet() {return FQuantityWavelet;}
	TypeWavelet GetTypeWavelet() {return FTypeWavelet;}
	int GetLengthFiltr() {return FLengthFilter;}

	int GetLengthSignal() {return FLengthSignal;}

	int GetLevel() {return FLevel;}	
	int GetLevelMaxCur() {return FLevelMaxCur;}
	int GetLevelMaxAll() {return FLevelMaxAll;}

	CString NameWavelet(int index);

	// ��������� ���������� �����
	long SetLevel(int val);
	void SetLevelMaxAll(int val);
	long SetLengthSignal(int val, bool bNewLevel = false);
	long SetTypeWavelet(TypeWavelet val);

	// ������ ������ ----------------------------
};
//------------------------------------------------------------------------------

