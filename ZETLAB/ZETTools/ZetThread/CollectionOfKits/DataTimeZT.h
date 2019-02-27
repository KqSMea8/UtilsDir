//------------------------------------------------------------------------------
//		����� "����� ������". ���������� ������� ����� ������� ������.
//		����, ������������ ��� ���������� ������:
// 1. �������� ������ ����������.
// 2. ���������� ����������� ������� ������� ����������.
// 3. ���� ������ ��������������.
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <ZetTools\include\ZetTools.h>
//------------------------------------------------------------------------------
// ����� "����� ������"
class CDataTimeZT
{
private:
	CZetTimeSpan m_zts_dtBuf;	// �������� ������� ��� ������� ������� ������ ������

	CZetTimeSpan m_zts_dtRead;	// ������ ������ ������
	CZetTimeSpan m_zts_dt�alc;	// ������ ������� ������������� �����������
	CZetTimeSpan m_zts_dtAver;	// ������ ���������� ������������� �����������

	CZetTimeSpan m_zts_read;	// ����� ������ ������
	CZetTimeSpan m_zts_calc;	// ����� ������� ������������� �����������
	CZetTimeSpan m_zts_aver;	// ����� ���������� ������������� �����������
	
protected:

public:
	CDataTimeZT();
	~CDataTimeZT();
	CDataTimeZT(const CDataTimeZT & val);
	CDataTimeZT & operator = (const CDataTimeZT & val);
	
	// �������� ������� ��� ������� ������� ������ ������ ----------------------------
	CZetTimeSpan GetIntervalBuffer();
	void SetIntervalBuffer(const CZetTimeSpan & ztInterval);

	// ������ ������ ������ --------------------------------------------------------
	CZetTimeSpan & GetIntervalRead();
	void SetIntervalRead(const CZetTimeSpan & ztInterval);
	void SetIntervalRead(const double dInterval);

	// ������ ������� ������������� ����������� ---------------------------------
	CZetTimeSpan & GetIntervalCalc();
	void SetIntervalCalc(const CZetTimeSpan & ztInterval, const bool bRound = true);
	void SetIntervalCalc(const double dInterval, const bool bRound = true);

	// ������ ���������� ������������� ����������� ---------------------------------
	CZetTimeSpan & GetIntervalAver();
	void SetIntervalAver(const CZetTimeSpan & ztInterval);
	void SetIntervalAver(const double dInterval);
	
	// ����� ������ ������ ----------------------------------------------------
	CZetTimeSpan & GetTimeRead();
	void SetTimeRead(const CZetTimeSpan & zTime);

	// ����� ������� ������������� ����������� --------------------------------
	CZetTimeSpan & GetTimeCalc();
	void SetTimeCalc(const CZetTimeSpan & zTime);

	// ����� ���������� ������������� ����������� -----------------------------
	CZetTimeSpan & GetTimeAver();
	void SetTimeAver(const CZetTimeSpan & zTime);

	//--------------------------------------------------------------------------
	// ������ ������� ----------------------------------------------------------
	//--------------------------------------------------------------------------

	// ��������� ������� ������� ������ ������
	void SetTimeReadStart(const double timeCur);

	// ��������� ������� ������ ������
	void IncrementTime();

	// ����� �������������� ������ ������ � ���
	double GetTime();

	// ��������� ������� ������� ������������� �����������
	void IncrementTimeCalc();
	// �������� ����������� ������� ������� ������������� �����������
	bool IsTimeCalc();

	// ��������� ������� ���������� ������������� �����������
	void IncrementTimeAver();
	// �������� ����������� ���������� ������������� �����������
	bool IsTimeAver();

	// ����� ������ ������ ��� ������������� �������
	// zts_dt - ������� ������ � ������� ������ ������ �������
	// (������������� ��������)
	void ReStartSRV(const CZetTimeSpan & zts_dt);

	// ��������� ������ �������� ����� ������, ������� � ���������� �
	// ������������ � ��������� ������������� ������ ���������� ��������.
	// ������� ������, ������� � ���������� ������ ���� ������.
	// timeCur - ������� ����� �������;
	// timeForInit - ����� �������������.
	// ��������������� ��������� ��������:
	// TimeRead = RoundingByRules(timeCur - timeForInit);
	// TimeCalc = TimeRead + timeForInit;
	// TimeAver = RoundingByRules(timeCur + IntervalTimeAver)
	void SetFirstTimesByRules(const double timeCur, const double timeForInit);
};
//------------------------------------------------------------------------------