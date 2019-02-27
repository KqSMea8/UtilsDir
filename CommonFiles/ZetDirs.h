#pragma once

#include <assert.h>
#include <windows.h>
#include <memory>
#include "Singleton.h"

// Список источников путей

// Список генерируется с помощью макрофункции

#define GENERATE_SOURCE_TABLE(TD) \
	TD(SOURCE_REG_ZET,    L"SOFTWARE\\ZET\\ZETLab",     KEY_READ) \
	TD(SOURCE_REG_ZETLAB, L"SOFTWARE\\ZETLAB\\Settings",KEY_READ | KEY_WOW64_64KEY)

#pragma comment(lib, "Advapi32.lib")

typedef enum
{
#define TD(index, subkey, sam) index,
	GENERATE_SOURCE_TABLE(TD)
#undef TD
} SOURCE_INDEX;

typedef struct
{
	LPCWSTR subKey;
	DWORD samDesired;
} SOURCE_ENTRY;

static const SOURCE_ENTRY SOURCE_TABLE[] =
{
#define TD(index, subkey, sam) { subkey, sam },
	GENERATE_SOURCE_TABLE(TD)
#undef TD
};

//*****************************************************************************
// класс отправки сообщений
class ZetDirsService
{
	friend class CSingleton<ZetDirsService>;
public://----------------------------------------------------------------------
	CString getSignalsDir();
	CString getCompressedDir();
	// получаем текущую директорию
	CString getCurrentDir();

private:
	static CString querySource(SOURCE_INDEX source, LPCWSTR lpName);
};

//*****************************************************************************
inline CString ZetDirsService::getSignalsDir()
{
	return querySource(SOURCE_REG_ZETLAB, L"DirSignal");
}

//*****************************************************************************
inline CString ZetDirsService::getCompressedDir()
{
	return querySource(SOURCE_REG_ZETLAB, L"DirCompressed");
}

//*****************************************************************************
inline CString ZetDirsService::getCurrentDir()
{
	TCHAR szPath[_MAX_PATH * 10];
	VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szPath, _MAX_PATH * 10));
	CString currentDir(szPath);
	int nIndex = currentDir.ReverseFind(_T('\\'));
	if (nIndex > 0)
		currentDir = currentDir.Left(nIndex + 1);
	return currentDir;
}

//*****************************************************************************
// Получение пути с указанного источника (пока только реестр HKLM)
inline CString ZetDirsService::querySource(SOURCE_INDEX source, LPCWSTR lpName)
{
	HKEY key(NULL);
	LONG status;
	DWORD type;
	DWORD cb;

	DWORD cchCount = MAX_PATH * 10;
	// Проверка параметров
	std::shared_ptr<WCHAR> lpPath(new WCHAR[cchCount]);

	//LPCWSTR lpPath = new WCHAR[cchCount];

	status = RegOpenKeyEx(HKEY_LOCAL_MACHINE, SOURCE_TABLE[source].subKey, 0, SOURCE_TABLE[source].samDesired, &key);
	if (ERROR_SUCCESS != status)
	{
		RegCloseKey(key);
		RegCreateKeyEx(HKEY_LOCAL_MACHINE, SOURCE_TABLE[source].subKey, 0, L"", 0, SOURCE_TABLE[source].samDesired | KEY_WRITE, NULL, &key, NULL);
		RegCloseKey(key);
		status = RegOpenKeyEx(HKEY_LOCAL_MACHINE, SOURCE_TABLE[source].subKey, 0, SOURCE_TABLE[source].samDesired, &key);
	}
	if (status == ERROR_SUCCESS)
	{
		cb = cchCount * sizeof(WCHAR);
		status = RegQueryValueEx(key, lpName, NULL, &type, (LPBYTE)lpPath.get(), &cb);
		cb /= sizeof(WCHAR);

		RegCloseKey(key);
	}

	return CString(lpPath.get());
}
