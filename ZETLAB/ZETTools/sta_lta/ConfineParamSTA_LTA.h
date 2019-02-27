//--------------------------------------------------------------------------
// ���� ConfineParamSTA_LTA.h
// ������ �� 24.09.2012
// ��� "����", ����������, �������� �.�.
//--------------------------------------------------------------------------
//	����� ������������ ��� ����� �����������, ������������� �� ���������
// ��������� STA_LTA. ���������� ������ ������������ � ���������
// "�������� STA_LTA" � � ��������������� ���������� SCADA
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <ippFiltr\IirFiltrBand.h>
//--------------------------------------------------------------------------
class CConfineParamSTA_LTA
{
private:
	float *pFreqADC;
	int SizeMax;
	CIirFiltrBand *pFiltr;
	bool bFiltrInternal;
	float PriorValueSTA(float newVal, float valLTA);
	float PriorValueLTA(float newVal, float valSTA);
protected:

public:
	CConfineParamSTA_LTA();
	virtual ~CConfineParamSTA_LTA();
	bool Create(float *freqADC, int sizeMax, CIirFiltrBand *pfiltr = NULL);

	// ���. �������� ��������� LTA � STA (=10)
	float m_min_Div_LTA_STA;
	// ����. �������� ��������� ����� LTA � STA � ����. ������ (=0.9)
	float m_max_Div_Sum_Tall;
	// ���. �������� STA � �������� ��� (=10)
	float m_min_Div_Sta_TimeADC;
	// ���. ����� �������������� (=1.5)
	float m_minThreshold;
	// ����. ����� �������������� (=60)
	float m_maxThreshold;
	// ���. �  ����. �������� ����������� ������
	DWORD m_minCompressRatio;		// = 400
	DWORD m_maxCompressRatio;		// = 4000

	// ������� ������
	float GetMinFmin() { return pFiltr->PriorValueFmin(1e-7f); }
	void NewFreqBand(float *pFmin, float *pFmax,
		float newFmin, float newFmax, short bNewParam);

	float GetMinSTA();
	float GetMaxSTA();
	float GetMinLTA();
	float GetMaxLTA();
	void NewDuration(float *pSta,  float *pLta,
					 float newSta, float newLta);

	void NewThreshold(float *pThreshold, float newThreshold);
	void NewCompressRatio(DWORD *pCompressRatio, DWORD newVal);
};
//--------------------------------------------------------------------------