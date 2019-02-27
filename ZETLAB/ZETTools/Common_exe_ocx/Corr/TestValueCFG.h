//------------------------------------------------------------------------------
// Глобальные функции, вызываемые из проектов Corr.exe и ZetCorr.ocx
//------------------------------------------------------------------------------
#pragma once
#include <mathZet.h>
#include <ZetColors.h>
//------------------------------------------------------------------------------
#define colorDanger			ZETColors::ControlColorError
#define colorWarning		ZETColors::ControlColorWarning
#define colorInform			ZETColors::ControlColorOk
//------------------------------------------------------------------------------
// Типы длительности, которая определяет размер анализируемого сигнала
enum TypeTimeDuration : WORD
{// size = 2 ^ (ttd + 8); Duration (mSec) = +- size_4 * 1000 / Fadc * (10 ^ decada)
	ttd_08,		// size = 2 ^ 8 = 256
	ttd_09,
	ttd_10,
	ttd_11,
	ttd_12,
	ttd_13,
	ttd_14,
	ttd_15,
	ttd_16,
	ttd_17,
	ttd_18,
	ttd_19,
	ttd_size
};
//-----------------------------------------------------------------------------
// Тип времени усреднения отображаемого результата
enum TypeTimeAver : WORD
{
	tta_0_1,
	tta_0_2,
	tta_0_4,
	tta_0_5,
	tta_1_s,
	tta_2_s,
	tta_3_s,
	tta_6_s,
	tta_15_s,
	tta_30_s,
	tta_1_m,
	tta_2_m,
	tta_3_m,
	tta_6_m,
	tta_15_m,
	tta_30_m,
	tta_1_h,
	tta_2_h,
	tta_3_h,
	tta_6_h,
	tta_12_h,
	tta_24_h,

	tta_size
};
//-----------------------------------------------------------------------------
enum TypeModeDC : BYTE
{
	typeDC_remove,		// пост. сост. удаляю вычитанием среднего
	typeDC_filtrDC,		// пост. сост. удаляю ФВЧ
	typeDC_yes,			// пост. сост. оставляем

	typeDC_size
};
//-----------------------------------------------------------------------------
// Тип отображаемых значений
enum TypeFuncView : BYTE
{	// Prepare_01
	tfv_corr_envel,		// корр. функция и её огибающая	- res0 + res1
	tfv_envel_corr,		// огибающая функции корр.		- res0 + res1
	tfv_maxTrue,		// функция правдоподобия		- res0
	// Prepare_23
	tfv_corrNorma,		// нормированноя функция корр.	- res0
	tfv_4_func,			// ΔC, AC11, AC22, XC12			- res0 + res1 + res2 + res4
	tfv_4_func_auto,	// ΔC, AC11, AC22, XC12, auto	- res0 + res1 + res2 + res4
	//
	tfv_factorCorr,		// коэфф. корреляции			- res0
	tfv_size
};
//-----------------------------------------------------------------------------
/* Параметры контейнеров (без каналов). Составная часть структур:
- записываемых в CFG-файле программы;
- передаваемых в окно параметров;
- являющихся параметрами контейнеров (STRUCT_CORRELATION).
Значения одинаковых полей этой структуры для разных контейнеров могут быть
разными.
*/
struct StructureCFG
{
	bool bCalcDistance;		// расчет дистанции
	bool bInversion;		// инверсия рассчитанных значений
	bool b2;
	bool b3;

	bool bFiltration;		// фильтрация
	bool bFilteringDiscrete;// фильтрация дискрет медианным фильтром
	bool bWriteToLogger;	// запись в логгер
	BYTE bDecade;			// декада

	TypeModeDC typeModeDC;	// режим работы с пост. сост. сигналов - BYTE
	TypeFuncView typeFuncView;			// тип отображаемой функции	- BYTE
	TypeTimeDuration typeTimeDuration;	// тип длительности - WORD
	float fTimeAver_sec;	// время усреднения отображаемого результата, сек

	float fFreqLowPass;		// частота среза ФНЧ
	float fFreqHighPass;	// частота среза ФВЧ

	float fInterChanDelay;	// временная задержка между каналами, мс
	float fDistance_0;	    // дистанция датчика 0, м
	float fDistance_1;	    // дистанция датчика 1, м
	float fShiftZero_funcA12;	    // сдвиг хитрых функций

	wchar_t sWriteComment[160];	    // комментарий для записи

	StructureCFG();
	void SetValueDefault();
};
//-----------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#define MinOrderFFT			7
#define MaxOrderFFT			19
#define MaxSize				524288	// 2 ^ MaxOrderFFT
#define MaxSize_2			262144	// 2 ^ (MaxOrderFFT - 1)
//------------------------------------------------------------------------------
// Константа, определяющая диапазон частот
#define g_factor_freq_band		0.4f
//-----------------------------------------------------------------------------
// Константа, определяющая граничные значения частот среза полосового фильтра
#define g_factor_filtrBandPass	500000
//-----------------------------------------------------------------------------
// Константа, определяющая максимальное значение смещения А12
#define g_factor_shiftA12		4	// shiftA12 = duration / g_factor_shiftA12
//-----------------------------------------------------------------------------
// Минимальное и максимальное значения межканальной временной задержки, мс
#define g_InterChanDelay_Min		1.f
#define g_InterChanDelay_Max		30000000.f
//-----------------------------------------------------------------------------
// Минимальное и максимальное значения дистанций, м
#define g_Distance_Min		-3000000.f
#define g_Distance_Max		3000000.f
//-----------------------------------------------------------------------------
// Минимальное и максимальное значения времени усреднения, сек
#define g_timeAver_Min			0.1f
#define g_timeAver_Max			86400.f		// (сутки)
//-----------------------------------------------------------------------------
// Максимальное значение декады
#define g_decada_Max			1
//-----------------------------------------------------------------------------
// Максимальное и минимальное значения нормированного сигнала
#define g_valNorma_Min			0.5f
#define g_valNorma_Max			2.f
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Функции проверки новых значений cfg-параметров
//
// Возвращаемое значение: true/false - новое значение входит в ОДЗ.
// Последний параметр функций: bChange - изменять или нет новые значения с целью
// вхождения их в ОДЗ.
//------------------------------------------------------------------------------
bool Test_BandPassFiltr(float & newVal_freq_0, float & newVal_freq_1,
	const float freqADC, const long decada,
	const TypeTestFiltrBandPass typeTest, const bool bChange);

bool Test_InterChanDela(float & newVal, const bool bChange);

bool Test_DistanceChan(float & newVal, const bool bChange);

bool Test_TimeAver(float & newVal, const bool bChange);

bool Test_Decada(BYTE & newVal, const bool bChange);

// Проверка величины Shift_A12
bool Test_ShiftA12(float & newVal,
	const float freqADC, const BYTE decada, const TypeTimeDuration typeTimeDuration,
	const bool bChange, float *pMaxVal = nullptr);
//-----------------------------------------------------------------------------
float GetFminForFiltr(const float freqADC);
float GetFmaxForFiltr(const float freqADC);
//-----------------------------------------------------------------------------