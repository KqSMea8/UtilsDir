//------------------------------------------------------------------------------
// ����� �VoltMeter_DC : header file
//------------------------------------------------------------------------------
#pragma once
#include "afxwin.h"
#include "..\resource\resource.h"
#include <Flags.h>
#include <Intel\ipp\ipps.h>
//#include <ViewResult\StructViewResult.h>
#ifdef _INTERFACE2012
#include <Interface2012\Custom\Dialog_ZETi12.h>
typedef CDialog_ZETi12 VoltMeterBase;
#else
#include <Dialog_ZET\Dialog_ZET.h>
typedef CDialog_ZET VoltMeterBase;
#endif
#include <ZetThread\ThreadSRVmultiSelfS.h>
#include <Shield\Shield.h>
#include <mathZet.h>
#include <ViewResult\CViewResult.h>
//------------------------------------------------------------------------------
struct CHANNEL_STRUCT_READ_CALC : public CHANNEL_STRUCT_READ
{
	uint16_t fast;			// ��� ���������� ���������� (0, 1 ��� 2)
	uint16_t numFast;		// ���-�� ����������, �� ����� 3, ��������� ��� 7000
	uint16_t nPageMax;		// ���-�� ������ (�� 1, 10 ��� 100)
	uint16_t nPageCur;		// ����� ������� ������ (�� 0 �� nPageMax-1)

	CodeQuality qualityAll;	// ��������� ��� �������� ����������
	float mean;				// ��������� �������
	
	double time;			// SRV-����� ����������
	CViewResultParam vrp;	// ��������� ����������� ����������

	float meanCur[100];
	int counterNotNaN[100];
	CodeQuality qualityCur[100];

	CHANNEL_STRUCT_READ_CALC() : CHANNEL_STRUCT_READ()
		, nPageMax(1)
		, nPageCur(0)
		, mean(0.f)
		, numFast(3)
		, qualityAll(0)
		, fast(1)
		, time(0.)
	{
		ZeroMemory(meanCur, sizeof(meanCur));
		ZeroMemory(counterNotNaN, sizeof(counterNotNaN));
		ZeroMemory(qualityCur, sizeof(qualityCur));
	}

	~CHANNEL_STRUCT_READ_CALC() {}

	CHANNEL_STRUCT_READ_CALC(const CHANNEL_STRUCT_READ_CALC & val)
		: CHANNEL_STRUCT_READ()
	{
		*this = val;
	}

	CHANNEL_STRUCT_READ_CALC & operator = (const CHANNEL_STRUCT_READ_CALC & val)
	{
		*((CHANNEL_STRUCT_READ*)this) = *((CHANNEL_STRUCT_READ*)&val);
		nPageMax = val.nPageMax;
		nPageCur = val.nPageCur;
		mean = val.mean;
		fast = val.fast;
		numFast = val.numFast;
		qualityAll = val.qualityAll;
		time = val.time;

		ippsCopy_32f(val.meanCur, meanCur, 100);
		ippsCopy_32s(val.counterNotNaN, counterNotNaN, 100);
		ippsCopy_32s((int*)val.qualityCur, (int*)qualityCur, 100);

		return *this;
	}

	CHANNEL_STRUCT_READ_CALC & operator = (const CHANNEL_STRUCT & val)
	{
		*((CHANNEL_STRUCT*)this) = val;
		return *this;
	}
	
	void SetFast(WORD codeFast)
	{
		// ����������� ������ numFast
		if (GetFlag(Flag_Chan_7000))
		{
			if (frequency * 0.1 >= 1.)
				numFast = 3;
			else if (frequency >= 1.)
				numFast = 2;
			else
				numFast = 1;
		}
		else
			numFast = 3;

		// �������� ������ �������� fast
		if (codeFast > 2)
			fast = 2;
		else
			fast = codeFast;

		// ����������� ������ �������� fast � ������ ������ numFast
		if (numFast == 3)
		{// ��� �������� fast ���������
		}
		else if (numFast == 2)
		{// ����� 1 ��� � 10 ���
			if (fast == 0)
				fast = 1;
		}
		else// numFast == 1
		{// ����� ������ 10 ���
			if (fast < 2)
				fast = 2;
		}

		// ����������� nPageMax �� fast
		if (GetFlag(Flag_Chan_7000))
		{
			nPageMax = 1;
		}
		else
		{
			switch (fast)
			{
			case 0:
				nPageMax = 1;
				break;
			case 1:
				nPageMax = 10;
				break;
			case 2:
				nPageMax = 100;
				break;
			}
		}
	}

	// �������������
	bool Init()
	{
	//	nPageCur = 0;

		float freq((float)frequency);
		if (fast == 0)
			freq /= 10;
		else if (fast == 2)
			freq *= 10;

		return vrp.InitParam(this, (DWORD)(int)floor(freq + 0.5), false);
	}

	void NewValue(const float newVal)
	{
		vrp.NewValue(newVal);
	}

	void ReStart()
	{
	//	nPageCur = 0;
		vrp.ReStart();
	}
};
//------------------------------------------------------------------------------
struct SaveStructure : public CustomSaveStructure
{// ���������, ������� �������� � ���������������� ����� ���������
	GUID id;					// GUID ������
	uint32_t fast;				// ������ - ��������
	uint32_t reserved;			// ��������� ��� ������������ �� 8 ����
};
//------------------------------------------------------------------------------
class CVoltMeter_DC : public VoltMeterBase
{
private:
	DWORD m_nMaxForDecada0 = 0;
	DWORD m_indxCur = 0;
	uint32_t m_curFast = 1;

	bool TestIndx(const long indx){
		return (0 <= indx) && (DWORD)indx < m_vectParam.size(); }

	CHANNEL_STRUCT_READ_CALC* GetCurParam(int index);

public:
	CVoltMeter_DC(UINT nIDTemplate, CWnd* pParent, CString exeName,
		CString cfgFilePreamble, ULONG nZPriority);
	~CVoltMeter_DC() {}

protected:
	// ���������� --------------------------------------------------------------
	CString m_sSec;
	CString m_sFast_0_1;
	CString m_sFast_1_0;
	CString m_sFast_10;
	CString m_sAbsent;
	CString m_sDisabled;

	struct SaveStructure m_sParam;
	CString m_strMean;	// ��������� ������ � ������� PreTranslateMessage
	
	UINT m_ElapseTimerReStart = 300;

	std::vector<CHANNEL_STRUCT_READ_CALC*> m_vectParam;		// ������ ���������� �������
	CThreadSRVmultiSelfS *m_pThread = nullptr;

	// ��������� �� ��������� ���������� ������ �������� ������
	CHANNEL_STRUCT_READ_CALC * m_pCurParam = nullptr;
	
	// ������������� � ��������� -----------------------------------------------
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();

	virtual void OnStructureDefault();	// ��������� ���������� �� ���������
	virtual void OnStructureNew();		// ��������� ����� ����������


	// ������ ------------------------------------------------------------------
	LRESULT OnSetUpdate(WPARAM wParam, LPARAM lParam);	// ����� ������
	afx_msg void OnTimer(UINT_PTR nIDEvent);			// ������ 
	virtual BOOL PreTranslateMessage(MSG* pMsg);		// Ctrl + C

	void StopAllThread();
	void ReStart();
	void New_channel();		//����� ������
	void New_fast();		//������ - ��������

	// ������ � �������� -------------------------------------------------------
	void AddChannelByNumber(long channel, BOOL bValueCur, bool bChangeInWin);
	void AddChannelByGUID(GUID id, BOOL bValueCur, bool bChangeInWin);
	void SetStartTime(CHANNEL_STRUCT_READ_CALC * ps);
	void SetFlagsForNewChanOk();

	void ViewNewChan_iw(bool bClearMean);
	void ViewTitle_iw(bool bClearMean);		// ����������� � ���� ��� ������ ������

	// ������� -----------------------------------------------------------------
	virtual void OnFileModeSRV(long GlobalPar);
	virtual void OnSetTimeSRV(long GlobalPar);
	virtual void OnStartFileSRV(long GlobalPar);
	virtual void OnModifySRV();
	virtual void OnFirstModifySRV_0();		// ������ � ����� Modify

	// UNIT --------------------------------------------------------------------
	virtual void OnReadyUnitParent(long param);	// UNIT-1
	virtual void OnReadyUnit2Param();			// UNIT-2

	void SetUnitParam_p0(const long val);	//����� ������
	void SetUnitParam_p1(const long val);	//��� ����������
	void SetUnitParam_m1(const long val);	//�������������� �����, ���-�� �������
	void SetUnitParam_m2(const long val);	//������ �������� ������
	void SetUnitParam_m3(const long val);	//������ ���������������� ������
	void SetUnitParam_m4(const long val);	//������ ������������ ������
	void SetUnitParam_m5(const long val);	//����� ������������ ������

//	void WriteUnitResult_p0(const long shift, const float val);			//���������
	void WriteUnitResult_p1(const long shift, WideResult<float> *pval);	//������� ���������

	DECLARE_MESSAGE_MAP()

//------------------------------------------------------------------------------
protected:
	// ����������� �������, ������� ���� ���������� � �������� ������
	// ��� ��������� � ��������� ����������
	virtual void Set_Title_iw(LPCHANNEL_STRUCT pStruct) {}	// ����������� ���������
	// ����������� ��� ����������/���������� ������
	// code = 0 => ����� �����������
	// code = 1 => ����� ��������
	virtual void Set_ChannelOff_iw(long code) {}

	virtual void Set_Mean_iw(LPCWSTR str) {}	// ����������� ����������� ��������
	virtual void Set_Mera_iw(LPCWSTR str) {}	// ����������� ����������� ������

	virtual void Set_Channel_iw(const long val) {}	// ��������� �������������� ������
	virtual void Set_HorScale_iw(const float val) {}// ����������� ������ ������
	virtual void Set_Fast_iw(const uint16_t val) {}		// ��������� ������� ����������
	virtual void Set_NumFast_iw(const uint16_t numPosition) {}	// ������� ������ ������ ����������

	virtual void View_NewChan_iw() {}				// ������� ������ �������

	virtual void ResetOutData_iw() {}				// �������� ����������� �������� � ���
	virtual void Quality_iw(const long val) {}		// ����������� �������� ������

	// ������� ���-�� �������� ������� � ����������
	virtual void SetDigitalNumber_iw(LPCHANNEL_STRUCT pStruct) {}
};
//------------------------------------------------------------------------------
// RCDATA
/*
UNIT2_PARAM_SIZE_MAX RCDATA 
BEGIN
	4L
END

UNIT2_RESULT_SIZE_MAX RCDATA 
BEGIN
	16L
END

UNIT2_PARAM_TYPE RCDATA 
BEGIN
	L"0: 32s: ����� ������;"
	L"1: 32s: ��� ����������;"
	L"-1: 32s: �������������� �����, ���-�� �������;"
	L"-2: 32s: ������ �������� ������;"
	L"-3: 32s: ������ ���������������� ������;"
	L"-4: 32s: ������ ������������ ������;"
	L"-5: 32s: ����� ������������ ������;"

END

UNIT2_RESULT_TYPE RCDATA 
BEGIN
	L"1: wrs_32f: ������� ���������;"
END
*/
//------------------------------------------------------------------------------