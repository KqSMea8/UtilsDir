#pragma once
#include <ZetDictionary/include/ZetDictionary.h>

#ifndef _TRANSLATE
#define _TRANSLATE
#endif

#define DICTIONARY_NAME		"ZetDictionary"
#define DICTIONARY_LIB_EXT	".lib"

/* ���� � ���������� */
#ifndef DEBUG
#	define DICTIONARY_LIB_NAME	DICTIONARY_NAME DICTIONARY_LIB_EXT
#else
#	define DICTIONARY_LIB_NAME	DICTIONARY_NAME "D" DICTIONARY_LIB_EXT
#endif

#ifndef _WIN64
#	define ARCHITECTURE_WIN	"Win32"
#else
#	define ARCHITECTURE_WIN	"Win64"
#endif

#pragma comment(lib, DICTIONARY_NAME "\\bin\\" ARCHITECTURE_WIN "\\" DICTIONARY_LIB_NAME)

#ifdef UNICODE

// ������� ������ � �������:
// ���� �������� ���� - �������;
// ���� �������� ���� - �� ������.
void InitLanguageDst();

// ��� ������� ���������� ������� ���� ��� � ������������ �������� ���� ���������.
// ������� ������ � �������:
// ���� �������� ���� - �������;
// ���� �������� ���� - �� �������.
// ������� ��������� �������� _exeName:
// ���� _exeName = L"", �� �������� - ��� ��� ���������.
void InitZetDictionary(_In_ const CString & _exeName = L"");

// ������� �����. ����� �������� ������� �������� �������.
// _String - ����������� �����;
// sParam - �������� �������� (���� �� �����, �� ��� ���������).
// ������������ ������: ���� � ������� ���� �������� ������,
// �� - ������������ ������,
// ����� - �������� ������.
CString TranslateString(_In_ const CString & _String, _In_opt_ const CString & sParam = L"");

// ������� ����� �� ��������. ����� �������� ������� �������� �������.
// nID - ���������� ����������� ����� � ��������.
// sParam - �������� �������� (���� �� �����, �� ��� ���������).
// �������� �������� - ��� ���������.
// ������������ ������: ���� � ������� ���� �������� ������
// �� - ������������ ������,
// ����� - �������� ������.
// ��������!!! � �������� �� ����� ���� WCHAR.
CString TranslateResourse(_In_ const UINT & nID, _In_opt_ const CString & sParam = L"");

// �������������� ����������� �������������� ���� �� ������ ������� � ��������� ����.
// ������� ������������� ��� ��������, ������� ����������� � ������� exit(0).
void ExitProgramm(_In_ const int code);

// ������� ����� � ������� ������������ ������� ���������. ����� �������� ������� �������� �������.
// _String - ����������� �����;
// sParam - �������� �������� (���� �� �����, �� ��� ���������).
// ������������ ������: ���� � ������� ���� �������� ������ � ��� ����� ������� ���������,
// �� - ��������� � ��������� ������,
// ����� - ���������� �������� ������.
CString TranslateDividedString(_In_ const CString & _String, _In_opt_ const CString & sParam = L"");

// ������� ��� ��������� ����� ���������.
CString GetLanguageDst();

// ������� ��������� ����� ���������.
void SetLanguageSrc(CString newVal);

CString GetLanguageSrc();

#endif	// #ifdef UNICODE