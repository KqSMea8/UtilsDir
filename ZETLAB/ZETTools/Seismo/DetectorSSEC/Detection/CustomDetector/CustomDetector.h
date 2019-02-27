//--------------------------------------------------------------------------
// ���� CustomDetector.h
//--------------------------------------------------------------------------
/*		������� ����� ���������� ��� ������������ �����. ������� ��������
3-� ������������ (x, y, z), � ������� - 4 (+ ��������� ��� ����������
���������). ��� ���� ������ 0 - ��� x, 1 - y, 2 - z, 3 - ���������.
		������ ����������� �� ������, � ������ �������� "�����" ������,
������� ������ �� ����������.
		�������� ������������ ��� ������ ���������� ��������� � ���������
�������� ��� ��� ������������� �� ������ ����� ���������.
*/
#define MaxSizeBuffer	100000	// 1 000 ��� ��� Fadc = 100 ��
//--------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <Intel\ipp\ipps.h>	// ������ SNR
#include <Algorithms\SomeFeatures.h>	// CalcAmpl_32f
#include <Buffer\ShiftBufferArbitraryPage.h>
#include <Seismo/DetectorSSEC/Detection/Struct_CFG/Ssm_cfg_Detector_STAProxy.h>
//--------------------------------------------------------------------------
using namespace zet::configuration::schema;
//------------------------------------------------------------------------------
enum TypeEventYesNo
{
	ted_No,			// ������� ���
	ted_Start,		// ������� ��������
	ted_Continue,	// ������� �����������
	ted_End,		// ������� �����������
	ted_End_SmallTime,	// ������� �������
	ted_End_LargeTime,	// ������� �������
};
//--------------------------------------------------------------------------
struct DetectorResult
{
	int sizePage;
	int sizePreHistory;
	int sizeArrays;
//	float SNR;	// SNR - ��������� ������/��� (��� �������/��� ����), ��
	double timeSRV_lastKadr;
};
//--------------------------------------------------------------------------
class CCustomDetector
{
private:

protected:
	bool m_bInit = false;		// �������� ������������������
	bool m_bEvent = false;		// ����������� �������
	bool m_bInterval = false;	// �������� � �������� ��������
	int m_counterForInit = 0;	// ������� �������������
	int m_counterInitMax = 0;
	int m_sizePreHistory = 2000;// �����������: 20 ��� ��� 100 ��
	int m_sizeEvent = 500;		// ������ �������: 5 ��� ��� 100 ��
	float m_threshold = 0.03f;	// ����� ��������������
	SmartArray<float> m_saTmpXYZ[3];	// ��� ������� ��������� � ��.

	TypeEventYesNo SetTypeEvent(bool bEvent);

public:
	CCustomDetector();
	virtual ~CCustomDetector() {}
	virtual bool Create(Ssm_cfg_Detector_STA_ns::Proxy & proxy);

	TypeEventYesNo m_typeEvent = ted_No;
	float m_peakXYZA[4];
	DetectorResult m_result;
	Ssm_cfg_Detector_STA m_cfg;

	// ������� �������
	bool GetFlagStartMode() { return !m_bInit; }
	bool GetFlagEvent() { return m_bInit && m_bEvent; }
	bool GetFlagReadyInterval() { return m_bInit && m_bInterval; }

	// ������� ������
	virtual void ZeroData();
	virtual bool Calculation(
		const float *pDataX, const float *pDataY,
		const float *pDataZ, const int sizePage,
		bool bNeedPeakValue = true);
};
//--------------------------------------------------------------------------