/*! ����� ��� ������ � ����������� Microsoft Word 2007 � ����.
������ � ���������� ��������. ������������ ����� ����� � ������ �����.
*/
#pragma once
#include "OfficeBase.h"

class CWordDocx : public COfficeBase
{
public:
	CWordDocx();
	~CWordDocx();

	int ReplacePictureFromFile(LPCSTR fileName);				//! ������� ��� ������ ����������� ��� ����������� � ��������� �� ����������� �� �����

protected:
	int FindDocument();											//! ������� ��� ������ ��������� ���������
};

