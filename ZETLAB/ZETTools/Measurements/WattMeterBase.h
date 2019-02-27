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

struct ValueStruct	// структура данных передаваемая Unit'у
{
	long quality;	// качество данных
	float S;		// полная мощность
	float P;		// активная мощность
	float Q;		// реактивная мощность
	double time;	// время по которому произошло считывание
};

struct SaveStruct : public CustomSaveStructure
{
	//wchar_t NameProgram[300];
	//int NumberProgram;											// номер программы
	int channel1;												// номер первого измерительного канала
	long IdChan1;												// id первого измерительного канала
	int channel2;												// номер вторго измерительного канала
	long IdChan2;												// id вторго измерительного канала
	int fast;													// время усреднения 
	int typeview;												// вид выводимых значений мощностей
};

struct MultiParamSelf
{
	long fast;									// время усреднения
	long typeValUnit;							// 
	long Channel;								// номер первого измерительного канала
	float* pData;
	DWORD sizeAll;								// размер массива
	DWORD numberPage;							// количество страниц
	float usred;								// тип времени устреднения
	DWORD numberPageCur;						// номер текущей страницы
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
	SaveStruct	m_sParam;										// для текущих значений
	SaveStruct	m_sParamNew;									// для новых значений, полученных извне
	long m_IdVirtChan1;											// id виртуального первого канала
	long m_IdVirtChan2;											// id виртуального второго канала
	long m_IdVirtChan3;											// id виртуального третьего канала
	std::vector<MultiParamSelf> m_multi;	
	MultiChannelSelf *m_pmChanCur1;								// указатель на структуру параметров чтения текущего канала	
	MultiChannelSelf *m_pmChanCur2;								// указатель на структуру параметров чтения текущего канала	
	MultiParamSelf   *m_pmParCur1;								// указатель на структуру параметров обработки текущего канала
	MultiParamSelf   *m_pmParCur2;								// указатель на структуру параметров обработки текущего канала
	bool m_bMulti;												// флаг работы в многоканальном режиме
	UINT_PTR m_IDtimerReStart;									// посылолк из UNIT
	UINT m_ElapseTimerReStart;									// таймер для отработки
	WideResult<vector<float>> m_result_f;						// вектор с вычисленными величинами
	virtual void OnReadyUnitParent(long param);
	virtual void OnReadyUnit2Param();

	void SetUnitParam_p0(const long val);						// установка значения первого канала 
	void SetUnitParam_p1(const long val);						// установка значения второго канала
	void SetUnitParam_p2(const long val);						// установка значения времения усреднения
	void SetUnitParam_p3(const long val);						// установка значения виды выводимого значения

	void SetUnitParam_m1(const long val);						// Многоканальный режим, количество каналов
	void SetUnitParam_m2(const long val);						// Индекс текущего канала
	void SetUnitParam_m3(const long val);						// Индекс останавливаемого канала
	void SetUnitParam_m4(const long val);						// Индекс запускаемого канала
	void SetUnitParam_m5(const long val);						// Номер добавляемого канала
public:
	CWattMeterBase(UINT nIDTemplate, CWnd* pParent, CString exeName,
		CString cfgFilePreamble, ULONG nZPriority);
	~CWattMeterBase();
	virtual BOOL OnInitDialog();
	virtual void SetChannel1(const int val)		{};				// установка первого канала измерения
	virtual void SetChannel2(const int val)		{};				// установка второго канала измерения
	virtual void SetFast(const long val);						// установка времени усреднения
	virtual void OnFileModeSRV(long GlobalPar);					// установка новых параметров 
	virtual void OnStartFileSRV(long GlobalPar);				// установка времени
	virtual void TestBackupChanMode();							// установка канала при запуске
	virtual void SetHorscaleVal(float val)		{};				// установка значения в идикатор уровня
	virtual void SetText1(const CString val)	{};				// установка знака активной мощности
	virtual void SetText2(const CString val)	{};				// установка знака полной мощности
	virtual void SetText3(const CString val)	{};				// установка знака реактивной мощности
	virtual void SetVal1(const CString val)		{};				// вывод значения активной мощности
	virtual void SetVal2(const CString val)		{};				// вывод значения полной мощности
	virtual void SetVal3(const CString val)		{};				// вывод значения реактивной мощности 
	virtual void SetMera1(const CString val)	{};				// вывод единицы измерения активной мощности
	virtual void SetMera2(const CString val)	{};				// вывод единицы измерения полной мощности
	virtual void SetMera3(const CString val)	{};				// вывод единицы измерения реактивной мощности
	virtual void AddMera1(CString val)			{};				// добавления приставки к единице измерения активной мощности
	virtual void AddMera2(CString val)			{};				// добавления приставки к единице измерения полной мощности
	virtual void AddMera3(CString val)			{};				// добавления приставки к единице измерения реактивной мощности
	virtual void SetFastI(const long val)		{};				// установка в комбобоксе времения усреднения
	virtual void SetViewI(const long val)		{};				// установка в комбобоксе вида выводимых значения 
	virtual void ResetOutData_iw()				{};				// очистить дисплей программы
	virtual void ResetMera()					{};				// установка изначальный есдиниц измерения мощностей
	virtual void SetColor(COLORREF val)			{};				// изменение цвета цифрового индикатора
	virtual void OnStructureDefault();							// установка параметров программы по умолчанию
	virtual void OnStructureNew();								// установка новых параметров программы
	virtual void SetTitleI(const int val1, const int val2)	{}; // установка текста в шапку программы
	LRESULT OnSetUpdate(WPARAM wParam, LPARAM lParam);			// обработчик сообщений
	afx_msg void OnDestroy();									// обработчик события 
	afx_msg void OnTimer(UINT_PTR nIDEvent);					// таймер
public:
	void ReStart(bool bstopThread, bool bNewTime = true);
	void New_channel(bool bTest, bool bReStart);				// установка нового канала измерений
	void NewViewDisplay(long val);								// установка вида выводимых значения мощности
	void StopAllThread();										// остановка потока
	void OnModifySRV();											// обработчик сообщений от сервера
	void OnSetTimeSRV(long GlobalPar);
	void New_fast(bool bTest, bool bReStart);					// установка нового времени усреднения
	void DefaultMultiParam();									// установка параметров по умолчанию
	void ViewNewChanInWindows();	
	void SetInfoVrtCahnnel(const long fchannel);				// установка настроек виртуального канала
	void NumberFormat(const float val, int &degree, 
		int &flo, CString &type);								// форматирование строки вывода значений мощностей
	DECLARE_MESSAGE_MAP()
};

