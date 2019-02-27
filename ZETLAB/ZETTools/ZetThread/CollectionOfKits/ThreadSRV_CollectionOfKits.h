//------------------------------------------------------------------------------
#pragma once

 #define MinSumTimeReaddForOneKit	1.		// � ���
 #define MaxSumTimeReaddForOneKit	100.	// � ���
// --------------------------------------------------------------------------------
/*	���� ThreadSRV_CollectionOfKits.h
����������, ��� "����", �������� �.�.
--------------------------------------------------------------------------------
	��������� ����� CThreadSRV_CollectionOfKits ������������ ��� ����������
 ����������� ������ ������ � ���������� ������� � ������ ������� �� ���������.
 --------------------------------------------------------------------------------
	������ �������, ��������������� ��� ���������� ��������� ������, ������������
� ��������� �������. ���-�� ������� � ��������� ������������. � ������ ���������
��� �����, ���� �������� ������ � ���� ��������� �������� ������. ������ ��������
�� ������ ���-���� ����� � ������ ����������. ������� ������������� ������� �
��������� �� ������� ���� ����������.
	����� �������� � ���������� ����������. ��������� ������ ����.
���-�� ���������� � ��������� ������������.
	��������� ���������� ������� ����������� � ������� std::vector.
 --------------------------------------------------------------------------------
	������������� ������� ������������� ������:
- ���������� ����������� ��������;
- ����� NumFileUsed;
- ������ �� ��������� �������;
- ������� ���������� ������������� ������� ���:
  - ��������� ������� �������;
  - ����������� ����������� ��������� ���������� ������;
- ��� ���������� � ��������� ��������� ����������� ��������� � ���������;
- ��� ��������� ������� ������� � ��������� ����������� ��������� � ���� ���������;
- ��� ��������� ������ � ��������� ����������� ��������� � ����� ������;
- �������������� ������� ������� ������ ������ �������.
--------------------------------------------------------------------------------
	����� ��������� ������������� ��� ���������� � ����������� � ����� ���������
�����-�� ����������, ��������: �������� ��� ����������� � ���� �������� �/���
����� ��� ����������� � Edit � Static. ��� ������������� � ��������� ����������
������� ������ ���������� ��� �������� �������:
	1. ������ ���� ���������� ���������, ���������� ������������ ���������.
��������: STRUCT_CALC_AND_VIEW. ����� ������������ ���������� ��������� �����
��������� ��������������� �������, ����������� ��� ������� ���� ����������,
������� ������������� ������� � ����������� ����� ��������� ����������.
	2. ������ ���� ������� ������� �������� ������������ ���������� �� ������,
��������� � SRV. ��������: Calculation. �������� �������� ���� ������� -
��������� �� ��������� ��������� STRUCT_CALC_AND_VIEW. �.�. ��� ������� �����
����������� � ������� ������ ���������, �� ��� ������ ���� ����������.
	3. �� �������� ������ � ���� ���������� ��������� ����� ���������� ���������
WM_NEW_DATA_SRV. ��� ������� � ���, ��� ���������� ��������� ���������. ��������
WPARAM �������-����������� ����� ��������� ����� ��������� ������, �� ��������
������� ���������� ��������� ��������� STRUCT_CALC_AND_VIEW, ������� ��������
����� ��������� ��� ����������� � ���������� ���������.
--------------------------------------------------------------------------------
	���������� ������ ������ � �� ��������� ����������� � ������� ������. �������
����� ������ - ��� ��������� ���������� �������, ������� �� ����� ������������ ������.
� ���� ������� ����������� ��� ���������� ������������� �������� (��. ����). �����
������ ���������� ������ ���������� ������� ��������� ����������� �������, �.�.
������� Calculation, � ������� ��������� ��������� �� ��������������� ���������
STRUCT_CALC_AND_VIEW.
	����� � ��������� ������� Calculation ���������� � ����� ��� ��� �������� ���
��������� ������� Create. �������� ��������� �������:
typedef long (*FunctionThread)(void *);
���������� ��������� ������� ������ ��������� ��������� CodeQuality, � ������� ����
�������� �������������� � long. ���� ���� AllData ���� ��������� �����
Quality_AllData_True, �� ������� ����� �������� � ����� GUI ���������
WM_NEW_DATA_SRV, ��� ������� � ������� ����� ������ ���������� ������.
��������� ������ ������ ����� �������� �������� ������ ����� ����������� ����
������ � ������ GUI ��� �� ����������� � ����������. ��� ����� ������������
������� ������ SetSynchro(indxKit).
	���� ����� ����������, �� ������ �� ����� �������� NumFileUsed, �, ���
���������, �������� �������. ������� ��� ���������� ����������� ������ ������
����� ������������ �������� EnableReadData.
--------------------------------------------------------------------------------
	�� �������� � �������� �������� ������:
virtual long End();				// ����������� ������� ������ "�����"
virtual long EndAndWait();		// ���������� ������� ������ "�����"
long Start();					// ������ ������
virtual long Stop();			// ����������� ������� ������ "����"
virtual long StopAndWait();		// ���������� ������� ������ "����"
--------------------------------------------------------------------------------
	������ ������������� ������ CThreadSRV_CollectionOfKits ��. � ��������� .

	���������� �������� �������� ������������� ������.

1. ��������� ��������� ������ STRUCT_CALC_AND_VIEW ��� ��������� ���������
���������� STRUCT_KIT_OF_CHANNEL. � �� �������� ������������ ��������� �
������������� ������ ��� �� �������. � ���� ��������� ������ ����:
   - ����������� �� ���������;
   - ����������;
   - ����������� �����������;
   - ��������������� �������� ������������.

2. ��������� ������� ������� �������� ������������ ����������:
	long Calculation(PVOID pParam).
����� pParam � ������� ��������� ��������� �� ��������� STRUCT_CALC_AND_VIEW,
�.�. �� ��������� � ������� ��������� ���������� ������ ������ � �������.
����������� ������ ���������� � ��������������� �������� ������������ ���������
STRUCT_KIT_OF_CHANNEL. � ������� ����������� ����������� ������� � ���� ������
������������ ���������, �� ������� ���������� ����������� ���, �� �������� ��
��������� ���������� ������ � ��������� ���������� ��������� WM_NEW_DATA_SRV.

3. ���������� ������� WM_NEW_DATA_SRV ������������ ��� ����������� � ���������
����������� ���������� ��������� ������ ���������.
	� WPARAM ���� ������� ������ ��������� (indxKit), � ������� ���������
�������� ���������� ������. LPARAM  - ������������ �������� Calculation.
   � ���� �����������:
   - ��������� ��������� m_pThread �� nullptr;
   - �� indxKit ���������� ��������� STRUCT_CALC_AND_VIEW;
   - �� ���� ��������� ����-�� �������� �������� ������������ ����������;
   - ��������� ������ ������ � ������ ����������, ������ �������
     CThreadSRV_CollectionOfKits::SetSynchro(indxKit);
   - ���������� � ���������� ��������� ������������� ����������.

   ������ ��������.

1. ��������� ��������� Param2 ��� ���������� ������� Calculation � ������
��������� ���� ���������. � ��� ��������� ������� ��������� ���������, �����
��� ���� �����������.

2. ��������� ��������� ������, �������� ���
   CThreadSRV_CollectionOfKits<STRUCT_CALC_AND_VIEW> * m_pThread = nullptr;

6. ������ ��������� ������ (�.�. ���������������� �����), �������� ���:
   m_pThread = new CThreadSRV_CollectionOfKits<STRUCT_CALC_AND_VIEW>();
   m_pThread->Create(L"", m_hWnd, m_psrv, Calc, &Param);
��� ���� ����� ������� ��������� �� ������ ����������, � ������� �����
���� ����� � ������� "-1".

7. ��� ��������� ���-�� ����������� ���������� ������� ������
SetNumberKits, AddKitToCollection � DeleteLastKit.

8. ��� ��������� ������� ������� � ����������� ���������� ������� ������:
ChangeAllChannelsInKitCur � ChangeAllChannelsInKit.

9. ��� ������ ������ ������ � ���������� ���������� ������� ������:
ChangeOneChannelInKitCur � ChangeOneChannelInKit.

10. � ��������� ����� ��������� ���������� ��������� WM_END_JOB_SRV, � �������
��������� ������ ���������. � CDialog_ZET ��������� ����� ��������� ��� ����.
*/
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <CodeQuality.h>				// ��� �������� ������
#include "KitOfChannels.h"				// ��������� ��������� �������
#include <ZetThread\CustomThreadSRV.h>	// ������� �����
//------------------------------------------------------------------------------
// ���������, ������������ �� ��������� ���������� ������� ������ ������.
//
template <class T>	// T - ��� ���������, ����������� �� STRUCT_KIT_OF_CHANNEL
struct ZetThreadParam2
{
	int m_indxKitRead;			// ������ ���������, � ������� ��������� ���������� ������
	double * pTimeReadSumMax;	// ������������ ����� ������ ������ ������ ����������
	T * m_pKitRead;				// ��������� ����� ���������
	CDSRV * m_psrv;				// ��������� �� ������
	CZetTime * m_pTimeStartSRV;		// ����� ������ ������ �������
	PVOID m_pParamFromDlg;			// ��������� �� Dlg

	ZetThreadParam2()
		: m_indxKitRead(0)
		, pTimeReadSumMax(nullptr)
		, m_pKitRead(nullptr)
		, m_psrv(nullptr)
		, m_pTimeStartSRV(nullptr)
		, m_pParamFromDlg(nullptr)
	{}

	~ZetThreadParam2() {}

	// ��������� ����������� ������� ������ ������ ���������
	CZetTime GetAbsoluteTimeSRV()
	{
		return *m_pTimeStartSRV + m_pKitRead->m_zts_timeDataCur;
	}
};
//------------------------------------------------------------------------------
// ��������� ������ CThreadSRV_CollectionOfKits. ��� �� ���������� �
// ��������� ���������� ������� ������ ������.
//
template <class T>	// T - ��� ���������, ����������� �� STRUCT_KIT_OF_CHANNEL
struct ZetThreadParam1
{// ��������� ���������� ������,
// ������������ � ��������� ���������� �������
	// ���������� ��������� � ��������� �������� �������������
	bool m_bSupport7000;
	// ��� ������ ���� ���������� ������ ���� ��������
	bool m_bChanAllOk;
	// ���������� �� ������ ������
	bool m_bReadData;
	// ��������� ��������� ���������� �������
	ZetThreadParam2<T> * m_pParam2;
	// ������ � ���������
	CRITICAL_SECTION m_crSectinCollec;
	// ������ ����������
	std::vector<T> m_vectKits;
	// ������� ���������� ������
	FunctionThread m_FunctionAver;

	ZetThreadParam1()
		: m_bSupport7000(true)
		, m_bChanAllOk(false)
		, m_bReadData(true)
		, m_pParam2(nullptr)
		, m_FunctionAver(nullptr)
	{
		InitializeCriticalSection(&m_crSectinCollec);
	}

	~ZetThreadParam1()
	{
		DeleteCriticalSection(&m_crSectinCollec);
	}
};
//------------------------------------------------------------------------------
template <class T>	// T - ��� ���������, ����������� �� STRUCT_KIT_OF_CHANNEL
class CThreadSRV_CollectionOfKits : public CCustomThreadSRV
{
private:
	// ����������� ������� ������������� ����������� ������
	DWORD m_indxKitCur;				// ������ �������� ���������
	DWORD m_numberKits;				// ���-�� ����������� � ���������
	DWORD m_numberKitsMax;			// ������������ ���-�� ����������� � ���������
	T * m_pKitCur;					// ��������� �������� ���������
	double m_freqADCMin;			// �������� ����������� ������� (50 ��)
	double m_timeReadSumMax;
	CZetTime m_zt_StartSRV;			// ���������� ����� ������ �������
	ZetThreadParam1<T> m_parSRV1;	// ���������, ������������ � ��������� �������
	ZetThreadParam2<T> m_parSRV2;	// ���������, ������������ �� ��������� �������
	CHANNEL_FOR_KIT m_chanTmp;		// TMP-������ ��� CDSRV::GetChannelParEx

	// �������������� ������� ���������� ������ �� ��� ������
	bool SetFirstChanByNumber(CHANNEL_FOR_KIT * pchFK, long channel);
	// �������� ���������� ���������� ��������� ���������� �������
	void WaitEndingFuncSecond();
	// ��������� � ���� ����������� ����������������� �������
	void SetSynchroForAllKits();

protected:

public:
	// 
	CThreadSRV_CollectionOfKits();
	virtual ~CThreadSRV_CollectionOfKits();

	// �������� ���������� ������.
	// nameThread - ��� ���������� (����� L"");
	// hwndParent - ���������� ���� ��� ������� WM_NEW_DATA_SRV � WM_END_JOB_SRV;
	// psrv - ��������� �� ������;
	// function - ��� ��������� ���������� ������� ��� ���������� ��������� ������;
	// paramFunction - �������� ��������� ���������� ������� m_pParamFromDlg;
	// ����� ������� ���� ������� ���������� ��������� psrv->Connect().
	virtual bool Create(
		CString nameThread,
		HWND hwndParent,
		CDSRV * psrv,
		FunctionThread function,
		PVOID paramFunction,
		FunctionThread functionAver = nullptr);

	// �������� ������ --------------------------------------------------------

	// ��������� ������������� ���-�� �����������
	DWORD GetNumberKitsMax();
	// ������� ������������� ���-�� �����������
	void SetNumberKitsMax(const DWORD val);
	//
	// ��������� �������� ����������-��������� ������� ������������� ������ ���
	// ������� ������ � ���������� ��������
	double GetMinFreqADC();
	// ������� ����������-��������� ������� ������������� ������ ���
	// ������� ������ � ���������� ��������. �� ��������� = 50 ��.
	void SetMinFreqADC(const double freq);

	// ��������� ����� ���������� ������ � ���������� ��������
	bool GetSupport7000();
	// ������� ����� ���������� ������ � ���������� ��������
	void SetSupport7000(const bool val);
	
	// ��������� ������� �������� ���������� �������
	DWORD GetIndexKitCur();
	// ������� �������� ���������� �������.
	// ������������ �������� true ���� index ����������.
	bool SetIndexKitCur(const DWORD indxKit);

	// ��������� ����� ���������� ������ ������ �� ���� �����������
	bool GetEnableReadData();
	// ������� ����� ���������� ������ ������ �� ���� �����������
	void SetEnableReadData(const bool val);

	// ��������� ����� ���������� ������ ������ � ������� ����������
	bool GetEnableReadDataInKitCur();
	// ������� ����� ���������� ������ ������ � ������� ����������
	void SetEnableReadDataInKitCur(const bool val);

	// ��������� ���-�� ����������� � ���������
	DWORD GetNumberKits();
	// ������� ���-�� ����������� � ���������.
	// ���� ����� ���-�� ������ �������� ���-��, �� ����������� ���������� ��
	// ������ ������ � ������� "-1" (������ ���������� �� ����������).
	// ����� � ����� ����� ������� ��������������� ����� ����������� (����������
	// �� ���������). ����� ���-�� ������ ���� ������ 0.
	// ������� ������-���������.
	bool SetNumberKits(const DWORD newNum);

	// ������� ��������� ������� ��������� ----------------------------------------
	//
	// ���������� � ��������� ���������� �������, ������� ����� ������� �����������.
	// ����������� ��������� ������ ��������� ��� ������� ���� �����.
	// numChan - ���-�� ������� � ����������� ����������;
	// chan1 - ������ ����� � ����������� ����������.
	// ������ ������� �������� ���������� ���� long.
	// ����� ������ "-1" ��������.
	// ������� ������-���������.
	// ������� ���������� ������ �������� ������.
	DWORD AddKitToCollection(const DWORD numChan, const long chan1, ...);

	// ���������� � ��������� ���������� �������, ������� ����� ������� �����������.
	// ������ ������� ������������ ���������� ����� ����� �� ������� vectChannels.
	// vectChannels �� ����� ���� ������.
	// ����� ������ "-1" ��������.
	// ������� ������-���������.
	// ������� ���������� ������ �������� ������.
	DWORD AddKitToCollection(const std::vector<long> & vectChannels);

	// �������� �� ��������� ���������� ����������
	// ������� ������-���������.
	void DeleteLastKit();

	// ������� ��������� ������� ������� ������ ���������� ---------------------------
	//
	// ������ ���� ������� � ������� ����������.
	// numChan - ����� ���-�� ������� � ������� ����������;
	// chan1 - ����� ������ ����� � ������� ����������.
	// ������ ������� �������� ���������� ���� long.
	// ����� ������ "-1" ��������.
	// ������� ������-���������.
	void ChangeAllChannelsInKitCur(const DWORD numChan, const long chan1, ...);

	// ������ ���� ������� � ������� ����������.
	// ����� ������ ������� �������� ���������� ����� ����� �� ������� vectChannels.
	// ������� ������-���������.
	void ChangeAllChannelsInKitCur(const std::vector<long> & vectChannels);

	// ������ ���� ������� � �������� ����������.
	// indxKit - ������ ��������� ����������;
	// numChan - ����� ���-�� ������� � �������� ����������;
	// chan1 - ����� ������ ����� � �������� ����������.
	// ������ ������� �������� ���������� ���� long.
	// ����� ������ "-1" ��������.
	// ������������ �������� true ���� indxKit ����������.
	// ������� ������-���������.
	bool ChangeAllChannelsInKit(const DWORD indxKit, const DWORD numChan, const long chan1, ...);
	// ������ ���� ������� � �������� ����������.
	// ����� ������ ������� ��������� ���������� ����� ����� �� ������� vectChannels.
	// vectChannels �� ����� ���� ������.
	// ������������ �������� true ���� indxKit ����������.
	// ������� ������-���������.
	bool ChangeAllChannelsInKit(const DWORD indxKit, const std::vector<long> & vectChannels);

	// ������ ������ ������ � �������� ����������.
	// indxChanInKir - ������ ������ � ������� ����������;
	// chanNew - ����� ������ ������.
	// ����� ������ "-1" ��������.
	// ������������ true ���� indxChanInKir ����������.
	// ������� ������-���������.
	bool ChangeOneChannelInKitCur(const DWORD indxChanInKir, const long chanNew);

	// ������ ������ ������ � �������� ����������.
	// indxKit - ������ ���������� � ���������;
	// indxChanInKir - ������ ������ � ������� ����������;
	// chanNew - ����� ������ ������.
	// ������������ true ���� ��� ������� ����������.
	// ������� ������-���������.
	bool ChangeOneChannelInKit(const DWORD indxKit, const DWORD indxChanInKir, const long chanNew);

	// ������� ��������� ������� � ����������� ---------------------------------------
	//
	// ��������� ��������� �� ��������� ���������� �������� ����������
	// ���� � ��������� ��� ����������� ������������ nullptr
	T * GetPointerOfKitCur();
	// ��������� ��������� �� ��������� ���������� ��������� ����������
	// nullptr ������������, ���� index �� ����������.
	T * GetPointerOfKit(const DWORD indxKit);

	// ������� ��������� ���������� ��������� (�� ����� � ���������)
	void IncrementKitCur();

	// ������������� ��������� � ����������� -----------------------------------------
	//
	// ������������� ��������� � ����������� ���������� ������ indxKit-��� ����������.
	// ������������ �������� true ���� indxKit ����������.
	bool SetSynchro(const DWORD indxKit);

	// ������� �������� ���������� ������ �� ������� �� ��������� -------------------
	//
	// ��������� ������� ������� Modify.
	// �� GUID ������ ����� ���������� ��� ���� ���������. ���� �����-�� ��������
	// ���������, �� ��������� ��������������� ����. ���� ���������� �������
	// �������������, �� ������ ������ �������� ��������������� ReSize.
	// ���� ������ �������������, �� ���������� SRV-����� ������ �����������.
	// ���� ���-�� ���������� � ���������� �������, �� ��������� true.
	bool Modify();

	// �������� ���������� ������� �������� ����������.
	// ���� ���-�� ����������, �� ��������� true.
	bool TestChannelsInKitCur();

	// ������� ������� ������� ������� ������ (� ������� ������� ����������) --------
	//
	// ������� ������� ������ ��� �������� ����������.
	// time - ����� ������.
	void SetTimeStartForKitCur(const double time);
	// ������� ������� ������ ��� �������� ����������.
	// ����� ������ ����� ���������� � ������ ������� ������� ����������.
	void SetTimeStartForKitCur();

	// ������� ������� ������ ��� ��������� ����������.
	// indxKit - ������ ���������� � ���������;
	// time - ����� ������.
	// ������������ �������� true ���� indxKit ����������.
	bool SetTimeStartForKit(const DWORD indxKit, const double time);
	// ������� ������� ������ ��� ��������� ����������.
	// indxKit - ������ ���������� � ���������;
	// ����� ������ ����� ���������� � ������ ������� ������� ����������.
	// ������������ �������� true ���� indxKit ����������.
	bool SetTimeStartForKit(const DWORD indxKit);

	// ������� ������������ ������� ����� ���� �������
	// ���������� false, ���� ���� ���������� ��� ������� �������
	bool SetTimeStart();

	// ������� ������� ��������� ���������� ----------------------------------------
	//
	// ��������� ��������� ���������� ������ � ������ �������� ����������.
	void GetTimeParamForKitCur(
		CZetTimeSpan & timeInternalRead, CZetTimeSpan & timeInternalBuffer);
	// ������� ��������� ���������� ������ � ������ �������� ����������.
	// ��� ��������� ��������� ������ �������������� ����������
	// ������� ������� ������� �������� ����������
	void SetTimeParamForKitCur(
		const CZetTimeSpan & timeInternalRead, const CZetTimeSpan & timeInternalBuffer = CZetTimeSpan());
	
	// ��������� ��������� ���������� ������ � ������ ����������.
	// ������������ �������� true ���� indxKit ����������.
	bool GetTimeParamForKit(const DWORD indxKit,
		CZetTimeSpan & timeInternalRead, CZetTimeSpan & timeInternalBuffer);

	// ������� ��������� ���������� ������ � ������ ����������.
	// ��� ��������� ��������� ������ �������������� ����������
	// ������� ������� ������� ����������
	// ������������ �������� true ���� indxKit ����������.
	bool SetTimeParamForKit(const DWORD indxKit,
		const CZetTimeSpan & timeInternalRead, const CZetTimeSpan & timeInternalBuffer = CZetTimeSpan());

	// ��������� ��������� ������� ���������� �������� ����������
	void GetTimeIntervalAverForKitCur(CZetTimeSpan & timeInternalAver);

	// ������� ��������� ������� ���������� �������� ����������
	void SetTimeIntervalAverForKitCur(const CZetTimeSpan & timeInternalAver);

	// ��������� ��������� ������� ���������� ��������� ����������
	// ������������ �������� true ���� indxKit ����������.
	bool GetTimeIntervalAverForKit(const DWORD indxKit, CZetTimeSpan & timeInternalAver);

	// ������� ��������� ������� ���������� ��������� ����������
	// ������������ �������� true ���� indxKit ����������.
	bool SetTimeIntervalAverForKit(const DWORD indxKit, const CZetTimeSpan & timeInternalAver);

	// ������ ������� ------------------------------------------------------------
	// �������������� �������������� ������� ������� � ����������
	CZetTime ConvertTimeSRVtoAbsolute(const CZetTimeSpan & ztsSRV);

	// ������������ ��������� ����� ����� ����������� ������ ��� ������ ����������
	double GetMaxSummTimeReadForOneKit();
	void SetMaxSummTimeReadForOneKit(const double val);
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//   ���������� ������� SRV_ThreadMulti - ��� ��������� ������� ������.
// Ÿ �������� - ��� ��������� �� ��������� ��������� ����� ����������,
// ������� �������� � ���� ��������� �� ��������� ������� ����������.
//   � ������� ������ ������� ����������� ����� � ������� ����������� �������.
// � ������� ������ ��� �����! �� ��������� ������� ����������� ������ � ���
// ������� ������ ���������� ��������� ���������� �������, ��� ������ �����������
// ��������� ��������� ������ ������. ��� ������� ���������� ��� ��������
// ���������� ����������� ������ �� ������. ���� ��� AllData ����� ���� ����� 0
// (��� ������ ����������), �� ��������� ���������� ����������� ������ � ������
// ��������� ����, � ����� ����� ��������� ��������� WM_NEW_DATA_SRV.
//------------------------------------------------------------------------------
//		��� ���������� ���� �������� ������ �������� ��������� ���������:
//	WM_END_JOB_SRV - ��� ������� ������, ��������� ��� ����������;
//	WM_NEW_DATA_SRV - ������ �� ������ �� ������� ������� � ����������,
//		��������� ����� ������ � ��� �������� ���������� ���������.
//------------------------------------------------------------------------------
template <class T>
DWORD WINAPI Function_CollectionOfKits(PVOID pParam)
{// ��������� ���������� ������� �������� ������

	// ���������� ��������� -----------------------------------------
	// ����� ��������� ���������� ������-����������� CCustomZetThread
	ZetThreadParam0 *par0 = (ZetThreadParam0*)pParam;
	par0->statusEx = running;

	// ����� ��������� ���������� ������-�������� CCustomThreadSRV
	ZetThreadParamSRV0 *parSRV0 = (ZetThreadParamSRV0*)par0->pParamNext;
	auto psrv = parSRV0->pSRV;	// ������

	// ����� ��������� ���������� ������� ������ CThreadSRVmulti
	ZetThreadParam1<T> *parSRV1 = (ZetThreadParam1<T>*)parSRV0->pParamNext;
	// ��������� ��������� ���������� �������
	ZetThreadParam2<T> *parSRV2 = (ZetThreadParam2<T>*)parSRV1->m_pParam2;

	// �������� -------------------------------------------------------
	while (par0->FlagJob)
	{// ���� �������� ������ -------------------------------------------
		par0->FlagViewStop = false;

		// ����� �� ���� ������� � �������� ----------------------------
		par0->statusEx = numFileUse;
		if (psrv->NumFileUsed(0) == -1)
		{	// ������ ����������
			if (par0->hWndParent != NULL)
				::PostMessageW(par0->hWndParent, WM_END_JOB_SRV, 0, 0);
			break;	// ������� �� while (par0->FlagJob)
		}

		par0->statusEx = waitObject;
		bool bFlag = par0->FlagJob && !par0->FlagStop && parSRV1->m_bReadData;

		// ������ �� ��������� -----------------------------------------
		if (bFlag)
		{
			EnterCriticalSection(&parSRV1->m_crSectinCollec);

			int indexKit = 0;
			// ���� �� �����������,it - ��� ��������� -------------------------------
			for (auto & it : parSRV1->m_vectKits)
			{	// ��� ����� � �������� ������ � ��������� ������ ����� ����������
				EnterCriticalSection(&it.m_crSectinKit);
				par0->statusEx = working;

				bFlag = par0->FlagJob && !par0->FlagStop &&
					parSRV1->m_bReadData && it.GetFlag(Flag_Kit_Read);

				if (parSRV1->m_bChanAllOk)
					bFlag &= it.m_numberChannels == it.m_numberChannelsWork;
				//	bFlag &= it.m_numberChannels == it.GetNumChannelsWork();

				if (bFlag)
				{
					par0->statusEx = dataTime;
					BYTE whileCount = 0;
					parSRV2->m_indxKitRead = indexKit;
					parSRV2->m_pKitRead = &it;

					if (parSRV1->m_FunctionAver == nullptr)
					{
						while (it.ReadinessData(psrv, parSRV1->m_bSupport7000))
						{
							if (!par0->FlagJob || par0->FlagStop ||
								!parSRV1->m_bReadData ||
								!it.GetFlag(Flag_Kit_Read))
								break;	// ����� �� while (it.ReadinessData(psrv))

							par0->statusEx = waitSynshroGUI;

							if (it.m_hEventSynchro != NULL)
							{
								bFlag = WAIT_OBJECT_0 ==
									WaitForSingleObject(it.m_hEventSynchro, par0->TimeOutSend);
							}
							if (bFlag)
							{// ���� ������������ ������ ������� ��������� ���� � ���������
								if (!par0->FlagJob || par0->FlagStop ||
									!parSRV1->m_bReadData ||
									!it.GetFlag(Flag_Kit_Read))
									break;	// ����� �� while (it.ReadinessData(psrv))

								par0->statusEx = dataRead;
								// m_zts_timeDataCur
								if (it.GetData(psrv, parSRV1->m_bSupport7000))
								{
									par0->statusEx = dataProcessing;
									// ����� ��������� ���������� �������, ���������
									// ���������� ������
									CodeQuality retFunction = (*par0->Function)(parSRV2);
									par0->statusEx = working;
									if (retFunction.IsAllData() && (par0->hWndParent != NULL))
									{// ������� � ���������� ��������� ������
										::PostMessageW(par0->hWndParent, WM_NEW_DATA_SRV,
											(WPARAM)indexKit, (LPARAM)retFunction);
									}
								}// if (it.GetData(psrv))
								it.ResetAllFlagChangeFreq();
							}// if (wait1 == WAIT_OBJECT_0)
							else// GUI �� ���������
								break;	// ����� �� while (it.ReadinessData(psrv))

							if (((++whileCount) * (double)it.m_cTimeRead.GetIntervalRead()) >
								(*parSRV2->pTimeReadSumMax))
								//		if (++whileCount > WhileCountMax)
								break;	// ����� �� while (it.ReadinessData(psrv))
						}// while (it.ReadinessData(psrv))
					}
					else// if (parSRV1->m_FunctionAver == nullptr)
					{
						double timeCur = it.GetTimeMin(psrv, parSRV1->m_bSupport7000);
						if (timeCur != DBL_MAX)
						{
							bool bRead(true), bAver(true);
							double timeCur0 = timeCur;

							while (bRead || bAver)
							{
								if (!par0->FlagJob || par0->FlagStop ||
									!parSRV1->m_bReadData ||
									!it.GetFlag(Flag_Kit_Read))
									break;	// ����� �� while (bRead || bAver)

								bRead = timeCur >= it.m_cTimeRead.GetTime();
								bAver = timeCur >= (double)it.m_cTimeRead.GetTimeAver();
								if (bRead)
								{
									par0->statusEx = dataRead;
									if (it.GetData(psrv, parSRV1->m_bSupport7000))
									{	// ���� � GetData ���� ������, �� � m_zts_timeDataCur ������������
										// ����� ����������� ������ � ���������������� ����� ������
										par0->statusEx = dataProcessing;
										// ����� ��������� ���������� �������, ���������
										// ���������� ������
										(*par0->Function)(parSRV2);
										par0->statusEx = working;
									}
									it.ResetAllFlagChangeFreq();
								}
								if (bAver)
								{
									par0->statusEx = waitSynshroGUI;

									if (it.m_hEventSynchro != NULL)
									{
										bFlag = WAIT_OBJECT_0 ==
											WaitForSingleObject(it.m_hEventSynchro, par0->TimeOutSend);
									}
									if (bFlag)
									{
										par0->statusEx = dataProcessingAver;
										CodeQuality retFunction = (*parSRV1->m_FunctionAver)(parSRV2);
										par0->statusEx = working;
										if (retFunction.IsAllData() && (par0->hWndParent != NULL))
										{// ������� � ���������� ���������� ������
											::PostMessageW(par0->hWndParent, WM_NEW_DATA_SRV,
												(WPARAM)indexKit, (LPARAM)retFunction);
										}
										it.m_cTimeRead.IncrementTimeAver();
									}
									else// GUI �� ���������
										break;	// ����� �� while (bRead || bAver)
								}
								if ((timeCur - timeCur0) > (*parSRV2->pTimeReadSumMax))
									//		if (++whileCount > WhileCountMax)
									break;	// ����� �� while (bRead || bAver)
							}// while (bRead || bAver)
						}//if (timeCur != DBL_MAX)
					}// else// if (parSRV1->m_FunctionAver == nullptr)
				}//if (bFlag)
				// �������� ������ �������� ������� �� ������� ����������
				for (auto & it_chan : it.m_vectChannels)
				{// ���� �� ������� ����������
					if (it_chan.GetFlag(Flag_Chan_Stop_Time) && (par0->hWndParent != NULL))
						::PostMessageW(par0->hWndParent, WM_STOP_TIME_CHANNEL, (WPARAM)it_chan.channel, 0);
				}
				LeaveCriticalSection(&it.m_crSectinKit);
				++indexKit;
			}// for (auto & it : *pVectKits)
			LeaveCriticalSection(&parSRV1->m_crSectinCollec);
		}// if (parSRV1->m_bReadData)

		// ���������� ����� while ----------------------------------------------
		if (!par0->FlagJob)		// ��� �������� ������ ���� ������
			break;

		if (!par0->FlagStop)
		{	// ���� ������ ���������
			par0->statusEx = sleeping;
			Sleep(par0->TimeSleep);
		}
		else
		{// ���� ���������� �����
			par0->FlagViewStop = true;
			par0->statusEx = suspend;
			SuspendThread(par0->hThread);	// ��������� ������� � ����� while
		}
	}// while ( par0->FlagJob )
	par0->statusEx = ending;
	return 0;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
template <class T>
CThreadSRV_CollectionOfKits<T>::CThreadSRV_CollectionOfKits()
	: CCustomThreadSRV()
	, m_indxKitCur(0)
	, m_numberKits(0)
	, m_numberKitsMax(200)
	, m_freqADCMin(50.f)
	, m_timeReadSumMax(20.)
	, m_pKitCur(nullptr)
{
	m_parSRV1.m_pParam2 = &m_parSRV2;
	parSRV0.pParamNext = &m_parSRV1;
}
//------------------------------------------------------------------------------
template <class T>
CThreadSRV_CollectionOfKits<T>::~CThreadSRV_CollectionOfKits()
{// ���������� ������� ��������� ���������� �����, � ����� ������� ������,
	// ����� �������� ��������, ����� ����� ��������� � ��� �������� ��������
	SynchroFinishThread();
}
//------------------------------------------------------------------------------
template <class T>
bool CThreadSRV_CollectionOfKits<T>::Create(
	CString nameThread,			// ��� ������
	HWND hwnd,					// HWND ������������� ����
	CDSRV *psrv, 				// ��������� �� ������	
	FunctionThread function,	// ����� ��������� �������
	PVOID paramFunction,		// ��������� ��������� �������,
	FunctionThread functionAver)// ����� ������� ����������
{
	bool ret = (function != nullptr) && (psrv != nullptr);
	if (ret)
	{
		m_zt_StartSRV = (__time64_t)psrv->GetStartTime();
		m_parSRV2.pTimeReadSumMax = &m_timeReadSumMax;
		m_parSRV2.m_psrv = psrv;
		m_parSRV2.m_pTimeStartSRV = &m_zt_StartSRV;
		m_parSRV2.m_pParamFromDlg = paramFunction;

		bool ret = CCustomThreadSRV::Create(nameThread, hwnd, psrv, 1,
			function, paramFunction);
		if (ret)
		{
			ret = CreateZetThread(&Function_CollectionOfKits<T>);
			if (ret)
			{
				m_parSRV1.m_FunctionAver = functionAver;
				m_parSRV1.m_vectKits.reserve(1);
				m_parSRV1.m_vectKits.resize(1);
				m_pKitCur = m_parSRV1.m_vectKits.data();
				m_numberKits = 1;
			}
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
// �������� ���������� ��������� ������� ������ (���). ������� ������������� ���
// ������ �� ������ GUI. ���� ��� �������, �� ������������ ��!
void CThreadSRV_CollectionOfKits<T>::WaitEndingFuncSecond()
{
	while (true)
	{
		if ((zParam0.statusEx == dataProcessing) ||
			(zParam0.statusEx == dataProcessingAver))
			Sleep(zParam0.TimeSleep);
		else
			break;
	}
}
//------------------------------------------------------------------------------
template <class T>
// ��������� � ���� ����������� ����������������� �������
void CThreadSRV_CollectionOfKits<T>::SetSynchroForAllKits()
{
	if (!m_parSRV1.m_vectKits.empty())
	{
		for (auto & it : m_parSRV1.m_vectKits)
			it.SetSynchro();
	}
}
//------------------------------------------------------------------------------
template <class T>
// ��������� ������������� ���-�� �����������
DWORD CThreadSRV_CollectionOfKits<T>::GetNumberKitsMax()
{
	return m_numberKitsMax;
}

template <class T>
// ������� ������������� ���-�� �����������
void CThreadSRV_CollectionOfKits<T>::SetNumberKitsMax(const DWORD val)
{
	if (val > 0)
		m_numberKitsMax = val;
}
//------------------------------------------------------------------------------
template <class T>
// ��������� �������� ����������-��������� ������� �������������� ������ ���
// ������� ������ � ���������� ��������
double CThreadSRV_CollectionOfKits<T>::GetMinFreqADC()
{
	return m_freqADCMin;
};
//------------------------------------------------------------------------------
template <class T>
// ������� ����������-��������� ������� �������������� ������ ���
// ������� ������ � ���������� ��������
void CThreadSRV_CollectionOfKits<T>::SetMinFreqADC(const double freq)
{
	if (freq > 0.f)
		m_freqADCMin = freq;
}
//------------------------------------------------------------------------------
template <class T>
// ��������� ������� �������� ���������� �������
DWORD CThreadSRV_CollectionOfKits<T>::GetIndexKitCur()
{
	return m_indxKitCur;
}
//------------------------------------------------------------------------------
template <class T>
// ������� �������� ���������� �������
// ������������ �������� true ���� index ����������.
bool CThreadSRV_CollectionOfKits<T>::SetIndexKitCur(const DWORD indxKit)
{
	bool ret = indxKit < m_numberKits;
	if (ret)
	{
		m_indxKitCur = indxKit;
		m_pKitCur = m_parSRV1.m_vectKits.data() + m_indxKitCur;
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
// ������� ��������� ���������� ��������� (�� ����� � ���������)
void CThreadSRV_CollectionOfKits<T>::IncrementKitCur()
{
	if (m_numberKits > 1)
	{
		++m_indxKitCur;
		if (m_indxKitCur == m_numberKits)
			m_indxKitCur = 0;
		m_pKitCur = m_parSRV1.m_vectKits.data() + m_indxKitCur;
	}
}
//------------------------------------------------------------------------------
template <class T>
// ��������� ����� ���������� ������ � ���������� ��������
bool CThreadSRV_CollectionOfKits<T>::GetSupport7000()
{
	return m_parSRV1.m_bSupport7000;
}
//------------------------------------------------------------------------------
template <class T>
// ������� ����� ���������� ������ � ���������� ��������
void CThreadSRV_CollectionOfKits<T>::SetSupport7000(const bool val)
{
	m_parSRV1.m_bSupport7000 = val;
}
//------------------------------------------------------------------------------
template <class T>
// ��������� ����� ���������� ������ ������
bool CThreadSRV_CollectionOfKits<T>::GetEnableReadData()
{
	return m_parSRV1.m_bReadData;
}
//------------------------------------------------------------------------------
template <class T>
// ������� ����� ���������� ������ ������
void CThreadSRV_CollectionOfKits<T>::SetEnableReadData(const bool val)
{
	m_parSRV1.m_bReadData = val;
	if (val)
		SetSynchroForAllKits();
	else
		WaitEndingFuncSecond();
}
//------------------------------------------------------------------------------
template <class T>
T * CThreadSRV_CollectionOfKits<T>::GetPointerOfKitCur()
{
	return m_pKitCur;
}
//------------------------------------------------------------------------------
template <class T>
// ��������� ����� ���������� ������ ������ � ������� ����������
bool CThreadSRV_CollectionOfKits<T>::GetEnableReadDataInKitCur()
{
	return (m_pKitCur != nullptr) ? m_pKitCur->GetFlag(Flag_Kit_Read) : false;
}

template <class T>
// ������� ����� ���������� ������ ������ � ������� ����������
void CThreadSRV_CollectionOfKits<T>::SetEnableReadDataInKitCur(const bool val)
{
	if (m_pKitCur != nullptr)
	{
		if (val)
			m_pKitCur->SetSynchro();
		else
			WaitEndingFuncSecond();
		m_pKitCur->SetFlag(Flag_Kit_Read, val);
	}
}
//------------------------------------------------------------------------------
template <class T>
T * CThreadSRV_CollectionOfKits<T>::GetPointerOfKit(const DWORD indxKit)
{
	T * ret(nullptr);
	if ((!m_parSRV1.m_vectKits.empty()) && (indxKit < m_parSRV1.m_vectKits.size()))
		ret = m_parSRV1.m_vectKits.data() + indxKit;
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
bool CThreadSRV_CollectionOfKits<T>::SetFirstChanByNumber(
	CHANNEL_FOR_KIT * pchFK, long channel)
{
	bool ret = pchFK != nullptr;
	if (ret)
	{
		pchFK->InitFlags();
		pchFK->PlaceFlag(Flag_Chan_OnOff);

		if (channel >= 0)
		{
			ret = 0 == parSRV0.pSRV->GetChannelParEx(channel, pchFK);
			pchFK->channel = channel;
			pchFK->PlaceFlag(Flag_Chan_Exist);
			pchFK->DoChangeStatus();
		//	pchFK->m_numKadrMax = parSRV0.pSRV->DecadeBufferSize(pchFK->m_decade) / 2;
			pchFK->m_numKadrMax = (DWORD)(int)(parSRV0.pSRV->DecadeBufferSize(pchFK->m_decade) * 0.8f);
			pchFK->SetFlag(Flag_Chan_7000, pchFK->frequency < m_freqADCMin);
		}
		else
		{
			pchFK->channel = -1;
			pchFK->ResetFlag(Flag_Chan_Exist);

			pchFK->name[0] = 0;
			pchFK->measure[0] = 0;
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
// ��������� ���-�� ����������� � ���������
DWORD CThreadSRV_CollectionOfKits<T>::GetNumberKits()
{
	return m_numberKits;
}
//------------------------------------------------------------------------------
template <class T>
// ������� ���-�� ����������� � ���������.
// ���� ����� ���-�� ������ �������� ���-��, �� ����������� ����������,
// ������ ���������� �� ����������. ���-�� ������� � ����� �����������
// ������������ � ������������ ��������� �.
// ����� � ����� ����� ������� ��������������� ����� ����������� (����������
// �� ���������). ����� ���-�� ������ ���� ������ 0.
// ������� ������-���������.
// ����������� ������ �� ���-�� � ������ ���, �.�. ��� ����������� � ������
// ��������� ���!!!
bool CThreadSRV_CollectionOfKits<T>::SetNumberKits(const DWORD newNum)
{
	bool ret = newNum <= m_numberKitsMax;
	if (ret)
	{
		DWORD number = (newNum > 0) ? newNum : 1;
		if (m_numberKits != number)
		{	// ���� ��������� ���������� � ��������� �� ������
			bool bEnableReadData = GetEnableReadData();
			SetEnableReadData(false);
			EnterCriticalSection(&m_parSRV1.m_crSectinCollec);

			// ���� ����������� ��������� �������, �� ���� �����
			WaitEndingFuncSecond();

			m_parSRV1.m_vectKits.resize(number);

			m_numberKits = number;
			if (m_indxKitCur >= m_numberKits)
				m_indxKitCur = m_numberKits - 1;
			m_pKitCur = m_parSRV1.m_vectKits.data() + m_indxKitCur;

			if (newNum == 0)
				SetFirstChanByNumber(m_pKitCur->m_vectChannels.data(), -1);

			SetSynchroForAllKits();
			LeaveCriticalSection(&m_parSRV1.m_crSectinCollec);
			SetEnableReadData(bEnableReadData);
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
// ���������� � ��������� ���������� �������, ������� ����� ������� �����������.
// ����������� ��������� ������ ��������� ��� ������� ���� �����.
// numChan - ���-�� ������� � ����������� ����������;
// chan1 - ������ ����� � ����������� ����������.
// ������ ������� �������� ���������� ���� long.
// ����� ������ "-1" ��������.
// ������� ������-���������.
// ������� ���������� ������ �������� ������.
DWORD CThreadSRV_CollectionOfKits<T>::AddKitToCollection(const DWORD numChan, const long chan1, ...)
{	// ���� ��������� ���������� � ��������� �� ������
	if (m_numberKits < m_numberKitsMax)
	{
		bool bEnableReadData = GetEnableReadData();
		SetEnableReadData(false);
		EnterCriticalSection(&m_parSRV1.m_crSectinCollec);

		// ���� ����������� ��������� �������, �� ���� �����
		WaitEndingFuncSecond();

		m_indxKitCur = m_numberKits;
		++m_numberKits;
		m_parSRV1.m_vectKits.reserve(m_numberKits);
		m_parSRV1.m_vectKits.resize(m_numberKits);
		m_pKitCur = m_parSRV1.m_vectKits.data() + m_indxKitCur;

		auto pvectChan = &m_pKitCur->m_vectChannels;
		pvectChan->reserve(numChan);
		pvectChan->resize(numChan);
		auto pCur = pvectChan->data();
		SetFirstChanByNumber(pCur, chan1);

		if (numChan > 1)
		{
			va_list arguments;
			va_start(arguments, chan1);

			for (DWORD i = 1; i < numChan; ++i)
			{
				long ch = va_arg(arguments, long);
				SetFirstChanByNumber(++pCur, ch);
			}

			va_end(arguments);
		}
		m_pKitCur->m_numberChannels = numChan;
		m_pKitCur->SetTimeStart(parSRV0.pSRV, m_parSRV1.m_bSupport7000);

		SetSynchroForAllKits();
		LeaveCriticalSection(&m_parSRV1.m_crSectinCollec);
		SetEnableReadData(bEnableReadData);
	}
	return m_indxKitCur;
}
//------------------------------------------------------------------------------
template <class T>
// ���������� � ��������� ���������� �������, ������� ����� ������� �����������.
// ������ ������� ������������ ���������� ����� ����� �� ������� vectChannels.
// ����� ������ "-1" ��������.
// ������� ������-���������.
// ������� ���������� ������ �������� ������.
DWORD CThreadSRV_CollectionOfKits<T>::AddKitToCollection(const std::vector<long> & vectChannels)
{
	if ((m_numberKits < m_numberKitsMax) && (!vectChannels.empty()))
	{	// ���� ��������� ���������� � ��������� �� ������
		bool bEnableReadData = GetEnableReadData();
		SetEnableReadData(false);
		EnterCriticalSection(&m_parSRV1.m_crSectinCollec);

		// ���� ����������� ��������� �������, �� ���� �����
		WaitEndingFuncSecond();

		m_indxKitCur = m_numberKits;
		++m_numberKits;
		m_parSRV1.m_vectKits.reserve(m_numberKits);
		m_parSRV1.m_vectKits.resize(m_numberKits);
		m_pKitCur = m_parSRV1.m_vectKits.data() + m_indxKitCur;

		auto pvectChan = &m_pKitCur->m_vectChannels;
		UINT numChan = vectChannels.size();
		pvectChan->reserve(numChan);
		pvectChan->resize(numChan);
		auto pCur = pvectChan->data();

		for (long ch : vectChannels)
			SetFirstChanByNumber(pCur++, ch);

		m_pKitCur->m_numberChannels = numChan;
		m_pKitCur->SetTimeStart(parSRV0.pSRV, m_parSRV1.m_bSupport7000);

		SetSynchroForAllKits();
		LeaveCriticalSection(&m_parSRV1.m_crSectinCollec);
		SetEnableReadData(bEnableReadData);
	}
	return m_indxKitCur;
}
//------------------------------------------------------------------------------
template <class T>
// �������� �� ��������� ���������� ����������. ��������� �� ���������, �� ���
// ������ � -1. ������� ������-���������.
void CThreadSRV_CollectionOfKits<T>::DeleteLastKit()
{	// ���� ��������� ���������� � ��������� �� ������
	bool bEnableReadData = GetEnableReadData();
	SetEnableReadData(false);
	EnterCriticalSection(&m_parSRV1.m_crSectinCollec);

	// ���� ����������� ��������� �������, �� ���� �����
	WaitEndingFuncSecond();

	if (m_numberKits > 1)
	{// � ��������� ����� ������ ����������, ��������
		m_parSRV1.m_vectKits.pop_back();		
		--m_numberKits;

		if (m_indxKitCur >= m_numberKits)
			m_indxKitCur = m_numberKits - 1;
		m_pKitCur = m_parSRV1.m_vectKits.data() + m_indxKitCur;

	}
	else
	{// � ��������� ���� ���������, ��� ��� ������ � -1
		for (auto * it : m_pKitCur->m_vectChannels)
			SetFirstChanByNumber(it, -1);
	}

	// ����������� ������ ���������
	SetSynchroForAllKits();
	LeaveCriticalSection(&m_parSRV1.m_crSectinCollec);
	SetEnableReadData(bEnableReadData);
}
//------------------------------------------------------------------------------
template <class T>
// ������ ���� ������� � ������� ����������.
// numChan - ����� ���-�� ������� � ������� ����������;
// chan1 - ����� ������ ����� � ������� ����������.
// ������ ������� �������� ���������� ���� long.
// ����� ������ "-1" ��������.
// ������� ������-���������.
void CThreadSRV_CollectionOfKits<T>::ChangeAllChannelsInKitCur(const DWORD numChan, const long chan1, ...)
{
	if ((m_pKitCur != nullptr) && (numChan > 0))
	{
		bool bEnableReadData = m_pKitCur->GetFlag(Flag_Kit_Read);
		m_pKitCur->ResetFlag(Flag_Kit_Read);
		EnterCriticalSection(&m_pKitCur->m_crSectinKit);

		// ���� ����������� ��������� �������, �� ���� �����
		WaitEndingFuncSecond();

		auto pvectChan = &m_pKitCur->m_vectChannels;

		pvectChan->reserve(numChan);
		pvectChan->resize(numChan);

		auto pCur = pvectChan->data();
		SetFirstChanByNumber(pCur, chan1);

		if (numChan > 1)
		{
			va_list arguments;
			va_start(arguments, chan1);

			for (DWORD i = 1; i < numChan; ++i)
			{
				long ch = va_arg(arguments, long);
				SetFirstChanByNumber(++pCur, ch);
			}

			va_end(arguments);
		}
		m_pKitCur->m_numberChannels = numChan;
		m_pKitCur->SetTimeStart(parSRV0.pSRV, m_parSRV1.m_bSupport7000);
		m_pKitCur->SetSynchro();
		LeaveCriticalSection(&m_pKitCur->m_crSectinKit);
		m_pKitCur->SetFlag(Flag_Kit_Read, bEnableReadData);
	}
}
//------------------------------------------------------------------------------
template <class T>
// ������ ���� ������� � ������� ����������.
// ����� ������ ������� �������� ���������� ����� ����� �� ������� vectChannels.
// ������� ������-���������.
void CThreadSRV_CollectionOfKits<T>::ChangeAllChannelsInKitCur(const std::vector<long> & vectChannels)
{
	if ((m_pKitCur != nullptr) && !vectChannels.empty())
	{
		bool bEnableReadData = m_pKitCur->GetFlag(Flag_Kit_Read);
		m_pKitCur->ResetFlag(Flag_Kit_Read);
		EnterCriticalSection(&m_pKitCur->m_crSectinKit);

		// ���� ����������� ��������� �������, �� ���� �����
		WaitEndingFuncSecond();

		auto pvectChan = &m_pKitCur->m_vectChannels;

		UINT numChan = vectChannels.size();
		pvectChan->reserve(numChan);
		pvectChan->resize(numChan);

		auto pCur = pvectChan->data();

		for (long ch : vectChannels)
			SetFirstChanByNumber(pCur++, ch);

		m_pKitCur->m_numberChannels = numChan;
		m_pKitCur->SetTimeStart(parSRV0.pSRV, m_parSRV1.m_bSupport7000);

		m_pKitCur->SetSynchro();
		LeaveCriticalSection(&m_pKitCur->m_crSectinKit);
		m_pKitCur->SetFlag(Flag_Kit_Read, bEnableReadData);
	}
}
//------------------------------------------------------------------------------
template <class T>
// ������ ���� ������� � �������� ����������.
// indxKit - ������ ��������� ����������;
// numChan - ����� ���-�� ������� � �������� ����������;
// chan1 - ����� ������ ����� � �������� ����������.
// ������ ������� �������� ���������� ���� long.
// ����� ������ "-1" ��������.
// ������������ �������� true ���� indxKit ����������.
// ������� ������-���������.
bool CThreadSRV_CollectionOfKits<T>::ChangeAllChannelsInKit(const DWORD indxKit, const DWORD numChan, const long chan1, ...)
{
	bool ret = (indxKit < m_parSRV1.m_vectKits.size()) && (numChan > 0);
	if (ret)
	{
		auto pKit = m_parSRV1.m_vectKits.data() + indxKit;

		bool bEnableReadData = pKit->GetFlag(Flag_Kit_Read);
		pKit->ResetFlag(Flag_Kit_Read);
		EnterCriticalSection(&pKit->m_crSectinKit);

		// ���� ����������� ��������� �������, �� ���� �����
		WaitEndingFuncSecond();

		auto pvectChan = &pKit->m_vectChannels;

		pvectChan->reserve(numChan);
		pvectChan->resize(numChan);

		auto pCur = pvectChan->data();
		SetFirstChanByNumber(pCur, chan1);

		if (numChan > 1)
		{
			va_list arguments;
			va_start(arguments, chan1);

			for (DWORD i = 1; i < numChan; ++i)
			{
				long ch = va_arg(arguments, long);
				SetFirstChanByNumber(++pCur, ch);
			}

			va_end(arguments);
		}

		pKit->m_numberChannels = numChan;
		pKit->SetTimeStart(parSRV0.pSRV, m_parSRV1.m_bSupport7000);

		pKit->SetSynchro();
		LeaveCriticalSection(&pKit->m_crSectinKit);
		pKit->SetFlag(Flag_Kit_Read, bEnableReadData);
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
// ������ ���� ������� � �������� ����������.
// ����� ������ ������� ��������� ���������� ����� ����� �� ������� vectChannels.
// ������������ �������� true ���� indxKit ����������.
// ������� ������-���������.
bool CThreadSRV_CollectionOfKits<T>::ChangeAllChannelsInKit(const DWORD indxKit, const std::vector<long> & vectChannels)
{
	bool ret = (indxKit < m_parSRV1.m_vectKits.size()) && !vectChannels.empty();
	if (ret)
	{
		auto pKit = m_parSRV1.m_vectKits.data() + indxKit;

		bool bEnableReadData = pKit->GetFlag(Flag_Kit_Read);
		pKit->ResetFlag(Flag_Kit_Read);
		EnterCriticalSection(&pKit->m_crSectinKit);

		// ���� ����������� ��������� �������, �� ���� �����
		WaitEndingFuncSecond();

		auto pvectChan = &pKit->m_vectChannels;

		UINT numChan = vectChannels.size();
		pvectChan->reserve(numChan);
		pvectChan->resize(numChan);

		auto pCur = pvectChan->data();

		for (long ch : vectChannels)
			SetFirstChanByNumber(pCur++, ch);

		pKit->m_numberChannels = numChan;
		pKit->SetTimeStart(parSRV0.pSRV, m_parSRV1.m_bSupport7000);

		m_pKit->SetSynchro();
		LeaveCriticalSection(&pKit->m_crSectinKit);
		pKit->SetFlag(Flag_Kit_Read, bEnableReadData);
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
// ������ ������ ������ � �������� ����������.
// indxChanInKir - ������ ������ � ������� ����������;
// chanNew - ����� ������ ������.
// ����� ������ "-1" ��������.
// ������������ true ���� indxChanInKir ����������.
// ������� ������-���������.
bool CThreadSRV_CollectionOfKits<T>::ChangeOneChannelInKitCur(const DWORD indxChanInKir, const long chanNew)
{
	bool ret = m_pKitCur != nullptr;
	if (ret)
	{
		auto pvectChan = &m_pKitCur->m_vectChannels;
		ret = indxChanInKir < pvectChan->size();
		if (ret)
		{
			auto pCur = pvectChan->data() + indxChanInKir;
			if (pCur->channel != chanNew)
			{
				EnterCriticalSection(&pCur->m_crSectionChan);
				SetFirstChanByNumber(pCur, chanNew);
				m_pKitCur->SetTimeStart(parSRV0.pSRV, m_parSRV1.m_bSupport7000);
				LeaveCriticalSection(&pCur->m_crSectionChan);
			}
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
// ������ ������ ������ � �������� ����������.
// indxKit - ������ ���������� � ���������;
// indxChanInKir - ������ ������ � ������� ����������;
// chanNew - ����� ������ ������.
// ������������ true ���� ��� ������� ����������.
// ������� ������-���������.
bool CThreadSRV_CollectionOfKits<T>::ChangeOneChannelInKit(const DWORD indxKit, const DWORD indxChanInKir, const long chanNew)
{
	bool ret = indxKit < m_parSRV1.m_vectKits.size();
	if (ret)
	{
		auto pKit = m_parSRV1.m_vectKits.data() + indxKit;
		auto pvectChan = &pKit->m_vectChannels;
		ret = indxChanInKir < pvectChan->size();
		if (ret)
		{
			auto pCur = pvectChan->data() + indxChanInKir;
			if (pCur->channel != chanNew)
			{
				EnterCriticalSection(&pCur->m_crSectionChan);
				SetFirstChanByNumber(pCur, chanNew);
				pKit->SetTimeStart(parSRV0.pSRV, m_parSRV1.m_bSupport7000);
				LeaveCriticalSection(&pCur->m_crSectionChan);
			}
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
// ������������� ��������� � ����������� ���������� ������ indxKit-��� ����������.
// ������������ �������� true ���� indxKit ����������.
bool CThreadSRV_CollectionOfKits<T>::SetSynchro(const DWORD indxKit)
{
	bool ret = indxKit < m_parSRV1.m_vectKits.size();
	if (ret)
	{
		auto pKit = m_parSRV1.m_vectKits.data() + indxKit;
		pKit->SetSynchro();
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
// ������� ������� ������ ��� �������� ����������.
// val - ����� ������.
void CThreadSRV_CollectionOfKits<T>::SetTimeStartForKitCur(const double time)
{
	if (m_pKitCur)
		m_pKitCur->SetTimeStart(time);
}
//------------------------------------------------------------------------------
template <class T>
// ������� ������� ������ ��� �������� ����������.
// ����� ������ ����� ���������� � ������ ������� ������� ����������.
void CThreadSRV_CollectionOfKits<T>::SetTimeStartForKitCur()
{
	if (m_pKitCur)
		m_pKitCur->SetTimeStart(parSRV0.pSRV, m_parSRV1.m_bSupport7000);
}
//------------------------------------------------------------------------------
template <class T>
// ������� ������� ������ ��� ��������� ����������.
// indxKit - ������ ���������� � ���������;
// val - ����� ������.
// ������������ �������� true ���� indxKit ����������.
bool CThreadSRV_CollectionOfKits<T>::SetTimeStartForKit(const DWORD indxKit, const double time)
{
	bool ret = indxKit < m_parSRV1.m_vectKits.size();
	if (ret)
	{
		auto pKit = m_parSRV1.m_vectKits.data() + indxKit;
		pKit->SetTimeStart(time);
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
// ������� ������� ������ ��� ��������� ����������.
// indxKit - ������ ���������� � ���������;
// ����� ������ ����� ���������� � ������ ������� ������� ����������.
// ������������ �������� true ���� indxKit ����������.
bool CThreadSRV_CollectionOfKits<T>::SetTimeStartForKit(const DWORD indxKit)
{
	bool ret = indxKit < m_parSRV1.m_vectKits.size();
	if (ret)
	{
		auto pKit = m_parSRV1.m_vectKits.data() + indxKit;
		pKit->SetTimeStart(parSRV0.pSRV, m_parSRV1.m_bSupport7000);
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
bool CThreadSRV_CollectionOfKits<T>::SetTimeStart()
{
	bool ret = !m_parSRV1.m_vectKits.empty();
	if (ret)
	{
		for (auto & it : m_parSRV1.m_vectKits)
		{
			bool b = it.SetTimeStart(parSRV0.pSRV, m_parSRV1.m_bSupport7000);
			ret &= b;	// ������ ���!!!
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
bool CThreadSRV_CollectionOfKits<T>::Modify()
{	// �������� �� ReStart �������
	CZetTime ztSRV = (__time64_t)parSRV0.pSRV->GetStartTime();
	CZetTimeSpan dztStartSRV = ztSRV - m_zt_StartSRV;
	bool bReStart = (0 != dztStartSRV.GetSeconds()) &&
		(0 != dztStartSRV.GetNanoseconds());	// ��� ReStart �������
	bool ret = !m_parSRV1.m_vectKits.empty();
	if (ret)
	{
		ret = false;
		for (auto & it : m_parSRV1.m_vectKits)
		{
			bool bEnableReadData = it.GetFlag(Flag_Kit_Read);
			it.ResetFlag(Flag_Kit_Read);

			EnterCriticalSection(&it.m_crSectinKit);
			WaitEndingFuncSecond();

			if (it.Modify(parSRV0.pSRV, m_chanTmp, m_freqADCMin))
				ret = true;
			if (bReStart)
			{	// ��� ReStart �������, ���� �������� ������� ������ � ����������,
				// �.�. ������� ������ ����� �� ���� ��������� ������
				// �������� ������� ����� ������ SRV
				it.m_cTimeRead.ReStartSRV(dztStartSRV);
			}

			it.SetSynchro();
			LeaveCriticalSection(&it.m_crSectinKit);

			it.SetFlag(Flag_Kit_Read, bEnableReadData);
			it.DefineFlagsOfKit();
		}
	}
	if (bReStart)
		m_zt_StartSRV = ztSRV;
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
// �������� ���������� ������� �������� ����������.
// ���� ���-�� ����������, �� ��������� true.
bool CThreadSRV_CollectionOfKits<T>::TestChannelsInKitCur()
{
	bool ret = (m_pKitCur != nullptr) &&
		!m_pKitCur->m_vectChannels.empty();
	if (ret)
	{
		ret = false;
		for (auto & it : m_pKitCur->m_vectChannels)
		{	// ���������� ������ ���� ����� �����
			if (it.GetFlag(Flag_Chan_OnOff))
			{	// ����� ��� �� ������������ ���
				// ����� ������ ��� ����������
				EnterCriticalSection(&it.m_crSectionChan);
				it.channel = parSRV0.pSRV->detectChanNum(it.id);
				it.SetFlag(Flag_Chan_Exist, it.channel >= 0);

				if (it.GetFlag(Flag_Chan_Exist))
				{
					parSRV0.pSRV->GetChannelParEx(it.channel, &m_chanTmp);
					it.SetFlag(Flag_Chan_7000, m_chanTmp.frequency < m_freqADCMin);
					if (it.TestChangeStruct(m_chanTmp))
						ret = true;
					it.DoChangeStatus();	// ������ ��� ����������
				}

				it = m_chanTmp;		// ������� ����� ��������
				LeaveCriticalSection(&it.m_crSectionChan);
			}
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
// ��������� ��������� ���������� ������ � ������ �������� ����������.
template <class T>
void CThreadSRV_CollectionOfKits<T>::GetTimeParamForKitCur(
	CZetTimeSpan & timeInternalRead, CZetTimeSpan & timeInternalBuffer)
{
	if (m_pKitCur)
	{
		timeInternalRead = m_pKitCur->m_cTimeRead.GetIntervalRead();
		timeInternalBuffer = m_pKitCur->m_cTimeRead.GetIntervalBuffer();
	}
	else
	{
		timeInternalRead = timeInternalBuffer = CZetTimeSpan();
	}
}
//------------------------------------------------------------------------------
template <class T>
void CThreadSRV_CollectionOfKits<T>::SetTimeParamForKitCur(
	const CZetTimeSpan & timeInternalRead, const CZetTimeSpan & timeInternalBuffer)
{
	if (m_pKitCur)
	{//  ������ � ����� �������
		CZetTimeSpan timeBuffer = ((CZetTimeSpan)timeInternalBuffer != CZetTimeSpan()) ?
									timeInternalBuffer : timeInternalRead;
		m_pKitCur->SetAllFlagChangeFreq(m_pKitCur->m_cTimeRead.GetIntervalBuffer() != timeBuffer);
		m_pKitCur->SetTimeParam(timeInternalRead, timeBuffer);
	}
}
//------------------------------------------------------------------------------
template <class T>
// ��������� ��������� ���������� ������ � ������ ����������.
// ������������ �������� true ���� indxKit ����������.
bool CThreadSRV_CollectionOfKits<T>::GetTimeParamForKit(const DWORD indxKit,
	CZetTimeSpan & timeInternalRead, CZetTimeSpan & timeInternalBuffer)
{
	auto pKit = GetPointerOfKit(indxKit);
	bool ret = pKit != nullptr;
	if (ret)
	{
		timeInternalRead = pKit->m_cTimeRead.GetIntervalRead();
		timeInternalBuffer = pKit->m_cTimeRead.GetIntervalBuffer();
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
bool CThreadSRV_CollectionOfKits<T>::SetTimeParamForKit(const DWORD indxKit,
	const CZetTimeSpan & timeInternalRead, const CZetTimeSpan & timeInternalBuffer)
{
	auto pKit = GetPointerOfKit(indxKit);
	bool ret = pKit != nullptr;
	if (ret)
	{//  ������ � ����� �������
		CZetTimeSpan timeBuffer = ((CZetTimeSpan)timeInternalBuffer != CZetTimeSpan()) ?
									timeInternalBuffer : timeInternalRead;
		pKit->SetAllFlagChangeFreq(pKit->m_cTimeRead.GetIntervalBuffer() != timeBuffer);
		pKit->SetTimeParam(timeInternalRead, timeBuffer);
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
// ��������� ��������� ������� ���������� �������� ����������
void CThreadSRV_CollectionOfKits<T>::GetTimeIntervalAverForKitCur(CZetTimeSpan & timeInternalAver)
{
	if (m_pKitCur)
	{
		timeInternalAver = m_pKitCur->m_cTimeRead.GetIntervalAver();
	}
}
//------------------------------------------------------------------------------
template <class T>
// ������� ��������� ������� ���������� �������� ����������
void CThreadSRV_CollectionOfKits<T>::SetTimeIntervalAverForKitCur(const CZetTimeSpan & timeInternalAver)
{
	if (m_pKitCur)
	{
		m_pKitCur->m_cTimeRead.SetIntervalAver(timeInternalAver);
	}
}
//------------------------------------------------------------------------------
template <class T>
// ��������� ��������� ������� ���������� �������� ����������
// ������������ �������� true ���� indxKit ����������.
bool CThreadSRV_CollectionOfKits<T>::GetTimeIntervalAverForKit(const DWORD indxKit, CZetTimeSpan & timeInternalAver)
{
	auto pKit = GetPointerOfKit(indxKit);
	bool ret = pKit != nullptr;
	if (ret)
	{
		timeInternalAver = m_pKit->m_cTimeRead.GetIntervalAver();
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
// ������� ��������� ������� ���������� �������� ����������
// ������������ �������� true ���� indxKit ����������.
bool CThreadSRV_CollectionOfKits<T>::SetTimeIntervalAverForKit(const DWORD indxKit, const CZetTimeSpan & timeInternalAver)
{
	auto pKit = GetPointerOfKit(indxKit);
	bool ret = pKit != nullptr;
	if (ret)
	{
		m_pKit->m_cTimeRead.SetIntervalAver(timeInternalAver);
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
// �������������� �������������� ������� ������� � ����������
CZetTime CThreadSRV_CollectionOfKits<T>::ConvertTimeSRVtoAbsolute(const CZetTimeSpan & ztsSRV)
{
	return m_zt_StartSRV + ztsSRV;
}
//------------------------------------------------------------------------------
// ������������ ��������� ����� ����� ����������� ������ ��� ������ ����������
template <class T>
double CThreadSRV_CollectionOfKits<T>::GetMaxSummTimeReadForOneKit()
{
	return m_timeReadSumMax;
}

template <class T>
void CThreadSRV_CollectionOfKits<T>::SetMaxSummTimeReadForOneKit(const double val)
{
	if (val < MinSumTimeReaddForOneKit)
		m_timeReadSumMax = MinSumTimeReaddForOneKit;
	else if (val > MaxSumTimeReaddForOneKit)
		m_timeReadSumMax = MaxSumTimeReaddForOneKit;
	else
		m_timeReadSumMax = val;
}
//------------------------------------------------------------------------------