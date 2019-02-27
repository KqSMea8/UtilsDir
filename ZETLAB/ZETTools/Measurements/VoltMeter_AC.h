//------------------------------------------------------------------------------
// ����� �VoltMeter_AC : header file
//------------------------------------------------------------------------------
#pragma once
#include "afxwin.h"
#include "resource.h"
#include <Intel\ipp\ipps.h>
#include <mathZet.h>
#include <ZetThread\ThreadSRVmultiSelf.h>
#include <Shield\Shield.h>
#include <CustomSaveStruct.h>
#ifdef _INTERFACE2012
	#include <Interface2012\Custom\Dialog_ZETi12.h>
	typedef CDialog_ZETi12 VoltMeterBase;
#else
	#include <Dialog_ZET\Dialog_ZET.h>
	typedef CDialog_ZET VoltMeterBase;
#endif
//------------------------------------------------------------------------------
struct MultiParamSelf
{// ��������� ���������� ������ � �������������� ������ ������, ���������������
//	��� ���������
	long fast;			// ��� ������� ���������� (0, 1 ��� 2)
	long ampl;			// �����������: 0 - ���, 1 - ���������
	long decibel;		// ����������� � ���������: 0 - ���, 1- ��
	long typeValUnit;	// ��� ������ ��� �������� � Unit
	float valAmpl;		// ���������� �������� ���������
	float valSks;		// ���������� �������� ���
	float valPeak;		// ���������� ������� ��������
	float *pData;		// ������ ������ ���� �������
	DWORD sizeAll;		// ������ ����� �������
	DWORD numberPage;	// ���-�� ������� (1, 10 ��� 100)
	DWORD numberPageCur;// ����� ������� ��������
	CodeQuality qualityAll;	// ��������� ��� �������� ����������
	CodeQuality pCodeQuality[100];	// �������� ������ ������
	double time;

	MultiParamSelf()
		: time(0.)
	{
		fast = ampl = decibel = typeValUnit = 0;
		valAmpl = valSks = valPeak = 0.f;
		sizeAll = numberPage = numberPageCur = 0;
		pData = nullptr;
	}
	~MultiParamSelf()

	{
		if (pData)
			
		{ 
			delete [] pData;
			pData = nullptr;
		}
		sizeAll = 0;
		time = 0.;
	}
	MultiParamSelf(const MultiParamSelf &val)
	{
		fast = val.fast;
		ampl = val.ampl;
		decibel = val.decibel;
		typeValUnit = val.typeValUnit;

		valAmpl = val.valAmpl;
		valSks = val.valSks;
		valPeak = val.valPeak;

		sizeAll = val.sizeAll;
		numberPage = val.numberPage;
		numberPageCur = val.numberPageCur;

		qualityAll = val.qualityAll;
		rsize_t n = numberPage * sizeof(long);
		memcpy_s(pCodeQuality, n, val.pCodeQuality, n);
	//	ippsCopy_32s((Ipp32s*)val.pCodeQuality, (Ipp32s*)pCodeQuality, (int)numberPage);

		if (val.pData)
		{
			pData = new float [sizeAll];
			ippsCopy_32f(val.pData, pData, sizeAll);
		}
		else
			pData = NULL;
	}
	bool ReSize(DWORD val)
	{
		bool ret;
		if (val > sizeAll)
		{
			if (pData)
			{ 
				delete [] pData;
				pData = nullptr;
			}
			pData = new float [val];
			sizeAll = val;
			ret = pData != NULL;
		}
		else
			ret = true;
		return ret;
	}
};
//------------------------------------------------------------------------------
struct SaveStruct : public CustomSaveStructure
{
	//wchar_t NameProgram[300];
	//int OrderProgram;
	long fast;			// ����������: 0 - 0,1 �; 1 - 1 �; 2 - 10 �;
	long channel;		// ����� ������
	long IdChan;		// ������������� ������
	long decibel;		// ����������� � ���������: 0 - ���, 1- ��
	long ampl;			// �����������: 0 - ���, 1 - ���������, 2 - ���
	long reserved;
};
//------------------------------------------------------------------------------
class CVoltMeter_AC : public VoltMeterBase
{
// Construction
public:
	CVoltMeter_AC(UINT nIDTemplate, CWnd* pParent, CString exeName,
		CString cfgFilePreamble, ULONG nZPriority);
	~CVoltMeter_AC() {}

protected:
	// ���������� --------------------------------------------------------------
	CString m_sSec;			// �
	CString m_sDb;			// ��
	CString m_sFast_0_1;    // ������� 0.1 �
	CString m_sFast_1_0;    // ��������� 1 �
	CString m_sFast_10;     // ������������� 10 �
	CString m_srms;			// ���
	CString m_srms_db;		// ���, ��
	CString m_samp;			// ���������
	CString m_samp_db;		// ���������, ��
	CString m_sPeak;		// ���


	SaveStruct	m_sParam,		// ��� ������� ��������
		m_sParamNew;	// ��� ����� ��������, ���������� �����
	CString strAmpl, strPeak, strTemp;//, m_strFormat;
	float m_ampl, m_sks, m_peak;
	UINT m_ElapseTimerReStart;	// ������ ��� ���������
	UINT_PTR m_IDtimerReStart;	//		�������� �� UNIT

	std::vector<MultiParamSelf> m_multi;
	bool m_bMulti;				// ���� ������ � �������������� ������
	// ��������� �� ��������� ���������� ������ �������� ������
	MultiChannelSelf *m_pmChanCur;
	// ��������� �� ��������� ���������� ��������� �������� ������
	MultiParamSelf   *m_pmParCur;

	// ������������� � ��������� -----------------------------------------------
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();

	virtual void OnStructureDefault();	// ��������� ���������
	virtual void OnStructureNew();		// ��������� ����� ����������

	void ViewNewChanInWindows();
	void SetFast(const long val);

	void DefaultMultiParam();
	void Show7000();
	long CalcMeasureType(const long ampl, const long decibel);

	// ������ ------------------------------------------------------------------
	LRESULT OnSetUpdate(WPARAM wParam, LPARAM lParam);	// ����� ������
	afx_msg void OnTimer(UINT_PTR nIDEvent);			// ������
	virtual BOOL PreTranslateMessage(MSG* pMsg);		// Ctrl + C

	// ��������� ���������� ----------------------------------------------------
	void StopAllThread();
	void New_channel(bool bTest, bool bReStart);	// ����� ������
	void New_fast(bool bTest, bool bReStart);		// ������ - ��������
	void New_ampl_decibel(bool bTest);				// 
	void ReStart(bool bStopThread, bool bNewTime = true);

	// ������� -----------------------------------------------------------------
	virtual void OnFileModeSRV(long GlobalPar);
	virtual void OnSetTimeSRV(long GlobalPar);
	virtual void OnStartFileSRV(long GlobalPar);
	virtual void OnModifySRV();
	virtual void OnFirstModifySRV_0();		// ������ � ����� Modify

	// UNIT --------------------------------------------------------------------
	virtual void OnReadyUnitParent(long param);
	virtual void OnReadyUnit2Param();

	void SetUnitParam_p0(const long val);	// ����� ������
	void SetUnitParam_p1(const long val);	// ��� ����������
	void SetUnitParam_p2(const long val);	// ��� ������ ��� UNIT
	void SetUnitParam_p3(const long val);	// ��� ������������� ��

	void SetUnitParam_m1(const long val);	// �������������� �����, ���-�� �������
	void SetUnitParam_m2(const long val);	// ������ �������� ������
	void SetUnitParam_m3(const long val);	// ������ ���������������� ������
	void SetUnitParam_m4(const long val);	// ������ ������������ ������
	void SetUnitParam_m5(const long val);	// ����� ������������ ������

//	void WriteUnitResult_p0(const long shift, const float val);			// ���������
	void WriteUnitResult_p1(const long shift, WideResult<float> *pval);	// ������� ���������

	DECLARE_MESSAGE_MAP()
//------------------------------------------------------------------------------
protected:
	// ����������� �������, ������� ���� ���������� � �������� ������
	// ��� ��������� � ��������� ����������
	virtual void CreateElementInterface() {}		// �������� ��������� ����������
	virtual void DeleteElementInterface() {}		// �������� ��������� ����������

	virtual void Set_Title_iw(const long val) {}	// ����������� ���������

	virtual void Set_Icon_iw() {}					// ����������� ������ ���������
	virtual void Set_Mean_iw(const CString &str, const float val) {}	// ����������� ����������� ��������
	virtual void Set_Peak_iw(const CString &str, const float val) {}	// ����������� ����������� ����
	virtual void Set_Mera_iw(const CString &str) {}	// ����������� ����������� ������

	virtual void Set_Channel_iw(const long val) {}	// ��������� �������������� ������
	virtual void Set_HorScale_iw(const float val) {}// ����������� ������ ������
	// ����������� ���� ������������� �������� - ���; ���, ��; ���������; ���������, ��
	virtual void Set_MeasureType_iw(const long val) {}
	virtual void Set_Fast_iw(const long val) {}		// ��������� ������� ����������

	virtual void View_NewChan_iw() {}				// ������� ������ �������
	virtual void View_MeasureType_iw() {}			// ������� ������ ����� ��������
	virtual void View_Average_iw() {}				// ������� ������ ������ ����������

	virtual void ResetOutData_iw() {}				// �������� ����������� �������� � ����
	virtual void Quality_iw(const long val) {}		// ����������� �������� ������
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
	L"2: 32s: ��� ������ ��� UNIT;"
	L"3: 32s: ��� ������������� ��;"
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