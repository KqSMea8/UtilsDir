//------------------------------------------------------------------------------
#pragma once
#include <atlstr.h>
#include <stdio.h>
#include "share.h"
//------------------------------------------------------------------------------
//		���������� �������
//------------------------------------------------------------------------------
// �������� ����� �� �������������
bool FileExists(const CString &fileName);

// �������� ���������� �� �������������
bool DirectoryExists(const CString &sDirectory);

// �������� �������������� ����� (���� ������ ������������)
bool CreateNewDirectory(const CString &directoryName);

// �������� ��������������� ���� �� �����
bool CreatePath(const CString &fileName);

// ������ ����� ����������� ���������� ����� (char)
CStringA LoadAllFromFileA(const CString &fileName, const long maxLines);

// ������ ����� ����������� ���������� ����� (wchar_t)
bool ReadFileText(const CString &fileName, CString * text);

//������ (��� ����������) ���������� ����� (wchar_t)
bool WriteFileText(const CString &fileName, const CString &text);

//���������� ������ � ����
bool AddTextToFile(const CString &fileName, const CString &text);

// ������ ������ � �������� ����
bool WriteFileBinary(const CString &fileName, const PVOID pointer, const UINT size);

// ������ ������ �� ��������� �����
bool ReadFileBinary(const CString &fileName, PVOID pointer, const UINT size);

// ������ ����������� ansi-����� � CString � ��������� ���������.
bool LoadAllFromANSIFileByCodePage(const CString &fileName, CString &outString, UINT codePage = CP_ACP);

// �������� ������������� ����� � �������� ������
bool CheckDriveLetter(wchar_t letter);

void AddLog(const CString &sTitle, const CString &sText, const CString &sNameLogFile = L"ZetFile");
//------------------------------------------------------------------------------