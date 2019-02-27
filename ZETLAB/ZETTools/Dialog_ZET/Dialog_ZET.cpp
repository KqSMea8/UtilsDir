//------------------------------------------------------------------------------
// CDialog_ZET.cpp : файл реализации
//
//------------------------------------------------------------------------------
//подключение стандартных заголовков 
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
				{// Файл был перезаписан
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
// конструктор диалогового окна CDialog_ZET
CDialog_ZET::CDialog_ZET(
	UINT nIDTemplate,			// идентификатор программы
	CWnd* pParent,				// указатель на владельца
	CString exeName,			// имя exe-файла программы
	CString cfgFilePreamble,	// имя преамбулы конфигурационного файла
	ULONG nZPriority,			// приоритет программы в Zet-панели (от 0 до 3)
	bool needSRV,				// нужен сервер
	long numVirtChan,			// кол-во создаваемых виртуальных каналов
	bool needDAC,				// нужен ЦАП
	long numDAC) :				// номер используемого ЦАП
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

	// имя программы ---------------------------------------------------
	m_sExeName.Preallocate(SIZE_NAME_PROGRAM);
	ztGetNameSelfExe(m_sExeName.GetBuffer(), SIZE_NAME_PROGRAM);
	m_sExeName.ReleaseBuffer();
	m_sCfgName = m_sExeName;
	// Код программы нужен для логера при всех типах запуска
	m_sCfgName.MakeLower();
	m_crc32ByNameExe = ztCRC32(m_sCfgName);
	m_sCfgName = m_sExeName;	// восстановление регистра букв

	// Словарь ----------------------------------------------------------
	InitZetDictionary(exeName);
	TranslatePhrasesThis();
	// ------------------------------------------------------------------
	//приоритет запуска программы (от 0 до 3 включительно)
	m_nZPriority = nZPriority;

	// переменные для работы с сервером данных
	m_bNeedSRV = needSRV;
	m_bNeedDAC = needDAC;
	m_NumVirtChan = numVirtChan;
	m_NumDAC = numDAC;
	
	// флаги режимов работы по умолчанию
	m_bJobManual = true;
	m_bJobUnit = false;
	m_bJobUnit2 = false;
	m_bJobPanel = false;
	m_bProgramInitialized = false;
	m_bEnableChannelsSRV = true;	// обязательно
	m_bInterruptSRV = false;		// обязательно
	m_bReadCFGFile = false;
	m_bEnableInterface = true;

	m_nOrder = 0;	// на всякий случай
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
	m_panelhwnd = 0;					//хэндл программной панели ZETPanel

	// rect для ограничения минимальных размеров окна
	m_rectOld.left = m_rectOld.right = m_rectOld.top = m_rectOld.bottom = 0;
}
//------------------------------------------------------------------------------
void CDialog_ZET::TranslatePhrasesThis()
{	// Переводимые фразы базового класса
	m_sCodeError = L". " + TranslateString(L"Код ошибки") + L" = ";

	m_sNumber = TranslateString(L"№");
	m_sHz = TranslateString(L"Гц");
	m_sFreqADC = TranslateString(L"Частота дискретизации АЦП");
	m_sFolder = TranslateString(L"Папка");
	m_sNotAvailable = TranslateString(L"недоступна");
	m_sError_ConnectToSRV = TranslateString(L"Ошибка при подключении к серверу данных");
	m_sError_ConnectToUNIT = TranslateString(L"Ошибка при подключению к UNIT");
	m_sError_CreateClass = TranslateString(L"Ошибка при создании экземпляра класса");
	m_sError_CfgNotAvailable = TranslateString(L"Файл конфигурации недоступен");
	m_sError_ZeroChanSRV = TranslateString(L"Нет каналов сервера данных");
	m_sError_NotSRV = TranslateString(L"Нет сервера данных");
	m_sError_Help = TranslateString(L"Ошибка при вызове справки программы");
	m_sInform_Starting = TranslateString(L"Программа начала работать");
	m_sInform_StartFromUnit = TranslateString(L"Программа запущена через UNIT");
	m_sInform_StartFromPanel = TranslateString(L"Программа запущена из Z-панели");
	m_sInform_ProgramWillBeClosed = TranslateString(L"Программа будет закрыта");
	m_sInform_ProgramCompleted = TranslateString(L"Программа завершила работу");
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

	if (m_ppath)					// удаляю экземпляр ZetPath
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
	if (m_pausc)	// класс масштабирования
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
	//сообщение о получении хэндла окна программной панели ZETPanel
	ON_MESSAGE(WM_GET_HWND_OF_PANEL, OnGetHwndOfPanel)
	//готовлю Save-структуру и отправляю её в ZETPanel
	ON_MESSAGE(WM_SEND_STRUCTURE, OnSendStructure)
	//принимаю и обрабатываю Save-структуру
	ON_MESSAGE(WM_COPYDATA, OnReceiveStructureFromPanel)
	// сообщения сервера данных
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
	ON_COMMAND(ID_HELP, OnHelp)		//справка по F1

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
// Задание имён общих папок и проверка их доступности
void CDialog_ZET::GetCommonDir()
{	// папка DirConfig --------------------------------------------------
	m_sFullFolderCFG = ReadPathFolder(L"DirConfig");

	// папка DirHelp и файл справки -------------------------------------
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
	// папка DirSignal -------------------------------------------------
	m_sFullFolderSignal = ReadPathFolder(L"DirSignal");
	if (!CheckFileAccess(m_sFullFolderSignal + strTmp))
		MessageError(m_sFolder + L" DirSignal " + m_sNotAvailable, 0);

	// папка DirCompressed -------------------------------------------------
	m_sFullFolderCompressed = ReadPathFolder(L"DirCompressed");
	if (!CheckFileAccess(m_sFullFolderCompressed + strTmp))
		MessageError(m_sFolder + L" DirCompressed " + m_sNotAvailable, 0);

	// папка DirResult --------------------------------------------------
	m_sFullFolderResult = ReadPathFolder(L"DirResult");
	if (!CheckFileAccess(m_sFullFolderResult + strTmp))
		MessageError(m_sFolder + L" DirResult " + m_sNotAvailable, 0);

	// папки DirCorrect и InstallLocation -------------------------------
	// здесь проверять доступность папок путём записи в неё файла tmp.tmp
	// нельзя, т.к. если ZetLab установлен в "Program Files", то
	// программа вылетит. В эту папку пользователям ничего писать нельзя.
	m_sFullFolderCorrect = ReadPathFolder(L"DirCorrect");

	// папка InstallLocation ---------------------------------------------
	m_sFullFolderInstLoc = ReadPathFolder(L"InstallLocation");

	// Папка %SystemRoot%. Вначале надо получить размер будущей строки
	size_t size(0);
	if (_wgetenv_s(&size, NULL, 0, L"SystemRoot") == 0)
	{
		if (size > 0)
		{// Размер получен, вычитываю саму строку
			wchar_t *text = new wchar_t[size];
			if (_wgetenv_s(&size, text, size, L"SystemRoot") == 0)
				m_sSystemRoot = (CString)text + L"\\";
			delete[] text;
			text = nullptr;
		}
	}
}
//------------------------------------------------------------------------------
// Определение типа запуска программы, т.е. стартовых флагов:
// m_bJobManual;		программа запущена оператором
// m_bJobUnit;			программа запущена UNIT-1
// m_bJobUnit2;			программа запущена UNIT-2
// m_bJobPanel;			программа запущена в составе проекта Zet-панели
// Создание соответствующих типу запуска осх-ов.
//
void CDialog_ZET::DefineStartFlags()
{	// Вначале независимо от типа запуска:
	// если в командной строке запрос дескриптора своего окна
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

	// Определение флагов ------------------------------------------------------------
	m_punitParent = new CDUnit();
	if (!m_punitParent->Create(L"", WS_CHILD, CRect(0, 0, 0, 0), this, IDC_UNIT_PARENT))
		MessageError(m_sError_ConnectToUNIT, 0);

	if (m_punitParent->UnitReg((ULONG)m_hWnd) == 0)
	{	// загрузка через UNIT
		m_bJobUnit = true;
		m_bJobManual = false;
		m_bJobPanel = false;
		m_bJobUnit2 = false;
		m_bEnableInterface = false;
	}
	else
	{	// загрузка не через UNIT, удаляю m_punitParent
		m_punitParent->UnitUnReg((ULONG)m_hWnd);
		m_punitParent->DestroyWindow();
		delete m_punitParent;
		m_punitParent = nullptr;

		m_bJobUnit = false;
		
		if (sCommandLine.Find(L"-z") != -1)
		{	// загрузка в составе проекта Z-панели
			m_bJobPanel = true;
			m_bJobManual = false;
			m_bJobUnit2 = false;
		}
		else if (sCommandLine.Find(L"-u") != -1)
		{	// загрузка через UNIT-2
			m_bJobUnit2 = true;
			m_bJobManual = false;
			m_bJobPanel = false;
			m_bEnableInterface = false;

			// создаю m_punitSrv
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
		{	// ручной режим работы
			m_bJobManual = true;
			m_bJobPanel = false;
			m_bJobUnit2 = false;
			m_bJobHide = sCommandLine.Find(L"-hide") != -1;
		}
	}
}
//------------------------------------------------------------------------------
// Установки при ручном запуске программы.
// Это когда программа запущена не под UNIT и не в составе Z-панели, т.е.
// запуски руками различными способами, с помощью Z-панели, ExeStarter-ом и т.д.
// В этом режиме по номеру программы определяется CFG-файл, а из него
// вычитываются параметры программы и положение окон.
//
void CDialog_ZET::SetForStart_Manual()
{
	CString str;
	

	// Номер программы 
	ztGetNumberSelfExe(&m_numberSelfExe);	// для логера
	m_nOrder = (long)m_numberSelfExe;		// номер программы

	if (pSaveData != nullptr)
	{
		CustomSaveStructure * pcss = (CustomSaveStructure*)pSaveData;
		pcss->numberProgram = m_numberSelfExe;

		ztStringCopyW(pcss->nameProgram, SIZE_NAME_PROGRAM, m_sExeName);
		ztStringAppendW(pcss->nameProgram, SIZE_NAME_PROGRAM, L".exe");
	}

	str.Format(L" " + m_sNumber + L"%02d. ", m_nOrder);
	// m_sCommentAndOrder надо определять в конструкторе программы
	m_sCommentAndOrder += str;

	// по номеру программы определяю параметры по умолчанию
	OnStructureDefault();

	// копирую структуру Default в структуру New
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

	m_bExistFileCfg =  //флаг для проверки существующего файла
		(zfFileExistsW(m_sFullNameFileCFG) == 1);

	m_bReadCFGFile =	// флаг нужен
		m_bExistFileCfg &&
		ReadCFGFile(); 

	if (m_bLoadAltConfig)
		m_sFullNameFileCFG.Empty();

	if (m_bReadCFGFile)
	{	// запуск таймера, по которому после завершения инициализации,
		// т.е. после выполнения функции OnPostInitDialog будут
		// применены параметры из СFG-файла, которые в pSaveDataNew
		m_typeNewParam = param_cfg_file;
		SetTimer(m_idTimerStart, m_ElapseTimerStart, NULL);
	}
	else
	{
		SetWindowPosDefault(m_numberSelfExe, m_crc32ByNameExe);	// положение по умолчанию
		OnStructureSave();		// всё это сохранить
		WriteCFGFile();			// и записать в CFG
	}

	CreateThreadWatchCFG();
}
//------------------------------------------------------------------------------
void CDialog_ZET::CreateThreadWatchCFG()
{	// следилка за изменениями CFG-файла
	m_DataForWatchCFG.handle[1] =
		m_hEventEnd = CreateEventW(	// создаю локальное событие для завершения потока
		NULL,	// защита по умолчанию
		TRUE,	// с ручным сбросом
		FALSE,	// в занятом состоянии
		NULL);	// без имени

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
// Установки при запуске программы под UNIT
void CDialog_ZET::SetForStart_Unit()
{	// В этом режиме программы не должны писать в логер, это должна делать
	// сама скада, но пока на всякий случай
	m_nOrder = 0;
	m_numberSelfExe = 0;

	OnStructureDefault();
	memcpy_s(pSaveDataNew, m_sizeSaveData, pSaveData, m_sizeSaveData);
	// Далее от UNIT будут приходить параметры программы.
}
//------------------------------------------------------------------------------
// Установки при запуске программы в составе проекта Z-панели
void CDialog_ZET::SetForStart_Zproj()
{	// На всякий случай
	m_nOrder = 0;
	m_numberSelfExe = 0;

	OnStructureDefault();
	memcpy_s(pSaveDataNew, m_sizeSaveData, pSaveData, m_sizeSaveData);
	// Далее от панели придёт CFG с номером программы, её параметрами и
	// положением окон
}
//------------------------------------------------------------------------------
// обработчики сообщений CDialogZET
BOOL CDialog_ZET::OnInitDialog()
{
	BaseClass::OnInitDialog();
	TranslatePhrases();		// виртуальная функция для перевода фраз программы

	// Тип окна в части масштабируемости
	m_bNeedReSize = (GetStyle() & WS_SIZEBOX) != 0;

	// виндовый журнал ошибок -------------------------------------------
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

	// работа с реестром через ZetPath ----------------------------------
	m_ppath = new CDZetPath();
	if (!m_ppath->Create(L"", WS_CHILD, CRect(0, 0, 0, 0), this, IDC_ZETPATH))
	{
		MessageError(m_sError_CreateClass + L" CDZetPath", 0);
		m_ppath->DestroyWindow();
		delete m_ppath;
		m_ppath = nullptr;
	}

	// подключаемся к серверу данных ------------------------------------
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
	{// UNIT (1 или 2)
		SetForStart_Unit();
	}
	else if (m_bJobPanel)
	{// в составе проекта Z-панели
		SetForStart_Zproj();
	}
	else
	{// Всё остальное - загрузка оператором 
		SetForStart_Manual();
	}
	
	m_bInterruptSRV = true;
	return TRUE;
}
//------------------------------------------------------------------------------
void CDialog_ZET::SetWindowPosDefault(
	WORD num,		// номер программы
	DWORD code)		// код, определяющий программу
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
// Получение номера канала сервера по умолчанию.
// Номер канала определяется по номеру программы number.
// Возвращаемое значение 0 - канала получен, иначе код ошибки
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
// Получение номера канала сервера по умолчанию и его ID.
// Номер канала определяется по номеру программы number.
// Возвращаемое значение 0 - канала получен, иначе код ошибки
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
// Получение номера канала сервера по умолчанию и его GUID.
// Номер канала определяется по номеру программы number.
// Возвращаемое значение 0 - канала получен, иначе код ошибки
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
/*Вызывать в конце OnInitDialog инициализируемой программы.
При отсутствии этой функции событие ON_MODIFYSVR не обрабатывается*/
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
	// восстановление положения и размеров окна по данным из CFG -------------------------
	CRect rect;
	bool bPlacement = m_bJobManual && m_bPlacementCFG && m_bReadCFGFile;
	if (m_bNeedReSize)
	{// масштабируемые окна
		if (bPlacement)
			((CustomSaveStructure *)pSaveDataNew)->SetPlacement(m_hWnd);
	}
	else
	{// окна с шириной 492 пкс
		if (bPlacement)
		{	// Для таких окон сохранённые размеры должны совпадать с текущими,
			// иначе либо "левые" размеры в CFG, либо у окна новые размеры
			// pSaveData - после OnStructureDefault
			// pSaveDataNew - после ReadCFGFile
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

	// флаг, что инициализация завершена -------------------------------------
	m_bProgramInitialized = true;

	// Автомастшабирование элементов внутри окна --------------------------------
	// Обязательно, т.к. у экрана может быть выбран масштаб более 100%.
	// SendMessage, т.к. нужно вызвать переопределённую в наследнике функцию
	// OnSize и дождаться её завершения.
	bool bNeedReSize = m_bNeedReSize;
	m_bNeedReSize = true;			// чтобы правильно сработала функция OnSize
	GetClientRect(&rect);
	SendMessage(WM_SIZE, SIZE_RESTORED,	
				MAKEWPARAM((WORD)rect.Width(), (WORD)rect.Height()));
	m_bNeedReSize = bNeedReSize;	// возврат флагу его значения

	// удаление не используемых классов ---------------------------------------
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
			// Проверку на наличие каналов сервера убрал, т.к. есть формула
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
		{// первый Modify
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
	
	//проверка на соответствие размерности
	if( vd->cbData == m_sizeSaveData )
	{
		//копируем параметры в структуру New
		memcpy_s( pSaveDataNew, m_sizeSaveData,
				  (void*)vd->lpData, m_sizeSaveData );

		CustomSaveStructure * css = (CustomSaveStructure *)pSaveDataNew;
		m_nOrder = (long)css->numberProgram;
		css = (CustomSaveStructure *)pSaveData;
		css->numberProgram = (uint32_t)m_nOrder;
		m_numberSelfExe = (WORD)(UINT)m_nOrder;

		// вызов функции OnStructureNew - сразу или по таймеру позже
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
//проверяем доступ к файлу, его существование и т.д.
bool CDialog_ZET::CheckFileAccess(CString fileName)
{	//создаем объект проверяемого файла
	bool ret(false);
	FILE *pFile(nullptr);
	long retL = zfFileExists(fileName);

	if (retL == 1)
	{// файл есть
		ret = zfOpenFile(fileName, L"r+b", &pFile) == 0;
		zfCloseFile(pFile);
	}
	else if (retL == 0)
	{// файла нет
		ret = zfOpenFile(fileName, L"w+b", &pFile) == 0;
		zfCloseFile(pFile);
		DeleteFile(fileName);
	}
	//else
	//{// ошибка
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
// Виртуальная функция, будет переопределена
void CDialog_ZET::OnStructureDefault()
{
	if (m_bPlacementCFG)
		((CustomSaveStructure *)pSaveData)->GetPlacement(m_hWnd);
}
//------------------------------------------------------------------------------
// Чтение данных из конфигурационного файла. Функция виртуальная, её
// необходимо переопределить для программ, у которых размер CFG-файла не
// является постоянным.
bool CDialog_ZET::ReadCFGFile()
{
	OnPreReadCfg();

	DWORD sizeFileCfgInByte(0);
	bool ret =
		(0 == zfGetFileSizeInBytes(m_sFullNameFileCFG, &sizeFileCfgInByte)) &&
		(sizeFileCfgInByte == m_sizeSaveData);

	if (ret)
		ret = zfReadFileBinary(m_sFullNameFileCFG, pSaveDataNew, m_sizeSaveData) == 0;

//	// Восстановление размеров из CFG в OnPostInitDialog
// 	if (m_bPlacementCFG && ret)
// 	{
// 		CustomSaveStructure * pcss = (CustomSaveStructure *)pSaveDataNew;
// 		pcss->SetPlacement(m_hWnd);
// 	}
	OnPostReadCfg();

	return ret;
}
//------------------------------------------------------------------------------
// Запись данных в конфигурационный файл. Функция виртуальная, её
// необходимо переопределить для программ, у которых размер CFG-файла не
// является постоянным.
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
	{	// если файл справки был найден
		STARTUPINFO si = { sizeof(si) };
		CString strComLine = m_sSystemRoot + L"hh.exe " + m_sFullNameFileCHM;
		// создавать переменную strComLine ОБЯЗАТЕЛЬНО (см. Рихтера)!!!
		if ( CreateProcessW(NULL, strComLine.GetBuffer(),
						NULL, NULL, false, 0, NULL, NULL, &si, &piHelp) )
			CloseHandle(piHelp.hThread);
		else
			MessageWarning(m_sError_Help, 0);
		strComLine.ReleaseBuffer();
	}
}
//------------------------------------------------------------------------------
// функция поиска всех окон по hWnd
BOOL CALLBACK CallBackFuncFindAllWindows(HWND hwnd, LPARAM lParam)
{	// перебираю все окна
	wchar_t text[MAX_PATH] = { 0 };
	std::set<HWND> *pset = (std::set<HWND>*)lParam;
	if (IsWindowVisible(hwnd) != FALSE)
		if (GetWindowTextW(hwnd, text, MAX_PATH) > 0)	// окно с заголовком	
			pset->insert(hwnd);		
	return TRUE;
}
//------------------------------------------------------------------------------
void CDialog_ZET::OnClose()
{// красный крестик в правом, верхнем углу
	OnStructureSave();
	// дополнительные окна у программы могут не закрыться если основное окно получит WM_CLOSE
	// в таком случае в OnDestroy основного окна не зайдет пока вручную не закрыть доп окна
	// для этого закрываем все созданные окна сами именно в OnClose
	std::set<HWND> pset;	// HWND найденных окон	
	EnumThreadWindows(GetCurrentThreadId(), CallBackFuncFindAllWindows, (LPARAM)&pset);
	for (auto hwnd : pset)
		if (hwnd != m_hWnd)	// отправляем всем окнам кроме главного сообщение закрыться
			::PostMessageW(hwnd, WM_CLOSE, NULL, NULL);

	BaseClass::OnOK();
}
//------------------------------------------------------------------------------
void CDialog_ZET::OnCancel()
{// если этой функции не будет или снять комментарий, то программа
	// будет закрываться по нажатию кнопки Esc
	//CDialogEx::OnCancel();
}
//------------------------------------------------------------------------------
void CDialog_ZET::OnOK() 
{// Если раскомментировать, то при нажатии кнопки "Enter" программа закрывается
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
	// вначале остановить поток - следилку за CFG
	SetEvent(m_hEventEnd);
	if (m_pThreadWatchCFG)
	{
		WaitForSingleObject(m_pThreadWatchCFG->m_hThread, 1000);
		m_pThreadWatchCFG = NULL;
	}
	FindCloseChangeNotification(m_hWatchCFG);
	CloseHandle(m_hEventEnd);

//	OnStructureSave() перенёс в OnClose, т.к. эта функция должна вызываться
//	до вызова OnDestroy класса наследника.
//	Вначале сохраняем состояние объектов, потом их уничтожаем!

	if (piHelp.hProcess)		// закрываю окно справки
	{
		TerminateProcess( piHelp.hProcess, 0 );
		CloseHandle(piHelp.hProcess);
	}

	if (m_bNeedSRV)				// отключаемся от сервера
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
			WriteCFGFile();			// сохраняем конфигурацию
	}
	else if (m_bJobPanel)
	{
		if (m_panelhwnd)			// сообщаю в Zet-панель
			::PostMessageW(m_panelhwnd, WM_SEND_CHILD_END, (WPARAM)m_hWnd, 0);
	}
	else if (m_bJobUnit)
	{							// отключаемся от UNIT
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
{// ограничение минимальных размеров окна
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
		m_rectOld = *pRect;	// запоминаю последнеее положение
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
//		Обрабатываются события таймеров: m_idTimerStart и m_idTimerModify.
//
//		Таймер m_idTimerStart = 8, время 100 сек.
// По нему программа стартует, если:
// - программа запущена оператором и успешно прочтёт конфигурационный файл;
// - программа запущена от UNIT и получены параметры;
// - программа запущена в составе Z-проекта и от панели получены параметры.
// Таймер m_idTimerStart взводится, если программа завершила инициализацию,
// т.е. если m_bProgramInitialized = true.
//
//		Таймер m_idTimerModify = 9. Время 300 сек
// Он предназначен для пропуска пачек событий ZetServer Modify(0).
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
				if (!(((n >= 48) && (n <= 57)) ||	// цифры от 0 до 9
					(n == 46) ||		// точка
					(n == 45) ||		// минус
					(n == 8) ||		// забой
					(n == 101)))	// "e" английская для 1.2e-3
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
{// WM_ERROR_READ_SRV - ошибка при чтении данных с сервера
	OnErrorReadData((long)wParam, (long)lParam);
	return 0;
}
//------------------------------------------------------------------------------
LRESULT CDialog_ZET::_OnErrorCalcData(WPARAM wParam, LPARAM lParam)
{// WM_ERROR_CALC_SRV - ошибка при обрабоке данных, считанных с сервера
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
{// Обработка события WM_UNIT2_APP - получение параметра
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
			wchar_t mas[2];	// 2 т.к. в конце должен быть 0
			GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, mas, 2);
			if ( *mas != 46 )	// 46 - это точка
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