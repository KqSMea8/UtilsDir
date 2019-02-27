/*! ����� ��� ������ � ������ ZIP-������ �������.
����� ������� ��� ������ � ������ DOCX-������. DOCX ������ ������������ ����� ZIP ����� � ������������ DEFLATE.
��� ���������� � ������������� ������������ ���������� "zlib.dll", ������� ����� ����� ������������ � ������ ������:
RFC 1950 (zlib compressed format)
RFC 1951 (deflate compressed format)
RFC 1952 (gzip compressed format)
� ��������� ����� � ������ ����������� ������ ������ �� ������ DEFLATE.
��������� � ������ 1-��������.
2014. ������� ������.
http://zlib.net - ����������� ����
http://zlib.net.ru - ������� �� ������� ����
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

	int ReadZipFromFront();				//! ������ zip-���� �� ������ � ������ �������
	int WriteZipFromFront();			//! ���������� ��������� zip-����� � ������ � ������ �������

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
	
	int CalculateTotalCompressSize();	//! ������� ���������� ������� ������ ������, ������� ��������� ���������
};