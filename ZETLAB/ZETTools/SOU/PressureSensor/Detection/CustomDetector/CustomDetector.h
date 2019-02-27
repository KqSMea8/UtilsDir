//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------  ���� CustomDetector.h  -----------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
/*	������� ����� ���������� ��� ����� ���. �������� ����������������, ���� �����. ����� ����������� �� ������, � ������ 
�������� "�����" ������, ������� ������ �� ����������.
	�������� ������������ ��� ������ ���������� ��������� � ��������� �������� ��� ��� ������������� �� ������ ����� 
	���������.
*/
#define MaxSizeBuffer	100000	// 1 000 ��� ��� Fadc = 100 ��
//----------------------------------------------------------------------------------------------------------------------
#pragma once
#include "Intel\ipp\ipps.h"	// ������ SNR
#include <Buffer\ShiftBufferArbitraryPage.h>
#include <SOU\PressureSensor\Detection\Struct_CFG\Lds_cfg_Detector_PressureProxy.h>
//----------------------------------------------------------------------------------------------------------------------
using namespace zet::configuration::schema;
//----------------------------------------------------------------------------------------------------------------------
enum TypeEventYesNo
{
	ted_No,			// ������� ���
	ted_Start,		// ������� ��������
	ted_Continue,	// ������� �����������
	ted_End,		// ������� �����������
	ted_End_SmallTime,	// ������� �������
	ted_End_LargeTime,	// ������� �������
};
//----------------------------------------------------------------------------------------------------------------------
struct DetectorResult
{
	int sizePage;
	int sizePreHistory;
	int sizeArrays;
	double timeSRV_lastKadr;
};
//--------------------------------------------------------------------------
class CCustomDetectorPressure
{
private:

protected:
	bool m_bInit = false;		// �������� ������������������
	bool m_bEvent = false;		// ����������� �������
	bool m_bInterval = false;	// �������� � �������� ��������
	//bool m_bExtremum = false;
	int m_counterForInit = 0;	// ������� �������������
	int m_counterInitMax = 0;
	int m_sizePreHistory = 2000;// �����������: 20 ��� ��� 100 ��
	int m_sizeEvent = 500;		// ������ �������: 5 ��� ��� 100 ��
	float m_threshold = 0.03f;	// ����� ��������������
	
	TypeEventYesNo SetTypeEvent(bool bEvent);

public:
	CCustomDetectorPressure();
	virtual ~CCustomDetectorPressure() {}
	virtual bool Create(Lds_cfg_Detector_Pressure_ns::Proxy & proxy);

	TypeEventYesNo m_typeEvent = ted_No;
	DetectorResult m_result;
	Lds_cfg_Detector_Pressure m_cfg;

	// ������� �������
	bool GetFlagStartMode() { return !m_bInit; }
	bool GetFlagEvent() { return m_bInit && m_bEvent; }
	bool GetFlagReadyInterval() { return m_bInit && m_bInterval; }

	// ������� ������
	virtual void ZeroData();
	virtual bool Calculation(const float* pData, const int sizePage);
};
//--------------------------------------------------------------------------