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
											//0x08 - SD, 0x10 - Ethernet, 0x20 - Bluetooth, 0x40 - PWM, 0x80 - DigAnaliz,
											//0x100 - 1-Wire, 0x200 - Подстройка по GPS, 0x400 - Синхронизация по PTP
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
	unsigned short	TypeConnection;			//тип интерфейса (1 - PCI, 2 - USB, 3 - Ethernet, 4 - Bluetouch) (был выравниванием!!!)
	double			Amplitude[32];			//коэффициент усиления усилителя АЦП
	unsigned short	PRUS[32];				//коды усиления усилителя заряда / вкл. (выкл.) HCP
	double			PreAmplitude[32];		//коэффициент усиления предусилителя
	unsigned short	Atten[4];				//коды аттенюатора ЦАП
	double			Reduction[4];			//коэффициент передачи аттенюатора ЦАП

	// Конфигурация частот
	unsigned short	EnaExtStart;			//разрешение внешнего старта АЦП 0-внутр. 1-внеш
	unsigned short	EnaExtFreq;				//разрешение внешней частоты ЦАП 0-внутр. 1-внеш
	unsigned short	EnaCycle;				//разрешение циклического накопления 0-однократно, 1-циклически
	unsigned short	ExtCycle;				//MasterSynchr: 1 - Задатчик синхронизации, 0 - ведомое устройство в синхронизации
	unsigned short	ModaADC;				//мода работы АЦП 0-1, 1-2, 2-4, 3-5, 4-8, 5-10 прореживание

	unsigned short	EnaExtStartDAC;			//разрешение внешнего старта ЦАП 0-внутр. 1-внеш (был выравниванием!!!)
	unsigned short	EnaExtFreqDAC;			//разрешение внешней частоты ЦАП 0-внутр. 1-внеш (был выравниванием!!!)
unsigned short	Reserv1;				//


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
	unsigned short	QuantityChannelVirtual;	//Максимальное количество виртуальных каналов

unsigned short	Reserv3;				//

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
	char			verDSP[32];				//Версия DSP
	char			DeviceName[16];			//Имя прибора
char			Reserv4[16];			//Зарезервировано
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

	unsigned short	Revision;				//Ревизия устройства

	unsigned short	Reserved[9];			//Зарезервировано
} ADC_INFO, *PADC_INFO;



// sizeof(ADC_INFO_EXT) = 2048 байт
typedef struct _ADC_INFO_EXT{
	// Аппаратная конфигурация DSP
	unsigned short	numberDSP;				//Порядковый номер DSP на плате
	unsigned short	typeADC;				//Режим работы DSP: 0 - ADC, 1 - DAC
	unsigned short	EnableADC;				//Наличие АЦП
	unsigned short	EnableDAC;				//Наличие ЦАП
	unsigned short	EnableExtDevice;		//0 - ничего нет, 0x01 - PRUS, 0x02 - HCP, 0x04 - Atten,
											//0x08 - SD, 0x10 - Ethernet, 0x20 - Bluetooth, 0x40 - PWM, 0x80 - DigAnaliz(Расширинный режим),
											//0x100 - 1-Wire, 0x200 - Подстройка по GPS, 0x400 - синхронизация PTP, 0x800 - мастер PTP
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
	unsigned short	TypeConnection;			//тип интерфейса (1 - PCI, 2 - USB, 3 - Ethernet, 4 - Bluetouch) (был выравниванием!!!)
	double			Amplitude[32];			//коэффициент усиления усилителя АЦП
	unsigned short	PRUS[32];				//коды усиления усилителя заряда / вкл. (выкл.) HCP
	double			PreAmplitude[32];		//коэффициент усиления предусилителя
	unsigned short	Atten[4];				//коды аттенюатора ЦАП
	double			Reduction[4];			//коэффициент передачи аттенюатора ЦАП

	// Конфигурация частот
	unsigned short	EnaExtStart;			//разрешение внешнего старта АЦП 0-внутр. 1-внеш
	unsigned short	EnaExtFreq;				//разрешение внешней частоты АЦП 0-внутр. 1-внеш
	unsigned short	EnaCycle;				//разрешение циклического накопления 0-однократно, 1-циклически
//	unsigned short	ExtCycle;				//0 - генерация сигнала из внутренней памяти DSP, без подкачки из внешней памяти
	unsigned short	MasterSynchr;			//1 - Задатчик синхронизации, 0 - ведомое устройство в синхронизации
	unsigned short	ModaADC;				//мода работы АЦП 0-1, 1-2, 2-4, 3-5, 4-8, 5-10 прореживание

	unsigned short	EnaExtStartDAC;			//разрешение внешнего старта ЦАП 0-внутр. 1-внеш (был выравниванием!!!)
	unsigned short	EnaExtFreqDAC;			//разрешение внешней частоты ЦАП 0-внутр. 1-внеш (был выравниванием!!!)
unsigned short	Reserv1;				//

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
	unsigned short	QuantityChannelVirtual;	//Максимальное количество виртуальных каналов

unsigned short	Reserv3;				//

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
	char			verDSP[32];				//Версия DSP
	char			DeviceName[16];			//Имя прибора
char			Reserv4[16];			//Зарезервировано
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

	unsigned short	Revision;				//Ревизия устройства
	unsigned short	Reserved[7];			//Зарезервировано (НЕ СОВПАДАЕТ С СТРУКТУРОЙ ADC_INFO !!!!!!!!!!!!!!!!!!!!!!!!!)

	float			DigitalResolChanADC[32];//откалиброванный вес младшего разряда АЦП
	float			DigitalResolChanDAC[4];	//откалиброванный вес младшего разряда ЦАП

	unsigned long	SizeBufferDSP_DAC;		// Размер буфера SDRAM ЦАП в DSP
	unsigned long	MaxSizeBufferDSP_DAC;	// Макс. размер буфера SDRAM ЦАП в DSP

	double			ExtOporFreqDAC;			//внешняя опорная частота

	long			OffsetChanADC[32];		//откалиброванное смещение АЦП
	long			OffsetChanDAC[4];		//откалиброванное смещение ЦАП

	ULONGLONG		StartTime;				//Дата и время синхронного запуска
	ULONGLONG		GPSTime;				//Дата и время приемника GPS
	LONGLONG		Drift;					//Текущее смещение отн-но ведущего устр-ва (младшие 4 байта)
	LONGLONG		SyncDrift;				//Текущее смещение в момент последней синхронизации
	unsigned char	SyncCount;				//Счетчик синхронизаций
	unsigned char	SyncFlag;				//Флаг синхронизации
	unsigned char	SatsInUse;				//Кол-во навигационных спутников, доступных для использования
	unsigned char	StatePTP;				//Состояние порта PTP (0 - PTP недоступен, 3 - Disabled, 6 - Master, 9 - Slave)
	unsigned char	BatteryCharge;			//Уровень заряда аккумулятора (1..100, если 0 - информация недоступна)
	char			BoardTemperature;		//Температура платы в градусах цельсия
	unsigned short	BatteryVoltage;			//Уровень напряжения аккумулятора в мВ (0 если недоступно)
	unsigned short	BatteryStatus;			//На будущее (возможно, состояние аккумулятора)
	unsigned short	BatteryStatus2;			//На будущее (возможно, ток заряда)

	long			SDTotalSectors;			//Общее количество секторов на SD карте (0 - нет информации, < 0 - ошибка)
	long			SDFreeSectors;			//Количество свободных секторов на SD карте (0 - нет информации, < 0 - ошибка)

	unsigned short	BuiltinGenSupport;		//Маска поддержки встроенного генератора (биты: 0 - включение, 1 - добавление синуса)
	unsigned short	BuiltinGenControl;		//Маска управления встроенным генератором (биты: 0 - включение, 1 - добавление синуса)
	long			BuiltinGenSineFreq;		//Частота синуса, Гц
	long			BuiltinGenSineAmpl;		//Амплитуда синуса (не СКЗ), код ЦАП
	long			BuiltinGenSineBias;		//Смещение синуса, код ЦАП

	unsigned short	DACMUX_Support;			// флаги поддерживаемых режимов DACMUX
											// 0x0001 - код 0x35 (СУВ)
	unsigned short	DACMUX_Active;			// флаги активации режимов DACMUX
											// 0x0001 - код 0x35 (СУВ)
	unsigned short	DACMUX_WaitCode;		// если старший бит не 0, то режим ожидания, младшие 15 бит - код причины входа в режим
	unsigned short	DACMUX_WaitReset;		// если не 0, выйти из режима ожидания
	unsigned long	DACMUX_Diagnost;		// диагностическая информация, пока передаётся количество отсчётов опережения в буфере ЦАП
	unsigned long	DACMUX_DriverAhead;		// опережение, вычисляемое драйвером
	unsigned short	DACMUX_WaitSet;			// код экстренной остановки (если не 0)
	unsigned short	DACMUX_Reserved;
	unsigned long	DACMUX_FrameBoost;		// на сколько увеличивается скорость приема данных

	unsigned long	Stats_CountStarts;		// количество стартов АЦП с момента подключения драйвера
	ULONGLONG		Stats_CountADC;			// количество принятых слов АЦП с момента последнего старта
	ULONGLONG		Stats_CountDAC;			// количество отправленных слов ЦАП с момента последнего старта
	ULONGLONG		Stats_TimeADC;			// время работы АЦП с момента последнего старта, в мс
	ULONGLONG		Stats_TimeDAC;			// время работы ЦАП с момента последнего старта, в мс

	unsigned char	SyncAdjustment;			//Текущее значение подстройки в процентах (1..100, если 0 - нет информации)
	unsigned char	ReservedChars[3];		//Зарезервировано

	unsigned long	DeviceUptime;			//Время работы устройства с момента включения, с
	unsigned long	DriverUptime;			//Время работы драйвера с момент подключения, с
	unsigned short	ReservedExt[212];		//Зарезервировано
} ADC_INFO_EXT, *PADC_INFO_EXT;


// sizeof(SYNHR_QUARTZ_INFO) = 64 байт
typedef struct _SYNHR_QUARTZ_INFO
{
	ULONGLONG		FreqCounterPC;
	ULONGLONG		StartCounterPC;
	ULONGLONG		EndCounterPC;
	ULONGLONG		DurationSynhrPC;
	unsigned long	TimerDSP;
	unsigned long	NumberSecondDSP;
	unsigned long	FreqQuartzDSP;
	unsigned long	Reserved1;
	unsigned long	Reserved2;
	unsigned long	Reserved3;
	unsigned long	Reserved4;
	unsigned long	Reserved5;
} SYNHR_QUARTZ_INFO, *PSYNHR_QUARTZ_INFO;

#ifndef _COMMAND_INFO_USB_h
#define _COMMAND_INFO_USB_h
typedef struct _COMMAND_INFO_USB {
	// Команды DSP
	unsigned short	Command;			//команда
	unsigned short	Error;				//дополнительный код или ошибка
	unsigned long	Size;				//размер в байтах
	unsigned short	Data[252];			//данные
} COMMAND_INFO_USB, *PCOMMAND_INFO_USB;
#endif



#endif
