//------------------------------------------------------------------------------
#pragma once
#define _COMPLEX_DEFINED
#include <complex>
#include <mathZet.h>
#include <CustomSaveStruct.h>
#include <Buffer\SmartArray.h>
#include <Dialog_ZET\Translate.h>
#include <Dialog_ZET\CDZetPath.h>
#include <ZetThread\ThreadSRVmultiSelfS.h>
#include <ZetLogger/include/ZetLogger.h>
#include <Structures\UltraZoom\UltraZoom_Analysis_CFG.zschema1.h>
#include <Structures\UltraZoom\UltraZoom_Analysis_Result.zschema1.h>
//------------------------------------------------------------------------------
#define ULTRA_ZOOM_MAX_LINE_IN_GRAMMA	30000
#define MAX_SIZE_ULTRA_ZOOM 50000
//------------------------------------------------------------------------------
/*		Формулы и неравенства
	m_dF_need = freq_band * typeResolution
	
	2 * freq_band <= freq_F0 - freq_band/2
	freq_F0 + freq_band/2 <= 0.45 * freq_ADC
	
	0.5 * freq_ADC / 1000 <= freq_band <= 0.5 * freq_ADC / 20
	k1 = 0.5 * freq_ADC / freq_band

	freq_band / (16 * 1024) <= m_dF_need <= freq_band / 128
	k2 = freq_band / m_dF_need

	Чтение данных с сервера с 4-х кратным перекрытием интервалов.
	Порциями читать нельзя т.к. вероятность того, что F0 кратно freq_ADC не велика.

	Для синхронизации данных двух копий программы:
	- инкремент времени чтения m_time_ADC2 - ОБЯЗАТЕНО должен быть кратен 1 сек.
	- первое время чтения - целое число сек + m_time_ADC2

	Время завершения переходных процессов Т = k1 * k2 / freq_ADC (и где-то * на 4)

	Размеры массивов:
	1.	Исходные данные частоты freq_ADC - для расчетов сигнала частоты m_freq_ADC2		
		m_saCos, m_saSin, m_saSig_src
		Размер - это целое число периодов частоты F0 => N_F0 = m * freq_ADC / freq_F0
		m >= 2 так, чтобы N_F0 >= 20

	2. промежуточные данные частоты m_freq_ADC2 - для отображения в окне Sig_F0
		m_saSig_F0_Re, m_saSig_F0_Im, m_saSig_F0_Am, m_saSig_F0_Fi_grad
		Размер N_Re = interval_SigF0 * m_freq_ADC2;

	3. m_saLineGramma_new, m_saLineGramma_old, m_saLineGramma_sum2, m_saLineGramma_aver
		Размер N_LG = 2 * freq_band / m_dF_fact

	Кол-во строк в грамме N_G = grammaTime * m_freq_ADC2;
	m_bGrammaIsFull = m_numLineInGrammaCur == m_numLineInGrammaMax;

	m_time_ADC2 = 0.1 сек (m_freq_ADC2 = 10 Гц)
*/
//------------------------------------------------------------------------------
enum TypeResolution : BYTE
{
	tr_0_1,
	tr_0_05,
	tr_0_025,
	tr_0_01,
	tr_0_005, 
	tr_0_0025,
	tr_0_001,
	tr_size
};
//------------------------------------------------------------------------------
enum TypeTransform : BYTE
{
	tt_FFT,
	tt_Wigner_Ville,
	tt_size
};
//------------------------------------------------------------------------------
struct ArrayForLogger
{
	float x;
	float y;
};
//----------------------------------------------------------------------
struct StructureCFG
{
	bool bWindow_sigF0;				// флаг отображения доп. окна sigF0
	bool bWindow_sigAver;			// флаг отображения доп. окна sigAver
	TypeTransform typeTransform;	// флаг тип преобразования
	TypeResolution typeResolution;	// тип требуемого частотного разрешения
	float freq_F0;					// требуемая центральная частота, Гц

	float freq_band;				// отображаемая полоса анализа, Гц
	float time_grammaY;				// время отображения данных в грамме, сек

	long channel;					// канал сервера
	long code_updata;				// код обновления (0 - 0,1 сек; 1 - 1 сек; 2 - 10 сек)

	long l1;
	long l2;

	GUID channelGuid;				// GUID канала сервера

	StructureCFG();
	StructureCFG(const StructureCFG & val);
	StructureCFG & operator = (const StructureCFG & val);

	void SetValueDefault(CDSRV * psrv = nullptr, long chan = 0);
	bool ChangeOnlyFlagWindow(StructureCFG & cfgNew);
	bool ChangeOnlyGrammaTime(StructureCFG & cfgNew);
	bool ChangeNo(StructureCFG & cfgNew);
};

struct SaveStructure : public CustomSaveStructure
{
	StructureCFG cfg;
	WINDOWPLACEMENT placement[3];	// положение доп. окон
	
	SaveStructure();
	SaveStructure(const SaveStructure & val);
	SaveStructure & operator = (const SaveStructure & val);
};
//------------------------------------------------------------------------------
// Структура параметров, передаваемых через UNIT-2
struct SrtuctParamForUNIT
{
	DWORD sizeInByte;		// размер структуры, байты
	DWORD sizeX;			// размер граммы по Х, отсчеты
	DWORD sizeY;			// размер граммы по Y, линии
	float val_F0;			// фактическое значение центральной частоты, Гц
	float val_dF;			// фактическое част. разрешение, Гц
	float val_band;			// отображаемая полоса анализа (копия из CFG)
	GUID channelID;			// GUID канала
	wchar_t unitResult[16];	// размерность единицы

	SrtuctParamForUNIT()
		: sizeX(0)
		, sizeY(0)
		, val_F0(0.f)
		, val_dF(0.f)
		, val_band(0.f)
		, channelID(GUID_NULL)
	{
		sizeInByte = (DWORD)sizeof(SrtuctParamForUNIT);
		ZeroMemory(unitResult, sizeof(unitResult));
	}
};
//------------------------------------------------------------------------------
// Структура параметров одного канала для выполнения расчётов
struct StructCalcParam
{	// массивы сигнала центральной частоты
	bool m_bGrammaIsFull;		// в грамме заполнены все линии по Y
	bool m_bDataSrcIsFull;		// в массиве исходных данных есть все отсчёты
	bool m_bFirstPage;			// первая порция в исходных данных
	bool m_bNeedCalc;			// можно выполнять вычисления
	DWORD m_size_fft;			// размер для FFT = 2 ^ N

	int m_len_2;				// = m_size_fft / 2
	int m_indx_F0;				// индекс центр. частоты в массиве после FFT
	int m_indx_band;			// индекс первой полосы для граммы

	int m_shift;				// смещение первого текущего отчета в исходном массиве
	DWORD m_num_NewDataCur;		// текущее кол-во данных, ещё не вошедших в результат

	DWORD m_num_NewDataMax;		// максимальное кол-во данных, ещё не вошедших в результат
	DWORD m_num_src;			// текущее кол-во отсчётов в исходных данных

	DWORD m_num_grammaY;		// текущее кол-во линий в грамме
	DWORD m_size_grammaY;		// максимальное кол-во линий в грамме

	DWORD m_size_grammaX;		// кол-во точек в линии граммы
	IppsFFTSpec_C_64fc * m_pSruct_FFT;			// структура FFT
	SmartArray<Ipp8u> m_sa8u_Sruct_FFT;
	SmartArray<Ipp8u> m_sa8u_Work_FFT;
	IppsHilbertSpec * m_pSruct_Hilbert;	// структура преобразования Гильберта
	SmartArray<Ipp8u> m_sa8u_Sruct_Hilbert;
	SmartArray<Ipp8u> m_sa8u_Work_Hilbert;
	
// 	IppsHilbertSpec_32f32fc * m_pSruct_Hilbert;	// структура преобразования Гильберта

	double m_fazaOld_32f;		// последнее значение float-фазы
	double m_dF_fact;			// фактическое частотное разрешение
	double m_extinction_1;		// обратный весовой коэффициент используемого временного окна
	Ipp64fc m_fazaOld_32fc;		// предыдущее комплексное значение анал. си гнала на центральной частоте

	CZetTimeSpan m_ztsDataTimeSRV;	// время данных

	// массивы размером m_size_fft
	SmartArray<float> m_sa_src_sig_32;		// исходный сигнал
	SmartArray<float> m_sa_src_sigRe_32;	// Re сигнал float
	SmartArray<double> m_sa_src_sigRe_64;	// Re сигнал double
	SmartArray<Ipp32fc> m_sa1_analit_32;	// комплексный сигнал float
	SmartArray<Ipp32fc> m_sa2_analit_32;	// комплексный сигнал float
	SmartArray<Ipp64fc> m_sa_analit_64;		// комплексный сигнал double до и после FFT
	SmartArray<double> m_sa_fft_Ampl_64;	// амплитуды double после FFT

	// массивы размером m_size_grammaY
	SmartArray<float> m_sa_f0_Ampl;			// для отображения
	SmartArray<float> m_sa_f0_Faza;			// для расчетов в радианах
	SmartArray<float> m_sa_f0_dFaz;			// для отображения производная фазы


	// массивы размером m_size_grammaX
	SmartArray<float> m_saLineGramma_new;	// новая строка в грамме
	SmartArray<float> m_saLineGramma_old;	// последняя строка в грамме
	SmartArray<float> m_saLineGramma_sum2;	// промежуточные данные - сумма квадратов
	SmartArray<float> m_saLineGramma_aver;	// усреднённая строка для отображения
	
	StructCalcParam();
	~StructCalcParam();

	void CalcSize(const float freq_ADC, const SaveStructure & cfg);
	bool ReSize();
	void ReStart();
	float CalcTimeAver(const float freq_ADC);

	bool CopyInpData(SmartArray<float> & saData, CZetTimeSpan & ztsData,
		StructureCFG & cfg);
	bool Calc_AnaliticSignal_FTT();
	bool Calc_AnaliticSignal_Wigner_Ville();
	bool Calc_Spectr();
	bool NewData(float time_updata, StructureCFG & cfg);

	double Calc_Extinction(int size, TypeTransform typeTransform);
};
//------------------------------------------------------------------------------
struct CHANNEL_STRUCT_READ_CALC : public CHANNEL_STRUCT_READ
{
	CodeQuality qualityAll;	// суммарный код качества результата
	double time;			// SRV-время результата

	CHANNEL_STRUCT_READ_CALC() : CHANNEL_STRUCT_READ()
		, qualityAll(0)
		, time(0.)
	{}

	~CHANNEL_STRUCT_READ_CALC() {}

	CHANNEL_STRUCT_READ_CALC(const CHANNEL_STRUCT_READ_CALC & val)
		: CHANNEL_STRUCT_READ()
	{
		*this = val;
	}

	CHANNEL_STRUCT_READ_CALC & operator = (const CHANNEL_STRUCT_READ_CALC & val)
	{
		*((CHANNEL_STRUCT_READ*)this) = val;
		qualityAll = val.qualityAll;
		time = val.time;

		return *this;
	}

	CHANNEL_STRUCT_READ_CALC & operator = (const CHANNEL_STRUCT & val)
	{
		*((CHANNEL_STRUCT*)this) = val;
		return *this;
	}

	void ReStart()
	{
		qualityAll = 0;
		time = 0.;
	}
};
//----------------------------------------------------------------------
struct StractParamNext
{
	bool * pbStop;
	SaveStructure * pCFG;
	StructCalcParam * pCalcParam;
	CDSRV * psrv;
	CDZetPath * ppath;
	CHANNEL_STRUCT_READ_CALC * pcsrc;
	
	StractParamNext()
		: pbStop(nullptr)
		, pCFG(nullptr)
		, pCalcParam(nullptr)
		, psrv(nullptr)
		, ppath(nullptr)
		, pcsrc(nullptr)
	{}
};
//----------------------------------------------------------------------
float Calc_F0_min(const float freq_band);
float Calc_F0_max(const float freq_band, const float freq_ADC, const TypeTransform typeTransform);

float Calc_freqBand_min(const float freq_F0, const float freq_ADC);
float Calc_freqBand_max(const float freq_F0, const float freq_ADC, const TypeTransform typeTransform);


// Расчет отображаемого на графиках интервала частот
void Calc_F1_F2(const SaveStructure * pCFG, const StructCalcParam * pCalcParam, double & f1, double & f2);

// Расчет кол-ва строк в грамме
long Calc_NumberGrammaLine(float timeY, long codeUpdate);

// Расчет времени обновления данных
float Calc_TimeUpdata(const long codeUpdata);

// Расчет Min и Max значений времени отображения в грамме
void Calc_TimeViewInGramma(const long codeUpdate, float & timeMin, float & timeMax);

// Преобразование времени в строку
CString ConvertTimeWait(const float timeInSec,
	const CString sHour, const CString sMin, const CString sSec);

// Расчет разрешения по частоте от ширины частотной полосы
float Calc_df_need(const float freq_band, const TypeResolution type);

// Проверка значений на допустимость
bool TestValues(const StructureCFG & cfg,
	const float freq_F0,
	const float freq_band,
	const float freq_ADC);

// Проверка значений CFG на допустимость
bool TestCFG(const SaveStructure & cfg, const float freq_ADC);

// Расчет фактического частотного разрешения
void Calc_df_fact(const StructureCFG & cfg,
	const float freq_ADC,
	float * pdf_fact,
	DWORD * psize_fft);

void Calc_Derivative_Phase(
	Ipp64fc * pValOld_c, Ipp64fc * pValNew_c,
	double valOld, double & valNew, double & dVal);

void WriteCFGtoLogger(
	StructureCFG & cfg,
	UltraZoom_Analysis_CFG & cfgForLogger,
	CZETLoggerClient * pLogger,
	DATE dt,
	bool bFirst);

void CalcResultToLogger(
	float * pData,
	int sizeData,
	SmartArray<BYTE> & saForLogger,
	int sizeArrayForLogger,
	float freq_F0,
	float freq_dF);

void WriteResultToLogger(
	SmartArray<BYTE> & saForLogger,
	CZETLoggerClient * pLogger,
	uint64_t program_ID,
	uint16_t program_Num,
	uint32_t module_ID,
	uint16_t module_Num,
	LPCWCH unitResult,
	GUID channelID,
	DATE dt);
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------