//------------------------------------------------------------------------------
// Класс СVoltMeter_AC : header file
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
{// структура параметров канала в многоканальном режиме работы, предназначенных
//	для обработки
	long fast;			// код времени усреднения (0, 1 или 2)
	long ampl;			// отображение: 0 - скз, 1 - амплитуда
	long decibel;		// отображение в децибелах: 0 - нет, 1- да
	long typeValUnit;	// тип данных для передачи в Unit
	float valAmpl;		// измеренное значение амплитуды
	float valSks;		// измеренное значение СКЗ
	float valPeak;		// измеренное пиковое значение
	float *pData;		// массив данных всех страниц
	DWORD sizeAll;		// размер этого массива
	DWORD numberPage;	// кол-во страниц (1, 10 или 100)
	DWORD numberPageCur;// номер текущей страницы
	CodeQuality qualityAll;	// суммарный код качества результата
	CodeQuality pCodeQuality[100];	// качество каждой порции
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
	long fast;			// усреднение: 0 - 0,1 с; 1 - 1 с; 2 - 10 с;
	long channel;		// номер канала
	long IdChan;		// идентификатор канала
	long decibel;		// отображение в децибелах: 0 - нет, 1- да
	long ampl;			// отображение: 0 - скз, 1 - амплитуда, 2 - пик
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
	// переменные --------------------------------------------------------------
	CString m_sSec;			// с
	CString m_sDb;			// дБ
	CString m_sFast_0_1;    // быстрое 0.1 с
	CString m_sFast_1_0;    // медленное 1 с
	CString m_sFast_10;     // сверхмедленно 10 с
	CString m_srms;			// СКЗ
	CString m_srms_db;		// СКЗ, дБ
	CString m_samp;			// Амплитуда
	CString m_samp_db;		// Амплитуда, дБ
	CString m_sPeak;		// пик


	SaveStruct	m_sParam,		// для текущих значений
		m_sParamNew;	// для новых значений, полученных извне
	CString strAmpl, strPeak, strTemp;//, m_strFormat;
	float m_ampl, m_sks, m_peak;
	UINT m_ElapseTimerReStart;	// таймер для отработки
	UINT_PTR m_IDtimerReStart;	//		посылолк из UNIT

	std::vector<MultiParamSelf> m_multi;
	bool m_bMulti;				// флаг работы в многоканальном режиме
	// указатель на структуру параметров чтения текущего канала
	MultiChannelSelf *m_pmChanCur;
	// указатель на структуру параметров обработки текущего канала
	MultiParamSelf   *m_pmParCur;

	// инициализация и настройка -----------------------------------------------
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();

	virtual void OnStructureDefault();	// установка параметро
	virtual void OnStructureNew();		// установка новых параметров

	void ViewNewChanInWindows();
	void SetFast(const long val);

	void DefaultMultiParam();
	void Show7000();
	long CalcMeasureType(const long ampl, const long decibel);

	// работа ------------------------------------------------------------------
	LRESULT OnSetUpdate(WPARAM wParam, LPARAM lParam);	// новые данные
	afx_msg void OnTimer(UINT_PTR nIDEvent);			// таймер
	virtual BOOL PreTranslateMessage(MSG* pMsg);		// Ctrl + C

	// изменения параметров ----------------------------------------------------
	void StopAllThread();
	void New_channel(bool bTest, bool bReStart);	// номер канала
	void New_fast(bool bTest, bool bReStart);		// быстро - медленно
	void New_ampl_decibel(bool bTest);				// 
	void ReStart(bool bStopThread, bool bNewTime = true);

	// события -----------------------------------------------------------------
	virtual void OnFileModeSRV(long GlobalPar);
	virtual void OnSetTimeSRV(long GlobalPar);
	virtual void OnStartFileSRV(long GlobalPar);
	virtual void OnModifySRV();
	virtual void OnFirstModifySRV_0();		// первый в пачке Modify

	// UNIT --------------------------------------------------------------------
	virtual void OnReadyUnitParent(long param);
	virtual void OnReadyUnit2Param();

	void SetUnitParam_p0(const long val);	// Номер канала
	void SetUnitParam_p1(const long val);	// Код усреднения
	void SetUnitParam_p2(const long val);	// Код данных для UNIT
	void SetUnitParam_p3(const long val);	// Код представления дБ

	void SetUnitParam_m1(const long val);	// Многоканальный режим, кол-во каналов
	void SetUnitParam_m2(const long val);	// Индекс текущего канала
	void SetUnitParam_m3(const long val);	// Индекс останавливаемого канала
	void SetUnitParam_m4(const long val);	// Индекс запускаемого канала
	void SetUnitParam_m5(const long val);	// Номер добавляемого канала

//	void WriteUnitResult_p0(const long shift, const float val);			// Результат
	void WriteUnitResult_p1(const long shift, WideResult<float> *pval);	// Широкий результат

	DECLARE_MESSAGE_MAP()
//------------------------------------------------------------------------------
protected:
	// виртуальные функции, которые надо определить в дочернем классе
	// для обращения к элементам интерфейса
	virtual void CreateElementInterface() {}		// создание элементов интерфейса
	virtual void DeleteElementInterface() {}		// удаление элементов интерфейса

	virtual void Set_Title_iw(const long val) {}	// отображение заголовка

	virtual void Set_Icon_iw() {}					// отображение иконки программы
	virtual void Set_Mean_iw(const CString &str, const float val) {}	// отображение измеренного значения
	virtual void Set_Peak_iw(const CString &str, const float val) {}	// отображение измеренного пика
	virtual void Set_Mera_iw(const CString &str) {}	// отображение размерности канала

	virtual void Set_Channel_iw(const long val) {}	// установка измерительного канала
	virtual void Set_HorScale_iw(const float val) {}// отображение уровня канала
	// отображение типа отображаемого значения - СКЗ; СКЗ, дБ; Амплитуда; Амплитуда, дБ
	virtual void Set_MeasureType_iw(const long val) {}
	virtual void Set_Fast_iw(const long val) {}		// установка времени усреднения

	virtual void View_NewChan_iw() {}				// задание списка каналов
	virtual void View_MeasureType_iw() {}			// задание списка типов значений
	virtual void View_Average_iw() {}				// задание списка времен усреднения

	virtual void ResetOutData_iw() {}				// стирание измеренного значения и пика
	virtual void Quality_iw(const long val) {}		// отображение качества данных
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
	L"0: 32s: Номер канала;"
	L"1: 32s: Код усреднения;"
	L"2: 32s: Код данных для UNIT;"
	L"3: 32s: Код представления дБ;"
	L"-1: 32s: Многоканальный режим, кол-во каналов;"
	L"-2: 32s: Индекс текущего канала;"
	L"-3: 32s: Индекс останавливаемого канала;"
	L"-4: 32s: Индекс запускаемого канала;"
	L"-5: 32s: Номер добавляемого канала;"
END

UNIT2_RESULT_TYPE RCDATA 
BEGIN
	L"1: wrs_32f: Широкий результат;"
END
*/
//------------------------------------------------------------------------------