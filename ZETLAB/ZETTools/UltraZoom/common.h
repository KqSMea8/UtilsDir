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
/*		������� � �����������
	m_dF_need = freq_band * typeResolution
	
	2 * freq_band <= freq_F0 - freq_band/2
	freq_F0 + freq_band/2 <= 0.45 * freq_ADC
	
	0.5 * freq_ADC / 1000 <= freq_band <= 0.5 * freq_ADC / 20
	k1 = 0.5 * freq_ADC / freq_band

	freq_band / (16 * 1024) <= m_dF_need <= freq_band / 128
	k2 = freq_band / m_dF_need

	������ ������ � ������� � 4-� ������� ����������� ����������.
	�������� ������ ������ �.�. ����������� ����, ��� F0 ������ freq_ADC �� ������.

	��� ������������� ������ ���� ����� ���������:
	- ��������� ������� ������ m_time_ADC2 - ��������� ������ ���� ������ 1 ���.
	- ������ ����� ������ - ����� ����� ��� + m_time_ADC2

	����� ���������� ���������� ��������� � = k1 * k2 / freq_ADC (� ���-�� * �� 4)

	������� ��������:
	1.	�������� ������ ������� freq_ADC - ��� �������� ������� ������� m_freq_ADC2		
		m_saCos, m_saSin, m_saSig_src
		������ - ��� ����� ����� �������� ������� F0 => N_F0 = m * freq_ADC / freq_F0
		m >= 2 ���, ����� N_F0 >= 20

	2. ������������� ������ ������� m_freq_ADC2 - ��� ����������� � ���� Sig_F0
		m_saSig_F0_Re, m_saSig_F0_Im, m_saSig_F0_Am, m_saSig_F0_Fi_grad
		������ N_Re = interval_SigF0 * m_freq_ADC2;

	3. m_saLineGramma_new, m_saLineGramma_old, m_saLineGramma_sum2, m_saLineGramma_aver
		������ N_LG = 2 * freq_band / m_dF_fact

	���-�� ����� � ������ N_G = grammaTime * m_freq_ADC2;
	m_bGrammaIsFull = m_numLineInGrammaCur == m_numLineInGrammaMax;

	m_time_ADC2 = 0.1 ��� (m_freq_ADC2 = 10 ��)
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
	bool bWindow_sigF0;				// ���� ����������� ���. ���� sigF0
	bool bWindow_sigAver;			// ���� ����������� ���. ���� sigAver
	TypeTransform typeTransform;	// ���� ��� ��������������
	TypeResolution typeResolution;	// ��� ���������� ���������� ����������
	float freq_F0;					// ��������� ����������� �������, ��

	float freq_band;				// ������������ ������ �������, ��
	float time_grammaY;				// ����� ����������� ������ � ������, ���

	long channel;					// ����� �������
	long code_updata;				// ��� ���������� (0 - 0,1 ���; 1 - 1 ���; 2 - 10 ���)

	long l1;
	long l2;

	GUID channelGuid;				// GUID ������ �������

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
	WINDOWPLACEMENT placement[3];	// ��������� ���. ����
	
	SaveStructure();
	SaveStructure(const SaveStructure & val);
	SaveStructure & operator = (const SaveStructure & val);
};
//------------------------------------------------------------------------------
// ��������� ����������, ������������ ����� UNIT-2
struct SrtuctParamForUNIT
{
	DWORD sizeInByte;		// ������ ���������, �����
	DWORD sizeX;			// ������ ������ �� �, �������
	DWORD sizeY;			// ������ ������ �� Y, �����
	float val_F0;			// ����������� �������� ����������� �������, ��
	float val_dF;			// ����������� ����. ����������, ��
	float val_band;			// ������������ ������ ������� (����� �� CFG)
	GUID channelID;			// GUID ������
	wchar_t unitResult[16];	// ����������� �������

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
// ��������� ���������� ������ ������ ��� ���������� ��������
struct StructCalcParam
{	// ������� ������� ����������� �������
	bool m_bGrammaIsFull;		// � ������ ��������� ��� ����� �� Y
	bool m_bDataSrcIsFull;		// � ������� �������� ������ ���� ��� �������
	bool m_bFirstPage;			// ������ ������ � �������� ������
	bool m_bNeedCalc;			// ����� ��������� ����������
	DWORD m_size_fft;			// ������ ��� FFT = 2 ^ N

	int m_len_2;				// = m_size_fft / 2
	int m_indx_F0;				// ������ �����. ������� � ������� ����� FFT
	int m_indx_band;			// ������ ������ ������ ��� ������

	int m_shift;				// �������� ������� �������� ������ � �������� �������
	DWORD m_num_NewDataCur;		// ������� ���-�� ������, ��� �� �������� � ���������

	DWORD m_num_NewDataMax;		// ������������ ���-�� ������, ��� �� �������� � ���������
	DWORD m_num_src;			// ������� ���-�� �������� � �������� ������

	DWORD m_num_grammaY;		// ������� ���-�� ����� � ������
	DWORD m_size_grammaY;		// ������������ ���-�� ����� � ������

	DWORD m_size_grammaX;		// ���-�� ����� � ����� ������
	IppsFFTSpec_C_64fc * m_pSruct_FFT;			// ��������� FFT
	SmartArray<Ipp8u> m_sa8u_Sruct_FFT;
	SmartArray<Ipp8u> m_sa8u_Work_FFT;
	IppsHilbertSpec * m_pSruct_Hilbert;	// ��������� �������������� ���������
	SmartArray<Ipp8u> m_sa8u_Sruct_Hilbert;
	SmartArray<Ipp8u> m_sa8u_Work_Hilbert;
	
// 	IppsHilbertSpec_32f32fc * m_pSruct_Hilbert;	// ��������� �������������� ���������

	double m_fazaOld_32f;		// ��������� �������� float-����
	double m_dF_fact;			// ����������� ��������� ����������
	double m_extinction_1;		// �������� ������� ����������� ������������� ���������� ����
	Ipp64fc m_fazaOld_32fc;		// ���������� ����������� �������� ����. �� ����� �� ����������� �������

	CZetTimeSpan m_ztsDataTimeSRV;	// ����� ������

	// ������� �������� m_size_fft
	SmartArray<float> m_sa_src_sig_32;		// �������� ������
	SmartArray<float> m_sa_src_sigRe_32;	// Re ������ float
	SmartArray<double> m_sa_src_sigRe_64;	// Re ������ double
	SmartArray<Ipp32fc> m_sa1_analit_32;	// ����������� ������ float
	SmartArray<Ipp32fc> m_sa2_analit_32;	// ����������� ������ float
	SmartArray<Ipp64fc> m_sa_analit_64;		// ����������� ������ double �� � ����� FFT
	SmartArray<double> m_sa_fft_Ampl_64;	// ��������� double ����� FFT

	// ������� �������� m_size_grammaY
	SmartArray<float> m_sa_f0_Ampl;			// ��� �����������
	SmartArray<float> m_sa_f0_Faza;			// ��� �������� � ��������
	SmartArray<float> m_sa_f0_dFaz;			// ��� ����������� ����������� ����


	// ������� �������� m_size_grammaX
	SmartArray<float> m_saLineGramma_new;	// ����� ������ � ������
	SmartArray<float> m_saLineGramma_old;	// ��������� ������ � ������
	SmartArray<float> m_saLineGramma_sum2;	// ������������� ������ - ����� ���������
	SmartArray<float> m_saLineGramma_aver;	// ���������� ������ ��� �����������
	
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
	CodeQuality qualityAll;	// ��������� ��� �������� ����������
	double time;			// SRV-����� ����������

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


// ������ ������������� �� �������� ��������� ������
void Calc_F1_F2(const SaveStructure * pCFG, const StructCalcParam * pCalcParam, double & f1, double & f2);

// ������ ���-�� ����� � ������
long Calc_NumberGrammaLine(float timeY, long codeUpdate);

// ������ ������� ���������� ������
float Calc_TimeUpdata(const long codeUpdata);

// ������ Min � Max �������� ������� ����������� � ������
void Calc_TimeViewInGramma(const long codeUpdate, float & timeMin, float & timeMax);

// �������������� ������� � ������
CString ConvertTimeWait(const float timeInSec,
	const CString sHour, const CString sMin, const CString sSec);

// ������ ���������� �� ������� �� ������ ��������� ������
float Calc_df_need(const float freq_band, const TypeResolution type);

// �������� �������� �� ������������
bool TestValues(const StructureCFG & cfg,
	const float freq_F0,
	const float freq_band,
	const float freq_ADC);

// �������� �������� CFG �� ������������
bool TestCFG(const SaveStructure & cfg, const float freq_ADC);

// ������ ������������ ���������� ����������
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