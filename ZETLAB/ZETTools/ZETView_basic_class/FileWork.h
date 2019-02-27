//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include "atlbase.h"
#include <ZetFile\ZETFile.h>
//------------------------------------------------------------------------------
struct CFileWork
{
	//извлечение пути к директории SCADA системы
	//static CString GetConfigPath();
	CString GetSCADAAppData();
	//извлечение пути к директории ZETLab системы
	//CString GetZETLabPath();
	//создание директории
	static bool CreateNewDirectory(LPCTSTR sPathTo);
	//проверка файла на существование
	static bool FileExists(CString fileName)
	{
		return ::FileExists(fileName);
	}
	//проверка директории на существование
	static bool DirectoryExists(CString sDirectory)
	{
		return ::DirectoryExists(sDirectory);
	}
	//создаться файл
	//static bool CreateNewFile(CString fileName);
	//добавиться строку в файл
	static void AddStringToFile(CString fileName, CString addingStr, CString endOfString)
	{
		AddTextToFile(fileName, addingStr + endOfString);
	}
	//перезапись файла
	static void WriteFile(CString fileName, CString addingStr)
	{
		WriteFileText(fileName, addingStr);
	}
	//считать содержимое из файла
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

	// чтение содержимого ansi-файла в CString с указанием кодировки.
	static bool LoadAllFromANSIFileByCodePage(CString &fileName, CString &outString, UINT codePage = CP_ACP);

	//поиск всех файлов в директории
	//CArray <CString,CString&> m_sFiles;
	//void showFiles(CString dir, CString mask);
};
//------------------------------------------------------------------------------