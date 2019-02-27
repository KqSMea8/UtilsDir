#include "stdafx.h"
#include "ExcelXlsx.h"

#include <string>


CExcelXlsx::CExcelXlsx()
{
}

CExcelXlsx::~CExcelXlsx()
{
	if (!m_strings.empty())
	{
		std::vector<SSharedString>::iterator siter;
		for (siter = m_strings.begin(); siter != m_strings.end(); ++siter)
		{
			if (siter->cStr != NULL)
			{ 
				delete siter->cStr;
				siter->cStr = nullptr;
			}
			if (siter->cNew != NULL)
			{ 
				delete siter->cNew;
				siter->cNew = nullptr;
			}
		}
		m_strings.clear();
	}	
}

int CExcelXlsx::FindDocument()
{
	char doc[21] = "xl/sharedStrings.xml";

	std::list<LocalFile>::iterator itFile;
	std::list<CentralDir>::iterator itDir;

	for ( itFile = m_lFiles.begin(), itDir = m_lDirs.begin(); itFile != m_lFiles.end(), itDir != m_lDirs.end(); ++itFile, ++itDir )
	{
		int ret = strcmp(itFile->pFileName, doc);
		if ( ret == 0 )
		{
			m_iterFile = itFile;
			m_iterDir = itDir;
			return 0;
		}
	}

	return -1;
}

int CExcelXlsx::ReplacePictureFromFile(LPCSTR fileName)
{
	if (strlen((char*)fileName) == 0)
		return -1;

	// ищем имя файла изображения в файле "\xl\drawings\_rels\drawing1.xml.rels" 
	char drawing[36] = "xl/drawings/_rels/drawing1.xml.rels";
	FindFile(drawing);

	char* cfirst = (char*)m_iterFile->pUncmData;
	char* cpos = cfirst;
	char* cend = cpos;
	char* ctemp = NULL;
	char* image1 = NULL;
	int sizei1 = 0;
	while (cpos != NULL && ULONG(cpos - cfirst) < m_iterFile->zhead.uncompressed_size)
	{
		cpos = strstr(cpos, "Type=");
		if (cpos == NULL)
			return -1;
		cpos += 6;
		cend = strstr(cpos, "\"");
		ctemp = strstr(cpos, "relationships/image");
		if (ctemp != NULL && ctemp > cpos && ctemp < cend)
			break;
	}
	if (cpos == NULL)
		return -2;
	cpos = strstr(cpos, "Target=");
	if (cpos == NULL)
		return -1;
	cend = strstr(cpos, "\"/>");
	sizei1 = cend - cpos - 8;
	if (sizei1 < 1) 
		return -3;
	image1 = new char[sizei1+1];
	strncpy_s(image1, sizei1+1, cpos+8, sizei1);
	image1[0] = 'x';
	image1[1] = 'l';

	// ищем имя файла изображения и его типа в файле "\[Content_Types].xml" 
	char content[20] = "[Content_Types].xml";
	FindFile(content);

	char *cimg = NULL;
	char *ccnt = NULL;
	char *cend2 = NULL;
	char *image2 = NULL;
	int sizei2 = 0;
	cimg = strstr((char*)m_iterFile->pUncmData, image1);
	if (cimg != NULL)
	{
		ccnt = strstr(cimg, "ContentType=") - 1;
		cend2 = strstr(ccnt, "\"/");
		sizei2 = cend2 - ccnt - 14;
		image2 = new char[sizei2 + 1];
		strncpy_s(image2, sizei2 + 1, ccnt + 14, sizei2);
	}

	// задаём новые строки для файлов
	char *cext = NULL;
	char *cext1 = NULL;
	char *cext2 = NULL;
	char *reimage1 = NULL;
	char *reimage2 = NULL;
	int extsize = 0;
	int resize1 = 0;
	int resize2 = 0;
	cext = strstr((char*)fileName, ".") + 1;
	extsize = strlen(cext);
	cext1 = strstr(image1, ".") + 1;
	resize1 = cext1 - image1 + strlen(cext) + 1;
	reimage1 = new char[resize1];
	strncpy_s(reimage1, resize1, image1, cext1 - image1);
	strncpy_s(&reimage1[cext1 - image1], strlen(cext) + 1, cext, strlen(cext));
	if (image2 != NULL)
	{
		cext2 = strstr(image2, "/") + 1;
		resize2 = cext2 - image2 + strlen(cext) + 1;
		reimage2 = new char[resize2];
		strncpy_s(reimage2, resize2, image2, cext2 - image2);
		strncpy_s(&reimage2[cext2 - image2], strlen(cext) + 1, cext, strlen(cext));
	}

	// замена файла "\xl\media\image2.emf", конкретно имя можно узнать из других документов
	FindFile(image1);

	// читаем файл изображения в битовый буффер
	FILE* fid(nullptr);
	errno_t err = fopen_s(&fid, fileName, "rb");
	if (err != 0)
		return -4;
	fseek(fid, 0, SEEK_END);
	unsigned int filesize = ftell(fid);
	fseek(fid, 0, SEEK_SET);
	Byte *pBuffer = new Byte[filesize];
	unsigned int readsize = fread(pBuffer, 1, filesize, fid);
	fclose(fid);
	fid = NULL;

	// заменяем данные в файле изображения, изображения не сжимаются
	delete[] m_iterFile->pUncmData;
	m_iterFile->pUncmData = nullptr;
	m_iterFile->pUncmData = pBuffer;
	if (m_iterFile->zdesc.uncompressed_size > 0)
		m_iterFile->zdesc.uncompressed_size = filesize;
	if (m_iterFile->zhead.uncompressed_size > 0)
		m_iterFile->zhead.uncompressed_size = filesize;
	m_iterDir->cdHead.uncompressed_size = filesize;

	delete [] m_iterFile->pFileName;
	m_iterFile->pFileName = nullptr;
	m_iterFile->pFileName = new char[resize1];
	strncpy_s(m_iterFile->pFileName, resize1, reimage1, resize1 - 1);
	m_iterFile->zhead.file_name_length = resize1 - 1;

	delete[] m_iterDir->pFileName;
	m_iterDir->pFileName = nullptr;
	m_iterDir->pFileName = new char[resize1];
	strncpy_s(m_iterDir->pFileName, resize1, reimage1, resize1 - 1);
	m_iterDir->cdHead.file_name_length = resize1 - 1;

	// замена ссылки на файл изображения в файле "\xl\drawings\_rels\drawing1.xml.rels" 
	FindFile(drawing);
	ReplaceString(image1+2, reimage1+2);

	// замена ссылки и типа файла изображения в файле "\[Content_Types].xml" 
	FindFile(content);
	int repos = 0;
	repos = ReplaceString(image1, reimage1);
	if (image2 != NULL)
		repos = ReplaceString(image2, reimage2);

	return 0;
}

int CExcelXlsx::FillNumbersInSheets()
{
	// заполняем локальную таблицу строк
	FillSharedStrings();

	// перебираем листы таблицы
	long num = 0;
	while (true)
	{
		num++;
		if (num >= 100)
			break;

		char* name = new char[27];
		ZeroMemory(name, 27);
		memcpy_s(name, 19, "xl/worksheets/sheet", 19);
		_ltoa_s(num, name+19, 2, 10);
		strcat_s(name, 27, ".xml");

		// если такого файла нет, то выходим из цикла
		if (FindFile(name) < 0)
			break;

		ReplaceNumbersInSheet();
	}

	// исправляем количество использованных строк в файле
	RecalculateStrings();

	// пересчитываем индексы незаменённых строк
	SetNewIndexes();

	// перебираем листы и заменяем старые индексы на новые
	num = 0;
	while (true)
	{
		num++;
		if (num >= 100)
			break;

		char* name = new char[27];
		ZeroMemory(name, 27);
		memcpy_s(name, 19, "xl/worksheets/sheet", 19);
		_ltoa_s(num, name + 19, 2, 10);
		strcat_s(name, 27, ".xml");

		// если такого файла нет, то выходим из цикла
		if (FindFile(name) < 0)
			break;

		ReplaceNewIndexes();

		RemoveErrors();
	}

	return 0;
}

bool CExcelXlsx::ToDouble(char* pch, long size, double &val)
{
	char* buffer = new char[size + 1];
	ZeroMemory(buffer, size + 1);
	memcpy_s(buffer, size, pch, size);
	char* dt = strstr(buffer, ",");
	if (dt != NULL)
		dt[0] = '.';
	_locale_t loc = _get_current_locale();
	char* cend = buffer + size;
	val = _strtod_l(buffer, &cend, loc);
	bool ret = false;
	if (cend == buffer + size)
		ret = true;
	delete []buffer;
	buffer = nullptr;
	return ret;
}

bool CExcelXlsx::ToLong(char* pch, long size, long &val)
{
	char* buffer = new char[size + 1];
	ZeroMemory(buffer, size + 1);
	memcpy_s(buffer, size, pch, size);
	_locale_t loc = _get_current_locale();
	char* cend = buffer + size;
	val = _strtol_l(buffer, &cend, 10, loc);
	bool ret = false;
	if (cend == buffer + size)
		ret = true;
	delete []buffer;
	buffer = nullptr;
	return ret;
}

bool CExcelXlsx::IsNumber(char* pch, long size, double &val)
{
	bool sign = false;
	bool bdot = false;
	bool bexp = false;
	for (long i = 0; i < size; i++)
	{
		// если очередной знак не цифра
		if (pch[i] < '0' || pch[i] > '9')
		{
			// знак
			if (pch[i] == '-' || pch[i] == '+')
				if (i == 0) // в начале числа игнорируем
					continue;
				else		// в середине проверяем, чтобы он следовал за знаком экспоненты
				{

				}
			// если встеритили знак экспоненты нужно проверить на повторение
			if (pch[i] == 'e' || pch[i] == 'E')
				if (bexp)
					return false;
				else
				{
					bexp = true;
					continue;
				}
			// знак дробной части не должен встречаться более одного раза
			if (pch[i] == '.' || pch[i] == ',')
				if (bdot)
					return false;
				else
				{
					bdot = true;
					continue;
				}

			return false;
		}


	}
	return true;
}

void CExcelXlsx::FillSharedStrings()
{
	// перемещаем указатель на список строк
	FindDocument();

	// разбираем по порядку все строки в списке
	char* cfirst = (char*)m_iterFile->pUncmData;		// начало файла
	char* cpos = cfirst;
	long count = 0;		// индекс проверяемой строки в файле со списком строк
	double val;
	cpos = strstr(cpos, "<si>");
	while (cpos != NULL && ULONG(cpos - cfirst) < m_iterFile->zhead.uncompressed_size)
	{
		cpos = strstr(cpos+6, ">") + 1;
		if (cpos == NULL || ULONG(cpos - cfirst) > m_iterFile->zhead.uncompressed_size)
			break;
		char* cend = strstr(cpos, "</t></si>");

		m_strings.push_back(SSharedString());
		m_strings.back().lSize = cend - cpos;
		if (m_strings.back().lSize > 0)
		{
			m_strings.back().cStr = new char[m_strings.back().lSize + 1];
			ZeroMemory(m_strings.back().cStr, m_strings.back().lSize + 1);
			memcpy_s(m_strings.back().cStr, m_strings.back().lSize, cpos, m_strings.back().lSize);
			char* dt = strstr(m_strings.back().cStr, ",");
			if (dt != NULL)
				dt[0] = '.';

			// выбираем строки, которые являются числами
			if (ToDouble(cpos, cend - cpos, val))
				m_strings.back().isNumber = true;
			else
				m_strings.back().isNumber = false;
		}
		else
		{
			m_strings.pop_back();
		}

		cpos = strstr(cpos, "<si>");
	}
}

void CExcelXlsx::ReplaceNumbersInSheet()
{
	char* cfirst = (char*)m_iterFile->pUncmData;		// начало файла
	char* cpos = cfirst;
	char* cend;
	char* ctstr;		// указатель на тип ячейки, определённой как текстовая
	char* cval;			// указатель на значение, записанное в ячейке
	long index;

	cpos = strstr(cpos, "<sheetData>");
	while (cpos != NULL && ULONG(cpos - cfirst) < m_iterFile->zhead.uncompressed_size)
	{
		cpos = strstr(cpos, "<c ");
		if (cpos == NULL || ULONG(cpos - cfirst) > m_iterFile->zhead.uncompressed_size)
			break;
		ctstr = strstr(cpos, "t=\"s\"");
		cval = strstr(cpos, "<v>");
		cend = strstr(cpos, "</v></c>");

		// если ячейка обладает свойством, обозначающим её как текстовую
		if (ctstr > cpos && ctstr < cval && ctstr < cend && cval < cend)
		{
			cval += 3;
			if (ToLong(cval, cend - cval, index))
			{
				if (index < (long)m_strings.size())
				if (m_strings[index].isNumber)
				{
					long pos;
					pos = DeleteChars(ctstr-1, 6);

					cfirst = (char*)m_iterFile->pUncmData;
					cpos = cfirst + pos;
					cval = strstr(cpos, "<v>");
					cend = strstr(cpos, "</v></c>");
					cval += 3;
					pos = ReplaceChars(cval, cend - cval, m_strings[index].cStr, m_strings[index].lSize);

					cfirst = (char*)m_iterFile->pUncmData;
					cpos = cfirst + pos;
					cend = strstr(cpos, "</v></c>");

					m_strings[index].lCount++;
				}
			}
		}

		if (cend == NULL)
			break;

		cpos = strstr(cend, "<c ");
	}
}

void CExcelXlsx::RecalculateStrings()
{
	// перемещаем указатель на список строк
	FindDocument();

	// разбираем по порядку все строки в списке
	char* cfirst = (char*)m_iterFile->pUncmData;		// начало файла
	char* cpos = cfirst;
	char* cend;
	long count = 0;
	long uniqe = 0;
	long pos;

	cpos = strstr(cpos, "count=\"");
	if (cpos == NULL)
		return;
	cpos += 7;
	cend = strstr(cpos, "\"");
	
	if (!ToLong(cpos, cend - cpos, count))
		return;

	cpos = strstr(cpos, "uniqueCount=\"");
	if (cpos == NULL)
		return;
	cpos += 13;
	cend = strstr(cpos, "\"");

	if (!ToLong(cpos, cend - cpos, uniqe))
		return;

	for (size_t i = 0; i < m_strings.size(); i++)
	{
		if (m_strings[i].lCount > 0)
		{
			uniqe--;
			count -= m_strings[i].lCount;
		}
	}

	char ccount[16];
	char cuniqe[16];
	ZeroMemory(ccount, 16);
	ZeroMemory(cuniqe, 16);
	_ltoa_s(count, ccount, 16, 10);
	_ltoa_s(uniqe, cuniqe, 16, 10);

	cpos = cfirst;
	cpos = strstr(cpos, "count=\"");
	if (cpos == NULL)
		return;
	cpos += 7;
	cend = strstr(cpos, "\"");

	pos = ReplaceChars(cpos, cend - cpos, ccount, strlen(ccount));

	cfirst = (char*)m_iterFile->pUncmData;
	cpos = cfirst + pos;
	cpos = strstr(cpos, "uniqueCount=\"");
	if (cpos == NULL)
		return;;
	cpos += 13;
	cend = strstr(cpos, "\"");

	pos = ReplaceChars(cpos, cend - cpos, cuniqe, strlen(cuniqe));

}

void CExcelXlsx::SetNewIndexes()
{
	long count = 0;
	size_t i;
	for (i = 0; i < m_strings.size(); i++)
	{
		// если строку не разу не заменили, то она должна поменять индекс
		if (m_strings[i].lCount == 0)
		{
			m_strings[i].cNew = new char[16];
			ZeroMemory(m_strings[i].cNew, 16);
			_itoa_s(count, m_strings[i].cNew, 16, 10);
			count++;
		}
	}

	// удаляем ненужные строки
	FindDocument();

	// разбираем по порядку все строки в списке
	char* cfirst = (char*)m_iterFile->pUncmData;		// начало файла
	char* cpos = cfirst;
	char* cend = cfirst;
	bool bDel = false;

	i = 0;						// индекс проверяемой строки в файле со списком строк
	while (cpos != NULL && cend != NULL)
	{
		if (!bDel)
		{
			cpos = strstr(cend, "<si>");
			cend = strstr(cpos, "</si>");
		}
		else
			bDel = false;

		if (cpos == NULL || cend == NULL || 
			ULONG(cpos - cfirst) > m_iterFile->zhead.uncompressed_size || 
			ULONG(cpos - cfirst) > m_iterFile->zhead.uncompressed_size)
			break;

		if (m_strings[i].lCount > 0)
		{
			long pos = DeleteChars(cpos, cend - cpos + 5);
			cfirst = (char*)m_iterFile->pUncmData;
			cpos = cfirst + pos;
			cend = strstr(cpos, "</si>");
			bDel = true;
		}
		i++;

		if (i == m_strings.size())
			break;
	}
}

void CExcelXlsx::ReplaceNewIndexes()
{
	char* cfirst = (char*)m_iterFile->pUncmData;		// начало файла
	char* cpos = cfirst;
	char* cend;
	char* ctstr;		// указатель на тип ячейки, определённой как текстовая
	char* cval;			// указатель на значение, записанное в ячейке
	long index;

	cpos = strstr(cpos, "<sheetData>");
	while (cpos != NULL && ULONG(cpos - cfirst) < m_iterFile->zhead.uncompressed_size)
	{
		cpos = strstr(cpos, "<c ");
		if (cpos == NULL || ULONG(cpos - cfirst) > m_iterFile->zhead.uncompressed_size)
			break;
		ctstr = strstr(cpos, "t=\"s\"");
		cval = strstr(cpos, "<v>");
		cend = strstr(cpos, "</v></c>");

		// если ячейка обладает свойством, обозначающим её как текстовую
		if (ctstr > cpos && ctstr < cval && ctstr < cend && cval < cend)
		{
			cval += 3;
			if (ToLong(cval, cend - cval, index))
			{
				if (index < (long)m_strings.size())
				{
					if (m_strings[index].lCount == 0)
					{
						long pos;
						pos = ReplaceChars(cval, cend - cval, m_strings[index].cNew, strlen(m_strings[index].cNew));

						cfirst = (char*)m_iterFile->pUncmData;
						cpos = cfirst + pos;
						cend = strstr(cpos, "</v></c>");
					}
				}
			}
		}

		if (cend == NULL)
			break;

		cpos = strstr(cend, "<c ");
	}
}

void CExcelXlsx::ApplyNewStyle()
{
	char sStyle[14] = "xl/styles.xml";
	FindFile(sStyle);

	char* cpos = (char*)m_iterFile->pUncmData;
	char* cend = cpos;

	cpos = strstr(cpos, "");
}

void CExcelXlsx::RemoveErrors()
{
	char* cfirst = (char*)m_iterFile->pUncmData;		// начало файла
	char* cpos = cfirst;
	char* cend;
	char* cerr;			// указатель на тип ячейки, определённой как ошибочная
	char* cvend;

	cpos = strstr(cpos, "<sheetData>");
	while (cpos != NULL && ULONG(cpos - cfirst) < m_iterFile->zhead.uncompressed_size)
	{
		cpos = strstr(cpos, "<c ");
		if (cpos == NULL || ULONG(cpos - cfirst) > m_iterFile->zhead.uncompressed_size)
			return;
		cerr = strstr(cpos, "t=\"e\"");
		cend = strstr(cpos, "</c>");

		if (cerr == NULL || cend == NULL ||
			ULONG(cpos - cfirst) > m_iterFile->zhead.uncompressed_size ||
			ULONG(cpos - cfirst) > m_iterFile->zhead.uncompressed_size)
			break;

		// если ячейка содержит тэг обозначающий ошибку, то тэг нужно удалить
		if (cerr > cpos && cend > cerr)
		{
			long pos = DeleteChars(cerr - 1, 6);

			cfirst = (char*)m_iterFile->pUncmData;
			cpos = cfirst + pos;
			cend = strstr(cpos, "</c>");

			cerr = strstr(cpos, "<v>");
			cvend = strstr(cpos, "</v>");

			if (cerr == NULL || cvend == NULL)
				break;

			if (cvend < cend)
			{
				long pos = DeleteChars(cerr - 1, cvend - cerr + 4);

				cfirst = (char*)m_iterFile->pUncmData;
				cpos = cfirst + pos;
				cend = strstr(cpos, "</c>");
			}
		}

		if (cend == NULL)
			break;

		cpos = strstr(cend, "<c ");
	}
}


void CExcelXlsx::OpenDocument()
{
	m_doc.load_buffer(m_iterFile->pUncmData, m_iterFile->zhead.uncompressed_size);
}

void CExcelXlsx::CloseDocument()
{
	//pugi::xml_memory_writer wrt;
	//m_doc.save(wrt);

	if (m_iterFile->pUncmData)
	{ 
		delete[] m_iterFile->pUncmData;
		m_iterFile->pUncmData = nullptr;
	}
	m_iterFile->pUncmData = new Byte[m_iterFile->zdesc.uncompressed_size];

	//wrt.write(m_iterFile->pUncmData, m_iterFile->zdesc.uncompressed_size);

	m_doc.reset();
}

bool CExcelXlsx::ReplaceXmlStrings(LPCTSTR strFind, LPCTSTR strReplace)
{
	pugi::xml_node node = m_doc.first_child();
	pugi::xml_node child;
	pugi::xml_node text;
	pugi::xml_text txt;
	if (wcscmp(node.name(), L"sst") == 0)
	{
		child = node.first_child();
		while (child != nullptr)
		{
			text = child.first_child();
			txt = text.text();
			if (wcscmp(txt.as_string(), strFind) == 0)
			{
				m_iterFile->zdesc.uncompressed_size += wcslen(strReplace) - wcslen(txt.as_string());
				if (m_iterFile->zdesc.signature != 0x08074B50)
					m_iterFile->zhead.uncompressed_size = m_iterFile->zdesc.uncompressed_size;
				txt.set(strReplace);
				text.set_value(strReplace);
			}
			child = child.next_sibling();
		}
	}
	return false;
}