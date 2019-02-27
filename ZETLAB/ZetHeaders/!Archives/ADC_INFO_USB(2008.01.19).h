// adc_info_usb.h
#ifndef __adc_info_usb_h__
#define __adc_info_usb_h__

// Коды команд (ADC_INFO_USB.Command):
//	0x0000 - GetInfo
// 	0x0001 - Reading Digital port
//	0x0002 - Writing to Digital port
//	0x0003 - Digital output enable
//	0x0004 - PWM, Open 1Wire EEPROM
//	0x0005 - Read 1Wire EEPROM
//	0x0006 - Write 1Wire EEPROM
//	0x0007 - Close 1Wire EEPROM
//	0x0012 - PutInfo
//	0x0013 - read correct settings
//	0x0014 - save correct settings
//	0x0015 - read LAN settings
//	0x0016 - save LAN settings
//	0x0105 - read_dir
//	0x0108 - read time from RTC
//	0x0109 - set time to RTC
//	0x0110 - read from SD
//	0x0114 - del File SD
//	0x0116 - write to SD
//	0x0120 - save InfoInt in file
//	0x0122 - Format SD

// 	Биты EnaExtStartFreq;		//разрешение внешнего старта, внешней частоты 0-внутр. 1-внеш
#define ENA_EXT_START_ADC	0x0001	// 0 бит - внешний старт АЦП
#define ENA_EXT_START_DAC	0x0002	// 1 бит - внешний старт ЦАП
#define ENA_EXT_FREQ_ADC	0x0004	// 2 бит - внешняя опорная частота АЦП
#define ENA_EXT_FREQ_DAC	0x0008	// 3 бит - внешняя опорная частота ЦАП
#define ENA_INT_CYCLE_DAC	0x0010	// 4 бит - работа ЦАП без подкачки данных извне

typedef struct _ADC_INFO_USB {
	// Команды DSP
	unsigned short	Command;				//Команда
	unsigned short	TypeADC;				//Режим работы DSP: 0 - ADC, 1 - DAC
	short			Start;					//состояние ввода данных 1- ввод
	short			StartDAC;				//состояние ввода данных 1- ввод

	// Аппаратная конфигурация DSP
	unsigned short	EnableADC;				//Наличие АЦП
	unsigned short	EnableDAC;				//Наличие ЦАП
	unsigned short	EnableExtDevice;		//0 - ничего нет, 0x01 - PRUS, 0x02 - HCP, 0x04 - Atten,
											//0x08 - SD, 0x10 - Ethernet, 0x20 - 1-Wire, 0x40 - PWM, 0x80 - DigAnaliz
	unsigned short	QuantityChannelADC;		//Максимальное количество каналов в модуле АЦП
	unsigned short	QuantityChannelDAC;		//Максимальное количество каналов в модуле ЦАП

	// Конфигурация каналов
	unsigned long	Channel;				//битовая комбинация вводимых каналов
	unsigned long	ChannelDAC;				//битовая комбинация выводимых каналов
	unsigned long	HCPChannel;				//битовая комбинация включенных каналов HCP
	unsigned long	OverloadChannel;		//битовая комбинация перегруженных каналов
	unsigned short	WorkChADC;				//количество работающих каналов АЦП
	unsigned short	WorkChDAC;				//количество работающих каналов ЦАП

	// Конфигурация коэф. усиления
	unsigned short	CodAmplify[32];			//коды усиления на АЦП
	unsigned short	PRUS[32];				//коды усиления усилителя заряда / вкл. (выкл.) HCP
	unsigned short	Atten[4];				//коды аттенюатора ЦАП

	// Конфигурация частот
	unsigned short	EnaExtStart;			//разрешение внешнего старта 0-внутр. 1-внеш
	unsigned short	EnaExtFreq;				//разрешение внешней частоты 0-внутр. 1-внеш
	unsigned short	EnaCycle;				//разрешение циклического накопления 0-однократно, 1-циклически
	unsigned short	EnaCycleDAC;			//разрешение циклического накопления 0-однократно, 1-циклически
	unsigned short	ExtCycleDAC;			//0 - генерация сигнала из внутренней памяти DSP
											//1 - генерация сигнала из внешней памяти с подкачкой по прерываниям
	unsigned short	ModaADC;				//мода работы АЦП 0-1, 1-2, 2-4, 3-5, 4-8, 5-10 прореживание
	unsigned short	Rate;					//коэффициент деления опорной частоты
	unsigned short	RateDAC;				//коэффициент деления опорной частоты

	// Конфигурация буферов
	unsigned short	SizeInterrupt;			//размер для перекачки данных АЦП при каждом прерывании
	unsigned short	SizeInterruptDAC;		//размер для перекачки данных ЦАП при каждом прерывании
	unsigned long	SizeBufferDAC;			//размер буфера ЦАП SDRAM в словах

	// Переменные отслеживающие временные параметры
	ULONGLONG		Flag;					//количество прерываний

	// Прочие переменные
	unsigned short	AdrDSP;					//Adress in DSP
	unsigned short	Count;					//Количество слов для передачи
	unsigned short	NumberChannel;			//Номер канала (служебная переменная)
	unsigned long	DigitalInput;			//Данные цифрового порта
	unsigned long	DigitalOutput;			//Данные цифрового порта
	unsigned short	TestCode[6];			//Тестовые проверки
	char			VersionDSP[64];			//Версия DSP
	unsigned long	SerialNumber;			//Серийный номер DSP
	unsigned long	ModelNumber;			//Тип платы с DSP
	unsigned long	Maska;					//Программируемая маска
	unsigned long	ChannelDiff;			//битовая комбинация дифференциальных каналов
	unsigned long	DigitalOutEnable;		//битовая комбинация выходов цифрового порта

	unsigned long	DigitalResolutionLowADC;	//вес младшего разряда АЦП (младшая часть)
	unsigned long	DigitalResolutionHighADC;	//вес младшего разряда АЦП (старшая часть)
	unsigned long	DigitalResolutionLowDAC;	//вес младшего разряда ЦАП (младшая часть)
	unsigned long	DigitalResolutionHighDAC;	//вес младшего разряда ЦАП (старшая часть)


	unsigned long	DigitalResolChanADC[32];	//откалиброванный вес младшего разряда АЦП
	unsigned long	DigitalResolChanDAC[4];		//откалиброванный вес младшего разряда ЦАП
	ULONGLONG		SystemTime;					//системное время формата UTC (структура FILETIME)

	// ШИМ
	unsigned short	StartPWM;				//Запуск ШИМ (0x01 - старт 1 канала, 0x02 - старт 2 канала, 0x04 - старт 3 канала;
											//			  0x100 - стоп 1 канала, 0x200 - стоп 2 канала, 0x400 - стоп 3 канала)
	unsigned short	RatePWM;				//Коэф. деления опорной частоты для ШИМ
	unsigned short	PeriodPWM;				//Период ШИМ	
	unsigned short	OnDutyPWM0;				//Скважность ШИМ (канал 0)
	unsigned short	OnDutyPWM1;				//Скважность ШИМ (канал 1)
	unsigned short	OnDutyPWM2;				//Скважность ШИМ (канал 2)
	unsigned short	ShiftPWM1;				//Сдвиг ШИМ (канал 0)
	unsigned short	ShiftPWM2;				//Сдвиг ШИМ (канал 1)
	
	unsigned short	Reserved[4];			//Зарезервировано
} ADC_INFO_USB, *PADC_INFO_USB;


typedef struct _ADC_INFO_USB2 {
	// Команды DSP
	unsigned short	Command;				//Команда
	unsigned short	TypeADC;				//Режим работы DSP: 0 - ADC, 1 - DAC
	signed short	Start;					//состояние ввода данных 1- ввод
	signed short	StartDAC;				//состояние ввода данных 1- ввод

	// Аппаратная конфигурация DSP
	unsigned short	EnableADC;				//Наличие АЦП
	unsigned short	EnableDAC;				//Наличие ЦАП
	unsigned short	EnableExtDevice;		//0 - ничего нет, 0x01 - PRUS, 0x02 - HCP, 0x04 - Atten,
											//0x08 - SD, 0x10 - Ethernet, 0x20 - 1-Wire, 0x40 - PWM, 0x80 - DigAnaliz
	unsigned short	QuantityChannelADC;		//Максимальное количество каналов в модуле АЦП
	unsigned short	QuantityChannelDAC;		//Максимальное количество каналов в модуле ЦАП
	unsigned char	TypeDataADC;			//Тип данных АЦП 0-16бит, 1-32бит, 2-float 
	unsigned char	TypeDataDAC;			//Тип данных АЦП 0-16бит, 1-32бит, 2-float 
//unsigned short Reserved0;	// выравнивание
	
	// Конфигурация каналов
	unsigned long	Channel;				//битовая комбинация вводимых каналов
	unsigned long	ChannelDAC;				//битовая комбинация выводимых каналов
	unsigned long	HCPChannel;				//битовая комбинация включенных каналов HCP
	unsigned long	OverloadChannel;		//битовая комбинация перегруженных каналов
	unsigned short	WorkChADC;				//количество работающих каналов АЦП
	unsigned short	WorkChDAC;				//количество работающих каналов ЦАП

	// Конфигурация коэф. усиления
	unsigned short	CodAmplify[16];			//коды усиления на АЦП
unsigned short Reserved1[16];	
	unsigned short	PRUS[16];				//коды усиления усилителя заряда / вкл. (выкл.) HCP
unsigned short Reserved2[16];	
	unsigned short	Atten[4];				//коды аттенюатора ЦАП

	// Конфигурация частот
//	unsigned short	EnaExtStart;			//разрешение внешнего старта 0-внутр. 1-внеш
//	unsigned short	EnaExtFreq;				//разрешение внешней частоты 0-внутр. 1-внеш
//	unsigned short	EnaCycle;				//разрешение циклического накопления 0-однократно, 1-циклически
//	unsigned short	EnaCycleDAC;			//разрешение циклического накопления 0-однократно, 1-циклически
//	unsigned short	ExtCycleDAC;			//0 - генерация сигнала из внутренней памяти DSP
											//1 - генерация сигнала из внешней памяти с подкачкой по прерываниям

unsigned long	ProgramMask;	// лицензия программного кода (LAN)
unsigned long	ProgramMaskExt;	//
	// Конфигурация частот
	unsigned short	EnaExtStartFreq;		//разрешение внешнего старта, внешней частоты 0-внутр. 1-внеш
											// 0 бит - внешний старт АЦП
											// 1 бит - внешний старт ЦАП
											// 2 бит - внешняя опорная частота АЦП
											// 3 бит - внешняя опорная частота ЦАП
											// 4 бит - работа ЦАП без подкачки данных извне
	unsigned short	ModaADC;				//мода работы АЦП 0-1, 1-2, 2-4, 3-5, 4-8, 5-10 прореживание
	unsigned short	Rate;					//коэффициент деления опорной частоты
	unsigned short	RateDAC;				//коэффициент деления опорной частоты

	// Конфигурация буферов
	unsigned short	SizeInterrupt;			//размер для перекачки данных при каждом прерывании
	unsigned short	SizeInterruptDAC;		//размер для перекачки данных при каждом прерывании
//	unsigned short	PointerDSP;				//индекс прошедшего накопления в DSP
	unsigned long	SizeBufferDAC;			//размер буфера ЦАП SDRAM в словах

	// Переменные отслеживающие временные параметры
	ULONGLONG		Flag;					//количество прерываний

	// Прочие переменные
//	unsigned short	AdrDSP;					//Adress in DSP
//	unsigned short	Count;					//Количество слов для передачи
//	unsigned short	NumberChannel;			//Номер канала (служебная переменная)
unsigned short Reserved3[4];	

	unsigned long	DigitalInput;			//Данные цифрового порта
	unsigned long	DigitalOutput;			//Данные цифрового порта
	unsigned short	TestCode[6];			//Тестовые проверки
	char			VersionDSP[32];			//Версия DSP
unsigned short Reserved4[16];	
	unsigned long	SerialNumber;			//Серийный номер DSP
	unsigned long	ModelNumber;			//Тип платы с DSP
	unsigned long	Maska;					//Программируемая маска
	unsigned long	ChannelDiff;			//битовая комбинация дифференциальных каналов
	unsigned long	DigitalOutEnable;		//битовая комбинация выходов цифрового порта

	unsigned long	DigitalResolutionADC;		//вес младшего разряда АЦП (младшая часть)
	unsigned long	DigitalResolutionADCExt;	//вес младшего разряда АЦП (старшая часть)
	unsigned long	DigitalResolutionDAC;		//вес младшего разряда ЦАП (младшая часть)
	unsigned long	DigitalResolutionDACExt;	//вес младшего разряда ЦАП (старшая часть)


	unsigned long	DigitalResolChanADC[16];	//откалиброванный вес младшего разряда АЦП
	unsigned long	Amplify[16];				//откалиброванный коэф. усиления
	unsigned long	DigitalResolChanDAC[4];		//откалиброванный вес младшего разряда ЦАП
	ULONGLONG		SystemTime;					//системное время формата UTC (структура FILETIME)

	// ШИМ
	unsigned short	StartPWM;				//Запуск ШИМ (0x01 - старт 1 канала, 0x02 - старт 2 канала, 0x04 - старт 3 канала;
											//			  0x100 - стоп 1 канала, 0x200 - стоп 2 канала, 0x400 - стоп 3 канала)
	unsigned short	RatePWM;				//Коэф. деления опорной частоты для ШИМ
	unsigned short	PeriodPWM;				//Период ШИМ	
	unsigned short	OnDutyPWM0;				//Скважность ШИМ (канал 0)
	unsigned short	OnDutyPWM1;				//Скважность ШИМ (канал 1)
	unsigned short	OnDutyPWM2;				//Скважность ШИМ (канал 2)
	unsigned short	ShiftPWM1;				//Сдвиг ШИМ (канал 0)
	unsigned short	ShiftPWM2;				//Сдвиг ШИМ (канал 1)
	
	unsigned short	Reserved[4];			//Зарезервировано
} ADC_INFO_USB2, *PADC_INFO_USB2;



typedef struct _COMMAND_INFO_USB {
	// Команды DSP
	unsigned short	Command;			//команда
	unsigned short	Error;				//дополнительный код или ошибка
	unsigned long	Size;				//размер в байтах
	unsigned short	Data[252];			//данные
} COMMAND_INFO_USB, *PCOMMAND_INFO_USB;

#endif

