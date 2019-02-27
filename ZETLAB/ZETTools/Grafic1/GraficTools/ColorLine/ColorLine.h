#pragma once
#include <ZetTools\include\ZetTools.h>
#include <vector>

#define COLORLINE_HEIGHT		30		// ������ ������� � COLORLINE
//-------------------------------------------------------------------------------------------------
// ��������� � ������������� �������
struct SpanColorLinePixStruct
{
	float PixStart;			// ��������� �������
	float PixEnd;			// �������� �������
	COLORREF Color;			// ���� ���������
	SpanColorLinePixStruct()
	{
		SpanColorLinePixStruct(0.f, 0.f, RGB(255, 255, 255));
	}
	SpanColorLinePixStruct(float _Start, float _End, COLORREF _Color)
		: PixStart		(_Start)
		, PixEnd		(_End)
		, Color			(_Color)
	{}
};
//-------------------------------------------------------------------------------------------------
// ��������� � ��������� ������� ��� �����������
struct SpanColorLineIntervalStruct
{
	// �������� � �������� ��� ��������� ������
	int IndexStart, IndexEnd;
	// ��������� �������� ��� ��������� ������
	double IntervalStart, IntervalEnd;			
	COLORREF Color;			// ���� ���������
	SpanColorLineIntervalStruct()
		: IndexStart(0)
		, IndexEnd(0)
		, Color(0)
	{
		SpanColorLineIntervalStruct(ZetTimeMin, ZetTimeMax, 0, 0, RGB(255, 255, 255));
	}
	SpanColorLineIntervalStruct(double _TimeStart, double _TimeEnd, int _IndexBegin, int _IndexEnd, COLORREF _Color)
		: IntervalStart	(_TimeStart)
		, IntervalEnd	(_TimeEnd)
		, IndexStart	(_IndexBegin)
		, IndexEnd		(_IndexEnd)
		, Color			(_Color)
	{}
	SpanColorLineIntervalStruct(double _TimeStart, double _TimeEnd, COLORREF _Color)
		: IntervalStart	(_TimeStart)
		, IntervalEnd	(_TimeEnd)
		, Color			(_Color)
	{
	}
	SpanColorLineIntervalStruct(int _IndexBegin, int _IndexEnd, COLORREF _Color)
		: IndexStart	(_IndexBegin)
		, IndexEnd		(_IndexEnd)
		, Color			(_Color)
	{}
};
//-------------------------------------------------------------------------------------------------
struct ColorLineStruct		// ��������� � ����������� �� ��������
{
	bool bIndexWork;		// ���� ��� ����������� ������ �������� ��������

	COLORREF FontColor;		// ���� ������ ����
	COLORREF TextColor;		// ���� ������ ��� ��������

	CRect FullRect;			// ���������� ��������������

	float PixBegin,			// ���������� ������ � �����
		  PixEnd;				// ��������� ������� ������� ��������

	CString Caption;		// �������� ����������
	GLuint FontList;		// �����

	// ������ ���������� �� �������� ��� ��������� ��������� �� ������
	std::vector<SpanColorLinePixStruct> SpanDrowVector;	
	// ������ ���������� �� ������� ��� ��������� ��������� �� ������
	std::vector<SpanColorLineIntervalStruct> SpanDateVector;

	ColorLineStruct()
		: FontColor			(RGB(255, 255, 255))
		, TextColor			(RGB(0, 0, 0))
		, bIndexWork(false)
		, PixBegin(0.f)
		, PixEnd(0.f)
	{
	}
};
//-------------------------------------------------------------------------------------------------
class CColorLine
{
public:	//-----------------------------------------------------------------------------------------
	CColorLine();
	~CColorLine();
	// ���������� ������� c �����������
	void DrowColorLine(_In_ CDC* pdc, _In_ struct ColorLineStruct *Struct);
private://-----------------------------------------------------------------------------------------
public:	//-----------------------------------------------------------------------------------------
private://-----------------------------------------------------------------------------------------
};

