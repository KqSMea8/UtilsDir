//  ласс дл€ вычислени€ коэффициента нелинейных искажений 
// последовательного вычислени€ гармоник по методу √ерцел€

#pragma once

#include <vector>
#include <Intel\nsp.h>
//#include "ToolTipListCtrl.h"


class CGoertzel
{
private:
	double* m_data;		//! массив с исходными данным
	double* m_sin;		//! массив с синусом заданной частоты
	double* m_cos;		//! массив с косинусом заданной частоты
	double* m_outdata;
	double* m_outpogr;

	std::vector<DCplx> spectr;
	std::vector<double> win;

	std::vector<double> freqs;
	std::vector<double> phase;
	std::vector<double> power;

	double m_dTHD;
	double m_dTHDN;
	double m_dSNR;
	double m_dSFDR;
	double m_dSINAD;
	double m_dENOB;
	double m_dFS;
	double m_dENOB_FS;

	double m_dRate;		//! частота дискретизации данных
	double m_dMax;		//! максимальное значение по каналу
	double m_dMin;		//! минимальное значение которое могут принимать характеристики
	double m_dMaxEnob;	//! максимальна€ разр€дность дл€ ј÷ѕ
	double m_dResolution;	//!	разрешающа€ способность канала

	long m_size;
	long m_order;		//! пор€док ‘урье преобразовани€

	CString m_sUnit;	//! единицы измерени€ по каналу
	CString m_sMax;

	void CheckSize(long size);
	void CheckMinimum();
	void Calculate();
	void CalculateCharacteristic();
	void SinusCosinus(double freq, double amplitude, double phase);
	
	double Frequency();
	double Convolution(double &phase);	//! вычисление амплитуды синуса по методу √ерцел€

public:
	CGoertzel();
	~CGoertzel();

	void SetSamplingRate(double rate);			//! задать частоту дискретизации
	void SetMaxChannelValue(double dmax);		//!	задать максимальное значение по каналу
	void SetChannelUnits(CString unit);			//! задать единицы измерени€ по каналу
	void SetResolution(double resol);			//!	задать разрешающую способность канала

	void Calculate(float* data, long size);
	void Calculate(double* data, long size);
	//void PrintResultOnListControl(CToolTipListCtrl* pList, bool bLog);

	double GetTHD()   { return m_dTHD; };
	double GetTHDN()  { return m_dTHDN; };
	double GetSNR()   { return m_dSNR; };
	double GetSFDR()  { return m_dSFDR; };
	double GetSINAD() { return m_dSINAD; };
	double GetENOB()  { return m_dENOB; };
	double GetFS()	  { return m_dFS; };
	double GetENOBFS()  { return m_dENOB_FS; };

	double GetHarmonic(long index);
	double GetFrequency(long index);

	long GenerateOutputData(float* signal, float* distortion, long size);
	long GenerateOutputData(double* signal, double* distortion, long size);
};

