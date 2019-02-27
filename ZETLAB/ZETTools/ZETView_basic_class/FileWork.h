//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include "atlbase.h"
#include <ZetFile\ZETFile.h>
//------------------------------------------------------------------------------
struct CFileWork
{
	//���������� ���� � ���������� SCADA �������
	//static CString GetConfigPath();
	CString GetSCADAAppData();
	//���������� ���� � ���������� ZETLab �������
	//CString GetZETLabPath();
	//�������� ����������
	static bool CreateNewDirectory(LPCTSTR sPathTo);
	//�������� ����� �� �������������
	static bool FileExists(CString fileName)
	{
		return ::FileExists(fileName);
	}
	//�������� ���������� �� �������������
	static bool DirectoryExists(CString sDirectory)
	{
		return ::DirectoryExists(sDirectory);
	}
	//��������� ����
	//static bool CreateNewFile(CString fileName);
	//���������� ������ � ����
	static void AddStringToFile(CString fileName, CString addingStr, CString endOfString)
	{
		AddTextToFile(fileName, addingStr + endOfString);
	}
	//���������� �����
	static void WriteFile(CString fileName, CString addingStr)
	{
		WriteFileText(fileName, addingStr);
	}
	//������� ���������� �� �����
	static CString LoadAllFromFile(CString fileName, long maxLines = -1)
	{
		CString ret;
		ReadFileText(fileName, &ret);
		return ret;
	}
	static CStringA LoadAllFromFileA(CString fileName, long maxLines = -1)
	{
		return ::LoadAllFromFileA(fileName, maxLines);
	}

	// ������ ����������� ansi-����� � CString � ��������� ���������.
	static bool LoadAllFromANSIFileByCodePage(CString &fileName, CString &outString, UINT codePage = CP_ACP);

	//����� ���� ������ � ����������
	//CArray <CString,CString&> m_sFiles;
	//void showFiles(CString dir, CString mask);
};
//------------------------------------------------------------------------------