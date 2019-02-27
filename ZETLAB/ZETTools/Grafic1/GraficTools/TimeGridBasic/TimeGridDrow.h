#pragma once
#include "Struct_trend.h"
#include <mathZet.h>
#include <tuple>
#include <Grafic1\GraficTools\ColorLine\ColorLine.h>
#include <freetype_bin\include\FreeTypeFont.h>
#include <Grafic1/GraficTools/GridDigits/GridDigits.h>
#include <Grafic1\GraficTools\GridLayoutX\GridLayoutX.h>
//*************************************************************************************************
#define INTERVAL_BETWENN_COLORLINES		4			// �������� ����� ����������� �������
#define SPLIT_FIELDS_DELTA				17			// ������ ��� ����� ������������� ������������ � ����������� �������� ����� ��� ������ SplittingFields
#define MAX_PIX_SIZE					150			// ���������� ����� ������������� ������ ����� �������� �������� ������������� �����
#define SHAG_CURSOR_INPIX				3			// ������������ ������ ���� �������
#define NEED_INVALIDATE					2			// ������������ �������� � ������ ������������� ������������ �������
#define CURSOR_SIZE						7			// ������ �������
#define PERSENTAGE_INDENTION			0.01 * 5	// �� ����� ������� �� ������ ������� ������� ������ �� ����� ��� ������������
#define MIN_WIDTH						1.e-10		// ����������� ������ ��� ����������
//*************************************************************************************************
// ������� ��������� �������� �������� �����, � ��������� � ����
float CalculateStunnerValues(_In_ float Val, _Out_opt_ float *PrevVal = nullptr, _Out_opt_ float *NextVal = nullptr);
//*************************************************************************************************
class CTimeGridDrow
{
public:
	CTimeGridDrow();
	~CTimeGridDrow();
protected:	
	//*********************************************************************************************
	struct MyPointsStruct   // ��������� � ������� ��� ���������
	{
		GLint x;
		GLfloat y;
	};
	//*********************************************************************************************
	typedef struct MarkStruct
	{
		MarkStruct()
			: MarkStruct(0, _T(""), RGB(255, 0, 0))
		{}
		MarkStruct(double _Time, CString _Text, COLORREF _Color)
			: Time	(_Time)
			, Text	(_Text)
			, Color	(_Color)
		{}
		double Time;
		CString Text;
		COLORREF Color;
	};
	std::vector<MarkStruct> m_MarkLines;			// ������ ������� �� �������
	//*********************************************************************************************
	typedef struct ToolTipMemb
	{
		ToolTipMemb()
		{
			text.Empty();
			pos = 0;
			rect = CRect(0, 0, 20, 20);
			addedToVisible = false;
		}
		CString text;
		long pos;
		CRect rect;
		bool addedToVisible;
	};
	std::vector <ToolTipMemb> m_toolTips;			// ��� �������� �������
	//*********************************************************************************************
	// ������ ������������ ����� �������
	enum CursorFigure
	{
		Figure_Thriangle = 0,
		Figure_Square,
		Figure_Circle
	};
	// ���� �������
	enum CursorType
	{
		Type_Default = 0,		// ������� ������, ���������� ����, ������������� � �����
		Type_Imposition,		// ������, �������������� ��� ������ � ������ ��������� ��������
		Type_Users				// ��������� �������������
	};
	typedef struct CursorStruct
	{
		CursorStruct()
			: CursorStruct(0, CursorFigure::Figure_Thriangle, RGB(0, 0, 255), CursorType::Type_Default)
		{}
		CursorStruct(double _CurrentPos, CursorFigure _Figure, COLORREF _Color, CursorType _Type)
			: CurrentPos	(_CurrentPos)
			, Figure		(_Figure)
			, Color			(_Color)
			, Type			(_Type)
		{}
		double CurrentPos;		// ������� ��������� �������
		CursorFigure Figure;	// ������ ������������ ��� �������������� �������
		COLORREF Color;			// ���� �������
		CursorType Type;		// ��� �������
	};
	//*********************************************************************************************
	typedef struct SplitFieldsStruct
	{
		float MinValue;			// �����������  �������� ��� ����
		float MaxValue;			// ������������ �������� ��� ����
		float PixYMin;			// ������� �� ������� ��������� �����������  �������� ��� ����� ����
		float PixYMax;			// ������� �� ������� ��������� ������������ �������� ��� ����� ����
		float TopLineCoord;		// ���������� ������� ����� ����������� �������			

		SplitFieldsStruct()
		{
			ZeroMemory(this, sizeof(SplitFieldsStruct));
		}
		SplitFieldsStruct(const SplitFieldsStruct & val)
		{
			memcpy_s(this, sizeof(SplitFieldsStruct), &val, sizeof(SplitFieldsStruct));
		}
		SplitFieldsStruct & operator = (const SplitFieldsStruct & val)
		{
			memcpy_s(this, sizeof(SplitFieldsStruct), &val, sizeof(SplitFieldsStruct));
			return *this;
		}
	};
	float m_bSplitFieldsHeight;			// ������, ��������� ������� ����
	// ���� ������������� ���������� ��� ������ �����
	// false - �� ������������ ��� ������� ������, �� ����������� ���������� ��� �����
	// true - ��� ���� ������������ ������
	bool m_bSplitFieldsNeedNameOnTop;
	std::vector<SplitFieldsStruct> m_SplitFieldsData;		// ������ ���������� ��� ������� ����
	//*********************************************************************************************
	std::vector<ColorLineStruct> m_ColorLine;		// ������ ������� �����
	//*********************************************************************************************
	std::vector<bool> m_bValidGraphs;				// ��������� �������		| true - ������������ / false - �� ������������
	std::vector<CString> m_FieldsName;				// ������ � ����������� ��� ������� ����
	std::vector<CString> m_FieldsUnits;				// ������ � ��������� ��������� ��� ������� ����
	//*********************************************************************************************
	GridLayoutX CursorCoords;						// ��������� ���������� ������������ ��������
	CGridDigits m_VertDigits;							// ������������ ��������
	std::vector<CursorStruct> m_CoordVertCurs;		// ���������� ������������ ��������
	std::vector<CursorStruct> m_CoordGorCurs;		// ���������� �������������� ��������
	//std::vector<double> m_CoordVertCurs;				// ���������� ������������ ��������
	//std::vector<double> m_CoordGorCurs;				// ���������� �������������� ��������
	int CurSelectedCursX;							// ������ �������� ���������� ������������� ������� 
	int CurSelectedCursY;							// ������ �������� ���������� ��������������� ������� 
	bool VertCursMoves;								// ���� ����������� ������������� �������
	bool GorCursMoves;								// ���� �������������� ��������������� �������
	//*********************************************************************************************
	GLuint m_FontList;
	HPALETTE m_hPalette;
	HGLRC m_hRC;
	HDC  m_hDC;										//��������� �� �������� ����������	

	bool m_bNeedLoadNewFont;						// ���� ���� ��� ��� �������� ����� �����
	FreeTypeFont m_TextFont;						// ����� ����������� ������

	HFONT m_FontNew;
	bool stop;
	bool bUseOrtho2D;								// ���� ���� ��� ����� ������������ 2������ ���������
	//*********************************************************************************************
public:
	double m_LeftXCoord, m_RightXCoord;					// ����� � ������ ������� �� ��� �
	double m_VisibleLeftXCoord, m_VisibleRightXCoord;	// ����� � ������ ������� �� ��� � ������� ������������ ��������
	double m_TopYCoord, m_BottomYCoord;					// ������� � ������ ������� �� ��� Y
	double m_VisibleTopYCoord, m_VisibleBottomYCoord;	// ������� � ������ ������� �� ��� Y ������� ������������ ��������
	double m_IntervalTopYCoord, m_IntervalBottomYCoord;	// ������� � ������ ������� �� ��� Y �� ������� ���������
	//*********************************************************************************************
public:
	CString m_SelectedGraphName;						// ��� ���������� �������
	int m_FontListsNum;
	size_t m_CountGraph;								// ����� ���������� ������������ �����
	size_t m_CurGraphCount;								// ���������� ������������ �����
protected:
	void TextGL(GLfloat x, GLfloat y, LPCTSTR text, GLubyte R, GLubyte G, GLubyte B);
	void glPrint(LPCTSTR text);
	//*********************************************************************************************
	float GetPixX(struct TrendGrid *grd, double x);
	float GetPixY(struct TrendGrid *grd, double y);
	//*********************************************************************************************
	// �������� ������� � ��������� ����
	CString GetTimeText(_In_ CZetTimeSpan DeltaTime);	// ������� �� �������
	CString GetDataText(_In_ double DeltaTime);			// ������� � ������
	// ���������� ����� ���������
	void GetCursDistance(struct TrendGrid* grd, _Out_ CString &HorStr, _Out_ CString &VertStr);
	//*********************************************************************************************
	// �������� ������� ������� ��������� ���� ��� ��������� ��� ���� ��������
	CString GetCurrentUnitStr();
	//*********************************************************************************************
	// ��������� �������� �� ��������� �� � � Y
	double GetXFromCoord(struct TrendGrid *grd, long);
	double GetYFromCoord(struct TrendGrid *grd, long);
	//*********************************************************************************************
#ifndef _UNICODE
	enum { m_FontListsStart = 0, m_FontListsEnd = 257 };
#else
	enum { m_FontListsStart = 0, m_FontListsEnd = (1 + (int)MAXWORD) };
#endif
	//*********************************************************************************************
	void InicializeOrtho(CRect Rect);									// ������������� ������ ORTHO
	bool PrepareOGL(struct TrendGrid *grd, CDC* pdc, const CRect &rc);	// ��������� OPENGL
	bool BuildFontOGL(HDC hDC, HFONT hFont, long FontSize = -1);		// ��������� ������
	//*********************************************************************************************
	void CalculateControlSize(CDC* pdc, struct TrendGrid *grd, const CRect &rcBounds);	// ��������� ������� ��������
	int  SetSetka(CDC* pdc, struct TrendGrid *grd, const CRect &rcBounds, bool m_bEnable, 
				  int CountGraphs = 0);	// ������ ��� ������������� � �����
	//*********************************************************************************************
	void DrowVertLine(struct TrendGrid* grd, MyPointsStruct pnt1, MyPointsStruct pnt2, long posX);
	long VertLines(CDC* pdc, struct TrendGrid* grd);					// ������������ �����
	long DefaultHorLines(struct TrendGrid* grd);						// ����������� �������������� �����
	void PaintIntermediateLines(struct TrendGrid* grd, float Top, float Bottom);// ������������ ������������� ����� � ������ ���������� �� �������
	long SplitFieldsHorLines(CDC* pdc, struct TrendGrid* grd);			// �������������� ����� ��� ������ ���������� �� �������
	long ImpositionHorLines(CDC* pdc, struct TrendGrid* grd);			// �������������� ����� ��� ������ ���������� �� �������
	//*********************************************************************************************
	void DrowVertCursor(_In_ struct TrendGrid *grd, _In_ const CursorStruct &CursInfo, _In_ bool bSelected);
	void DrowHorzCursor(_In_ struct TrendGrid *grd, _In_ const CursorStruct &CursInfo, _In_ bool bSelected);	
	void DrawCursorsX(_Inout_ struct TrendGrid *grd);					// ���������� ������������ �������
	void DrawCursorsY(_Inout_ struct TrendGrid *grd);					// ���������� �������������� �������
	//*********************************************************************************************
	void PaintDistinguish(CPoint, CPoint);								// ������ ������������� � ����������
	//*********************************************************************************************
	_Check_return_ int SetSetkaDigits(CDC* pdc, struct TrendGrid *grd, const CRect &rcBounds);	// ���������� NEED_INVALIDATE ���� ����� �������� �������
	long VertDigits(CDC* pdc, struct TrendGrid* grd, const CRect &rcBounds);					// ������������ ��������
	//*********************************************************************************************
	/*	*������� ������������ ��� � ����� �������� �������� �����������
		@param Data - �������� ������ � ������� [Y ���������� �� ������� ���� ����������� �������, �������]
		@return NEED_INVALIDATE ���� ����� ����� ����������� ������	*/	
	//long CalculateSeismoNames(CDC* pdc, struct TrendGrid* grd, const CRect &rcBounds, _Out_ std::vector<std::pair<float, CString>> *Data);
	long VertDigitsSeismo(CDC* pdc, struct TrendGrid* grd, const CRect &rcBounds);				// �������� � ���� ���� ������� �������
	//*********************************************************************************************
	long SplitFieldsVertDigits(CDC* pdc, struct TrendGrid* grd, const CRect &rcBounds);			// ������������ �������� ��� ������ ���������� �� �������
	// ������������� ������������ ��������
	long CalculateIntermediateSplitDigits(_Inout_ struct TrendGrid *grd, _In_ CDC* pdc,				
										  _In_ double MaxVal, _In_ double MinVal, 
										  _In_ GLfloat CurCoord, _In_ GLfloat HeightVert, _In_ bool NeedDrow = false);
	void CalculateSplitFieldsSettings(_In_ struct TrendGrid* grd, _In_ size_t CountOfFields, _In_opt_ bool bCheckHeight = true);
	//*********************************************************************************************
	long HorDigits(CDC* pdc, struct TrendGrid* grd);											// �������� �� �����������
	//*********************************************************************************************
	virtual void CheckScaleX(struct TrendGrid *grd);							// �������� ��� ��������������� ����� �� ��� �
	void CheckScaleY(struct TrendGrid *grd);							// �������� ��� ��������������� ����� �� ��� Y
	//*********************************************************************************************
	void AutoScaleVisibleData(struct TrendGrid* grd);					// ������������������� ������������ �����
	void AutoScaleAllData(struct TrendGrid* grd);						// ������������������� ���� �����
	void AutoScaleYData(struct TrendGrid* grd);							// ������������������� ������ � ������� ������� �� ��� Y
	//*********************************************************************************************
	void DrowColorLine(CDC* pdc, struct TrendGrid* grd, const CRect &rcBounds);	// ������ ����� ����� ��������
	void DrowMarkLine (CDC* pdc, struct TrendGrid* grd, const CRect &rcBounds);	// ������������ ������� �� �������
	//*********************************************************************************************
	void DrawToolTips(struct TrendGrid* grd, CDC* pdc);
	void AddOrRemoveToolTipToVisible(CPoint point);
private:
	void SetupPalette();
	void AutoScale (_Inout_ struct TrendGrid* grd, _In_ double _LeftXCoord,   _In_ double _RightXCoord, _In_ double _BottomYCoord, _In_ double _TopYCoord);
	void AutoScaleX(_Inout_ struct TrendGrid* grd, _In_ double _LeftXCoord,   _In_ double _RightXCoord);
	void AutoScaleY(_Inout_ struct TrendGrid* grd, _In_ double _BottomYCoord, _In_ double _TopYCoord);
};