#ifndef ZETLAB_ZETPATH_H
#define ZETLAB_ZETPATH_H

#include <windows.h>

// Интерфейс для программ на C++

#ifdef __cplusplus

#include <string>

// Получение каталога установки ZETLab
#define zpGetInstallLocation()                    zpGetWString(zpQueryInstallLocation)

// Получение каталога с конфигурационными файлами
#define zpGetDirConfig()                          zpGetWString(zpQueryDirConfig)

// Получение каталога с файлами сигналов
#define zpGetDirSignal()                          zpGetWString(zpQueryDirSignal)

// Получение каталога с файлами трендов
#define zpGetDirCompressed()                      zpGetWString(zpQueryDirCompressed)

// Получение каталога с файлами результатов
#define zpGetDirResult()                          zpGetWString(zpQueryDirResult)

// Получение каталога с файлами пользоавтельских поправок
#define zpGetDirCorrect()                         zpGetWString(zpQueryDirCorrect)

// Получение дополнительной подпапки к директории расположения сигналов
#define zpGetDirSuffix()                          zpGetWString(zpQueryDirSuffix)

// Получение каталога с файлами СКАДА ZETView
#define zpGetDirSCADA()                           zpGetWString(zpQueryDirSCADA)

// Получение WatchDogTimer
#define zpGetDirWatchDogTimer()                   zpGetWString(zpQueryDirWatchDogTimer)

// получаем установленный язык программ ZETLab
#define zpGetLanguage()							  zpGetWString(zpQueryLanguage)

// Установка каталога с конфигурационными файлами
#define zpSetDirConfig(dirConfig)                 zpSetWString(zpEditDirConfig, dirConfig)

// Установка каталога с файлами сигналов
#define zpSetDirSignal(dirSignal)                 zpSetWString(zpEditDirSignal, dirSignal)

// Установка каталога с файлами трендов
#define zpSetDirCompressed(dirCompressed)         zpSetWString(zpEditDirCompressed, dirCompressed)

// Установка каталога с файлами результатов
#define zpSetDirResult(dirResult)                 zpSetWString(zpEditDirResult, dirResult)

// Установка каталога с файлами пользовательских поправок сигналов
#define zpSetDirSuffix(dirSuffix)                 zpSetWString(zpEditDirSuffix, dirSuffix)

// Установка языка для программ ZETLab
#define zpSetLanguage(Language)					  zpSetWString(zpEditLanguage, Language)

// Вызов функций ZETPath.dll и конвертирование строки в std::wstring
FORCEINLINE std::wstring zpGetWString(long (WINAPI *zpQueryFunc)(LPWSTR lpPath, DWORD cchCount))
{
	WCHAR buffer[MAX_PATH];
	return (zpQueryFunc(buffer, MAX_PATH) == 0 ? std::wstring(buffer) : std::wstring());
}

// Вызов функции ZETPath.dll и конвертирование строки из std::wstring
FORCEINLINE long zpSetWString(long (WINAPI *zpSetFunc)(LPCWSTR lpPath), const std::wstring &path)
{
	return zpSetFunc(path.c_str());
}

#endif // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif

#ifdef ZETPATH_EXPORTS
#define ZETPATH_API __declspec(dllexport)
#else
#define ZETPATH_API __declspec(dllimport)
#endif

// Функции, которые на самом деле экспортируются из ZETPath.dll
// Каждая функция возвращает код из ZetErrorCodes.h
ZETPATH_API long WINAPI zpQueryInstallLocation(LPWSTR lpPath, DWORD cchCount);
ZETPATH_API long WINAPI zpQueryDirConfig(LPWSTR lpPath, DWORD cchCount);
ZETPATH_API long WINAPI zpQueryDirSignal(LPWSTR lpPath, DWORD cchCount);
ZETPATH_API long WINAPI zpQueryDirCompressed(LPWSTR lpPath, DWORD cchCount);
ZETPATH_API long WINAPI zpQueryDirResult(LPWSTR lpPath, DWORD cchCount);
ZETPATH_API long WINAPI zpQueryDirSuffix(LPWSTR lpPath, DWORD cchCount);
ZETPATH_API long WINAPI zpQueryDirSCADA(LPWSTR lpPath, DWORD cchCount);
ZETPATH_API long WINAPI zpQueryLanguage(LPWSTR LanguageName, DWORD cchCount = LOCALE_NAME_MAX_LENGTH);

// Функции могут использоваться только в программах имеющих права администратора
// такой программой является ZetLoggerService
ZETPATH_API long WINAPI zpEditDirConfig(LPCWSTR lpPath);
ZETPATH_API long WINAPI zpEditDirSignal(LPCWSTR lpPath);
ZETPATH_API long WINAPI zpEditDirCompressed(LPCWSTR lpPath);
ZETPATH_API long WINAPI zpEditDirResult(LPCWSTR lpPath);
ZETPATH_API long WINAPI zpEditDirSuffix(LPCWSTR lpPath);
ZETPATH_API long WINAPI zpEditDirSCADA(LPWSTR lpPath, DWORD cchCount);
ZETPATH_API long WINAPI zpEditLanguage(LPCWSTR LanguageName);

#ifdef __cplusplus
}
#endif

#endif
