/*! Класс для чтения и записи ZIP-файлов архивов.
Класс написан для чтения и записи DOCX-файлов. DOCX формат представляет собой ZIP архив с компрессором DEFLATE.
Для компрессии и дексомпрессии используется библиотека "zlib.dll", которая также умеет использовать и другие методы:
RFC 1950 (zlib compressed format)
RFC 1951 (deflate compressed format)
RFC 1952 (gzip compressed format)
В настоящее время в классе реализовано только сжатие по методу DEFLATE.
Кодировка в файлах 1-байтовая.
2014. Бегишев Сергей.
http://zlib.net - официальный сайт
http://zlib.net.ru - перевод на русский язык
*/

#pragma once
#include "ZipStructures.h"
#include "ZipErrors.h"
#include "zlib.h"
#include <list>
#include <string>

typedef std::wstring tstring;

class CZipFile
{
public:
	CZipFile();
	CZipFile(tstring filename);
	~CZipFile();

	int ReadZip(tstring filename);
	int WriteZip(tstring filename);
	void Clear();

protected:
	std::list<LocalFile> m_lFiles;
	std::list<CentralDir> m_lDirs;
	EndRecord m_End;

private:
	tstring m_sFileName;

	Byte* m_pBuffer;
	unsigned int m_iBufferSize;

	int ReadZipFromFront();				//! читаем zip-файл из буфера в прямом порядке
	int WriteZipFromFront();			//! записываем структуры zip-файла в буффер в прямом порядке

	int ReadBuffer();
	int WriteBuffer();
	void CleanBuffer();

	int ReadLocalFile(unsigned int &position);
	int ReadCentralDirectory(unsigned int &position);
	int ReadEndRecord(unsigned int &position);

	int WriteLocalFile();
	int WriteCentralDirectory();
	int WriteEndRecord();

	int CompressData();
	int UncompressData();
	
	int CalculateTotalCompressSize();	//! подсчёт суммарного размера сжатых данных, включая структуры описатели
};