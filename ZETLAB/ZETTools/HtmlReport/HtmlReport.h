//--------------------------------------------------------------------------------
// HTMLREPORT - класс для сохранения любых текстовых отчётов в формате HTML
// автор - Бегишев Сергей 2014 год.
//--------------------------------------------------------------------------------
#pragma once

#include <list>

typedef enum HorAlign
{
	haDefault = 0,		//	выравнивание по умолчанию
	haLeft = 1,			//	выравнивание по левому краю
	haCenter = 2,		//	выравнивание по центру
	haRight = 3,		//	выравнивание по правому краю
	haJustify = 4		//	выравнивание по ширине (поддерживается не всеми браузерами)
};

typedef enum VertAlign
{
	vaDefault = 0,		//	выравнивание по умолчанию
	vaTop = 1,			//	выравнивание по верхнему краю
	vaMiddle = 2,		//	выравнивание по середине
	vaBottom = 3		//	выравнивание по нижнему краю
};

typedef enum TextBlockSize
{
	tbsDefault = 0,		//	
	tbsBig = 1,			//	большой заголовок
	tbsMedium = 2,		//	средний заголовок
	tbsNormal = 3,		//	нормальный заголовок
	tbsSmall = 4		//	маленький заголовок
};

typedef enum TableRowType
{
	trtDefault = 0,		//
	trtHead = 1,		//	заголовок таблицы
	trtBody = 2,		//	тело таблицы
	trtFoot = 3			//	окончание таблицы
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

	//	очистить все строки в списке
	void Clear();	

	//	задать заголовок гиппертекстового файла
	void SetTitle(std::wstring title);

	//	открыть запись в таблицу
	void SetTable(std::wstring caption, HorAlign halign = haDefault, VertAlign valign = vaDefault, int border = -1, int width = -1);	/*
	CString caption -- название таблицы, указывается по умолчанию перед таблицей
	HorAlign halign -- выравнивание таблицы по горизонтали
	VertAlign valign - выравнивание таблицы по вертикали
	int border ------- ширина границы между ячейками, по умолчанию границы не рисуются
	int width -------- ширина всей таблицы в пикселях, по умолчанию определяется шириной содержимого
	После того, как все строки будут записаны в таблицу, необходимо обозначить конец таблицы коммандой CloseTable()*/

	//	закрыть запись в таблицу
	void CloseTable();

	//	создать заголовок для текста
	void AddTextHeadline(std::wstring text, TextBlockSize size = tbsDefault, HorAlign halign = haDefault); /*
	CString text ------- текст заголовка
	TextBlockSize size - размер заголовка, по умолчанию очень маленький
	HorAlign halign ---- выравнивание заголовка по горизонтали	*/

	//	создать текстовый блок
	void AddTextBlock(std::wstring text, HorAlign halign = haDefault); /*
	CString text ---- текст, знаки переноса автоматически заменяются на теги переноса строки
	HorAlign halign - выравнивание текста по горизонтали*/

	//	добавить тег переноса строки
	void AddBreak(int number = 0); /*
	int number - количество переносов*/

	//	добавить ряд в таблицу
	void AddTableRow(HorAlign halign = haDefault, VertAlign valign = vaDefault, TableRowType rowtype = trtDefault); /*
	HorAlign halign -- выравнивание по горизонтали, применяется ко всем ячейкам
	VertAlign valign - выравнивание по вертикали, применяется ко всем ячейкам
	После добавления всех ячеек в таблицу необходимо завершить рад командой EndTableRow()*/

	//	завершить ряд в таблице
	void EndTableRow(TableRowType rowtype = trtDefault);

	//	добавить в таблицу ячейку с загловком
	void AddTableHeaderCell(std::wstring text, HorAlign halign = haDefault, VertAlign valign = vaDefault, int colspan = 0, int rowspan = 0, int width = -1, int height = -1); /*
	CString text ----- текст заголовка
	HorAlign halign -- выравнивание текста в ячейке по горизонтали
	VertAlign valign - врывнивание текста в ячейке по вертикали
	int colspan ------ объединить следующие в строке ячейки в одну, по умолчанию все ячейки имеют единичную длину
	int rowspan ------ объединить следующие в столбце ячейки в одну, по умолчанию все ячейки имеют единичную высоту
	int width -------- ширина ячейки в пикселях, ширина всего столбца определяется максимальным значением
	int height ------- высота ячейки в пикселях, высота всей строки определяется максимальным значением
	Заголовочные ячейки в некторых браузерах могут иметь свой стиль, отличный от ячеек данных*/

	//	добавить в таблицу ячейку с данными
	void AddTableDataCell(std::wstring text, HorAlign halign = haDefault, VertAlign valign = vaDefault, int colspan = 0, int rowspan = 0, int width = -1, int height = -1); /*
	CString text ----- текст с анными
	HorAlign halign -- выравнивание текста в ячейке по горизонтали
	VertAlign valign - врывнивание текста в ячейке по вертикали
	int colspan ------ объединить следующие в строке ячейки в одну, по умолчанию все ячейки имеют единичную длину
	int rowspan ------ объединить следующие в столбце ячейки в одну, по умолчанию все ячейки имеют единичную высоту
	int width -------- ширина ячейки в пикселях, ширина всего столбца определяется максимальным значением
	int height ------- высота ячейки в пикселях, высота всей строки определяется максимальным значением
	Ячейки с данными в некторых браузерах могут иметь свой стиль, отличный от ячеек с заголовком*/

	//	добавить изображение 
	void AddImage(std::wstring url);

	//	сохранить данные в файл
	int SaveFile(std::wstring fileName);
};

