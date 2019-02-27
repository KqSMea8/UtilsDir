//------------------------------------------------------------------------------
//	Файл FOFT.h  - Fractional Octave Frequency Transformation
//	Зеленоград, ЗАО "ЭТМС", Савостин Ю.М.
//------------------------------------------------------------------------------
/*		Класс предназначен для расчёта коэффициентов 
*/
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <list>
//#include <Buffer\SmartArray.h>
#include <Decimation\Decimation.h>
#include <ippFiltrDouble\IirOctaveDouble.h>
//------------------------------------------------------------------------------
#define Freq_Min 1.25
//------------------------------------------------------------------------------
struct ParamOctaveBand 
{
	bool bDecimalData;
	CIirOctaveDouble * pFiltr;
	ParamOctaveBand()
		: bDecimalData(false)
		, pFiltr(NULL)
	{}
	void operator = (const ParamOctaveBand &val)
	{
		bDecimalData = val.bDecimalData;
		if (pFiltr != NULL)
		{
			delete pFiltr;
			pFiltr = NULL;
		}
		if (val.pFiltr != NULL)
		{
			pFiltr = new CIirOctaveDouble();
			pFiltr->Create(val.pFiltr->GetPointerToFreqADC());
			pFiltr->InitFiltr(tfOctave, val.pFiltr->GetFreq(), val.pFiltr->GetOctavePart());
		}
	}
	ParamOctaveBand(const ParamOctaveBand &val)
		: bDecimalData(false)
		, pFiltr(NULL)
	{
		*this = val;
	}
	~ParamOctaveBand()
	{
		if (pFiltr != NULL)
			delete pFiltr;
	}
};
//------------------------------------------------------------------------------
class CFOFT
{
private:
	bool m_bInit;				// экземпляр класса готов к работе
	bool m_bNeedDecim;			// есть фильтры, требующие децимации
	double m_freq_0;
	double m_freq_N;
	double m_FreqADC;
	double m_FreqADC_decim;
	const double m_precision;
	OctavePart m_OctavePart;		// часть октавы
	SmartArray<float> m_result;
	SmartArray<double> m_dataDouble;		// входные данные в double
	SmartArray<double> m_dataDoubleDecim;	// децимированные данные
	std::list<ParamOctaveBand> m_list;	// долеоктавные фильтры с параметрами
	CDecimation *m_pDecimal;		// децимированные данные

	bool PushBackInList(double freqCur, OctavePart octPart, bool bOpportDecimation);

protected:

public:
	CFOFT();
	virtual ~CFOFT();
	bool Create(float freqMin, float freqMax, float freqADC, OctavePart octavePart);

	// методы чтения/записи свойств -----------------
	bool IsInit() { return m_bInit; }

	float GetFreqMin() { return (float)m_freq_0; }
	float GetFreqMax() { return (float)m_freq_N; }

	OctavePart GetOctavePart() { return m_OctavePart; }
	bool SetOctavePart(OctavePart val);

	DWORD GetNumberBand() { return (DWORD)m_list.size(); }
	
	// указатель на результат
	float* GetPointerToResult() {
		return m_bInit ? m_result.pData : NULL; }

	// методы класса ----------------------------
	bool Calculation(double time, int number, float *pData, long quality);
};
//------------------------------------------------------------------------------

