//------------------------------------------------------------------------------
// CDialog_ZET.h : ���� ���������
//------------------------------------------------------------------------------
#pragma once
//#include <share.h>

// #ifdef _INTERFACE2012
// #include <Interface2012\Custom\Custom_ZETi12.h>
// typedef CCustom_ZETi12 BaseClass;
// #else
// #include <afxdisp.h>
// #include "afxwin.h"
// #include "afxdialogex.h"
// typedef CDialogEx BaseClass;
// #endif

#include <afxdisp.h>
#include "afxwin.h"
#include "afxdialogex.h"
typedef CDialogEx BaseClass;

#ifdef _CAutoScaleXY
#include <Dialog_ZET\AutoScaleXY.h>
#endif
#include <ZETView_basic_class\CDSRV.h>
#include <Dialog_ZET\CDZetPath.h>
#include <Dialog_ZET\CDWindowsLog.h>
#include <Dialog_ZET\CDUnit.h>
#include <Dialog_ZET\CDUnitSrv.h>
#include <Dialog_ZET\Translate.h>
#include <Dialog_ZET\ConnectToZetFile.h>
#include <Dialog_ZET\ConnectToZetTools.h>
#include <mathZet.h>
#include <Constants.h>
#include <CustomSaveStruct.h>

#ifdef _Grid
#include <Grafic1\gridglctrl1.h>
#endif
#ifdef _Gramma
#include <Grafic1\grammaglctrl1.h>
#endif
#ifdef _Plotter
#include <Grafic1\plotterxyctrl1.h>
#endif
#ifdef _Colscale
#include <Grafic1\colscalectrl1.h>
#endif
//------------------------------------------------------------------------------
// ���������, � �������� �������� ��������� ZETLab, � �.�. � �.�.
#include <ZetDef.h>
//------------------------------------------------------------------------------
// �������������� ��������, ��������� � ������� ������ �����������
// �������� �������� �� 1700 �� 1749
#define IDC_SRV			1700
#define IDC_ZETPATH		1701
#define IDC_WINLOG		1702
#define IDC_UNIT_PARENT	1703
#define IDC_MemoryLeak	1704
#define IDC_UNIT_Srv	1705
//------------------------------------------------------------------------------
// define � ��������� ���� "���������" � ZetDef.h
//------------------------------------------------------------------------------
struct DataForWatchCFG
{
	HWND hWnd;
	CString *pFileName;
	HANDLE handle[2];
	FILETIME lastWriteTime;
	WIN32_FILE_ATTRIBUTE_DATA attrib;
	DataForWatchCFG()
		: hWnd(0)

	{
		ZeroMemory(handle, 2 * sizeof(HANDLE));
		attrib.dwFileAttributes = 0;
		attrib.ftCreationTime = { 0 };
		attrib.ftLastAccessTime = { 0 };
		attrib.ftLastWriteTime = { 0 };
		attrib.nFileSizeHigh = 0;
		attrib.nFileSizeLow = 0;
		lastWriteTime = { 0 };
		pFileName = nullptr;
	}
};
//------------------------------------------------------------------------------
// ���������� ���� CDialog_ZET
class CDialog_ZET : public CDialogEx
//class CDialog_ZET : public BaseClass
{
public:
	CDialog_ZET(UINT nIDTemplate, CWnd* pParent,
				CString exeName,
				CString cfgFilePreamble, ULONG nZPriority,
				bool needSRV = true,  long numVirtChan = 0,
				bool needDAC = false, long numDAC = 0);
	virtual ~CDialog_ZET();

// ����������	
private:
	bool m_bFileHelp;				//	���� ���� ������� ���������
	PROCESS_INFORMATION piHelp;
	HANDLE m_hWatchCFG;
	HANDLE m_hEventEnd;
	CWinThread *m_pThreadWatchCFG;
	DataForWatchCFG m_DataForWatchCFG;

	void _OnModifySRV(long GlobalPar);
	void _OnFileModeSRV(long GlobalPar);
	virtual void _OnStartFileSRV(long GlobalPar);
	void _OnSetTimeSRV(float GlobalPar);
	void _MessageToZETPanel(UINT wm);

	
	//��������  ����� (�����) �� �����������
	bool CheckFileAccess(CString fileName);
	long CreateSRV(long numVirtChan, bool needDAC, long numDAC);

	//���������� ������� ��������� ��������� ������
	LRESULT OnReceiveStructureFromPanel(WPARAM wParam, LPARAM lParam);
	//���������� ���������� � ZETProject
	LRESULT OnSendStructure(WPARAM wParam, LPARAM lParam);
	//�������� ����� ������ ZETPanel
	LRESULT OnGetHwndOfPanel (WPARAM wParam, LPARAM lParam);
	
	//��������� ������� ������
	LRESULT _OnErrorReadData(WPARAM wParam, LPARAM lParam);
	LRESULT _OnErrorCalcData(WPARAM wParam, LPARAM lParam);
	LRESULT OnEndSRV(WPARAM wParam, LPARAM lParam);
	LRESULT OnStopTimeSRV(WPARAM wParam, LPARAM lParam);

	// ��������� UNIT-1
	void _OnReadyUnitParent(long param);

	// ��������� UNIT-2
	LRESULT _OnReadyUnit2Param(WPARAM wParam, LPARAM lParam);
	LRESULT OnFileCfgChange(WPARAM wParam, LPARAM lParam);

	// ������� ��� ����� ����� � �������� �� �����������
	void GetCommonDir();
	// ����������� ���� ������� ���������, �.�. ��������� ������.
	// �������� ��������������� ���� ������� ���-��.
	void DefineStartFlags();
	// ��������� ��� ������ ������� ���������
	void SetForStart_Manual();
	// ��������� ��� ������� ��������� ��� UNIT
	void SetForStart_Unit();
	// ��������� ��� ������� ��������� � ������� ������� Z-������
	void SetForStart_Zproj();
	// �������� �������� �� ����������� CFG-�����
	void CreateThreadWatchCFG();


protected:
	// ����������� �����
	CString m_sCodeError;
	CString m_sHz;
	CString m_sNumber;
	CString m_sFreqADC;
	CString m_sFolder;
	CString m_sNotAvailable;
	CString m_sError_ConnectToSRV;
	CString m_sError_ConnectToUNIT;
	CString m_sError_CreateClass;
	CString m_sError_CfgNotAvailable;
	CString m_sError_ZeroChanSRV;
	CString m_sError_NotSRV;
	CString m_sError_Help;
	CString m_sInform_Starting;
	CString m_sInform_StartFromUnit;
	CString m_sInform_StartFromPanel;
	CString m_sInform_ProgramWillBeClosed;
	CString m_sInform_ProgramCompleted;

	// �����, ����������� ������ ������ ���������
	bool m_bJobManual;			// ��������� �������� ����������
	bool m_bJobHide;
	bool m_bJobUnit;			// ��������� �������� UNIT-1
	bool m_bJobUnit2;			// ��������� �������� UNIT-2
	bool m_bJobPanel;			// ��������� �������� � ������� ������� Zet-������
	bool m_bProgramInitialized;	// ��������� ������������� ���������
	bool m_bNeedReSize;			// � ��������� �������� ��������� �������� ����
	bool m_bReadCFGFile;		// ������ ������ ���������������� ����
	bool m_bEnableInterface;	// ��������� ����������� �������� ����
	bool m_bPlacementCFG;		// ���������������� ���� � ������� � ��������� ����
	bool m_bLoadAltConfig;		// ��������� �������������� cfg-������
	bool m_bExistFileCfg;		// �������� �� ������� ����� cfg

	// ����� � ���������� ��� ������ � �������� ������
	bool m_bInterruptSRV;		// ��������� ��������� ������� ZetServer
	bool m_bModifySRV;			// ��� ����� ������� Modify
	bool m_bEnableChannelsSRV;	// � ZetServer ���� ������
	bool m_bNeedSRV;			// ��������� ��������� ZetServer
	bool m_bNeedDAC;			// ��������� ��������� ������ ���
	long m_NumVirtChan;			// ���-�� ����������� �������, ��������� ����������
	long m_NumDAC;				// ���-�� ��������� ������� ���
	float m_FreqADC;			// ������� ������������� ���
	float m_TimeADC;			// ������ ������� ������������� ���

	// ����� ������ � �����
	CString m_sSystemRoot;		// ��������� �����
	CString m_sCommentAndOrder;	// �������� ��������� � �������
	CString m_sCfgName;			// ��� cfg-����� (��� ���� � ����������)
	CString m_sExeName;			// ��� exe-��������� (��� ���� � ����������)
	CString m_sNameAndOrder;	// ��� ��������� � �������
	CString m_sFullNameFileCFG;	// ��� (� ����)) ����������������� �����
	CString m_sFullNameFileCHM;	// ��� (� ����)) ����� �������
	CString m_sFullFolderInstLoc;	// ����� ZetLab
	CString m_sFullFolderCFG;		// ����� Config
	CString m_sFullFolderCorrect;	// ����� Correct
	CString m_sFullFolderResult;	// ����� Result
	CString m_sFullFolderSignal;	// ����� Signal
	CString m_sFullFolderCompressed;// ����� Compressed

	virtual BOOL OnInitDialog();

	/*�������� � ����� OnInitDialog ���������������� ���������.
	��� ���������� ���� ������� ������� ON_MODIFYSVR �� ��������������*/
	void OnPostInitDialog(bool bNeedAutoScaleXY = true, bool bNeedZetPath = true);
	//������ �� ������� ���� � ���������� ������������
	CString ReadPathFolder(CString dir);
	//-----------------------------------------
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()

protected:
	RECT m_rectOld;				// ���������� ����� ��� ����������� �������� ����
	long m_nOrder;				// ����� ����������� ���������� ���������
	HWND m_panelhwnd;			// ���������� Zet-������
	ULONG m_nZPriority;			// Zet-��������� ���������
	UINT_PTR m_idTimerStart;	// ID ������� ��� ������ � ������ �����������
	UINT m_ElapseTimerStart;	//			����� ����� ������� (100 ����)
	UINT_PTR m_idTimerModify;	// ID ������� ��� �������� ����� SRV::Modify
	UINT m_ElapseTimerModify;	//			����� ����� ������� (500 ����)
	HWND *m_pEditForDigit;		// ��������� �� ������ �Edit ��� ����� �����
	int m_NumEditForDigit;		//			������ ����� �������
	UINT m_crc32ByNameExe = 0;	// ����������� �����, ������������ �� ����� EXE, ��. �����������
	uint16_t m_numberSelfExe = 1;	// ����� ����������� ��������, ���� �� 1
	TypeNewParam m_typeNewParam = param_no;

	//������, ������� ����� ����������� �� ���������
	PVOID pSaveData;			// ��������� �� ��������� Save
	PVOID pSaveDataNew;			// ��������� �� ��������� SaveNew
	int m_sizeSaveData;			// ������ ��������� Save � ������
	
	// ��������� ���������
	void OnStructureNewIfInit();
	void TranslatePhrasesThis();

	// ����������� ������� ��� ��������������� � �����������
	virtual void OnStructureDefault();	// ��������� ���������� �� ���������
	virtual void OnStructureNew();		// ��������� ����� ����������
	virtual void OnStructureSave() {}	// ���������� ������� ���������� ����� ��������� ���������
	virtual void OnPreReadCfg() {};		// ������� ����������� ����� ������� CFG-�����
	virtual void OnPostReadCfg() {};	// ������� ����������� ����� ������ CFG-�����
	virtual void OnStopTimeChannel(long chanStop) {}	// ������������ ����� �� ������ chanStop
	virtual void TranslatePhrases() {}	// ����������� ����� ���������
	virtual void OnErrorReadData(long indChan, long kodError){}	// ������ ��� ������ ������ � �������
	virtual void OnErrorCalcData(long indChan, long kodError){}	// ������ ��� �������� ������, ��������� � �������

	// ������ ������ �� ����������������� �����. ������� �����������, �
	// ���������� �������������� ��� ��������, � ������� ������ CFG-����� ��
	// �������� ����������.
	virtual bool ReadCFGFile();			// � pSaveDataNew
	// ������ ������ � ���������������� ����. ������� �����������, �
	// ���������� �������������� ��� ��������, � ������� ������ CFG-����� ��
	// �������� ����������.
	virtual bool WriteCFGFile();		// �� pSaveData
		
	// ��������� ������� ������ ocx
	// ZetServer
	virtual void OnFileModeSRV(long GlobalPar){}	// ���������/���������� ���������������
	virtual void OnSetTimeSRV(float GlobalPar){}	// ��������� ������� � ������ ���������������
	virtual void OnStartFileSRV(long GlobalPar){}	// ����� � ������ ���������������
	virtual void OnModifySRV(){}					// ��������� Modify
	virtual void OnFirstModifySRV_0(){}				// ������ Modify

	// Unit
	virtual void OnReadyUnitParent(long param){}
	virtual void OnReadyUnit2Param(){}
	
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT wParam, LPRECT pRect);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnQueryEndSession();
	afx_msg void OnEndSession(BOOL bEnding);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
//	afx_msg void OnClickedButtonParam();
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	// ������� Edit-��� ��� ����� �����
	void SetNumberEditForDigit(int num);
	// �����������/������� ����������
	void ShowInterface(bool val);
	void CopyStringToClipboard(CString str, bool bDig = true);

	// ������� ��������� ���� �� ��������� � ����������� �� ������ ���������
	void SetWindowPosDefault(WORD num, DWORD code);

	// ��������� ������ ������ ������� �� ���������.
	// ����� ������ ������������ �� ������ ��������� number.
	// ������������ �������� 0 - ������ �������, ����� ��� ������ ErrorCodes
	long GetDefaultChannel(long number, long & chan);
	// ��������� ������ ������ ������� �� ��������� � ��� ID.
	// ����� ������ ������������ �� ������ ��������� number.
	// ������������ �������� 0 - ������ �������, ����� ��� ������ ErrorCodes
	long GetDefaultChannel(long number, long & chan, long & id);
	// ��������� ������ ������ ������� �� ��������� � ��� GUID.
	// ����� ������ ������������ �� ������ ��������� number.
	// ������������ �������� 0 - ������ �������, ����� ��� ������ ErrorCodes
	long GetDefaultChannel(long number, long & chan, GUID & guid);

	void SetWindowPosDefault();

	CDSRV *m_psrv;
	CDZetPath *m_ppath;
	CDWindowslog *m_pwinlog;
	CDUnit *m_punitParent;
	CDUnitSrv *m_punitSrv;
	Unit2DataStruct m_paramUnit2;

	#ifdef _CAutoScaleXY
		CAutoScaleXY *m_pausc;
	#endif
//	CMemoryLeak *m_pmemoryLeak;
public:
	CToolTipCtrl *m_ptooltip;
	void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	void OnHelp();

	// ��������� ��������� � ������ ----------------------------------
	void MessageError(CString str, long id, long kod = 0);
	void MessageWarning(CString str, long id, long kod = 0);
	void MessageInformation(CString str);

	// ��������� ����������� ������ ��� ��������
	void SetGraficColors(CWnd *cont);
};
//------------------------------------------------------------------------------