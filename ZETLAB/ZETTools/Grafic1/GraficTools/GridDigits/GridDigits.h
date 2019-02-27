#pragma once
#include <vector>
//*************************************************************************************************
#define USE_DEFAULT -1
//*************************************************************************************************
class CGridDigits
{
public://******************************************************************************************
	CGridDigits();
	//				��������� ��������  /       �������� ��������  /  ������ ����� ���� � ��������
	CGridDigits(_In_ const double &FirstVal, _In_ const double &LastVal, 
				_In_ const int &WidthInPix, _In_opt_ const int &MaxColumnWidth = USE_DEFAULT);
	virtual ~CGridDigits();
	//*********************************************************************************************
	std::vector<long double> m_DigitsList;			// ������ ��������
	std::vector<long double> m_IntermediateDigits;	// ������ ������������� ��������
	CString m_Format;								// ������ ������� �������� �����
public://******************************************************************************************	
	//				��������� ��������  /       �������� �������� 
	void CalcDigits(_In_ double FirstVal, _In_ double LastVal, 
	//				������ ����� ���� � �������� / ������������ ������ �������
					_In_ int WidthInPix, _In_opt_ const int &MaxColumnWidth = USE_DEFAULT);
protected://***************************************************************************************
};	//*********************************************************************************************