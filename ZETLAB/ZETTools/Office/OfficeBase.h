/*! ����� ��� ������ � ����������� Microsoft Office 2007 � ����.
������ � ���������� ��������. ������������ ����� ����� � ������ �����.
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

	//virtual int ReplacePictureFromFile(LPCSTR fileName);				//! ������� ��� ������ ����������� ��� ����������� � ��������� �� ����������� �� �����

	int GetDocumentSize();
	LPSTR GetString(int position, int length);

protected:
	std::list<LocalFile>::iterator m_iterFile;
	std::list<CentralDir>::iterator m_iterDir;

	virtual int FindDocument() { return 0; };								//! ������� ��� ������ ��������� ���������
	//virtual int FindDocument(int num) { return 0; };						//! ������� ��� ������ ��������� �� ������
	int FindFile(LPCSTR FileName);											//! ������� ��� ������ ������������ ���������

	int ReplaceChars(char* src, long srcsize, char* dest, long destsize);	//! ������� ��� ������ ������ �������� � �������� �����
	int DeleteChars(char* src, long srcsize);								//! ������� ��� �������� ������ �������� � �������� �����

	char* getUtf8String(LPCTSTR str);
};

