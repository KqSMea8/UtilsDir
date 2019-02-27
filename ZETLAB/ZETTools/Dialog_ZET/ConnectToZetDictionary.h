// ---------------------------------------------------------------------
// Подключение к проекту ZettDictionary.dll
// ---------------------------------------------------------------------
// В свойствах проекта для всех конфигураций в разделе
// "Компоновщик->Общие->Дополнительные каталоги библиотек"
// должна быть строка "$(ZetLibsDir)",
// что выполнено в подавляющем большинстве проектов ZetLab и ZetView.
// ---------------------------------------------------------------------
#pragma once
#include <ZetDictionary/include/ZetDictionary.h>

#define ZetDictionary_NAME		"ZetDictionary"
#define ZetDictionary_LIB_EXT	".lib"

#ifndef DEBUG
#define ZetDictionary_LIB_NAME	ZetDictionary_NAME ZetDictionary_LIB_EXT
#else
#define ZetDictionary_LIB_NAME	ZetDictionary_NAME "D" ZetDictionary_LIB_EXT
#endif

#ifndef _WIN64
#define ARCHITECTURE_WIN	"Win32"
#else
#define ARCHITECTURE_WIN	"Win64"
#endif

#pragma comment(lib, ZetDictionary_NAME "\\bin\\" ARCHITECTURE_WIN "\\" ZetDictionary_LIB_NAME)
// ---------------------------------------------------------------------