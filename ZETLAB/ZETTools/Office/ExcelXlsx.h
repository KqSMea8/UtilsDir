/*! ����� ��� ������ � ����������� Microsoft Excel 2007 � ����.
������ � ��������� ��������. ������������ ����� � ������ �����.
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

	char* cStr;			//! ��������� �� ������
	char* cNew;			//! ��������� �� ������ � ����� ��������
	long lSize;			//! ������ ������
	long lCount;		//! ���������� ���������� �����
	bool isNumber;		//! ������ �������� ������
} SSharedString;

class CExcelXlsx : public COfficeBase
{
public:
	CExcelXlsx();
	~CExcelXlsx();

	int ReplacePictureFromFile(LPCSTR fileName);				//! ������� ��� ������ ����������� ��� ����������� � ��������� �� ����������� �� �����

	int FillNumbersInSheets();									//! ������� ��� ������ ����� �� ����� � ������ �������

	void OpenDocument();
	void CloseDocument();
	bool ReplaceXmlStrings(LPCTSTR strFind, LPCTSTR strReplace);

protected:
	std::list<LocalFile>::iterator m_strFile;					//! ��������� �� ���� �� ������� �����
	std::list<CentralDir>::iterator m_strDir;					//! ��������� �� �������� ����� �� ������� �����

	std::vector<SSharedString> m_strings;						//! ������ �������������� � ������

	int FindDocument();											//! ������� ��� ������ ��������� ���������

	bool IsNumber(char* pch, long size, double &val);			//! ������� ��� �������� ������ �� ���������� �����
	bool ToDouble(char* pch, long size, double &val);			//! ������� ��� �������������� ������ � �����
	bool ToLong(char* pch, long size, long &val);				//! ������� ��� �������������� ������ � ����� �����	

	void FillSharedStrings();									//! ������� ��� ������ ����� �� ����� �� ������� ����� �� ���������
	void ReplaceNumbersInSheet();								//! ������� ��� ������ �������� ����� �� ������ � ������� 
	void RecalculateStrings();									//! ������� ��� ��������� ���������� ������������ �����
	void SetNewIndexes();										//! ������� ��� ������� ����� ������� ��� �����
	void ReplaceNewIndexes();									//! ������� ��� ������� ����� �������� �����
	void ApplyNewStyle();										//! ������� ��� ������� � ���� ������ ����������� ������
	void RemoveErrors();										//! ������� ��� �������� ����� ������������ ������ �� �����


	pugi::xml_document m_doc;


};

