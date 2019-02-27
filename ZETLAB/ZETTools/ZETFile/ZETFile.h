//------------------------------------------------------------------------------
#pragma once
#include <atlstr.h>
#include <stdio.h>
#include "share.h"
//------------------------------------------------------------------------------
//		Глобальные функции
//------------------------------------------------------------------------------
// проверка файла на существование
bool FileExists(const CString &fileName);

// проверка директории на существование
bool DirectoryExists(const CString &sDirectory);

// создание несуществующей папки (путь должен существовать)
bool CreateNewDirectory(const CString &directoryName);

// создание несуществующего пути до файла
bool CreatePath(const CString &fileName);

// чтение всего содержимого текстового файла (char)
CStringA LoadAllFromFileA(const CString &fileName, const long maxLines);

// чтение всего содержимого текстового файла (wchar_t)
bool ReadFileText(const CString &fileName, CString * text);

//запись (или перезапись) текстового файла (wchar_t)
bool WriteFileText(const CString &fileName, const CString &text);

//добавление записи в файл
bool AddTextToFile(const CString &fileName, const CString &text);

// запись данных в бинарный файл
bool WriteFileBinary(const CString &fileName, const PVOID pointer, const UINT size);

// чтение данных из бинарного файла
bool ReadFileBinary(const CString &fileName, PVOID pointer, const UINT size);

// чтение содержимого ansi-файла в CString с указанием кодировки.
bool LoadAllFromANSIFileByCodePage(const CString &fileName, CString &outString, UINT codePage = CP_ACP);

// Проверка существования диска с заданным именем
bool CheckDriveLetter(wchar_t letter);

void AddLog(const CString &sTitle, const CString &sText, const CString &sNameLogFile = L"ZetFile");
//------------------------------------------------------------------------------