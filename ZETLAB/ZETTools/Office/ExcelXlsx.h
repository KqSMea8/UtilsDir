/*! Класс для работы с документами Microsoft Excel 2007 и выше.
Читает и сохраняет документ. Осуществляет поиск и замену строк.
*/
#pragma once
//#include "WordDocx.h"
#include "OfficeBase.h"
#include "windows.h"
#include <vector>

#include "pugixml.hpp"

typedef struct _SSharedString
{
	_SSharedString()
	{
		cStr = NULL;
		cNew = NULL;
		lSize = 0;
		lCount = 0;
		isNumber = false;
	};

	char* cStr;			//! указатель на строку
	char* cNew;			//! указатель на строку с новым индексом
	long lSize;			//! размер строки
	long lCount;		//! количество выплненных замен
	bool isNumber;		//! строка является числом
} SSharedString;

class CExcelXlsx : public COfficeBase
{
public:
	CExcelXlsx();
	~CExcelXlsx();

	int ReplacePictureFromFile(LPCSTR fileName);				//! функция для замены изображения или изображения в документе на изображение из файла

	int FillNumbersInSheets();									//! функция для замены строк на числа в листах таблицы

	void OpenDocument();
	void CloseDocument();
	bool ReplaceXmlStrings(LPCTSTR strFind, LPCTSTR strReplace);

protected:
	std::list<LocalFile>::iterator m_strFile;					//! указатель на файл со списком строк
	std::list<CentralDir>::iterator m_strDir;					//! указатель на описание файла со списком строк

	std::vector<SSharedString> m_strings;						//! строки использованные в отчёте

	int FindDocument();											//! функция для поиска основного документа

	bool IsNumber(char* pch, long size, double &val);			//! функция для проверки строки на содержание числа
	bool ToDouble(char* pch, long size, double &val);			//! функция для преобразования строки в число
	bool ToLong(char* pch, long size, long &val);				//! функция для преобразования строки в целое число	

	void FillSharedStrings();									//! функция для чтения строк из файла со списком строк из документа
	void ReplaceNumbersInSheet();								//! функция для замены индексов строк на строки с числами 
	void RecalculateStrings();									//! функция для пересчёта количества используемых строк
	void SetNewIndexes();										//! функция для задания новых номеров для строк
	void ReplaceNewIndexes();									//! функция для задания новых индексов строк
	void ApplyNewStyle();										//! функция для добавки в файл стилей необходимой строки
	void RemoveErrors();										//! функция для удаления тегов обозначающих ошибку из ячеек


	pugi::xml_document m_doc;


};

