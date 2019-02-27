//------------------------------------------------------------------------------
//	���� CustomDWF.cpp
//	����������, ��� "����", �������� �.�.
//------------------------------------------------------------------------------
#include "StdAfx.h"
#include "CustomFDWT.h"
//------------------------------------------------------------------------------
CCustomFDWT::CCustomFDWT()
{	
	fwdFltOffs = -1;
	invFltOffs = 0;
	bInitForward = bInitInverse = false;
	FQuantityWavelet = 23;
	FLevelMaxAll = 13;
	FTypeWavelet = no;
	FLengthFilter =
		FLevel = FLevelMaxCur = FLevelOld = 
		FLengthSignalOld = FLengthSignal = 0;

	m_pNameWavelet = new CString [FQuantityWavelet];
	m_pNameWavelet[0] = L"Haar";
	m_pNameWavelet[1] = L"db1";
	m_pNameWavelet[2] = L"db2";
	m_pNameWavelet[3] = L"db3";
	m_pNameWavelet[4] = L"db4";
	m_pNameWavelet[5] = L"db5";
	m_pNameWavelet[6] = L"db6";
	m_pNameWavelet[7] = L"db7";
	m_pNameWavelet[8] = L"db8";
	m_pNameWavelet[9] = L"db9";
	m_pNameWavelet[10] = L"db10";
	m_pNameWavelet[11] = L"sym1";
	m_pNameWavelet[12] = L"sym2";
	m_pNameWavelet[13] = L"sym3";
	m_pNameWavelet[14] = L"sym4";
	m_pNameWavelet[15] = L"sym5";
	m_pNameWavelet[16] = L"sym6";
	m_pNameWavelet[17] = L"sym7";
	m_pNameWavelet[18] = L"coif1";
	m_pNameWavelet[19] = L"coif2";
	m_pNameWavelet[20] = L"coif3";
	m_pNameWavelet[21] = L"coif4";
	m_pNameWavelet[22] = L"coif5";
}
//------------------------------------------------------------------------------
CCustomFDWT::~CCustomFDWT()
{
	delete [] m_pNameWavelet;
	m_pNameWavelet = nullptr;
}
//------------------------------------------------------------------------------
long CCustomFDWT::Create(
	TypeWavelet typeWavelet,	// ��� ��������
	int level,					// ����� ������ ���������� (�� ����� 1)
	bool needInverse,			// ��������� �������� ��������������
	int lengthSignal)			// ���-�� ����� � �������
{
	FbNeedInverse = needInverse;
	
	FLengthSignalClient = lengthSignal;
	long ret = _setTypeWavelet(typeWavelet);
	FLevel = min( max(level, 1), FLevelMaxAll );
	if (lengthSignal != INFINITE)
	{
		_setLengthSignal(lengthSignal);
		_setLevelMaxCur();
		if ( FLevel > FLevelMaxCur )
		{
			FLevel = FLevelMaxCur;
			_setLengthSignal(lengthSignal);
		}
		SetpSize();
	}
	else
	{		
		ippsZero_32s(m_pSize, 32);
		FLevelMaxCur = FLevelMaxAll;
	}
	if ( FLevel != level )
		ret |= wtrvNewLevel;
	if ( FLengthSignal != lengthSignal )
		ret |= wtrvNewLengthSignal;
	return ret;
}
//------------------------------------------------------------------------------
//	��������� �������
//------------------------------------------------------------------------------
long CCustomFDWT::_setTypeWavelet(TypeWavelet val)
{// ������� ������ �������� ���� ������������ �������� �, ��� ���������,
// ����� ����� �������
	FTypeWavelet = val;
	switch (val)
	{// ������� ����� - ��������� �� ��������� ��. default
	case Haar : FLengthFilter = FilterLen_Haar;
		ippsCopy_32f(pTaps_fwdLo_Haar, pTaps_fwdLo, FLengthFilter);
		ippsCopy_32f(pTaps_fwdHi_Haar, pTaps_fwdHi, FLengthFilter);
		ippsCopy_32f(pTaps_invLo_Haar, pTaps_invLo, FLengthFilter);
		ippsCopy_32f(pTaps_invHi_Haar, pTaps_invHi, FLengthFilter);
		break;
	// �������� ������ 1 - 10
	case db1 : FLengthFilter = FilterLen_db1;
		ippsCopy_32f(pTaps_fwdLo_db1, pTaps_fwdLo, FLengthFilter);
		break;
	case db2 : FLengthFilter = FilterLen_db2;
		ippsCopy_32f(pTaps_fwdLo_db2, pTaps_fwdLo, FLengthFilter);
		break;
	case db3 : FLengthFilter = FilterLen_db3;
		ippsCopy_32f(pTaps_fwdLo_db3, pTaps_fwdLo, FLengthFilter);
		break;
	case db4 : FLengthFilter = FilterLen_db4;
		ippsCopy_32f(pTaps_fwdLo_db4, pTaps_fwdLo, FLengthFilter);
		break;
	case db5 : FLengthFilter = FilterLen_db5;
		ippsCopy_32f(pTaps_fwdLo_db5, pTaps_fwdLo, FLengthFilter);
		break;
	case db6 : FLengthFilter = FilterLen_db6;
		ippsCopy_32f(pTaps_fwdLo_db6, pTaps_fwdLo, FLengthFilter);
		break;
	case db7 : FLengthFilter = FilterLen_db7;
		ippsCopy_32f(pTaps_fwdLo_db7, pTaps_fwdLo, FLengthFilter);
		break;
	case db8 : FLengthFilter = FilterLen_db8;
		ippsCopy_32f(pTaps_fwdLo_db8, pTaps_fwdLo, FLengthFilter);
		break;
	case db9 : FLengthFilter = FilterLen_db9;
		ippsCopy_32f(pTaps_fwdLo_db9, pTaps_fwdLo, FLengthFilter);
		break;
	case db10 : FLengthFilter = FilterLen_db10;
		ippsCopy_32f(pTaps_fwdLo_db10, pTaps_fwdLo, FLengthFilter);
		break;
	// �������� (�������� ) 1 - 7
	case sym1 : FLengthFilter = FilterLen_sym1;
		ippsCopy_32f(pTaps_fwdLo_sym1, pTaps_fwdLo, FLengthFilter);
		break;
	case sym2 : FLengthFilter = FilterLen_sym2;
		ippsCopy_32f(pTaps_fwdLo_sym2, pTaps_fwdLo, FLengthFilter);
		break;
	case sym3 : FLengthFilter = FilterLen_sym3;
		ippsCopy_32f(pTaps_fwdLo_sym3, pTaps_fwdLo, FLengthFilter);
		break;
	case sym4 : FLengthFilter = FilterLen_sym4;
		ippsCopy_32f(pTaps_fwdLo_sym4, pTaps_fwdLo, FLengthFilter);
		break;
	case sym5 : FLengthFilter = FilterLen_sym5;
		ippsCopy_32f(pTaps_fwdLo_sym5, pTaps_fwdLo, FLengthFilter);
		break;
	case sym6 : FLengthFilter = FilterLen_sym6;
		ippsCopy_32f(pTaps_fwdLo_sym6, pTaps_fwdLo, FLengthFilter);
		break;
	case sym7 : FLengthFilter = FilterLen_sym7;
		ippsCopy_32f(pTaps_fwdLo_sym7, pTaps_fwdLo, FLengthFilter);
		break;
	// �������� (�������� �������) 1 - 5
	case coif1 : FLengthFilter = FilterLen_coif1;
		ippsCopy_32f(pTaps_fwdLo_coif1, pTaps_fwdLo, FLengthFilter);
		break;
	case coif2 : FLengthFilter = FilterLen_coif2;
		ippsCopy_32f(pTaps_fwdLo_coif2, pTaps_fwdLo, FLengthFilter);
		break;
	case coif3 : FLengthFilter = FilterLen_coif3;
		ippsCopy_32f(pTaps_fwdLo_coif3, pTaps_fwdLo, FLengthFilter);
		break;
	case coif4 : FLengthFilter = FilterLen_coif4;
		ippsCopy_32f(pTaps_fwdLo_coif4, pTaps_fwdLo, FLengthFilter);
		break;
	case coif5 : FLengthFilter = FilterLen_coif5;
		ippsCopy_32f(pTaps_fwdLo_coif5, pTaps_fwdLo, FLengthFilter);
		break;
		
	default : FLengthFilter = FilterLen_Haar;	// ����
		ippsCopy_32f(pTaps_fwdLo_Haar, pTaps_fwdLo, FLengthFilter);
		ippsCopy_32f(pTaps_fwdHi_Haar, pTaps_fwdHi, FLengthFilter);
		ippsCopy_32f(pTaps_invLo_Haar, pTaps_invLo, FLengthFilter);
		ippsCopy_32f(pTaps_invHi_Haar, pTaps_invHi, FLengthFilter);
		FTypeWavelet = Haar;	// �����������
	}
	if (FTypeWavelet > 0)	// �� ��� �����
	{// ��������� ������������ �������� ��� ������� ��������
		int i, n = 1;
		int m = FLengthFilter - 1;
		for(i=0; i<FLengthFilter; ++i)
		{
			pTaps_fwdHi[m--] = n * pTaps_fwdLo[i];
			n *= -1;
		}
		m = FLengthFilter - 1;
		for(i=0; i<FLengthFilter; ++i)
		{
			pTaps_invLo[i] = pTaps_fwdLo[m];
			pTaps_invHi[i] = pTaps_fwdHi[m--];
		}
	}
	long ret;
	if (FTypeWavelet == val)
		ret = wtrvNoError;
	else
		ret = wtrvNewTypeWavelet;
	return ret;
}
//------------------------------------------------------------------------------
void CCustomFDWT::_setLengthSignal(int val)
{// ������� ����� ����� �������
// ����� ����� ������� ������ ������������ ��������, �������� 2^Level,
//	�� �������� (��� �������) val
	int len = 1 << FLevel;
	FLengthSignal = (val / len) * len;
	if ( FLengthSignal != val )
		FLengthSignal += len;
}
//------------------------------------------------------------------------------
void CCustomFDWT::_setLevelMaxCur()
{// ������ ����������� ���������� �������� �������� ������������� ������
// �������� ������ - ����� �������
	FLevelMaxCur = 0;
	int len = FLengthSignal;
	while (len > 0)
	{
		FLevelMaxCur++;
		len = len >> 1;
	}
}
//------------------------------------------------------------------------------
void CCustomFDWT::_setLevel(int val)
{
	FLevel = min( max(val, 1), min(FLevelMaxCur, FLevelMaxAll) );
}
//------------------------------------------------------------------------------
long CCustomFDWT::SetTypeWavelet(TypeWavelet val)
{//	������� �������� TypeWavelet.
//	��������������� FTypeWavelet � FLengthFilter.
	long ret;
	if ( FTypeWavelet != val )
	{// ������� ���������� ����� ��������, ����� �������� �������� � �����������
		ret = _setTypeWavelet(val);
		NewTypeWavelet();
	}
	else
		ret = 1;
	return ret;
}
//------------------------------------------------------------------------------
void CCustomFDWT::SetLevelMaxCur()
{//FLengthSignal ��� ������ ���� ������
	int oldVal = FLevelMaxCur;
	if (FLengthSignal != INFINITE)
		_setLevelMaxCur();
	else
		FLevelMaxCur = FLevelMaxAll;
	// ���� �������� ���������� --------------------------------
	if ( FLevelMaxCur != oldVal )
	{
		if (FLevel > FLevelMaxCur)
			SetLevel(FLevelMaxCur);
	}
}
//------------------------------------------------------------------------------
void CCustomFDWT::SetLevelMaxAll(int val)
{
	if (val < 1)
		FLevelMaxAll = 1;
	else if (val < 32)
		FLevelMaxAll = val;
	else
		FLevelMaxAll = 31;
}
//------------------------------------------------------------------------------
long CCustomFDWT::SetLengthSignal(int val, bool bNewLevel)
{//		������� �������� �������� LengthSignal (���-�� ����� � �������).
//		��������������� FLengthSignal � FLevelMax.
	FLengthSignalClient = val;
	int newVal, len = 1 << FLevel;
	// �������� �������� ------------------------------------
	if (val != INFINITE)
		newVal = max(len, val);
	else
		newVal = INFINITE;
	// ����� �������� ---------------------------------------
	FLengthSignalOld = FLengthSignal;	// ������ ��������

	if ( (FLengthSignal != newVal) || bNewLevel )
	{// ����� �������� FLengthSignal
		if (newVal != INFINITE)
			_setLengthSignal(newVal);
		else
			FLengthSignal = INFINITE;
	}
	// ���� �������� ���������� -----------------------------
	if ( FLengthSignal != FLengthSignalOld )
	{// ����� ���������: ������. ������� � ������ ��������
		SetLevelMaxCur();
		SetpSize();
	}
	// � ��� ������ ������ ����� ����������� ������� ��� ����������
	// �������� ������� � �������-�����������
	if (newVal != INFINITE)	// ��� INFINITE ������ �� ����
	if ( (FLengthSignal != FLengthSignalOld) || bNewLevel )
	// ���� ����� ����� ��� ����� �������
		NewLengthSignal();

	// ������������ �������� --------------------------------
	long ret;
	if ( FLengthSignal != val )
		ret = wtrvNoError;
	else
		ret = wtrvNewLengthSignal;
	return ret;
}
//------------------------------------------------------------------------------
long CCustomFDWT::SetLevel(int val)
{//	������� �������� �������� Level (������� ����������)
// ����� ���������� FLevelMax
	// ������������ �������� - �������� ��������:
	// 1 - �����������, 0 - ����������� ������ ��������
	// �������� �������� ---------------------------
	//int newVal = max(1, val);
	// ����� �������� ------------------------------
	FLevelOld = FLevel;
	_setLevel(val);	//	FLevel = min( max(1, val), FLevelMax );
	// ���� �������� ���������� ----------------
	if ( FLevel != FLevelOld )
	{
		if (FLevel > FLevelOld)
			SetLengthSignal(FLengthSignalClient, true);
		SetpSize();	// ����� SetLengthSignal
		// � ��� ������ ������ ����� ����������� ������� ��� ����������	
		NewLevel();	// �������� ������� � �������-�����������
	}
	// ������������ �������� ------------------------
	long ret;
	if ( FLevel != val )
		ret = wtrvNoError;
	else
		ret = wtrvNewLevel;
	return ret;
}
//------------------------------------------------------------------------------
void CCustomFDWT::SetpSize()
{// ������ m_pSize �������� ������� �������� di.
// m_pSize[0] - ������ ��������� �������
	int size;
	ippsZero_32s(m_pSize, 32);
	if (FLengthSignal != INFINITE)
	{
		size = FLengthSignal;
		for(int i=0; i<=FLevel; ++i)
		{
			m_pSize[i] = size;
			size /= 2;
		}
	}
}
//------------------------------------------------------------------------------
CString CCustomFDWT::NameWavelet(int index)
{
	CString ret;
	if ( ( index >= 0 ) && ( index < FQuantityWavelet ) )
		ret = m_pNameWavelet[index];
	else
		ret.Empty();
	return ret;
}
//------------------------------------------------------------------------------