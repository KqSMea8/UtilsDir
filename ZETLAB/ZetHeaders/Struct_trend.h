#pragma once

#include <windef.h>
#include <minwinbase.h>
#include <vector>
#include <ZetColors.h>
#include <ZetTools/include/ZetTools.h>

static const double DEFAULT_UPOR_DELTA = 1.e-9/*0.0001*/;

enum DisplayMode
{
	Events = 0,			// ����������� ����� ��� ����� �������� �� Y (���������� �����)
	Default,			// ����������� ���� ����� � ����� ����
	SplittingFields,	// ����������� ������ ����� � ��������� �������
	SeismicGrath,		// ����������� ������� ������� � ��������� �������
	ImpositionGraths	// ����������� ���� �������� � ���������� ���� �� �����
};
//*************************************************************************************************
// ������� ��������� ��� ��������� ��������
struct TrendGrid
{
	long sizeofgrid;						// ������  grid-� � ������
	int left_x,								// ������� �����
		right_x,							// �������������� ���������� ������
		top_y,								// ���� �������
		bottom_y,							// ���������� �� ������� ���� ���� �� ������� ���� �������
		width_x,							// ������  ������� �� ��� X
		height_y,							// ������  ������� �� ��� Y
		ColorLine_Height,					// ������ ��� CLocolLine
		mouse_x,							// ������� ���� � �������� ���� - ���
		mouse_y,							// ������� ���� � �������� ���� - ���
		timer,								// ������� ��������� �������
		update;    							// ������� ���������� �������
	long font_size,							// ������  ������
		make_upor,							// ������� ������ �� ��� Y, 0- ��� ������, 1- ����		
		type_abs,							/*	0 -  ���������� � ����� �� � � y
												1 -  ��� ����� �������� �����
												4 -  ��� ����� ���� �����
												5 -  ������ �������� �� � � �
												12 - ������������ �������� � �����
												13 - ������ ��� ��������
												17 - ���� ����� � ��������
												21 - ������ ���� ��������
												29 - ��� ����*/
		Element_width;						// ������ ���������
	double math_width_x,					// �������. ������ ����� �� �
		math_width_y,						// �������. ������ ����� �� Y
		math_min_x,							// ������� ����������� �������� �� X
		math_min_y,							// ������� ����������� �������� �� Y
		upor_delta,							// ����������� �������� ������ ����� �� Y
		upor_down,							// ������  ������ �� Y
		upor_up,							// ������� ������ �� Y
		x_first,							// ������  ������ �� �
		x_end;								// ������� ������ �� �
	wchar_t font_name[MAX_PATH];			// �������� ������
	COLORREF cursor_color;					// ���� �������
	COLORREF digits_color;					// ���� ����
	COLORREF fon_color;						// ���� ����
	COLORREF grid_color;		 			// ���� �����
	bool vert_CursValid;					// ����������� ������������	  �������� 
											// false - �� ���������� // true - ����������
	bool gor_CursValid;						// ����������� �������������� �������� 
											// false - �� ���������� // true - ����������
	DisplayMode Display_Mode;				// ����� ������ ���������
	bool bVerticalGraphs;					// ���� ��� ������� ����� ������������ �����������
	TrendGrid()
		: sizeofgrid		(sizeof(TrendGrid))
		, left_x			(16)
		, right_x			(15)
		, top_y				(16)
		, bottom_y			(12 + 4)/*font_size + 4*/
		, width_x			(100)
		, height_y			(100)
		, ColorLine_Height	(0)
		, mouse_x			(0)
		, mouse_y			(0)
		, timer				(0)
		, update			(0x4)/*UPDATE_DRAW_ALL*/
		, font_size			(12)
		, make_upor			(0)
		, type_abs			(0)
		, Element_width		(7)
		, math_width_x		(0)
		, math_width_y		(101)
		, math_min_x		(0)
		, math_min_y		(-1)
		, upor_delta		(DEFAULT_UPOR_DELTA)
		, upor_down			(-1.e10)
		, upor_up			(1.e10)
		, x_first			(ZetTimeMin)
		, x_end				(ZetTimeMax)
		, vert_CursValid	(true)
		, gor_CursValid		(true)
		, Display_Mode		(Events)
		, bVerticalGraphs	(false)
	{
		CZetTime zt1;
		CZetTime zt2;
		zt1.ConvertFromString(L"01.01.2015 00:00:00");
		zt2.SetCurrentTime();
		math_min_x = zt1;
		math_width_x = zt2 - zt1;
		
		ZeroMemory(&font_name, sizeof(font_name));
		fon_color	 = ZETColors::BackgroundColor;
		grid_color	 = ZETColors::GridColor;
		cursor_color = ZETColors::CursorColor;
		digits_color = ZETColors::DigitsColor;
	}
	TrendGrid & operator = (const TrendGrid & val)
	{
		auto size = sizeof(TrendGrid);
		memcpy_s(this, size, &val, size);
		return *this;
	}
	bool operator == (const TrendGrid & val)
	{
		return memcmp(this, &val, sizeof(TrendGrid)) == 0;
	}
	bool operator != (const TrendGrid & val)
	{
		return !(*this == val);
	}
};
//*************************************************************************************************
// ��������� � ����������� ������� ��� ZETTrendValueGrid
struct ZetTrendValueGridStuct : public TrendGrid
{
	size_t SizeOfDataGridStruct;	// ������ ��������� � �����������
	bool bFillGraph;				// ���� ���� ��� ���������� ��������� ������� ����� ������� ������
	COLORREF FillColor;				// ���� ���������� ������� ����� �������
	int ActivePropSheetIndex;		// ����� �������� �������� � �����������
	long CountMaxVisiblePoints;		// ����������� ������������� ������������� ���������� �����
	bool bLimitMaxVisiblePoints;	// ���� ���� ��� ���������� ������������ ������������ ��������� ������� �����
	//*********************************************************************************************
	ZetTrendValueGridStuct()
		: TrendGrid()
		, SizeOfDataGridStruct(sizeof(ZetTrendValueGridStuct))
		, bFillGraph			(false)
		, FillColor				(RGB(0, 0, 0))
		, ActivePropSheetIndex	(0)
		, CountMaxVisiblePoints	(400000)
		, bLimitMaxVisiblePoints(false)
	{
		Display_Mode = DisplayMode::Default;
		Element_width = 2;
	}

	ZetTrendValueGridStuct(const TrendGrid Parrent)
		: TrendGrid(Parrent)
		, SizeOfDataGridStruct	(sizeof(ZetTrendValueGridStuct))
		, bFillGraph			(false)
		, FillColor				(RGB(0, 0, 0))
		, ActivePropSheetIndex	(0)
		, CountMaxVisiblePoints	(400000)
		, bLimitMaxVisiblePoints(false)
	{}
	//*********************************************************************************************
	ZetTrendValueGridStuct operator = (const ZetTrendValueGridStuct &val)
	{
		auto size = sizeof(ZetTrendValueGridStuct);
		memcpy_s(this, size, &val, size);
		return *this;
	}

	ZetTrendValueGridStuct operator = (const TrendGrid &val)
	{
		*(TrendGrid*)this = *(TrendGrid*)&val;
		return *this;
	}

	bool operator == (const ZetTrendValueGridStuct & val)
	{
		return memcmp(this, &val, sizeof(ZetTrendValueGridStuct)) == 0;
	}

	bool operator != (const ZetTrendValueGridStuct & val)
	{
		return !(*this == val);
	}
};	//*********************************************************************************************