// Поддерживаемые типы устройств
#define KD1610	0	// ADC 16/200
#define KD216	1	// APC 216
#define KD500	2	// ADC 16/500
#define KD500P	3	// ADC 16/500P
#define KD816	4	// ADC 816
#define KD1002	5	// ADC 1002
#define KDU216	6	// APC 216 USB
#define KD24	7	// ADC 24
#define KD1432	8	// ADC 1432
#define KDUACPB	9	// ACPB USB
#define KDU1616	10	// ZET210
#define KDUPD14	11	// PD14 USB
#define KDUVN	12	// ZET110
#define KDUOSC	13	// ZET302
#define KDU8500	14	// A17U8
#define KDU2500	15	// A17U2
#define KDU1624	16	// ZET220
#define KDU0424	17	// ZET230
#define KDU0414	18	// ZET048
#define KDU0824	19	// ZET240

#define ZET_DEVICE_TYPE_COUNT 20
#define ZET_DEFINE_DEVICE_NAMES(name) \
static const TCHAR *name[ZET_DEVICE_TYPE_COUNT] = { \
	_T("ADC 16/200"), \
	_T("APC 216"), \
	_T("ADC 16/500"), \
	_T("ADC 16/500P"), \
	_T("ADC 816"), \
	_T("ADC 1002"), \
	_T("APC 216 USB"), \
	_T("ADC 24"), \
	_T("ADC 1432"), \
	_T("ACPB USB"), \
	_T("ZET210"), \
	_T("PD14 USB"), \
	_T("ZET110"), \
	_T("ZET302"), \
	_T("A17U8"), \
	_T("A17U2"), \
	_T("ZET220"), \
	_T("ET230"), \
	_T("ZET048"), \
	_T("ZET240"), \
}

//Все функции возвращают код ошибки. 0 - нет ошибки, иначе - код ошибки

// ------------- Функции для работы через LAN -------------
// Пропинговать IP-адрес по LAN
long WINAPI ZnetPing(char *strIP, long *Timeout);

// соединиться с устройством по LAN
long WINAPI ZnetOpen(long typeDevice, long numberDSP, long OpenTimeout, long CommandTimeout, char *AddressIP, long PortIP);
// разорвать соединение с устройством через LAN
long WINAPI ZnetClose(long typeDevice, long numberDSP, long Timeout);
// Проверить установлено ли соединение с устройством через LAN
long WINAPI ZnetGetReady(long typeDevice, long numberDSP);
// Проверить установлено ли соединение с устройством через LAN с опросом IP-адреса (AddressIP[16])
long WINAPI ZnetGetReadyExt(long typeDevice, long numberDSP, char *AddressIP, long *PortIP);
// ZnetGetReady возвращает:
//	0 - нет ошибок
//	1 - не открыто и не открывается
//	2 - открывается, но связь еще не установлена
//	3 - открывается, связь с командным сокетом установлена
//	4,5,6 Связь нарушена
// 0x100 - неправильный numberDSP

// Запустить поток сканирования устройств в сети.
// Функция func будет вызываться из отдельного потока.
// Для остановки сканирования и закрытия потока нужно вызвать функцию с параметром func = NULL.
// Значения status:
// -1 - устройство пропало (перестало рассылать пакеты)
//  0 - устройство доступно для подключения
//  1 - устройство уже имеет подключение (в этом случае peer_ip может содержать адрес того, кто к нему подключился)
long WINAPI ZnetScan(long typeDevice,
					 void (WINAPI *func)(long serial, long status, const WCHAR *label, const char *ip, long port, const char *peer_ip, void *arg),
					 void *arg);

// ------------- Функции эксклюзивных режимов -------------
// Перейти в эксклюзивный режим №1
//(заблокирован только доступ к устройству, открытие и выполнение функций драйвера - разрешен)
long WINAPI ZExclusive1Open(long typeDevice, long numberDSP);
// Выйти из эксклюзивного режима №1
long WINAPI ZExclusive1Close(long typeDevice, long numberDSP);
// Перейти в эксклюзивный режим №2
//(заблокирован и к устройству и к драйверу, т.е. устройство становится скрытым)
long WINAPI ZExclusive2Open(long typeDevice, long numberDSP);
// Выйти из эксклюзивного режима №2
long WINAPI ZExclusive2Close(long typeDevice, long numberDSP);
// послать команду DSP (size - кратен 512)
long WINAPI ZSendCommand(long typeDevice, long numberDSP, void *buffer, long *size);
// принять команду DSP (size - кратен 512)
long WINAPI ZReceiveCommand(long typeDevice, long numberDSP, void *buffer, long *size);
// послать данные ЦАП (size - кратен 512)	Сделано только для USB !!!
long WINAPI ZSendDataDAC(long typeDevice, long numberDSP, void *buffer, long *size);
// принять данные АЦП (size - кратен 512)	Сделано только для USB !!!
long WINAPI ZReceiveDataADC(long typeDevice, long numberDSP, void *buffer, long *size);

// ------------- Функции настройки TCP/IP -------------
// Прочитать настройки Ethernet устройства для TCP/IP из ПЗУ
long WINAPI ZReadSettingsEthernet(long typeDevice, long numberDSP, unsigned char *sourceIPAdr,
								  unsigned short *sourcePort0, unsigned char *subnetMask,
								  unsigned char *gatewayIPAdr, unsigned char *sourceMACAdr,
								  unsigned char *duplex, unsigned char *speed100Mb,
								  unsigned short *TimeoutWDOG, unsigned short *Reserved);

// Сохранить настройки Ethernet устройства для TCP/IP в ПЗУ
long WINAPI ZSaveSettingsEthernet(long typeDevice, long numberDSP, unsigned char *sourceIPAdr,
								  unsigned short *sourcePort0, unsigned char *subnetMask,
								  unsigned char *gatewayIPAdr, unsigned char *sourceMACAdr,
								  unsigned char *duplex, unsigned char *speed100Mb,
								  unsigned short TimeoutWDOG, unsigned short Reserved);

// ------------- Функции настройки PTP -------------
// Прочитать настройки PTP из ПЗУ
long WINAPI ZReadSettingsPTP(long typeDevice, long numberDSP,
							 unsigned char *enableMaster, unsigned char *enableSlave, unsigned char *domain,
							 unsigned char *absolutePriority, unsigned char *relativePriority);

// Сохранить настройки PTP в ПЗУ
long WINAPI ZSaveSettingsPTP(long typeDevice, long numberDSP,
							 unsigned char enableMaster, unsigned char enableSlave, unsigned char domain,
							 unsigned char absolutePriority, unsigned char relativePriority);

// ------------- Функции TEDS -------------
// проверить поддерживается ли модуль TEDS
long WINAPI ZFindTEDS(long typeDevice, long numberDSP, long* present);
// Переключить заданный канал в режим 1Wire (TEDS) и открыть EEPROM, в буфер серийный номер EEPROM
// ZOpenTEDS возвращает:
//	0 - нет ошибок
//	2 - Нет ответа от контроллера 1-Wire
//	3 - Ошибка обмена с контроллером 1-Wire
//	4 - 1-Wire закарочена
//	5 - alarming presence pulse (неправильная работа или подключено устройство другого интерфейса)
//	6 - no presence pulse (нет ответа от TEDS)
//	7-9 - Нет ответа от контроллера 1-Wire
// 0x100 - неправильный numberDSP
// 0x200 - неправильный номер канала
// 0x300 - модуль TEDS не поддерживается
long WINAPI ZOpenTEDS(long typeDevice, long numberDSP, long numberChannel, void *pBuffer);

//  Выключить режим 1Wire (TEDS)
long WINAPI ZCloseTEDS(long typeDevice, long numberDSP);
// Прочитать Size = 1-8 байт, начиная с адреса Address
// ZReadTEDS возвращает:
//	0 - нет ошибок
//	2 - Нет ответа от контроллера 1-Wire
//	3 - Ошибка обмена с контроллером 1-Wire
//	4 - 1-Wire закарочена
//	5 - alarming presence pulse
//	6 - no presence pulse (нет ответа от TEDS)
//	7-12 - Нет ответа от контроллера 1-Wire
// 0x100 - неправильный numberDSP
// 0x200 - неправильный номер канала
// 0x300 - модуль TEDS не поддерживается
long WINAPI ZReadTEDS(long typeDevice, long numberDSP, long Address, void *pBuffer, long *Size);



// ------------- Функции калибровки АЦП/ЦАП -------------
// Функции для калибровки АЦП/ЦАП
// DigitalResolChan:	 при коэф. усиления == 1.0 - вес младшего разряда, В)
//						 при коэф. усиления > 1.0 - коэф. усиления
// offsetChan - текущее смещение, В 
long WINAPI ZGetCalibrDataADC(long typeDevice, long numberDSP, long numberChannel, double *digitalResolChan, double *offsetChan);
long WINAPI ZGetCalibrDataDAC(long typeDevice, long numberDSP, long numberChannel, double *digitalResolChan, double *offsetChan);
long WINAPI ZSetCalibrDataADC(long typeDevice, long numberDSP, long numberChannel, double digitalResolChan, double offsetChan);
long WINAPI ZSetCalibrDataDAC(long typeDevice, long numberDSP, long numberChannel, double digitalResolChan, double offsetChan);
long WINAPI ZReadCalibrData(long typeDevice, long numberDSP);		// прочитать из ПЗУ прибора в драйвер
long WINAPI ZSetDefaultCalibrData(long typeDevice, long numberDSP);	// установить калибровки по умолчанию
long WINAPI ZSaveCalibrData(long typeDevice, long numberDSP);		// записать из драйвера в ПЗУ
// Функция калибровки инверсного выхода дифференциального канала ЦАП
// Прямой (неинвертированный) выход калибруется с помощью ZSetCalibrDataDAC
// numberChannelDiff - указывается номер канала ЦАП такой же, как в ZSetCalibrDataDAC
// nonidentityMul, nonidentityAdd - мультипликативная и аддитивная неидентичность инверсного выхода
long WINAPI ZGetCalibrDataDiffDAC(long typeDevice, long numberDSP, long numberChannelDiff, double *nonidentityMul, double *nonidentityAdd);
long WINAPI ZSetCalibrDataDiffDAC(long typeDevice, long numberDSP, long numberChannelDiff, double nonidentityMul, double nonidentityAdd);

// ------------- Общие функции -------------
// подключиться к драйверу
long WINAPI ZOpen(long typeDevice, long numberDSP);
// отключиться от драйвера
long WINAPI ZClose(long typeDevice, long numberDSP);

// сброс и останов сигнальных процессоров (влияет на все DSP одного устройства)
long WINAPI ZResetDSP(long typeDevice, long numberDSP);
// проинициализировать сигнальный процессор
long WINAPI ZInitDSP(long typeDevice, long numberDSP, const char *fileName);
// получить серийный номер DSP
long WINAPI ZGetSerialNumberDSP(long typeDevice, long numberDSP, long *serialNumber);
// получить номер ревизии микропрограммы DSP
long WINAPI ZGetRevisionDSP(long typeDevice, long numberDSP, long *revisionNumber);
// получить имя устройства (возвращаемая строка не более 16 символов вместе с завершающим 0, maxSizeStr - размер буфера strName[])
long WINAPI ZGetNameDevice(long typeDevice, long numberDSP, char *strName, long maxSizeStr);
// получить версии ПО (возвращаемые строки не более 64 символов вместе с завершающим 0)
long WINAPI ZGetVersion(long typeDevice, long numberDSP, char *verDSP, char *verDRV, char *verLIB);
// получить тип интерфейса (1 - PCI, 2 - USB, 3 - Ethernet)
long WINAPI ZGetTypeConnection(long typeDevice, long numberDSP, long *type);
// опросить аппаратные возможности устр-ва
long WINAPI ZGetDeviceCapabilities(long typeDevice, long numberDSP, long *capabilities);
// прочитать код ошибки
long WINAPI ZGetError(long typeDevice, long numberDSP, long *error);
// определить кол-во изменений параметров с момента загрузки
long WINAPI ZGetModify(long typeDevice, long numberDSP, long *modify);

// прочитать всю информацию о модуле АЦП, ЦАП из драйвера
long WINAPI ZGetData(long typeDevice, long numberDSP, ADC_INFO *Data);
long WINAPI ZGetDataExt(long typeDevice, long numberDSP, ADC_INFO_EXT *Data);
// прочитать всю информацию о модуле АЦП, ЦАП из DSP
long WINAPI ZGetInfo(long typeDevice, long numberDSP, ADC_INFO *Data);
// передать информацию в DSP (кроме размера прерывания и старта)
long WINAPI ZPutInfo(long typeDevice, long numberDSP, ADC_INFO *Data);
// прочитать расширенную информацию из DSP
long WINAPI ZGetInfoExt(long typeDevice, long numberDSP, ADC_INFO_EXT *Data);

// прочитать флаг - количество прерываний DSP (пакетов) с момента ZStartADC()
long WINAPI ZGetFlag(long typeDevice, long numberDSP, unsigned long *flag);

// прочитать статус синхронизации по внешней частоте (устаревшая функция, см соответствующие функции для АЦП и ЦАП)
long WINAPI ZGetEnableExtFreq(long typeDevice, long numberDSP, long *enable);
// вкл./выкл. синхронизации по внешней частоте (устаревшая функция, см соответствующие функции для АЦП и ЦАП)
long WINAPI ZSetEnableExtFreq(long typeDevice, long numberDSP, long enable);
// прочитать статус внешнего запуска (устаревшая функция, см соответствующие функции для АЦП и ЦАП)
long WINAPI ZGetEnableExtStart(long typeDevice, long numberDSP, long *enable);
// вкл./выкл. внешнего запуска (устаревшая функция, см соответствующие функции для АЦП и ЦАП)
long WINAPI ZSetEnableExtStart(long typeDevice, long numberDSP, long enable);
// запустить тест шлейфа для плат PnP
long WINAPI ZTestCode(long typeDevice, long numberDSP, long *code0, long *code1, long *code2);


// Запросить кол-во линий цифрового порта
long WINAPI ZGetQuantityChannelDigPort(long typeDevice, long numberDSP, long *quantityChannel);
// Запросить маску выходов порта
long WINAPI ZGetDigOutEnable(long typeDevice, long numberDSP, unsigned long *digitalOutEnableMask);
// Записать маску выходов порта
long WINAPI ZSetDigOutEnable(long typeDevice, long numberDSP, unsigned long digitalOutEnableMask);
// Установить линию цифрового порта на выход (без изменения состояния других линий, numberOfBit = 0..quantityChannel-1)
long WINAPI ZSetBitDigOutEnable(long typeDevice, long numberDSP, long numberOfBit);
// Установить маску линий цифрового порта на выход (без изменения состояния других линий)
long WINAPI ZSetBitMaskDigOutEnable(long typeDevice, long numberDSP, unsigned long maskOfBits);
// Установить линию цифрового порта на вход (без изменения состояния других линий, numberOfBit = 0..quantityChannel-1)
long WINAPI ZClrBitDigOutEnable(long typeDevice, long numberDSP, long numberOfBit);
// Установить маску линий цифрового порта на вход (без изменения состояния других линий)
long WINAPI ZClrBitMaskDigOutEnable(long typeDevice, long numberDSP, unsigned long maskOfBits);
// Прочитать данные со входов порта
long WINAPI ZGetDigInput(long typeDevice, long numberDSP, unsigned long *digitalInput);
// Прочитать данные, выдаваемые на выходы порта
long WINAPI ZGetDigOutput(long typeDevice, long numberDSP, unsigned long *digitalOutput);
// Записать данные в порт
long WINAPI ZSetDigOutput(long typeDevice, long numberDSP, unsigned long digitalOutput);
// Установить вывод цифрового порта в "1" (без изменения состояния других выводов, numberOfBit = 0..quantityChannel-1)
long WINAPI ZSetBitDigOutput(long typeDevice, long numberDSP, long numberOfBit);
// Установить маску выводов цифрового порта в "1" (без изменения состояния других выводов)
long WINAPI ZSetBitMaskDigOutput(long typeDevice, long numberDSP, unsigned long maskOfBits);
// Установить вывод цифрового порта в "0" (без изменения состояния других выводов, numberOfBit = 0..quantityChannel-1)
long WINAPI ZClrBitDigOutput(long typeDevice, long numberDSP, long numberOfBit);
// Установить маску выводов цифрового порта в "0" (без изменения состояния других выводов)
long WINAPI ZClrBitMaskDigOutput(long typeDevice, long numberDSP, unsigned long maskOfBits);
// Опросить режим потокового ввода-вывода (mode=0 - аналоговый порт, mode=1 - цифрового порт, mode=2 - UART)
										// +0x04 - Инвертированный UART
										// +0x08 - 9 бит UART
										// +0x10 - 2 стоп-бита UART (не исп.)
										// +0x20 - контроль четности UART (не исп.)
										// +0x40 - IrDA UART (не исп.)
long WINAPI ZGetDigitalMode(long typeDevice, long numberDSP, long *mode);
// Переключить режим потокового ввода-вывода (mode=0 - аналоговый порт, mode=1 - цифрового порт, mode=2 - UART)
long WINAPI ZSetDigitalMode(long typeDevice, long numberDSP, long mode);
// Опрос разрешения генерации сигналов синхронизации на цифровой порт  
long WINAPI ZGetMasterSynchr(long typeDevice, long numberDSP, long *enable);
// Разрешить генерацию сигналов синхронизации на цифровой порт  
long WINAPI ZSetMasterSynchr(long typeDevice, long numberDSP, long enable);

// Запустить синхронизацию опорного кварцевого генератора
long WINAPI ZStartSynhrQuartz(long typeDevice, long numberDSP);
// Остановить синхронизацию опорного кварцевого генератора
long WINAPI ZStopSynhrQuartz(long typeDevice, long numberDSP);
// Опросить временные параметры сихронизации
long WINAPI ZGetSynhrTimeQuartz(long typeDevice, long numberDSP, SYNHR_QUARTZ_INFO *synhrElement);
// Подстроить частоту опорного кварцевого генератора
long WINAPI ZTrimmingQuartz(long typeDevice, long numberDSP, long size);
// Опросить дату и время GPS
long WINAPI ZGetGPSTime(long typeDevice, long numberDSP, ULONGLONG *time);
// Опросить дату и время старта
long WINAPI ZGetStartTime(long typeDevice, long numberDSP, ULONGLONG *time);
// Установить дату и время старта (time=0 - отключить запуск по времени)
long WINAPI ZSetStartTime(long typeDevice, long numberDSP, ULONGLONG time);

// проверить поддерживается ли модуль GPS
long WINAPI ZFindUART(long typeDevice, long numberDSP, long *present);
// Сконфигурировать UART (stopBits = 1, 2; ConfigParity: 0 бит - Parity, 1 бит - Even, 2 бит - StickParity)
long WINAPI ZSetConfigUART(long typeDevice, long numberDSP, long baudRate, short stopBits, short ConfigParity);
// Принять данные напрямую с UART
long WINAPI ZReadUART(long typeDevice, long numberDSP, void *buffer, long *size);
// Послать данные напрямую в UART
long WINAPI ZWriteUART(long typeDevice, long numberDSP, void *buffer, long *size);
// Запустить поток для буферезированного чтения UART 
long WINAPI ZStartBufReadingUART(long typeDevice, long numberDSP);
// Запустить поток для буферезированной записи UART
long WINAPI ZStartBufWritingUART(long typeDevice, long numberDSP);
// Остановить поток для буферезированного чтения UART
long WINAPI ZStopBufReadingUART(long typeDevice, long numberDSP);
// Остановить поток для буферезированной записи UART
long WINAPI ZStopBufWritingUART(long typeDevice, long numberDSP);
// Прочитать данные UART через буфер
long WINAPI ZReadBufUART(long typeDevice, long numberDSP, void *buffer, long *size);
// Записать данные UART через буфер
long WINAPI ZWriteBufUART(long typeDevice, long numberDSP, void *buffer, long *size);
// Переключение порта UART между GPS и акустическим модемом (switchRead: 0 - чтение потока GPS, 1 - чтение потока модема; switchWrite: 1 - поток с UART на порт GPIOB, 0 - работа GPIOB по умолчанию)
long WINAPI ZSwitchUART(long typeDevice, long numberDSP, long switchRead, long switchWrite);
// Прочитать некоторое количество байт побайтно непосредственно с UART
long WINAPI ZReadFromUART(long typeDevice, long numberDSP, void *buffer, long *size);
// Перейти в режим регистрации
long WINAPI ZSetRegistratorMode(long typeDevice, long numberDSP);
// Перезагрузка акустического модема
long WINAPI ZResetAM(long typeDevice, long numberDSP);
// Считывание уровня напряжения на входах дополнительного АЦП и температуры
long WINAPI ZReadUT(long typeDevice, long numberDSP, float *U1, float *U2, float *U3, float *U4, float *T);
// Асинхронный импульс
long WINAPI ZAsynchrGate(long typeDevice, long numberDSP, long gate);
// проверить поддерживается ли модуль ШИМ
long WINAPI ZFindPWM(long typeDevice, long numberDSP, long* present);
// запуск каналов ШИМ
long WINAPI ZStartPWM(long typeDevice, long numberDSP,long Start0, long Start1, long Start2);
// останов каналов ШИМ
long WINAPI ZStopPWM(long typeDevice, long numberDSP, long Stop0, long Stop1, long Stop2);
// задать частоту ШИМ через коэф. деления опорной частоты и период
long WINAPI ZSetFreqPWM(long typeDevice, long numberDSP, long Rate, long Period);
// задать скважность и сдвиг каналов ШИМ
long WINAPI ZSetOnDutyPWM(long typeDevice, long numberDSP, long OnDutyPWM0, long OnDutyPWM1, long OnDutyPWM2, long ShiftPWM1, long ShiftPWM2);
// обмен массивом коэффициентов управления с DSP
long WINAPI ZRegulatorPWM(long typeDevice, long numberDSP, void *data, long *size);


// ------------- Парные функции для работы с АЦП или ЦАП -------------
// поддержка АЦП в текущем режиме DSP
long WINAPI ZGetEnableADC(long typeDevice, long numberDSP, long *enable);
// поддержка ЦАП в текущем режиме DSP
long WINAPI ZGetEnableDAC(long typeDevice, long numberDSP, long *enable);
// установить сигнальный процессор в режим АЦП/ЦАП
long WINAPI ZSetTypeADC(long typeDevice, long numberDSP);
long WINAPI ZSetTypeDAC(long typeDevice, long numberDSP);
// чтение в каком состоянии находится АЦП/ЦАП
long WINAPI ZGetStartADC(long typeDevice, long numberDSP, long *status);
long WINAPI ZGetStartDAC(long typeDevice, long numberDSP, long *status);
// запуск процедуры ввода/вывода сигнала
long WINAPI ZStartADC(long typeDevice, long numberDSP);
long WINAPI ZStartDAC(long typeDevice, long numberDSP);
// останов процедуры ввода/вывода
long WINAPI ZStopADC(long typeDevice, long numberDSP);
long WINAPI ZStopDAC(long typeDevice, long numberDSP);

// прочитать кол-во каналов АЦП/ЦАП
long WINAPI ZGetQuantityChannelADC(long typeDevice, long numberDSP, long *quantityChannel);
long WINAPI ZGetQuantityChannelDAC(long typeDevice, long numberDSP, long *quantityChannel);
// прочитать вес младшего разряда АЦП/ЦАП (устаревшая функция)
long WINAPI ZGetDigitalResolutionADC(long typeDevice, long numberDSP, double *digitalResolution);
long WINAPI ZGetDigitalResolutionDAC(long typeDevice, long numberDSP, double *digitalResolution);
// прочитать откалиброванный поканально вес младшего разряда АЦП/ЦАП (в вольтах) 
long WINAPI ZGetDigitalResolChanADC(long typeDevice, long numberDSP, long numberChannel, double *digitalResolChan);
long WINAPI ZGetDigitalResolChanDAC(long typeDevice, long numberDSP, long numberChannel, double *digitalResolChan);
// прочитать кол-во бит в одном отсчете АЦП/ЦАП
long WINAPI ZGetBitsADC(long typeDevice, long numberDSP, long *numberBits);
long WINAPI ZGetBitsDAC(long typeDevice, long numberDSP, long *numberBits);
// прочитать кол-во слов в одном отсчете АЦП/ЦАП
long WINAPI ZGetWordsADC(long typeDevice, long numberDSP, long *numberWords);
long WINAPI ZGetWordsDAC(long typeDevice, long numberDSP, long *numberWords);

// прочитать частоту дискретизации 
long WINAPI ZGetFreqADC(long typeDevice, long numberDSP, double *freq);
long WINAPI ZGetFreqDAC(long typeDevice, long numberDSP, double *freq);
// прочитать частоту из списка возможных частот дискретизации 
long WINAPI ZGetListFreqADC(long typeDevice, long numberDSP, long next, double *freq);
long WINAPI ZGetListFreqDAC(long typeDevice, long numberDSP, long next, double *freq);
// установить следующую из списка частоту дискретизации 
long WINAPI ZSetNextFreqADC(long typeDevice, long numberDSP, long next, double *freq);
long WINAPI ZSetNextFreqDAC(long typeDevice, long numberDSP, long next, double *freq);
// установить частоту дискретизации
long WINAPI ZSetFreqADC(long typeDevice, long numberDSP, double freqIn, double *freqOut);
long WINAPI ZSetFreqDAC(long typeDevice, long numberDSP, double freqIn, double *freqOut);
// определить установленный коэффициент деления (служебн., пользоваться не рекомендуется)
long WINAPI ZGetRateADC(long typeDevice, long numberDSP, long *rate);
long WINAPI ZGetRateDAC(long typeDevice, long numberDSP, long *rate);
// Rate - коэффициент деления опорной частоты определяемой в AdcGetOporFreq()
// установить частоту преобразования АЦП (служебн., пользоваться не рекомендуется)
long WINAPI ZSetRateADC(long typeDevice, long numberDSP, long rate);
long WINAPI ZSetRateDAC(long typeDevice, long numberDSP, long rate);

// прочитать значение текущей опорной частоты
long WINAPI ZGetExtFreqADC(long typeDevice, long numberDSP, double *oporFreq);
long WINAPI ZGetExtFreqDAC(long typeDevice, long numberDSP, double *oporFreq);
// установить значение внешней опорной частоты
long WINAPI ZSetExtFreqADC(long typeDevice, long numberDSP, double extFreq);
long WINAPI ZSetExtFreqDAC(long typeDevice, long numberDSP, double extFreq);
// прочитать статус синхронизации по внешней частоте 
long WINAPI ZGetEnableExtFreqADC(long typeDevice, long numberDSP, long *enable);
long WINAPI ZGetEnableExtFreqDAC(long typeDevice, long numberDSP, long *enable);
// вкл./выкл. синхронизации по внешней частоте
long WINAPI ZSetEnableExtFreqADC(long typeDevice, long numberDSP, long enable);
long WINAPI ZSetEnableExtFreqDAC(long typeDevice, long numberDSP, long enable);
// прочитать статус внешнего запуска
long WINAPI ZGetEnableExtStartADC(long typeDevice, long numberDSP, long *enable);
long WINAPI ZGetEnableExtStartDAC(long typeDevice, long numberDSP, long *enable);
// вкл./выкл. внешнего запуска
long WINAPI ZSetEnableExtStartADC(long typeDevice, long numberDSP, long enable);
long WINAPI ZSetEnableExtStartDAC(long typeDevice, long numberDSP, long enable);

// определить установленный размер буфера при прерывании
long WINAPI ZGetInterruptADC(long typeDevice, long numberDSP, long *size);
long WINAPI ZGetInterruptDAC(long typeDevice, long numberDSP, long *size);
// определить макс. возможный размер буфера при прерывании
long WINAPI ZGetMaxInterruptADC(long typeDevice, long numberDSP, long *size);
long WINAPI ZGetMaxInterruptDAC(long typeDevice, long numberDSP, long *size);
// установить размер буфера для перекачки при прерывании
long WINAPI ZSetInterruptADC(long typeDevice, long numberDSP, long size);
long WINAPI ZSetInterruptDAC(long typeDevice, long numberDSP, long size);

// определить установленный размер пакета данных DSP
long WINAPI ZGetSizePacketADC(long typeDevice, long numberDSP, long *size);
long WINAPI ZGetSizePacketDAC(long typeDevice, long numberDSP, long *size);
// определить макс. возможный размер пакета данных DSP
long WINAPI ZGetMaxSizePacketADC(long typeDevice, long numberDSP, long *size);
long WINAPI ZGetMaxSizePacketDAC(long typeDevice, long numberDSP, long *size);
// установить размер пакета данных DSP
long WINAPI ZSetSizePacketADC(long typeDevice, long numberDSP, long size);
long WINAPI ZSetSizePacketDAC(long typeDevice, long numberDSP, long size);

// определить установленное кол-во пакетов за одно прерывание
long WINAPI ZGetQuantityPacketsADC(long typeDevice, long numberDSP, long *size);
long WINAPI ZGetQuantityPacketsDAC(long typeDevice, long numberDSP, long *size);
// определить макс. возможное кол-во пакетов за одно прерывание
long WINAPI ZGetMaxQuantityPacketsADC(long typeDevice, long numberDSP, long *size);
long WINAPI ZGetMaxQuantityPacketsDAC(long typeDevice, long numberDSP, long *size);
// установить кол-во пакетов за одно прерывание
long WINAPI ZSetQuantityPacketsADC(long typeDevice, long numberDSP, long size);
long WINAPI ZSetQuantityPacketsDAC(long typeDevice, long numberDSP, long size);

// установить однократное или циклическое накопление в буфер ОЗУ ПК 
long WINAPI ZSetCycleSampleADC(long typeDevice, long numberDSP, long enable);
long WINAPI ZSetCycleSampleDAC(long typeDevice, long numberDSP, long enable);

// задать размер буфера АЦП в ОЗУ ПК
long WINAPI ZSetBufferSizeADC(long typeDevice, long numberDSP, long size);
long WINAPI ZSetBufferSizeDAC(long typeDevice, long numberDSP, long size);
// запросить буфер в ОЗУ ПК
long WINAPI ZGetBufferADC(long typeDevice, long numberDSP, void **buffer, long *size);
long WINAPI ZGetBufferDAC(long typeDevice, long numberDSP, void **buffer, long *size);
// освободить буфер в ОЗУ ПК
long WINAPI ZRemBufferADC(long typeDevice, long numberDSP, void **buffer);
long WINAPI ZRemBufferDAC(long typeDevice, long numberDSP, void **buffer);
// чтение указателя буфера в ОЗУ ПК
long WINAPI ZGetPointerADC(long typeDevice, long numberDSP, long *pointer);
long WINAPI ZGetPointerDAC(long typeDevice, long numberDSP, long *pointer);

// опрос количества включенных каналов
long WINAPI ZGetNumberInputADC(long typeDevice, long numberDSP, long *workChannels);
long WINAPI ZGetNumberOutputDAC(long typeDevice, long numberDSP, long *workChannels);
// опрос включен ли заданный канал
long WINAPI ZGetInputADC(long typeDevice, long numberDSP, long numberChannel, long *enable);
long WINAPI ZGetOutputDAC(long typeDevice, long numberDSP, long numberChannel, long *enable);
// включить/выключить заданный канал
long WINAPI ZSetInputADC(long typeDevice, long numberDSP, long numberChannel, long enable);
long WINAPI ZSetOutputDAC(long typeDevice, long numberDSP, long numberChannel, long enable);
// включить/выключить каналы с помощью битовой маски (служебн., пользоваться не рекомендуется)
long WINAPI ZSetChannelADC(long typeDevice, long numberDSP, unsigned long channelMask);
long WINAPI ZSetChannelDAC(long typeDevice, long numberDSP, unsigned long channelMask);


// ------------- Функции для работы только с АЦП -------------
// получить оставшиеся данные АЦП (больше не поддерживается!)
long WINAPI ZGetLastDataADC(long typeDevice, long numberDSP, long numberChannel, void *buffer, long size);

// прочитать режим работы устройства
long WINAPI ZGetModaADC(long typeDevice, long numberDSP, long *moda);
// установить режим работы устройства (служебн., пользоваться не рекомендуется)
long WINAPI ZSetModaADC(long typeDevice, long numberDSP, long moda);

// прочитать коэф. усиления
long WINAPI ZGetAmplifyADC(long typeDevice, long numberDSP, long numberChannel, double *amplify);
// прочитать коэф. усиления из списка возможных коэф. усиления
long WINAPI ZGetListAmplifyADC(long typeDevice, long numberDSP, long next, double *amplify);
// установить следующий коэф. усиления
long WINAPI ZSetNextAmplifyADC(long typeDevice, long numberDSP, long numberChannel, long next, double *amplify);
// установить коэф. усиления
long WINAPI ZSetAmplifyADC(long typeDevice, long numberDSP, long numberChannel, double amplifyIn, double *amplifyOut);

// прочитать коэф. усиления ПУ8/10
long WINAPI ZGetPreAmplifyADC(long typeDevice, long numberDSP, long numberChannel, double *amplify);
// прочитать коэф. усиления из списка возможных коэф. усиления ПУ8/10
long WINAPI ZGetListPreAmplifyADC(long typeDevice, long numberDSP, long next, double *amplify);
// установить следующий коэф. усиления ПУ8/10
long WINAPI ZSetNextPreAmplifyADC(long typeDevice, long numberDSP, long numberChannel, long next, double *amplify);
// установить коэф. усиления ПУ8/10
long WINAPI ZSetPreAmplifyADC(long typeDevice, long numberDSP, long numberChannel, double amplifyIn, double *amplifyOut);

// проверить поддерживается и подключен ли модуль HCP
long WINAPI ZFindHCPADC(long typeDevice, long numberDSP, long *present);
// прочитать о состоянии заданного канала модуля HCP
long WINAPI ZGetHCPADC(long typeDevice, long numberDSP, long numberChannel, long *enable);
// установить состояние заданного канала модуля HCP
long WINAPI ZSetHCPADC(long typeDevice, long numberDSP, long numberChannel, long enable);

// опрос дифференциального режима заданного канала для ввода 
long WINAPI ZGetInputDiffADC(long typeDevice, long numberDSP, long numberChannel, long *enable);
// установить-сбросить заданный канал для ввода в дифференциальный режим
long WINAPI ZSetInputDiffADC(long typeDevice, long numberDSP, long numberChannel, long enable);


// ------------- Функции для работы только с ЦАП -------------
// установить внешнюю (из памяти ПК) или внутреннюю (только из памяти DSP) загрузку в ЦАП
long WINAPI ZSetExtCycleDAC(long typeDevice, long numberDSP, long enable);

// опросить поддерживается ли программный аттенюатор
long WINAPI ZFindSoftAtten(long typeDevice, long numberDSP, long *present);
// прочитать коф. затухания аттенюатора
long WINAPI ZGetAttenDAC(long typeDevice, long numberDSP, long numberChannel, double *reduction);
// установить коф. затухания аттенюатора
long WINAPI ZSetAttenDAC(long typeDevice, long numberDSP, long numberChannel, double reductionIn, double *reductionOut);

// опрос максимального размера буфера ЦАП в DSP
long WINAPI ZGetMaxSizeBufferDSPDAC(long typeDevice, long numberDSP, long *size);
// опрос размера буфера ЦАП в DSP
long WINAPI ZGetSizeBufferDSPDAC(long typeDevice, long numberDSP, long *size);
// установить размер буфера ЦАП в DSP
long WINAPI ZSetSizeBufferDSPDAC(long typeDevice, long numberDSP, long size);


// ------------- Функции для работы с меткой устройства -------------
// установить метку устройства
long WINAPI ZSetDeviceLabel(long typeDevice, long numberDSP, const WCHAR *label);
// прочитать метку устройства
long WINAPI ZGetDeviceLabel(long typeDevice, long numberDSP, WCHAR *label, long maxWchars);

// ------------- Функции для работы с СУВ -------------
// установить и прочитать параметры СУВ
long WINAPI ZSetVibrationControlParams(long typeDevice, long numberDSP, long numberChannel, double value, long numberSamples, double coef, const void *ext, long extSize);
long WINAPI ZGetVibrationControlDiagnostics(long typeDevice, long numberDSP, void *data, long *size);

// ------------- Функции для работы со встроенным генератором -------------
// включить/отключить встроенный генератор
long WINAPI ZGetEnableBuiltinGenerator(long typeDevice, long numberDSP, long *enable);
// прочитать состояние встроенного генератора
long WINAPI ZSetEnableBuiltinGenerator(long typeDevice, long numberDSP, long enable);
// установить параметры синусоидальной формы встроенного генератора
long WINAPI ZSetSineBuiltinGenerator(long typeDevice, long numberDSP, long active, double freq, double level, double offset);
// прочитать параметры синусоидальной формы встроенного генератора
long WINAPI ZGetSineBuiltinGenerator(long typeDevice, long numberDSP, long *active, double *freq, double *level, double *offset);
// установить параметры формы 1PPS встроенного генератора
long WINAPI ZSet1PPSBuiltinGenerator(long typeDevice, long numberDSP, long active);
// прочитать параметры формы 1PPS встроенного генератора
long WINAPI ZGet1PPSBuiltinGenerator(long typeDevice, long numberDSP, long *active);


// ------------- Функции для работы с DAC_MUX -------------
// проверить поддержку режима DAC_MUX
long WINAPI ZCheckModeDACMUX(long typeDevice, long numberDSP, long mode);
// установить режим (0 - снять режим)
long WINAPI ZSetModeDACMUX(long typeDevice, long numberDSP, long mode);
// прочитать текущий режим
long WINAPI ZGetModeDACMUX(long typeDevice, long numberDSP, long *mode);
// прочитать код ожидания
long WINAPI ZGetWaitDACMUX(long typeDevice, long numberDSP, long *waitCode);
// установить код ожидания (экстренная остановка)
long WINAPI ZSetWaitDACMUX(long typeDevice, long numberDSP, long waitCode);
// сбросить код ожидания
long WINAPI ZResetWaitDACMUX(long typeDevice, long numberDSP, long waitCode);
// получить копию DAC_MUX_FB_STRUCT
long WINAPI ZGetDataDACMUX(long typeDevice, long numberDSP, void *data, long *dataSize);


// Устаревшее описание функций (см. ниже) !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//Все функции возвращают код ошибки. 0 - нет ошибки, иначе код ошибки
// Функции воздействующие на оба DSP:
int WINAPI Adc_StopDSP(int typeDevice, int numberDSP);	// сброс и останов сигнальных процессоров (влияет на все DSP одного устройства)

// ------------- Функции эксклюзивного режима №1 -------------

// подключиться к драйверу в эксклюзивном режиме №1
int WINAPI Adc_Exclusive1Open(int typeDevice, int numberDSP);
// отключиться от драйвера в эксклюзивном режиме №1
int WINAPI Adc_Exclusive1Close(int typeDevice, int numberDSP);
// послать команду DSP
int WINAPI Adc_SendCommand(int typeDevice, int numberDSP, void *buffer, long *size);
// принять команду DSP
int WINAPI Adc_ReceiveCommand(int typeDevice, int numberDSP, void *buffer, long *size);
// послать данные ЦАП (size - кратен 512)
int WINAPI Adc_SendDataDAC(int typeDevice, int numberDSP, void *buffer, long *size);
// принять данные АЦП (size - кратен 512)
int WINAPI Adc_ReceiveDataADC(int typeDevice, int numberDSP, void *buffer, long *size);


// ------------- Функции настройки TCP/IP -------------
// Прочитать настройки LAN устройства для TCP/IP из ПЗУ
int WINAPI Adc_ReadSettingsLAN(int typeDevice, int numberDSP, unsigned char *sourceIPAdr,
							   unsigned short *sourcePort0, unsigned char *subnetMask,
							   unsigned char *gatewayIPAdr, unsigned char *sourceMACAdr);
// Сохранить настройки LAN устройства для TCP/IP в ПЗУ
int WINAPI Adc_SaveSettingsLAN(int typeDevice, int numberDSP, unsigned char *sourceIPAdr,
							   unsigned short *sourcePort0, unsigned char *subnetMask,
							   unsigned char *gatewayIPAdr, unsigned char *sourceMACAdr);


// ------------- Общие функции -------------
int WINAPI AdcOpen(int typeDevice, int numberDSP);		// подключиться к драйверу
int WINAPI AdcClose(int typeDevice, int numberDSP);		// отключиться от драйвера
int WINAPI Adc_Load_DSP(int typeDevice, int numberDSP, ULONG *mass, ULONG size, ULONG adrdsp);	// загрузить данные в память сигнального процессора
int WINAPI Adc_UnLoad_DSP(int typeDevice, int numberDSP, ULONG *mass, ULONG size, ULONG adrdsp);	// прочитать данные из памяти сигнального процессора
int WINAPI Adc_Zagr_DSP(int typeDevice, int numberDSP, char *filename);	// загрузить программу в сигнальный процессор
int WINAPI AdcGetVersion(int typeDevice, int numberDSP, char *verDSP, char *verDRV, char *verLIB); // Получить строки версий
int WINAPI AdcGetData(int typeDevice, int numberDSP, ADC_INFO *Data);	// прочитать всю информацию о модуле АЦП, ЦАП из драйвера
int WINAPI AdcGetDataExt(int typeDevice, int numberDSP, ADC_INFO_EXT *Data);
int WINAPI AdcGetInfo(int typeDevice, int numberDSP, ADC_INFO *Data);	// прочитать всю информацию о модуле АЦП, ЦАП из DSP
int WINAPI AdcPutInfo(int typeDevice, int numberDSP, ADC_INFO *Data);	// передать информацию в DSP (кроме размера прерывания и старта)
int WINAPI AdcError(int typeDevice, int numberDSP, int *error);		// прочитать код ошибки
int WINAPI AdcFlag(int typeDevice, int numberDSP, UINT *flag);		// чтение флага - количество прерываний от момента AdcStart()
int WINAPI AdcGetModify(int typeDevice, int numberDSP, int *modify);	// определить кол-во изменений параметров с момента загрузки
int WINAPI AdcGetTypeAdc(int typeDevice, int numberDSP, int *typeDeviceADC);// определить в каком режиме находится сигнальный процессор
int WINAPI AdcTestCode(int typeDevice, int numberDSP, int *TestCode0, int *TestCode1, int *TestCode2);	// Запустить тест шлейфа для плат PnP

int WINAPI AdcGetDigOutEnable(int typeDevice, int numberDSP, int *OutEnableMask);	// Запросить маску выходов порта
int WINAPI AdcSetDigOutEnable(int typeDevice, int numberDSP, int OutEnableMask);	// Установить маску выходов порта
int WINAPI AdcGetDigInput(int typeDevice, int numberDSP, int *Input);		// Прочитать данные со входов порта
int WINAPI AdcGetDigOutput(int typeDevice, int numberDSP, int *Output);	// Прочитать данные, выдаваемые на выходы порта
int WINAPI AdcSetDigOutput(int typeDevice, int numberDSP, int Output);	// Записать данные в порт

// Функции для работы с АЦП или ЦАП:
int WINAPI AdcSetTypeDac(int typeDevice, int numberDSP);					// установить сигнальный процессор в режим ЦАП
int WINAPI AdcSetTypeAdc(int typeDevice, int numberDSP);					// установить сигнальный процессор в режиме АЦП
int WINAPI AdcGetEnableAdc(int typeDevice, int numberDSP, int *EnableADC);	// поддержка АЦП в текущем режиме DSP
int WINAPI AdcGetEnableDac(int typeDevice, int numberDSP, int *EnableDAC);	// поддержка ЦАП в текущем режиме DSP
int WINAPI AdcStart(int typeDevice, int numberDSP);							// запуск процедуры ввода сигнала АЦП
int WINAPI AdcStartDac(int typeDevice, int numberDSP);						// запуск процедуры вывода сигнала ЦАП
int WINAPI AdcGetStart(int typeDevice, int numberDSP, UINT *status);			// чтение в каком состоянии находится АЦП
int WINAPI AdcGetStartDac(int typeDevice, int numberDSP, UINT *status);		// чтение в каком состоянии находится ЦАП
int WINAPI AdcStop(int typeDevice, int numberDSP);							// останов процедуры ввода АЦП
int WINAPI AdcStopDac(int typeDevice, int numberDSP);							// останов процедуры вывода ЦАП

int WINAPI AdcGetQuantityChannelAdc(int typeDevice, int numberDSP, int *QuantityChannelADC);	// кол-во каналов АЦП
int WINAPI AdcGetQuantityChannelDac(int typeDevice, int numberDSP, int *QuantityChannelDAC);	// кол-во каналов ЦАП
int WINAPI AdcGetDigitalResolutionAdc(int typeDevice, int numberDSP, double *DigitalResolutionADC);// вес младшего разряда АЦП
int WINAPI AdcGetDigitalResolutionDac(int typeDevice, int numberDSP, double *DigitalResolutionDAC);// вес младшего разряда ЦАП
int WINAPI AdcGetDigitalResolChanAdc(int typeDevice, int numberDSP, int numberChannel, double *digitalResolChan);
int WINAPI AdcGetDigitalResolChanDac(int typeDevice, int numberDSP, int numberChannel, double *digitalResolChan);

int WINAPI AdcGetAdcBits(int typeDevice, int numberDSP, int *numberBITS);		// разрядность АЦП
int WINAPI AdcGetDacBits(int typeDevice, int numberDSP, int *numberBITS);		// разрядность ЦАП
int WINAPI AdcGetAdcWords(int typeDevice, int numberDSP, int *numberWORDS);	// кол-во слов в одном отсчете АЦП
int WINAPI AdcGetDacWords(int typeDevice, int numberDSP, int *numberWORDS);	// кол-во слов в одном отсчете ЦАП

int WINAPI AdcRate(int typeDevice, int numberDSP, int Rate);			// установить частоту преобразования АЦП (служебн.)
int WINAPI AdcRateDac(int typeDevice, int numberDSP, int Rate);		// установить частоту преобразования ЦАП (служебн.)
								// Rate - коэффициент деления опорной частоты определяемой в AdcGetOporFreq()
int WINAPI AdcGetRate(int typeDevice, int numberDSP, int *Rate);		// определить установленный коэффициент деления (служебн.)
int WINAPI AdcGetRateDac(int typeDevice, int numberDSP, int *Rate);	// определить установленный коэффициент деления (служебн.)
int WINAPI AdcGetFreq(int typeDevice, int numberDSP, double *freq);		// определить частоту АЦП 
int WINAPI AdcGetFreqDac(int typeDevice, int numberDSP, double *freq);	// определить частоту ЦАП
int WINAPI AdcGetListFreq(int typeDevice, int numberDSP, int next, double *freq);		// получить список частот АЦП
int WINAPI AdcGetListFreqDac(int typeDevice, int numberDSP, int next, double *freq);	// получить список частот ЦАП
int WINAPI AdcSetNextFreq(int typeDevice, int numberDSP, int next, double *freq);		// установить следующую из списка частоту АЦП 
int WINAPI AdcSetNextFreqDac(int typeDevice, int numberDSP, int next, double *freq);	// установить следующую из списка частоту ЦАП
int WINAPI AdcSetFreq(int typeDevice, int numberDSP, double freqin, double *freqout);		// установить частоту АЦП
int WINAPI AdcSetFreqDac(int typeDevice, int numberDSP, double freqin, double *freqout);	// установить частоту ЦАП
int WINAPI AdcSetExtFreq(int typeDevice, int numberDSP, double extFreq);		// установить значение внешней опорной частоты АЦП
int WINAPI AdcSetExtFreqDac(int typeDevice, int numberDSP, double extFreq);	// установить значение внешней опорной частоты ЦАП
int WINAPI AdcGetExtFreq(int typeDevice, int numberDSP, double *oporfreq);	// получить значение опорной частоты АЦП
int WINAPI AdcGetExtFreqDac(int typeDevice, int numberDSP, double *oporfreq);	// получить значение опорной частоты ЦАП
int WINAPI AdcEnaExtFreq(int typeDevice, int numberDSP, short ext);		// вкл./выкл. синхронизации по внешней частоте
int WINAPI AdcEnaExtFreqDac(int typeDevice, int numberDSP, short ext);		// вкл./выкл. синхронизации по внешней частоте
int WINAPI AdcGetEnaExtFreq(int typeDevice, int numberDSP, short *ext);	// прочитать статус синхронизации по внешней частоте
int WINAPI AdcGetEnaExtFreqDac(int typeDevice, int numberDSP, short *ext);	// прочитать статус синхронизации по внешней частоте
int WINAPI AdcEnaExtStart(int typeDevice, int numberDSP, short ext);		// вкл./выкл. внешнего запуска
int WINAPI AdcEnaExtStartDac(int typeDevice, int numberDSP, short ext);		// вкл./выкл. внешнего запуска
int WINAPI AdcGetEnaExtStart(int typeDevice, int numberDSP, short *ext);	// прочитать статус внешнего запуска
int WINAPI AdcGetEnaExtStartDac(int typeDevice, int numberDSP, short *ext);	// прочитать статус внешнего запуска


int WINAPI AdcInterrupt(int typeDevice, int numberDSP, int size);		// установить размер буфера для перекачки при прерывании АЦП
int WINAPI AdcInterruptDac(int typeDevice, int numberDSP, int size);	// установить размер буфера для перекачки при прерывании ЦАП
int WINAPI AdcGetInterrupt(int typeDevice, int numberDSP, int *size);		// определить установленный размер буфера при прерывании АЦП
int WINAPI AdcGetInterruptDac(int typeDevice, int numberDSP, int *size);	// определить установленный размер буфера при прерывании ЦАП
int WINAPI AdcGetMaxInterrupt(int typeDevice, int numberDSP, int *size);		// определить макс. возможный размер буфера при прерывании АЦП
int WINAPI AdcGetMaxInterruptDac(int typeDevice, int numberDSP, int *size);	// определить макс. возможный размер буфера при прерывании ЦАП

int WINAPI AdcSetSizePacketAdc(int typeDevice, int numberDSP, int size);	// установить размер пакета данных DSP
int WINAPI AdcSetSizePacketDac(int typeDevice, int numberDSP, int size);	
int WINAPI AdcGetSizePacketAdc(int typeDevice, int numberDSP, int *size);	// определить установленный размер пакета данных DSP
int WINAPI AdcGetSizePacketDac(int typeDevice, int numberDSP, int *size);
int WINAPI AdcGetMaxSizePacketAdc(int typeDevice, int numberDSP, int *size);	// определить макс. возможный размер пакета данных DSP
int WINAPI AdcGetMaxSizePacketDac(int typeDevice, int numberDSP, int *size);	

int WINAPI AdcSetQuantityPacketsAdc(int typeDevice, int numberDSP, int size);	// установить кол-во пакетов за одно прерывание
int WINAPI AdcSetQuantityPacketsDac(int typeDevice, int numberDSP, int size);
int WINAPI AdcGetQuantityPacketsAdc(int typeDevice, int numberDSP, int *size);	// определить установленное кол-во пакетов за одно прерывание
int WINAPI AdcGetQuantityPacketsDac(int typeDevice, int numberDSP, int *size);
int WINAPI AdcGetMaxQuantityPacketsAdc(int typeDevice, int numberDSP, int *size);	// определить макс. возможное кол-во пакетов за одно прерывание
int WINAPI AdcGetMaxQuantityPacketsDac(int typeDevice, int numberDSP, int *size);

int WINAPI AdcEnaCycleSample(int typeDevice, int numberDSP, short ext);		// однократное или циклическое накопление в буфер АЦП 
int WINAPI AdcEnaCycleSampleDac(int typeDevice, int numberDSP, short ext);	// однократное или циклическое накопление в буфер ЦАП
int WINAPI AdcSetBufferSize(int typeDevice, int numberDSP, int size);		// задать размер буфера АЦП в ОЗУ
int WINAPI AdcSetBufferSizeDac(int typeDevice, int numberDSP, int size);	// задать размер буфера ЦАП в ОЗУ
int WINAPI AdcGetBuffer(int typeDevice, int numberDSP, short **bufer, int *size);	// запросить буфер АЦП
int WINAPI AdcGetBufferDac(int typeDevice, int numberDSP,short **bufer, int *size);	// запросить буфер ЦАП
int WINAPI AdcRemBuffer(int typeDevice, int numberDSP, short **buffer);		// освободить буфер АЦП
int WINAPI AdcRemBufferDac(int typeDevice, int numberDSP, short **buffer);	// освободить буфер ЦАП
int WINAPI AdcGetPointer(int typeDevice, int numberDSP, ULONG *pointer);		// чтение указателя АЦП
int WINAPI AdcGetPointerDac(int typeDevice, int numberDSP, ULONG *pointer);	// чтение указателя ЦАП
int WINAPI AdcChannel(int typeDevice, int numberDSP, long channelbit);		// битовая маска включенных каналов АЦП
int WINAPI AdcChannelOutDac(int typeDevice, int numberDSP, long channelbit);	// битовая маска включенных каналов ЦАП
int WINAPI AdcSetInput(int typeDevice, int numberDSP, int channel, int enable);	// вкл./выкл. заданный канал АЦП
int WINAPI AdcGetInput(int typeDevice, int numberDSP, int channel, int *enable);	// опрос состояния заданного канала АЦП
int WINAPI AdcSetOutDac(int typeDevice, int numberDSP, int channel, int enable);	// вкл./выкл. заданный канал ЦАП
int WINAPI AdcGetOutDac(int typeDevice, int numberDSP, int channel, int *enable);	// опрос состояния заданного канала ЦАП	
int WINAPI AdcGetNumberInput(int typeDevice, int numberDSP, int *numberchannel);	// опрос количества включенных каналов АЦП
int WINAPI AdcGetNumberOutDac(int typeDevice, int numberDSP, int *numberchannel);	// опрос количества включенных каналов ЦАП


// Функции для работы только с АЦП:
int WINAPI AdcGetLastDataInt(int typeDevice, int numberDSP, int channel, short *buffer, unsigned long size); // получить оставшиеся данные АЦП
int WINAPI AdcModa(int typeDevice, int numberDSP, int moda);		// установить режим работы первичного цифрового фильтра АЦП
int WINAPI AdcGetModa(int typeDevice, int numberDSP, int *moda);	// прочитать режим работы первичного цифрового фильтра АЦП
int WINAPI AdcFindHCP(int typeDevice, int numberDSP, int *present);	// проверить поддерживается и подключен ли модуль HCP
int WINAPI AdcGetHCP(int typeDevice, int numberDSP, int numberChannel, int *state);	// прочитать о состоянии заданного канала модуля HCP
int WINAPI AdcSetHCP(int typeDevice, int numberDSP, int numberChannel, int state);	// установить состояние заданного канала модуля HCP

int WINAPI AdcGetListAmplify(int typeDevice, int numberDSP, int next, double *Amplify); //получить список возможных коэф. усиления
int WINAPI AdcSetNextAmplify(int typeDevice, int numberDSP, int numberchannel, int next, double *Amplify);//установить следующий коэф. усиления
int WINAPI AdcGetAmplify(int typeDevice, int numberDSP, int numberchannel, double *Amplify); // прочитать коэф. усиления
int WINAPI AdcSetAmplify(int typeDevice, int numberDSP, int numberchannel, double InAmplify, double *OutAmplify);//установить коэф. усиления
int WINAPI AdcAmplify(int typeDevice, int numberDSP, int kanal, int ampl);	// установка коэф. усиления (служебн.)

int WINAPI AdcGetListPreAmplify(int typeDevice, int numberDSP, int next, double *Amplify); //получить список возможных коэф. предусиления
int WINAPI AdcSetNextPreAmplify(int typeDevice, int numberDSP, int numberchannel, int next, double *Amplify);	// установить следующий коэф. предусиления
int WINAPI AdcSetPreAmplify(int typeDevice, int numberDSP, int numberchannel, double InAmplify, double *OutAmplify);	// прочитать коэф. предусиления
int WINAPI AdcGetPreAmplify(int typeDevice, int numberDSP, int numberchannel, double * OutAmplify);//установить коэф. усиления
int WINAPI AdcPreAmpl(int typeDevice, int numberDSP, int kanal, int ampl);		// установка коэф. предусиления (служебн.)

int WINAPI AdcSetInputDiff(int typeDevice, int numberDSP, int channel, int enable);//установить-сбросить заданный канал для ввода в дифференциальный режим
int WINAPI AdcGetInputDiff(int typeDevice, int numberDSP, int channel, int *enable);//опрос дифференциальный режима заданного канала для ввода 


// Функции для работы только с ЦАП:
int WINAPI AdcSetExtCycle(int typeDevice, int numberDSP, int trig);		// установить внешнюю или внутреннюю загрузку в ЦАП
int WINAPI AdcSetAtt(int typeDevice, int numberDSP, int channel, double att, double *setatt); // установить коф. затухания аттенюатора
int WINAPI AdcGetAtt(int typeDevice, int numberDSP, int channel, double *setatt); // прочитать коф. затухания аттенюатора
int WINAPI AdcAtten(int typeDevice, int numberDSP, int kanal, int ampl);		// установка коэффициента затухания аттенюатора (служебн.)
