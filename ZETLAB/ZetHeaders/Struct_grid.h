#pragma once
#include <ZetColors.h>
#include <Grafic1\gridglctrl1.h>
//*************************************************************************************************
//#define MaxNumber 80
#ifdef MaxNumber
#error Error: "MaxNumber" defined in some other file.
#endif
//*************************************************************************************************
namespace GridGL
{
	//---------------------------------------------------------------------------------------------
	const static int g_Axis_Count(4);				// ���������� ���� ��� �������
	const static int g_Max_Count_AdditionWnd(99);	// ������������ ���������� �������������� ���� �� �������
	const static int g_Max_Text_Length(200);		// ������������ ����� ������� �� �����
	//---------------------------------------------------------------------------------------------
	enum AxisSide			// ������������ ������ ��� �����
	{
		AxisDefault = 0,					// ������� ����� �����
		AxisSideRight_Right,				// ����� ������ �����
		AxisSideRight_Left,					// ����� ������ �����  �� ���
		AxisSideLeft_Right,					// ����� �����  ������ �� ���
	};
};
//*************************************************************************************************
struct AxisInfo				// ��������� � ����������� ������������ �����
{
	GridGL::AxisSide Side;					// ������� ������������ ����� 
	bool bUseAxis;							// true - ���������� ������ ��� | false - �� ���������� ������ ���
	float Multiplicative;					// ����������������� ����������� �������� ������ � �������� ������
	float Additive;							// ����������        ����������� �������� ������ � �������� ������
	long CurrentMaxCX;						// ������� ������������ ������ ������ ��� �����
	long CurrentTextHeight;					// ������� ������ ������ ��� �����
	COLORREF Color;							// ���� �����
	WCHAR Text[GridGL::g_Max_Text_Length];	// ����� �� �����
	AxisInfo()
	{
		SetValueDefault();
	}
	void SetValueDefault()
	{
		Side			= GridGL::AxisSide::AxisDefault;
		bUseAxis		= false;
		Multiplicative	= 1.f;
		Additive		= 0.f;
		CurrentMaxCX	= 0;
		CurrentTextHeight = 0;
		Color			= ZETColors::DigitsColor;
		ZeroMemory(Text, sizeof(Text));
	}
};
//*************************************************************************************************
struct AdditionalWindow		// ��������� � ����������� �������������� ����
{
	bool bVisible;								// true - ���������� | false - �� ����������
	long GraphIndex;							// ������ ������� � �������� ��������� ����
	long PointIndex;							// ������ ����� �� �������
	CPoint WindowPoint;							// ������������ ������ ����
	COLORREF TextColor;							// ���� ������
	COLORREF FonColor;							// ���� ����
	WCHAR Text[GridGL::g_Max_Text_Length * 2];	// ����� � ����
	AdditionalWindow()
	{
		SetValueDefault();
	}
	void SetValueDefault()
	{
		bVisible	= false;
		GraphIndex	= 0;
		PointIndex	= 0;
		WindowPoint.SetPoint(0, 0);
		TextColor	= ZETColors::TextColor;
		FonColor	= ZETColors::AddtionalWindowFon;
		ZeroMemory(Text, sizeof(Text));
	}
};
//*************************************************************************************************
struct grid 
{
	//��������� #define �� ����� ������ ��������� � ����� ���� �������������,
	//���������� ���������� ������ grid
	static const int MaxNumber = 240;
	static const int MaxTextLengthInByte = GridGL::g_Max_Text_Length;
	 long sizeofgrid;				//������ grid-� � ������
	 int left_x,					//������� �����
		 right_x,					//������ ������
		 top_y,						//���� �������
		 bottom_y,					//���������� �� ������� ���� ���� �� ������� ���� �������
		 width_x,					//������ ������� �� ��� X
		 height_y,					//������ ������� �� ��� Y
		 number_of_strings_in_hat,	//���������� ����� � �����
		 cursor_x,					//������� ������� �� ��� x � �������� ����������� 
		 step,						//��� �������, ���������� ����� ��������� ������� � �������� ����-���
		 mouse_x,					//������� ���� � �������� ���� - ���
		 mouse_y,					//������� ���� � �������� ���� - ���
		 timer,						//������� ��������� �������
		 update,    				//������� ���������� �������
		 spectr,					//������� ����, ��� ������� ������ - ������
		 osc;						//������� ����, ��� ������� ������ - ��������� ����������
	 long font_size,				//������ ������
		  make_upor,				//������� ������ �� ��� Y, 0- ��� ������, 1- ����
		  number,					//���-�� ��������
		  ind,						//����� �������
		  num_visible_points,		//����� ������� ����� �������
		  position_of_cursor,		//����� �������� ������� bf, �� ������� ����� ������, �������� ��������� �������� �� 0 �� Size
		  size,						//������ ������
		  type_grf[MaxNumber],      //��� ������� 0-�������������, 1-����� ����������
		  type_line,			    //0 - ���. �������, 1- �������� ������,
		  type_fill[MaxNumber],		//0 - � �����������, 1 - ��� ����������					
		  valid[MaxNumber],			//0 - �� ������ ��������� ������, 
									//1 - ������
		  type_abs,					//��� ���� 0- �����, 
									//1- ��� �� �,4-��� ���� �� Y, 
									//2 - ��� ���� �� , 
									//8 - ��� �������� �� Y, 
									//16 - ��� �������� �� �
		  type_x_axis,				//��� ����� �� ��� X, 0 - ��������, 
									//1 - ���������������
									//2 - ����������������
		  type_y_axis,				//��� ����� �� ��� X, 
									//0 - ��������, 
									//1 - ���������������
									//2 - �����������
		  type_y_data,              //0 - ������ ������ � �������,
									//1 - ������ ������ ����������
		  log,						//1 - � �������������� ���������� �������� ����� �� �
									//0 - 1/n - ��������
		  alt_lin,
		  line_width[MaxNumber],	//������� �����
		  graph_axis[MaxNumber];	//������ �����, � ������� �������� ������ | 0 - ������� ����� ����� / 1 - �������������� ����� ������
	double math_width_x,			//�������. ������ ����� �� �
		   math_width_y,			//�������. ������ ����� �� Y
		   math_min_x,				//������� ����������� �������� �� X
		   math_min_y,				//������� ����������� �������� �� Y
		   upor_delta,				//����������� �������� ������ ����� �� Y
		   upor_down,				//������ ������ �� Y
		   upor_up,					//������� ������ �� Y
		   x_first,					//������ ������ �� �
		   x_end,					//������� ������ �� �
		   reference;               //��������
	WCHAR
		 frm1[MaxTextLengthInByte],					//������ ������� ���������� �
		 frm2[MaxNumber][MaxTextLengthInByte],		//������ ������� ���������� �
		 graph_text[MaxNumber][MaxTextLengthInByte],//����� � �������
		 caption[MaxTextLengthInByte],				//
		 text_x_axis[MaxTextLengthInByte],			//������� ��� X
		 font_name[MaxTextLengthInByte];			//�������� ������

	COLORREF graph_color[MaxNumber];	//���� �������, ��� ������� ������� �������� ��������	
	COLORREF cursor_color;				//���� �������
	COLORREF digits_color;				//���� ����
	COLORREF fon_color;					//���� ����
	COLORREF grid_color;				//���� �����
	COLORREF legend_color;				//���� �������
	long show_scale_prop_page;          //FALSE - �������� �������� ������ ������� "�����" 
										//TRUE  - ����������
	long alt_coords;                    //TRUE - �������������� ����� �� ��� X  
	long power_coeff;					//	TRUE - � ���������� �������� ��������, FALSE - � ���������� �������� ���������
	AxisInfo AxisInfoStruct[GridGL::g_Axis_Count];	// ������� ���������� ��� ����
	AdditionalWindow AdditionalWindows[GridGL::g_Max_Count_AdditionWnd];	// ������� ���������� ��� �������������� ����
	grid()
	{
		SetValueDefault();
	}
	AxisInfo * GetAxisBySide(_In_ const GridGL::AxisSide &SideID)
	{
		if (SideID < 0 ||
			SideID > GridGL::g_Axis_Count)
			return nullptr;

		for (auto &AxisData : AxisInfoStruct)
		{
			if (AxisData.Side == SideID)
				return &AxisData;
		}

		return nullptr;
	}
	void SetValueDefault()
	{
		sizeofgrid		= sizeof(grid);
		left_x			= 0;
		right_x			= 0;
		top_y			= 0;
		bottom_y		= 0;
		width_x			= 0;
		height_y		= 0;
		number_of_strings_in_hat = 0;
		cursor_x		= 0;
		step			= 0;
		mouse_x			= 0;
		mouse_y			= 0;
		timer			= 0;
		update			= 0;
		spectr			= 0;
		osc				= 0;
		font_size		= 12;
		make_upor		= 0;
		number			= 1;
		ind				= 0;
		num_visible_points = -1;
		position_of_cursor = 0;
		size			= 100;
		type_line		= 1;
		type_abs		= 0;
		type_x_axis		= 0;
		type_y_axis		= 0;
		type_y_data		= 0;
		log				= 0;
		alt_lin			= 0;
		math_width_x	= 100.;
		math_width_y	= 2.;
		math_min_x		= 1.;
		math_min_y		= 0.;
		upor_delta		= 0.000001;
		upor_down		= -100000000.;
		upor_up			=  100000000.;
		x_first			= 0.;
		x_end			= 100.;
		reference		= 0.000001;
		show_scale_prop_page = 1;
		alt_coords		= 0;
		power_coeff		= 0;
		cursor_color	= ZETColors::CursorColor;
		digits_color	= ZETColors::DigitsColor;
		fon_color		= ZETColors::BackgroundColor;
		grid_color		= ZETColors::GridColor;
		legend_color	= ZETColors::LegendColor;

		ZeroMemory(frm1,		sizeof(frm1));
		ZeroMemory(frm2,		sizeof(frm2));
		ZeroMemory(caption,		sizeof(caption));
		ZeroMemory(text_x_axis, sizeof(text_x_axis));
		ZeroMemory(font_name,	sizeof(font_name));

		for (int i = 0; i < MaxNumber; ++i)
		{
			valid[i] = 1;
			type_fill[i] = 0;
			type_grf[i] = 0;
			line_width[i] = 1;
			graph_axis[i] = GridGL::AxisSide::AxisDefault;
			graph_color[i] = ZETColors::Graph_Colors[i % GRAPH_COLORS_COUNT];
			swprintf_s(graph_text[i], _T("������ %d"), i);
		}

		for (int i = 0; i < GridGL::g_Axis_Count; i++)
		{
			AxisInfoStruct[i].SetValueDefault();
			AxisInfoStruct[i].Side = GridGL::AxisSide(i);

			if (AxisInfoStruct[i].Side == GridGL::AxisSide::AxisDefault)
				AxisInfoStruct[i].bUseAxis = true;
			else
				AxisInfoStruct[i].bUseAxis = false;
		}

		for (auto & it : AdditionalWindows)
			it.SetValueDefault();
	}
	// ����������� �������� �� ������� � ���������
	void GetParamFromGrid(CGridglctrl1 * pGridGL)
	{
		if (pGridGL)
			pGridGL->InWindPtr((signed char*)this);
	}
	// �������� ������� �������� �� ���������
	void SetParamIntoGrid(CGridglctrl1 * pGridGL)
	{
		if (pGridGL)
			pGridGL->OutWindPtr((signed char*)this);
	}
};
//*************************************************************************************************