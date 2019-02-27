//------------------------------------------------------------------------------
// CDialog_ZET.cpp : ���� ����������
//
//------------------------------------------------------------------------------
//����������� ����������� ���������� 
#include "stdafx.h"
#include <set>
#include <Dialog_ZET\Dialog_ZET.h>
#include <ZetColors.h>
//------------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//------------------------------------------------------------------------------
UINT GlobFunctionWatchCFG(LPVOID pParam)
{
	DataForWatchCFG *pPar = (DataForWatchCFG*)pParam;
	bool flag = true;
	while (flag)
	{
		switch ( WaitForMultipleObjects(2, pPar->handle, FALSE, INFINITE) )
		{
		case WAIT_OBJECT_0:
			FindNextChangeNotification(pPar->handle[0]);
			if (GetFileAttributesEx(*pPar->pFileName,
					GetFileExInfoStandard, &pPar->attrib) != FALSE)
			{
				if ( CompareFileTime(&pPar->attrib.ftLastWriteTime, &pPar->lastWriteTime) == 1 )
				{// ���� ��� �����������
					pPar->lastWriteTime = pPar->attrib.ftLastWriteTime;
					::PostMessageW(pPar->hWnd, WM_FILE_CFG_CHANGE, 0, 0);
				}
			}
			break;
		case WAIT_OBJECT_0 + 1:
			flag = false;
		//	break;
		}
	}
	return 0;
}
//------------------------------------------------------------------------------
// ����������� ����������� ���� CDialog_ZET
CDialog_ZET::CDialog_ZET(
	UINT nIDTemplate,			// ������������� ���������
	CWnd* pParent,				// ��������� �� ���������
	CString exeName,			// ��� exe-����� ���������
	CString cfgFilePreamble,	// ��� ��������� ����������������� �����
	ULONG nZPriority,			// ��������� ��������� � Zet-������ (�� 0 �� 3)
	bool needSRV,				// ����� ������
	long numVirtChan,			// ���-�� ����������� ����������� �������
	bool needDAC,				// ����� ���
	long numDAC) :				// ����� ������������� ���
		BaseClass(nIDTemplate, pParent)
	, m_hWatchCFG		(NULL)
	, m_hEventEnd		(NULL)
	, m_pThreadWatchCFG	(NULL)
	, m_bFileHelp		(false)
	, m_bNeedReSize		(true)
	, m_bModifySRV		(true)
	, m_bLoadAltConfig	(false)
	, m_FreqADC			(0.1f)
	, m_TimeADC			(1.f)
	, pSaveData			(0)
	, pSaveDataNew		(0)
	, m_sizeSaveData	(0)
	, m_psrv			(nullptr)
	, m_ppath			(nullptr)
	, m_pwinlog			(nullptr)
	, m_punitSrv		(nullptr)
	, m_punitParent		(nullptr)
	, m_ptooltip		(nullptr)
	, m_bJobHide		(false)
	, m_bPlacementCFG	(false)
	, m_bExistFileCfg	(false)
{
	ztInitCreateDumpWhenException();

	// ��� ��������� ---------------------------------------------------
	m_sExeName.Preallocate(SIZE_NAME_PROGRAM);
	ztGetNameSelfExe(m_sExeName.GetBuffer(), SIZE_NAME_PROGRAM);
	m_sExeName.ReleaseBuffer();
	m_sCfgName = m_sExeName;
	// ��� ��������� ����� ��� ������ ��� ���� ����� �������
	m_sCfgName.MakeLower();
	m_crc32ByNameExe = ztCRC32(m_sCfgName);
	m_sCfgName = m_sExeName;	// �������������� �������� ����

	// ������� ----------------------------------------------------------
	InitZetDictionary(exeName);
	TranslatePhrasesThis();
	// ------------------------------------------------------------------
	//��������� ������� ��������� (�� 0 �� 3 ������������)
	m_nZPriority = nZPriority;

	// ���������� ��� ������ � �������� ������
	m_bNeedSRV = needSRV;
	m_bNeedDAC = needDAC;
	m_NumVirtChan = numVirtChan;
	m_NumDAC = numDAC;
	
	// ����� ������� ������ �� ���������
	m_bJobManual = true;
	m_bJobUnit = false;
	m_bJobUnit2 = false;
	m_bJobPanel = false;
	m_bProgramInitialized = false;
	m_bEnableChannelsSRV = true;	// �����������
	m_bInterruptSRV = false;		// �����������
	m_bReadCFGFile = false;
	m_bEnableInterface = true;

	m_nOrder = 0;	// �� ������ ������
	m_idTimerStart = 8;
	m_ElapseTimerStart = 100;
	m_idTimerModify = 9;
	m_ElapseTimerModify = 500;
	piHelp.hProcess = nullptr;

#ifdef _CAutoScaleXY
	m_pausc = nullptr;
#endif

	m_pEditForDigit = nullptr;
	m_NumEditForDigit = 0;
	m_panelhwnd = 0;					//����� ����������� ������ ZETPanel

	// rect ��� ����������� ����������� �������� ����
	m_rectOld.left = m_rectOld.right = m_rectOld.top = m_rectOld.bottom = 0;
}
//------------------------------------------------------------------------------
void CDialog_ZET::TranslatePhrasesThis()
{	// ����������� ����� �������� ������
	m_sCodeError = L". " + TranslateString(L"��� ������") + L" = ";

	m_sNumber = TranslateString(L"�");
	m_sHz = TranslateString(L"��");
	m_sFreqADC = TranslateString(L"������� ������������� ���");
	m_sFolder = TranslateString(L"�����");
	m_sNotAvailable = TranslateString(L"����������");
	m_sError_ConnectToSRV = TranslateString(L"������ ��� ����������� � ������� ������");
	m_sError_ConnectToUNIT = TranslateString(L"������ ��� ����������� � UNIT");
	m_sError_CreateClass = TranslateString(L"������ ��� �������� ���������� ������");
	m_sError_CfgNotAvailable = TranslateString(L"���� ������������ ����������");
	m_sError_ZeroChanSRV = TranslateString(L"��� ������� ������� ������");
	m_sError_NotSRV = TranslateString(L"��� ������� ������");
	m_sError_Help = TranslateString(L"������ ��� ������ ������� ���������");
	m_sInform_Starting = TranslateString(L"��������� ������ ��������");
	m_sInform_StartFromUnit = TranslateString(L"��������� �������� ����� UNIT");
	m_sInform_StartFromPanel = TranslateString(L"��������� �������� �� Z-������");
	m_sInform_ProgramWillBeClosed = TranslateString(L"��������� ����� �������");
	m_sInform_ProgramCompleted = TranslateString(L"��������� ��������� ������");
}
//------------------------------------------------------------------------------
CDialog_ZET::~CDialog_ZET()
{
	if (m_psrv)
	{
		delete m_psrv;
		m_psrv = nullptr;
	}

	if (m_punitSrv)
	{
		delete m_punitSrv;
		m_punitSrv = nullptr;
	}

	if (m_punitParent)
	{
		delete m_punitParent;
		m_punitParent = nullptr;
	}

	if (m_ppath)					// ������ ��������� ZetPath
	{
		delete m_ppath;
		m_ppath = nullptr;
	}

	if (m_ptooltip)
	{
		delete m_ptooltip;
		m_ptooltip = nullptr;
	}

	if (m_pEditForDigit)
	{
		delete[] m_pEditForDigit;
		m_pEditForDigit = nullptr;
	}

#ifdef _CAutoScaleXY
	if (m_pausc)	// ����� ���������������
	{
		delete m_pausc;
		m_pausc = nullptr;
	}
#endif

	if (m_pwinlog)
	{
		delete m_pwinlog;
		m_pwinlog = nullptr;
	}
}
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CDialog_ZET, BaseClass)
	//��������� � ��������� ������ ���� ����������� ������ ZETPanel
	ON_MESSAGE(WM_GET_HWND_OF_PANEL, OnGetHwndOfPanel)
	//������� Save-��������� � ��������� � � ZETPanel
	ON_MESSAGE(WM_SEND_STRUCTURE, OnSendStructure)
	//�������� � ����������� Save-���������
	ON_MESSAGE(WM_COPYDATA, OnReceiveStructureFromPanel)
	// ��������� ������� ������
	ON_MESSAGE(WM_ERROR_READ_SRV, _OnErrorReadData)
	ON_MESSAGE(WM_ERROR_CALC_SRV, _OnErrorCalcData)
	ON_MESSAGE(WM_END_JOB_SRV, OnEndSRV)
	ON_MESSAGE(WM_STOP_TIME_CHANNEL, OnStopTimeSRV)

	ON_MESSAGE(WM_UNIT2_APP, _OnReadyUnit2Param)
	ON_MESSAGE(WM_FILE_CFG_CHANGE, OnFileCfgChange)
	
	ON_WM_SHOWWINDOW()
	ON_WM_QUERYENDSESSION()
	ON_WM_ENDSESSION()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_WINDOWPOSCHANGING()
	ON_COMMAND(ID_HELP, OnHelp)		//������� �� F1

END_MESSAGE_MAP()
//------------------------------------------------------------------------------
BEGIN_EVENTSINK_MAP(CDialog_ZET, BaseClass)
	ON_EVENT(CDialog_ZET, IDC_SRV, 1, _OnModifySRV, VTS_I4)
	ON_EVENT(CDialog_ZET, IDC_SRV, 2, _OnFileModeSRV, VTS_I4)
	ON_EVENT(CDialog_ZET, IDC_SRV, 3, _OnStartFileSRV, VTS_I4)
	ON_EVENT(CDialog_ZET, IDC_SRV, 4, _OnSetTimeSRV, VTS_R4)
	ON_EVENT(CDialog_ZET, IDC_UNIT_PARENT, 1, _OnReadyUnitParent, VTS_I4)
END_EVENTSINK_MAP()
//------------------------------------------------------------------------------
// ������� ��� ����� ����� � �������� �� �����������
void CDialog_ZET::GetCommonDir()
{	// ����� DirConfig --------------------------------------------------
	m_sFullFolderCFG = ReadPathFolder(L"DirConfig");

	// ����� DirHelp � ���� ������� -------------------------------------
	CString strTmp = ReadPathFolder(L"DirHelp");

	bool bNotRu(false);
	CString sLang = GetLanguageDst();
	int m = sLang.Find(L"-");
	if (m != -1)
	{
		sLang.SetAt(m, 0);
		sLang.ReleaseBuffer();
		bNotRu = sLang != L"ru";
	}
	m_sFullNameFileCHM = strTmp + m_sExeName;
	if (bNotRu)
		m_sFullNameFileCHM += L"_" + sLang;
	m_sFullNameFileCHM += L".chm";

	m_bFileHelp = zfFileExists(m_sFullNameFileCHM) == 1;
	if (!m_bFileHelp)
	{
		m_sFullNameFileCHM = strTmp + L"HELP_ZETLAB";
		if (bNotRu)
			m_sFullNameFileCHM += L"_" + sLang;
		m_sFullNameFileCHM += L".chm";

		m_bFileHelp = zfFileExists(m_sFullNameFileCHM) == 1;
		if (m_bFileHelp)
			m_sFullNameFileCHM += L"::" + m_sExeName + L".htm";
	}

	strTmp = L"tmp.tmp";
	// ����� DirSignal -------------------------------------------------
	m_sFullFolderSignal = ReadPathFolder(L"DirSignal");
	if (!CheckFileAccess(m_sFullFolderSignal + strTmp))
		MessageError(m_sFolder + L" DirSignal " + m_sNotAvailable, 0);

	// ����� DirCompressed -------------------------------------------------
	m_sFullFolderCompressed = ReadPathFolder(L"DirCompressed");
	if (!CheckFileAccess(m_sFullFolderCompressed + strTmp))
		MessageError(m_sFolder + L" DirCompressed " + m_sNotAvailable, 0);

	// ����� DirResult --------------------------------------------------
	m_sFullFolderResult = ReadPathFolder(L"DirResult");
	if (!CheckFileAccess(m_sFullFolderResult + strTmp))
		MessageError(m_sFolder + L" DirResult " + m_sNotAvailable, 0);

	// ����� DirCorrect � InstallLocation -------------------------------
	// ����� ��������� ����������� ����� ���� ������ � �� ����� tmp.tmp
	// ������, �.�. ���� ZetLab ���������� � "Program Files", ��
	// ��������� �������. � ��� ����� ������������� ������ ������ ������.
	m_sFullFolderCorrect = ReadPathFolder(L"DirCorrect");

	// ����� InstallLocation ---------------------------------------------
	m_sFullFolderInstLoc = ReadPathFolder(L"InstallLocation");

	// ����� %SystemRoot%. ������� ���� �������� ������ ������� ������
	size_t size(0);
	if (_wgetenv_s(&size, NULL, 0, L"SystemRoot") == 0)
	{
		if (size > 0)
		{// ������ �������, ��������� ���� ������
			wchar_t *text = new wchar_t[size];
			if (_wgetenv_s(&size, text, size, L"SystemRoot") == 0)
				m_sSystemRoot = (CString)text + L"\\";
			delete[] text;
			text = nullptr;
		}
	}
}
//------------------------------------------------------------------------------
// ����������� ���� ������� ���������, �.�. ��������� ������:
// m_bJobManual;		��������� �������� ����������
// m_bJobUnit;			��������� �������� UNIT-1
// m_bJobUnit2;			��������� �������� UNIT-2
// m_bJobPanel;			��������� �������� � ������� ������� Zet-������
// �������� ��������������� ���� ������� ���-��.
//
void CDialog_ZET::DefineStartFlags()
{	// ������� ���������� �� ���� �������:
	// ���� � ��������� ������ ������ ����������� ������ ����
	CString sCommandLine = GetCommandLineW();
	int nArgs(0);
	LPWSTR * pArglist = CommandLineToArgvW(sCommandLine, &nArgs);
	if (nArgs > 1)
	{
		for (int i = 1; i < nArgs; ++i)
		{
			if (0 == wmemcmp(pArglist[i], g_sCreate, wcslen(g_sCreate)))
			{
				HWND hWndCreator(NULL);
				if ((2 == swscanf_s(pArglist[i], g_sCreateFormat, &hWndCreator, &nArgs))
					&& (hWndCreator != NULL))
					::PostMessageW(hWndCreator, WM_SEND_HWND, (WPARAM)m_hWnd, (LPARAM)nArgs);
				break;
			}
		}
	}
	LocalFree(pArglist);

	// ����������� ������ ------------------------------------------------------------
	m_punitParent = new CDUnit();
	if (!m_punitParent->Create(L"", WS_CHILD, CRect(0, 0, 0, 0), this, IDC_UNIT_PARENT))
		MessageError(m_sError_ConnectToUNIT, 0);

	if (m_punitParent->UnitReg((ULONG)m_hWnd) == 0)
	{	// �������� ����� UNIT
		m_bJobUnit = true;
		m_bJobManual = false;
		m_bJobPanel = false;
		m_bJobUnit2 = false;
		m_bEnableInterface = false;
	}
	else
	{	// �������� �� ����� UNIT, ������ m_punitParent
		m_punitParent->UnitUnReg((ULONG)m_hWnd);
		m_punitParent->DestroyWindow();
		delete m_punitParent;
		m_punitParent = nullptr;

		m_bJobUnit = false;
		
		if (sCommandLine.Find(L"-z") != -1)
		{	// �������� � ������� ������� Z-������
			m_bJobPanel = true;
			m_bJobManual = false;
			m_bJobUnit2 = false;
		}
		else if (sCommandLine.Find(L"-u") != -1)
		{	// �������� ����� UNIT-2
			m_bJobUnit2 = true;
			m_bJobManual = false;
			m_bJobPanel = false;
			m_bEnableInterface = false;

			// ������ m_punitSrv
			m_punitSrv = new CDUnitSrv();
			bool bNoError = m_punitSrv->Create(L"", WS_CHILD, CRect(0, 0, 0, 0), this,
				IDC_UNIT_Srv) != FALSE;
			if (bNoError)
			{
				bNoError = m_punitSrv->Activate() == NOERROR;
				if (bNoError)
				{
					DWORD sizeMax;
					bNoError = m_punitSrv->GetSizeMaxParam(&sizeMax) == NOERROR;
					if (bNoError)
						bNoError = m_paramUnit2.TestSize(sizeMax) == NOERROR;
				}
			}
			if (!bNoError)
			{
				MessageError(m_sError_ConnectToUNIT + L"-2", 0);
				m_punitSrv->DisActivate();
				delete m_punitSrv;
				m_punitSrv = nullptr;
			}
		}
		else
		{	// ������ ����� ������
			m_bJobManual = true;
			m_bJobPanel = false;
			m_bJobUnit2 = false;
			m_bJobHide = sCommandLine.Find(L"-hide") != -1;
		}
	}
}
//------------------------------------------------------------------------------
// ��������� ��� ������ ������� ���������.
// ��� ����� ��������� �������� �� ��� UNIT � �� � ������� Z-������, �.�.
// ������� ������ ���������� ���������, � ������� Z-������, ExeStarter-�� � �.�.
// � ���� ������ �� ������ ��������� ������������ CFG-����, � �� ����
// ������������ ��������� ��������� � ��������� ����.
//
void CDialog_ZET::SetForStart_Manual()
{
	CString str;
	

	// ����� ��������� 
	ztGetNumberSelfExe(&m_numberSelfExe);	// ��� ������
	m_nOrder = (long)m_numberSelfExe;		// ����� ���������

	if (pSaveData != nullptr)
	{
		CustomSaveStructure * pcss = (CustomSaveStructure*)pSaveData;
		pcss->numberProgram = m_numberSelfExe;

		ztStringCopyW(pcss->nameProgram, SIZE_NAME_PROGRAM, m_sExeName);
		ztStringAppendW(pcss->nameProgram, SIZE_NAME_PROGRAM, L".exe");
	}

	str.Format(L" " + m_sNumber + L"%02d. ", m_nOrder);
	// m_sCommentAndOrder ���� ���������� � ������������ ���������
	m_sCommentAndOrder += str;

	// �� ������ ��������� ��������� ��������� �� ���������
	OnStructureDefault();

	// ������� ��������� Default � ��������� New
	memcpy_s(pSaveDataNew, m_sizeSaveData, pSaveData, m_sizeSaveData);

	m_sNameAndOrder.Format(L"%s%02d", m_sExeName.GetString(), m_nOrder);
	
	CString sAltConfig(L"");
	CString sCommandLine = GetCommandLine();
	int nArgs;
	LPWSTR *szArglist = CommandLineToArgvW(sCommandLine, &nArgs);
	if (nullptr != szArglist)
	{
		for (int i = 1; i < nArgs; ++i)
		{
			CString sParam = szArglist[i];
			if (sParam == L"-loadconfig")
			{
				if (++i < nArgs)
				{
					m_bLoadAltConfig = true;
					sAltConfig = szArglist[i];
				}
			}
		}
	}

	if (!m_bLoadAltConfig)
		m_sFullNameFileCFG = m_sFullFolderCFG + m_sNameAndOrder + L".cfg";
	else
		m_sFullNameFileCFG = sAltConfig;

	m_bExistFileCfg =  //���� ��� �������� ������������� �����
		(zfFileExistsW(m_sFullNameFileCFG) == 1);

	m_bReadCFGFile =	// ���� �����
		m_bExistFileCfg &&
		ReadCFGFile(); 

	if (m_bLoadAltConfig)
		m_sFullNameFileCFG.Empty();

	if (m_bReadCFGFile)
	{	// ������ �������, �� �������� ����� ���������� �������������,
		// �.�. ����� ���������� ������� OnPostInitDialog �����
		// ��������� ��������� �� �FG-�����, ������� � pSaveDataNew
		m_typeNewParam = param_cfg_file;
		SetTimer(m_idTimerStart, m_ElapseTimerStart, NULL);
	}
	else
	{
		SetWindowPosDefault(m_numberSelfExe, m_crc32ByNameExe);	// ��������� �� ���������
		OnStructureSave();		// �� ��� ���������
		WriteCFGFile();			// � �������� � CFG
	}

	CreateThreadWatchCFG();
}
//------------------------------------------------------------------------------
void CDialog_ZET::CreateThreadWatchCFG()
{	// �������� �� ����������� CFG-�����
	m_DataForWatchCFG.handle[1] =
		m_hEventEnd = CreateEventW(	// ������ ��������� ������� ��� ���������� ������
		NULL,	// ������ �� ���������
		TRUE,	// � ������ �������
		FALSE,	// � ������� ���������
		NULL);	// ��� �����

	m_DataForWatchCFG.hWnd = m_hWnd;
	m_DataForWatchCFG.pFileName = &m_sFullNameFileCFG;
	m_DataForWatchCFG.handle[0] =
		m_hWatchCFG = FindFirstChangeNotification(m_sFullFolderCFG,
		false, FILE_NOTIFY_CHANGE_LAST_WRITE);
	if (CheckFileAccess(m_sFullNameFileCFG))
	{
		GetFileAttributesEx(m_sFullNameFileCFG,
			GetFileExInfoStandard, &m_DataForWatchCFG.attrib);
		m_DataForWatchCFG.lastWriteTime = m_DataForWatchCFG.attrib.ftLastWriteTime;
		if (m_hWatchCFG != INVALID_HANDLE_VALUE)
			m_pThreadWatchCFG = AfxBeginThread(GlobFunctionWatchCFG, &m_DataForWatchCFG);
	}
	else
		MessageError(m_sError_CfgNotAvailable, 0);
}
//------------------------------------------------------------------------------
// ��������� ��� ������� ��������� ��� UNIT
void CDialog_ZET::SetForStart_Unit()
{	// � ���� ������ ��������� �� ������ ������ � �����, ��� ������ ������
	// ���� �����, �� ���� �� ������ ������
	m_nOrder = 0;
	m_numberSelfExe = 0;

	OnStructureDefault();
	memcpy_s(pSaveDataNew, m_sizeSaveData, pSaveData, m_sizeSaveData);
	// ����� �� UNIT ����� ��������� ��������� ���������.
}
//------------------------------------------------------------------------------
// ��������� ��� ������� ��������� � ������� ������� Z-������
void CDialog_ZET::SetForStart_Zproj()
{	// �� ������ ������
	m_nOrder = 0;
	m_numberSelfExe = 0;

	OnStructureDefault();
	memcpy_s(pSaveDataNew, m_sizeSaveData, pSaveData, m_sizeSaveData);
	// ����� �� ������ ����� CFG � ������� ���������, � ����������� �
	// ���������� ����
}
//------------------------------------------------------------------------------
// ����������� ��������� CDialogZET
BOOL CDialog_ZET::OnInitDialog()
{
	BaseClass::OnInitDialog();
	TranslatePhrases();		// ����������� ������� ��� �������� ���� ���������

	// ��� ���� � ����� ����������������
	m_bNeedReSize = (GetStyle() & WS_SIZEBOX) != 0;

	// �������� ������ ������ -------------------------------------------
	m_pwinlog = new CDWindowslog();
	try
	{
		if (!m_pwinlog->Create(L"", WS_CHILD, CRect(0, 0, 0, 0), this, IDC_WINLOG))
			_MessageToZETPanel(WM_SEND_ERROR);
	}
	catch (...)
	{
		m_pwinlog->DestroyWindow();
		delete m_pwinlog;
		m_pwinlog = nullptr;
	}

	// ������ � �������� ����� ZetPath ----------------------------------
	m_ppath = new CDZetPath();
	if (!m_ppath->Create(L"", WS_CHILD, CRect(0, 0, 0, 0), this, IDC_ZETPATH))
	{
		MessageError(m_sError_CreateClass + L" CDZetPath", 0);
		m_ppath->DestroyWindow();
		delete m_ppath;
		m_ppath = nullptr;
	}

	// ������������ � ������� ������ ------------------------------------
	if (m_bNeedSRV)
	{
		long code = CreateSRV(m_NumVirtChan, m_bNeedDAC, m_NumDAC);
		if (code != 0)
			MessageError(m_sError_ConnectToSRV, 0, code);
	}

	// CToolTip
	m_ptooltip = new CToolTipCtrl();
	m_ptooltip->Create(this);
	m_ptooltip->Activate(1);

	// CAutoScaleXY
#ifdef _CAutoScaleXY
	m_pausc = new CAutoScaleXY();
	if (!m_pausc->Create(this))
		MessageError(m_sError_CreateClass + L" CAutoScaleXY", 0);
#endif

	GetCommonDir();
	DefineStartFlags();

	if (m_bJobUnit || m_bJobUnit2)
	{// UNIT (1 ��� 2)
		SetForStart_Unit();
	}
	else if (m_bJobPanel)
	{// � ������� ������� Z-������
		SetForStart_Zproj();
	}
	else
	{// �� ��������� - �������� ���������� 
		SetForStart_Manual();
	}
	
	m_bInterruptSRV = true;
	return TRUE;
}
//------------------------------------------------------------------------------
void CDialog_ZET::SetWindowPosDefault(
	WORD num,		// ����� ���������
	DWORD code)		// ���, ������������ ���������
{
	RECT rect = { 0 };
	GetWindowRect(&rect);

	int cx_screen = GetSystemMetrics(SM_CXSCREEN);
	int cy_screen = GetSystemMetrics(SM_CYSCREEN);
	int x0 = cx_screen / 20;
	int y0 = cy_screen / 10;
	int xe = cx_screen - 2 * x0 - (rect.right - rect.left);
	int ye = cy_screen - 2 * y0 - (rect.bottom - rect.top);

	int dx = GetSystemMetrics(SM_CXMIN) / 2;
	int dy = (3 * GetSystemMetrics(SM_CYMIN)) / 2;

	int xn = (num - 1) * dx + LOWORD(code);
	if (xn > xe)
	{
		if (xe != 0)
			xn -= (xn / xe) * xe;
		else
			xn = 0;
	}
	
	int yn = (num - 1) * dy + HIWORD(code);
	if (yn > ye)
	{
		if (ye != 0)
			yn -= (yn / ye) * ye;
		else
			yn = 0;
	}
	
	SetWindowPos(nullptr, x0 + xn, y0 + yn, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}
//------------------------------------------------------------------------------
// ��������� ������ ������ ������� �� ���������.
// ����� ������ ������������ �� ������ ��������� number.
// ������������ �������� 0 - ������ �������, ����� ��� ������
long CDialog_ZET::GetDefaultChannel(long number, long & chan)
{
	long ret = (m_psrv != nullptr) ? 0 : Err_NULL_Pointer;
	if (ret == 0)
	{
		chan = number;
		if (chan > 0)
		{
			long me = m_psrv->QuanChan();
			--chan;
			if (chan >= me)
				chan -= (chan / me) * me;
		}
	}
	else
		chan = 0;
	return ret;
}
//------------------------------------------------------------------------------
// ��������� ������ ������ ������� �� ��������� � ��� ID.
// ����� ������ ������������ �� ������ ��������� number.
// ������������ �������� 0 - ������ �������, ����� ��� ������
long CDialog_ZET::GetDefaultChannel(long number, long & chan, long & id)
{
	long ret = GetDefaultChannel(number, chan);
	if (ret == 0)
		m_psrv->IdChan(chan, &id);
	else
		id = 0;
	return ret;
}
//------------------------------------------------------------------------------
// ��������� ������ ������ ������� �� ��������� � ��� GUID.
// ����� ������ ������������ �� ������ ��������� number.
// ������������ �������� 0 - ������ �������, ����� ��� ������
long CDialog_ZET::GetDefaultChannel(long number, long & chan, GUID & guid)
{
	long ret = GetDefaultChannel(number, chan);
	if (ret == 0)
		m_psrv->IdChan(chan, &guid);
	else
		guid = GUID_NULL;
	return ret;
}
//------------------------------------------------------------------------------
/*�������� � ����� OnInitDialog ���������������� ���������.
��� ���������� ���� ������� ������� ON_MODIFYSVR �� ��������������*/
void CDialog_ZET::OnPostInitDialog(bool bNeedAutoScaleXY, bool bNeedZetPath)
{
	if (m_bJobUnit || m_bJobUnit2)
		MessageInformation(m_sInform_StartFromUnit);
	else if (m_bJobPanel)
		MessageInformation(m_sInform_StartFromPanel);
	else
		MessageInformation(m_sInform_Starting);

	if (m_bNeedSRV)
	{
		CString str;
		str.Format(m_sFreqADC + L" = %.2f " + m_sHz, m_FreqADC);
		MessageInformation(str);
	}
	// �������������� ��������� � �������� ���� �� ������ �� CFG -------------------------
	CRect rect;
	bool bPlacement = m_bJobManual && m_bPlacementCFG && m_bReadCFGFile;
	if (m_bNeedReSize)
	{// �������������� ����
		if (bPlacement)
			((CustomSaveStructure *)pSaveDataNew)->SetPlacement(m_hWnd);
	}
	else
	{// ���� � ������� 492 ���
		if (bPlacement)
		{	// ��� ����� ���� ���������� ������� ������ ��������� � ��������,
			// ����� ���� "�����" ������� � CFG, ���� � ���� ����� �������
			// pSaveData - ����� OnStructureDefault
			// pSaveDataNew - ����� ReadCFGFile
			RECT *pRectCur = &((CustomSaveStructure *)pSaveData)->wPlace.rcNormalPosition;
			RECT *pRectNew = &((CustomSaveStructure *)pSaveDataNew)->wPlace.rcNormalPosition;

			long cyCur = pRectCur->bottom - pRectCur->top;
			long cyNew = pRectNew->bottom - pRectNew->top;

			if ((cyCur > 0) && (cyNew > 0) && (cyCur == cyNew))
				((CustomSaveStructure *)pSaveDataNew)->SetPlacement(m_hWnd);
		}

		GetWindowRect(&rect);
		if (rect.Width() != WidthWin)
			MoveWindow(rect.left, rect.top, WidthWin, rect.Height());
	}

	// ����, ��� ������������� ��������� -------------------------------------
	m_bProgramInitialized = true;

	// ������������������� ��������� ������ ���� --------------------------------
	// �����������, �.�. � ������ ����� ���� ������ ������� ����� 100%.
	// SendMessage, �.�. ����� ������� ��������������� � ���������� �������
	// OnSize � ��������� � ����������.
	bool bNeedReSize = m_bNeedReSize;
	m_bNeedReSize = true;			// ����� ��������� ��������� ������� OnSize
	GetClientRect(&rect);
	SendMessage(WM_SIZE, SIZE_RESTORED,	
				MAKEWPARAM((WORD)rect.Width(), (WORD)rect.Height()));
	m_bNeedReSize = bNeedReSize;	// ������� ����� ��� ��������

	// �������� �� ������������ ������� ---------------------------------------
#ifdef _CAutoScaleXY
	if (!bNeedAutoScaleXY && (m_pausc != nullptr))
	{
		delete m_pausc;
		m_pausc = nullptr;
	}
#endif

	if ((!bNeedZetPath) && (m_ppath != nullptr))
	{
		m_ppath->DestroyWindow();
		delete m_ppath;
		m_ppath = nullptr;
	}

}
//------------------------------------------------------------------------------
void CDialog_ZET::_MessageToZETPanel(UINT wm)
{
	if (m_panelhwnd == 0)
	{
		CWnd* pwnd = FindWindowW(NULL, L"ZETLab");
		if (pwnd)
			m_panelhwnd = pwnd->m_hWnd;
	}
	if (m_panelhwnd != 0)	
		::PostMessageW( m_panelhwnd, wm, (WPARAM)(m_hWnd), 0 );
}
//------------------------------------------------------------------------------
void CDialog_ZET::MessageError(CString str, long id, long kod)
{
	if (kod != 0)
	{
		CString s;
		s.Format(L"%d", kod);
		ztMessageError(str + m_sCodeError + s, kod);
	}
	else
		ztMessageError(str);
	_MessageToZETPanel(WM_SEND_ERROR);
}
//------------------------------------------------------------------------------
void CDialog_ZET::MessageWarning(CString str, long id, long kod)
{
	if (kod != 0)
	{
		CString s;
		s.Format(L"%d", kod);
		ztMessageWarning(str + m_sCodeError + s, kod);
	}
	else
		ztMessageWarning(str);
	_MessageToZETPanel(WM_SEND_WARNING);
}
//------------------------------------------------------------------------------
void CDialog_ZET::MessageInformation(CString str)
{
	ztMessageInformation(str);
}
//------------------------------------------------------------------------------
long CDialog_ZET::CreateSRV(long numVirtChan, bool needDAC, long numDAC)
{
	long code(0);
	try
	{
		m_psrv = new CDSRV();
		if ( m_psrv->Create(L"", WS_CHILD, CRect(0,0,0,0), this, IDC_SRV) )
		{
			if (numVirtChan)
				code = m_psrv->ConnectVrtCh(numVirtChan);
			else if (needDAC)
				code = m_psrv->ConnectDac(numDAC);
			else
				code = m_psrv->Connect();

			if (code == 0)
			{
				m_FreqADC = m_psrv->Frequency(0);
				m_TimeADC = 1.f / m_FreqADC;
			}
			// �������� �� ������� ������� ������� �����, �.�. ���� �������
		}
		else
		{
			delete m_psrv;
			m_psrv = nullptr;
			code = -13;
		}
	}
	catch (CMemoryException*)
	{
		code = -13;		
	}
	return code;
}
//------------------------------------------------------------------------------
void CDialog_ZET::_OnModifySRV(long GlobalPar)
{
	if ( m_bProgramInitialized && m_bInterruptSRV )
	{
		SetTimer(m_idTimerModify, m_ElapseTimerModify, NULL);
		if (!m_bModifySRV)
		{// ������ Modify
			m_bModifySRV = true;
			OnFirstModifySRV_0();
		}
	}
}
//------------------------------------------------------------------------------
void CDialog_ZET::_OnFileModeSRV(long GlobalPar)
{
	if ( m_bProgramInitialized && m_bInterruptSRV )
		OnFileModeSRV(GlobalPar);
}
//------------------------------------------------------------------------------
void CDialog_ZET::_OnStartFileSRV(long GlobalPar)
{
	if ( m_bProgramInitialized && m_bInterruptSRV )
		OnStartFileSRV(GlobalPar);
}
//------------------------------------------------------------------------------
void CDialog_ZET::_OnSetTimeSRV(float GlobalPar)
{
	if ( m_bProgramInitialized && m_bInterruptSRV )
		OnSetTimeSRV(GlobalPar);
}
//------------------------------------------------------------------------------
void CDialog_ZET::OnStructureNewIfInit()
{
	if (m_bProgramInitialized)
		OnStructureNew();
	else
		SetTimer(m_idTimerStart, m_ElapseTimerStart, NULL);
}
//------------------------------------------------------------------------------
void CDialog_ZET::OnStructureNew()
{
	WriteCFGFile();
}
//------------------------------------------------------------------------------
LRESULT CDialog_ZET::OnReceiveStructureFromPanel(WPARAM wParam, LPARAM lParam)
{// WM_COPYDATA
	LRESULT ret;
	PCOPYDATASTRUCT vd;
	vd = (PCOPYDATASTRUCT)lParam;
	
	//�������� �� ������������ �����������
	if( vd->cbData == m_sizeSaveData )
	{
		//�������� ��������� � ��������� New
		memcpy_s( pSaveDataNew, m_sizeSaveData,
				  (void*)vd->lpData, m_sizeSaveData );

		CustomSaveStructure * css = (CustomSaveStructure *)pSaveDataNew;
		m_nOrder = (long)css->numberProgram;
		css = (CustomSaveStructure *)pSaveData;
		css->numberProgram = (uint32_t)m_nOrder;
		m_numberSelfExe = (WORD)(UINT)m_nOrder;

		// ����� ������� OnStructureNew - ����� ��� �� ������� �����
		m_typeNewParam = param_zet_project;
		OnStructureNewIfInit();
		ret = 1;
		m_bReadCFGFile = true;
	}
	else
	{
		ret = 0;
		m_bReadCFGFile = false;
	}
	return ret;
}
//------------------------------------------------------------------------------
LRESULT CDialog_ZET::OnSendStructure(WPARAM wParam, LPARAM lParam)
{// WM_SEND_STRUCTURE
	OnStructureSave();
	auto hWnd = GetSafeHwnd();
	if (m_bPlacementCFG)
	{
		CustomSaveStructure * pcss = (CustomSaveStructure *)pSaveData;
		pcss->GetPlacement(hWnd);
	}
	COPYDATASTRUCT data;
	data.cbData = m_sizeSaveData;
	data.dwData = m_nZPriority;
	data.lpData = pSaveData;
	return ::SendMessageW(m_panelhwnd, WM_COPYDATA,
		(WPARAM)hWnd, (LPARAM)&data);
}
//------------------------------------------------------------------------------
LRESULT CDialog_ZET::OnGetHwndOfPanel(WPARAM wParam, LPARAM lParam)
{// WM_GET_HWND_OF_PANEL
	m_panelhwnd = (HWND)wParam;
	return 1;
}
//------------------------------------------------------------------------------
//��������� ������ � �����, ��� ������������� � �.�.
bool CDialog_ZET::CheckFileAccess(CString fileName)
{	//������� ������ ������������ �����
	bool ret(false);
	FILE *pFile(nullptr);
	long retL = zfFileExists(fileName);

	if (retL == 1)
	{// ���� ����
		ret = zfOpenFile(fileName, L"r+b", &pFile) == 0;
		zfCloseFile(pFile);
	}
	else if (retL == 0)
	{// ����� ���
		ret = zfOpenFile(fileName, L"w+b", &pFile) == 0;
		zfCloseFile(pFile);
		DeleteFile(fileName);
	}
	//else
	//{// ������
	//}
	return ret;
}
//------------------------------------------------------------------------------
CString CDialog_ZET::ReadPathFolder(CString dir)
{
	BSTR bres = dir.AllocSysString();
	if (m_ppath)
	{
		CString sres = m_ppath->ZetPathW(&bres);
		SysFreeString(bres);
		return sres;
	}
	else
		return CString();
}
//------------------------------------------------------------------------------
// ����������� �������, ����� ��������������
void CDialog_ZET::OnStructureDefault()
{
	if (m_bPlacementCFG)
		((CustomSaveStructure *)pSaveData)->GetPlacement(m_hWnd);
}
//------------------------------------------------------------------------------
// ������ ������ �� ����������������� �����. ������� �����������, �
// ���������� �������������� ��� ��������, � ������� ������ CFG-����� ��
// �������� ����������.
bool CDialog_ZET::ReadCFGFile()
{
	OnPreReadCfg();

	DWORD sizeFileCfgInByte(0);
	bool ret =
		(0 == zfGetFileSizeInBytes(m_sFullNameFileCFG, &sizeFileCfgInByte)) &&
		(sizeFileCfgInByte == m_sizeSaveData);

	if (ret)
		ret = zfReadFileBinary(m_sFullNameFileCFG, pSaveDataNew, m_sizeSaveData) == 0;

//	// �������������� �������� �� CFG � OnPostInitDialog
// 	if (m_bPlacementCFG && ret)
// 	{
// 		CustomSaveStructure * pcss = (CustomSaveStructure *)pSaveDataNew;
// 		pcss->SetPlacement(m_hWnd);
// 	}
	OnPostReadCfg();

	return ret;
}
//------------------------------------------------------------------------------
// ������ ������ � ���������������� ����. ������� �����������, �
// ���������� �������������� ��� ��������, � ������� ������ CFG-����� ��
// �������� ����������.
bool CDialog_ZET::WriteCFGFile()
{
	if (m_bPlacementCFG)
	{
		CustomSaveStructure * pcss = (CustomSaveStructure *)pSaveData;
		pcss->GetPlacement(m_hWnd);
	}
	return zfWriteFileBinary(m_sFullNameFileCFG, pSaveData, m_sizeSaveData) == 0;
}
//------------------------------------------------------------------------------
void CDialog_ZET::OnHelp()
{
	if ( m_bFileHelp && !m_sSystemRoot.IsEmpty() )
	{	// ���� ���� ������� ��� ������
		STARTUPINFO si = { sizeof(si) };
		CString strComLine = m_sSystemRoot + L"hh.exe " + m_sFullNameFileCHM;
		// ��������� ���������� strComLine ����������� (��. �������)!!!
		if ( CreateProcessW(NULL, strComLine.GetBuffer(),
						NULL, NULL, false, 0, NULL, NULL, &si, &piHelp) )
			CloseHandle(piHelp.hThread);
		else
			MessageWarning(m_sError_Help, 0);
		strComLine.ReleaseBuffer();
	}
}
//------------------------------------------------------------------------------
// ������� ������ ���� ���� �� hWnd
BOOL CALLBACK CallBackFuncFindAllWindows(HWND hwnd, LPARAM lParam)
{	// ��������� ��� ����
	wchar_t text[MAX_PATH] = { 0 };
	std::set<HWND> *pset = (std::set<HWND>*)lParam;
	if (IsWindowVisible(hwnd) != FALSE)
		if (GetWindowTextW(hwnd, text, MAX_PATH) > 0)	// ���� � ����������	
			pset->insert(hwnd);		
	return TRUE;
}
//------------------------------------------------------------------------------
void CDialog_ZET::OnClose()
{// ������� ������� � ������, ������� ����
	OnStructureSave();
	// �������������� ���� � ��������� ����� �� ��������� ���� �������� ���� ������� WM_CLOSE
	// � ����� ������ � OnDestroy ��������� ���� �� ������ ���� ������� �� ������� ��� ����
	// ��� ����� ��������� ��� ��������� ���� ���� ������ � OnClose
	std::set<HWND> pset;	// HWND ��������� ����	
	EnumThreadWindows(GetCurrentThreadId(), CallBackFuncFindAllWindows, (LPARAM)&pset);
	for (auto hwnd : pset)
		if (hwnd != m_hWnd)	// ���������� ���� ����� ����� �������� ��������� ���������
			::PostMessageW(hwnd, WM_CLOSE, NULL, NULL);

	BaseClass::OnOK();
}
//------------------------------------------------------------------------------
void CDialog_ZET::OnCancel()
{// ���� ���� ������� �� ����� ��� ����� �����������, �� ���������
	// ����� ����������� �� ������� ������ Esc
	//CDialogEx::OnCancel();
}
//------------------------------------------------------------------------------
void CDialog_ZET::OnOK() 
{// ���� �����������������, �� ��� ������� ������ "Enter" ��������� �����������
	//CDialogEx::OnOK();
}
//------------------------------------------------------------------------------
BOOL CDialog_ZET::OnQueryEndSession()
{
	BOOL ret;
	if ( BaseClass::OnQueryEndSession() )
	{
		OnClose();
		ret = TRUE;
	}
	else
		ret = FALSE;
	return ret;
}
//------------------------------------------------------------------------------
void CDialog_ZET::OnEndSession(BOOL bEnding)
{
	BaseClass::OnEndSession(bEnding);
	OnClose();
}
//------------------------------------------------------------------------------
void CDialog_ZET::OnDestroy()
{
	// ������� ���������� ����� - �������� �� CFG
	SetEvent(m_hEventEnd);
	if (m_pThreadWatchCFG)
	{
		WaitForSingleObject(m_pThreadWatchCFG->m_hThread, 1000);
		m_pThreadWatchCFG = NULL;
	}
	FindCloseChangeNotification(m_hWatchCFG);
	CloseHandle(m_hEventEnd);

//	OnStructureSave() ������ � OnClose, �.�. ��� ������� ������ ����������
//	�� ������ OnDestroy ������ ����������.
//	������� ��������� ��������� ��������, ����� �� ����������!

	if (piHelp.hProcess)		// �������� ���� �������
	{
		TerminateProcess( piHelp.hProcess, 0 );
		CloseHandle(piHelp.hProcess);
	}

	if (m_bNeedSRV)				// ����������� �� �������
	{
		if (m_bNeedDAC)
			m_psrv->DisconnectDac(m_NumDAC);
		else if (m_NumVirtChan)
			m_psrv->DisconnectVrtCh();
		else
			m_psrv->Disconnect();
	}

	if (m_bJobManual)
	{
		if (m_bEnableChannelsSRV)
			WriteCFGFile();			// ��������� ������������
	}
	else if (m_bJobPanel)
	{
		if (m_panelhwnd)			// ������� � Zet-������
			::PostMessageW(m_panelhwnd, WM_SEND_CHILD_END, (WPARAM)m_hWnd, 0);
	}
	else if (m_bJobUnit)
	{							// ����������� �� UNIT
		if (m_punitParent)
			m_punitParent->UnitUnReg( (ULONG)m_hWnd );
	}
	else if (m_bJobUnit2)
	{
		if (m_punitSrv)
			m_punitSrv->DisActivate();
	}

	//
	if (m_pwinlog)
		MessageInformation(m_sInform_ProgramCompleted);

	BaseClass::OnDestroy();
}
//------------------------------------------------------------------------------
void CDialog_ZET::OnSizing(UINT wParam, LPRECT pRect)
{// ����������� ����������� �������� ����
	if (!m_bJobUnit && !m_bJobUnit2)
	{
		if (pRect->right - pRect->left < Width_Min)
		{
			pRect->left = m_rectOld.left;
			pRect->right = m_rectOld.right;
		}
		if (pRect->bottom - pRect->top < Height_Min)
		{
			pRect->top = m_rectOld.top;
			pRect->bottom = m_rectOld.bottom;
		}
		m_rectOld = *pRect;	// ��������� ���������� ���������
	}
}
//------------------------------------------------------------------------------
void CDialog_ZET::OnSize(UINT nType, int cx, int cy)
{
	#ifdef _CAutoScaleXY
	if ( m_bProgramInitialized && m_bNeedReSize && m_pausc &&
		( ( nType == SIZE_RESTORED ) || ( nType == SIZE_MAXIMIZED ) ) )
		m_pausc->AutoScale(cx, cy);
	#endif
}
//------------------------------------------------------------------------------
//		�������������� ������� ��������: m_idTimerStart � m_idTimerModify.
//
//		������ m_idTimerStart = 8, ����� 100 ���.
// �� ���� ��������� ��������, ����:
// - ��������� �������� ���������� � ������� ������ ���������������� ����;
// - ��������� �������� �� UNIT � �������� ���������;
// - ��������� �������� � ������� Z-������� � �� ������ �������� ���������.
// ������ m_idTimerStart ���������, ���� ��������� ��������� �������������,
// �.�. ���� m_bProgramInitialized = true.
//
//		������ m_idTimerModify = 9. ����� 300 ���
// �� ������������ ��� �������� ����� ������� ZetServer Modify(0).
//
void CDialog_ZET::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == m_idTimerStart)
	{
		if (m_bProgramInitialized)
		{
			KillTimer(m_idTimerStart);
			OnStructureNew();
		}
	}
	else if (nIDEvent == m_idTimerModify)
	{
		KillTimer(m_idTimerModify);
		m_bModifySRV = false;
		OnModifySRV();
	}
	BaseClass::OnTimer(nIDEvent);
}
//------------------------------------------------------------------------------
BOOL CDialog_ZET::PreTranslateMessage(MSG* pMsg)
{	
	if (pMsg->message == WM_CHAR)
	{
		if (m_NumEditForDigit)
		{
			HWND h = ::GetFocus();
			bool flag = false;
			for(int i= 0; i<m_NumEditForDigit; ++i)
			{
				if (m_pEditForDigit[i])
					flag = flag || ( h == m_pEditForDigit[i] );
			}
			if (flag)
			{
				int n = (int)pMsg->wParam;
				if (!(((n >= 48) && (n <= 57)) ||	// ����� �� 0 �� 9
					(n == 46) ||		// �����
					(n == 45) ||		// �����
					(n == 8) ||		// �����
					(n == 101)))	// "e" ���������� ��� 1.2e-3
					return TRUE;
			}
		}
	}

	if (m_ptooltip != nullptr)
		m_ptooltip->RelayEvent(pMsg);
	return BaseClass::PreTranslateMessage(pMsg);
}
//------------------------------------------------------------------------------
void CDialog_ZET::SetNumberEditForDigit(int num)
{
	if ( num != m_NumEditForDigit )
	{
		if (m_pEditForDigit)
		{
			delete[] m_pEditForDigit;
			m_pEditForDigit = nullptr;
		}
		m_NumEditForDigit = num;
		m_pEditForDigit = new HWND [num];

		ZeroMemory(m_pEditForDigit, num*sizeof(HWND));
	}
}
//------------------------------------------------------------------------------
void CDialog_ZET::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	if (( (m_bJobUnit || m_bJobUnit2) && !m_bEnableInterface) ||
		(m_bJobManual && m_bJobHide))
		lpwndpos->flags &= ~SWP_SHOWWINDOW;
	
	BaseClass::OnWindowPosChanging(lpwndpos);
}
//------------------------------------------------------------------------------
LRESULT CDialog_ZET::_OnErrorReadData(WPARAM wParam, LPARAM lParam)
{// WM_ERROR_READ_SRV - ������ ��� ������ ������ � �������
	OnErrorReadData((long)wParam, (long)lParam);
	return 0;
}
//------------------------------------------------------------------------------
LRESULT CDialog_ZET::_OnErrorCalcData(WPARAM wParam, LPARAM lParam)
{// WM_ERROR_CALC_SRV - ������ ��� �������� ������, ��������� � �������
	OnErrorCalcData((long)wParam, (long)lParam);
	return 0;
}
//------------------------------------------------------------------------------
LRESULT CDialog_ZET::OnEndSRV(WPARAM wParam, LPARAM lParam)
{// WM_END_JOB_SRV
	MessageError(m_sError_NotSRV + L". " + m_sInform_ProgramWillBeClosed, 0);
	PostMessageW(WM_CLOSE);
	return 0;
}
//------------------------------------------------------------------------------
LRESULT CDialog_ZET::OnStopTimeSRV(WPARAM wParam, LPARAM lParam)
{
	OnStopTimeChannel((long)wParam);
	return 0;
}
//------------------------------------------------------------------------------
void CDialog_ZET::_OnReadyUnitParent(long param)
{
	m_typeNewParam = param_unit_1;
	OnReadyUnitParent(param);
}
//------------------------------------------------------------------------------
LRESULT CDialog_ZET::_OnReadyUnit2Param(WPARAM wParam, LPARAM lParam)
{// ��������� ������� WM_UNIT2_APP - ��������� ���������
	if (m_punitSrv->GetParam(&m_paramUnit2) == NOERROR)
	{
		m_typeNewParam = param_unit_2;
		OnReadyUnit2Param();
	}
	return 0;
}
//------------------------------------------------------------------------------
LRESULT CDialog_ZET::OnFileCfgChange(WPARAM wParam, LPARAM lParam)
{
	bool bPlacementCFG = m_bPlacementCFG;
	m_bPlacementCFG = false;
	if (ReadCFGFile() &&
		(0 != memcmp(pSaveData, pSaveDataNew, m_sizeSaveData)))
	{
		m_typeNewParam = param_cfg_change;
		OnStructureNew();
	}
	m_bPlacementCFG = bPlacementCFG;
	return 0;
}
//------------------------------------------------------------------------------
void CDialog_ZET::OnShowWindow(BOOL bShow, UINT nStatus)
{
	BaseClass::OnShowWindow(bShow, nStatus);

	static int num = 0;
	static bool bUnitNoFirst = false;
	if (m_bJobUnit || m_bJobUnit2)
	{
		if ( ++num > 2 )
			bUnitNoFirst = true;

		if (bUnitNoFirst)
		{
			if (bShow)
				m_bEnableInterface = true;
			else
				m_bEnableInterface = false;
		}
	}
}
//------------------------------------------------------------------------------
void CDialog_ZET::ShowInterface(bool val)
{
	m_bEnableInterface = val;
	ShowWindow((int)val);
}
//------------------------------------------------------------------------------
void CDialog_ZET::CopyStringToClipboard(CString str, bool bDig)
{
	CString strTmp;
	if ( ::OpenClipboard(m_hWnd) && EmptyClipboard() )
	{
		strTmp = str;
		strTmp.TrimLeft();
		if (bDig)
		{
			wchar_t mas[2];	// 2 �.�. � ����� ������ ���� 0
			GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, mas, 2);
			if ( *mas != 46 )	// 46 - ��� �����
				strTmp.Replace(L".", (CString)mas);
		}
		size_t size = sizeof(TCHAR) * (1 + strTmp.GetLength());
		HGLOBAL hResult = GlobalAlloc(GMEM_MOVEABLE, size);
		if (hResult)
		{
			LPTSTR lptstrCopy = (LPTSTR)GlobalLock(hResult);
			if (lptstrCopy)
			{
				memcpy_s(lptstrCopy, size, strTmp.GetString(), size);
				GlobalUnlock(hResult);
				SetClipboardData(CF_UNICODETEXT, hResult);
				GlobalFree(hResult);
				hResult = nullptr;
			}
		}
	}
	CloseClipboard();
}
//----------------------------------------------------------------------
void CDialog_ZET::SetGraficColors(CWnd *cont)
{
	CString nameClass(L"");
	nameClass = (CString)cont->GetRuntimeClass()->m_lpszClassName;

#ifdef _Grid
	if (nameClass == L"CGridglctrl1")
// 	if ( ( nameClass == L"CGridglctrl1" ) ||
// 			( nameClass == L"CGridctrl1" ) )
	{
		CGridglctrl1* grid = (CGridglctrl1*)cont;
		grid->SetClrCrs(ZETColors::CursorColor);
		grid->SetClrDig(ZETColors::DigitsColor);
		grid->SetClrFon(ZETColors::BackgroundColor);
		grid->SetClrGrd(ZETColors::GridColor);
		grid->SetClrLeg(ZETColors::DigitsColor);
		grid->SetClrGrf(ZETColors::Graph_Colors[0]);
	}
#endif
#ifdef _Gramma
	if (nameClass == L"CGrammaglctrl1")
// 	if ( ( nameClass == L"CGrammaglctrl1" ) ||
// 				( nameClass == L"CGrammactrl1" ) )
	{
		CGrammaglctrl1* gram = (CGrammaglctrl1*)cont;
		gram->SetClrDig(ZETColors::DigitsColor);
		gram->SetClrFon(ZETColors::BackgroundColor);
		gram->SetClrLeg(ZETColors::DigitsColor);
		gram->SetClrGrd(ZETColors::GridColor);
	}
#endif
#ifdef _Plotter
	if ( nameClass == L"CPlotterxyctrl1" )
	{
		CPlotterxyctrl1* plot = (CPlotterxyctrl1*)cont;
		plot->SetClrCur(ZETColors::CursorColor);
		plot->SetClrNum(ZETColors::DigitsColor);
		plot->SetClrFon(ZETColors::BackgroundColor);
		plot->SetClrGrd(ZETColors::GridColor);
		plot->SetClrLeg(ZETColors::DigitsColor);
	}
#endif
#ifdef _Colscale
	if ( nameClass == L"CColscalectrl1" )
	{
		CColscalectrl1* colsc = (CColscalectrl1*)cont;
		colsc->SetClrDig(ZETColors::DigitsColor);
		colsc->SetClrFon(ZETColors::BackgroundColor);
		colsc->SetClrGrd(ZETColors::GridColor);
	}
#endif
}
//------------------------------------------------------------------------------