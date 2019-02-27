//------------------------------------------------------------------------------
//	Файл CustomPacketDWT.h
//	Зеленоград, ЗАО "ЭТМС", Савостин Ю.М.
//------------------------------------------------------------------------------
//		Базовый класс предназначен для расчёта коэффициентов дискретного
// вейвлет преобразования (Fast Discrete Wavelet Transform) одномерных
// сигналов в соответствии с пакетным алгоритмом. В классе создаётся
// двумерные массивы структур для прямого и обратного FDWT.
//		pppFwdState[i][n] - это указатель на структуру прямого FDWT
// (i+1)-ого уровня разложения, n-ой полосы.
//		Класс является наследником базового класса CCustomFDWT.
//		В классе используются функции библитеки IPP 7.0.
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <Buffer\SmartArray.h>
#include <ippFDWT\CustomFDWT.h>
//------------------------------------------------------------------------------
template <class T>				// T - это IppsWTFwdState_32f или IppsWTInvState_32f
struct fdwtState
{
	T * pState;
	SmartArray<Ipp8u> sa;

	fdwtState(const int sizeInByte = 0)
		: pState(nullptr)
		, sa()
	{
		ReSize(sizeInByte);
	}

	bool ReSize(const int sizeInByte)
	{
		bool ret = sizeInByte > 0;
		if (ret)
		{
			ret = sa.ReSize((DWORD)sizeInByte);
			pState = ret ? ((T*)sa.pData) : nullptr;
		}
		return ret;
	}
};
//------------------------------------------------------------------------------
class CCustomPacketFDWT : public CCustomFDWT
{
private:
	int nLevelForState;		// параметр размерности массивов структур FDWT

	bool CreateFwdState();	// создание структур прямого FDWT
	bool CreateInvState();	// создание структур обратного FDWT
	void DeleteFwdState();	// удаление структур прямого FDWT
	void DeleteInvState();	// удаление структур обратного FDWT
	bool _InitForward();	// инициализация структур прямого FDWT
	bool _InitInverse();	// инициализация структур обратного FDWT

protected:
	int nSizeMassThreshold;	// размер этого массива порогов
	fdwtState<IppsWTFwdState_32f> ***pppFwdState;	// ук. на двухмерные массивы
	fdwtState<IppsWTInvState_32f> ***pppInvState;	//		структур FDWT
// 	IppsWTFwdState_32f ***pppFwdState;	// ук. на двухмерные массивы
// 	IppsWTInvState_32f ***pppInvState;	//		структур FDWT
	int nLevelForBuf;		// размерность массива коэффициентов FDWT
	int FQuantityBand;		// кол-во полос высшего уровня разложения
	int m_pNumBand[32];		// массив кол-в полос в уровне
	bool _Init();			// инициализация всех структур
	bool CreateState();		// создание всех структур
	void DeleteState();		// удаление всех структур
	
	// Виртуальные функции, реализующие побочные свойстыа, т.е.
	virtual void NewTypeWavelet() { _Init(); }	// кол-во структур
	virtual void NewLevel();					//		FDWT

public:
	CCustomPacketFDWT(void);
	virtual ~CCustomPacketFDWT(void);
	virtual long Create(TypeWavelet typeWavelet, int level, bool bneedInverse,
		int lengthSignal);

	// массив указателей на коэффициенты FDWT последнего уровня
	float **m_ppBandData;

	// методы чтения/записи свойств -----------------
	int GetQuantityBand() { return FQuantityBand; }

	// методы класса ----------------------------
	float* GetPointerToTopLevelFactors(int band);
	int GetMaxFactorInBand(int band, float *pValMax = NULL);

	float BandWidth(float *pFreqADC)	// ширина полос
		{ return *pFreqADC / m_pNumBand[FLevel] * 0.5f; }
	float BandWidthHalf(float *pFreqADC)	// половина ширины
		{ return BandWidth(pFreqADC) * 0.5f; }
	float BandMiddleFreq(float *pFreqADC, int band);
	void  BandMiddleFreqAll(float *pFreqADC, float *pFreqBand);
};
//------------------------------------------------------------------------------

