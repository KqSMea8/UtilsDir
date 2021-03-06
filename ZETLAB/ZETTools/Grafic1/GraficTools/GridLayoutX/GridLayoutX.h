#pragma once
//*************************************************************************************************
#include <ZetTools\include\ZetTools.h>
#include <vector>
//*************************************************************************************************
#define NUMBER_SEC_IN_WEEK		7 * NUMBER_SEC_IN_DAY			// ���������� ������ � ������
//*************************************************************************************************
#define WIDTH_COLUMN_YEAR		(WIDTH_COLUMN_IN_PIX - 70)		// ������ ����� ��������� � ��������
#define WIDTH_COLUMN_IN_PIX		190								// ������ ����� ��������� � �������� 
//*************************************************************************************************
#define KOL_MONTHS_IN_YEAR		12								// ���������� ������� � ����
//*************************************************************************************************
#define PRECISION_NANSEC		1.e5							// ����������� �������� ��������� �������
//*************************************************************************************************
class GridLayoutX
{
public:
	// ��������� ����  /  �������� ����  /  ������ ����� ���� � ��������  /  ������������ ���������� ������ ����� ��������� (�� ��������� ��������� 190/120 ��������)
	GridLayoutX(const CZetTime, const CZetTime, const int, const int max_columns_width = -1);
	GridLayoutX();
	//*********************************************************************************************
	std::vector<CZetTime> VectorLayout;							// ������ ���������� ���� ��� ������� �� �������
	std::vector<CString> DateString;							// ������ ���������� ����������������� ������ � ������ ��� ������ �� �� ��� �
	//*********************************************************************************************
	/* ���������� ���������� ��� ����������
	0 - ����
	1 - ������
	2 - ������
	3 - ���
	4 - ����
	5 - ������
	6 - �������
	7 - ���� ������*/
	int del;
	CString FirstStrFormat;										// ������ ������ ������ ��� ������� ����� ���������� �� �����
	double FirstStrPrecision;									// �������� ������������� ������ ������
	//*********************************************************************************************
	int shift;													// �� ������� �����������
public:
	// ��������� ����  /  �������� ����  /  ������ ����� ���� � ��������  /  ������������ ���������� ������ ����� ��������� (�� ��������� ��������� 190/120 ��������)
	void CalcGrid(const CZetTime, const CZetTime, const int, const int max_columns_width = -1);
private:
	void GridLayout();
	void StepYear(int);											// ��������� �� ����		(1/2/5/10/20/50/100/200/500)
	void StepMonth(int);										// ��������� �� ������		(1/3/6)
	void StepWeek(int);											// ��������� �� ������		(1/2)
	void StepDay(int);											// ��������� �� ���			(1/3)
	void StepHour(int);											// ��������� �� ����		(1/3/6/12)
	void StepMinute(int);										// ��������� �� ������		(1/2/5/10/15/30)
	void StepSecond(int);										// ��������� �� �������		(1/2/5/10/15/30)
	void StepNanoSecond(double);									// ��������� �� �����������	(0.001/0.002/0.005/0.01/0.02/0.05/0.1/0.2/0.5)
	//*********************************************************************************************
	void PushBackData(CString, double precision = 1);			// ������ ��� � ������ CString
	//*********************************************************************************************
	CString Format;												// ������ ��� ������ ����� �� �����
	CZetTime ZetDateTemp;										// ��������� ���������� ��� ������ � ������			
	CZetTime first;												// ���� ����� �������
	CZetTime last;												// ���� ������ �������
	int width_pix;												// ������ ���� � �������� 
	int MaxColumnsWidth;										// ������������ ������ �������, �� ��������� 190/120 ��������
	//*********************************************************************************************
	uint16_t YearFirst;											// ��� ����� �������
	uint16_t MonthFirst;										// ����� ����� �������
	uint16_t DayFirst;											// ���� ����� �������
	uint16_t DayWeekFirst;										// ���� ������ ����� �������
	uint16_t HourFirst;											// ��� ����� �������
	uint16_t MinuteFirst;										// ������ ����� �������
	double SecondsFirst;										// ������� ����� �������
};	//*********************************************************************************************