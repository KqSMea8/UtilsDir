//------------------------------------------------------------------------------
// Функции обработки данных
#pragma once
//------------------------------------------------------------------------------
#include "afx.h"
#include "float.h"
#include <ippFDWT\OncePacketFDWT.h>
#include <mathZet.h>
#include <Buffer\SmartArray.h>
#include <ZetTools\include\ZetTools.h>
//------------------------------------------------------------------------------
enum Type_Fault
{// флаги типов неисправностей
	fault_Ok,				// неисправности нет
	fault_NoChan = 1,		// нет канала с заданным именем
	fault_OffChan = 2,		// канал отключен
	fault_OffPower = 4,		// питание датчика не в норме
	fault_ErrRead = 8,		// ошибка при чтении данных
	fault_NaN = 16,			// исходные данные содержат NaN
	fault_NoRange = 32,		// значение не входит в ОДЗ
	fault_NoRangeMin = 64,	// значение не входит в ОДЗ (меньше минимального)
	fault_NoRangeMax = 128,	// значение не входит в ОДЗ (больше максимального)
	fault_ErrReadFile = 256,// ошибка при чтении файла
	fault_YesChan = 512,	// появился канал с заданным именем
	fault_LessThreshold = 1024,	// значение меньше порогового
	fault_MoreThreshold = 2048,	// значение больше порогового
	fault_NotWork = 4096,	// внешний запрет на работы
	fault_OnPower = 8192,	// питание датчика в норме
	fault_FreqADC = 16384,	// неправильная частота дискретизации
};
//------------------------------------------------------------------------------
// Поиск начала события с помощью вейвлет-фильтрации.
// Используется в СКСВ и СОУ
bool SearchPositionBeginUsingFDWT(PVOID pointerDataInp, PVOID pointerDataOut,
	int dimention, int sizeAll, int sizePreHistory,
	int *pPositionLeft, int *pPositionRight,
	TypeWavelet typeWavelet = db2, int levelWavelet = 4, int numPointNotZero = 1);

// Проверка данных на ошибки
bool TestDataFromSRV(float *pData, DWORD sizeData,
	float minValue, float maxValue, int &fault, bool bTestNaN,
	int *pindxMin = NULL, int *pindxMax = NULL);

bool WriteDTU(CString nameFileFull, CString title,
	int size, float FreqADC,
	int numData, DATE T0, double t0, bool bAmpl,
	float *pdata0, float *pdata1, float *pdata2,
	CString name0, CString name1, CString name2,
	CString conv0, CString conv1, CString conv2);

bool CalcAmpl_32f(const float *px, const float *py, const float *pz,
	float *pAmpl, int number, bool bNeedSqrt,
	float *px2 = nullptr, float *py2 = nullptr, float *pz2 = nullptr);
bool CalcAmpl_32f_I(float *px, float *py, float *pz,
	float *pAmpl, int number, bool bNeedSqrt);

bool CalcAmpl_64f(const double *px, const double *py, const double *pz,
	double *pAmpl, int number, bool bNeedSqrt,
	double *px2 = nullptr, double *py2 = nullptr, double *pz2 = nullptr);
bool CalcAmpl_64f_I(double *px, double *py, double *pz,
	double *pAmpl, int number, bool bNeedSqrt);
//------------------------------------------------------------------------------