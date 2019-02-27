//------------------------------------------------------------------------------
// ����� Spectre_Base : header file
//------------------------------------------------------------------------------
#pragma once

#include <vector>

#include "afxwin.h"
#ifdef _INTERFACE2012
#include <Interface2012\Custom\Dialog_ZETi12.h>
typedef CDialog_ZETi12 SpectreBase;
#else
#include <Dialog_ZET\Dialog_ZET.h>
typedef CDialog_ZET SpectreBase;
#endif
#include <Shield\Shield.h>
#include <Buffer\SmartArray.h>
#include "spectre_params.h"
#include "window_funcs.h"
#include <ZetLogger/include/ZetLogger.h>
#include <Structures/Spectr/Spectr_Analysis_Result.zschema1.h>
#include <Structures/Spectr/Spectr_Analysis_CFG.zschema1.h>
#define WM_END_CRFILE_UNIT           WM_USER + 53

//------------------------------------------------------------------------------
struct ThreadParams
{
	CDSRV* server;
	//TODO:SV SaveStruct* saveParams;
	SaveStruct* saveParams;
	WorkParams* workParams;
	CalcDataStruct* calcData;
	std::vector<MultiParamSelf>* multiChannels;
	ThreadParams()
	{
		server = nullptr;
		saveParams = nullptr;
		workParams = nullptr;
		calcData = nullptr;
		multiChannels = nullptr;
	}
};
//------------------------------------------------------------------------------
class CSpectre_Base : public SpectreBase
{
private:
	wchar_t *m_pwcharTmp_400;
	UINT m_sizeWcharTmp_400;
	wchar_t *m_pwcharTmp_400_ts;
	UINT m_sizeWcharTmp_400_ts;
	wchar_t *m_pwcharTmp_100_prom;
	UINT m_sizeWcharTmp_100_prom;

// Construction
public:
	CSpectre_Base(UINT nIDTemplate, CWnd* pParent, CString exeName,	CString cfgFilePreamble, ULONG nZPriority);
	~CSpectre_Base();

protected:
	// ���������� --------------------------------------------------------------
	SaveStruct*	m_sParam;		// ��� ������� ��������
	SaveStruct*	m_sParamNew;	// ��� ����� ��������, ���������� �����
	ShowParams*	m_sParamReader;	// ��� �������� ���������� ����� ������� ���������������
	WorkParams m_workParams;
	CalcDataStruct m_calcData;
	ThreadParams m_threadParams; // ��������� ��� �������� � ������� �����

	CZETLoggerClient *m_pLogger = nullptr;
	Spectr_Analysis_Result* m_psaResultLogger = nullptr;
	Spectr_Analysis_CFG m_saConfigLogger;
	SmartArray<BYTE> m_saBufferLogger;
	float* m_pResultArrayLogger = nullptr;
	DWORD m_dwSizeArrayLogger;
	ZetHeader* m_pResultDescription = nullptr;

	CString stringTempl, m_strFormat;

	UINT m_ElapseTimerReStart;	// ������ ��� ���������
	UINT_PTR m_IDtimerReStart;	// �������� �� UNIT

	UINT m_ElapseTimerAutoscale;	// ����� �������� ����� �������������
	UINT_PTR m_IDtimerAutoscale;	// ID ������� ������������������� �������

	bool m_isStarted;			// ���� ������ ���������
	bool m_bMulti;				// ���� ������ � �������������� ������
	std::vector<MultiParamSelf> m_multi;

	int size;

	// ��������� �� ��������� ���������� ������ �������� ������
	MultiChannelSelf *m_pmChanCur;
	// ��������� �� ��������� ���������� ��������� �������� ������
	MultiParamSelf   *m_pmParCur;
	
	CThreadSRVmultiSelf *g_pThread;
	FileArrayStruct * m_pFAS;

	// ������������� � ��������� -----------------------------------------------
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void TestBackupChanMode();

	virtual void OnStructureDefault() override;	// ��������� ���������� ���������� �� ���������
	virtual void OnStructureNew() override;		// ��������� ����� ����������
	
	virtual void ViewNewChanInWindows();
	//void SetStrFormat(const long channel, const long decibel);
	//void SetFast(const long val);

	void DefaultMultiParam();
	void Show7000();
	bool CreateLogger();
	void SaveConfigToLogger();
	//long CalcMeasureType(const long ampl, const long decibel);

	// ������ ------------------------------------------------------------------
	LRESULT OnSetUpdate(WPARAM wParam, LPARAM lParam);	// ����� ������
	LRESULT OnEndGenerateFileUnit(WPARAM wParam, LPARAM lParam); // ���������� ��������� �� ��������� ������ � ���� 
	afx_msg void OnTimer(UINT_PTR nIDEvent);			// ������
	virtual BOOL PreTranslateMessage(MSG* pMsg);		// Ctrl + C

	// ��������� ���������� ----------------------------------------------------
	void StopAllThread();
	//void New_channel(bool bTest, bool bReStart);	// ����� ������
	//void New_fast(bool bTest, bool bReStart);		// ������ - ��������
	//void New_ampl_decibel(bool bTest);				// 
	void ReStart(bool bStopThread);

	// ������� -----------------------------------------------------------------
	virtual void OnFileModeSRV(long GlobalPar);
	virtual void OnSetTimeSRV(long GlobalPar);
	virtual void OnStartFileSRV(long GlobalPar);
	virtual void OnModifySRV();
	virtual void _OnModifySRV(long GlobalPar);
	virtual void OnFirstModifySRV_0();		// ������ � ����� Modify

	// UNIT --------------------------------------------------------------------
	virtual void OnReadyUnitParent(long param);
	virtual void OnReadyUnit2Param();

	void SetUnitParam_p0(const long val);	// ����� ������
	void SetUnitParam_p1(const long val);	// ��������� ������
	void SetUnitParam_p3(const double val);	// ��������� ���������� � ���
	void SetUnitParam_p4(const long val);	// ��������� ���� �������������
	void SetUnitParam_p5(const long val);	// ��������� �� ��������� ������� �������
	void SetUnitParam_p6(const long val);	// ��������� �� ��������� ������� ������
	void SetUnitParam_p7(const long val);	// ��������� �� ��������� �������� �������
	void SetUnitParam_p8(const long val);	// ��������� ���������� ����������, ������ ��
	void SetUnitParam_p9(const long val);	// ������� �������
	void SetUnitParam_p10(const long val);	// ��� ������� ��� ���
	void SetUnitParam_p11(const long val);	// ��� ������� �������
	void SetUnitParam_p12(const long val);	// ��������� �� ������ �������� ������
	void SetUnitParam_p13(const long val);	// �������������� ��� �����������������
	void SetUnitParam_p14(const long val);	// ������� ��� ���������������� �������
	void SetUnitParam_p15(const long val);	// ������ ����� � ����������
	void SetUnitParam_p16(const long val);	// ������ ��������� �����
	void SetUnitParam_p17(const long val);	// ��������� �������������� ������ ������
	void SetUnitParam_p18(const long val);	// ��������� �������
	void SetUnitParam_p19(const long val);	// ��������� �� ���������������� ��������� ������ (����� ������(0), ������(1), ������������ ������(2), ������� ������(3))
	void SetUnitParam_p20(const long val);	// ��������� �� ���������� ������������� �������
	void SetUnitParam_p21(const long val);	// ��������� �� ���������� �������� �������
	void SetUnitParam_p22(const double val);	// ��������� ��������� ������� �������������� ��������
	void SetUnitParam_p23(const CString val);	// 
	void SetUnitParam_p24(const double val);	// 
	void SetUnitParam_p25(const double val);	// 
	void SetUnitParam_p26(const long val);	// ��������� �� ������������� ��������� ������ � ������� ������������ �������
	void SetUnitParam_p27(const double val);	// ��������� ���������� ����������
	void SetUnitParam_p28(const long val);	// ��������� ���� ����������
	void SetUnitParam_p255(const long val);	// Start
	void SetUnitParam_p127(const long val);	// Stop


	void SetUnitParam_m1(const long val);	// �������������� �����, ���-�� �������
	void SetUnitParam_m2(const long val);	// ������ �������� ������
	void SetUnitParam_m3(const long val);	// ������ ���������������� ������
	void SetUnitParam_m4(const long val);	// ������ ������������ ������
	void SetUnitParam_m5(const long val);	// ����� ������������ ������

	//void WriteUnitResult_p0(const long shift, const float val);			// ���������
	//void WriteUnitResult_p1(const long shift, WideResult<float> *pval);	// ������� ���������

	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
//------------------------------------------------------------------------------
protected:
	// ����������� �������, ������� ���� ���������� � �������� ������
	// ��� ��������� � ��������� ����������
	virtual void CreateElementInterface() {}		// �������� ��������� ����������
	virtual void DeleteElementInterface() {}		// �������� ��������� ����������

	virtual void Set_Title_iw(const long val) {}	// ����������� ���������

	virtual void Set_Icon_iw() {}					// ����������� ������ ���������
	//virtual void Set_Mean_iw(const CString str) {}	// ����������� ����������� ��������
	//virtual void Set_Peak_iw(const CString str) {}	// ����������� ����������� ����
	//virtual void Set_Mera_iw(const CString str) {}	// ����������� ����������� ������

	//virtual void Set_Channel_iw(const long val) {}	// ��������� �������������� ������
	//virtual void Set_HorScale_iw(const float val) {}// ����������� ������ ������
	// ����������� ���� ������������� �������� - ���; ���, ��; ���������; ���������, ��
	//virtual void Set_MeasureType_iw(const long val) {}
	//virtual void Set_Fast_iw(const long val) {}		// ��������� ������� ����������

	//virtual void View_NewChan_iw() {}				// ������� ������ �������
	//virtual void View_MeasureType_iw() {}			// ������� ������ ����� ��������
	//virtual void View_Average_iw() {}				// ������� ������ ������ ����������

	virtual void ResetOutData_iw() {}				// ������� ����������� �������.
	//virtual void Quality_iw(const long val) {}		// ����������� �������� ������

	//**********************************************************************************

	virtual void SendOpen(HWND hWnd, int ind) {}

	virtual void InitializeView() {}

	bool StartSwitch();

	virtual void UpdateGrid() {}
	virtual void AutoscaleGrid() {}

	virtual void ShowGrammaWindow(bool isShow, bool sechgrammax = false, bool sechgrammay = false) {}
	virtual void UpdateGramma() {}
	virtual void ShowTDGrammaWindow(bool isShow) {}
	virtual void UpdateTDGramma() {}
	virtual void ShowKepstrWindow(bool isShow) {}
	virtual void UpdateKepstr() {}
	virtual void ShowProhodWindow(bool isShow) {}
	virtual void UpdateProhod() {}

	virtual void ShowResults();
	virtual void PaintSpectr() {}
	virtual void SetGridCursorPosition(int pos) {}
	virtual void SetGridStartPosition(double pos) {}
	virtual void SetGridInterval(double dx) {}

	virtual void ShowMaxGrf(bool isShow) {}
	virtual void PaintMaxGrf() {}
	virtual void ShowSredGrf(bool isShow) {}
	virtual void PaintSredGrf() {}
	virtual void ShowNormaGrf(bool isShow) {}
	virtual void PaintNormaGrf() {}

	virtual void PaintGramma() {}
	virtual void PaintTDGramma() {}
	virtual void PaintKepstr() {}
	virtual void PaintProhod() {}
	
	virtual void ResetGramma() {}
	virtual void ResetTDGramma() {}
	virtual void ResetProhod() {}

	virtual void SetCurAmplitude_iw(double ampl) {}
	virtual void SetCurInterval_iw(double curTimer) {}
	virtual void UpdateMaxValues_iw(float maxLevel, double maxFreq) {}

	virtual void WriteGridParams(FILE* out) {}

	virtual float GetGridStart() { return 0; }
	virtual float GetGridInterval() { return 0; }
	//=================================================================================

	virtual CString ShowDialogWriteQuestion(const CString &dirPath, const CString &fileName) { return L""; }

	virtual void ShowParametersWindow() {}

	//**********************************************************************************

	enum LocalStrings
	{
		StringTitle,
		StringNameOut,
		StringChangeChannel,
		StringMultiChannelMode,
		StringSetCurrentChannel,
		StringStartChannel,
		StringStopChannel,
		StringAddChannel,
		StringCantChangeSizeWhileShowGramma,
		StringPopravkaFile,
		StringArgHz,
		StringArgDecibel,
		StringWriteFileStringFreq, // ������ ��� ������ ������� � ����
		StringWriteFileStringDate,
		StringWriteFileStringTime,
		StringWriteFileStringFreqLevel,
		StringWrtieFileStringMax,
		StringWrtieFileStringSred,
		StringWrtieFileStringNorma,
		StringWrtieFileStringHz, 
		StringWriteIndicator, // ��������� ������
		StringWriteSimpling, // ������� (��)
		StringWriteMaximum, // ��������
		StringWriteStop, // ����
		StringWriteRecord, // ������
		StringWriteHzVal, //��
		StringWriteSettings, //��������� ���������� ������������� �������
		StringWriteSamplingRate, // ������� �������������
		StringWriteFreqRange, // ��������� ��������, ��
		StringWriteChanMeasure, // ����� ���������
		StringWriteFreqPermission, // ��������� ����������, ��
		StringWriteWeightFunc, // ������� �������
		StringWriteAveranging, // ����������, ���
		StringWriteInteDiffe, // ��������������/�����������������
		StringWriteRepresentation, // ��� �������������
		StringWriteCommentRecord, // ���������� ��� ������
		StringWriteCalculationY, // ������ �������� �� Y
		StringWriteMedianFilter, // ��������� ������
		StringWriteAdditionalWnd, // �������������� ����
		StringWriteIntervalCalc, // �������� �������
		StringWriteAdditionalDiagrams, // �������������� �������
		StringWriteAverangingType, // ��� ����������
		StringWriteProcessingType, // ��� ���������
		StringWriteNormFile, // ���� �����
		StringWriteKepstr, // ������
		StringWriteSpectrogram, // �������������
		StringWrite3DSpectrogram, // 3D - �������������
		StringWriteCheckP, // ���������
		StringWriteRangeCleaning, // ������� �������
		StringWriteMiddle, // �������
		StringWriteMax, // ������������
		StringWriteFileNorm, // ���� (�����)
		StringWriteFastFourier, // ������� ��
		StringWriteDiscreteFourier, // ���������� ��
		StringWriteTwoDiff, // ������� �����������������
		StringWriteOneDiff, // �����������������
		StringWriteNotProcessing, // ��� ���������
		StringWriteOneIntegrat, // ��������������
		StringWriteTwoIntegrat, // ������� ��������������
		StringWriteRectangular, // �������������
		StringWriteHanna, // �����
		StringWriteHamming, // �������
		StringWriteBlackman, // ��������
		StringWriteBarlet, // �������
		StringWriteKaizer, // �������
		StringWriteReefVincent, // ����-��������
		StringWriteBlackManHarris, // ��������-�������
		StringWriteNatal, // �������
		StringWriteBlackmanNatal, // ��������-�������
		StringWriteWndPlanePeak, // ���� � ������� ��������
		StringWriteLinear, // ��������
		StringWriteLogarithmic, // ���������������
		StringWriteSpectralDensity, // ������������ ���������
		StringWriteSpectralPower, // ������������ ��������
		StringWriteAverageSquare, // ������� ������������
		StringWritePeakValue, // ������� ��������
		StringWriteExponential, // ����������������
		StringWriteWndParam, // �������/������� ���� ����������
		StringWriteWndData, // �������/������� ���� ������
		StringWriteWndEnd, // �������
		StringWriteWndHelp, // �������
		StringWriteWndMin, // ��������
		StringWriteWndMax, // ����������
		StringWriteSpectr, // ������
		StringWriteHerz, // ��
		StringWriteStart, // �����
	};

	virtual CString GetLocalString(LocalStrings id) { return L""; }; // ��������� �������������� �������.

	virtual void InitializeDefaultParams(); // �������������� ������������ ����������.
	virtual void InitializeDefaultChannel(MultiParamSelf* pmp, MultiChannelSelf* mChan); // �������������� ������������ ������.
	virtual void InitializeDefaultData(); // �������������� ������������ ������.

	virtual void UpdateCurrentChannelParams(bool setDefaultForced = false, bool channelChangedByServer = false);

	void WriteFile(const CString &fileName);

	void UpdateMaxValues();

	bool IsCurrentChannelExist();
	void InitializeCurrentChannel(bool withUI = false);

	void resetAdditionalGraphs();

	//==================================================================================
private:

	bool setChannel(bool channelChangedByServer);
	bool setDecade();
	bool setIntegral();
	bool setUsredType();
	bool setUsredValue();
	bool setAnalis();
	bool setSpm();
	bool setInterval();
	bool setLinlog();
	bool setClean();
	bool setSizeAndWindType(bool byUser = true);
	bool setShowMaxGrf();
	bool setShowSredGrf();
	bool setAveragingType();

	void initializeMaxGraph(MultiParamSelf* pmp);
	void initializeSredGraph(MultiParamSelf* pmp);

	void initializeProhod();

	void resetAdditionalGraphs(MultiParamSelf* pmp);
};
//------------------------------------------------------------------------------