//------------------------------------------------------------------------------
#include "StdAfx.h"
#include <ZETFile\ZETFile.h>
#include <ATLComTime.h>
//------------------------------------------------------------------------------
// проверка файла на существование
bool FileExists(const CString &fileName)
{
	bool ret = !fileName.IsEmpty();
	if (ret)
	{
		if (fileName.GetAt(1) == L':')
			ret = CheckDriveLetter(fileName.GetAt(0));
		if (ret)
		{
			DWORD dwAttr = GetFileAttributes(fileName);
			ret = (dwAttr != INVALID_FILE_ATTRIBUTES) &&
				  (0 == (dwAttr & FILE_ATTRIBUTE_DIRECTORY));
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
// проверка директории на существование
bool DirectoryExists(const CString &sDirectory)
{
	bool ret = !sDirectory.IsEmpty();
	if (ret)
	{
		if (sDirectory.GetAt(1) == L':')
			ret = CheckDriveLetter(sDirectory.GetAt(0));
		if (ret)
		{
			DWORD dwAttr = GetFileAttributes(sDirectory);
			ret = (dwAttr != INVALID_FILE_ATTRIBUTES) &&
				(0 != (dwAttr & FILE_ATTRIBUTE_DIRECTORY));
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
// создание несуществующей папки (путь должен существовать)
bool CreateNewDirectory(const CString &directoryName)
{
	if (directoryName.IsEmpty())
		return false;

	if (DirectoryExists(directoryName))
		return true;

	int n = directoryName.GetLength() - 1;
	wchar_t w = directoryName.GetAt(n);
	if ((w == L'\\') || (w == L'/'))
		return CreateDirectory(directoryName.Mid(0, n), NULL) != FALSE;
	else
		return CreateDirectory(directoryName, NULL) != FALSE;
}
//------------------------------------------------------------------------------
// создание несуществующего пути до файла
bool CreatePath(const CString &fileName)
{
	bool ret = FileExists(fileName);
	if (!ret)
	{
		int nCur(0);
		while (true)
		{
			int nFind = fileName.Find(L"\\", nCur + 1);
			if (nFind != -1)
			{
				nCur = nFind;
				CString directory = fileName.Mid(0, nCur);
				if (!directory.IsEmpty())
				{
					ret = CreateNewDirectory(directory);
					if (!ret)
						break;
				}
				else
					break;
			}
			else
				break;
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
// чтение всего содержимого текстового файла (char)
CStringA LoadAllFromFileA(const CString &fileName, const long maxLines)
{
	CStringA ret;
	if (!fileName.IsEmpty() && FileExists(fileName))
	{
		long counterMax = (maxLines > 0) ? maxLines : LONG_MAX;
		FILE *in = _fsopen(CStringA(fileName), "rt", _SH_DENYNO);
		if (in != NULL)
		{
			long counter = 0;
			char* stringData = new char[2000];
			while (!feof(in))
			{
				if ((++counter >= counterMax) ||
					( fgets(stringData, 2000, in) == nullptr ))
					break;
				ret += CStringA(stringData);

			}
			delete [] stringData;
			stringData  = nullptr;
			fclose(in);
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
// чтение всего содержимого текстового файла (wchar_t)
bool ReadFileText(const CString &fileName, CString * text)
{
	bool ret = !fileName.IsEmpty() && (text != nullptr) && FileExists(fileName);
	if (ret)
	{
		FILE *in = _wfsopen(fileName, L"rt, ccs=UTF-8", _SH_DENYNO);
		ret = in != nullptr;
		if (ret)
		{
			*text = L"";
			wchar_t textDate[200] = { 0 };
			while (!feof(in))
			{
				if ( fgetws(textDate, 200, in) == nullptr)
					break;
				*text += CString(textDate);
			}
			fclose(in);
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
//запись (или перезапись) текстового файла (wchar_t)
// с созданием несуществующего пути до файла
bool WriteFileText(const CString &fileName, const CString &text)
{
	bool ret = !fileName.IsEmpty();
	if (ret)
	{
		if (!FileExists(fileName))
			ret = CreatePath(fileName);
		
		if (ret)
		{
			FILE *out = _wfsopen(fileName, L"wt, ccs=UTF-8", _SH_DENYNO);
			ret = out != NULL;
			if (ret)
			{
				if (!text.IsEmpty())
					ret = fwprintf_s(out, L"%s", text.GetString()) > 0;
				fclose(out);
			}
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
//добавление текста в файл
// с созданием несуществующего пути до файла
bool AddTextToFile(const CString &fileName, const CString &text)
{
	bool ret = !fileName.IsEmpty() && !text.IsEmpty();
	if (ret)
	{
		if (!FileExists(fileName))
			ret = CreatePath(fileName);
		
		if (ret)
		{
			FILE *out = _wfsopen(fileName, L"at, ccs=UTF-8", _SH_DENYNO);
			ret = out != NULL;
			if (ret)
			{
				ret = fwprintf_s(out, L"%s", text.GetString()) > 0;
				fclose(out);
			}
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
// запись данных в бинарный файл
bool WriteFileBinary(const CString &fileName, const PVOID pointer, const UINT size)
{
	bool ret = !fileName.IsEmpty() && (pointer != nullptr) && (size > 0);
	if (ret)
	{
		if (!FileExists(fileName))
			ret = CreatePath(fileName);
		
		if (ret)
		{
			FILE *out = _wfsopen(fileName, L"wb", _SH_DENYWR);
			ret = out != NULL;
			if (ret)
			{
				ret = fwrite(pointer, size, 1, out) == 1;
				fclose(out);
			}
		}

	}
	return ret;
}
//------------------------------------------------------------------------------
// чтение данных из бинарного файла
bool ReadFileBinary(const CString &fileName, PVOID pointer, const UINT size)
{
	bool ret = !fileName.IsEmpty() && FileExists(fileName) &&
		(pointer != nullptr) && (size > 0);
	if (ret)
	{
		FILE *out = _wfsopen(fileName, L"rb", SH_DENYRD);
		ret = out != NULL;
		if (ret)
		{// определяю размер файла
			ret = (fseek(out, 0L, SEEK_END) == 0) &&
				  ((UINT)ftell(out) == size);
			if (ret)
			{
				ret = (fseek(out, 0L, SEEK_SET) == 0) &&
					  (fread_s(pointer, size, size, 1, out) == 1);
			}
			fclose(out);
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
// чтение содержимого ansi-файла в CString с указанием кодировки.
bool LoadAllFromANSIFileByCodePage(const CString &fileName, CString &outString, UINT codePage)
{
	outString.Empty();
	bool ret = !fileName.IsEmpty() && FileExists(fileName);
	if (ret)
	{
		HANDLE hFile(CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL));
		ret = hFile != INVALID_HANDLE_VALUE;
		if (ret)
		{
			char charData[513] = { 0 };
			TCHAR wideData[513] = { 0 };
			DWORD bytesForRead = 512;
			DWORD bytesRead(0);
			while ((ReadFile(hFile, charData, bytesForRead, &bytesRead, NULL) != FALSE) && (bytesRead > 0))
			{
				charData[bytesRead] = '\0';
				if (MultiByteToWideChar(codePage, 0, charData, bytesRead + 1, wideData, 512) > 0)
				{
					outString += wideData;
				}
			}
			CloseHandle(hFile);
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
// Проверка существования диска с заданным именем
bool CheckDriveLetter(wchar_t letter)
{
	wchar_t list[MAX_PATH];
	DWORD numDrive = GetLogicalDriveStrings(MAX_PATH, list) / 4;
	bool ret = numDrive > 0;
	if (ret)
	{
		wchar_t ch = toupper(letter);
		for (DWORD i = 0; i < numDrive; ++i)
		{
			ret = ch == list[4 * i];
			if (ret)
				break;
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
void AddLog(const CString &sTitle, const CString &sText, const CString &sNameLogFile)
{
	if (!sTitle.IsEmpty() || (!sText.IsEmpty()))
	{	// папка log-файла
		wchar_t dirLog[MAX_PATH] = { 0 };
		HKEY hKey(NULL);
		DWORD type(REG_SZ);
		ULONG len(sizeof(dirLog));
		if (!((RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\ZET\\ZETLab", 0, KEY_READ, &hKey) == ERROR_SUCCESS) &&
			(hKey != NULL) &&
			(RegQueryValueEx(hKey, L"DirLog", 0, &type, (LPBYTE)dirLog, &len) == ERROR_SUCCESS) &&
			(type == REG_SZ)))
			wcscpy_s(dirLog, MAX_PATH, L"c:\\ZetLab\\Config\\Log\\");

		// Имя файла
		CString nameLogFile = sNameLogFile;
		if (nameLogFile.Find(L".log") == -1)
			nameLogFile += L".log";
		nameLogFile = (CString)dirLog + nameLogFile;

		// Проверка размера
		bool bNeedDel(false);
		DWORD sizeFile(0);
		if (FileExists(nameLogFile))
		{
			WIN32_FILE_ATTRIBUTE_DATA fad;
			if (GetFileAttributesEx(nameLogFile, GetFileExInfoStandard, &fad) != FALSE)
			{
				if (fad.nFileSizeHigh > 0)
					DeleteFile(nameLogFile);
				else
				{
					sizeFile = fad.nFileSizeLow;
					if (sizeFile > 1048576)
					{// Файл надо урезать
						char *array = new char[sizeFile];

						ZeroMemory(array, sizeFile);

						if (ReadFileBinary(nameLogFile, array, sizeFile))
						{
							char *pointer = array + 104858;
							for (int i = 0; i < MAX_PATH; ++i, ++pointer)
							{
								if ((pointer[0] == '\r') &&
									(pointer[1] == '\n'))
								{
									pointer += 2;
									break;
								}
							}
							WriteFileBinary(nameLogFile, pointer,
								sizeFile - (pointer - array));
						}
						else
							DeleteFile(nameLogFile);
						delete[] array;
					}
				}
			}
		}

		// дата + время
		COleDateTime odt = COleDateTime::GetTickCount();
		CString text = odt.Format(L"[%d.%m.%Y %H:%M:%S]") + L"\t";

		// имя программы
		CString str = GetCommandLine();
		int m = str.ReverseFind(L'\\');
		if (m != -1)
			str.Delete(0, m + 1);
		m = str.ReverseFind(L'\"');
		if (m != -1)
			str.Delete(m, 1);		// надо удалять
		str.TrimRight();	// удаляю последний пробел, он не нужен
		text += str;

		// имя проблемы и описание
		text += L"\t" + sTitle + L"\t" + sText + L"\n";

		AddTextToFile(nameLogFile, text);
	}
}
//------------------------------------------------------------------------------