//------------------------------------------------------------------------------
// ������������ ����� ��������� ���������� �������� ZetLab.
// ������ ������ � ���� ���������� ��. � ��������� "��������� ����������� ����"
//------------------------------------------------------------------------------
#pragma once
#include <stdint.h>
#include <ZetTools\include\ZetTools.h>	// ztStringCopyW
//------------------------------------------------------------------------------
// 62 - ��� ������������ ��������� �� 8 ���� (sizeof(WINDOWPLACEMENT) == 44)
#define SIZE_NAME_PROGRAM	62
//------------------------------------------------------------------------------
struct CustomSaveStructure
{
	uint32_t size;					// ������ ���������, ������� ���� �� ��������
	uint32_t numberProgram;			// ����� ���������� �����
	WINDOWPLACEMENT wPlace;			// ��������� ������������ ���� ���������
	wchar_t nameProgram[SIZE_NAME_PROGRAM];	// ��� ���������

	CustomSaveStructure()
	{
		ZeroMemory(this, sizeof(CustomSaveStructure));
		wPlace.length = sizeof(WINDOWPLACEMENT);
	}

	~CustomSaveStructure() {}

	CustomSaveStructure(const CustomSaveStructure & val)
	{
		*this = val;
	}

	CustomSaveStructure & operator = (const CustomSaveStructure & val)
	{
		auto size_s = sizeof(CustomSaveStructure);
		memcpy_s(this, size_s, &val, size_s);
		return *this;
	}

	// ���������� ������� �����
	bool SetName(LPCWSTR pName)
	{
		return 0 == ztStringCopyW(nameProgram, SIZE_NAME_PROGRAM, pName);
	}

	// ���������������� ���� ���������
	void SetPlacement(HWND hWnd)
	{
		if ((wPlace.length == sizeof(WINDOWPLACEMENT)) &&
			((wPlace.rcNormalPosition.right - wPlace.rcNormalPosition.left) > 0) &&
			(wPlace.rcNormalPosition.bottom - wPlace.rcNormalPosition.top) > 0)
			::SetWindowPlacement(hWnd, &wPlace);
	}

	// ���������� ���������� ���������������� ���� ���������
	void GetPlacement(HWND hWnd)
	{
		ZeroMemory(&wPlace, sizeof(WINDOWPLACEMENT));
		wPlace.length = sizeof(WINDOWPLACEMENT);
		::GetWindowPlacement(hWnd, &wPlace);
		if (wPlace.showCmd == SW_SHOWMINIMIZED)
			wPlace.showCmd = SW_SHOWNORMAL;
	}
};
//------------------------------------------------------------------------------