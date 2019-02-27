#pragma once

#ifdef ZETLOGGER_EXPORTS
#define ZETLOGGER_API __declspec(dllexport)
#else
#define ZETLOGGER_API __declspec(dllimport)
#endif
