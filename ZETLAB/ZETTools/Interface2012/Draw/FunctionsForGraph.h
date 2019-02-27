//------------------------------------------------------------------------------
//	FunctionForGraph.h - ���� ����������
//	������� ��� ������������� � ������ �������
//------------------------------------------------------------------------------
#pragma once

#include <limits>
#include <ipps.h>

//static double NAN = std::numeric_limits<double>::quiet_NaN();

#define MAX_QNT_MARKS 20 //������������ ���������� �����
#define MIN_QNT_MARKS 5 //����������� ���������� �����
#define MAX_PX_STEP 90 //������������ ��� � ��������
#define CONST1 0.3 //������� ����� ����������� ��������� 2*10^x
#define CONST2 0.7 //������� ����� ����������� ��������� 5*10^x
#define MAX_QNT_OF_POINTS 250000

enum timeID
{
    Year,
    Month,
    Day,
    Hour,
    Minute,
    Second,
    Millisecond
};

struct TimeInterval
{
    int tiData;
    timeID tiID;
};

//��� ���: ������������ ��� ��������������
enum GridType{
    gtHorizontal,
    gtVertical,
};

//��� �����: �������������� ��� ��������
enum ScaleType{
    stLog,
    stLinear,
    stNone
};

//��� �����: �������� �����������
enum LineType{
    ltFirm,
    ltDot
};

//�������, ��� ������� ���� ��� ��������������� ������� � ����������� �����������
enum zRect{
    gLeftShift,
    gRightShift,
    gTopShift,
    gBottomShift,
    gGridArea,
    gLeftScale,
    gRightScale,
    gTopScale,
    gBottomScale,
    gCenterAutoScale,
    gNone
};

enum FitType{
    ftLeft,
    ftRight,
    ftCenter
};

enum Settings{
    sAuto,
    sManual
};

enum PlotType{
    ptSpectr, // ������ (�������� �����, ������ ����������)
	ptSpectrLog, //������ (��������������� �����, ������ ����������)
    ptOscill, // ������������� (�������� �����, ������ - �����)
	ptOscillLog, // ������������� (��������������� �����, ������ - �����)
    ptTrend, //������� (�� ��� � ����� � ������� ���, ������ - �����)
	ptParam, //�������� ����������� (� �� Y, ����� ��������, ������ - �����)
    ptOld
};

struct TwoDouble{
    double Min;
    double Max;
	TwoDouble() 
		: Min(0)
		, Max(0)
	{
	}
};

struct TwoDate
{
    SYSTEMTIME Min;
    SYSTEMTIME Max;
	TwoDate() 
	{
		Min = {0};
		Max = {0};
	}
};

struct CursorValues
{
    CString X;
    CString Y;
    double pxX;
    double pxY;
	CursorValues()
		: X(L"")
		, Y(L"")
		, pxX(0.0)
		, pxY(0.0)
	{
	}
};