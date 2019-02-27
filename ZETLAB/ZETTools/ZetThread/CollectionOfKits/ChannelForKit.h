//------------------------------------------------------------------------------
//	���� ChannelForKit.h
//------------------------------------------------------------------------------
//		����� ������������ ��� ���������� ������ � �� ��������� � �����������
// ������� �������. �.�. �������, �������� �� ������ ���������,
// � ������� ��������� �������������, ������ ���-��� ������ � �.�.
//		���������, ��� ������ ������� �� ������� ���������� ���������!!!
//		��������� ���������� ������ � ������� ������ ����� ���� �����.
//------------------------------------------------------------------------------
#pragma once
#include "afx.h" 
#include <limits>
#include <chaninfo.h>
#include <Flags.h>
#include <Buffer\SmartArray.h>
#include <ZetTools\include\ZetTools.h>
//------------------------------------------------------------------------------
#ifndef WhileCountMax
#define WhileCountMax		100
#endif

#ifndef MaxDeltaTimeInSec
#define MaxDeltaTimeInSec	(60.)
#endif
//------------------------------------------------------------------------------
// ����� ������. ����� ������ 32, �� ��� ������������ 13.

#define Flag_Chan_OnOff			0x1		// 1 - ������ � �������
#define Flag_Chan_Exist			0x2		// 2 - ����� ����� � ������� ����
#define Flag_Chan_7000			0x4		// 3 - ����� � ��������� freqADC
#define Flag_Chan_Disabled		0x8		// 4 - ����� ��������

#define Flag_Change_Chan		0x10	// 5 - ������ ����� (ID ������)
#define Flag_Change_Status		0x20	// 6 - ��������� ������ ������

#define Flag_Change_Freq		0x40	// 7 - ���. �������
#define Flag_Change_Level		0x80	// 8 - ���. ����������� ��������
#define Flag_Change_Level_DC	0x100	// 9 - ���. ����. ������������
#define Flag_Change_Level_dB	0x200	// 10 - ���. ������� ����. ��� dB
#define Flag_Change_Name		0x400	// 11 - ���. ��� ������
#define Flag_Change_Measure		0x800	// 12 - ���. ����������� ������

#define Flag_Chan_Stop_Time		0x1000	// 13 - ������������ ����� �� ������
//------------------------------------------------------------------------------
// ��������� ���������� ������ �������, ��������������� ��� ������ � �������
// ��������� �������. �������� ���������, ����������� ��� ������ ������.
// ��������� CHANNEL_STRUCT.
//
struct CHANNEL_FOR_KIT : public CHANNEL_STRUCT
{
	long channel;				// ����� ������
	long m_decade;				// ������

	DWORD m_numKadrMin;			// ����������� ������ �������
	DWORD m_numKadrMax;			// ������������ ������ �������

	long retGetData;			// ���, ������������ �������� GetData
	Flags<uint32_t> bFlags;		// 32 ����� ������

	double m_lastTimeRead;		// ��������� ����� ������ ������ �� �������
	SmartArray<float> saArray;	// ������ ��� ������ ������

	CRITICAL_SECTION m_crSectionChan;	// ������ � ������

	CHANNEL_FOR_KIT();
	~CHANNEL_FOR_KIT();
	CHANNEL_FOR_KIT(const CHANNEL_FOR_KIT & val);
	CHANNEL_FOR_KIT & operator = (const CHANNEL_FOR_KIT & val);
	CHANNEL_FOR_KIT & operator = (const CHANNEL_STRUCT & val);
	
	// ������� ������� ������ ������ �� ��������� �������.
	// ����������� �������� �� min � max ��������.
	bool ReSize(const DWORD sizeBuffer);
	bool ReSize(const double dIntervalBuffer);
	bool ReSize(const CZetTimeSpan & ztsIntervalBuffer);

	// �������������� ��������� ������ ����� ������� ������
	void InitFlags();

	// ������ �����
	bool GetFlag(uint32_t flag);

	// ��������� ����� ��������� ���������
	void SetFlag(uint32_t flag, bool val);

	// ��������� ����� � 1
	void PlaceFlag(uint32_t flag);

	// ����� ����� � 0
	void ResetFlag(uint32_t flag);
	
	// �������� �������� ����� ��������� � ��������� ��������������� ������
	// ���������� true ���� ��������� ������
	bool TestChangeStruct(CHANNEL_STRUCT & pstr);

	// ���� ���������� ���� ��������� �������, �� ���������������� ����
	// ������������� ������ � ���� ��������� ������� ������������
	void DoChangeStatus();

	// ������ ������� ������ �� ������� ����������
	// true ���� ����� ��������
	bool IsDisabled();

	// ������ ������� ������ �� ������� ����������������� ������
	bool ChannelIsOk();
};
//------------------------------------------------------------------------------