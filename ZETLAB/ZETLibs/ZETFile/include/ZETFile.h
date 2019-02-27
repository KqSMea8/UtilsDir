//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
#ifdef ZETFILE_EXPORTS
#define ZETFILE_API __declspec(dllexport)
#else
#define ZETFILE_API __declspec(dllimport)
#endif
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <ZetErrorCodes.h>
#include "share.h"
//-----------------------------------------------------------------------------
#define NUMBER_BITS_IN_BYTE         8
#define NUMBER_BYTES_IN_KBYTE		1024
#define NUMBER_KBYTES_IN_MBYTE		1024
#define NUMBER_BYTES_IN_MBYTE		(NUMBER_KBYTES_IN_MBYTE * NUMBER_BYTES_IN_KBYTE)
#define NUMBER_MBYTES_IN_GBYTE		1024
#define NUMBER_KBYTES_IN_GBYTE		(NUMBER_MBYTES_IN_GBYTE * NUMBER_KBYTES_IN_MBYTE)
#define NUMBER_BYTES_IN_GBYTE		(NUMBER_KBYTES_IN_GBYTE * NUMBER_BYTES_IN_KBYTE)

#define MAX_SIZE_OF_STRING_IN_FILE	200
#define LOG_FILE_MAX_SIZE			1048576	// 1 Мб
#define LOG_FILE_SIZE_FOR_DELETE	102400	// 100 кб
#define TIME_FOR_NEXT_PING_IN_SEC	5		// 5 секунд
//-----------------------------------------------------------------------------
// Проверка существования диска с заданным char-именем.
// Возвращаемое значение :
// отрицательное значение - код ошибки;
// 0 - диска нет;
// 1 - диск есть.
_Check_return_
long ZETFILE_API zfCheckDriveLetterA(_In_ const char letter);
// Проверка существования диска с заданным wchar-именем.
// Возвращаемое значение :
// отрицательное значение - код ошибки;
// 0 - диска нет;
// 1 - диск есть.
_Check_return_
long ZETFILE_API zfCheckDriveLetterW(_In_ const wchar_t letter);

#ifdef UNICODE
#define zfCheckDriveLetter  zfCheckDriveLetterW
#else
#define zfCheckDriveLetter  zfCheckDriveLetterA
#endif
//-----------------------------------------------------------------------------
// Посылка ping сетевому компу, имя которого задано char-текстом.
// numPackage - кол-во посылаемых пакетов.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - нет ответов от сетевого компа или все они неправельные;
// <= numPackage - кол-во правильных ответов;
// numPackage + 1 - ping не было, т.к. последний пинг был недавно и удачным
_Check_return_
long ZETFILE_API zfPingA(_In_ LPCSTR lpIPorName, _In_ const BYTE numPackage = 4);
// Посылка ping сетевому компу, имя которого задано wchar-текстом.
// numPackage - кол-во посылаемых пакетов.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - нет ответов от сетевого компа или все они неправельные;
// <= numPackage - кол-во правильных ответов;
// numPackage + 1 - ping не было, т.к. последний пинг был недавно и удачным
_Check_return_
long ZETFILE_API zfPingW(_In_ LPCWSTR lpIPorName, _In_ const BYTE numPackage = 4);

#ifdef UNICODE
#define zfPing  zfPingW
#else
#define zfPing  zfPingA
#endif
//-----------------------------------------------------------------------------
// Проверка на существование файла, имя которого задано char-текстом.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - файла нет;
// 1 - файл есть.
_Check_return_
long ZETFILE_API zfFileExistsA(_In_ LPCSTR lpFileName);

// Проверка на существованиефайла , имя которого задано wchar-текстом..
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - файла нет;
// 1 - файл есть.
_Check_return_
long ZETFILE_API zfFileExistsW(_In_ LPCWSTR lpFileName);

#ifdef UNICODE
#define zfFileExists  zfFileExistsW
#else
#define zfFileExists  zfFileExistsA
#endif
//-----------------------------------------------------------------------------
// Получение размера файла.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - размер определён.
_Check_return_
long ZETFILE_API zfGetFileSizeInBytes(_In_ LPCWSTR lpFileName, _Out_ DWORD * pSize);
//-----------------------------------------------------------------------------
// Проверка на существование папки, заданной wchar-текстом.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - папки нет;
// 1 - папка есть.
_Check_return_
long ZETFILE_API zfDirectoryExists(_In_ LPCWSTR lpDirectoryName);
//-----------------------------------------------------------------------------
// Cоздание несуществующей папки (путь должен существовать).
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - папка создана или уже существует.
_Check_return_
long ZETFILE_API zfCreateNewDirectory(_In_ LPCWSTR lpDirectoryName);
//-----------------------------------------------------------------------------
// Создание несуществующего пути до файла.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - путь создан или уже существует.
_Check_return_
long ZETFILE_API zfCreatePath(_In_ LPCWSTR lpFileName);
//-----------------------------------------------------------------------------
// Открытие файла для чтения/записи данных.
// Если чтение, то вначале проверка файла на существование.
// Если запись, то вначале проверка пути, если нет, то путь создаётся.
// Возвращаемое значение:
// отрицательное знаечние - код ошибки;
// 0 - файл открыт.
//-----------------------------------------------
// Тип доступа (lpwsFileMode)
// r - только чтение, файл должен существовать
// w - только запись, существующий файл удаляется
// a - добавление в конец, файла может не быть
// r+ - чтение и запись, файл должен существовать
// w+ - чтение и запись, существующий файл удаляется
// a+ - чтение и добавление в конец, файла может не быть
//
// Для поддержки текстовых файлов с UNICODE-символами необходимо
// через запятую добавить флаг "ccs". Допустимые значения:
// ccs=UNICODE
// ccs=UTF-8
// ccs=UTF-16LE
// Внимание: UTF-16BE - не поддерживается, функция вернёт код ошибки = -118.
//
// Пример L"rt, ccs=UTF-8"
//-----------------------------------------------
// Тип данных в файле: t - текстовый, b - бинарный
//-----------------------------------------------
// Имя файла провряется на наличие недопустимых символов
_Check_return_
long ZETFILE_API zfOpenFileEx(_In_ LPCWSTR lpwsFileName, _In_ LPCWSTR lpwsFileMode, _In_ int iFlag, _Out_ FILE **ppFile);
// Вызов без блокировки
_Check_return_
long ZETFILE_API zfOpenFile(_In_ LPCWSTR lpwsFileName, _In_ LPCWSTR lpwsFileMode, _Out_ FILE **ppFile);
// Вызов с блокировкой
_Check_return_
long ZETFILE_API zfOpenFileDeny(_In_ LPCWSTR lpwsFileName, _In_ LPCWSTR lpwsFileMode, _Out_ FILE **ppFile);

//-----------------------------------------------------------------------------
// Закрытие открытого файла.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - файл закрыт.
long ZETFILE_API zfCloseFile(_In_ FILE *pFile);
//-----------------------------------------------------------------------------
// Добавление char-текста в файл.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - текст записан в файл.
_Check_return_
long ZETFILE_API zfAddTextToFileA(_In_ LPCWSTR lpwsFileName, _In_ LPCSTR lpsText);
// Добавление wchar-текста в файл.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - текст записан в файл.
_Check_return_
long ZETFILE_API zfAddTextToFileW(_In_ LPCWSTR lpwsFileName, _In_ LPCWSTR lpwsText);

#ifdef UNICODE
#define zfAddTextToFile  zfAddTextToFileW
#else
#define zfAddTextToFile  zfAddTextToFileA
#endif
//-----------------------------------------------------------------------------
// Чтение всего char-текста из файла.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - текст прочитан из файла.
_Check_return_
long ZETFILE_API zfReadFileTextA(_In_ LPCWSTR lpwsFileName, _In_opt_ LPSTR lpsBuffer, _In_ const DWORD sizeBuffer);
// Чтение всего wchar-текста из файла.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - текст прочитан из файла.
_Check_return_
long ZETFILE_API zfReadFileTextW(_In_ LPCWSTR lpwsFileName, _In_opt_ LPWSTR lpwsBuffer, _In_ const DWORD sizeBuffer);

#ifdef UNICODE
#define zfReadFileText  zfReadFileTextW
#else
#define zfReadFileText  zfReadFileTextA
#endif
//-----------------------------------------------------------------------------
// Запись (или перезапись) char-текста в файл
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - текст в файл записан.
_Check_return_
long ZETFILE_API zfWriteFileTextA(_In_ LPCWSTR lpwsFileName, _In_ LPCSTR lpsText);
_Check_return_
// Запись (или перезапись) wchar-текста в файл
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - текст в файл записан.
long ZETFILE_API zfWriteFileTextW(_In_ LPCWSTR lpwsFileName, _In_ LPCWSTR lpwsText);

#ifdef UNICODE
#define zfWriteFileText  zfWriteFileTextW
#else
#define zfWriteFileText  zfWriteFileTextA
#endif
//-----------------------------------------------------------------------------
// Запись данных в бинарный файл.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - данные в файл записаны.
_Check_return_
long ZETFILE_API zfWriteFileBinary(_In_ LPCWSTR lpwsFileName, _In_ const PVOID pointer, _In_ const DWORD size);
//-----------------------------------------------------------------------------
// Чтение данных из бинарного файла.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - данные из файла прочитаны.
_Check_return_
long ZETFILE_API zfReadFileBinary(_In_ LPCWSTR lpwsFileName, _In_opt_ PVOID pointer, _In_  const DWORD size);
//-----------------------------------------------------------------------------
// Запись в текстовый log-файл.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - данные в файл записаны.
_Check_return_
long ZETFILE_API zfAddLog(_In_ LPCWSTR lpwsTitle, _In_ LPCWSTR lpwsText, _In_ LPCWSTR lpcwsFileNameLog, _In_ const bool bNeedNameexe = true);
//-----------------------------------------------------------------------------
// Запись char-текста в открытый файл, без его закрытия.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - текст в файл записан.
_Check_return_
long ZETFILE_API zfWriteIntoFileA(_In_ FILE *pFile, _In_ LPCSTR lpsText);
// Запись wchar-текста в открытый файл, без его закрытия.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - текст в файл записан.
_Check_return_
long ZETFILE_API zfWriteIntoFileW(_In_ FILE *pFile, _In_ LPCWSTR lpwsText);

#ifdef UNICODE
#define zfWriteIntoFile  zfWriteIntoFileW
#else
#define zfWriteIntoFile  zfWriteIntoFileA
#endif
//-----------------------------------------------------------------------------
// Запись в открытый бинарный файл, без его закрытия.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - данные в файл записаны.
_Check_return_
long ZETFILE_API zfWriteIntoFileB(_In_ FILE *pFile, _In_ const PVOID pointer, _In_ const DWORD size);
//-----------------------------------------------------------------------------
// Чтение char-строки из открытого текстового файла, без его закрытия.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - текст из файла прочитан.
_Check_return_
long ZETFILE_API zfReadFromFileA(_In_ FILE *pFile, _In_opt_ LPSTR lpsBuffer, _In_ const DWORD sizeBuffer);
_Check_return_
// Чтение wchar-строки из открытого текстового файла, без его закрытия.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - текст из файла прочитан.
long ZETFILE_API zfReadFromFileW(_In_ FILE *pFile, _In_opt_ LPWSTR lpwsBuffer, _In_ const DWORD sizeBuffer);

#ifdef UNICODE
#define zfReadFromFile  zfReadFromFileW
#else
#define zfReadFromFile  zfReadFromFileA
#endif
//-----------------------------------------------------------------------------
// Чтение данных из открытого бинарного файла, без его закрытия.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - данные из файла прочитаны.
_Check_return_
long ZETFILE_API zfReadFromFileB(_In_ FILE *pFile, _In_opt_ PVOID pointer, _In_ const DWORD size);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//	Класс CFolderDialog предназначен для вызова диалога выбора папки (не файла!)
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/*	Использование класса:
CFolderDialog cd(L"Выбор папки для ...", L"c:\\Maker", L"c:", m_hWnd);
if (cd.DoModal() == IDOK)
{
	m_CEdit.SetWindowText(cd.GetSelectedFolder());
}	*/
//------------------------------------------------------------------------------
/* Есть функции задания владельца диалога, заголовка, корневой папки поиска и
текущей папки.
Если задан владелец класса, то окно диалога будет модальным.
Заголовок - это не имя окна поиска. Он отображается внутри окна,
в верхней строке.
Если задана корневая папка поиска (например: с:), то выбор возможен только в
подпапках этой папки (только на диске С).
Если задана текущая папка, то по умолчанию она будет выбранной.
*/
//------------------------------------------------------------------------------
#define CFOLDER_MAX_PATH		MAX_PATH * 10		// максимальная длина пути папки
//------------------------------------------------------------------------------
/* Конструктор класса
 CFolderDialog(LPCWSTR lpwsTitle = nullptr, LPCWSTR lpwsCurFolder = nullptr, LPCWSTR lpwsRoot = nullptr, const HWND hWndOwner = NULL)
 Параметры:
	- заголовок в окне выбора папки;
	- имя папки по умолчанию;
	- имя корневой папки поиска;
	- дескриптор окна владельца.
*/			 
//------------------------------------------------------------------------------
class ZETFILE_API CFolderDialog
{
private:
	HWND m_hWndOwner;
	wchar_t *m_sTitle;
	wchar_t *m_sRoot;
	wchar_t *m_sFolder;
	wchar_t *m_sTmp;

	bool b_UseRootFolder;
protected:

public:
	CFolderDialog(LPCWSTR lpwsTitle = nullptr, LPCWSTR lpwsCurFolder = nullptr,
		LPCWSTR lpwsRoot = nullptr, const HWND hWndOwner = NULL);
	~CFolderDialog();

	// Добавление к имени выбранной папки слеша
	bool m_bAddingSlash;
	// Отображение кнопки создать новую папку.
	// Блокирует отображение выбранной папки под заголовком и
	// отображение файлов в папке
	bool m_bCreateNewFolder;
	// Отображение имени выбранной папки под заголовком
	bool m_bStatusText;
	// Отображение файлов в папке
	bool m_bViewFilesInFolder;

	// функции свойств --------------------------------------------------------
	/* Имя выбранной папки */
	wchar_t* GetSelectedFolder();
	// Имя корневой папки выбора
	wchar_t* GetRootFolder();
	// Заголовок в окне выбора папки
	wchar_t* GetTitle();

	// Задание имени выбранной папки по умолчанию
	void SetCurFolder(LPCWSTR lpwsCurFolder);
	// Задание корневой папки выбора
	void SetRootFolder(LPCWSTR lpwsRoot);
	// Задание текста заголовка в окне выбора
	void SetTitle(LPCWSTR lpwsTitle);
	// Задание дескриптора окна владельца
	void SetOwner(const HWND hWndOwner);

	//// функции класса ---------------------------------------------------------
	/* Вызов окна диалога. Возвращаемое значение:
	- IDOK - папка выбрана;
	- IDCANCEL - папка не выбрана
	*/
	INT_PTR DoModal();
};
//-----------------------------------------------------------------------------
// Удаление файла, имя которого задано char-строкой.
// Вначале выполняется проверка наличия диска, потом проверка наличия файла.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - файл удалён или его не было.
_Check_return_ ZETFILE_API 
long zfDeleteFileA(_In_opt_ LPCSTR lpsName);

// Удаление файла, имя которого задано wchar-строкой.
// Вначале выполняется проверка наличия диска, потом проверка наличия файла.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - файл удалён или его не было.
_Check_return_ ZETFILE_API 
long zfDeleteFileW(_In_opt_ LPCWSTR lpwsName);

#ifdef UNICODE
#define  zfDeleteFile   zfDeleteFileW
#else
#define  zfDeleteFile   zfDeleteFileA
#endif
//-----------------------------------------------------------------------------
// Удаление всех файлов из папки, имя которой задано char-строкой.
// Вначале выполняется проверка наличия папки.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - файлы удалены или их не было.
_Check_return_ ZETFILE_API
long zfDeleteAllFilesInDirectoryA(_In_ LPCSTR lpsName);

// Удаление всех файлов из папки, имя которой задано wchar-строкой.
// Вначале выполняется проверка наличия папки.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - файлы удалены или их не было.
_Check_return_ ZETFILE_API
long zfDeleteAllFilesInDirectoryW(_In_ LPCWSTR lpwsName);

#ifdef UNICODE
#define  zfDeleteAllFilesInDirectory   zfDeleteAllFilesInDirectoryW
#else
#define  zfDeleteAllFilesInDirectory   zfDeleteAllFilesInDirectoryA
#endif
//-----------------------------------------------------------------------------
// Удаление пустой папки, имя которой задано char-строкой.
// Вначале выполняется проверка наличия папки.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - папка удалена или её не было.
_Check_return_ ZETFILE_API
long zfDeleteDirectoryA(_In_opt_ LPCSTR lpsName);

// Удаление пустой папки, имя которой задано wchar-строкой.
// Вначале выполняется проверка наличия папки.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - папка удалена или её не было.
_Check_return_ ZETFILE_API
long zfDeleteDirectoryW(_In_opt_ LPCWSTR lpwsName);

#ifdef UNICODE
#define  zfDeleteDirectory   zfDeleteDirectoryW
#else
#define  zfDeleteDirectory   zfDeleteDirectoryA
#endif
//-----------------------------------------------------------------------------
// Удаление пути (папок и файлов), имя которой задано char-строкой.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - все папки и файлы удалены или их не было.
_Check_return_ ZETFILE_API
long zfDeletePathA(_In_opt_ LPCSTR lpsName);

// Удаление пути (папок и файлов), имя которой задано wchar-строкой.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - все папки и файлы удалены или их не было.
_Check_return_ ZETFILE_API
long zfDeletePathW(_In_opt_ LPCWSTR lpwsName);

#ifdef UNICODE
#define  zfDeletePath   zfDeletePathW
#else
#define  zfDeletePath   zfDeletePathA
#endif
//-----------------------------------------------------------------------------
// Получение размера папки, имя которого задано char-строкой.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - размер определён.
_Check_return_ ZETFILE_API
long zfGetDirectorySizeInBytesA(_In_opt_ LPCSTR lpName, _Out_ ULONGLONG * pSize, _Out_ ULONGLONG * pSizeInDisk);

// Получение размера папки, имя которого задано wchar-строкой.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - размер определён.
_Check_return_ ZETFILE_API
long zfGetDirectorySizeInBytesW(_In_opt_ LPCWSTR lpwName, _Out_ ULONGLONG * pSize, _Out_ ULONGLONG * pSizeInDisk);

#ifdef UNICODE
#define  zfGetDirectorySizeInBytes   zfGetDirectorySizeInBytesW
#else
#define  zfGetDirectorySizeInBytes   zfGetDirectorySizeInBytesA
#endif
//-----------------------------------------------------------------------------
// Удаляет все файлы в папке с заданным расширением
//@param lpsDirectory - директория в которой будет происходить удаление файлов
//@param lpsExtension - расширение удаляемых файлов, задается в любом удобном виде (с точкой и без), если задана пустая строка удалит все файлы
//@param bIncludeSubdirectory - флаг что нужно удалить файлы из поддиректорий
//@return 0 если выполнено успешно
_Check_return_ ZETFILE_API
long zfDeleteFilesInDirectoryByExtensionA(_In_ LPCSTR lpDirectory, _In_ LPCSTR lpExtension, _In_opt_ bool bIncludeSubdirectory = false);
// Удаляет все файлы в папке с заданным расширением
//@param lpsDirectory - директория в которой будет происходить удаление файлов
//@param lpsExtension - расширение удаляемых файлов, задается в любом удобном виде (с точкой и без), если задана пустая строка удалит все файлы
//@param bIncludeSubdirectory - флаг что нужно удалить файлы из поддиректорий
//@return 0 если выполнено успешно
_Check_return_ ZETFILE_API
long zfDeleteFilesInDirectoryByExtensionW(_In_ LPCWSTR lpDirectory, _In_ LPCWSTR lpExtension, _In_opt_ bool bIncludeSubdirectory = false);

#ifdef UNICODE
#define  zfDeleteFilesInDirectoryByExtension  zfDeleteFilesInDirectoryByExtensionW
#else
#define  zfDeleteFilesInDirectoryByExtension  zfDeleteFilesInDirectoryByExtensionA
#endif
//-----------------------------------------------------------------------------
// Копирует папку и все ее содержимое по указанному пути в указанное место
//@param lpFolderDir	  - директория в которой находится копируемая папка
//@param lpDestinationDir - директория куда будет скопирована папка
//@return 0 если выполнено успешно, отриц число - код ошибки
ZETFILE_API long zfCopyFolderA(_In_ LPCSTR lpFolderDir, _In_ LPCSTR lpDestinationDir);

// Копирует папку и все ее содержимое по указанному пути в указанное место
//@param lpFolderDir	  - директория в которой находится копируемая папка
//@param lpDestinationDir - директория куда будет скопирована папка
//@return 0 если выполнено успешно, отриц число - код ошибки
ZETFILE_API long zfCopyFolderW(_In_ LPCWSTR lpFolderDir, _In_ LPCWSTR lpDestinationDir);

// Копирует папку и все ее содержимое по указанному пути в указанное место
#ifdef UNICODE
#define  zfCopyFolder  zfCopyFolderW
#else
#define  zfCopyFolder  zfCopyFolderA
#endif
//-----------------------------------------------------------------------------
// Проверка текстового файла пустой он или нет
//@param lpFileName	- имя файла
//@param bTextFile	- если true то файл проверяется как текстовый, в противном случае как бинарный
//@return 1 если пустой, 0 если не пустой, отриц число - код ошибки
ZETFILE_API long zfIsFileEmptyA(_In_ LPCSTR lpFileName, _In_opt_ bool bTextFile = true);

// Проверка текстового файла пустой он или нет
//@param lpFileName	- имя файла
//@param bTextFile	- если true то файл проверяется как текстовый, в противном случае как бинарный
//@return 1 если пустой, 0 если не пустой, отриц число - код ошибки
ZETFILE_API long zfIsFileEmptyW(_In_ LPCWSTR lpwsFileName, _In_opt_ bool bTextFile = true);

// Проверка текстового файла пустой он или нет
//@param lpFileName	- имя файла
//@param bTextFile	- если true то файл проверяется как текстовый, в противном случае как бинарный
//@return 1 если пустой, 0 если не пустой, отриц число - код ошибки

long ZETFILE_API zfWriteIntoFileFlushW(_In_ FILE *pFile, _In_ LPCWSTR lpwsText);
// Принудительная запись в файл pFile.
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - данные в файл принудительно записаны.

#ifdef UNICODE
#define  zfIsFileEmpty  zfIsFileEmptyW
#else
#define  zfIsFileEmpty  zfIsTextFileEmptyA
#endif
//-----------------------------------------------------------------------------