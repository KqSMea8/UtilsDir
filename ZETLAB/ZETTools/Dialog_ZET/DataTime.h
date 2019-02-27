//------------------------------------------------------------------------------
//		����� "����� ������". ���������� ������� ����� ������� ������.
// ������������ ��� ��������, � ������� ������������ ���������� 0,1 ���.
//		����, ������������ ��� ���������� ������:
// 1. �������� ������ ���������� ��� ����������� 0,1 ���.
// 2. ���������� ����������� ������� ������� ����������.
// 3. ���� ������ �������������� ��������� ����� 7000.
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <math.h>
#include <mathZet.h>
#include <ZetTools\include\ZetTools.h>
//------------------------------------------------------------------------------
enum Mode10sec
{// ��� ������ ������ ��� ���������� 10 ���
	m10s_every_1sec = 1,	// ������ �������� ������ �������
	m10s_every_10sec = 10	// ������ �������� ������ 10 ������
};
//------------------------------------------------------------------------------
// ����� "����� ������"
class CDataTime
{
private:
	// ����� ������: true - ������ � ������, false - � ���������
	bool m_bCodeMode;
	// ��� ������ � ������ ���������� ������ ������� ���-�� ����������� ��������:
	// true - �� m_timeAverage, false - �� m_zdt
	bool m_bCalcNumberMode;
	// ����� ������, ����� �������� = ����� ���������� � ������� ��� CZetTimeSpan
	bool m_bZetMode;
	// ����� ������, ����� �������� m_zdt � m_timeAverage �� ������� ���� � ������,
	// ����� ������������ ��� ���������� ���������� ������
	bool m_bInDependentMode;

	long reserved3;

	long m_codeFast;		// ��� ���������� �� �������
	Mode10sec m_mode10sec;	// ��� ������ ������ ��� ���������� 10 ���

	double m_timeAverage;	// ����� ����������
	CZetTimeSpan m_zdt;		// ���������� ������� ��� ����������, ���
	CZetTimeSpan m_zTimeRead;// ����� ������ ������

protected:

public:
	CDataTime();
	~CDataTime() {}
	CDataTime(const CDataTime & val);
	CDataTime & operator = (const CDataTime & val);

	// ������� ������� --------------------------------------------------------
	// ��� ����������
	long GetAverageCode() const { return m_codeFast; }
	void SetAverageCodeMode(const long code, const double freqADC = 0.);

	// C����� ������� ���-�� ����������� ��������:
	// true - �� m_timeAverage, false - �� m_zdt
	bool GetCalcNumberMode();
	void SetCalcNumberMode(bool bCalcByAver);

	// ��� ��������� ��� 10 ��� (���������� ������� - ��/���)
	Mode10sec GetMode10sec() const { return m_mode10sec; }
	void SetMode10sec(const Mode10sec val) { m_mode10sec = val; }

	// �������� ������
	double GetTimeInterval() { return m_zdt; }
	double GetTimeAverage() { return m_timeAverage; }
	
	// ������� ������ ---------------------------------------------------------
	// ������ ���� ������ ������
	bool IsCodeMode() const { return m_bCodeMode; }
	// ������� ���������� � user ����
	bool SetUserParamMode(double timeInterval, long indexMax = 0, double timeAverage = 0.);
	// �������� ���� ����������
	long TestFast(const long val);
	// ��������� �������� ������� ������ ������
	double GetTime() { return m_zTimeRead; }
	CZetTimeSpan & GetzTime() { return m_zTimeRead; }
	CZetTimeSpan & GetzTimeInterval() { return m_zdt; }

	// ��������� ������� ������� ������ ������
	void SetStartTime(const double timeCur);
	// ��������� ������� ������ ������
	void IncrementTime();
	// ������� ��������� � ������ Zet (�������� = ����� ����������)
	void SetZetMode(CZetTimeSpan & zts);
	// ������� ���������� � ������, ����� �������� � ����� ���������� �� �������
	void SetInDependentMode(const CZetTimeSpan & zTimeInterval, const double timeAver);
	// ������� �������� ������ ������ ��������
	void SetTimeRead(const CZetTimeSpan & zTime);
};
//------------------------------------------------------------------------------