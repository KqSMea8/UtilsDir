//--------------------------------------------------------------------------------
// HTMLREPORT - ����� ��� ���������� ����� ��������� ������� � ������� HTML
// ����� - ������� ������ 2014 ���.
//--------------------------------------------------------------------------------
#pragma once

#include <list>

typedef enum HorAlign
{
	haDefault = 0,		//	������������ �� ���������
	haLeft = 1,			//	������������ �� ������ ����
	haCenter = 2,		//	������������ �� ������
	haRight = 3,		//	������������ �� ������� ����
	haJustify = 4		//	������������ �� ������ (�������������� �� ����� ����������)
};

typedef enum VertAlign
{
	vaDefault = 0,		//	������������ �� ���������
	vaTop = 1,			//	������������ �� �������� ����
	vaMiddle = 2,		//	������������ �� ��������
	vaBottom = 3		//	������������ �� ������� ����
};

typedef enum TextBlockSize
{
	tbsDefault = 0,		//	
	tbsBig = 1,			//	������� ���������
	tbsMedium = 2,		//	������� ���������
	tbsNormal = 3,		//	���������� ���������
	tbsSmall = 4		//	��������� ���������
};

typedef enum TableRowType
{
	trtDefault = 0,		//
	trtHead = 1,		//	��������� �������
	trtBody = 2,		//	���� �������
	trtFoot = 3			//	��������� �������
};

class CHtmlReport
{
	std::list<std::wstring> m_data;

protected:
	void AddHorisontalAlign(std::wstring &command, HorAlign align);
	void AddVerticalAlign(std::wstring &command, VertAlign align);
	void AddCell(std::wstring &command, HorAlign halign = haDefault, VertAlign valign = vaDefault, int colspan = 0, int rowspan = 0, int width = -1, int height = -1);

public:
	CHtmlReport(void);
	~CHtmlReport(void);

	//	�������� ��� ������ � ������
	void Clear();	

	//	������ ��������� ���������������� �����
	void SetTitle(std::wstring title);

	//	������� ������ � �������
	void SetTable(std::wstring caption, HorAlign halign = haDefault, VertAlign valign = vaDefault, int border = -1, int width = -1);	/*
	CString caption -- �������� �������, ����������� �� ��������� ����� ��������
	HorAlign halign -- ������������ ������� �� �����������
	VertAlign valign - ������������ ������� �� ���������
	int border ------- ������ ������� ����� ��������, �� ��������� ������� �� ��������
	int width -------- ������ ���� ������� � ��������, �� ��������� ������������ ������� �����������
	����� ����, ��� ��� ������ ����� �������� � �������, ���������� ���������� ����� ������� ��������� CloseTable()*/

	//	������� ������ � �������
	void CloseTable();

	//	������� ��������� ��� ������
	void AddTextHeadline(std::wstring text, TextBlockSize size = tbsDefault, HorAlign halign = haDefault); /*
	CString text ------- ����� ���������
	TextBlockSize size - ������ ���������, �� ��������� ����� ���������
	HorAlign halign ---- ������������ ��������� �� �����������	*/

	//	������� ��������� ����
	void AddTextBlock(std::wstring text, HorAlign halign = haDefault); /*
	CString text ---- �����, ����� �������� ������������� ���������� �� ���� �������� ������
	HorAlign halign - ������������ ������ �� �����������*/

	//	�������� ��� �������� ������
	void AddBreak(int number = 0); /*
	int number - ���������� ���������*/

	//	�������� ��� � �������
	void AddTableRow(HorAlign halign = haDefault, VertAlign valign = vaDefault, TableRowType rowtype = trtDefault); /*
	HorAlign halign -- ������������ �� �����������, ����������� �� ���� �������
	VertAlign valign - ������������ �� ���������, ����������� �� ���� �������
	����� ���������� ���� ����� � ������� ���������� ��������� ��� �������� EndTableRow()*/

	//	��������� ��� � �������
	void EndTableRow(TableRowType rowtype = trtDefault);

	//	�������� � ������� ������ � ���������
	void AddTableHeaderCell(std::wstring text, HorAlign halign = haDefault, VertAlign valign = vaDefault, int colspan = 0, int rowspan = 0, int width = -1, int height = -1); /*
	CString text ----- ����� ���������
	HorAlign halign -- ������������ ������ � ������ �� �����������
	VertAlign valign - ����������� ������ � ������ �� ���������
	int colspan ------ ���������� ��������� � ������ ������ � ����, �� ��������� ��� ������ ����� ��������� �����
	int rowspan ------ ���������� ��������� � ������� ������ � ����, �� ��������� ��� ������ ����� ��������� ������
	int width -------- ������ ������ � ��������, ������ ����� ������� ������������ ������������ ���������
	int height ------- ������ ������ � ��������, ������ ���� ������ ������������ ������������ ���������
	������������ ������ � �������� ��������� ����� ����� ���� �����, �������� �� ����� ������*/

	//	�������� � ������� ������ � �������
	void AddTableDataCell(std::wstring text, HorAlign halign = haDefault, VertAlign valign = vaDefault, int colspan = 0, int rowspan = 0, int width = -1, int height = -1); /*
	CString text ----- ����� � ������
	HorAlign halign -- ������������ ������ � ������ �� �����������
	VertAlign valign - ����������� ������ � ������ �� ���������
	int colspan ------ ���������� ��������� � ������ ������ � ����, �� ��������� ��� ������ ����� ��������� �����
	int rowspan ------ ���������� ��������� � ������� ������ � ����, �� ��������� ��� ������ ����� ��������� ������
	int width -------- ������ ������ � ��������, ������ ����� ������� ������������ ������������ ���������
	int height ------- ������ ������ � ��������, ������ ���� ������ ������������ ������������ ���������
	������ � ������� � �������� ��������� ����� ����� ���� �����, �������� �� ����� � ����������*/

	//	�������� ����������� 
	void AddImage(std::wstring url);

	//	��������� ������ � ����
	int SaveFile(std::wstring fileName);
};

