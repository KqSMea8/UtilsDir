//------------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------------
#include "afx.h"
#include "math.h"
#include <Buffer\SmartArray.h>
#include <Intel\ipp\ipps.h>
#include <LDS\lds.h>
//------------------------------------------------------------------------------
#define NumEventInPercentage_MinValue 5.f
//------------------------------------------------------------------------------
// ����� ��� ������ � ����� �������������� �������.
// ����� ������ "��������" ���������. �� ������������� ��������������
// ����� � �������� ������� ���������� �������
class CLdsDataEnergyBand
{
private:
	bool m_bInit;				// ����� ����� � ������
	bool m_bFull;				// ����� ������ ��������
	bool m_bThresholdsEnabled;	// ����� ��������
	DWORD m_CurIndex;			// ������� ������ ������
	float m_Thresholds;			// �������� ������
	float m_Thresholds2;		// ������� �������� � ��������
	float m_alfa;				// �� ��������� = 2
	SmartArray<float> m_saAbsData;		// ������ ������ ������

protected:

public:
	CLdsDataEnergyBand();
	CLdsDataEnergyBand(const CLdsDataEnergyBand &val);
	~CLdsDataEnergyBand();
	bool Create(DWORD size = SizeForTraining, float alfa = 2.f);
	void operator = (const CLdsDataEnergyBand &val);

	// ������� �������
	DWORD GetSize() { return (DWORD)m_saAbsData.size; }
	bool IsInit() { return m_bInit; }
	bool IsThresholdEnabled() { return m_bThresholdsEnabled; }
	void ResetThreshold() { m_bThresholdsEnabled = false; }
	float GetThreshold();
	float GetThreshold2();
	float GetAlfa() { return m_alfa; }
	bool SetAlfa(float val);


	// ������� ������
	// ���������� � ����� ����� ��������
	bool AddAbsValue(float val, float val2, bool &bEvent, float &energyEvent2);
	// ������ ���������� ��������
	bool CalcThresholds();
};
//------------------------------------------------------------------------------
class CLdsEnergy
{
private:
	bool m_bInit;			// ����� ����� � ������
	float m_numBandForEventInPercentage;
	float m_EnergyNoise2;
	DWORD m_numBandForEvent;
	SmartArray<CLdsDataEnergyBand*> m_sapBand;

	void Delete();

protected:

public:
	CLdsEnergy();
	CLdsEnergy(const CLdsEnergy &val);
	~CLdsEnergy();
	bool Create(DWORD numBand, DWORD sizeBand = SizeForTraining, float alfa = 2.f);
	void operator = (const CLdsEnergy &val);

	// ������� �������
	DWORD GetNumBand() { return (DWORD)m_sapBand.size; }
	bool IsInit() { return m_bInit; }

	float GetNumBandForEventInPercentage() { return m_numBandForEventInPercentage; }
	bool SetNumBandForEventInPercentage(float val);
	DWORD GetNumBandForEvent() { return m_numBandForEvent; }
	float GetEnergyNoise();

	// ������� ������
	bool Add(float *pData, bool &bEvent,
		float &sign_noise, float &numBand_float,
		DWORD &indxBandWithEventMin, DWORD &indxBandWithEventMax);
	void CalcThresholds();
	void ResetThreshold();
};
//------------------------------------------------------------------------------