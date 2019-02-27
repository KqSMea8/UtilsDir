#include "stdafx.h"
#include "GridDigits.h"
//*************************************************************************************************
#define WIDTH_COLUMNS_IN_PIX	100
//*************************************************************************************************
CGridDigits::CGridDigits(_In_ const double &FirstVal, _In_ const double &LastVal, 
						 _In_ const int &WidthInPix, _In_opt_ const int &MaxColumnWidth /*= USE_DEFAULT*/)
	: m_Format(_T(""))
{
	CalcDigits(FirstVal, LastVal, WidthInPix, MaxColumnWidth);
}
//*************************************************************************************************
CGridDigits::CGridDigits()
	: m_Format(_T(""))
{
}
//*************************************************************************************************
CGridDigits::~CGridDigits()
{
}
//*************************************************************************************************
void CGridDigits::CalcDigits(_In_ double FirstVal, _In_ double LastVal, 
							 _In_ int WidthInPix, _In_opt_ const int &MaxColumnWidth /*= USE_DEFAULT*/)
{
	m_DigitsList.clear();
	m_IntermediateDigits.clear();

	int numvert, pokvert;
	float Delta, prom, IntermediateDelta;

	numvert = WidthInPix / (MaxColumnWidth == USE_DEFAULT ? WIDTH_COLUMNS_IN_PIX : MaxColumnWidth);		//	���������� ������������ �����
	if (numvert < 5)
		numvert = 5;
	prom = log10(float((LastVal - FirstVal) / numvert));
	pokvert = (int)(prom);
	if (prom >= 0)
		Delta = pow(10.f, prom - pokvert);
	else
	{
		Delta = pow(10.f, prom - pokvert + 1.f);
		pokvert--;
	}
	
	if (Delta <= 2){ Delta = 2; IntermediateDelta = 1; }
	else if (Delta <= 5){ Delta = 5; IntermediateDelta = 1; }
	else if (Delta <= 10){ Delta = 10; IntermediateDelta = 5; }
	else { Delta = 2; IntermediateDelta = 1; }
	// �������� ��� �� �������� ����� ����
	Delta = Delta * pow(10.f, pokvert);
	IntermediateDelta = IntermediateDelta * pow(10.f, pokvert);

	// ������ ������ ��� �����������
	if (pokvert >= 0)
		m_Format = L"%.0f";
	else
		m_Format.Format(L"%%0.%df", abs(pokvert));
	//Format = L"%g";

	// �������� ������� ��� ����� �������� ��� ��� ����� �������� ������ �����
	long double Count = ceil(FirstVal / Delta);
	long double CountIntermediate = ceil(FirstVal / IntermediateDelta);

	// ��������� ���������� ��� ������ � ����
	long double TempVal;
	// ������� � ������ ���������� ����� ���� �� ������ �� �����
// 	while ((TempVal = Count * (long float)Delta) <= LastVal)
// 	{
// 		// ���� ����������� ���������
// 		if (!m_DigitsList.empty() && m_DigitsList.back() == TempVal)
// 			break;
// 
// 		m_DigitsList.push_back(TempVal);		
// 		Count++;
// 	}
	// �������� ������ �����
	TempVal = Count * Delta;
	while (TempVal <= LastVal)
	{
		// ���� �� ���������� ����������, �� ��������� ����� �� �����������
		if (!m_DigitsList.empty() && m_DigitsList.back() == TempVal)
			break;

		m_DigitsList.push_back(TempVal);
		TempVal += Delta;
	}


	TempVal = CountIntermediate * IntermediateDelta;
	while (TempVal <= LastVal)
	{
		// ���� �� ���������� ����������, �� ��������� ����� �� �����������
		if (!m_IntermediateDigits.empty() && m_IntermediateDigits.back() == TempVal)
			break;

		m_IntermediateDigits.push_back(TempVal);
		TempVal += IntermediateDelta;
	}
}
//*************************************************************************************************