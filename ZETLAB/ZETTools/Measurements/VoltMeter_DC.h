//------------------------------------------------------------------------------
// Класс СVoltMeter_DC : header file
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
	uint16_t fast;			// код усреднения вольтметра (0, 1 или 2)
	uint16_t numFast;		// кол-во усреднений, не более 3, актуально для 7000
	uint16_t nPageMax;		// кол-во порций (от 1, 10 или 100)
	uint16_t nPageCur;		// номер текущей порции (от 0 до nPageMax-1)

	CodeQuality qualityAll;	// суммарный код качества результата
	float mean;				// результат среднее
	
	double time;			// SRV-время результата
	CViewResultParam vrp;	// параметры отображения результата

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
		// определение нового numFast
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

		// проверка нового значения fast
		if (codeFast > 2)
			fast = 2;
		else
			fast = codeFast;

		// определение нового значения fast с учетом нового numFast
		if (numFast == 3)
		{// все значения fast допустимы
		}
		else if (numFast == 2)
		{// можно 1 сек и 10 сек
			if (fast == 0)
				fast = 1;
		}
		else// numFast == 1
		{// можно только 10 сек
			if (fast < 2)
				fast = 2;
		}

		// определение nPageMax по fast
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

	// Инициализация
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
{// Структура, которая хранится в конфигурационном файле программы
	GUID id;					// GUID канала
	uint32_t fast;				// быстро - медленно
	uint32_t reserved;			// добавлено для выравнивания на 8 байт
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
	// переменные --------------------------------------------------------------
	CString m_sSec;
	CString m_sFast_0_1;
	CString m_sFast_1_0;
	CString m_sFast_10;
	CString m_sAbsent;
	CString m_sDisabled;

	struct SaveStructure m_sParam;
	CString m_strMean;	// требуется достут в функции PreTranslateMessage
	
	UINT m_ElapseTimerReStart = 300;

	std::vector<CHANNEL_STRUCT_READ_CALC*> m_vectParam;		// вектор параметров каналов
	CThreadSRVmultiSelfS *m_pThread = nullptr;

	// указатель на структуру параметров чтения текущего канала
	CHANNEL_STRUCT_READ_CALC * m_pCurParam = nullptr;
	
	// инициализация и настройка -----------------------------------------------
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();

	virtual void OnStructureDefault();	// установка параметров по умолчанию
	virtual void OnStructureNew();		// установка новых параметров


	// работа ------------------------------------------------------------------
	LRESULT OnSetUpdate(WPARAM wParam, LPARAM lParam);	// новые данные
	afx_msg void OnTimer(UINT_PTR nIDEvent);			// таймер 
	virtual BOOL PreTranslateMessage(MSG* pMsg);		// Ctrl + C

	void StopAllThread();
	void ReStart();
	void New_channel();		//номер канала
	void New_fast();		//быстро - медленно

	// Работа с каналами -------------------------------------------------------
	void AddChannelByNumber(long channel, BOOL bValueCur, bool bChangeInWin);
	void AddChannelByGUID(GUID id, BOOL bValueCur, bool bChangeInWin);
	void SetStartTime(CHANNEL_STRUCT_READ_CALC * ps);
	void SetFlagsForNewChanOk();

	void ViewNewChan_iw(bool bClearMean);
	void ViewTitle_iw(bool bClearMean);		// отображение в окне при замене канала

	// события -----------------------------------------------------------------
	virtual void OnFileModeSRV(long GlobalPar);
	virtual void OnSetTimeSRV(long GlobalPar);
	virtual void OnStartFileSRV(long GlobalPar);
	virtual void OnModifySRV();
	virtual void OnFirstModifySRV_0();		// первый в пачке Modify

	// UNIT --------------------------------------------------------------------
	virtual void OnReadyUnitParent(long param);	// UNIT-1
	virtual void OnReadyUnit2Param();			// UNIT-2

	void SetUnitParam_p0(const long val);	//Номер канала
	void SetUnitParam_p1(const long val);	//Код усреднения
	void SetUnitParam_m1(const long val);	//Многоканальный режим, кол-во каналов
	void SetUnitParam_m2(const long val);	//Индекс текущего канала
	void SetUnitParam_m3(const long val);	//Индекс останавливаемого канала
	void SetUnitParam_m4(const long val);	//Индекс запускаемого канала
	void SetUnitParam_m5(const long val);	//Номер добавляемого канала

//	void WriteUnitResult_p0(const long shift, const float val);			//Результат
	void WriteUnitResult_p1(const long shift, WideResult<float> *pval);	//Широкий результат

	DECLARE_MESSAGE_MAP()

//------------------------------------------------------------------------------
protected:
	// виртуальные функции, которые надо определить в дочернем классе
	// для обращения к элементам интерфейса
	virtual void Set_Title_iw(LPCHANNEL_STRUCT pStruct) {}	// отображение заголовка
	// отображение при отсутствии/отключении канала
	// code = 0 => канал отсутствует
	// code = 1 => канал отключен
	virtual void Set_ChannelOff_iw(long code) {}

	virtual void Set_Mean_iw(LPCWSTR str) {}	// отображение измеренного значения
	virtual void Set_Mera_iw(LPCWSTR str) {}	// отображение размерности канала

	virtual void Set_Channel_iw(const long val) {}	// установка измерительного канала
	virtual void Set_HorScale_iw(const float val) {}// отображение уровня канала
	virtual void Set_Fast_iw(const uint16_t val) {}		// установка времени усреднения
	virtual void Set_NumFast_iw(const uint16_t numPosition) {}	// задание списка времен усреднения

	virtual void View_NewChan_iw() {}				// задание списка каналов

	virtual void ResetOutData_iw() {}				// стирание измеренного значения и СКО
	virtual void Quality_iw(const long val) {}		// отображение качества данных

	// задание кол-ва цифровых позиций в индикаторе
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
	L"0: 32s: Номер канала;"
	L"1: 32s: Код усреднения;"
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