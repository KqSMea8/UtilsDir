//-----------------------------------------------------------------------------
// MetrologicalCode.h: главный файл заголовка для DLL MetrologicalCode
//
//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
#ifdef METROLOGICALCODE_EXPORTS
#define METROLOGICAL_CODE_API __declspec(dllexport)
#else
#define METROLOGICAL_CODE_API __declspec(dllimport)
#endif
//-----------------------------------------------------------------------------
METROLOGICAL_CODE_API long zmGetMetrologicalCode(const long code);
//-----------------------------------------------------------------------------