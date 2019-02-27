// ����� ��� ���������� ������������ ���������� ��������� 
// ����������������� ���������� �������� �� ������ �������

#pragma once

#include <vector>
#include <Intel\nsp.h>
//#include "ToolTipListCtrl.h"


class CGoertzel
{
private:
	double* m_data;		//! ������ � ��������� ������
	double* m_sin;		//! ������ � ������� �������� �������
	double* m_cos;		//! ������ � ��������� �������� �������
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

	double m_dRate;		//! ������� ������������� ������
	double m_dMax;		//! ������������ �������� �� ������
	double m_dMin;		//! ����������� �������� ������� ����� ��������� ��������������
	double m_dMaxEnob;	//! ������������ ����������� ��� ���
	double m_dResolution;	//!	����������� ����������� ������

	long m_size;
	long m_order;		//! ������� ����� ��������������

	CString m_sUnit;	//! ������� ��������� �� ������
	CString m_sMax;

	void CheckSize(long size);
	void CheckMinimum();
	void Calculate();
	void CalculateCharacteristic();
	void SinusCosinus(double freq, double amplitude, double phase);
	
	double Frequency();
	double Convolution(double &phase);	//! ���������� ��������� ������ �� ������ �������

public:
	CGoertzel();
	~CGoertzel();

	void SetSamplingRate(double rate);			//! ������ ������� �������������
	void SetMaxChannelValue(double dmax);		//!	������ ������������ �������� �� ������
	void SetChannelUnits(CString unit);			//! ������ ������� ��������� �� ������
	void SetResolution(double resol);			//!	������ ����������� ����������� ������

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

