// функции осциллографа
#ifdef EXPORTING_DLL
extern __declspec( dllexport ) long WINAPI ZbthExclusive1Open_Osc(long numberDSP, long Timeout);
extern __declspec( dllexport ) long WINAPI ZbthExclusive1Close_Osc(long numberDSP, long Timeout);
extern __declspec( dllexport ) long WINAPI ZbthExclusive2Open_Osc(long numberDSP, long Timeout);
extern __declspec( dllexport ) long WINAPI ZbthExclusive2Close_Osc(long numberDSP, long Timeout);
extern __declspec( dllexport ) long WINAPI ZbthLockCommands_Osc(long numberDSP);
extern __declspec( dllexport ) long WINAPI ZbthUnLockCommands_Osc(long numberDSP);
extern __declspec( dllexport ) long WINAPI ZbthSendCommand_Osc(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllexport ) long WINAPI ZbthReceiveCommand_Osc(long numberDSP, long Timeout, ADC_INFO_EXT *Data);

extern __declspec( dllexport ) long WINAPI ZbthOpen_Osc(long numberDSP, long OpenTimeout, long CommandTimeout, long ServerMode);
extern __declspec( dllexport ) long WINAPI ZbthClose_Osc(long numberDSP, long Timeout);
extern __declspec( dllexport ) long WINAPI ZbthGetReady_Osc(long numberDSP);
//extern __declspec( dllexport ) long WINAPI ZbthGetReadyExt_Osc(long numberDSP, SOCKADDR_BTH* BthAddress, long* BthPort);
// ZbthGetReady возвращает:
//	0 - нет ошибок
//	1 - не открыто и не открывается
//	2 - открывается, но связь еще не установлена
//	3 - открывается, связь с командным сокетом установлена
//	4,5,6 Связь нарушена
// 0x100 - неправильный numberDSP
extern __declspec( dllexport ) long WINAPI ZbthStopDSP_Osc(long numberDSP, long Timeout);

extern __declspec( dllexport ) long WINAPI ZbthGetData_Osc(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllexport ) long WINAPI ZbthPutData_Osc(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllexport ) long WINAPI ZbthGetInfo_Osc(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllexport ) long WINAPI ZbthPutInfo_Osc(long numberDSP, long Timeout, ADC_INFO_EXT *Data);

extern __declspec( dllexport ) long WINAPI ZbthBufferSize_Osc(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllexport ) long WINAPI ZbthBuffer_Osc(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllexport ) long WINAPI ZbthRemBuffer_Osc(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllexport ) long WINAPI ZbthStart_Osc(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllexport ) long WINAPI ZbthStop_Osc(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllexport ) long WINAPI ZbthFlag_Osc(long numberDSP, long Timeout, ADC_INFO_EXT *Data);

extern __declspec( dllexport ) long WINAPI ZbthSetDigOutEnable_Osc(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllexport ) long WINAPI ZbthGetDigInput_Osc(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllexport ) long WINAPI ZbthSetDigOutput_Osc(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
#else
extern __declspec( dllimport ) long WINAPI ZbthExclusive1Open_Osc(long numberDSP, long Timeout);
extern __declspec( dllimport ) long WINAPI ZbthExclusive1Close_Osc(long numberDSP, long Timeout);
extern __declspec( dllimport ) long WINAPI ZbthExclusive2Open_Osc(long numberDSP, long Timeout);
extern __declspec( dllimport ) long WINAPI ZbthExclusive2Close_Osc(long numberDSP, long Timeout);
extern __declspec( dllimport ) long WINAPI ZbthLockCommands_Osc(long numberDSP);
extern __declspec( dllimport ) long WINAPI ZbthUnLockCommands_Osc(long numberDSP);
extern __declspec( dllimport ) long WINAPI ZbthSendCommand_Osc(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllimport ) long WINAPI ZbthReceiveCommand_Osc(long numberDSP, long Timeout, ADC_INFO_EXT *Data);

extern __declspec( dllimport ) long WINAPI ZbthOpen_Osc(long numberDSP, long OpenTimeout, long CommandTimeout, long ServerMode);
extern __declspec( dllimport ) long WINAPI ZbthClose_Osc(long numberDSP, long Timeout);
extern __declspec( dllimport ) long WINAPI ZbthGetReady_Osc(long numberDSP);
//extern __declspec( dllimport ) long WINAPI ZbthGetReadyExt_Osc(long numberDSP, SOCKADDR_BTH* BthAddress, long* BthPort);
// ZbthGetReady возвращает:
//	0 - нет ошибок
//	1 - не открыто и не открывается
//	2 - открывается, но связь еще не установлена
//	3 - открывается, связь с командным сокетом установлена
//	4,5,6 Связь нарушена
// 0x100 - неправильный numberDSP

extern __declspec( dllimport ) long WINAPI ZbthStopDSP_Osc(long numberDSP, long Timeout);

extern __declspec( dllimport ) long WINAPI ZbthGetData_Osc(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllimport ) long WINAPI ZbthPutData_Osc(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllimport ) long WINAPI ZbthGetInfo_Osc(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllimport ) long WINAPI ZbthPutInfo_Osc(long numberDSP, long Timeout, ADC_INFO_EXT *Data);

extern __declspec( dllimport ) long WINAPI ZbthBufferSize_Osc(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllimport ) long WINAPI ZbthBuffer_Osc(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllimport ) long WINAPI ZbthRemBuffer_Osc(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllimport ) long WINAPI ZbthStart_Osc(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllimport ) long WINAPI ZbthStop_Osc(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllimport ) long WINAPI ZbthFlag_Osc(long numberDSP, long Timeout, ADC_INFO_EXT *Data);

extern __declspec( dllimport ) long WINAPI ZbthSetDigOutEnable_Osc(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllimport ) long WINAPI ZbthGetDigInput_Osc(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
extern __declspec( dllimport ) long WINAPI ZbthSetDigOutput_Osc(long numberDSP, long Timeout, ADC_INFO_EXT *Data);
#endif