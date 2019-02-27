//------------------------------------------------------------------------------
//	���� Decimation.h
//	����������, ��� "����", �������� �.�.
//------------------------------------------------------------------------------
/*		����� ������������ ��� ������������, �������� � ������ ��������������
������. ������ �����, ������� ������ � ����� �������������� ������ - ���
������ ����� � ��������, � �������� ����� ����� ������. ����� - ������ 10-���.
		����� ���������� �������� ������ ����������� ���������� � ������� ���,
� �������� ������� ����� ����� 5% �� ������� ������������� �������� ������.
		����� �� ������������ ������, ��� ������� �������� ������� �������������
�������� ������.
*/
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <Buffer\SmartArray.h>
#include <Decimation\BufferForDataWithTime.h>
#include <ippFiltrDouble\IirFiltrInternalDouble.h>
//------------------------------------------------------------------------------
class CDecimation
{
private:
	bool m_bInit;
	bool m_bEnableFirst;
	BYTE m_index;
	const BYTE m_indexMax;
	double m_FreqADC;
	double m_dt;
	const double m_precision;
	double m_lastTime;
	CBufferForDataWithTime<double> *m_pBufferDecimal;
	CIirFiltrInternalDouble *m_pFiltr;
	SmartArray<double> m_dataDouble;

protected:

public:
	CDecimation();
	virtual ~CDecimation();
	bool Create(float freqADC, DWORD sizeDecimalBuffer = 0);

	// ������ ������/������ ������� -----------------
	bool GetIsInit() { return m_bInit; }
	// ������� ������ ����� Create
	DWORD GetSizeDataDecimal() {
		return m_bInit ? m_pBufferDecimal->GetSize() : 0; }
	
	// ��������� �� ���������

	// ������ ������ ----------------------------
	bool PutData(double time, int number, float *pData, long quality);
	long GetData(double time, int number, double *pData, long *pQuality = NULL);
	long GetData(double time, int number, float *pData, long *pQuality = NULL);
};
//------------------------------------------------------------------------------

