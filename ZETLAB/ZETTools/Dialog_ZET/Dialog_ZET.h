//------------------------------------------------------------------------------
// CDialog_ZET.h : файл заголовка
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
// сообщения, с которыми работают программы ZETLab, и т.д. и т.п.
#include <ZetDef.h>
//------------------------------------------------------------------------------
// идентификаторы объектов, созданных в базовом классе динамически
// диапазон значений от 1700 до 1749
#define IDC_SRV			1700
#define IDC_ZETPATH		1701
#define IDC_WINLOG		1702
#define IDC_UNIT_PARENT	1703
#define IDC_MemoryLeak	1704
#define IDC_UNIT_Srv	1705
//------------------------------------------------------------------------------
// define с размерами окон "переехали" в ZetDef.h
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
// диалоговое окно CDialog_ZET
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

// Реализация	
private:
	bool m_bFileHelp;				//	есть файл справки программы
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

	
	//проверка  файла (папки) на доступность
	bool CheckFileAccess(CString fileName);
	long CreateSRV(long numVirtChan, bool needDAC, long numDAC);

	//объявление функций обработки сообщений панели
	LRESULT OnReceiveStructureFromPanel(WPARAM wParam, LPARAM lParam);
	//сохранение параметров в ZETProject
	LRESULT OnSendStructure(WPARAM wParam, LPARAM lParam);
	//получаем хэндл панели ZETPanel
	LRESULT OnGetHwndOfPanel (WPARAM wParam, LPARAM lParam);
	
	//сообщения сервера данных
	LRESULT _OnErrorReadData(WPARAM wParam, LPARAM lParam);
	LRESULT _OnErrorCalcData(WPARAM wParam, LPARAM lParam);
	LRESULT OnEndSRV(WPARAM wParam, LPARAM lParam);
	LRESULT OnStopTimeSRV(WPARAM wParam, LPARAM lParam);

	// сообщение UNIT-1
	void _OnReadyUnitParent(long param);

	// сообщение UNIT-2
	LRESULT _OnReadyUnit2Param(WPARAM wParam, LPARAM lParam);
	LRESULT OnFileCfgChange(WPARAM wParam, LPARAM lParam);

	// Задание имён общих папок и проверка их доступности
	void GetCommonDir();
	// Определение типа запуска программы, т.е. стартовых флагов.
	// Создание соответствующих типу запуска осх-ов.
	void DefineStartFlags();
	// Установки при ручном запуске программы
	void SetForStart_Manual();
	// Установки при запуске программы под UNIT
	void SetForStart_Unit();
	// Установки при запуске программы в составе проекта Z-панели
	void SetForStart_Zproj();
	// Создание следилки за изменениями CFG-файла
	void CreateThreadWatchCFG();


protected:
	// Переводимые фразы
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

	// флаги, описывающие режимы работы программы
	bool m_bJobManual;			// программа запущена оператором
	bool m_bJobHide;
	bool m_bJobUnit;			// программа запущена UNIT-1
	bool m_bJobUnit2;			// программа запущена UNIT-2
	bool m_bJobPanel;			// программа запущена в составе проекта Zet-панели
	bool m_bProgramInitialized;	// завершена инициализация программы
	bool m_bNeedReSize;			// у программы возможно изменение размеров окна
	bool m_bReadCFGFile;		// удачно считан конфигурационный файл
	bool m_bEnableInterface;	// требуется отображение главного окна
	bool m_bPlacementCFG;		// конфигурационный файл с данными о положении окна
	bool m_bLoadAltConfig;		// требуется альтернативные cfg-данные
	bool m_bExistFileCfg;		// Проверка на наличие файла cfg

	// флаги и переменные для работы с сервером данных
	bool m_bInterruptSRV;		// разрешена обработка событий ZetServer
	bool m_bModifySRV;			// идёт пачка событий Modify
	bool m_bEnableChannelsSRV;	// у ZetServer есть каналы
	bool m_bNeedSRV;			// программе требуется ZetServer
	bool m_bNeedDAC;			// программе требуются каналы ЦАП
	long m_NumVirtChan;			// кол-во виртуальных каналов, созданных программой
	long m_NumDAC;				// кол-во требуемых каналов ЦАП
	float m_FreqADC;			// частота дискретизации АЦП
	float m_TimeADC;			// период частоты дискретизации АЦП

	// имена файлов и папок
	CString m_sSystemRoot;		// системная папка
	CString m_sCommentAndOrder;	// описание программы с номером
	CString m_sCfgName;			// имя cfg-файла (без пути и расширения)
	CString m_sExeName;			// имя exe-программы (без пути и расширения)
	CString m_sNameAndOrder;	// имя программы с номером
	CString m_sFullNameFileCFG;	// имя (с путём)) конфигурационного файла
	CString m_sFullNameFileCHM;	// имя (с путём)) файла справки
	CString m_sFullFolderInstLoc;	// папка ZetLab
	CString m_sFullFolderCFG;		// папка Config
	CString m_sFullFolderCorrect;	// папка Correct
	CString m_sFullFolderResult;	// папка Result
	CString m_sFullFolderSignal;	// папка Signal
	CString m_sFullFolderCompressed;// папка Compressed

	virtual BOOL OnInitDialog();

	/*Вызывать в конце OnInitDialog инициализируемой программы.
	При отсутствии этой функции событие ON_MODIFYSVR не обрабатывается*/
	void OnPostInitDialog(bool bNeedAutoScaleXY = true, bool bNeedZetPath = true);
	//чтение из реестра пути к директории конфигурации
	CString ReadPathFolder(CString dir);
	//-----------------------------------------
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()

protected:
	RECT m_rectOld;				// переменная нужна для ограничения размеров окна
	long m_nOrder;				// номер запущенного экземпляра программы
	HWND m_panelhwnd;			// дескриптор Zet-панели
	ULONG m_nZPriority;			// Zet-приоритер программы
	UINT_PTR m_idTimerStart;	// ID таймера для старта с новыми параметрами
	UINT m_ElapseTimerStart;	//			время этого таймера (100 мсек)
	UINT_PTR m_idTimerModify;	// ID таймера для пропуска пачек SRV::Modify
	UINT m_ElapseTimerModify;	//			время этого таймера (500 мсек)
	HWND *m_pEditForDigit;		// указатель на массив СEdit для ввода чисел
	int m_NumEditForDigit;		//			размер этого массива
	UINT m_crc32ByNameExe = 0;	// контрольная сумма, рассчитанная по имени EXE, см. конструктор
	uint16_t m_numberSelfExe = 1;	// номер запущенного процесса, счёт от 1
	TypeNewParam m_typeNewParam = param_no;

	//объект, который будет сохраняться по умолчанию
	PVOID pSaveData;			// указатель на структуру Save
	PVOID pSaveDataNew;			// указатель на структуру SaveNew
	int m_sizeSaveData;			// размер структуры Save в байтах
	
	// параметры программы
	void OnStructureNewIfInit();
	void TranslatePhrasesThis();

	// Виртуальные функции для переопределения в наследниках
	virtual void OnStructureDefault();	// установка параметров по умолчанию
	virtual void OnStructureNew();		// установка новых параметров
	virtual void OnStructureSave() {}	// сохранение текущих параметров перед закрытием программы
	virtual void OnPreReadCfg() {};		// Функция выполняется перед чтением CFG-файла
	virtual void OnPostReadCfg() {};	// Функция выполняется после чтения CFG-файла
	virtual void OnStopTimeChannel(long chanStop) {}	// остановилось время по каналу chanStop
	virtual void TranslatePhrases() {}	// переводимые фразы программы
	virtual void OnErrorReadData(long indChan, long kodError){}	// ошибка при чтении данных с сервера
	virtual void OnErrorCalcData(long indChan, long kodError){}	// ошибка при обрабоке данных, считанных с сервера

	// Чтение данных из конфигурационного файла. Функция виртуальная, её
	// необходимо переопределить для программ, у которых размер CFG-файла не
	// является постоянным.
	virtual bool ReadCFGFile();			// в pSaveDataNew
	// Запись данных в конфигурационный файл. Функция виртуальная, её
	// необходимо переопределить для программ, у которых размер CFG-файла не
	// является постоянным.
	virtual bool WriteCFGFile();		// из pSaveData
		
	// обработка событий разных ocx
	// ZetServer
	virtual void OnFileModeSRV(long GlobalPar){}	// включение/выключение воспроизведения
	virtual void OnSetTimeSRV(float GlobalPar){}	// установка времени в режиме воспроизведения
	virtual void OnStartFileSRV(long GlobalPar){}	// старт в режиме воспроизведения
	virtual void OnModifySRV(){}					// последний Modify
	virtual void OnFirstModifySRV_0(){}				// первый Modify

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

	// задание Edit-тов для ввода чисел
	void SetNumberEditForDigit(int num);
	// отображение/скрытие интерфейса
	void ShowInterface(bool val);
	void CopyStringToClipboard(CString str, bool bDig = true);

	// Задание положения окна по умолчанию в зависимости от номера программы
	void SetWindowPosDefault(WORD num, DWORD code);

	// Получение номера канала сервера по умолчанию.
	// Номер канала определяется по номеру программы number.
	// Возвращаемое значение 0 - канала получен, иначе код ошибки ErrorCodes
	long GetDefaultChannel(long number, long & chan);
	// Получение номера канала сервера по умолчанию и его ID.
	// Номер канала определяется по номеру программы number.
	// Возвращаемое значение 0 - канала получен, иначе код ошибки ErrorCodes
	long GetDefaultChannel(long number, long & chan, long & id);
	// Получение номера канала сервера по умолчанию и его GUID.
	// Номер канала определяется по номеру программы number.
	// Возвращаемое значение 0 - канала получен, иначе код ошибки ErrorCodes
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

	// обработка сообщений в журнал ----------------------------------
	void MessageError(CString str, long id, long kod = 0);
	void MessageWarning(CString str, long id, long kod = 0);
	void MessageInformation(CString str);

	// установка стандартных цветов для графиков
	void SetGraficColors(CWnd *cont);
};
//------------------------------------------------------------------------------