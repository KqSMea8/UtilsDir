// adcinfo.h
#ifndef __adcinfo_h__
#define __adcinfo_h__

// sizeof(ADC_INFO) = 1176 байт
typedef struct _ADC_INFO {
	// Аппаратная конфигурация DSP
	unsigned short	numberDSP;				//Порядковый номер DSP на плате
	unsigned short	typeADC;				//Режим работы DSP: 0 - ADC, 1 - DAC
	unsigned short	EnableADC;				//Наличие АЦП
	unsigned short	EnableDAC;				//Наличие ЦАП
	unsigned short	EnableExtDevice;		//0 - ничего нет, 0x01 - PRUS, 0x02 - HCP, 0x04 - Atten,
											//0x08 - SD, 0x10 - Ethernet, 0x20 - 1-Wire, 0x40 - PWM, 0x80 - DigAnaliz

	// Запуск DSP
	short			Start;					//состояние ввода данных 1- ввод

	// Конфигурация каналов
	unsigned long	Channel;				//битовая комбинация вводимых каналов
	unsigned long	HCPChannel;				//битовая комбинация включенных каналов HCP
	unsigned long	OverloadChannel;		//битовая комбинация перегруженных каналов
	unsigned short	WorkChADC;				//количество работающих каналов АЦП
	unsigned short	WorkChDAC;				//количество работающих каналов ЦАП

	// Конфигурация коэф. усиления
	unsigned short	KodAmplify[32];			//коды усиления на АЦП
	unsigned short	Reserv0;				//
	double			Amplitude[32];			//коэффициент усиления усилителя АЦП
	unsigned short	PRUS[32];				//коды усиления усилителя заряда / вкл. (выкл.) HCP
	double			PreAmplitude[32];		//коэффициент усиления предусилителя
	unsigned short	Atten[4];				//коды аттенюатора ЦАП
	double			Reduction[4];			//коэффициент передачи аттенюатора ЦАП

	// Конфигурация частот
	unsigned short	EnaExtStart;			//разрешение внешнего старта 0-внутр. 1-внеш
	unsigned short	EnaExtFreq;				//разрешение внешней частоты 0-внутр. 1-внеш
	unsigned short	EnaCycle;				//разрешение циклического накопления 0-однократно, 1-циклически
	unsigned short	ExtCycle;				//0 - генерация сигнала из внутренней памяти DSP, без подкачки из внешней памяти
											//1 - генерация сигнала из внешней памяти с подкачкой по прерываниям
	unsigned short	ModaADC;				//мода работы АЦП 0-1, 1-2, 2-4, 3-5, 4-8, 5-10 прореживание

unsigned short	Reserv1;				//
unsigned short	Reserv2;				//
unsigned short	Reserv3;				//


	double			Freq;					//текущая частота преобразования (Freq = OporFreq/Rate)
	double			OporFreq;				//Опорная частота
	double			ExtOporFreq;			//внешняя опорная частота
	unsigned long	Rate;					//коэффициент деления опорной частоты

	// Конфигурация буферов
	unsigned long	sizeInterrupt;			//размер для перекачки данных при каждом прерывании
											//(sizeInterrupt = QuantityPacketsADC * SizePacketADC)
	short			*AddressBuffer;			//Адрес буфера в системной области
	short			*AddressBufferDAC;		//Адрес буфера в системной области
	short			*AddressUserBuffer;		//Адреса буфера в пользовательской области
	short			*AddressUserBufferDAC;	//Адрес буфера в пользовательской области
	unsigned long	SizeBuffer;				//Размер буфера ввода не более 1 000 000 отсчетов
	unsigned long	SizeBufferDAC;			//Размер буфера ввода не более 1 000 000 отсчетов
	unsigned long	Pointer;				//индекс текущего накопления в буфере АЦП
	unsigned long	PointerDAC;				//индекс текущего накопления в буфере ЦАП
	unsigned long	PointerDSP;				//индекс прошедшего накопления в DSP

	// Аппаратные параметры АЦП (ЦАП)
	unsigned short	Bits;					//количество бит в АЦП преобразователе
	unsigned short	BitsDAC;				//количество бит в ЦАП
	unsigned short	Words;					//количество 16-разрядных слов в одном отсчете
	unsigned short	WordsDAC;				//количество 16-разрядных слов в одном отсчете
	unsigned short	QuantityChannelDigPort;	// количество линий цифрового порта (был выравниванием!!!)
	double			DigitalResolution;		//вес младшего разряда АЦП
	double			DigitalResolutionDAC;	//вес младшего разряда ЦАП
	double			MaxFreq;				//Максимальная частота преобразования
	unsigned short	QuantityChannelADC;		//Максимальное количество каналов в модуле АЦП
	unsigned short	QuantityChannelDAC;		//Максимальное количество каналов в модуле ЦАП

unsigned short	Reserv4;				//
unsigned short	Reserv5;				//

	// Переменные отслеживающие временные параметры
	ULONGLONG		Flag;					//количество прерываний
	ULONGLONG		CurrentCounter;			//Значение счетчика в момент опроса
	ULONGLONG		IrqCounter;				//Значение счетчика в текущее прерывание
	ULONGLONG		IrqPeriod;				//Текущий период прерываний
	ULONGLONG		FreqCounter;			//Частота счетчика

	// Прочие переменные
	unsigned long	MyError;				//ошибка ввода или задания параметров
	unsigned short	modify;					//номер изменения, каждое существенное изменение параметров устройства
											//делает инкремент этого параметра
	unsigned long	AdrDSP;					//Adress in DSP
	void			*AdrMem;				//Adress of Memory for DSP
	unsigned long	count;					//Количество слов для передачи
	unsigned long	numberCHNL;				//Номер канала (служебная переменная)
	unsigned short	TestCode[6];			//Тестовые проверки
	char			verDSP[64];				//Версия DSP
	char			verDriver[64];			//Версия драйвера
	
	unsigned long	sizeInterruptDAC;		//размер для перекачки данных при каждом прерывании
											//(sizeInterruptDAC = QuantityPacketsDAC * SizePacketDAC)
	long			StartDAC;				//состояние ввода данных 1- ввод
	unsigned long	MaxSizeInterrupt;		//Максимально возможный размер для перекачки данных при каждом прерывании
	unsigned long	MaxSizeInterruptDAC;	//Максимально возможный размер для перекачки данных при каждом прерывании
	unsigned long	ChannelDAC;				//битовая комбинация выводимых каналов
	unsigned long	EnaCycleDAC;			//разрешение циклического накопления 0-однократно, 1-циклически
	unsigned long	ExtCycleDAC;			//0 - генерация сигнала из внутренней памяти DSP, без подкачки из внешней памяти
											//1 - генерация сигнала из внешней памяти с подкачкой по прерываниям
	double			FreqDAC;				//текущая частота преобразования (FreqDAC = OporFreqDAC/RateDAC)
	double			OporFreqDAC;			//Опорная частота
	double			MaxFreqDAC;				//Максимальная частота преобразования
	unsigned long	RateDAC;				//коэффициент деления опорной частоты
	unsigned long	DigitalInput;			//Данные цифрового порта
	unsigned long	DigitalOutput;			//Данные цифрового порта
	unsigned long	StopDevice;				//Останов устройства
	unsigned long	NumVersionDriver;		//Бинарная версия драйвера
	unsigned long	ChannelDiff;			//битовая комбинация дифференциальных каналов
	unsigned long	DigitalOutEnable;		//битовая комбинация выходов цифрового порта
	unsigned short	SizePacketADC;			//Кол-во слов данных в пакете АЦП
	unsigned short	SizePacketDAC;			//Кол-во слов данных в пакете ЦАП
	unsigned short	MaxSizePacketADC;		//Макс. кол-во слов данных в пакете АЦП
	unsigned short	MaxSizePacketDAC;		//Макс. кол-во слов данных в пакете ЦАП
	unsigned short	QuantityPacketsADC;		//Кол-во пакетов передаваемых за одно прерывание АЦП
	unsigned short	QuantityPacketsDAC;		//Кол-во пакетов передаваемых за одно прерывание ЦАП
	unsigned short	MaxQuantityPacketsADC;	//Макс. кол-во пакетов передаваемых за одно прерывание АЦП
	unsigned short	MaxQuantityPacketsDAC;	//Макс. кол-во пакетов передаваемых за одно прерывание ЦАП

	unsigned short	StartPWM;				//Запуск ШИМ (0x01 - старт 1 канала, 0x02 - старт 2 канала, 0x04 - старт 3 канала;
											//			  0x100 - стоп 1 канала, 0x200 - стоп 2 канала, 0x400 - стоп 3 канала)
	unsigned short	RatePWM;				//Коэф. деления опорной частоты для ШИМ
	unsigned short	PeriodPWM;				//Период ШИМ	
	unsigned short	OnDutyPWM0;				//Скважность ШИМ (канал 0)
	unsigned short	OnDutyPWM1;				//Скважность ШИМ (канал 1)
	unsigned short	OnDutyPWM2;				//Скважность ШИМ (канал 2)
	unsigned short	ShiftPWM1;				//Сдвиг ШИМ (канал 0)
	unsigned short	ShiftPWM2;				//Сдвиг ШИМ (канал 1)

	unsigned short	Reserved[10];			//Зарезервировано
} ADC_INFO, *PADC_INFO;



// sizeof(ADC_INFO_EXT) = 2048 байт
typedef struct _ADC_INFO_EXT{
	// Аппаратная конфигурация DSP
	unsigned short	numberDSP;				//Порядковый номер DSP на плате
	unsigned short	typeADC;				//Режим работы DSP: 0 - ADC, 1 - DAC
	unsigned short	EnableADC;				//Наличие АЦП
	unsigned short	EnableDAC;				//Наличие ЦАП
	unsigned short	EnableExtDevice;		//0 - ничего нет, 1 - PRUS, 2 - HCP

	// Запуск DSP
	short	Start;							//состояние ввода данных 1- ввод

	// Конфигурация каналов
	unsigned long	Channel;				//битовая комбинация вводимых каналов
	unsigned long	HCPChannel;				//битовая комбинация включенных каналов HCP
	unsigned long	OverloadChannel;		//битовая комбинация перегруженных каналов
	unsigned short	WorkChADC;				//количество работающих каналов АЦП
	unsigned short	WorkChDAC;				//количество работающих каналов ЦАП

	// Конфигурация коэф. усиления
	unsigned short	KodAmplify[32];			//коды усиления на АЦП
unsigned short	Reserv0;				//
	double			Amplitude[32];			//коэффициент усиления усилителя АЦП
	unsigned short	PRUS[32];				//коды усиления усилителя заряда / вкл. (выкл.) HCP
	double			PreAmplitude[32];		//коэффициент усиления предусилителя
	unsigned short	Atten[4];				//коды аттенюатора ЦАП
	double			Reduction[4];			//коэффициент передачи аттенюатора ЦАП

	// Конфигурация частот
	unsigned short	EnaExtStart;			//разрешение внешнего старта 0-внутр. 1-внеш
	unsigned short	EnaExtFreq;				//разрешение внешней частоты 0-внутр. 1-внеш
	unsigned short	EnaCycle;				//разрешение циклического накопления 0-однократно, 1-циклически
	unsigned short	ExtCycle;				//0 - генерация сигнала из внутренней памяти DSP, без подкачки из внешней памяти
											//1 - генерация сигнала из внешней памяти с подкачкой по прерываниям
	unsigned short	ModaADC;				//мода работы АЦП 0-1, 1-2, 2-4, 3-5, 4-8, 5-10 прореживание

unsigned short	Reserv1;				//
unsigned short	Reserv2;				//
unsigned short	Reserv3;				//

	double			Freq;					//текущая частота преобразования (Freq = OporFreq/Rate)
	double			OporFreq;				//Опорная частота
	double			ExtOporFreq;			//внешняя опорная частота
	unsigned long	Rate;					//коэффициент деления опорной частоты

	// Конфигурация буферов
	unsigned long	sizeInterrupt;			//размер для перекачки данных при каждом прерывании
											//(sizeInterrupt = QuantityPacketsADC * SizePacketADC)
	short			*AddressBuffer;			//Адрес буфера в системной области
	short			*AddressBufferDAC;		//Адрес буфера в системной области
	short			*AddressUserBuffer;		//Адреса буфера в пользовательской области
	short			*AddressUserBufferDAC;	//Адрес буфера в пользовательской области
	unsigned long	SizeBuffer;				//Размер буфера ввода не более 1 000 000 отсчетов
	unsigned long	SizeBufferDAC;			//Размер буфера ввода не более 1 000 000 отсчетов
	unsigned long	Pointer;				//индекс текущего накопления в буфере АЦП
	unsigned long	PointerDAC;				//индекс текущего накопления в буфере ЦАП
	unsigned long	PointerDSP;				//индекс прошедшего накопления в DSP

	// Аппаратные параметры АЦП (ЦАП)
	unsigned short	Bits;					//количество бит в АЦП преобразователе
	unsigned short	BitsDAC;				//количество бит в ЦАП
	unsigned short	Words;					//количество 16-разрядных слов в одном отсчете
	unsigned short	WordsDAC;				//количество 16-разрядных слов в одном отсчете
unsigned short	QuantityDigitalChannel;				//
	double			DigitalResolution;		//вес младшего разряда АЦП
	double			DigitalResolutionDAC;	//вес младшего разряда ЦАП
	double			MaxFreq;				//Максимальная частота преобразования
	unsigned short	QuantityChannelADC;		//Максимальное количество каналов в модуле АЦП
	unsigned short	QuantityChannelDAC;		//Максимальное количество каналов в модуле ЦАП

unsigned short	Reserv4;				//
unsigned short	Reserv5;				//

	// Переменные отслеживающие временные параметры
	ULONGLONG		Flag;					//количество прерываний
	ULONGLONG		CurrentCounter;			//Значение счетчика в момент опроса
	ULONGLONG		IrqCounter;				//Значение счетчика в текущее прерывание
	ULONGLONG		IrqPeriod;				//Текущий период прерываний
	ULONGLONG		FreqCounter;			//Частота счетчика

	// Прочие переменные
	unsigned long	MyError;				//ошибка ввода или задания параметров
	unsigned short	modify;					//номер изменения, каждое существенное изменение параметров устройства
											//делает инкремент этого параметра
	unsigned long	AdrDSP;					//Adress in DSP
	void			*AdrMem;				//Adress of Memory for DSP
	unsigned long	count;					//Количество слов для передачи
	unsigned long	numberCHNL;				//Номер канала (служебная переменная)
	unsigned short	TestCode[6];			//Тестовые проверки
	char			verDSP[64];				//Версия DSP
	char			verDriver[64];			//Версия драйвера
	
	unsigned long	sizeInterruptDAC;		//размер для перекачки данных при каждом прерывании
											//(sizeInterruptDAC = QuantityPacketsDAC * SizePacketDAC)
	long			StartDAC;				//состояние ввода данных 1- ввод
	unsigned long	MaxSizeInterrupt;		//Максимально возможный размер для перекачки данных при каждом прерывании
	unsigned long	MaxSizeInterruptDAC;	//Максимально возможный размер для перекачки данных при каждом прерывании
	unsigned long	ChannelDAC;				//битовая комбинация выводимых каналов
	unsigned long	EnaCycleDAC;			//разрешение циклического накопления 0-однократно, 1-циклически
	unsigned long	ExtCycleDAC;			//0 - генерация сигнала из внутренней памяти DSP, без подкачки из внешней памяти
											//1 - генерация сигнала из внешней памяти с подкачкой по прерываниям
	double			FreqDAC;				//текущая частота преобразования (FreqDAC = OporFreqDAC/RateDAC)
	double			OporFreqDAC;			//Опорная частота
	double			MaxFreqDAC;				//Максимальная частота преобразования
	unsigned long	RateDAC;				//коэффициент деления опорной частоты
	unsigned long	DigitalInput;			//Данные цифрового порта
	unsigned long	DigitalOutput;			//Данные цифрового порта
	unsigned long	StopDevice;				//Останов устройства
	unsigned long	NumVersionDriver;		//Бинарная версия драйвера
	unsigned long	ChannelDiff;			//битовая комбинация дифференциальных каналов
	unsigned long	DigitalOutEnable;		//битовая комбинация выходов цифрового порта
	unsigned short	SizePacketADC;			//Кол-во слов данных в пакете АЦП
	unsigned short	SizePacketDAC;			//Кол-во слов данных в пакете ЦАП
	unsigned short	MaxSizePacketADC;		//Макс. кол-во слов данных в пакете АЦП
	unsigned short	MaxSizePacketDAC;		//Макс. кол-во слов данных в пакете ЦАП
	unsigned short	QuantityPacketsADC;		//Кол-во пакетов передаваемых за одно прерывание АЦП
	unsigned short	QuantityPacketsDAC;		//Кол-во пакетов передаваемых за одно прерывание ЦАП
	unsigned short	MaxQuantityPacketsADC;	//Макс. кол-во пакетов передаваемых за одно прерывание АЦП
	unsigned short	MaxQuantityPacketsDAC;	//Макс. кол-во пакетов передаваемых за одно прерывание ЦАП

	// ШИМ
	signed short	StartPWM;				//Запуск ШИМ (1 - старт, -1 - стоп)
	unsigned short	RatePWM;				//Коэф. деления опорной частоты для ШИМ
	unsigned short	PeriodPWM;				//Период ШИМ	
	unsigned short	OnDutyPWM0;				//Скважность ШИМ (канал 0)
	unsigned short	OnDutyPWM1;				//Скважность ШИМ (канал 1)
	unsigned short	OnDutyPWM2;				//Скважность ШИМ (канал 2)
	unsigned short	ShiftPWM1;				//Сдвиг ШИМ (канал 0)
	unsigned short	ShiftPWM2;				//Сдвиг ШИМ (канал 1)

	unsigned short	Reserved[8];			//Зарезервировано (НЕ СОВПАДАЕТ С СТРУКТУРОЙ ADC_INFO !!!!!!!!!!!!!!!!!!!!!!!!!)

	float			DigitalResolChanADC[32];//откалиброванный вес младшего разряда АЦП
	float			DigitalResolChanDAC[4];	//откалиброванный вес младшего разряда ЦАП

	unsigned short	ReservedExt[366];			//Зарезервировано
} ADC_INFO_EXT, *PADC_INFO_EXT;


#endif
