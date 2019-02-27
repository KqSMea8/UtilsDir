// adc_info_usb.h
#ifndef __adc_info_usb_h__
#define __adc_info_usb_h__

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
	unsigned short	SizeInterrupt;			//размер для перекачки данных при каждом прерывании
	unsigned short	SizeInterruptDAC;			//размер для перекачки данных при каждом прерывании
	unsigned short	PointerDSP;				//индекс прошедшего накопления в DSP

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


typedef struct _COMMAND_INFO_USB {
	// Команды DSP
	unsigned short	Command;			//команда
	unsigned short	Error;				//дополнительный код или ошибка
	unsigned long	Size;				//размер в байтах
	unsigned short	Data[252];			//данные
} COMMAND_INFO_USB, *PCOMMAND_INFO_USB;

#endif
