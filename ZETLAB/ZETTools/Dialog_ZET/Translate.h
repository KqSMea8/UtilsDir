#pragma once
#include <ZetDictionary/include/ZetDictionary.h>

#ifndef _TRANSLATE
#define _TRANSLATE
#endif

#define DICTIONARY_NAME		"ZetDictionary"
#define DICTIONARY_LIB_EXT	".lib"

/* Путь к библиотеке */
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

// Задание языков в словаре:
// язык исходных фраз - русский;
// язык выходных фраз - из локали.
void InitLanguageDst();

// Эту функцию необходимо вызвать один раз в конструкторе главного окна программы.
// Задание языков в словаре:
// язык исходных фраз - русский;
// язык выходных фраз - из локалию.
// Задание параметра перевода _exeName:
// если _exeName = L"", то параметр - это имя программы.
void InitZetDictionary(_In_ const CString & _exeName = L"");

// Перевод фразы. Языки перевода следует задавать заранее.
// _String - переводимая фраза;
// sParam - параметр перевода (если не задан, то имя программы).
// Возвращаемая строка: если в словаре есть исходная строка,
// то - переведенная строка,
// иначе - исходная строка.
CString TranslateString(_In_ const CString & _String, _In_opt_ const CString & sParam = L"");

// Перевод фразы из ресурсов. Языки перевода следует задавать заранее.
// nID - дескриптор переводимой фраза в ресурсах.
// sParam - параметр перевода (если не задан, то имя программы).
// Параметр перевода - имя программы.
// Возвращаемая строка: если в словаре есть исходная строка
// то - переведенная строка,
// иначе - исходная строка.
// ВНИМАНИЕ!!! В ресурсах не может быть WCHAR.
CString TranslateResourse(_In_ const UINT & nID, _In_opt_ const CString & sParam = L"");

// Принудительное копирование непереведенных фраз из буфера словаря в текстовый файл.
// Функция предназначена для программ, которые закрываются с помощью exit(0).
void ExitProgramm(_In_ const int code);

// Перевод фразы в которой используются единицы измерения. Языки перевода следует задавать заранее.
// _String - переводимая фраза;
// sParam - параметр перевода (если не задан, то имя программы).
// Возвращаемая строка: если в словаре есть исходная строка и она имеет единицы измерения,
// то - разделяем и переводим строку,
// иначе - возвращаем исходную строку.
CString TranslateDividedString(_In_ const CString & _String, _In_opt_ const CString & sParam = L"");

// Функция для получения языка программы.
CString GetLanguageDst();

// Функция установки языка программы.
void SetLanguageSrc(CString newVal);

CString GetLanguageSrc();

#endif	// #ifdef UNICODE