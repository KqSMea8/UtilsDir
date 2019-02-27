//------------------------------------------------------------------------------
#pragma once
#ifndef	zdpdef_h
#define	zdpdef_h


enum zdGUIType
{
	gtGdiPlus,
	gtOpenGl,
	gtDirect2D
};

//------------------------------------------------------------------------------
enum zdDrawMode
{// ���� ��� ����������
	dmHighSpeed,
	dmHighQuality
};
//------------------------------------------------------------------------------
enum zdFigureType
{
	ftLine,
	ftRectangle,
	ftEllipse,
	ftRoundRect,
	ftPolygon,
	ftCurve
};
//------------------------------------------------------------------------------
enum zdFillType
{// ���� ����������
	ftNone,
	ftSolid,
	ftTexture,
	ftGradientLine,
	ftGradientPoint
};
//------------------------------------------------------------------------------
enum zdLineStyle
{// ��� �����
	lsSolid,	// ��������
	lsDashed,	// ��������� �� �������� �����
	lsDotted,	// ���������� (�� �����)
	lsDashDot,	// �����-���������� (����, �����)
};
//------------------------------------------------------------------------------
enum zdFontStyle
{// ����� �������
	fsRegular    = 0,
	fsBold       = 1,
	fsItalic     = 2,
	fsBoldItalic = 3,
	fsUnderline  = 4,
	fsStrikeout  = 8
};
//------------------------------------------------------------------------------
enum zdStringAlignment
{// ���� ������������ �����
	saNear,
	saCenter,
	saFar
};
//------------------------------------------------------------------------------
struct zdRect
{// ������������� - ����� ������� �����, ������, ������
	long x;
	long y;
	long cx;
	long cy;
	zdRect() { x = y = cx = cy = 0; }
};
//------------------------------------------------------------------------------
struct zdSize
{// ������� - ������, ������
	long cx;
	long cy;
	zdSize() { cx = cy = 0; }
};
//------------------------------------------------------------------------------
struct zdFontParam
{// ��������� ������
	const WCHAR *family;
	float size;
	zdFontStyle style;
	COLORREF color;

	zdStringAlignment aligHor;
	zdStringAlignment aligVert;

	zdFontParam()
	 : family(L"Arial")
	 , size(9.0f)
	 , style(fsRegular)
	 , color(RGB(23, 23, 23))
	 , aligHor(saCenter)
	 , aligVert(saCenter)
	{}
};
//------------------------------------------------------------------------------
// struct zdFigureParam
// {// ��������� ������
// 	zdFigureType f_type;
// 	long size_params;
// 	long *f_params;
// 
// 	zdFigureParam()
// 	 : f_params(NULL)
// 	 , size_params(0)
// 	{}
// 
// 	~zdFigureParam()
// 	{
// 		if(f_params)			// ��������� ������ ���� ������ ������� ������� zdDeleteFigure, �� �� ������ ������ ����� ����������� � ���� ��������� �� �������, �� ������� �����.
// 			delete[] f_params;
// 	}
// 
// };
//------------------------------------------------------------------------------
struct zdPenParam
{// ��������� ����
	COLORREF color;
	float thickness;
	zdLineStyle lStyle;

	zdPenParam()
	 : color(RGB(23, 23, 23))
	 , thickness(1.0f)
	 , lStyle(lsSolid)
	{}
};
//------------------------------------------------------------------------------
struct zdBrushParam
{// ��������� �����
	zdFillType type;
	COLORREF colorFrom;
	COLORREF colorTo;

	float angle;		

	float transformX;	//only for texture brush
	float transformY;	//
	
	float focus;		 
	float scale;	

	POINT centrPoint;	// only for point brush

	zdRect rect;

	zdBrushParam()
	 : type(ftNone)
	 , colorFrom(RGB(255, 255, 255))
	 , colorTo(RGB(23, 23, 23))
	 , angle(0.0f)
	 , transformX(0.0f)
	 , transformY(0.0f)
	 , focus(0.0f)
	 , scale(0.0f)
	{}
};
//------------------------------------------------------------------------------
struct zdFont
{// "�����", �� ����������� � ������-���� ������������ ����������
	PVOID pointer;
	zdFontParam param;

	zdFont() 
	 : pointer(NULL)
	{}

	~zdFont()
	{
		if(pointer)				// ��������� ������ ���� ������ ������� ������� zdDeleteFont, �� �� ������ ������ ����� ����������� � ���� ��������� �� �������, �� ������� �����.
			delete pointer;
	}
};
//------------------------------------------------------------------------------
// struct zdFigure
// {// "������", �� ����������� � ������-���� ������������ ����������
// 	PVOID pointer;
// 	zdFigureParam param;
// 
// 	zdFigure() 
// 	 : pointer(NULL)
// 	{}
// 
// 	~zdFigure()
// 	{
// 		if(pointer)				// ��������� ������ ���� ������ ������� ������� zdDeletePicture, �� �� ������ ������ ����� ����������� � ���� ��������� �� �������, �� ������� �����.
// 			delete pointer;
// 	}
// };
//------------------------------------------------------------------------------
struct zdPicture
{// "��������", �� ����������� � ������-���� ������������ ����������
	PVOID pointer;
	zdSize param;	// �������� �������

	zdPicture() 
	 : pointer(NULL)
	{}

	~zdPicture()
	{
		if(pointer)				// ��������� ������ ���� ������ ������� ������� zdDeletePicture, �� �� ������ ������ ����� ����������� � ���� ��������� �� �������, �� ������� �����.
			delete pointer;
	}
};
//------------------------------------------------------------------------------
struct zdPen
{// "����", �� ����������� � ������-���� ������������ ����������
	PVOID pointer;
	zdPenParam param;

	zdPen()
	 : pointer(NULL)
	{}

	~zdPen()
	{
		if(pointer)				// ��������� ������ ���� ������ ������� ������� zdDeletePen, �� �� ������ ������ ����� ����������� � ���� ��������� �� �������, �� ������� �����.
			delete pointer;
	}
};
//------------------------------------------------------------------------------
struct zdBrush
{// "�����", �� ����������� � ������-���� ������������ ����������
	PVOID pointer;
	zdBrushParam param;

	zdBrush() 
	 : pointer(NULL)
	{}

	~zdBrush()
	{
		if(pointer)				// ��������� ������ ���� ������ ������� ������� zdDeleteBrush, �� �� ������ ������ ����� ����������� � ���� ��������� �� �������, �� ������� �����.
			delete pointer;
	}
};
//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------