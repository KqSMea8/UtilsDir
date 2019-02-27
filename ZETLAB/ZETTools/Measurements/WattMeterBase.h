#pragma once
#include "afxdialogex.h"
#include <Intel\ipp\ipps.h>
#include <Dialog_ZET\Dialog_ZET.h>
#include <ZetThread\ThreadSRVmultiSelf.h>
#include <Shield\Shield.h>
#include <NaNsupport.h>
#include "resource.h"
#include <vector>
#include <WideResult\WideResult.h>
#include <ResultSynchronization\ResultSynchronization2.h>
using std::vector;
typedef CDialog_ZET WattMeter_Base;

struct ValueStruct	// ��������� ������ ������������ Unit'�
{
	long quality;	// �������� ������
	float S;		// ������ ��������
	float P;		// �������� ��������
	float Q;		// ���������� ��������
	double time;	// ����� �� �������� ��������� ����������
};

struct SaveStruct : public CustomSaveStructure
{
	//wchar_t NameProgram[300];
	//int NumberProgram;											// ����� ���������
	int channel1;												// ����� ������� �������������� ������
	long IdChan1;												// id ������� �������������� ������
	int channel2;												// ����� ������ �������������� ������
	long IdChan2;												// id ������ �������������� ������
	int fast;													// ����� ���������� 
	int typeview;												// ��� ��������� �������� ���������
};

struct MultiParamSelf
{
	long fast;									// ����� ����������
	long typeValUnit;							// 
	long Channel;								// ����� ������� �������������� ������
	float* pData;
	DWORD sizeAll;								// ������ �������
	DWORD numberPage;							// ���������� �������
	float usred;								// ��� ������� �����������
	DWORD numberPageCur;						// ����� ������� ��������
	CodeQuality qualityAll;
	CodeQuality pCodeQuality[100];
//	CResultSynchronization2<float*, float*> *p;
	double time;
	MultiParamSelf()
		: Channel(0)
		, typeValUnit(0)
		, fast(0)
		, usred(0.f)
		, time(0.0)
		, numberPage(0)
		,pData(nullptr)
	{
		sizeAll = numberPageCur = 0;
//		p = new CResultSynchronization2<float*, float*>;
	}
	MultiParamSelf(const MultiParamSelf &val)
	{
		fast = val.fast;
		typeValUnit = val.typeValUnit;
		sizeAll = val.sizeAll;
		numberPage = val.numberPage;
		numberPageCur = val.numberPageCur;
		qualityAll = val.qualityAll;
//		if (p && val.p)
//		memcpy_s(p, sizeof(CResultSynchronization2<float*, float*>), val.p, sizeof(CResultSynchronization2<float*, float*>));
		rsize_t n = numberPage * sizeof(long);
		memcpy_s(pCodeQuality, n, val.pCodeQuality, n);
		if (val.pData)
		{
			pData = new float[sizeAll];
			ippsCopy_32f(val.pData, pData, sizeAll);
		}
		else
			pData = nullptr;
	}
	~MultiParamSelf()
	{
		if (pData != nullptr)
		{
			delete[] pData;
			pData = nullptr;
		}
		sizeAll = 0;
	}
	bool ReSize(DWORD val)
	{
		bool ret;
		if (val > sizeAll)
		{
			if (pData)
			{ 
				delete[] pData;
				pData = nullptr;
			}
			sizeAll = val;
			pData = new float[val];
			ret = pData != nullptr;
		}
		else
			ret = true;
		return ret;
	}
};

class CWattMeterBase : public WattMeter_Base
{
protected:
	SaveStruct	m_sParam;										// ��� ������� ��������
	SaveStruct	m_sParamNew;									// ��� ����� ��������, ���������� �����
	long m_IdVirtChan1;											// id ������������ ������� ������
	long m_IdVirtChan2;											// id ������������ ������� ������
	long m_IdVirtChan3;											// id ������������ �������� ������
	std::vector<MultiParamSelf> m_multi;	
	MultiChannelSelf *m_pmChanCur1;								// ��������� �� ��������� ���������� ������ �������� ������	
	MultiChannelSelf *m_pmChanCur2;								// ��������� �� ��������� ���������� ������ �������� ������	
	MultiParamSelf   *m_pmParCur1;								// ��������� �� ��������� ���������� ��������� �������� ������
	MultiParamSelf   *m_pmParCur2;								// ��������� �� ��������� ���������� ��������� �������� ������
	bool m_bMulti;												// ���� ������ � �������������� ������
	UINT_PTR m_IDtimerReStart;									// �������� �� UNIT
	UINT m_ElapseTimerReStart;									// ������ ��� ���������
	WideResult<vector<float>> m_result_f;						// ������ � ������������ ����������
	virtual void OnReadyUnitParent(long param);
	virtual void OnReadyUnit2Param();

	void SetUnitParam_p0(const long val);						// ��������� �������� ������� ������ 
	void SetUnitParam_p1(const long val);						// ��������� �������� ������� ������
	void SetUnitParam_p2(const long val);						// ��������� �������� �������� ����������
	void SetUnitParam_p3(const long val);						// ��������� �������� ���� ���������� ��������

	void SetUnitParam_m1(const long val);						// �������������� �����, ���������� �������
	void SetUnitParam_m2(const long val);						// ������ �������� ������
	void SetUnitParam_m3(const long val);						// ������ ���������������� ������
	void SetUnitParam_m4(const long val);						// ������ ������������ ������
	void SetUnitParam_m5(const long val);						// ����� ������������ ������
public:
	CWattMeterBase(UINT nIDTemplate, CWnd* pParent, CString exeName,
		CString cfgFilePreamble, ULONG nZPriority);
	~CWattMeterBase();
	virtual BOOL OnInitDialog();
	virtual void SetChannel1(const int val)		{};				// ��������� ������� ������ ���������
	virtual void SetChannel2(const int val)		{};				// ��������� ������� ������ ���������
	virtual void SetFast(const long val);						// ��������� ������� ����������
	virtual void OnFileModeSRV(long GlobalPar);					// ��������� ����� ���������� 
	virtual void OnStartFileSRV(long GlobalPar);				// ��������� �������
	virtual void TestBackupChanMode();							// ��������� ������ ��� �������
	virtual void SetHorscaleVal(float val)		{};				// ��������� �������� � �������� ������
	virtual void SetText1(const CString val)	{};				// ��������� ����� �������� ��������
	virtual void SetText2(const CString val)	{};				// ��������� ����� ������ ��������
	virtual void SetText3(const CString val)	{};				// ��������� ����� ���������� ��������
	virtual void SetVal1(const CString val)		{};				// ����� �������� �������� ��������
	virtual void SetVal2(const CString val)		{};				// ����� �������� ������ ��������
	virtual void SetVal3(const CString val)		{};				// ����� �������� ���������� �������� 
	virtual void SetMera1(const CString val)	{};				// ����� ������� ��������� �������� ��������
	virtual void SetMera2(const CString val)	{};				// ����� ������� ��������� ������ ��������
	virtual void SetMera3(const CString val)	{};				// ����� ������� ��������� ���������� ��������
	virtual void AddMera1(CString val)			{};				// ���������� ��������� � ������� ��������� �������� ��������
	virtual void AddMera2(CString val)			{};				// ���������� ��������� � ������� ��������� ������ ��������
	virtual void AddMera3(CString val)			{};				// ���������� ��������� � ������� ��������� ���������� ��������
	virtual void SetFastI(const long val)		{};				// ��������� � ���������� �������� ����������
	virtual void SetViewI(const long val)		{};				// ��������� � ���������� ���� ��������� �������� 
	virtual void ResetOutData_iw()				{};				// �������� ������� ���������
	virtual void ResetMera()					{};				// ��������� ����������� ������� ��������� ���������
	virtual void SetColor(COLORREF val)			{};				// ��������� ����� ��������� ����������
	virtual void OnStructureDefault();							// ��������� ���������� ��������� �� ���������
	virtual void OnStructureNew();								// ��������� ����� ���������� ���������
	virtual void SetTitleI(const int val1, const int val2)	{}; // ��������� ������ � ����� ���������
	LRESULT OnSetUpdate(WPARAM wParam, LPARAM lParam);			// ���������� ���������
	afx_msg void OnDestroy();									// ���������� ������� 
	afx_msg void OnTimer(UINT_PTR nIDEvent);					// ������
public:
	void ReStart(bool bstopThread, bool bNewTime = true);
	void New_channel(bool bTest, bool bReStart);				// ��������� ������ ������ ���������
	void NewViewDisplay(long val);								// ��������� ���� ��������� �������� ��������
	void StopAllThread();										// ��������� ������
	void OnModifySRV();											// ���������� ��������� �� �������
	void OnSetTimeSRV(long GlobalPar);
	void New_fast(bool bTest, bool bReStart);					// ��������� ������ ������� ����������
	void DefaultMultiParam();									// ��������� ���������� �� ���������
	void ViewNewChanInWindows();	
	void SetInfoVrtCahnnel(const long fchannel);				// ��������� �������� ������������ ������
	void NumberFormat(const float val, int &degree, 
		int &flo, CString &type);								// �������������� ������ ������ �������� ���������
	DECLARE_MESSAGE_MAP()
};

