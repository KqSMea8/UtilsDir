#include "stdafx.h"
#include "GridLayoutX.h"

GridLayoutX::GridLayoutX()
	: Format(_T(""))				// ������ ��� ������ ����� �� �����
	, FirstStrFormat(_T(""))		// ������ ������ ������ ��� ������� ����� ���������� �� �����
	, FirstStrPrecision(0)			// �������� ������������� ������ ������
	, ZetDateTemp(ZetTimeMin)		// ��������� ���������� ��� ������ � ������			
	, first(ZetTimeMin)				// ���� ����� �������
	, last(ZetTimeMin)				// ���� ������ �������
	, width_pix(0)					// ������ ���� � �������� 
	, MaxColumnsWidth(0)			// ������������ ������ �������, �� ��������� 190/120 ��������
	, del(0)						// ���������� ���������� ��� ����������
	, shift(0)						// �� ������� �����������
	, YearFirst(0)					// ��� ����� �������
	, MonthFirst(0)					// ����� ����� �������
	, DayFirst(0)					// ���� ����� �������
	, DayWeekFirst(0)				// ���� ������ ����� �������
	, HourFirst(0)					// ��� ����� �������
	, MinuteFirst(0)				// ������ ����� �������
	, SecondsFirst(0)				// ������� ����� �������
{}

GridLayoutX::GridLayoutX(const CZetTime firstx, const CZetTime lastx, const int width_pix_x, const int max_columns_width)
{
	CalcGrid(firstx, lastx, width_pix_x, max_columns_width);
}

void GridLayoutX::CalcGrid(const CZetTime firstx, const CZetTime lastx, const int width_pix_x, const int max_columns_width)
{
	VectorLayout.clear();
	DateString.clear();
	del = 0;
	YearFirst = MonthFirst = DayFirst = DayWeekFirst = 0;

	first = firstx;
	last = lastx;
	width_pix = width_pix_x;
	MaxColumnsWidth = max_columns_width;

	Format.Empty();
	FirstStrFormat = L"dd.MM.yyyy";
	FirstStrPrecision = 1.0;

	if (first > ZetTimeMax)
		first = ZetTimeMin;
	if (last > ZetTimeMax)
		last = ZetTimeMax;

	first.GetYear(YearFirst, false);
	first.GetMonth(MonthFirst, false);
	first.GetDayOfWeek(DayWeekFirst, false);
	first.GetDay(DayFirst, false);
	first.GetHour(HourFirst, false);
	first.GetMinute(MinuteFirst, false);
	first.GetSecond(SecondsFirst);

	GridLayout();
}

// ������ ������ � UTC
void GridLayoutX::GridLayout()
{
	CZetTimeSpan dt = last - first;		// ������� � �������� ����� ��������� � �������� ������

	double kolyears = dt.GetYears();
	int kolmonth = (int)dt.GetMonths();
	int koldays = (int)dt.GetDays();
	int kolweeks = (int)koldays / 7;
	int kolhours = (int)dt.GetHours();
	int kolminutes = (int)dt.GetMinutes();
	double kolseconds = dt;
	double kolnanoseconds = kolseconds * PRECISION_NANSEC;

	int numvert, shvert;
	
	if (MaxColumnsWidth > 0)
		numvert = width_pix / MaxColumnsWidth;					//	���������� ������������ �����
	else
		numvert = width_pix / WIDTH_COLUMN_YEAR;				//	���������� ������������ �����

	if (numvert < 2) numvert = 2;								//	�� ����� ���� ����� 2	
	shvert = (int)round(kolyears / numvert);	//	���������� ��� ����� ���������
	FirstStrPrecision = 1.0;
	if (shvert > 0)
	{
		FirstStrFormat = L"yyyy";
		StepYear(shvert);										// ���� ��� ������ ���� 
	}
	else
	{
		if (MaxColumnsWidth > 0)
			numvert = width_pix / MaxColumnsWidth;					//	���������� ������������ �����
		else
			numvert = width_pix / WIDTH_COLUMN_IN_PIX;				//	���������� ������������ �����
		
		if (numvert < 2) numvert = 2;								//	�� ����� ���� ����� 2
		shvert = kolmonth / numvert;
		if (shvert > 0)
		{
			FirstStrFormat = L"MMMM yyyy";
			StepMonth(shvert);									//  ���� ��� ������ ������
		}
		else
		{			
			shvert = (int)round((double)kolweeks / numvert);
			if (shvert > 0)
			{
				FirstStrFormat = L"MMMM yyyy";
				StepWeek(shvert);								//  ���� ��� ������ ������
			}
			else
			{
				shvert = koldays / numvert;
				if (shvert > 0)
				{
					FirstStrFormat = L"dd MMMM yyyy";
					StepDay(shvert);							//  ���� ��� ������ ��� 
				}
				else
				{
					shvert = kolhours / numvert;
					if (shvert > 0)
					{
						FirstStrFormat = L"HH:mm dd.MM.yyyy";
						StepHour(shvert);							//  ���� ��� ������ ���� 
					}
					else
					{
						shvert = kolminutes / numvert;
						if (shvert > 0)
						{
							FirstStrFormat = L"HH:mm dd.MM.yyyy";
							StepMinute(shvert);							//  ���� ��� ������ ������ 
						}
						else
						{
							shvert = (int)(kolseconds / numvert);
							if (shvert > 0)
							{
								FirstStrFormat = L"HH:mm:ss dd.MM.yyyy";
								StepSecond(shvert);							//  ���� ��� ������ ������� 
							}
							else
							{
								FirstStrFormat = L"HH:mm:ss dd.MM.yyyy";
								//shvert = (int)round(kolnanoseconds / numvert);	// ���������� ��� ���� ����� ��������� ������� ����������� �����������
								double ShvertForNanoSec(kolnanoseconds / numvert);
								if (ShvertForNanoSec > 0)
									StepNanoSecond(ShvertForNanoSec);								// ��������� �� ����� �������		
							}
						}
					}
				}
			}
		}		
	}	
}

// ������� ����������� �������� ���� � SYSTEMTIME �������� ��� � ztTime 
// � ��������� �� ����� �� �� �� ���������� �������
bool IncrementYear(_Inout_ SYSTEMTIME &sTime, _Out_ CZetTime &ztTime, 
				   _In_ const double &Year, _In_ const CZetTime &LastDate)
{
	sTime.wYear = (WORD)Year; 
	ztTime = sTime;
	if (ztTime <= LastDate)
		return true;
	else
		return false;
}

void GridLayoutX::StepYear(int shvert)
{
	del = 0;
	double y, y2, y3, prom;
	int pokvert, i;
	prom = log10(shvert);									//	���������� ������� ����
	pokvert = (int)prom;									//	� ��������� ��� �� �������� ������ 1-���� shvert>10, 2 - ���� svert>100, 3- ���� shvert>1000 (�������, � ������� ���� �������� 10, ����� �������� ���)
	if (prom >= 0) y = pow(10., prom - pokvert);
	else { y = pow(10., prom - pokvert + 1); pokvert--; }	//�.�. ����� svert<1(� 50 pix < 1 ���. ��-��)
	y2 = 2;
	if (y <= 1.3) y2 = 1;
	else
		if (y <= 2.7) y2 = 2;
		else
			if (y <= 5.8) y2 = 5;
			else
				if (y <= 10) y2 = 10;

	y3 = y2 * pow(10., pokvert);
	i = (int)(((YearFirst) / y3) + 1);

	// ���������� �������� ��� �� ������� ���� ���������
	SYSTEMTIME SystemTemp = { 0 };
	SystemTemp.wDay = 1;
	SystemTemp.wMonth = 1;
	Format = L"yyyy";
	while (IncrementYear(SystemTemp, ZetDateTemp, i * y3, last))
	{		
		VectorLayout.push_back(ZetDateTemp);
		PushBackData(Format);
		i++;
	}

	shift = (int)y3;
}

void GridLayoutX::StepMonth(int shvert)
{
	del = 1;
	if (shvert >= 4)
	{
		// ���������� �� ��� ����
		SYSTEMTIME SystemTemp = { 0 }; // ������ �����
		SystemTemp.wDay = 1;
		SystemTemp.wMonth = MonthFirst <= 7 ? 7 : 1;
		SystemTemp.wYear = MonthFirst <= 7 ? YearFirst : YearFirst + 1;
		ZetDateTemp = SystemTemp;
		if (ZetDateTemp < first)
		{
			if (SystemTemp.wMonth == 7)
			{
				SystemTemp.wMonth = 1;
				SystemTemp.wYear += 1;
			}
			else
				SystemTemp.wMonth = 7;

			ZetDateTemp = SystemTemp;
		}		
		VectorLayout.push_back(ZetDateTemp);
		Format = L"MMMM_yyyy";
		PushBackData(Format);

		while (ZetDateTemp <= last)
		{
			if (SystemTemp.wMonth == 7)
			{
				SystemTemp.wMonth = 1;
				SystemTemp.wYear += 1;
			}
			else
				SystemTemp.wMonth = 7;

			ZetDateTemp = SystemTemp;
			VectorLayout.push_back(ZetDateTemp);
			Format = L"MMMM_yyyy";
			PushBackData(Format);
		}
		shift = 6;
	}
	else if (shvert == 1)
	{
		// ���������� �� ������ ������
		SYSTEMTIME SystemTemp = { 0 }; // ������ ����
		SystemTemp.wDay = 1;
		if (DayFirst == 1)
		{
			SystemTemp.wMonth = MonthFirst;
			SystemTemp.wYear = YearFirst;
		}
		else
		{
			if (MonthFirst == 12)
			{
				SystemTemp.wMonth = 1;
				SystemTemp.wYear = YearFirst + 1;
			}
			else
			{
				SystemTemp.wMonth = MonthFirst + 1;
				SystemTemp.wYear = YearFirst;
			}
		}
		ZetDateTemp = SystemTemp;
		if (ZetDateTemp < first)
		{
			if (MonthFirst == 12)
			{
				SystemTemp.wMonth = 1;
				SystemTemp.wYear = YearFirst + 1;
			}
			else
			{
				SystemTemp.wMonth = MonthFirst + 1;
				SystemTemp.wYear = YearFirst;
			}
			ZetDateTemp = SystemTemp;
		}
		VectorLayout.push_back(ZetDateTemp);
		Format = L"MMMM_yyyy";
		PushBackData(Format);

		while (ZetDateTemp <= last)
		{
			if (SystemTemp.wMonth == 12)
			{
				SystemTemp.wMonth = 1;
				SystemTemp.wYear += 1;
			}
			else
				SystemTemp.wMonth += 1;

			ZetDateTemp = SystemTemp;
			VectorLayout.push_back(ZetDateTemp);
			Format = L"MMMM_yyyy";
			PushBackData(Format);
		}
		shift = 1;
	}
	else
	{
		// ���������� �� ��������
		SYSTEMTIME SystemTemp = { 0 }; // ������ ����
		// ���������� � ����� ���� �������� ������
		if (DayFirst != 1)						// ��������� ���� �� ���������� �� ���� ���
			if (MonthFirst == 12)
			{
				MonthFirst = 1;
				YearFirst += 1;
			}
			else
				MonthFirst += 1;

		if (MonthFirst > 10)
		{
			MonthFirst = 1;
			YearFirst += 1;
		}
		else if ((MonthFirst > 7) && (MonthFirst < 10))
			MonthFirst = 10;
		else if ((MonthFirst > 4) && (MonthFirst < 7))
			MonthFirst = 7;
		else if ((MonthFirst > 1) && (MonthFirst < 4))
			MonthFirst = 4;

		SystemTemp.wDay = 1;
		SystemTemp.wMonth = MonthFirst;
		SystemTemp.wYear = YearFirst;
		ZetDateTemp = SystemTemp;

		if (ZetDateTemp < first)
		{
			if (MonthFirst > 10)
			{
				MonthFirst = 1;
				YearFirst += 1;
			}
			else if ((MonthFirst > 7) && (MonthFirst < 10))
				MonthFirst = 10;
			else if ((MonthFirst > 4) && (MonthFirst < 7))
				MonthFirst = 7;
			else if ((MonthFirst > 1) && (MonthFirst < 4))
				MonthFirst = 4;

			ZetDateTemp = SystemTemp;
		}
		
		VectorLayout.push_back(ZetDateTemp);
		Format = L"MMMM_yyyy";
		PushBackData(Format);

		while (ZetDateTemp <= last)
		{
			if (SystemTemp.wMonth == 10)
			{
				SystemTemp.wMonth = 1;
				SystemTemp.wYear += 1;
			}
			else
				SystemTemp.wMonth += 3;

			ZetDateTemp = SystemTemp;
			VectorLayout.push_back(ZetDateTemp);
			Format = L"MMMM_yyyy";
			PushBackData(Format);
		}
		shift =  3;
	}
}

void GridLayoutX::StepWeek(int shvert)
{
	del = 2;
	SYSTEMTIME SystemTemp = { 0 };					// ������ �����

	if (shvert >= 2)
		shvert = 2;									// �� ��� ������
	else
		shvert = 1;									// �� 1 ������

	uint16_t YearPrevious(0), YearCurrent(0);
	CZetTimeSpan dtweek(NUMBER_SEC_IN_WEEK);

	int   ret;
	DWORD value;
	ret = GetLocaleInfo(LOCALE_USER_DEFAULT,
		LOCALE_IFIRSTDAYOFWEEK,
		(LPTSTR)&value,
		sizeof(value) / sizeof(TCHAR));
	int systemfirstdayofweek = (int)value - 47;										// c 48 ���������� ������ ���� ������ 

	if (DayWeekFirst != systemfirstdayofweek)
	{
		DayFirst = DayFirst + 7 - (DayWeekFirst == 0 ? 7 : DayWeekFirst/*������� �����������*/) + systemfirstdayofweek; // �������� ��������� ������ ���� ������
		switch (MonthFirst)
		{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
		{
			// ������ � 31 ����
			if (DayFirst > 31)
			{
				DayFirst = DayFirst - 31;
				if (MonthFirst == KOL_MONTHS_IN_YEAR)
				{
					MonthFirst = 1;
					YearFirst += 1;
				}
				else
					MonthFirst += 1;
			}
			break;
		}
		case 2:
		{
			// �������
			if ((YearFirst % 400) == 0 || (YearFirst % 4 == 0 && YearFirst % 100 != 0))
			//if (YearFirst % 4 == 0)												// ���������� ���
			{
				if (DayFirst > 29)
				{
					DayFirst = DayFirst - 29;
					MonthFirst += 1;
				}
			}
			else
			{
				if (DayFirst > 28)
				{
					DayFirst = DayFirst - 28;
					MonthFirst += 1;
				}

			}
			break;
		}
		default:
		{
			// ������ � 30� �����
			if (DayFirst > 30)
			{
				DayFirst = DayFirst - 30;
				MonthFirst += 1;
			}
			break;
		}
		}// switch(month)
	}
	Format = L"dddd_dd.MM.yyyy";

	SystemTemp.wDay = DayFirst;
	SystemTemp.wMonth = MonthFirst;
	SystemTemp.wYear = YearFirst;
	ZetDateTemp = SystemTemp;
	while (ZetDateTemp < first)
		ZetDateTemp += dtweek;
	VectorLayout.push_back(ZetDateTemp);
	PushBackData(Format);

	while (ZetDateTemp <= last)
	{
		Format = L"dddd_dd MMMM";

		ZetDateTemp.GetYear(YearPrevious, false);
		ZetDateTemp += shvert * dtweek;
		ZetDateTemp.GetYear(YearCurrent, false);

		if (YearPrevious != YearCurrent)
			Format = L"dd.MM_yyyy";												// ��������� ������� �� ���� ���


		VectorLayout.push_back(ZetDateTemp);
		PushBackData(Format);				
	}
	shift = shvert;
}

void GridLayoutX::StepDay(int shvert)
{
	del = 3;
	SYSTEMTIME SystemTemp = { 0 }; // ������ �����
	uint16_t YearPrevious(0), YearCurrent(0);
	CZetTimeSpan dtday(NUMBER_SEC_IN_DAY);

	if (shvert >= 2)
		shvert = 3;																// �� 3 ���
	else
		shvert = 1;																// �� 1 ���

	Format = L"dd.MM_yyyy";
	SystemTemp.wDay = DayFirst;
	SystemTemp.wMonth = MonthFirst;
	SystemTemp.wYear = YearFirst;
	ZetDateTemp = SystemTemp;
	while (ZetDateTemp < first)
		ZetDateTemp += dtday;
	VectorLayout.push_back(ZetDateTemp);
	PushBackData(Format);
	
	while (ZetDateTemp <= last)
	{
		Format = L"dd.MM";
		ZetDateTemp.GetYear(YearPrevious, false);
		ZetDateTemp += shvert * dtday;
		ZetDateTemp.GetYear(YearCurrent, false);

		if (YearPrevious != YearCurrent)			
			Format = L"dd.MM_yyyy";												// ��������� ������� �� ���� ���


		VectorLayout.push_back(ZetDateTemp);
		PushBackData(Format);
	}
	shift = shvert;
}

void GridLayoutX::StepHour(int shvert)
{
	del = 4;
	SYSTEMTIME SystemTemp = { 0 }; // ������ �����
	Format = L"HH:mm _dd.MM.yyyy";

	CZetTimeSpan dthour(NUMBER_SEC_IN_HOUR);
	uint16_t TempHour(0), MonthPrevious(0), MonthCurrent(0);

	if (shvert == 1)
		shvert = 1;																// �� 1 ����
	else if (shvert >= 12)
		shvert = 12;															// �� 12 �����
	else if ((shvert < 12) && (shvert >= 6))
		shvert = 6;																// �� 6 �����
	else
		shvert = 3;																// �� 3 ����

	SystemTemp.wDay = DayFirst;
	SystemTemp.wMonth = MonthFirst;
	SystemTemp.wYear = YearFirst;
	ZetDateTemp = SystemTemp;
	// ��������� ����� � �������� ��������
	while (ZetDateTemp < first)
		ZetDateTemp += shvert * dthour;

	VectorLayout.push_back(ZetDateTemp);
	PushBackData(Format);

	while (ZetDateTemp <= last)
	{
		Format = L"HH:mm_";

		
		ZetDateTemp.GetMonth(MonthPrevious, false);
		ZetDateTemp += shvert * dthour;
		ZetDateTemp.GetHour(TempHour, false);

		if (TempHour == 0)														// ���� ��������� ����� ���
		{
			Format = L"HH:mm _dd.MM.yyyy";
			ZetDateTemp.GetMonth(MonthCurrent, false);
			if (MonthPrevious != MonthCurrent)									// ���� ��������� ����� �������
				Format = L"HH:mm _dd.MM.yyyy";	///////////////// ��������� ������� ����� ������
		}
		VectorLayout.push_back(ZetDateTemp);
		PushBackData(Format);
	}
	shift = shvert;
}

void GridLayoutX::StepMinute(int shvert)
{
	del = 5;
	SYSTEMTIME SystemTemp = { 0 }; // ������ �����
	Format = L"HH:mm _dd.MM.yyyy";

	CZetTimeSpan dtminute(NUMBER_SEC_IN_MIN);
	uint16_t DayPrevious(0), DayCurrent(0);

	if (shvert == 1)
		shvert = 1;																// �� 1 ������
	else if (shvert >= 30)
		shvert = 30;															// �� 30 �����
	else if ((shvert >= 15) && (shvert < 30))
		shvert = 15;															// �� 15 �����
	else if ((shvert >= 10) && (shvert < 15))
		shvert = 10;															// �� 10 �����
	else if ((shvert >= 4) && (shvert < 10))
		shvert = 5;																// �� 5 �����
	else
		shvert = 2;																// �� 2 ������

	SystemTemp.wHour = HourFirst;
	SystemTemp.wDay = DayFirst;
	SystemTemp.wMonth = MonthFirst;
	SystemTemp.wYear = YearFirst;
	ZetDateTemp = SystemTemp;
	// ��������� ����� � �������� ��������
	while (ZetDateTemp < first)
		ZetDateTemp += shvert * dtminute;

	VectorLayout.push_back(ZetDateTemp);
	PushBackData(Format);

	while (ZetDateTemp <= last)
	{
		Format = L"HH:mm_";

		ZetDateTemp.GetDay(DayPrevious, false);
		ZetDateTemp += shvert * dtminute;
		ZetDateTemp.GetDay(DayCurrent, false);

		if (DayPrevious != DayCurrent)											// ���� ��������� ����� ���
			Format = L"HH:mm _dd.MM.yyyy";

		VectorLayout.push_back(ZetDateTemp);
		PushBackData(Format);
	}
	shift = shvert;
}

void GridLayoutX::StepSecond(int shvert)
{
	del = 6;
	SYSTEMTIME SystemTemp = { 0 };												// ������ �����
	Format = L"HH:mm:ss _dd.MM.yyyy";

	CZetTimeSpan dtseconds(1);													// 1 �������
	uint16_t DayPrevious(0), DayCurrent(0);

	if (shvert == 1)
		shvert = 1;																// �� 1 �������
	else if (shvert >= 30)
		shvert = 30;															// �� 30 ������
	else if ((shvert >= 15) && (shvert < 30))
		shvert = 15;															// �� 15 ������
	else if ((shvert >= 10) && (shvert < 15))	
		shvert = 10;															// �� 10 ������
	else if ((shvert >= 4) && (shvert < 10))
		shvert = 5;																// �� 5 ������
	else
		shvert = 2;																// �� 2 �������
		
	SystemTemp.wMinute = MinuteFirst;
	SystemTemp.wHour = HourFirst;
	SystemTemp.wDay = DayFirst;
	SystemTemp.wMonth = MonthFirst;
	SystemTemp.wYear = YearFirst;
	ZetDateTemp = SystemTemp;
	// ��������� ����� � �������� ��������
	while (ZetDateTemp < first)
		ZetDateTemp += shvert * dtseconds;

	VectorLayout.push_back(ZetDateTemp);
	PushBackData(Format);

	// �������� �� ����� ������� �������
	while (ZetDateTemp <= last)
	{
		Format = L"HH:mm:ss_";

		ZetDateTemp.GetDay(DayPrevious, false);
		ZetDateTemp += shvert * dtseconds;
		ZetDateTemp.GetDay(DayCurrent, false);

		if (DayPrevious != DayCurrent)	
			Format = L"HH:mm _dd.MM.yyyy";

		VectorLayout.push_back(ZetDateTemp);
		PushBackData(Format);
	}
	shift = shvert;
}

void GridLayoutX::StepNanoSecond(double temp)
{
	double shvert = (double)temp;
	del = 7;
	SYSTEMTIME SystemTemp = { 0 };												// ������ �����
	Format = L"HH:mm:ss _dd.MM.yyyy";

	uint16_t DayPrevious(0), DayCurrent(0);
	uint16_t HourPrevious(0), HourCurrent(0);
	double grade = 0.1;															// �������� ���������												

	float Shvert4Val = 0.4 * PRECISION_NANSEC;
	float Shvert2Val = 0.2 * PRECISION_NANSEC;
	float Shvert1Val = 0.1 * PRECISION_NANSEC;

	for (;;)
	{
		if (shvert >= Shvert4Val)
		{
			shvert = 5;			
			break;
		}
		else if ((shvert >= Shvert2Val) && (shvert < Shvert4Val))
		{
			shvert = 2;
			break;
		}
		else if ((shvert >= Shvert1Val) && (shvert < Shvert2Val))
		{
			shvert = 1;
			break;
		}
		else
		{
			grade /= 10;
			Shvert4Val /= 10;
			Shvert2Val /= 10;
			Shvert1Val /= 10;
		}

		if (grade < 1 / PRECISION_NANSEC)
		{
			grade = 1 / PRECISION_NANSEC;
			shvert = 1;
			break;
		}
	}

	// ����� 0,001 ���������� ����� � 1.e-6
	if (grade < 0.001)
	{
		shvert *= grade / 1.e-6;
		grade = 1.e-6;
	}

	FirstStrPrecision = grade;

	SystemTemp.wSecond = (WORD)SecondsFirst;
	SystemTemp.wMinute = MinuteFirst;
	SystemTemp.wHour = HourFirst;
	SystemTemp.wDay = DayFirst;
	SystemTemp.wMonth = MonthFirst;
	SystemTemp.wYear = YearFirst;
	ZetDateTemp = SystemTemp;
	// ��������� ����� � �������� ��������
	while (ZetDateTemp < first)
		ZetDateTemp += shvert * grade;

	VectorLayout.push_back(ZetDateTemp);
	PushBackData(Format, grade);

	double tempgrade = grade;
	// �������� �� ����� ������� �������
	while (ZetDateTemp <= last)
	{
		grade = tempgrade;
		Format = L"HH:mm:ss";

		ZetDateTemp.GetHour(HourPrevious, false);
		ZetDateTemp.GetDay(DayPrevious, false);
		ZetDateTemp += shvert * grade;
		ZetDateTemp.GetDay(DayCurrent, false);
		ZetDateTemp.GetHour(HourCurrent, false);
		
		if (HourPrevious != HourCurrent)
		{			
			grade = 1;
			if (DayPrevious != DayCurrent)									// ���� ��������� ����� ���
				Format = L"HH:mm:ss _dd.MM.yyyy";
			else
				Format = L"HH:mm:ss";										// ���� ��������� ����� ����
		}
		VectorLayout.push_back(ZetDateTemp);
		PushBackData(Format, grade);
	}	
	shift = (int)shvert;
}

void GridLayoutX::PushBackData(CString DateFormat, double precision)
{
	CString StringDateTemp; 
	long err = ZetDateTemp.ConvertToString(StringDateTemp.GetBuffer(32), 32, precision, DateFormat/*, false*/);
	StringDateTemp.ReleaseBuffer();
	DateString.push_back(StringDateTemp);
}