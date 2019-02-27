//----------------------------------------------------------------------
// Класс реализует потоковый медианный фильтр
//----------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <Intel\ipp\ipps.h>
#include <FiltrDef.h>
#include <Buffer\SmartArray.h>
//----------------------------------------------------------------------
struct IppsMdaState_32f
{
	int len;		// длина фильтра
	int len_1;		// длина фильтра без 1
	int *pDistIndx;	// массив выходных индексов, размер len
	int *pTmpIndx;	// массив временных индексов, размер len
	float *pTail;	// хвост данных, размер len-1
	int IndxMda;	// индекс среднего значения
	BOOL bOdd;		// нечетная длина фильтра
	bool bEnabledTail;	// массив pTail задан

	IppsMdaState_32f();
	~IppsMdaState_32f();
	bool Create(int lenght);
	void Delete();
	void ReStart();
};
//----------------------------------------------------------------------
class CMedianFiltr
{
private:
	IppsMdaState_32f *m_pState;	// структура фильтра
	float *m_pData;				// промежуточный массив данных и его
	int m_sizeData;				//			размер
	SmartArray<Ipp8u> m_sa_8u;

protected:
	bool m_bInit;				// флаг инициализации фильтра
	TypeFiltr m_FType;			// тип фильтра
	float *m_pFreqADC;			// ук. на частоту дискретизации в Гц

	bool _InitFiltr(int lenght);
	bool _Filtration(float *pInpData, float *pOutData, int num);

public:
	CMedianFiltr();
	virtual ~CMedianFiltr();
	bool Create(float *pfreqADC);

	// методы чтения/записи свойств -----------------
	TypeFiltr GetTypeFiltr() { return m_FType; }
	BOOL GetOdd();
	int GetLengthInPoint();
	float GetLengthInTime();

	// методы класса --------------------------------
	// Виртуальная функция инициализации фильтра новыми параметрами.
	// В наследниках необходимо переопределять
	bool InitFiltrInPoint(int lenght);
	bool InitFiltrInTime(float lenTime);

	// Функции фильтрации
	bool Filtration(float *pInpData, float *pOutData, int num);
	bool Filtration_I(float *pInpOutData, int num);

	// Переинициализация фильтра со старыми параметрами
	bool ReStart();
};
//----------------------------------------------------------------------

