// ---------------------------------------------------------------------
// Подключение к проекту MetrologicalCode.dll
// ---------------------------------------------------------------------
// В свойствах проекта для всех конфигураций в разделе
// "Компоновщик->Общие->Дополнительные каталоги библиотек"
// должна быть строка "$(ZetLibsDir)",
// что выполнено в подавляющем большинстве проектов ZetLab и ZetView.
// ---------------------------------------------------------------------
#pragma once
#include <MetrologicalCode/include/MetrologicalCode.h>

#define MetrologicalCode_NAME		"MetrologicalCode"
#define MetrologicalCode_LIB_EXT		".lib"

#ifndef DEBUG
#define MetrologicalCode_LIB_NAME	MetrologicalCode_NAME MetrologicalCode_LIB_EXT
#else
#define MetrologicalCode_LIB_NAME	MetrologicalCode_NAME "D" MetrologicalCode_LIB_EXT
#endif

#ifndef _WIN64
#define ARCHITECTURE_WIN	"Win32"
#else
#define ARCHITECTURE_WIN	"Win64"
#endif

#pragma comment(lib, MetrologicalCode_NAME "\\bin\\" ARCHITECTURE_WIN "\\" MetrologicalCode_LIB_NAME)
// ---------------------------------------------------------------------