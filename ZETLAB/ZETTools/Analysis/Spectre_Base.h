//------------------------------------------------------------------------------
// Класс Spectre_Base : header file
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
	// переменные --------------------------------------------------------------
	SaveStruct*	m_sParam;		// для текущих значений
	SaveStruct*	m_sParamNew;	// для новых значений, полученных извне
	ShowParams*	m_sParamReader;	// для хранения параметров после запуска воспроизведения
	WorkParams m_workParams;
	CalcDataStruct m_calcData;
	ThreadParams m_threadParams; // структура для передачи в рабочий поток

	CZETLoggerClient *m_pLogger = nullptr;
	Spectr_Analysis_Result* m_psaResultLogger = nullptr;
	Spectr_Analysis_CFG m_saConfigLogger;
	SmartArray<BYTE> m_saBufferLogger;
	float* m_pResultArrayLogger = nullptr;
	DWORD m_dwSizeArrayLogger;
	ZetHeader* m_pResultDescription = nullptr;

	CString stringTempl, m_strFormat;

	UINT m_ElapseTimerReStart;	// таймер для отработки
	UINT_PTR m_IDtimerReStart;	// посылолк из UNIT

	UINT m_ElapseTimerAutoscale;	// время ожидания перед автомасштабом
	UINT_PTR m_IDtimerAutoscale;	// ID таймера автомасштабирования графика

	bool m_isStarted;			// флаг работы программы
	bool m_bMulti;				// флаг работы в многоканальном режиме
	std::vector<MultiParamSelf> m_multi;

	int size;

	// указатель на структуру параметров чтения текущего канала
	MultiChannelSelf *m_pmChanCur;
	// указатель на структуру параметров обработки текущего канала
	MultiParamSelf   *m_pmParCur;
	
	CThreadSRVmultiSelf *g_pThread;
	FileArrayStruct * m_pFAS;

	// инициализация и настройка -----------------------------------------------
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void TestBackupChanMode();

	virtual void OnStructureDefault() override;	// установка параметров значениями по умолчанию
	virtual void OnStructureNew() override;		// установка новых параметров
	
	virtual void ViewNewChanInWindows();
	//void SetStrFormat(const long channel, const long decibel);
	//void SetFast(const long val);

	void DefaultMultiParam();
	void Show7000();
	bool CreateLogger();
	void SaveConfigToLogger();
	//long CalcMeasureType(const long ampl, const long decibel);

	// работа ------------------------------------------------------------------
	LRESULT OnSetUpdate(WPARAM wParam, LPARAM lParam);	// новые данные
	LRESULT OnEndGenerateFileUnit(WPARAM wParam, LPARAM lParam); // Обработчик сообщения об окончании записи в файл 
	afx_msg void OnTimer(UINT_PTR nIDEvent);			// таймер
	virtual BOOL PreTranslateMessage(MSG* pMsg);		// Ctrl + C

	// изменения параметров ----------------------------------------------------
	void StopAllThread();
	//void New_channel(bool bTest, bool bReStart);	// номер канала
	//void New_fast(bool bTest, bool bReStart);		// быстро - медленно
	//void New_ampl_decibel(bool bTest);				// 
	void ReStart(bool bStopThread);

	// события -----------------------------------------------------------------
	virtual void OnFileModeSRV(long GlobalPar);
	virtual void OnSetTimeSRV(long GlobalPar);
	virtual void OnStartFileSRV(long GlobalPar);
	virtual void OnModifySRV();
	virtual void _OnModifySRV(long GlobalPar);
	virtual void OnFirstModifySRV_0();		// первый в пачке Modify

	// UNIT --------------------------------------------------------------------
	virtual void OnReadyUnitParent(long param);
	virtual void OnReadyUnit2Param();

	void SetUnitParam_p0(const long val);	// Номер канала
	void SetUnitParam_p1(const long val);	// установка декады
	void SetUnitParam_p3(const double val);	// установка усреднения в сек
	void SetUnitParam_p4(const long val);	// установка типа представления
	void SetUnitParam_p5(const long val);	// установка на получение размера спектра
	void SetUnitParam_p6(const long val);	// установка на получение массива частот
	void SetUnitParam_p7(const long val);	// установка на получение значений спектра
	void SetUnitParam_p8(const long val);	// установка частотного разрешения, размер ПФ
	void SetUnitParam_p9(const long val);	// очистка спектра
	void SetUnitParam_p10(const long val);	// тип анализа БПФ ДПФ
	void SetUnitParam_p11(const long val);	// тип весовой функции
	void SetUnitParam_p12(const long val);	// установка на запрет передачи данных
	void SetUnitParam_p13(const long val);	// интегрирование или дифференцирование
	void SetUnitParam_p14(const long val);	// линейно или логарифмического расчета
	void SetUnitParam_p15(const long val);	// чтение нормы и прорисовка
	void SetUnitParam_p16(const long val);	// запрет рисования нормы
	void SetUnitParam_p17(const long val);	// установка идентификатора номера канала
	void SetUnitParam_p18(const long val);	// установка курсора
	void SetUnitParam_p19(const long val);	// установка на последовательное получение данных (сетка частот(0), спектр(1), максимальный спектр(2), средний спектр(3))
	void SetUnitParam_p20(const long val);	// установка на разрешение максимального спектра
	void SetUnitParam_p21(const long val);	// установка на разрешение среднего спектра
	void SetUnitParam_p22(const double val);	// установка интервала расчета дополнительных графиков
	void SetUnitParam_p23(const CString val);	// 
	void SetUnitParam_p24(const double val);	// 
	void SetUnitParam_p25(const double val);	// 
	void SetUnitParam_p26(const long val);	// установка на одновременное получение данных в массиве учетверённого размера
	void SetUnitParam_p27(const double val);	// установка частотного разрешения
	void SetUnitParam_p28(const long val);	// установка типа усреднения
	void SetUnitParam_p255(const long val);	// Start
	void SetUnitParam_p127(const long val);	// Stop


	void SetUnitParam_m1(const long val);	// Многоканальный режим, кол-во каналов
	void SetUnitParam_m2(const long val);	// Индекс текущего канала
	void SetUnitParam_m3(const long val);	// Индекс останавливаемого канала
	void SetUnitParam_m4(const long val);	// Индекс запускаемого канала
	void SetUnitParam_m5(const long val);	// Номер добавляемого канала

	//void WriteUnitResult_p0(const long shift, const float val);			// Результат
	//void WriteUnitResult_p1(const long shift, WideResult<float> *pval);	// Широкий результат

	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
//------------------------------------------------------------------------------
protected:
	// виртуальные функции, которые надо определить в дочернем классе
	// для обращения к элементам интерфейса
	virtual void CreateElementInterface() {}		// создание элементов интерфейса
	virtual void DeleteElementInterface() {}		// удаление элементов интерфейса

	virtual void Set_Title_iw(const long val) {}	// отображение заголовка

	virtual void Set_Icon_iw() {}					// отображение иконки программы
	//virtual void Set_Mean_iw(const CString str) {}	// отображение измеренного значения
	//virtual void Set_Peak_iw(const CString str) {}	// отображение измеренного пика
	//virtual void Set_Mera_iw(const CString str) {}	// отображение размерности канала

	//virtual void Set_Channel_iw(const long val) {}	// установка измерительного канала
	//virtual void Set_HorScale_iw(const float val) {}// отображение уровня канала
	// отображение типа отображаемого значения - СКЗ; СКЗ, дБ; Амплитуда; Амплитуда, дБ
	//virtual void Set_MeasureType_iw(const long val) {}
	//virtual void Set_Fast_iw(const long val) {}		// установка времени усреднения

	//virtual void View_NewChan_iw() {}				// задание списка каналов
	//virtual void View_MeasureType_iw() {}			// задание списка типов значений
	//virtual void View_Average_iw() {}				// задание списка времен усреднения

	virtual void ResetOutData_iw() {}				// Очистка отображения спектра.
	//virtual void Quality_iw(const long val) {}		// отображение качества данных

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
		StringWriteFileStringFreq, // Строки для записи спектра в файл
		StringWriteFileStringDate,
		StringWriteFileStringTime,
		StringWriteFileStringFreqLevel,
		StringWrtieFileStringMax,
		StringWrtieFileStringSred,
		StringWrtieFileStringNorma,
		StringWrtieFileStringHz, 
		StringWriteIndicator, // индикатор уровня
		StringWriteSimpling, // Частота (Гц)
		StringWriteMaximum, // Максимум
		StringWriteStop, // Стоп
		StringWriteRecord, // Запись
		StringWriteHzVal, //Гц
		StringWriteSettings, //Настройка параметров узкополосного спектра
		StringWriteSamplingRate, // Частота дискретизации
		StringWriteFreqRange, // Частотный диапазон, Гц
		StringWriteChanMeasure, // Канал измерений
		StringWriteFreqPermission, // Частотное разрешение, Гц
		StringWriteWeightFunc, // Весовая функция
		StringWriteAveranging, // Усреднение, сек
		StringWriteInteDiffe, // Интегрирование/дифференцирование
		StringWriteRepresentation, // Тип представления
		StringWriteCommentRecord, // Коментарий для записи
		StringWriteCalculationY, // Расчет значение по Y
		StringWriteMedianFilter, // Медианный фильтр
		StringWriteAdditionalWnd, // Дополнительные окна
		StringWriteIntervalCalc, // Интервал расчета
		StringWriteAdditionalDiagrams, // Дополнительные графики
		StringWriteAverangingType, // Тип усреднения
		StringWriteProcessingType, // Тип обработки
		StringWriteNormFile, // Файл нормы
		StringWriteKepstr, // Кепстр
		StringWriteSpectrogram, // Спектрограмма
		StringWrite3DSpectrogram, // 3D - Спектрограмма
		StringWriteCheckP, // Проходная
		StringWriteRangeCleaning, // Очистка спектра
		StringWriteMiddle, // Средний
		StringWriteMax, // Максимальный
		StringWriteFileNorm, // Файл (норма)
		StringWriteFastFourier, // Быстрое ПФ
		StringWriteDiscreteFourier, // Дискретное ПФ
		StringWriteTwoDiff, // Двойное дифференцирование
		StringWriteOneDiff, // Дифференцирование
		StringWriteNotProcessing, // Без обработки
		StringWriteOneIntegrat, // Интегрирование
		StringWriteTwoIntegrat, // Двойное Интегрирование
		StringWriteRectangular, // Прямоугольная
		StringWriteHanna, // Ханна
		StringWriteHamming, // Хеминга
		StringWriteBlackman, // Блэкмана
		StringWriteBarlet, // Барлета
		StringWriteKaizer, // Кайзера
		StringWriteReefVincent, // Рифа-Винсента
		StringWriteBlackManHarris, // Блэкмана-Харриса
		StringWriteNatal, // Наталла
		StringWriteBlackmanNatal, // Блэкмана-Наталла
		StringWriteWndPlanePeak, // Окно с плоской вершиной
		StringWriteLinear, // Линейное
		StringWriteLogarithmic, // Логарифмическое
		StringWriteSpectralDensity, // спектральная плотность
		StringWriteSpectralPower, // спектральная мощность
		StringWriteAverageSquare, // среднее квадратичное
		StringWritePeakValue, // пиковое значение
		StringWriteExponential, // Экспоненциальное
		StringWriteWndParam, // Открыть/Закрыть окно параметров
		StringWriteWndData, // Открыть/Закрыть окно данных
		StringWriteWndEnd, // Закрыть
		StringWriteWndHelp, // Справка
		StringWriteWndMin, // Свернуть
		StringWriteWndMax, // Развернуть
		StringWriteSpectr, // Спектр
		StringWriteHerz, // Гц
		StringWriteStart, // Старт
	};

	virtual CString GetLocalString(LocalStrings id) { return L""; }; // получение локализованной строчки.

	virtual void InitializeDefaultParams(); // первоначальная инциализация параметров.
	virtual void InitializeDefaultChannel(MultiParamSelf* pmp, MultiChannelSelf* mChan); // первоначальная инциализация канала.
	virtual void InitializeDefaultData(); // первоначальная инциализация данных.

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