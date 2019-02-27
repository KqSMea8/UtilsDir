// ---------------------------------------------------------------------
// Подключение к проекту ZetTools.dll
// ---------------------------------------------------------------------
// В свойствах проекта для всех конфигураций в разделе
// "Компоновщик->Общие->Дополнительные каталоги библиотек"
// должна быть строка "$(ZetLibsDir)",
// что выполнено в подавляющем большинстве проектов ZetLab и ZetView.
// ---------------------------------------------------------------------
#pragma once
#include <ZetTools/include/ZetTools.h>

#define ZetTools_NAME		"ZetTools"
#define ZetTools_LIB_EXT	".lib"

#ifndef DEBUG
#define ZetTools_LIB_NAME	ZetTools_NAME ZetTools_LIB_EXT
#else
#define ZetTools_LIB_NAME	ZetTools_NAME "D" ZetTools_LIB_EXT
#endif

#ifndef _WIN64
#define ARCHITECTURE_WIN	"Win32"
#else
#define ARCHITECTURE_WIN	"Win64"
#endif

#pragma comment(lib, ZetTools_NAME "\\bin\\" ARCHITECTURE_WIN "\\" ZetTools_LIB_NAME)
// ---------------------------------------------------------------------