#include "HtmlReport.h"

CHtmlReport::CHtmlReport(void)
{
}

CHtmlReport::~CHtmlReport(void)
{
}

void CHtmlReport::Clear()
{
	if (!m_data.empty())
		m_data.clear();
}

void CHtmlReport::AddHorisontalAlign(std::wstring &command, HorAlign halign)
{
	switch (halign)
	{
		case haLeft		: command.append(L" ALIGN=\"left\""); break;
		case haCenter	: command.append(L" ALIGN=\"center\""); break;
		case haRight	: command.append(L" ALIGN=\"right\""); break;
		case haJustify	: command.append(L" ALIGN=\"justify\""); break;
	}
}

void CHtmlReport::AddVerticalAlign(std::wstring &command, VertAlign valign)
{
	switch (valign)
	{
		case vaTop		: command.append(L" VALIGN=\"top\""); break;
		case vaMiddle	: command.append(L" VALIGN=\"middle\""); break;
		case vaBottom	: command.append(L" VALIGN=\"bottom\""); break;
	}
}

void CHtmlReport::SetTitle(std::wstring title)
{
	m_data.push_back(L"<HEAD>\n");
	m_data.push_back(L"\t<title>" + title + L"</title>\n");
	m_data.push_back(L"</HEAD>\n");
}

void CHtmlReport::AddTextHeadline(std::wstring text, TextBlockSize size /* = tbsDefault */, HorAlign halign/* = haDefault */)
{
	std::wstring tmpStr;
	switch (size)
	{
		case tbsBig		: tmpStr.append(L"<H1"); break;
		case tbsMedium	: tmpStr.append(L"<H2"); break;
		case tbsNormal	: tmpStr.append(L"<H3"); break;
		case tbsSmall	: tmpStr.append(L"<H4"); break;
		default			: tmpStr.append(L"<H5"); break;
	}
	AddHorisontalAlign(tmpStr, halign);
	tmpStr.append(L">");
	tmpStr.append(text);
	switch (size)
	{
		case tbsBig		: tmpStr.append(L"</H1>"); break;
		case tbsMedium	: tmpStr.append(L"</H2>"); break;
		case tbsNormal	: tmpStr.append(L"</H3>"); break;
		case tbsSmall	: tmpStr.append(L"</H4>"); break;
		default			: tmpStr.append(L"</H5>"); break;
	}
	tmpStr.append(L"\n");
	m_data.push_back(tmpStr);
}

void CHtmlReport::AddTextBlock(std::wstring text, HorAlign halign /* = haDefault */)
{
	std::wstring tmpStr;
	tmpStr.append(L"<P>");
	AddHorisontalAlign(tmpStr, halign);
	std::wstring::size_type num;
	while ( (num = text.find(L"\n")) != std::wstring::npos && text.length() > 0)
	{
		tmpStr.append(text.substr(0, num));
		tmpStr.append(L"<BR>");
		text = text.substr(num + 1);
	}
	if (text.length() > 0)
		tmpStr.append(text);
	tmpStr.append(L"</P>\n");
	m_data.push_back(tmpStr);
}

void CHtmlReport::AddBreak(int number /* = 0*/)
{
	if (number > 0)
	{
		std::wstring tmpStr;
		for (int i = 0; i < number; i++)
			tmpStr.append(L"<BR>");
		tmpStr.append(L"\n");
		m_data.push_back(tmpStr);
	}
	else
		m_data.push_back(L"<BR>\n");
}

void CHtmlReport::SetTable(std::wstring caption, HorAlign halign /* = haDefault */, VertAlign valign /* = vaDefault */, int border /* = -1 */, int width /* = -1 */)
{
	std::wstring tmpStr;
	wchar_t sTemp[64];
	tmpStr.append(L"<TABLE");
	AddHorisontalAlign(tmpStr, halign);
	AddVerticalAlign(tmpStr, valign);
	if (border >= 0)
	{
		swprintf_s(sTemp, L" BORDER=%d", border);
		tmpStr.append(sTemp);
	}
	if (width >= 0)
	{
		swprintf_s(sTemp, L" WIDTH=%d%%", width);
		tmpStr.append(sTemp);
	}
	tmpStr.append(L">\n");
	m_data.push_back(tmpStr);
	if (caption.length() > 0)
		m_data.push_back(L"<CAPTION>" + caption + L"</CAPTION>\n");
}

void CHtmlReport::CloseTable()
{
	m_data.push_back(L"</TABLE>\n");
}

void CHtmlReport::AddTableRow(HorAlign halign /* = haDefault */, VertAlign valign /* = vaDefault */, TableRowType rowtype /* = trtDefault */)
{
	std::wstring tmpStr;
	switch (rowtype)
	{
	case trtDefault:		break;
	case trtHead:			tmpStr.append(L"\t<THEAD>\n");	break;
	case trtBody:			tmpStr.append(L"\t<TBODY>\n");	break;
	case trtFoot:			tmpStr.append(L"\t<TFOOT>\n");	break;
	default:				break;
	}
	tmpStr.append(L"\t<TR");
	AddHorisontalAlign(tmpStr, halign);
	AddVerticalAlign(tmpStr, valign);
	tmpStr.append(L">\n");
	m_data.push_back(tmpStr);
}

void CHtmlReport::EndTableRow(TableRowType rowtype /* = trtDefault */)
{
	std::wstring tmpStr;
	tmpStr.append(L"\t</TR>\n");
	switch (rowtype)
	{
	case trtDefault:		break;
	case trtHead:			tmpStr.append(L"\t</THEAD>\n");	break;
	case trtBody:			tmpStr.append(L"\t</TBODY>\n");	break;
	case trtFoot:			tmpStr.append(L"\t</TFOOT>\n");	break;
	default:				break;
	}
	m_data.push_back(tmpStr);
}

void CHtmlReport::AddCell(std::wstring &command, HorAlign halign /* = haDefault */, VertAlign valign /* = vaDefault */, int colspan /* = 0 */, int rowspan /* = 0 */, int width /* = -1 */, int height /* = -1 */)
{
	wchar_t sTemp[64];
	AddHorisontalAlign(command, halign);
	AddVerticalAlign(command, valign);
	if (colspan > 0)
	{
		swprintf_s(sTemp, L" COLSPAN=%d", colspan);
		command.append(sTemp);
	}
	if (rowspan > 0)
	{
		swprintf_s(sTemp, L" ROWSPAN=%d", rowspan);
		command.append(sTemp);
	}
	if (width >= 0)
	{
		swprintf_s(sTemp, L" WIDTH=%d", width);
		command.append(sTemp);
	}
	if (height >= 0)
	{
		swprintf_s(sTemp, L" HEIGHT=%d", height);
		command.append(sTemp);
	}
	command.append(L">");
}

void CHtmlReport::AddTableDataCell(std::wstring text, HorAlign halign /* = haDefault */, VertAlign valign /* = vaDefault */, int colspan /* = 0 */, int rowspan /* = 0 */, int width /* = -1 */, int height /* = -1 */)
{
	std::wstring tmpStr;
	tmpStr.append(L"\t\t<TD");
	AddCell(tmpStr, halign, valign, colspan, rowspan, width, height);
	tmpStr.append(text);
	tmpStr.append(L"</TD>\n");
	m_data.push_back(tmpStr);
}

void CHtmlReport::AddTableHeaderCell(std::wstring text, HorAlign halign /* = haDefault */, VertAlign valign /* = vaDefault */, int colspan /* = 0 */, int rowspan /* = 0 */, int width /* = -1 */, int height /* = -1 */)
{
	std::wstring tmpStr;
	tmpStr.append(L"\t\t<TH");
	AddCell(tmpStr, halign, valign, colspan, rowspan, width, height);
	tmpStr.append(text);
	tmpStr.append(L"</TH>\n");
	m_data.push_back(tmpStr);
}

void CHtmlReport::AddImage(std::wstring url)
{
	std::wstring tmpStr;
	tmpStr.append(L"<IMG SRC=\"");
	tmpStr.append(url);
	tmpStr.append(L"\">\n");
}

int CHtmlReport::SaveFile(std::wstring fileName)
{
	FILE* pFile(nullptr);

	errno_t error = _wfopen_s(&pFile, fileName.c_str(), L"wt, ccs=UTF-8");
	if (error != 0)
		return error;
	if (pFile == nullptr)
		return -1;

	fwprintf(pFile, L"<HTML>\n");
	for (std::wstring& str : m_data)
	{
		int wnum = fwprintf(pFile, str.c_str());
		if (wnum == -1)
			_get_errno(&error);
	}
	fwprintf(pFile, L"</HTML>\n");
	
	fclose(pFile);
	pFile = NULL;

	return 0;
}