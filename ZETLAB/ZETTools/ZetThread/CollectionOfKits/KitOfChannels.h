//------------------------------------------------------------------------------
//	���� KitOfChannels.h
//	����������, ��� "����", �������� �.�.
//------------------------------------------------------------------------------
//		����� ������������ ��� ������ � ��������� ������
// ��������� ���������� �������.
//		����� �������� ����� ��������� ������������ ���-�� �������. ������
// ������� ��������� ����������� ���������� ������ ������ � ������� �����
// ���������. ����� ������ ������ � ������ ��������� ���.
//		��������� ����� ��������� ������������ ���-�� ����������.
//------------------------------------------------------------------------------
#pragma once

#include "ChannelForKit.h"
#include <vector>
#include "DataTimeZT.h"
#include <ZETView_basic_class\CDSRV.h>
//------------------------------------------------------------------------------
// ����� ��������� �������. ����� ������ 32, �� ��� ������������ 4.

#define Flag_Kit_Channes_IsOk	0x1		// 1 - � ��������� ���� ������� ������
#define Flag_Kit_Channes_AllOk	0x2		// 2 - � ��������� ��� ������ �������
#define Flag_Kit_Read 			0x4		// 3 - ���������� ������ ������
#define Flag_Kit_Change_Status	0x8		// 4 - ��������� ������ ������- �� ������
// #define Flag_Kit_			0x10	// 5 - 
// #define Flag_Kit_			0x20	// 6 - 
// #define Flag_Kit_			0x40	// 7 - 
// #define Flag_Kit_			0x80	// 8 - 
// #define Flag_Kit_			0x100	// 9 - 
// #define Flag_Kit_			0x200	// 10 - 
// #define Flag_Kit_			0x400	// 11 - 
//------------------------------------------------------------------------------
#define Factor_Chan_Stop_Time	10.		// ����� ���� double
//------------------------------------------------------------------------------
// ��������� ���������� ��������� �������.
struct STRUCT_KIT_OF_CHANNEL
{ 
	Flags<uint32_t> m_bFlags;	// 32 ����� ��������� �������
	HANDLE m_hEventSynchro;		// ���������������� ������� � ����������� � � ���������� (���������) ����������

	DWORD m_numberChannels;     // ���-�� ������� � ���������� (������ ������� �������)
	DWORD m_numberChannelsWork;	// ���-�� ������� ������� � ����������
	DWORD m_numberReadOk;		// ���-�� ������� � �������� �������
	DWORD dw1;

	std::vector<CHANNEL_FOR_KIT> m_vectChannels;
	CZetTime m_zt_lastTimeReadComp;		// ��������� ����� ������ ������ �� ����������
	CZetTimeSpan m_zts_timeDataCur;		// ����� ������� ����������� ������
	CDataTimeZT m_cTimeRead;			// ����� ������
	CRITICAL_SECTION m_crSectinKit;		// ������ � ����������

	STRUCT_KIT_OF_CHANNEL();
	~STRUCT_KIT_OF_CHANNEL();
	STRUCT_KIT_OF_CHANNEL(const STRUCT_KIT_OF_CHANNEL & val);
	STRUCT_KIT_OF_CHANNEL & operator = (const STRUCT_KIT_OF_CHANNEL & val);

	// ������ �����
	bool GetFlag(uint32_t flag);

	// ��������� ����� ��������� ���������
	void SetFlag(uint32_t flag, bool val);

	// ��������� ����� � 1
	void PlaceFlag(uint32_t flag);

	// ����� ����� � 0
	void ResetFlag(uint32_t flag);

	// ��������� ���-�� ������� ������� � ���������
	DWORD GetNumChannelsWork();

	// ������� ������� ������� ��� ������ ������ � ������������ � ����������
	// ������� �� CDataTimeZT. ����������� �������� �� min � max ��������.
	// ������������ �������� - ����� ������� (� ������ ��������) �����������.
	bool ReSize();

	// ��������� ������������ SRV-������� �� ��������� �
	// �������� �������� ������� �� �������
	double GetTimeMin(CDSRV * psrv, const bool bNeed7000);

	// ������� ������� ������� ������
	bool SetTimeStart(CDSRV * psrv, const bool bNeed7000);
	bool SetTimeStart(const double time);

	// �������� ���������� ����������� ������ ������ �
	// ��������� �������� ������� �� �������
	bool ReadinessData(CDSRV * psrv, const bool bNeed7000);

	// ���������� ������ ������ � ������� ���������.
    // ���� ���� ������, �� � m_zts_timeDataCur ������������ �����
    // ����������� ������ � ���������������� ����� ������
	// ���������� ���� - ������ ����/�� ����.
	bool GetData(CDSRV * psrv, const bool bNeed7000);

	// �������� ���������� ������� �� ������� �� ���������.
	// ���� ���-�� ����������, �� ����������� true.
	bool Modify(CDSRV * psrv, CHANNEL_STRUCT & tmp, const double FriqMin);

	// ��������� ����������������� ������� ����������
	void SetSynchro();

	// ����������� ������ ����������
	void DefineFlagsOfKit();
	
	// ��������� ���������� ������� ����������
	void SetTimeParam(const CZetTimeSpan & timeInternalRead, const CZetTimeSpan & timeInternalBuffer);
    
	// ����� ������ Flag_Change_Freq � ���� ������� ����������
	void ResetAllFlagChangeFreq();

	// ������� ������ Flag_Change_Freq � ���� ������� ����������
	void SetAllFlagChangeFreq(bool val);

	// �����, ���������� �� ���������� ����������, ���
	double GetTimeBeforeNextAver(CDSRV * psrv, const bool bNeed7000);
	// �����, ���������� �� ���������� ������, ���
	double GetTimeBeforeNextRead(CDSRV * psrv, const bool bNeed7000);
};
//------------------------------------------------------------------------------