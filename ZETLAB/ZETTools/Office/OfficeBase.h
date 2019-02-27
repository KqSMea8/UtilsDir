/*! Класс для работы с документами Microsoft Office 2007 и выше.
Читает и записывает документ. Осуществляет поиск строк и замену строк.
*/
#pragma once
#include "Zip/ZipFile.h"
#include "windows.h"

class COfficeBase :	public CZipFile
{
public:
	COfficeBase();
	~COfficeBase();

	int Read(LPCTSTR FileName);
	int Write(LPCTSTR FileName);

	int FindString(LPCTSTR strFind, unsigned int start = 0);
	int FindString(LPCSTR strFind, unsigned int start = 0);
	int ReplaceString(LPCTSTR strFind, LPCTSTR strReplace, unsigned int start = 0);
	int ReplaceString(LPCSTR strFind, LPCSTR strReplace, unsigned int start = 0);

	//virtual int ReplacePictureFromFile(LPCSTR fileName);				//! функция для замены изображения или изображения в документе на изображение из файла

	int GetDocumentSize();
	LPSTR GetString(int position, int length);

protected:
	std::list<LocalFile>::iterator m_iterFile;
	std::list<CentralDir>::iterator m_iterDir;

	virtual int FindDocument() { return 0; };								//! функция для поиска основного документа
	//virtual int FindDocument(int num) { return 0; };						//! функция для поиска документа по номеру
	int FindFile(LPCSTR FileName);											//! функция для поиска определённого документа

	int ReplaceChars(char* src, long srcsize, char* dest, long destsize);	//! функция для замены группы символов в исходном файле
	int DeleteChars(char* src, long srcsize);								//! функция для удаления группы символов в исходном файле

	char* getUtf8String(LPCTSTR str);
};

