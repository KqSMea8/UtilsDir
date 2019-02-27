//------------------------------------------------------------------------------
// Функции обработки данных
#pragma once
//------------------------------------------------------------------------------
#include "afx.h"
#include <Intel\ipp\ipps.h>
#include <ZetErrorCodes.h>	// коды ощибок
// #include "float.h"
// #include <mathZet.h>
// #include <ZetTools\include\ZetTools.h>
//------------------------------------------------------------------------------
// Удаление постоянной составляющей сигнала
bool RemovingConstComponent_32f(const float *pSrc, float *pDst, int len, float *mean = nullptr);
bool RemovingConstComponent_32f_I(float *pSrcDst, int len, float *mean = nullptr);

bool RemovingConstComponent_64f(const double *pSrc, double *pDst, int len, double *mean = nullptr);
bool RemovingConstComponent_64f_I(double *pSrcDst, int len, double *mean = nullptr);
//------------------------------------------------------------------------------
// Преобразование float в дБ
bool ConvertAmplToDB_32f(const float referAmpl, const float *pSrc, float *pDst, int len, const bool bSrcNegative = true);
bool ConvertAmplToDB_32f_I(const float referAmpl, float *pSrcDst, int len, const bool bSrcNegative = true);

bool ConvertPowerToDB_32f(const float referAmpl, const float *pSrc, float *pDst, int len, const bool bSrcNegative = true);
bool ConvertPowerToDB_32f_I(const float referAmpl, float *pSrcDst, int len, const bool bSrcNegative = true);

// Преобразование float в дБ с ограничением снизу
bool ConvertAmplToDB_32f(const float referAmpl, const float subLevelAmpl, const float *pSrc, float *pDst, int len, const bool bSrcNegative = true);
bool ConvertAmplToDB_32f_I(const float referAmpl, const float subLevelAmpl, float *pSrcDst, int len, const bool bSrcNegative = true);

bool ConvertPowerToDB_32f(const float referAmpl, const float subLevelPower, const float *pSrc, float *pDst, int len, const bool bSrcNegative = true);
bool ConvertPowerToDB_32f_I(const float referAmpl, const float subLevelPower, float *pSrcDst, int len, const bool bSrcNegative = true);

// Преобразование double в дБ
bool ConvertAmplToDB_64f(const double referAmpl, const double *pSrc, double *pDst, int len, const bool bSrcNegative = true);
bool ConvertAmplToDB_64f_I(const double referAmpl, double *pSrcDst, int len, const bool bSrcNegative = true);

bool ConvertPowerToDB_64f(const double referAmpl, const double *pSrc, double *pDst, int len, const bool bSrcNegative = true);
bool ConvertPowerToDB_64f_I(const double referAmpl,  double *pSrcDst, int len, const bool bSrcNegative = true);
//------------------------------------------------------------------------------
// Поиск значимого экстремума. Исходный ряд должен быть отцентрирован.
// Значимый - это такой, значение которого больше произведения СКО на factor
bool ExtremumOverNose_32f(const float *pSrc, const int len, int & xExt, float & yExt, const float factor = 3.f);
bool ExtremumOverNose_64f(const double *pSrc, const int len, int & xExt, double & yExt, const double factor = 3.);
//------------------------------------------------------------------------------
// Поиск пикового значения. Ряд должен быть отцентрированным
bool FindPeak_32f(const float *pSrc, const int len, int & xPeak);
//----------------------------------------------------------------------------------------------------------------------
// Определение оптимального порядка фильтра по соотношению частот среза и дискретизации
const UINT getFilterOrder(_In_ const double fs, _In_ const double fadc);
//----------------------------------------------------------------------------------------------------------------------
//--------------------------------------- Функции работы с резонансами -------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
// Поиск возможных резонансных частот и оценка добротности по фазовой характеристике
// Параметры:
//		pSrcPhase	- массив данных фазовой характеристики
//		size		- размер массива данных фазовой характеристики
//		freq		- частота дискретизации
//		phase		- пороговое значение фазы для поиска резонансов
//		deltaPhase	- ширина коридора значений фазы для поиска резонансов
//		f1			- начало диапазона частот, в котором можно искать
//		f2			- конец диапазона частот, в котором можно искать
//		ppFreq		- указатель, куда будут записаны найденные возможные резонансные частоты
//		ppQ			- указатель, куда будут записаны добротности
//		count		- количество найденных возможных резонансных частот
int calcPossibleResonancesByPhase(_In_ const double* pSrcPhase, _In_ const int size, _In_ const double freq,
	_In_ const double phase, _In_ const double deltaPhase, _In_ const double f1, _In_ const double f2,
	_Out_ double** ppFreq, _Out_ double** ppQ, _Out_ int& count);
//----------------------------------------------------------------------------------------------------------------------
// Расчет точных резонансной и собственной частоты по оценочному значению резонансной частоты и добротности
// Параметры:
//		pSrcAmpl	- массив данных амплитудной характеристики
//		size		- размер массива данных амплитудной характеристики
//		freqADC		- частота дискретизации
//		fResSrc		- оценка значения резонансной частоты
//		q			- оценка значения добротности
//		f0			- точное значение собственной частоты
//		fResDst		- точное значение резонансной частоты
//		amplRes		- точное значение максимума амплитудной характеристики на резонансной частоте
long calcResonanceFreqs(_In_ const double* pSrcAmpl, _In_ const int size, _In_ const double freqADC,
	_In_ const double fResSrc, _In_ const double q, _Out_ double& f0, _Out_ double& fResDst, _Out_ double& amplRes);
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------- Функции расчета импульсных характеристик -----------------------------------------
//----------------------------------------------------------------------------------------------------------------------
struct СalcImpRespState
{
	double freq = 0.;		// частота дискретизации сигнала
	int needLpf = 0;		// 0 - ФНЧ не нужен, 1 - ФНЧ нужен
	int needHpf = 0;		// 0 - ФВЧ не нужен, 1 - ФВЧ нужен
	double lpfFreq = 0.;	// частота среза ФНЧ
	double hpfFreq = 0.;	// частота среза ФВЧ
	int direction = 0;		// 0 - прямая импульсная характеристика, 1 - обратная импульсная характеристика
	int noPhase = 0;		// 0 - учитывать фазу при расчете, 1 - не учитывать фазу при расчете
	int symm = 0;			// 0 - несимметричная, 1 - симметричная
};
// Функция расчета импульсной характеристики
// param - параметры расчета
// pCrossSpectrum - взаимный спектр
// pAmp2Spectr1 - квадрат амплитуды спектра первого канала
// pAmp2Spectr2 - квадрат амплитуды спектра второго канала
// pMemory - дополнительная память для расчета (размер size*2)
// size - размер массивов
// gain - коэффициент передачи по постоянной составляющей
// pImpResp - рассчитанная импульсная характеристика
int CalcImpRespByCrossSpectrum(_In_ const СalcImpRespState& param, _In_ const Ipp64fc* pCrossSpectrum,
	_In_ const double* pAmp2Spectr1, _In_ Ipp64fc* pMemory, _In_ const int size, _In_ const Ipp64fc gain, _Out_writes_all_(size * 2) double* pImpResp);
//----------------------------------------------------------------------------------------------------------------------
int CalcImpRespByCrossSpectrum(_In_ const СalcImpRespState& param, _In_ const double* pH12, const double* pPhase,
	_In_ Ipp64fc* pMemory, _In_ const int size, _Out_writes_all_(size * 2) double* pImpResp);
//----------------------------------------------------------------------------------------------------------------------