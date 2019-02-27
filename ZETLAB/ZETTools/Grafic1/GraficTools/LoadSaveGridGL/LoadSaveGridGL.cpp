#include "stdafx.h"
#include "LoadSaveGridGL.h"
#include <boost/lexical_cast.hpp>
//*************************************************************************************************
#define CURRENT_CONFIG_VERSION	1.0	// ферсия конфигурационного файла
//*************************************************************************************************
template <class T>
void AddChildToNode(_In_ pugi::xml_node Node, _In_ const CString &Name, _In_ const T &Val);
//-------------------------------------------------------------------------------------------------
template <class T>
void AddAttributeToChild(_In_ pugi::xml_node Node, _In_ const CString &Name, _In_ const T &Val);
//-------------------------------------------------------------------------------------------------
template <class T>
bool GetAttributeFromNode(_In_ pugi::xml_node Node, _In_ const CString &Name, _In_ T *Val, _In_opt_ const size_t &TextLength = 0);
//-------------------------------------------------------------------------------------------------
template <class T>
bool GetValueFromNode(_In_ pugi::xml_node Node, _In_ const CString &Name, _In_ T *Val, _In_opt_ const size_t &TextLength = 0);
//*************************************************************************************************
CLoadSaveGridGL::CLoadSaveGridGL()
{
}


CLoadSaveGridGL::~CLoadSaveGridGL()
{
}

bool CLoadSaveGridGL::SaveGridParams(_In_ CString File, _In_ grid *GridGLParams, 
									 _In_opt_ CString *Comments /*= nullptr*/,
									 _In_opt_ size_t   CommentsCount /*= 0*/)
{
	using namespace pugi;
	xml_document hFile;
	pugi::xml_node MainNode = hFile.append_child(L"Config");
	pugi::xml_attribute ChildNode = MainNode.append_attribute(L"version");
	ChildNode.set_value(CURRENT_CONFIG_VERSION);
	//---------------------------------------------------------------------------------------------
	// сохраняем общие параметры сетки
	AddChildToNode(MainNode, L"MaxNumber",				GridGLParams->MaxNumber);
	AddChildToNode(MainNode, L"MaxTextLengthInByte",	GridGLParams->MaxTextLengthInByte);
	AddChildToNode(MainNode, L"sizeofgrid",				GridGLParams->sizeofgrid);
	AddChildToNode(MainNode, L"number_of_strings_in_hat", GridGLParams->number_of_strings_in_hat);
	AddChildToNode(MainNode, L"cursor_x",				GridGLParams->cursor_x);
	AddChildToNode(MainNode, L"step",					GridGLParams->step);
	AddChildToNode(MainNode, L"mouse_x",				GridGLParams->mouse_x);
	AddChildToNode(MainNode, L"mouse_y",				GridGLParams->mouse_y);
	AddChildToNode(MainNode, L"timer",					GridGLParams->timer);
	AddChildToNode(MainNode, L"update",					GridGLParams->update);
	AddChildToNode(MainNode, L"spectr",					GridGLParams->spectr);
	AddChildToNode(MainNode, L"osc",					GridGLParams->osc);
	AddChildToNode(MainNode, L"font_size",				GridGLParams->font_size);
	AddChildToNode(MainNode, L"make_upor",				GridGLParams->make_upor);
	AddChildToNode(MainNode, L"number",					GridGLParams->number);
	AddChildToNode(MainNode, L"ind",					GridGLParams->ind);
	AddChildToNode(MainNode, L"num_visible_points",		GridGLParams->num_visible_points);
	AddChildToNode(MainNode, L"position_of_cursor",		GridGLParams->position_of_cursor);
	AddChildToNode(MainNode, L"size",					GridGLParams->size);
	//---------------------------------------------------------------------------------------------
	// сохраняем размеры сетки
	pugi::xml_node GridSizes = MainNode.append_child(L"GridDimensions");
	AddAttributeToChild(GridSizes, L"left_x",			GridGLParams->left_x);
	AddAttributeToChild(GridSizes, L"right_x",			GridGLParams->right_x);
	AddAttributeToChild(GridSizes, L"top_y",			GridGLParams->top_y);
	AddAttributeToChild(GridSizes, L"bottom_y",			GridGLParams->bottom_y);
	AddAttributeToChild(GridSizes, L"width_x",			GridGLParams->width_x);
	AddAttributeToChild(GridSizes, L"height_y",			GridGLParams->height_y);
	//---------------------------------------------------------------------------------------------
	// сохраняем параметры сетки
	pugi::xml_node GridSetup = MainNode.append_child(L"GridSetup");
	AddAttributeToChild(GridSetup, L"type_line",		GridGLParams->type_line);
	AddAttributeToChild(GridSetup, L"type_abs",			GridGLParams->type_abs);
	AddAttributeToChild(GridSetup, L"type_x_axis",		GridGLParams->type_x_axis);
	AddAttributeToChild(GridSetup, L"type_y_axis",		GridGLParams->type_y_axis);
	AddAttributeToChild(GridSetup, L"type_y_data",		GridGLParams->type_y_data);
	AddAttributeToChild(GridSetup, L"log",				GridGLParams->log);
	AddAttributeToChild(GridSetup, L"alt_lin",			GridGLParams->alt_lin);
	AddAttributeToChild(GridSetup, L"show_scale_prop_page", GridGLParams->show_scale_prop_page);
	AddAttributeToChild(GridSetup, L"alt_coords",		GridGLParams->alt_coords);
	AddAttributeToChild(GridSetup, L"power_coeff",		GridGLParams->power_coeff);
	//---------------------------------------------------------------------------------------------
	// сохраняем математические данные сетки
	pugi::xml_node GridMathData = MainNode.append_child(L"GridMathData");
	AddAttributeToChild(GridMathData, L"math_width_x",	GridGLParams->math_width_x);
	AddAttributeToChild(GridMathData, L"math_width_y",	GridGLParams->math_width_y);
	AddAttributeToChild(GridMathData, L"math_min_x",	GridGLParams->math_min_x);
	AddAttributeToChild(GridMathData, L"math_min_y",	GridGLParams->math_min_y);
	AddAttributeToChild(GridMathData, L"upor_delta",	GridGLParams->upor_delta);
	AddAttributeToChild(GridMathData, L"upor_down",		GridGLParams->upor_down);
	AddAttributeToChild(GridMathData, L"upor_up",		GridGLParams->upor_up);
	AddAttributeToChild(GridMathData, L"x_first",		GridGLParams->x_first);
	AddAttributeToChild(GridMathData, L"x_end",			GridGLParams->x_end);
	AddAttributeToChild(GridMathData, L"reference",		GridGLParams->reference);
	//---------------------------------------------------------------------------------------------
	// сохраняем цвета сетки
	pugi::xml_node GridColors = MainNode.append_child(L"GridColors");
	AddAttributeToChild(GridColors, L"cursor_color",	GridGLParams->cursor_color);
	AddAttributeToChild(GridColors, L"digits_color",	GridGLParams->digits_color);
	AddAttributeToChild(GridColors, L"fon_color",		GridGLParams->fon_color);
	AddAttributeToChild(GridColors, L"grid_color",		GridGLParams->grid_color);
	AddAttributeToChild(GridColors, L"legend_color",	GridGLParams->legend_color);
	//---------------------------------------------------------------------------------------------
	// сохраняем текстовые данные сетки
	pugi::xml_node GridText = MainNode.append_child(L"GridText");
	AddAttributeToChild(GridText, L"frm1",				GridGLParams->frm1);
	AddAttributeToChild(GridText, L"caption",			GridGLParams->caption);
	AddAttributeToChild(GridText, L"text_x_axis",		GridGLParams->text_x_axis);
	AddAttributeToChild(GridText, L"font_name",			GridGLParams->font_name);
	//---------------------------------------------------------------------------------------------
	// сохраняем параметры каждого графика
	pugi::xml_node GraphSettings = MainNode.append_child(L"GraphSettings");
	for (long i = 0; i < GridGLParams->number; ++i)
	{
		pugi::xml_node CurGraphNode = GraphSettings.append_child(L"Graph");
		AddAttributeToChild(CurGraphNode, L"number", i);
		AddAttributeToChild(CurGraphNode, L"type_grf",		GridGLParams->type_grf[i]);
		AddAttributeToChild(CurGraphNode, L"type_fill",		GridGLParams->type_fill[i]);
		AddAttributeToChild(CurGraphNode, L"valid",			GridGLParams->valid[i]);
		AddAttributeToChild(CurGraphNode, L"line_width",	GridGLParams->line_width[i]);
		AddAttributeToChild(CurGraphNode, L"graph_axis",	GridGLParams->graph_axis[i]);
		AddAttributeToChild(CurGraphNode, L"frm2",			GridGLParams->frm2[i]);
		AddAttributeToChild(CurGraphNode, L"graph_text",	GridGLParams->graph_text[i]);
		AddAttributeToChild(CurGraphNode, L"graph_color",	GridGLParams->graph_color[i]);
	}
	//---------------------------------------------------------------------------------------------
	// сохраняем параметры осей
	pugi::xml_node AxisInfo = MainNode.append_child(L"AxisInfo");
	for (long i = 0; i < GridGL::g_Axis_Count; ++i)
	{
		if (GridGLParams->AxisInfoStruct[i].bUseAxis)
		{
			pugi::xml_node CurAxisNode = AxisInfo.append_child(L"Axis");
			AddAttributeToChild(CurAxisNode, L"number", i);
			AddAttributeToChild(CurAxisNode, L"Side",			(long)GridGLParams->AxisInfoStruct[i].Side);
			AddAttributeToChild(CurAxisNode, L"UseAxis",		GridGLParams->AxisInfoStruct[i].bUseAxis);
			AddAttributeToChild(CurAxisNode, L"Multiplicative", GridGLParams->AxisInfoStruct[i].Multiplicative);
			AddAttributeToChild(CurAxisNode, L"Additive",		GridGLParams->AxisInfoStruct[i].Additive);
			AddAttributeToChild(CurAxisNode, L"Color",			GridGLParams->AxisInfoStruct[i].Color);
			AddAttributeToChild(CurAxisNode, L"Text",			GridGLParams->AxisInfoStruct[i].Text);
		}
	}
	//---------------------------------------------------------------------------------------------
	// сохраняем параметры дополнитльеных окон
	pugi::xml_node AdditionalWindows = MainNode.append_child(L"AdditionalWindows");
	for (long i = 0; i < GridGL::g_Max_Count_AdditionWnd; ++i)
	{
		if (GridGLParams->AdditionalWindows[i].bVisible)
		{
			pugi::xml_node CurWindowNode = AdditionalWindows.append_child(L"Window");
			AddAttributeToChild(CurWindowNode, L"number",		i);
			AddAttributeToChild(CurWindowNode, L"Visible",		GridGLParams->AdditionalWindows[i].bVisible);
			AddAttributeToChild(CurWindowNode, L"GraphIndex",	GridGLParams->AdditionalWindows[i].GraphIndex);
			AddAttributeToChild(CurWindowNode, L"PointIndex",	GridGLParams->AdditionalWindows[i].PointIndex);
			AddAttributeToChild(CurWindowNode, L"WindowPointX", GridGLParams->AdditionalWindows[i].WindowPoint.x);
			AddAttributeToChild(CurWindowNode, L"WindowPointY", GridGLParams->AdditionalWindows[i].WindowPoint.y);
			AddAttributeToChild(CurWindowNode, L"TextColor",	GridGLParams->AdditionalWindows[i].TextColor);
			AddAttributeToChild(CurWindowNode, L"FonColor",		GridGLParams->AdditionalWindows[i].FonColor);
			AddAttributeToChild(CurWindowNode, L"Text",			GridGLParams->AdditionalWindows[i].Text);
		}
	}
	//---------------------------------------------------------------------------------------------
	pugi::xml_node CommentsNode = MainNode.append_child(L"Comments");
	// сохраняем комментарии
	if ((Comments != nullptr) && (CommentsCount > 0))
	{
		CString CommentName;
		for (UINT Index = 0; Index < CommentsCount; ++Index)
		{
			CommentName.Format(L"Comment_%d", Index + 1);
			AddChildToNode(CommentsNode, CommentName, Comments[Index]);			
		}
	}
	//---------------------------------------------------------------------------------------------
	return hFile.save_file(File, PUGIXML_TEXT("\t"), format_default, encoding_utf8);
}

long CLoadSaveGridGL::LoadGridParams(_In_ CString File, _Out_ grid *GridGLParams,
									 _Out_opt_ std::list<CString> &Comments /*= std::list<CString>()*/)
{
	ZeroMemory(GridGLParams, sizeof(GridGLParams));
	
	using namespace pugi;
	xml_document hFile;
	
	if (!hFile.load_file(File, parse_default, encoding_utf8))
		return LoadSaveGridSetting::Errors::ERROR_LOAD_FILE;
	
	pugi::xml_node MainNode = hFile.child(L"Config");
	if (!MainNode)
		return LoadSaveGridSetting::Errors::ERROR_NO_CONFIG;
	
	GetValueFromNode(MainNode, L"sizeofgrid",	&GridGLParams->sizeofgrid);
	GetValueFromNode(MainNode, L"number_of_strings_in_hat", &GridGLParams->number_of_strings_in_hat);
	GetValueFromNode(MainNode, L"cursor_x",		&GridGLParams->cursor_x);
	GetValueFromNode(MainNode, L"step",			&GridGLParams->step);
	GetValueFromNode(MainNode, L"mouse_x",		&GridGLParams->mouse_x);
	GetValueFromNode(MainNode, L"mouse_y",		&GridGLParams->mouse_y);
	GetValueFromNode(MainNode, L"timer",		&GridGLParams->timer);
	GetValueFromNode(MainNode, L"update",		&GridGLParams->update);
	GetValueFromNode(MainNode, L"spectr",		&GridGLParams->spectr);
	GetValueFromNode(MainNode, L"osc",			&GridGLParams->osc);
	GetValueFromNode(MainNode, L"font_size",	&GridGLParams->font_size);
	GetValueFromNode(MainNode, L"make_upor",	&GridGLParams->make_upor);
	GetValueFromNode(MainNode, L"number",		&GridGLParams->number);
	GetValueFromNode(MainNode, L"ind",			&GridGLParams->ind);
	GetValueFromNode(MainNode, L"num_visible_points", &GridGLParams->num_visible_points);
	GetValueFromNode(MainNode, L"position_of_cursor", &GridGLParams->position_of_cursor);
	GetValueFromNode(MainNode, L"size",			&GridGLParams->size);
	//---------------------------------------------------------------------------------------------
	pugi::xml_node GridSizes = MainNode.child(L"GridDimensions");
	if (GridSizes)
	{
		// сохраняем размеры сетки
		GetAttributeFromNode(GridSizes, L"left_x",		&GridGLParams->left_x);
		GetAttributeFromNode(GridSizes, L"right_x",		&GridGLParams->right_x);
		GetAttributeFromNode(GridSizes, L"top_y",		&GridGLParams->top_y);
		GetAttributeFromNode(GridSizes, L"bottom_y",	&GridGLParams->bottom_y);
		GetAttributeFromNode(GridSizes, L"width_x",		&GridGLParams->width_x);
		GetAttributeFromNode(GridSizes, L"height_y",	&GridGLParams->height_y);
	}
	//---------------------------------------------------------------------------------------------
	// сохраняем параметры сетки
	pugi::xml_node GridSetup = MainNode.child(L"GridSetup");
	if (GridSetup)
	{
		GetAttributeFromNode(GridSetup, L"type_line",	&GridGLParams->type_line);
		GetAttributeFromNode(GridSetup, L"type_abs",	&GridGLParams->type_abs);
		GetAttributeFromNode(GridSetup, L"type_x_axis", &GridGLParams->type_x_axis);
		GetAttributeFromNode(GridSetup, L"type_y_axis", &GridGLParams->type_y_axis);
		GetAttributeFromNode(GridSetup, L"type_y_data", &GridGLParams->type_y_data);
		GetAttributeFromNode(GridSetup, L"log",			&GridGLParams->log);
		GetAttributeFromNode(GridSetup, L"alt_lin",		&GridGLParams->alt_lin);
		GetAttributeFromNode(GridSetup, L"show_scale_prop_page", &GridGLParams->show_scale_prop_page);
		GetAttributeFromNode(GridSetup, L"alt_coords",	&GridGLParams->alt_coords);
		GetAttributeFromNode(GridSetup, L"power_coeff",	&GridGLParams->power_coeff);
	}
	//---------------------------------------------------------------------------------------------
	// сохраняем математические данные сетки
	//pugi::xml_node GridMathData = MainNode.append_child(L"GridMathData");
	pugi::xml_node GridMathData = MainNode.child(L"GridMathData");
	if (GridMathData)
	{
		GetAttributeFromNode(GridMathData, L"math_width_x", &GridGLParams->math_width_x);
		GetAttributeFromNode(GridMathData, L"math_width_y", &GridGLParams->math_width_y);
		GetAttributeFromNode(GridMathData, L"math_min_x",	&GridGLParams->math_min_x);
		GetAttributeFromNode(GridMathData, L"math_min_y",	&GridGLParams->math_min_y);
		GetAttributeFromNode(GridMathData, L"upor_delta",	&GridGLParams->upor_delta);
		GetAttributeFromNode(GridMathData, L"upor_down",	&GridGLParams->upor_down);
		GetAttributeFromNode(GridMathData, L"upor_up",		&GridGLParams->upor_up);
		GetAttributeFromNode(GridMathData, L"x_first",		&GridGLParams->x_first);
		GetAttributeFromNode(GridMathData, L"x_end",		&GridGLParams->x_end);
		GetAttributeFromNode(GridMathData, L"reference",	&GridGLParams->reference);
	}
	//---------------------------------------------------------------------------------------------
	// сохраняем цвета сетки
	//pugi::xml_node GridColors = MainNode.append_child(L"GridColors");
	pugi::xml_node GridColors = MainNode.child(L"GridColors");
	if (GridColors)
	{
		GetAttributeFromNode(GridColors, L"cursor_color",	&GridGLParams->cursor_color);
		GetAttributeFromNode(GridColors, L"digits_color",	&GridGLParams->digits_color);
		GetAttributeFromNode(GridColors, L"fon_color",		&GridGLParams->fon_color);
		GetAttributeFromNode(GridColors, L"grid_color",		&GridGLParams->grid_color);
		GetAttributeFromNode(GridColors, L"legend_color",	&GridGLParams->legend_color);
	}
	//---------------------------------------------------------------------------------------------
	// сохраняем текстовые данные сетки
	//pugi::xml_node GridText = MainNode.append_child(L"GridText");
	pugi::xml_node GridText = MainNode.child(L"GridText");
	if (GridText)
	{
		GetAttributeFromNode(GridText, L"frm1",			GridGLParams->frm1,			GridGLParams->MaxTextLengthInByte);
		GetAttributeFromNode(GridText, L"caption",		GridGLParams->caption,		GridGLParams->MaxTextLengthInByte);
		GetAttributeFromNode(GridText, L"text_x_axis",	GridGLParams->text_x_axis,	GridGLParams->MaxTextLengthInByte);
		GetAttributeFromNode(GridText, L"font_name",	GridGLParams->font_name,	GridGLParams->MaxTextLengthInByte);
	}
	//---------------------------------------------------------------------------------------------
	// сохраняем параметры каждого графика
	pugi::xml_node GraphSettings = MainNode.child(L"GraphSettings");	
	if (GraphSettings )
	{
		pugi::xml_node CurGraphNode = GraphSettings.child(L"Graph");		
		long Index(0);
		do
		{
			if (!CurGraphNode)
				break;
			// вычитываем индекс настраиваемого графика
			GetAttributeFromNode(CurGraphNode, L"number", &Index);
			if (Index >= GridGLParams->MaxNumber)
				continue;
			// заполняем параметрами выбранный график
			GetAttributeFromNode(CurGraphNode, L"type_grf",		&GridGLParams->type_grf[Index]);
			GetAttributeFromNode(CurGraphNode, L"type_fill",	&GridGLParams->type_fill[Index]);
			GetAttributeFromNode(CurGraphNode, L"valid",		&GridGLParams->valid[Index]);
			GetAttributeFromNode(CurGraphNode, L"line_width",	&GridGLParams->line_width[Index]);
			GetAttributeFromNode(CurGraphNode, L"graph_axis",	&GridGLParams->graph_axis[Index]);
			GetAttributeFromNode(CurGraphNode, L"frm2",			 GridGLParams->frm2[Index],			GridGLParams->MaxTextLengthInByte);
			GetAttributeFromNode(CurGraphNode, L"graph_text",	 GridGLParams->graph_text[Index],	GridGLParams->MaxTextLengthInByte);
			GetAttributeFromNode(CurGraphNode, L"graph_color",	&GridGLParams->graph_color[Index]);

		} while ((CurGraphNode = CurGraphNode.next_sibling()) != nullptr);
	}
	//---------------------------------------------------------------------------------------------
	// сохраняем параметры осей
	pugi::xml_node AxisInfo = MainNode.child(L"AxisInfo");
	if (AxisInfo)
	{
		pugi::xml_node CurAxisNode = GraphSettings.child(L"Axis");
		long Index(0);
		do
		{
			if (!CurAxisNode)
				break;
			// вычитываем индекс настраиваемого графика
			GetAttributeFromNode(CurAxisNode, L"number", &Index);
			if (Index >= GridGL::g_Axis_Count)
				continue;
			// заполняем параметрами выбранную ось
			int Val(0);
			if (GetAttributeFromNode(CurAxisNode, L"Side", &Val))
				GridGLParams->AxisInfoStruct[Index].Side = GridGL::AxisSide(Val);
			GetAttributeFromNode(CurAxisNode, L"UseAxis",			&GridGLParams->AxisInfoStruct[Index].bUseAxis);
			GetAttributeFromNode(CurAxisNode, L"Multiplicative",	&GridGLParams->AxisInfoStruct[Index].Multiplicative);
			GetAttributeFromNode(CurAxisNode, L"Additive",			&GridGLParams->AxisInfoStruct[Index].Additive);
			GetAttributeFromNode(CurAxisNode, L"Color",				&GridGLParams->AxisInfoStruct[Index].Color);
			GetAttributeFromNode(CurAxisNode, L"Text",				 GridGLParams->AxisInfoStruct[Index].Text, GridGLParams->MaxTextLengthInByte);

		} while ((CurAxisNode = CurAxisNode.next_sibling()) != nullptr);
	}
	//---------------------------------------------------------------------------------------------
	// сохраняем параметры дополнитльеных окон
	pugi::xml_node AdditionalWindows = MainNode.child(L"AdditionalWindows");
	if (AdditionalWindows)
	{
		pugi::xml_node CurWindowNode = AdditionalWindows.child(L"Window");
		long Index(0);
		do
		{
			if (!CurWindowNode)
				break;
			// вычитываем индекс настраиваемого графика
			GetAttributeFromNode(CurWindowNode, L"number", &Index);
			if (Index >= GridGL::g_Max_Count_AdditionWnd)
				continue;
			// заполняем параметрами выбранное окно
			GetAttributeFromNode(CurWindowNode, L"Visible",			&GridGLParams->AdditionalWindows[Index].bVisible);
			GetAttributeFromNode(CurWindowNode, L"GraphIndex",		&GridGLParams->AdditionalWindows[Index].GraphIndex);
			GetAttributeFromNode(CurWindowNode, L"PointIndex",		&GridGLParams->AdditionalWindows[Index].PointIndex);
			GetAttributeFromNode(CurWindowNode, L"WindowPointX",	&GridGLParams->AdditionalWindows[Index].WindowPoint.x);
			GetAttributeFromNode(CurWindowNode, L"WindowPointY",	&GridGLParams->AdditionalWindows[Index].WindowPoint.y);
			GetAttributeFromNode(CurWindowNode, L"TextColor",		&GridGLParams->AdditionalWindows[Index].TextColor);
			GetAttributeFromNode(CurWindowNode, L"FonColor",		&GridGLParams->AdditionalWindows[Index].FonColor);
			GetAttributeFromNode(CurWindowNode, L"Text",			 GridGLParams->AdditionalWindows[Index].Text, GridGLParams->MaxTextLengthInByte);

		} while ((CurWindowNode = CurWindowNode.next_sibling()) != nullptr);
	}
	//---------------------------------------------------------------------------------------------
	pugi::xml_node CommentsNode = MainNode.child(L"Comments");
	if (CommentsNode)
	{
		pugi::xml_node CurComment = CommentsNode.first_child();
		do
		{
			if (!CurComment)
				break;
			Comments.push_back(CurComment.first_child().value());
		} while ((CurComment = CurComment.next_sibling()) != nullptr);
	}
	//---------------------------------------------------------------------------------------------
	return S_OK;
}

template <class T>
void AddChildToNode(_In_ pugi::xml_node Node, _In_ const CString &Name, _In_ const T &Val)
{
#pragma warning( disable : 4244)	// убираем глупые предупреждегния студии
	try
	{
		std::wstring Value;
		if (std::is_same<T, CString>::value)
			Value = Val;
		else
			Value = boost::lexical_cast<std::wstring>(Val);
		Node.append_child(Name).append_child(pugi::node_pcdata).set_value(Value.c_str());
	}
	catch (boost::bad_lexical_cast)
	{
	}
}

template <class T>
void AddAttributeToChild(_In_ pugi::xml_node Node, _In_ const CString &Name, _In_ const T &Val)
{
	try
	{
		Node.append_attribute(Name).set_value(boost::lexical_cast<std::wstring>(Val).c_str());
	}
	catch (boost::bad_lexical_cast)
	{
	}
}

template <class T>
bool GetAttributeFromNode(_In_ pugi::xml_node Node, _In_ const CString &Name, _In_ T *Val, _In_opt_ const size_t &TextLength /*= 0*/)
{
#pragma warning( disable : 4800)	// убираем глупые предупреждегния студии

	bool bRes(Val != nullptr);
	pugi::xml_attribute Atr = Node.attribute(Name);
	bRes &= (Atr != nullptr);
	if (bRes)
	{
		if (std::is_same<T, bool>::value)
			*Val = (T)Atr.as_bool();
		else if (std::is_same<T, int>::value)
			*Val = (T)Atr.as_int();
		else if (std::is_same<T, unsigned int>::value)
			*Val = (T)Atr.as_uint();
		else if (std::is_same<T, long>::value || 
				 std::is_same<T, long long>::value)
			*Val = (T)Atr.as_llong();
		else if (std::is_same<T, unsigned long>::value		|| 
				 std::is_same<T, unsigned long long>::value ||
				 std::is_same<T, COLORREF>::value)
			*Val = (T)Atr.as_ullong();
		else if (std::is_same<T, float>::value)
			*Val = (T)Atr.as_float();
		else if (std::is_same<T, double>::value)
			*Val = (T)Atr.as_double();
		else if (std::is_same<T, WCHAR>::value)			
			wcsncpy_s((WCHAR *)Val, TextLength, CString(Atr.as_string()).GetString(), TextLength - 1);
		else if (std::is_same<T, CString>::value)
			*(CString*)Val = (CString)Atr.as_string();
	}

	return bRes;
}

template <class T>
bool GetValueFromNode(_In_ pugi::xml_node Node, _In_ const CString &Name, _In_ T *Val, _In_opt_ const size_t &TextLength /*= 0*/)
{
	bool bRes(Val != nullptr);	
	pugi::xml_node Child = Node.child(Name).first_child();
	bRes &= (Child != nullptr);
	if (bRes)
	{
		CString strVal(_T(""));
		strVal = Child.value();

		if (std::is_same<T, bool>::value)
			*Val = _wtoi(strVal) != 0;
		else if (std::is_same<T, int>::value)
			*Val = _wtoi(strVal);
		else if (std::is_same<T, unsigned int>::value)
			*Val = (T)_wtoi(strVal);
		else if (std::is_same<T, long>::value				|| 
				 std::is_same<T, long long>::value			|| 
				 std::is_same<T, unsigned long>::value		|| 
				 std::is_same<T, unsigned long long>::value ||
				 std::is_same<T, COLORREF>::value)
			*Val = (T)_wtoll(strVal);
		else if (std::is_same<T, float>::value || 
				 std::is_same<T, double>::value)
			*Val = (T)_wtof(strVal);
		else if (std::is_same<T, WCHAR>::value)
			wcsncpy_s((WCHAR *)Val, TextLength, strVal.GetString(), TextLength - 1);
		else if (std::is_same<T, CString>::value)
			*((CString*)Val) = strVal;
	}

	return bRes;
}
