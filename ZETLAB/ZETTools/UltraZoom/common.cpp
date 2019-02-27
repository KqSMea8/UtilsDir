//------------------------------------------------------------------------------
#include "stdafx.h"
#include "common.h"
#include <Intel\ipp\ipps.h>
//------------------------------------------------------------------------------
/*
			Глобальные функции
*/
//------------------------------------------------------------------------------
// Расчет отображаемого интервала частот
void Calc_F1_F2(const SaveStructure * pCFG, const StructCalcParam * pCalcParam, double & f1, double & f2)
{
	f1 = pCalcParam->m_indx_band * pCalcParam->m_dF_fact + pCalcParam->m_dF_fact * 0.5f;
	f2 = f1 + pCalcParam->m_size_grammaX * pCalcParam->m_dF_fact;
}
//------------------------------------------------------------------------------
float Calc_TimeUpdata(const long codeUpdata)
{
	float ret(1.f);
	if (codeUpdata < 1)
		ret = 0.1f;
	else if (codeUpdata > 1)
		ret = 10.f;
	return ret;
}
//------------------------------------------------------------------------------
void Calc_TimeViewInGramma(const long codeUpdate, float & timeMin, float & timeMax)
{
	float time = Calc_TimeUpdata(codeUpdate);
	timeMin = 10 * time;

	timeMax = ULTRA_ZOOM_MAX_LINE_IN_GRAMMA * time;
}
//------------------------------------------------------------------------------
long Calc_NumberGrammaLine(float timeY, long codeUpdate)
{
	float timeMin(0), timeMax(0), time(timeY);
	Calc_TimeViewInGramma(codeUpdate, timeMin, timeMax);
	if (time < timeMin)
		time = timeMin;
	else if (time > timeMax)
		time = timeMax;

	float num = time / Calc_TimeUpdata(codeUpdate);
	long ret = (long)floor(num);
	if (num > (float)ret)
		++ret;

	return ret;
}
//------------------------------------------------------------------------------
CString ConvertTimeWait(const float timeInSec,
	const CString sHour, const CString sMin, const CString sSec)
{
	CString ret;
	CString s;
	UINT t((int)floor(timeInSec));
	div_t dt;

	// часы ----------------------------
	if (t >= 3600)
	{
		dt = div(t, 3600);
		ret.Format(L"%u", dt.quot);
		ret += L" " + sHour + L" ";
		t = dt.rem;
	}
	// минуты --------------------------
	if (t >= 60)
	{
		dt = div(t, 60);
		s.Format(L"%u", dt.quot);
		ret += s + L" " + sMin + L" ";
		t = dt.rem;
	}
	else
	{
		if (!ret.IsEmpty())
			ret += L" 0 " + sMin + L" ";
	}

	// секунды -----------------------
	s.Format(L"%u", t);
	ret += s + L" " + sSec;
	return ret;
}
//------------------------------------------------------------------------------
float Calc_df_need(const float freq_band, const TypeResolution type)
{
	float k(0.1f);
	switch (type)
	{
	case tr_0_1:	k = 0.1f;		break;
	case tr_0_05:	k = 0.05f;		break;
	case tr_0_025:	k = 0.025f;		break;
	case tr_0_01:	k = 0.01f;		break;
	case tr_0_005:	k = 0.005f;		break;
	case tr_0_0025:	k = 0.0025f;	break;
	case tr_0_001:	k = 0.001f;		break;
	}
	return freq_band * k;
}
//------------------------------------------------------------------------------
// Расчет фактического частотного разрешения
void Calc_df_fact(const StructureCFG & cfg,
	const float freq_ADC,
	float * pdf_fact,
	DWORD * psize_fft)
{
	float dF_need = Calc_df_need(cfg.freq_band, cfg.typeResolution);
	DWORD size_fft = 128;		// минимальный размер
	int N = 7;					// 128 = 2 ^ 7

	int N1(N);
	DWORD size1(size_fft);
	DWORD nF0 = (DWORD)(int)floor(freq_ADC / cfg.freq_F0);
	while (size1 < nF0)
	{
		++N1;
		size1 *= 2;
	}

	int N2(N);
	DWORD size2(size_fft);
	auto df = freq_ADC / size2;
	while (df > dF_need)
	{
		++N2;
		size2 *= 2;
		df = freq_ADC / size2;
		if (cfg.typeTransform == tt_Wigner_Ville)
			df *= 0.5f;
	}
	if (N1 > N2)
	{
		N = N1;
		size_fft = size1;
	}
	else
	{
		N = N2;
		size_fft = size2;
	}
	df = freq_ADC / size_fft;
	if (cfg.typeTransform == tt_Wigner_Ville)
		df *= 0.5f;

	if (pdf_fact)
		*pdf_fact = df;
	if (psize_fft)
		*psize_fft = size_fft;
}
//------------------------------------------------------------------------------
void Calc_Derivative_Phase(
	Ipp64fc * pValOld_c, Ipp64fc * pValNew_c,
	double valOld, double & valNew, double & dVal)
{
	valNew = atan2(pValNew_c->im, pValNew_c->re);
	if (_finite(valOld) != 0)
	{// val1 - ok
		dVal = valNew - valOld;
		if ((pValOld_c->re < 0.f) && (pValNew_c->re < 0.f))
		{
			bool b1 = (pValOld_c->im > 0.f) && (pValNew_c->im < 0.f);
			bool b2 = (pValOld_c->im < 0.f) && (pValNew_c->im > 0.f);
			if (b1 || b2)
			{
				if (b1)
					dVal += IPP_2PI;
				if (b2)
					dVal -= IPP_2PI;
			}
		}
	}
	else	// val1 - NAN
		dVal = 0;
}
//------------------------------------------------------------------------------
void WriteCFGtoLogger(
	StructureCFG & cfg,
	UltraZoom_Analysis_CFG & cfgForLogger,
	CZETLoggerClient * pLogger,
	DATE dt,
	bool bFirst)
{
	if (pLogger == nullptr)
		return;

	bool bFlag = bFirst ||
		(*((GUID*)cfgForLogger.channelID) != cfg.channelGuid) ||
		(cfgForLogger.freq_F0 != cfg.freq_F0) ||
		(cfgForLogger.freq_band != cfg.freq_band) ||
		(cfgForLogger.typeResolution != cfg.typeResolution) ||
		(cfgForLogger.time_grammaY != cfg.time_grammaY) ||
		(cfgForLogger.code_updata != cfg.code_updata);

	if (bFlag)
	{
		*((GUID*)cfgForLogger.channelID) = cfg.channelGuid;
		cfgForLogger.freq_F0 = cfg.freq_F0;
		cfgForLogger.freq_band = cfg.freq_band;
		cfgForLogger.typeResolution = (uint8_t)cfg.typeResolution;
		cfgForLogger.time_grammaY = cfg.time_grammaY;
		cfgForLogger.code_updata = cfg.code_updata;

		ZetHeader * pzh = (ZetHeader*)&cfgForLogger;
		if (dt != 0.)
		{
			long err = ztSetStructureTimeLocal(pzh, dt);
		}
		else
		{
			long err = ztSetStructureCurrentTimeUTC(pzh);
		}

		if (0 == ztCloseStructure(pzh))
		{
			pLogger->writeStructureToBuff(pzh);
		}
	}
}
//------------------------------------------------------------------------------
// Расчет параметров максимума и массива для записи в логгер
//
void CalcResultToLogger(
	float * pData,			// исходные данные (часть результата FFT)
	int sizeData,			// размер исходных данных (нечетное число)
	SmartArray<BYTE> & saForLogger,	// буфер для структуры
	int sizeArrayForLogger,			// размер массива в структуре (нечетное число)
	float freq_F0,			// значение частоты в центральной полосе
	float freq_dF)			// частотное разрешение
{
	if ((pData == nullptr) ||
		(sizeData < 3) || (sizeArrayForLogger < 3))
		return;

	UINT sizeInByte = ZETLAB_ULTRAZOOM_ANALYSIS_RESULT_MIN_SIZE +
		sizeArrayForLogger * sizeof(ArrayForLogger);

	if (!saForLogger.ReSize(sizeInByte) ||
		(0 != ztOpenStructure((ZetHeader*)saForLogger.pData, ZETLAB_ULTRAZOOM_ANALYSIS_RESULT_TYPE, sizeInByte)))
		return;

	int size_2_src = sizeData / 2;
	int size_2 = sizeArrayForLogger / 2;
	int indx_F0 = size_2 + 1;
	size_2 = min(size_2, size_2_src);
	UltraZoom_Analysis_Result * pStructResult = (UltraZoom_Analysis_Result *)saForLogger.pData;

	ArrayForLogger * pArray_F0 = (ArrayForLogger *)pStructResult->_ + indx_F0;

	// максимум в исходных данных ----------------------------------------
	// индекс максимума в исходных данных
	int indx = 0;
	// индекс центральной частоты (равной freq_F0 Гц) в исходных данных (точная середина)
	int indx_F0_src = size_2_src + 1;
	ippsMaxIndx_32f(pData, sizeData, &pStructResult->amplMax, &indx);
	pStructResult->freqMax = freq_F0 + (indx - indx_F0_src) * freq_dF;	

	// данные в массиве структуры ----------------------------------------
	// центральная частота
	pArray_F0->x = freq_F0;
	pArray_F0->y = pData[indx_F0_src];
	// правее центральной частоты
	float freqOld = freq_F0;
	ArrayForLogger * pArrayCur = pArray_F0 + 1;
	for (int i = 1; i <= size_2; ++i, ++pArrayCur)
	{
		freqOld += freq_dF;
		pArrayCur->x = freqOld;
		pArrayCur->y = pData[indx_F0_src + i];
	}
	// левее центральной частоты
	freqOld = freq_F0;
	pArrayCur = pArray_F0 - 1;
	for (int i = 1; i <= size_2; ++i, --pArrayCur)
	{
		freqOld -= freq_dF;
		pArrayCur->x = freqOld;
		pArrayCur->y = pData[indx_F0_src - i];
	}
}
//------------------------------------------------------------------------------
// Подготовка структуры и ее запись
//
void WriteResultToLogger(
	SmartArray<BYTE> & saForLogger,	// буфер для структуры
	CZETLoggerClient * pLogger,
	uint64_t program_ID,
	uint16_t program_Num,
	uint32_t module_ID,
	uint16_t module_Num,
	LPCWCH unitResult,				// размерность результата
	GUID channelID,
	DATE dt)						// абсолютное время результата
{
	if ((pLogger == nullptr) && (saForLogger.size > 0))
		return;

	UltraZoom_Analysis_Result * pStructResult = (UltraZoom_Analysis_Result *)saForLogger.pData;

	// параметры канала
	pStructResult->numberOfChannels = 1;
	*((GUID*)pStructResult->channelID) = channelID;

	//	размерность результата - надо перевести в char
	int indx = min(wcslen(unitResult), 15);
	WideCharToMultiByte(CP_ACP, 0, unitResult, indx, pStructResult->unitResult, indx, NULL, NULL);
	pStructResult->unitResult[indx] = 0;
	
	ZetHeader * pzh = &pStructResult->zet_header;

	if ((0 == ztSetStructureTimeLocal(pzh, dt)) &&
		(0 == ztSetProgramValuesInHeader(pzh, program_ID, program_Num)) &&
		(0 == ztSetModuleValuesInHeader(pzh, module_ID, module_Num)) &&
		(0 == ztCloseStructure(pzh)))
	{
		pLogger->writeStructureToBuff(pzh);
	}
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/*
			Функции SaveStructure
*/
//------------------------------------------------------------------------------
StructureCFG::StructureCFG()
{
	SetValueDefault();
}
//------------------------------------------------------------------------------
StructureCFG::StructureCFG(const StructureCFG & val)
{
	*this = val;
}
//------------------------------------------------------------------------------
StructureCFG & StructureCFG::operator = (const StructureCFG & val)
{
	UINT size = sizeof(StructureCFG);
	memcpy_s(this, size, &val, size);
	return *this;
}
//------------------------------------------------------------------------------
bool StructureCFG::ChangeNo(StructureCFG & cfgNew)
{
	return
		(bWindow_sigF0 == cfgNew.bWindow_sigF0) &&
		(bWindow_sigAver == cfgNew.bWindow_sigAver) &&
		(typeTransform == cfgNew.typeTransform) &&
		(typeResolution == cfgNew.typeResolution) &&
		(code_updata == cfgNew.code_updata) &&
		(time_grammaY == cfgNew.time_grammaY) &&
		(freq_F0 == cfgNew.freq_F0) &&
		(freq_band == cfgNew.freq_band) &&
		(channelGuid == cfgNew.channelGuid);
}
//------------------------------------------------------------------------------
bool StructureCFG::ChangeOnlyFlagWindow(StructureCFG & cfgNew)
{
	return
		(typeResolution == cfgNew.typeResolution) &&
		(typeTransform == cfgNew.typeTransform) &&
		(code_updata == cfgNew.code_updata) &&
		(time_grammaY == cfgNew.time_grammaY) &&
		(freq_F0 == cfgNew.freq_F0) &&
		(freq_band == cfgNew.freq_band) &&
		(channelGuid == cfgNew.channelGuid);
}
//------------------------------------------------------------------------------
bool StructureCFG::ChangeOnlyGrammaTime(StructureCFG & cfgNew)
{
	return
		(bWindow_sigF0 == cfgNew.bWindow_sigF0) &&
		(bWindow_sigAver == cfgNew.bWindow_sigAver) &&
		(typeTransform == cfgNew.typeTransform) &&
		(typeResolution == cfgNew.typeResolution) &&
		(code_updata == cfgNew.code_updata) &&
		(freq_F0 == cfgNew.freq_F0) &&
		(freq_band == cfgNew.freq_band) &&
		(channelGuid == cfgNew.channelGuid);

}
//------------------------------------------------------------------------------
void StructureCFG::SetValueDefault(CDSRV * psrv, long chan)
{
	bWindow_sigF0 = false;
	bWindow_sigAver = false;
	typeTransform = tt_Wigner_Ville;
	typeResolution = tr_0_1;

	time_grammaY = 10.f;
	code_updata = 1;
	channel = chan;

	float freq_ADC(100.f);
	if (psrv)
	{
		freq_ADC = psrv->Frequency(channel);
		psrv->IdChan(channel, &channelGuid);
	}
	else
	{
		channelGuid = GUID_NULL;
	}
	freq_F0 = 0.01f * freq_ADC;
	freq_band = 0.1f * freq_F0;
}
//------------------------------------------------------------------------------
SaveStructure::SaveStructure() : CustomSaveStructure()
{
	cfg.SetValueDefault();

	ZeroMemory(placement, sizeof(placement));
	UINT size = sizeof(WINDOWPLACEMENT);
	for (auto & it : placement)
		it.length = size;
}
//------------------------------------------------------------------------------
SaveStructure::SaveStructure(const SaveStructure & val)
{
	*this = val;
}
//------------------------------------------------------------------------------
SaveStructure & SaveStructure::operator = (const SaveStructure & val)
{
	UINT size(sizeof(SaveStructure));
	memcpy_s(this, size, &val, size);
	return *this;
}
//------------------------------------------------------------------------------
/*
		Функции StructCalcParam
*/
//------------------------------------------------------------------------------
StructCalcParam::StructCalcParam()
		: m_bGrammaIsFull(false)
		, m_bDataSrcIsFull(false)
		, m_bFirstPage(true)
		, m_bNeedCalc(false)
		, m_size_fft(0)
		, m_indx_F0(0)
		, m_indx_band(0)
		, m_num_src(0)
		, m_num_grammaY(0)
		, m_num_NewDataCur(0)
		, m_size_grammaY(0)
		, m_size_grammaX(0)
		, m_pSruct_FFT(nullptr)
		, m_pSruct_Hilbert(nullptr)
		, m_dF_fact(0.)
		, m_extinction_1(1.f)
{}
//------------------------------------------------------------------------------
StructCalcParam::~StructCalcParam()
{
// 	if (m_pSruct_FFT)
// 	{ 
// 		ippsFFTFree_C_64fc(m_pSruct_FFT);
// 		m_pSruct_FFT = nullptr;
// 	}
// 	if (m_pSruct_Hilbert)
// 	{ 
// 		ippsHilbertFree_32f32fc(m_pSruct_Hilbert);
// 		m_pSruct_Hilbert = nullptr;
// 	}
}
//------------------------------------------------------------------------------
void StructCalcParam::CalcSize(const float freq_ADC, const SaveStructure & cfg)
{	// Определяю m_size_fft и m_dF_fact
	if (freq_ADC < 100.f)
		return;

	float dF_need = Calc_df_need(cfg.cfg.freq_band, cfg.cfg.typeResolution);

	m_size_fft = 128;		// минимальный размер
	int N = 7;				// 128 = 2 ^ 7

	int N1(N);
	DWORD size1(m_size_fft);
	DWORD nF0 = (DWORD)(int)floor(freq_ADC / cfg.cfg.freq_F0);
	while (size1 < nF0)
	{
		++N1;
		size1 *= 2;
	}

	int N2(N);
	DWORD size2(m_size_fft);
	auto df = freq_ADC / size2;
	while (df > dF_need)
	{
		++N2;
		size2 *= 2;
		df = freq_ADC / size2;
		if (cfg.cfg.typeTransform == tt_Wigner_Ville)
			df *= 0.5f;
	}
	if (N1 > N2)
	{
		N = N1;
		m_size_fft = size1;
	}
	else
	{
		N = N2;
		m_size_fft = size2;
	}
	// подготовка к FFT
	m_len_2 = (int)(m_size_fft / 2);
	m_dF_fact = freq_ADC / m_size_fft;
	if (cfg.cfg.typeTransform == tt_Wigner_Ville)
		m_dF_fact *= 0.5f;
	IppStatus is(ippStsNoErr);
	int order = (int)log2(m_size_fft);
	int size_struct_FFT(0), size_init_FFT(0), size_work_FFT(0);
	is = ippsFFTGetSize_C_64fc(order, IPP_FFT_DIV_FWD_BY_N, ippAlgHintNone,
						&size_struct_FFT, &size_init_FFT, &size_work_FFT);
	m_sa8u_Sruct_FFT.ReSize(size_struct_FFT);
	m_sa8u_Work_FFT.ReSize(size_init_FFT);
	ippsFFTInit_C_64fc(&m_pSruct_FFT, order, IPP_FFT_DIV_FWD_BY_N, ippAlgHintNone, m_sa8u_Sruct_FFT.pData, m_sa8u_Work_FFT.pData);
	m_sa8u_Work_FFT.ReSize(size_work_FFT);

// 	if (m_pSruct_FFT)
// 	{
// 		is = ippsFFTFree_C_64fc(m_pSruct_FFT);
// 		m_pSruct_FFT = nullptr;
// 	}
// 	is = ippsFFTInitAlloc_C_64fc(&m_pSruct_FFT, N, IPP_FFT_DIV_FWD_BY_N, ippAlgHintNone);
	
// 	ippsFFTGetBufSize_C_32fc(m_pSruct_FFT, &N1);
// 	m_sa_tmp_FFT.ReSize(N1);

	// Определяю m_size_grammaY
	m_size_grammaY = Calc_NumberGrammaLine(cfg.cfg.time_grammaY, cfg.cfg.code_updata);

	m_indx_F0 = (int)floor(cfg.cfg.freq_F0 / m_dF_fact + 0.5f);
	// Определяю m_size_grammaX - кол-во отображаемых частотных полос
	// порядок строк кода менять нельзя
	m_size_grammaX = (int)floor(cfg.cfg.freq_band / m_dF_fact * 0.5f);
	m_indx_band = m_indx_F0 - m_size_grammaX;
	m_size_grammaX *= 2;
	++m_size_grammaX;

	if (cfg.cfg.code_updata == 0)
	{
		m_num_NewDataMax = (DWORD)(int)floor(freq_ADC * 0.1f);
	}
	else if (cfg.cfg.code_updata == 1)
	{
		m_num_NewDataMax = (DWORD)(int)floor(freq_ADC * 1.f);
	}
	else if (cfg.cfg.code_updata == 2)
	{
		m_num_NewDataMax = (DWORD)(int)floor(freq_ADC * 10.f);
	}
}
//------------------------------------------------------------------------------
float StructCalcParam::CalcTimeAver(const float freq_ADC)
{
	return m_size_fft / freq_ADC;
}
//------------------------------------------------------------------------------
bool StructCalcParam::ReSize()
{// При таком коде вызовутся все ReSize
	// массивы размером m_size_fft
	if ((m_size_fft == 0) ||
		(m_size_grammaY == 0) ||
		(m_size_grammaX == 0))
		return false;

	bool ret(true);
	bool b(true);
	
	b = m_sa_src_sig_32.ReSize(m_size_fft);
	ret &= b;
	b = m_sa_src_sigRe_32.ReSize(m_size_fft);
	ret &= b;
	b = m_sa_src_sigRe_64.ReSize(m_size_fft);
	ret &= b;
	b = m_sa1_analit_32.ReSize(m_size_fft);
	ret &= b;
	b = m_sa2_analit_32.ReSize(m_size_fft);
	ret &= b;
	b = m_sa_analit_64.ReSize(m_size_fft);
	ret &= b;
	b = m_sa_fft_Ampl_64.ReSize(m_size_fft);
	ret &= b;

	// массивы размером m_size_grammaY
	b = m_sa_f0_Ampl.ReSize(m_size_grammaY);
	ret &= b;
	b = m_sa_f0_Faza.ReSize(m_size_grammaY);
	ret &= b;
	b = m_sa_f0_dFaz.ReSize(m_size_grammaY);
	ret &= b;

	// массивы размером m_size_grammaX
	b = m_saLineGramma_new.ReSize(m_size_grammaX);
	ret &= b;
	b = m_saLineGramma_old.ReSize(m_size_grammaX);
	ret &= b;
	b = m_saLineGramma_sum2.ReSize(m_size_grammaX);
	ret &= b;
	b = m_saLineGramma_aver.ReSize(m_size_grammaX);
	ret &= b;

	return ret;
}
//------------------------------------------------------------------------------
void StructCalcParam::ReStart()
{
	m_bGrammaIsFull = false;
	m_bDataSrcIsFull = false;
	m_bFirstPage = true;
	m_bNeedCalc = false;

	m_num_grammaY = 0;
	m_num_src = 0;
	m_num_NewDataCur = 0;
	m_fazaOld_32f = NAN;
	m_fazaOld_32fc.re = NAN;
	m_fazaOld_32fc.im = NAN;

	m_sa_src_sig_32.Zero();
	m_sa_src_sigRe_32.Zero();
	m_sa_src_sigRe_64.Zero();
	m_sa1_analit_32.Zero();
	m_sa2_analit_32.Zero();
	m_sa_analit_64.Zero();
	m_sa_fft_Ampl_64.Zero();

	m_sa_f0_Ampl.Zero();
	m_sa_f0_Faza.Zero();
	m_sa_f0_dFaz.Zero();

	m_saLineGramma_new.Zero();
	m_saLineGramma_old.Zero();
	m_saLineGramma_sum2.Zero();
	m_saLineGramma_aver.Zero();
}
//------------------------------------------------------------------------------
double StructCalcParam::Calc_Extinction(int size, TypeTransform typeTransform)
{
	static SmartArray<double> saArray;
	saArray.ReSize(size);
	double ret(1.f);
	ippsSet_64f(ret, saArray.pData, size);
	ippsWinBartlett_64f_I(saArray.pData, size);
	ippsMean_64f(saArray.pData, size, &ret);
	switch (typeTransform)
	{
	case tt_FFT:
		ret = 4. / (ret * ret);
		break;
	case tt_Wigner_Ville:
		ret = 1. / ( 2. * ret * ret * ret * ret);
		break;
	default:
		break;
	}
	return ret;
}
//------------------------------------------------------------------------------
// Копирование исходных данных в сдвиговый буфер m_sa_src_sig_32,
// определение сдвига, текущих ылагов, размеров и т.д.
//
// Входные данные:
// saData - размер 100 при Fqdc = 100 Гц и cfg.code_updata = 1
//
// Выходные данные:
// m_sa_src_sig
// m_pSruct_Hilbert
// m_extinction_1
// m_num_src
// m_bDataSrcIsFull

bool StructCalcParam::CopyInpData(
	SmartArray<float> & saData,	// saArray в CHANNEL_STRUCT_READ
	CZetTimeSpan & ztsData,		// время данных
	StructureCFG & cfg)			// CFG
{	// анализ времени -------------------------------------
	if (cfg.code_updata == 1)
	{// целое число секунд
		m_bNeedCalc = 0 == ztsData.GetNanoseconds();
	}
	else if (cfg.code_updata == 2)
	{// целое число секунд && кол-во секунд кратно 10
		m_bNeedCalc = false;
		if (0 == ztsData.GetNanoseconds())
		{
			int64_t m = ztsData.GetSeconds();
			int64_t m10 = (m / 10) * 10;
			m_bNeedCalc = m10 == m;
		}
	}
	else
		m_bNeedCalc = true;

	bool ret(true);
	m_num_NewDataCur += saData.size;
	m_bNeedCalc &= m_num_NewDataCur >= m_num_NewDataMax;

	// ----------------------------------------------------
	if (saData.size <= m_size_fft)
	{
		ret &= m_sa_src_sig_32.NewDataInShiftBuffer(saData.pData, saData.size);
		bool bFlag(false);
		if (!m_bDataSrcIsFull)
		{
			m_num_src += saData.size;
			m_shift = (int)(m_size_fft - m_num_src);
			if (m_num_src >= m_size_fft)
			{
				m_num_src = m_size_fft;
				m_shift = 0;
				m_bDataSrcIsFull = true;
				bFlag = true;
			}
			if (m_bNeedCalc)
			{
				bFlag = true;
			}
		}

		if (bFlag)
		{
			m_extinction_1 = Calc_Extinction(m_num_src, cfg.typeTransform);
			if (cfg.typeTransform == tt_Wigner_Ville)
			{
				if (m_pSruct_Hilbert != nullptr)
				{
					int sizeSpec(0), sizeBuf(0);
					ippsHilbertGetSize_32f32fc(m_num_src, ippAlgHintNone, &sizeSpec, &sizeBuf);
					m_sa8u_Sruct_Hilbert.ReSize(sizeSpec);
					m_sa8u_Work_Hilbert.ReSize(sizeBuf);
					ippsHilbertInit_32f32fc(m_num_src, ippAlgHintNone, m_pSruct_Hilbert, m_sa8u_Sruct_Hilbert.pData);
				}
// 				if (m_pSruct_Hilbert != nullptr)
// 				{
// 					ippsHilbertFree_32f32fc(m_pSruct_Hilbert);
// 					m_pSruct_Hilbert = nullptr;
// 				}
// 				ippsHilbertInitAlloc_32f32fc(&m_pSruct_Hilbert, (int)m_num_src, ippAlgHintAccurate);
			}
		}
	}
	else
	{// маловероятно, но на всякий случай!!!
		// saData.size > m_size_fft. Беру последние m_size_fft
		ret &= m_sa_src_sig_32.Copy(saData.pData + (saData.size - m_size_fft), m_size_fft);
		m_num_src = m_size_fft;
		m_shift = 0;

		if (!m_bDataSrcIsFull)
		{
			m_extinction_1 = Calc_Extinction((int)m_size_fft, cfg.typeTransform);
			if (cfg.typeTransform == tt_Wigner_Ville)
			{
				if (m_pSruct_Hilbert != nullptr)
				{
					int sizeSpec(0), sizeBuf(0);
					ippsHilbertGetSize_32f32fc(m_num_src, ippAlgHintNone, &sizeSpec, &sizeBuf);
					m_sa8u_Sruct_Hilbert.ReSize(sizeSpec);
					m_sa8u_Work_Hilbert.ReSize(sizeBuf);
					ippsHilbertInit_32f32fc(m_num_src, ippAlgHintNone, m_pSruct_Hilbert, m_sa8u_Sruct_Hilbert.pData);
				}
// 				if (m_pSruct_Hilbert != nullptr)
// 				{
// 					ippsHilbertFree_32f32fc(m_pSruct_Hilbert);
// 					m_pSruct_Hilbert = nullptr;
// 
// 				}
// 				ippsHilbertInitAlloc_32f32fc(&m_pSruct_Hilbert, (int)m_num_src, ippAlgHintAccurate);
			}
			m_bDataSrcIsFull = true;
		}
	}

	if (m_bNeedCalc)
	{
		m_num_NewDataCur = 0;
	}
	return ret;
}
//------------------------------------------------------------------------------
// Расчет комплексного аналитического сигнала и наложения на него временного окна
//
// Входные данные:
// m_sa_src_sig
// m_pSruct_Hilbert
// m_extinction_1
// m_num_src
// m_bDataSrcIsFull
//
// Выходные данные:
//	m_sa_src_analit

bool StructCalcParam::Calc_AnaliticSignal_FTT()
{
	IppStatus st(ippStsNoErr);
	bool ret(true);
	// переход в double
	st = ippsConvert_32f64f(m_sa_src_sig_32.pData + m_shift, m_sa_src_sigRe_64.pData + m_shift, m_num_src);
	// удаление пост. составляющей
	double mean(0.);
	st = ippsMean_64f(m_sa_src_sigRe_64.pData + m_shift, m_num_src, &mean);
	ret &= st == ippStsNoErr;
	st = ippsSubC_64f_I(mean, m_sa_src_sigRe_64.pData + m_shift, m_num_src);
	ret &= st == ippStsNoErr;
	// временное окно
	st = ippsWinBartlett_64f_I(m_sa_src_sigRe_64.pData + m_shift, m_num_src);
	ret &= st == ippStsNoErr;
	// аналитический сигнал, у которого мнимая часть всегда 0
	for (UINT i = 0; i < m_size_fft; ++i)
	{
		auto p = m_sa_analit_64.pData + i;
		p->re = m_sa_src_sigRe_64.pData[i];
		p->im = 0.f;
	}
	ret &= st == ippStsNoErr;

	return ret;
}
//------------------------------------------------------------------------------
// Расчет исходных данных для Wigner_Vill
void Wigner_Ville(
	const Ipp32fc * pInpData,	// исходные данные
	Ipp32fc * pOutData,			// выходные данные
	const DWORD len)			// размер данных
{
	DWORD N_1 = len - 1;
	std::complex<float> val1, val2;
	
	if (len & 1)
	{// не четное число ----------------------------------------------
		for (DWORD n = 0; n < len; ++n)
		{
			val1 = *(std::complex<float>*)(pInpData + n);
			val2 = *(std::complex<float>*)(pInpData + (N_1 - n));
			val1 = val1 * conj(val2);
			pOutData[n] = *(Ipp32fc*)&val1;
		}
	}
	else
	{// четное число -------------------------------------------------
		DWORD N_2 = N_1 - 1;
		for (DWORD n = 0; n < N_1; ++n)
		{
			val1 = *(std::complex<float>*)(pInpData + n);
			val2 = *(std::complex<float>*)(pInpData + (N_2 - n));
			val1 = val1 * conj(val2);
			pOutData[n] = *(Ipp32fc*)&val1;
		}
		val1 = *(std::complex<float>*)(pInpData + N_1);
	//	val1 = val1 * conj(val1);
		val1 = norm(val1);	
		pOutData[N_1] = *(Ipp32fc*)&val1;
	}
}
//------------------------------------------------------------------------------
bool StructCalcParam::Calc_AnaliticSignal_Wigner_Ville()
{	// преобразование Гильберта есть только для float
	IppStatus st(ippStsNoErr);
	bool ret(true);

	// удаление пост. составляющей
	float mean(0.f);
	st = ippsMean_32f(m_sa_src_sig_32.pData + m_shift, m_num_src, &mean, ippAlgHintNone);
	ret &= st == ippStsNoErr;
	st = ippsSubC_32f(m_sa_src_sig_32.pData + m_shift, mean, m_sa_src_sigRe_32.pData + m_shift, m_num_src);
	ret &= st == ippStsNoErr;
	// временное окно
	st = ippsWinBartlett_32f_I(m_sa_src_sigRe_32.pData + m_shift, m_num_src);
	ret &= st == ippStsNoErr;
	// Преобразование Гильберта
	st = ippsHilbert_32f32fc(m_sa_src_sigRe_32.pData + m_shift, m_sa1_analit_32.pData + m_shift, m_pSruct_Hilbert, m_sa8u_Work_Hilbert.pData);
	ret &= st == ippStsNoErr;
	// расчет квадрата
	Wigner_Ville(m_sa1_analit_32.pData + m_shift, m_sa2_analit_32.pData + m_shift, m_num_src);
	// переход в double. В IPP 7.0 такой функции нет, поэтому руками 
	auto p32 = m_sa2_analit_32.pData + m_shift;
	auto p64 = m_sa_analit_64.pData  + m_shift;
	for (UINT i = 0; i < m_num_src; ++i, ++p32, ++p64)
	{
		p64->re = p32->re;
		p64->im = p32->im;
	}
	return ret;
}
//------------------------------------------------------------------------------
bool StructCalcParam::Calc_Spectr()
{	// расчет FFT и нормировка на окно
	IppStatus st(ippStsNoErr);
	bool ret(true);
	// Выполнение FFT
	st = ippsFFTFwd_CToC_64fc_I(m_sa_analit_64.pData, m_pSruct_FFT, NULL);
	ret &= st == ippStsNoErr;
	// Расчет спектральной плотности мощности
	st = ippsPowerSpectr_64fc(m_sa_analit_64.pData, m_sa_fft_Ampl_64.pData, m_len_2);
	ret &= st == ippStsNoErr;
	// поправка на временное окно
	st = ippsMulC_64f_I(m_extinction_1, m_sa_fft_Ampl_64.pData, m_len_2);
	ret &= st == ippStsNoErr;
	// переход к исходной величине
	st = ippsSqrt_64f_I(m_sa_fft_Ampl_64.pData, m_len_2);
	ret &= st == ippStsNoErr;

// 	st = ippsMagnitude_64fc(m_sa_analit.pData, m_sa_fft_Ampl.pData, len_2);
// 	ret &= st == ippStsNoErr;

	return ret;
}
//------------------------------------------------------------------------------
bool StructCalcParam::NewData(
	float time_updata,
	StructureCFG & cfg)
{	// Расчет аналитического сигнала и спектра -------------------------------
	bool ret(true);
	if (cfg.typeTransform == tt_FFT)
		ret = Calc_AnaliticSignal_FTT();
	else
		ret = Calc_AnaliticSignal_Wigner_Ville();

	ret &= Calc_Spectr();

	if (ret)
	{	// данные доп. окон рассчитываю независимо от того, отображаются они или нет
		IppStatus st(ippStsNoErr);
		bool b(true);

		if (cfg.typeTransform == tt_Wigner_Ville)
		{
			st = ippsSqrt_64f_I(m_sa_fft_Ampl_64.pData, m_len_2);
			ret &= st == ippStsNoErr;
		}
		
		// новая строка для граммы ---------------------------------------------
		st = ippsConvert_64f32f(m_sa_fft_Ampl_64.pData + m_indx_band, m_saLineGramma_new.pData, m_size_grammaX);
		ret &= st == ippStsNoErr;
		// данные для графиков сигнала F0 --------------------------------------
		b = m_sa_f0_Ampl.NewDataInShiftBuffer((float)m_sa_fft_Ampl_64.pData[m_indx_F0]);
		ret &= b;
		auto pc = m_sa_analit_64.pData + m_indx_F0;
		double valNew(0.);
		double dVal(0.);
		Calc_Derivative_Phase(&m_fazaOld_32fc, pc, m_fazaOld_32f, valNew, dVal);

		b = m_sa_f0_Faza.NewDataInShiftBuffer((float)valNew);
		ret &= b;
		dVal *= 180. / (IPP_PI * time_updata);
		b = m_sa_f0_dFaz.NewDataInShiftBuffer((float)dVal);
		ret &= b;
		m_fazaOld_32fc = *pc;
		m_fazaOld_32f = valNew;

		// данные для графика среднего сигнала ---------------------------------
		// добавляю новую строку

		st = ippsSqr_32f(m_saLineGramma_new.pData, m_saLineGramma_aver.pData, m_size_grammaX);
		ret &= st == ippStsNoErr;
		st = ippsAdd_32f_I(m_saLineGramma_aver.pData, m_saLineGramma_sum2.pData, m_size_grammaX);
		ret &= st == ippStsNoErr;

		float factor(0.);
		if (m_bGrammaIsFull)
		{	// вычитаю старую строку
			factor = 1.f / m_size_grammaY;
			st = ippsSqr_32f(m_saLineGramma_old.pData, m_saLineGramma_aver.pData, m_size_grammaX);
			ret &= st == ippStsNoErr;
			st = ippsSub_32f_I(m_saLineGramma_aver.pData, m_saLineGramma_sum2.pData, m_size_grammaX);
			ret &= st == ippStsNoErr;
		}
		else
		{
			factor = 1.f / ++m_num_grammaY;
			m_bGrammaIsFull = m_num_grammaY > m_size_grammaY;
		}
		// нормировка и извлечение корня
		st = ippsMulC_32f(m_saLineGramma_sum2.pData, factor, m_saLineGramma_aver.pData, m_size_grammaX);
		ret &= st == ippStsNoErr;
		st = ippsAbs_32f_I(m_saLineGramma_aver.pData, m_size_grammaX);		// надо ?????
		ret &= st == ippStsNoErr;
		st = ippsSqrt_32f_I(m_saLineGramma_aver.pData, m_size_grammaX);
		ret &= st == ippStsNoErr;
	}
	return ret;
}
//------------------------------------------------------------------------------
/*		Формулы и неравенства
m_dF_need = freq_band * typeResolution

2 * freq_band <= freq_F0 - freq_band / 2
freq_F0 + freq_band / 2 <= 0.45 * freq_ADC

0.5 * freq_ADC / 1000 <= freq_band <= 0.5 * freq_ADC / 20
k1 = 0.5 * freq_ADC / freq_band

freq_band / (16 * 1024) <= m_dF_need <= freq_band / 128
k2 = freq_band / m_dF_need

		Округлять через строку в следующих 4-х функциях
(Calc_F0_min, Calc_F0_max, Calc_freqBand_min, Calc_freqBand_max)
надо обязателдьно, иначе граничные значения могут не устанавливаться.
*/
//------------------------------------------------------------------------------
float Calc_F0_min(const float freq_band)
{// 2 * freq_band <= freq_F0 - freq_band / 2
	float ret = 2.5f * freq_band;
	CString s;
	s.Format(L"%.3f", ret);
	ret = (float)_wtof(s);
	return ret;
}
//------------------------------------------------------------------------------
float Calc_F0_max(const float freq_band, const float freq_ADC, const TypeTransform typeTransform)
{// freq_F0 + freq_band / 2 <= 0.45 * freq_ADC
	float factor(0.45f);
	switch (typeTransform)
	{
// 	case tt_FFT:
// 		factor = 0.45f;
// 		break;
	case tt_Wigner_Ville:
		factor = 0.225f;		// = 0.45f * 0.5f;
		break;
	default:
		break;
	}
	float ret = factor * freq_ADC - 0.5f * freq_band;
	CString s;
	s.Format(L"%.3f", ret);
	ret = (float)_wtof(s);

	return ret;
}
//------------------------------------------------------------------------------
float Calc_freqBand_min(const float freq_F0, const float freq_ADC)
{	// f3 = 0.5 * freq_ADC / 2000 <= freq_band
	float ret = freq_ADC * 0.5f / 2000;
	CString s;
	s.Format(L"%.3f", ret);
	ret = (float)_wtof(s);
	return ret;
}
//------------------------------------------------------------------------------
float Calc_freqBand_max(const float freq_F0, const float freq_ADC, const TypeTransform typeTransform)
{	// f1 => 2 * freq_band <= freq_F0 - freq_band / 2
	float f1 = 0.4f * freq_F0;

	// f2 => freq_F0 + freq_band / 2 <= 0.45 * freq_ADC
	float factor(0.45f);
	switch (typeTransform)
	{
		// 	case tt_FFT:
		// 		factor = 0.45f;
		// 		break;
	case tt_Wigner_Ville:
		factor = 0.225f;		// = 0.45f * 0.5f;
		break;
	default:
		break;
	}
	float f2 = 2 * (factor * freq_ADC - freq_F0);

	// f3 => freq_band <= 0.5 * freq_ADC / 20
	float f3 = freq_ADC * 0.025f;

	float ret = min(min(f1, f2), f3);
	CString s;
	s.Format(L"%.3f", ret);
	ret = (float)_wtof(s);
	return ret;
}
//------------------------------------------------------------------------------
bool TestValues(const StructureCFG & cfg,
	const float freq_F0,
	const float freq_band,
	const float freq_ADC)
{
	float val = Calc_F0_min(freq_band);
	bool ret = val <= freq_F0;
	val = Calc_F0_max(freq_band, freq_ADC, cfg.typeTransform);
	ret &= freq_F0 <= val;

	val = Calc_freqBand_min(freq_F0, freq_ADC);
	ret &= val <= freq_band;
	val = Calc_freqBand_max(freq_F0, freq_ADC, cfg.typeTransform);
	ret &= freq_band <= val;

	return ret;
}
//------------------------------------------------------------------------------
bool TestCFG(const SaveStructure & cfg, const float freq_ADC)
{
	return TestValues(cfg.cfg, cfg.cfg.freq_F0, cfg.cfg.freq_band, freq_ADC);
}
//------------------------------------------------------------------------------