// Поддерживаемые типы устройств
#define KD1610		0	// ADC 16/200
#define KD216		1	// APC 216
#define KD500		2	// ADC 16/500
#define KD500P		3	// ADC 16/500P
#define KD816		4	// ADC 816
#define KD1002		5	// ADC 1002
#define KDU216		6	// APC 216 USB
#define KD24		7	// ADC 24
#define KD1432		8	// ADC 1432
#define KDUACPB		9	// ACPB USB
#define KDU1616		10	// ZET210
#define KDUPD14		11	// PD14 USB
#define KDUVN		12	// ZET110
#define KDUOSC		13	// ZET302
#define KDU8500		14	// A17U8
#define KDU2500		15	// A17U2
#define KDU1624		16	// ZET220
#define KDU0424		17	// ZET230
#define KDU0414		18	// ZET
#define KDU0824		19	// ZET240


//Все функции возвращают код ошибки. 0 - нет ошибки, иначе - код ошибки

// ------------- Общие функции -------------
// подключиться к драйверу
__declspec(dllimport) long WINAPI ZOpen(long typeDevice, long numberDSP);
// отключиться от драйвера
__declspec(dllimport) long WINAPI ZClose(long typeDevice, long numberDSP);

// сброс и останов сигнальных процессоров (влияет на все DSP одного устройства)
__declspec(dllimport) long WINAPI ZResetDSP(long typeDevice, long numberDSP);
// проинициализировать сигнальный процессор
__declspec(dllimport) long WINAPI ZInitDSP(long typeDevice, long numberDSP, const char *fileName);
// получить серийный номер DSP
__declspec(dllimport) long WINAPI ZGetSerialNumberDSP(long typeDevice, long numberDSP, long *serialNumber);
// получить имя устройства (возвращаемая строка не более 16 символов вместе с завершающим 0, maxSizeStr - размер буфера strName[])
__declspec(dllimport) long WINAPI ZGetNameDevice(long typeDevice, long numberDSP, char *strName, long maxSizeStr);
// получить версии ПО (возвращаемые строки не более 64 символов вместе с завершающим 0)
__declspec(dllimport) long WINAPI ZGetVersion(long typeDevice, long numberDSP, char *verDSP, char *verDRV, char *verLIB);
// получить тип интерфейса (1 - PCI, 2 - USB, 3 - Ethernet)
__declspec(dllimport) long WINAPI ZGetTypeConnection(long typeDevice, long numberDSP, long *type);
// прочитать код ошибки
__declspec(dllimport) long WINAPI ZGetError(long typeDevice, long numberDSP, long *error);
// определить кол-во изменений параметров с момента загрузки
__declspec(dllimport) long WINAPI ZGetModify(long typeDevice, long numberDSP, long *modify);

// прочитать флаг - количество прерываний DSP (пакетов) с момента ZStartADC()
__declspec(dllimport) long WINAPI ZGetFlag(long typeDevice, long numberDSP, unsigned long *flag);

// прочитать статус синхронизации по внешней частоте (устаревшая функция, см соответствующие функции для АЦП и ЦАП)
__declspec(dllimport) long WINAPI ZGetEnableExtFreq(long typeDevice, long numberDSP, long *enable);
// вкл./выкл. синхронизации по внешней частоте (устаревшая функция, см соответствующие функции для АЦП и ЦАП)
__declspec(dllimport) long WINAPI ZSetEnableExtFreq(long typeDevice, long numberDSP, long enable);
// прочитать статус внешнего запуска (устаревшая функция, см соответствующие функции для АЦП и ЦАП)
__declspec(dllimport) long WINAPI ZGetEnableExtStart(long typeDevice, long numberDSP, long *enable);
// вкл./выкл. внешнего запуска (устаревшая функция, см соответствующие функции для АЦП и ЦАП)
__declspec(dllimport) long WINAPI ZSetEnableExtStart(long typeDevice, long numberDSP, long enable);

// запустить тест шлейфа для плат PnP
__declspec(dllimport) long WINAPI ZTestCode(long typeDevice, long numberDSP, long *code0, long *code1, long *code2);


// Запросить кол-во линий цифрового порта
__declspec(dllimport) long WINAPI ZGetQuantityChannelDigPort(long typeDevice, long numberDSP, long *quantityChannel);
// Запросить маску выходов порта
__declspec(dllimport) long WINAPI ZGetDigOutEnable(long typeDevice, long numberDSP, unsigned long *digitalOutEnableMask);
// Установить маску выходов порта
__declspec(dllimport) long WINAPI ZSetDigOutEnable(long typeDevice, long numberDSP, unsigned long digitalOutEnableMask);
// Установить линию цифрового порта на выход (без изменения состояния других линий, numberOfBit = 0..quantityChannel-1)
__declspec(dllimport) long WINAPI ZSetBitDigOutEnable(long typeDevice, long numberDSP, long numberOfBit);
// Установить маску линий цифрового порта выход (без изменения состояния других линий)
__declspec(dllimport) long WINAPI ZSetBitMaskDigOutEnable(long typeDevice, long numberDSP, unsigned long maskOfBits);
// Установить линию цифрового порта на вход (без изменения состояния других линий, numberOfBit = 0..quantityChannel-1)
__declspec(dllimport) long WINAPI ZClrBitDigOutEnable(long typeDevice, long numberDSP, long numberOfBit);
// Установить маску линий цифрового порта вход (без изменения состояния других линий)
__declspec(dllimport) long WINAPI ZClrBitMaskDigOutEnable(long typeDevice, long numberDSP, unsigned long maskOfBits);
// Прочитать данные со входов порта
__declspec(dllimport) long WINAPI ZGetDigInput(long typeDevice, long numberDSP, unsigned long *digitalInput);
// Прочитать данные, выдаваемые на выходы порта
__declspec(dllimport) long WINAPI ZGetDigOutput(long typeDevice, long numberDSP, unsigned long *digitalOutput);
// Записать данные в порт
__declspec(dllimport) long WINAPI ZSetDigOutput(long typeDevice, long numberDSP, unsigned long digitalOutput);
// Установить вывод цифрового порта в "1" (без изменения состояния других выводов, numberOfBit = 0..quantityChannel-1)
__declspec(dllimport) long WINAPI ZSetBitDigOutput(long typeDevice, long numberDSP, long numberOfBit);
// Установить маску выводов цифрового порта в "1" (без изменения состояния других выводов)
__declspec(dllimport) long WINAPI ZSetBitMaskDigOutput(long typeDevice, long numberDSP, unsigned long maskOfBits);
// Установить вывод цифрового порта в "0" (без изменения состояния других выводов, numberOfBit = 0..quantityChannel-1)
__declspec(dllimport) long WINAPI ZClrBitDigOutput(long typeDevice, long numberDSP, long numberOfBit);
// Установить маску выводов цифрового порта в "0" (без изменения состояния других выводов)
__declspec(dllimport) long WINAPI ZClrBitMaskDigOutput(long typeDevice, long numberDSP, unsigned long maskOfBits);
// Опросить режим потокового ввода-вывода (mode=0 - аналоговый порт, mode=1 - цифрового порт, mode=2 - UART)
										// +0x04 - Инвертированный UART
										// +0x08 - 9 бит UART
										// +0x10 - 2 стоп-бита UART (не исп.)
										// +0x20 - контроль четности UART (не исп.)
										// +0x40 - IrDA UART (не исп.)
__declspec(dllimport) long WINAPI ZGetDigitalMode(long typeDevice, long numberDSP, long *mode);
// Переключить режим потокового ввода-вывода (mode=0 - аналоговый порт, mode=1 - цифрового порт, mode=2 - UART)
__declspec(dllimport) long WINAPI ZSetDigitalMode(long typeDevice, long numberDSP, long mode);
// Опрос разрешения генерации сигналов синхронизации на цифровой порт  
__declspec(dllimport) long WINAPI ZGetMasterSynchr(long typeDevice, long numberDSP, long *enable);
// Разрешить генерацию сигналов синхронизации на цифровой порт  
__declspec(dllimport) long WINAPI ZSetMasterSynchr(long typeDevice, long numberDSP, long enable);


// проверить поддерживается ли модуль ШИМ
__declspec(dllimport) long WINAPI ZFindPWM(long typeDevice, long numberDSP, long* present);
// запуск каналов ШИМ
__declspec(dllimport) long WINAPI ZStartPWM(long typeDevice, long numberDSP, long Start0, long Start1, long Start2);
// останов каналов ШИМ
__declspec(dllimport) long WINAPI ZStopPWM(long typeDevice, long numberDSP, long Stop0, long Stop1, long Stop2);
// задать частоту ШИМ через коэф. деления опорной частоты и период
__declspec(dllimport) long WINAPI ZSetFreqPWM(long typeDevice, long numberDSP, long Rate, long Period);
// задать скважность и сдвиг каналов ШИМ
__declspec(dllimport) long WINAPI ZSetOnDutyPWM(long typeDevice, long numberDSP, long OnDutyPWM0, long OnDutyPWM1, long OnDutyPWM2, long ShiftPWM1, long ShiftPWM2);
// обмен массивом коэффициентов управления с DSP
__declspec(dllimport)long WINAPI ZRegulatorPWM(long typeDevice, long numberDSP, void *data, long *size);


// ------------- Парные функции для работы с АЦП или ЦАП -------------
// поддержка АЦП в текущем режиме DSP
__declspec(dllimport) long WINAPI ZGetEnableADC(long typeDevice, long numberDSP, long *enable);
// поддержка ЦАП в текущем режиме DSP
__declspec(dllimport) long WINAPI ZGetEnableDAC(long typeDevice, long numberDSP, long *enable);
// установить сигнальный процессор в режим АЦП/ЦАП
__declspec(dllimport) long WINAPI ZSetTypeADC(long typeDevice, long numberDSP);
__declspec(dllimport) long WINAPI ZSetTypeDAC(long typeDevice, long numberDSP);
// чтение в каком состоянии находится АЦП/ЦАП
__declspec(dllimport) long WINAPI ZGetStartADC(long typeDevice, long numberDSP, long *status);
__declspec(dllimport) long WINAPI ZGetStartDAC(long typeDevice, long numberDSP, long *status);
// запуск процедуры ввода/вывода сигнала
__declspec(dllimport) long WINAPI ZStartADC(long typeDevice, long numberDSP);
__declspec(dllimport) long WINAPI ZStartDAC(long typeDevice, long numberDSP);
// останов процедуры ввода/вывода
__declspec(dllimport) long WINAPI ZStopADC(long typeDevice, long numberDSP);
__declspec(dllimport) long WINAPI ZStopDAC(long typeDevice, long numberDSP);

// прочитать кол-во каналов АЦП/ЦАП
__declspec(dllimport) long WINAPI ZGetQuantityChannelADC(long typeDevice, long numberDSP, long *quantityChannel);
__declspec(dllimport) long WINAPI ZGetQuantityChannelDAC(long typeDevice, long numberDSP, long *quantityChannel);
// прочитать вес младшего разряда АЦП/ЦАП (устаревшая функция)
__declspec(dllimport) long WINAPI ZGetDigitalResolutionADC(long typeDevice, long numberDSP, double *digitalResolution);
__declspec(dllimport) long WINAPI ZGetDigitalResolutionDAC(long typeDevice, long numberDSP, double *digitalResolution);
// прочитать откалиброванный поканально вес младшего разряда АЦП/ЦАП
__declspec(dllimport) long WINAPI ZGetDigitalResolChanADC(long typeDevice, long numberDSP, long numberChannel, double *digitalResolChan);
__declspec(dllimport) long WINAPI ZGetDigitalResolChanDAC(long typeDevice, long numberDSP, long numberChannel, double *digitalResolChan);
// прочитать кол-во бит в одном отсчете АЦП/ЦАП
__declspec(dllimport) long WINAPI ZGetBitsADC(long typeDevice, long numberDSP, long *numberBits);
__declspec(dllimport) long WINAPI ZGetBitsDAC(long typeDevice, long numberDSP, long *numberBits);
// прочитать кол-во слов в одном отсчете АЦП/ЦАП
__declspec(dllimport) long WINAPI ZGetWordsADC(long typeDevice, long numberDSP, long *numberWords);
__declspec(dllimport) long WINAPI ZGetWordsDAC(long typeDevice, long numberDSP, long *numberWords);

// прочитать частоту дискретизации 
__declspec(dllimport) long WINAPI ZGetFreqADC(long typeDevice, long numberDSP, double *freq);
__declspec(dllimport) long WINAPI ZGetFreqDAC(long typeDevice, long numberDSP, double *freq);
// прочитать частоту из списка возможных частот дискретизации 
__declspec(dllimport) long WINAPI ZGetListFreqADC(long typeDevice, long numberDSP, long next, double *freq);
__declspec(dllimport) long WINAPI ZGetListFreqDAC(long typeDevice, long numberDSP, long next, double *freq);
// установить следующую из списка частоту дискретизации 
__declspec(dllimport) long WINAPI ZSetNextFreqADC(long typeDevice, long numberDSP, long next, double *freq);
__declspec(dllimport) long WINAPI ZSetNextFreqDAC(long typeDevice, long numberDSP, long next, double *freq);
// установить частоту дискретизации
__declspec(dllimport) long WINAPI ZSetFreqADC(long typeDevice, long numberDSP, double freqIn, double *freqOut);
__declspec(dllimport) long WINAPI ZSetFreqDAC(long typeDevice, long numberDSP, double freqIn, double *freqOut);

// прочитать значение текущей опорной частоты
__declspec(dllimport) long WINAPI ZGetExtFreqADC(long typeDevice, long numberDSP, double *oporFreq);
__declspec(dllimport) long WINAPI ZGetExtFreqDAC(long typeDevice, long numberDSP, double *oporFreq);
// установить значение внешней опорной частоты
__declspec(dllimport) long WINAPI ZSetExtFreqADC(long typeDevice, long numberDSP, double extFreq);
__declspec(dllimport) long WINAPI ZSetExtFreqDAC(long typeDevice, long numberDSP, double extFreq);
// прочитать статус синхронизации по внешней частоте 
__declspec(dllimport) long WINAPI ZGetEnableExtFreqADC(long typeDevice, long numberDSP, long *enable);
__declspec(dllimport) long WINAPI ZGetEnableExtFreqDAC(long typeDevice, long numberDSP, long *enable);
// вкл./выкл. синхронизации по внешней частоте
__declspec(dllimport) long WINAPI ZSetEnableExtFreqADC(long typeDevice, long numberDSP, long enable);
__declspec(dllimport) long WINAPI ZSetEnableExtFreqDAC(long typeDevice, long numberDSP, long enable);
// прочитать статус внешнего запуска
__declspec(dllimport) long WINAPI ZGetEnableExtStartADC(long typeDevice, long numberDSP, long *enable);
__declspec(dllimport) long WINAPI ZGetEnableExtStartDAC(long typeDevice, long numberDSP, long *enable);
// вкл./выкл. внешнего запуска
__declspec(dllimport) long WINAPI ZSetEnableExtStartADC(long typeDevice, long numberDSP, long enable);
__declspec(dllimport) long WINAPI ZSetEnableExtStartDAC(long typeDevice, long numberDSP, long enable);

// определить установленный размер буфера при прерывании
__declspec(dllimport) long WINAPI ZGetInterruptADC(long typeDevice, long numberDSP, long *size);
__declspec(dllimport) long WINAPI ZGetInterruptDAC(long typeDevice, long numberDSP, long *size);
// определить макс. возможный размер буфера при прерывании
__declspec(dllimport) long WINAPI ZGetMaxInterruptADC(long typeDevice, long numberDSP, long *size);
__declspec(dllimport) long WINAPI ZGetMaxInterruptDAC(long typeDevice, long numberDSP, long *size);
// установить размер буфера для перекачки при прерывании
__declspec(dllimport) long WINAPI ZSetInterruptADC(long typeDevice, long numberDSP, long size);
__declspec(dllimport) long WINAPI ZSetInterruptDAC(long typeDevice, long numberDSP, long size);

// определить установленный размер пакета данных DSP
__declspec(dllimport) long WINAPI ZGetSizePacketADC(long typeDevice, long numberDSP, long *size);
__declspec(dllimport) long WINAPI ZGetSizePacketDAC(long typeDevice, long numberDSP, long *size);
// определить макс. возможный размер пакета данных DSP
__declspec(dllimport) long WINAPI ZGetMaxSizePacketADC(long typeDevice, long numberDSP, long *size);
__declspec(dllimport) long WINAPI ZGetMaxSizePacketDAC(long typeDevice, long numberDSP, long *size);
// установить размер пакета данных DSP
__declspec(dllimport) long WINAPI ZSetSizePacketADC(long typeDevice, long numberDSP, long size);
__declspec(dllimport) long WINAPI ZSetSizePacketDAC(long typeDevice, long numberDSP, long size);

// определить установленное кол-во пакетов за одно прерывание
__declspec(dllimport) long WINAPI ZGetQuantityPacketsADC(long typeDevice, long numberDSP, long *size);
__declspec(dllimport) long WINAPI ZGetQuantityPacketsDAC(long typeDevice, long numberDSP, long *size);
// определить макс. возможное кол-во пакетов за одно прерывание
__declspec(dllimport) long WINAPI ZGetMaxQuantityPacketsADC(long typeDevice, long numberDSP, long *size);
__declspec(dllimport) long WINAPI ZGetMaxQuantityPacketsDAC(long typeDevice, long numberDSP, long *size);
// установить кол-во пакетов за одно прерывание
__declspec(dllimport) long WINAPI ZSetQuantityPacketsADC(long typeDevice, long numberDSP, long size);
__declspec(dllimport) long WINAPI ZSetQuantityPacketsDAC(long typeDevice, long numberDSP, long size);

// установить однократное или циклическое накопление в буфер ОЗУ ПК 
__declspec(dllimport) long WINAPI ZSetCycleSampleADC(long typeDevice, long numberDSP, long enable);
__declspec(dllimport) long WINAPI ZSetCycleSampleDAC(long typeDevice, long numberDSP, long enable);

// задать размер буфера АЦП в ОЗУ ПК
__declspec(dllimport) long WINAPI ZSetBufferSizeADC(long typeDevice, long numberDSP, long size);
__declspec(dllimport) long WINAPI ZSetBufferSizeDAC(long typeDevice, long numberDSP, long size);
// запросить буфер в ОЗУ ПК
__declspec(dllimport) long WINAPI ZGetBufferADC(long typeDevice, long numberDSP, void **buffer, long *size);
__declspec(dllimport) long WINAPI ZGetBufferDAC(long typeDevice, long numberDSP, void **buffer, long *size);
// освободить буфер в ОЗУ ПК
__declspec(dllimport) long WINAPI ZRemBufferADC(long typeDevice, long numberDSP, void **buffer);
__declspec(dllimport) long WINAPI ZRemBufferDAC(long typeDevice, long numberDSP, void **buffer);
// чтение указателя буфера в ОЗУ ПК
__declspec(dllimport) long WINAPI ZGetPointerADC(long typeDevice, long numberDSP, long *pointer);
__declspec(dllimport) long WINAPI ZGetPointerDAC(long typeDevice, long numberDSP, long *pointer);

// опрос количества включенных каналов
__declspec(dllimport) long WINAPI ZGetNumberInputADC(long typeDevice, long numberDSP, long *workChannels);
__declspec(dllimport) long WINAPI ZGetNumberOutputDAC(long typeDevice, long numberDSP, long *workChannels);
// опрос включен ли заданный канал
__declspec(dllimport) long WINAPI ZGetInputADC(long typeDevice, long numberDSP, long numberChannel, long *enable);
__declspec(dllimport) long WINAPI ZGetOutputDAC(long typeDevice, long numberDSP, long numberChannel, long *enable);
// включить/выключить заданный канал
__declspec(dllimport) long WINAPI ZSetInputADC(long typeDevice, long numberDSP, long numberChannel, long enable);
__declspec(dllimport) long WINAPI ZSetOutputDAC(long typeDevice, long numberDSP, long numberChannel, long enable);


// ------------- Функции для работы только с АЦП -------------
// получить оставшиеся данные АЦП
__declspec(dllimport) long WINAPI ZGetLastDataADC(long typeDevice, long numberDSP, long numberChannel, void *buffer, long size);

// прочитать коэф. усиления
__declspec(dllimport) long WINAPI ZGetAmplifyADC(long typeDevice, long numberDSP, long numberChannel, double *amplify);
// прочитать коэф. усиления из списка возможных коэф. усиления
__declspec(dllimport) long WINAPI ZGetListAmplifyADC(long typeDevice, long numberDSP, long next, double *amplify);
// установить следующий коэф. усиления
__declspec(dllimport) long WINAPI ZSetNextAmplifyADC(long typeDevice, long numberDSP, long numberChannel, long next, double *amplify);
// установить коэф. усиления
__declspec(dllimport) long WINAPI ZSetAmplifyADC(long typeDevice, long numberDSP, long numberChannel, double amplifyIn, double *amplifyOut);

// прочитать коэф. усиления ПУ8/10
__declspec(dllimport) long WINAPI ZGetPreAmplifyADC(long typeDevice, long numberDSP, long numberChannel, double *amplify);
// прочитать коэф. усиления из списка возможных коэф. усиления ПУ8/10
__declspec(dllimport) long WINAPI ZGetListPreAmplifyADC(long typeDevice, long numberDSP, long next, double *amplify);
// установить следующий коэф. усиления ПУ8/10
__declspec(dllimport) long WINAPI ZSetNextPreAmplifyADC(long typeDevice, long numberDSP, long numberChannel, long next, double *amplify);
// установить коэф. усиления ПУ8/10
__declspec(dllimport) long WINAPI ZSetPreAmplifyADC(long typeDevice, long numberDSP, long numberChannel, double amplifyIn, double *amplifyOut);

// проверить поддерживается и подключен ли модуль HCP
__declspec(dllimport) long WINAPI ZFindHCPADC(long typeDevice, long numberDSP, long *present);
// прочитать о состоянии заданного канала модуля HCP
__declspec(dllimport) long WINAPI ZGetHCPADC(long typeDevice, long numberDSP, long numberChannel, long *enable);
// установить состояние заданного канала модуля HCP
__declspec(dllimport) long WINAPI ZSetHCPADC(long typeDevice, long numberDSP, long numberChannel, long enable);

// опрос дифференциального режима заданного канала для ввода 
__declspec(dllimport) long WINAPI ZGetInputDiffADC(long typeDevice, long numberDSP, long numberChannel, long *enable);
// установить-сбросить заданный канал для ввода в дифференциальный режим
__declspec(dllimport) long WINAPI ZSetInputDiffADC(long typeDevice, long numberDSP, long numberChannel, long enable);


// ------------- Функции для работы только с ЦАП -------------
// установить внешнюю (из памяти ПК) или внутреннюю (только из памяти DSP) загрузку в ЦАП
__declspec(dllimport) long WINAPI ZSetExtCycleDAC(long typeDevice, long numberDSP, long enable);

// опросить поддерживается ли программный аттенюатор
__declspec(dllimport) long WINAPI ZFindSoftAtten(long typeDevice, long numberDSP, long *present);
// прочитать коф. затухания аттенюатора
__declspec(dllimport) long WINAPI ZGetAttenDAC(long typeDevice, long numberDSP, long numberChannel, double *reduction);
// установить коф. затухания аттенюатора
__declspec(dllimport) long WINAPI ZSetAttenDAC(long typeDevice, long numberDSP, long numberChannel, double reductionIn, double *reductionOut);

// опрос максимального размера буфера ЦАП в DSP
__declspec(dllimport) long WINAPI ZGetMaxSizeBufferDSPDAC(long typeDevice, long numberDSP, long *size);
// опрос размера буфера ЦАП в DSP
__declspec(dllimport) long WINAPI ZGetSizeBufferDSPDAC(long typeDevice, long numberDSP, long *size);
// установить размер буфера ЦАП в DSP
__declspec(dllimport) long WINAPI ZSetSizeBufferDSPDAC(long typeDevice, long numberDSP, long size);




