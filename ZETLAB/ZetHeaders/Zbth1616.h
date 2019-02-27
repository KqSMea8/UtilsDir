// File: Zbth1616.h
//
#ifndef INDLL_H
#define INDLL_H

#ifdef EXPORTING_DLL
extern __declspec( dllexport ) long WINAPI ZbthExclusive1Open_1616(long numberDSP, long Timeout);
extern __declspec( dllexport ) long WINAPI ZbthExclusive1Close_1616(long numberDSP, long Timeout);
extern __declspec( dllexport ) long WINAPI ZbthLockCommands_1616(long numberDSP);
extern __declspec( dllexport ) long WINAPI ZbthUnLockCommands_1616(long numberDSP);
extern __declspec( dllexport ) long WINAPI ZbthSendCommand_1616(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllexport ) long WINAPI ZbthReceiveCommand_1616(long numberDSP, long Timeout, ADC_INFO_EXT *Data);

extern __declspec( dllexport ) long WINAPI ZbthOpen_1616(long numberDSP, long OpenTimeout, long CommandTimeout, long ServerMode);
extern __declspec( dllexport ) long WINAPI ZbthClose_1616(long numberDSP, long Timeout);
extern __declspec( dllexport ) long WINAPI ZbthGetReady_1616(long numberDSP);
//extern __declspec( dllexport ) long WINAPI ZbthGetReadyExt_1616(long numberDSP, SOCKADDR_BTH* BthAddress, long* BthPort);
// ZbthGetReady возвращает:
//	0 - нет ошибок
//	1 - не открыто и не открывается
//	2 - открывается, но связь еще не установлена
//	3 - открывается, связь с командным сокетом установлена
//	4,5,6 Связь нарушена
// 0x100 - неправильный numberDSP
extern __declspec( dllexport ) long WINAPI ZbthStopDSP_1616(long numberDSP, long Timeout);

extern __declspec( dllexport ) long WINAPI ZbthGetData_1616(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllexport ) long WINAPI ZbthPutData_1616(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllexport ) long WINAPI ZbthGetInfo_1616(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllexport ) long WINAPI ZbthPutInfo_1616(long numberDSP, long Timeout, ADC_INFO_EXT *Data);

extern __declspec( dllexport ) long WINAPI ZbthBufferSize_1616(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllexport ) long WINAPI ZbthBuffer_1616(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllexport ) long WINAPI ZbthRemBuffer_1616(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllexport ) long WINAPI ZbthStart_1616(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllexport ) long WINAPI ZbthStop_1616(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllexport ) long WINAPI ZbthFlag_1616(long numberDSP, long Timeout, ADC_INFO_EXT *Data);

extern __declspec( dllexport ) long WINAPI ZbthSetDigOutEnable_1616(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllexport ) long WINAPI ZbthGetDigInput_1616(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllexport ) long WINAPI ZbthSetDigOutput_1616(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
#else
extern __declspec( dllimport ) long WINAPI ZbthExclusive1Open_1616(long numberDSP, long Timeout);
extern __declspec( dllimport ) long WINAPI ZbthExclusive1Close_1616(long numberDSP, long Timeout);
extern __declspec( dllimport ) long WINAPI ZbthLockCommands_1616(long numberDSP);
extern __declspec( dllimport ) long WINAPI ZbthUnLockCommands_1616(long numberDSP);
extern __declspec( dllimport ) long WINAPI ZbthSendCommand_1616(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllimport ) long WINAPI ZbthReceiveCommand_1616(long numberDSP, long Timeout, ADC_INFO_EXT *Data);

extern __declspec( dllimport ) long WINAPI ZbthOpen_1616(long numberDSP, long OpenTimeout, long CommandTimeout, long ServerMode);
extern __declspec( dllimport ) long WINAPI ZbthClose_1616(long numberDSP, long Timeout);
extern __declspec( dllimport ) long WINAPI ZbthGetReady_1616(long numberDSP);
//extern __declspec( dllimport ) long WINAPI ZbthGetReadyExt_1616(long numberDSP, SOCKADDR_BTH* BthAddress, long* BthPort);
// ZbthGetReady возвращает:
//	0 - нет ошибок
//	1 - не открыто и не открывается
//	2 - открывается, но связь еще не установлена
//	3 - открывается, связь с командным сокетом установлена
//	4,5,6 Связь нарушена
// 0x100 - неправильный numberDSP

extern __declspec( dllimport ) long WINAPI ZbthStopDSP_1616(long numberDSP, long Timeout);

extern __declspec( dllimport ) long WINAPI ZbthGetData_1616(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllimport ) long WINAPI ZbthPutData_1616(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllimport ) long WINAPI ZbthGetInfo_1616(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllimport ) long WINAPI ZbthPutInfo_1616(long numberDSP, long Timeout, ADC_INFO_EXT *Data);

extern __declspec( dllimport ) long WINAPI ZbthBufferSize_1616(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllimport ) long WINAPI ZbthBuffer_1616(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllimport ) long WINAPI ZbthRemBuffer_1616(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllimport ) long WINAPI ZbthStart_1616(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllimport ) long WINAPI ZbthStop_1616(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllimport ) long WINAPI ZbthFlag_1616(long numberDSP, long Timeout, ADC_INFO_EXT *Data);

extern __declspec( dllimport ) long WINAPI ZbthSetDigOutEnable_1616(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllimport ) long WINAPI ZbthGetDigInput_1616(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllimport ) long WINAPI ZbthSetDigOutput_1616(long numberDSP, long Timeout, ADC_INFO_EXT *Data);

#endif


#endif