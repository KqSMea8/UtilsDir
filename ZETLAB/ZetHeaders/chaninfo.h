// chaninfo.h
//
#ifndef __chaninfo_h__
#define __chaninfo_h__

#include "ADCINFO.h"
#include <sys/timeb.h>
#include <stdint.h>
#include "ZetTools\include\ZetTools.h" 

#define MAXUNITS			200
#define U_INVITE			-1
#define U_USED				1
#define U_VRTCHN			2
#define MAXDSP				137
#define MAXMODULE			50
#define MAXCHAN				400
#define MAXDECADE			5
#define MAXDACS				100
#define MODIFY				1
#define FILEMODE			2
#define STARTFILE			3
#define SETTIME				4
#define GPSSIZE				400
#define MAXCHANMOD			32
#define MODULEADCDAC		500
#define DIGEVENTSQUELEN     10

typedef enum _CHANNEL_STATUS
{
	csADC = 0,
	csDAC,
	csVirtual,
	csDigital,
	csLost,
	csLostDevice,
	csSmartSensor,
	csLostSmartSensor,
	csFastSmartSensor,
	csInactiveVirtual,
	csHiddenADC,
	csHiddenDAC
} CHANNEL_STATUS;

typedef enum _COMMAND_ID
{
	cidSetDigOutEnable = 0,
	cidGetDigOutEnable,
	cidGetDigInput,
	cidGetDigOutput,
	cidSetDigOutput,
	cidGetDigBits,
	cidSetBitDigOutput,
	cidClrBitDigOutput,
	cidGetFreqADC,
	cidGetFreqDAC,
	cidSetFreqADC,
	cidSetFreqDAC,
	cidGetNextFreqADC,
	cidGetNextFreqDAC,
	cidOnOffChannel,
	cidSinDiffChannel,
	cidEnableBuiltinGenerator,
	cid1PPSBuiltinGenerator,
#pragma region DAC MUX
	cidDACMUXMode,
	cidDACMUXSetWait,
	cidDACMUXResetWait,
#pragma endregion DAC MUX
} COMMAND_ID;

typedef struct _SRV_COMMAND_INFO
{
	union
	{
		struct
		{
			long lParametr1;
			long lParametr2;
		} Parameters_long_long;

		struct
		{
			long lParametr1;
			float fParametr2;
		} Parameters_long_float;

		struct
		{
			float fParametr1;
			float fParametr2;
		} Parameters_float_float;

		long lParameter_long;
		float fParameter_float;
	};

	union
	{
		unsigned long ulResult;
		long lResult;
		float fResult;
	};

	long lError;
	long lFlag;
	long lUse;
	long lCommandID;
	long lTypeDsp;
	long lNumberDsp;
	long lNeedRewriteDeviceCfg;		// флаг необходимости перезаписать device.cfg
	long lNeedRestart;				// флаг необходмости перестартовки АЦП
} SRV_COMMAND_INFO, *pSRV_COMMAND_INFO;
////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _INTER_PROC
{	// межпроцессорный обмен
	int			Used;					// испльзование ячейки
	int			Dac;					// 1 - для генераторов, 0 - для остальных (используется при обработке файлов)
	HWND		hUnitWin;				// хэндлер окна класса unit в приложении
	int			EndFile;				// порция файла прочитана - 1
	wchar_t		UnitFileName[200];		// имя процесса с ".exe"
	int			FileNameWriten;			// индикатор присоединения к серверу, 1 - запрос на соединение, 0 - ответ
	short*		DacBuffer[MAXDACS];		// Адреса буферов ЦАП в памяти драйвера
	int			TypeSrvDac[MAXDACS];	// для отключения драйвера, запоминаются типы устройств
	int			NumSrvDac[MAXDACS];		// и номер сигнального процессора
	long		Guid[MAXDACS];			// идентификатор канала ЦАП
	int			TalkToBorland;			// запущен из Борланд Дельфи
	int			countlive;				// счетчик обращений к серверу для перезапуска сервера
	int			prevcountlive;
	HANDLE		hMFile;					// хэндлеры
	LPVOID		lpBaseAddress;			// и указатели на область данных, где находятся массивы данных
	int			NumSourse;
	SRV_COMMAND_INFO srvCommand;
 
 } INTER_PROC, *pINTER_PROC;
////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _CHANNEL_EVENT
{
	GUID guid;          // Уникальный идентификатор канала, по которому произошло событие изменения состояния
	long sec;           // Секундная составляющая времени
	long nanosec;       // Наносекундная составляющая времени
	long type;          // Тип события (6 - цифровое, 7 - событие начала, 8 - событие конца)
	char reserve[200];  // Место для хранения дополнительной информации по событию, в зависимости от типа события
} CHANNEL_EVENT, *pCHANNEL_EVENT;
////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _CHANNEL_EVENTS
{
	long currPos;   // Индекс для записи нового события в очередь
	CHANNEL_EVENT events[DIGEVENTSQUELEN];   // Очередь событий изменения состояния цифрового канала
} CHANNEL_EVENTS, *pCHANNEL_EVENTS;
////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _ALL_CHAN_INFO {				// информация работающих каналов
	int			IndexTable;						// индекс (строка) таблицы TABSIG.CFG
	int			NumModule;						// НОМЕР МОДУЛЯ
	int			NumContact;						// номер контакта в модуле
	int			TypeChannel;					// 0 - АЦП, 1 - ЦАП, 2 - виртуальный канал, 3-цифровой канал, 4-отмерший канал, 5 - упавшее устройство, 6 - интеллектуальный (от MODBUSZETLAB), 7 - отмерший виртуальный в MODBUSZETLAB 8-быстрый канал
	double		FreqVirt;						// частота виртуального канала
	int			UpDownInt;						// коэффициент прореживания ЦАП-АЦП целая (0-если плавающая)
	double		UpDownFlt;						// коэффициент прореживания ЦАП-АЦП плавающая
	double		PhasaDac;						// фаза целых отсчетов при прореживаниии
	int			PhasaDownDac;					// фаза целых отсчетов при прореживаниии
	double		CurLevel;						// текущий уровень
	double		MaxLevel;						// Максимальный сигнал
	float		Subtraction;					// то что надо вычесть
	float		Multiplier;						// то на что надо умножить
	GUID		IdChannel;						// идентификатор канала
	int			WatermarkFlag;					//
	double		hpf[6];
	double		hpfHistory[4];
} ALL_CHAN_INFO, *pALL_CHAN_INFO;
//////////////////////////////////////////////////////////////////////////////////////////////
//num    sense   convert ampl  prus  refer   afch    dc  comment
typedef struct _TABSIG_INFO {					//данные из файла DEVICES.CFG
	double		Sense;				//чувствительность
	wchar_t		Convert[200];		//единица измерения
	double		PreAmpl;			//коэффициент предварительного усиления
	int			PrUs;				//наличие усилителя 0-нет, 1-ПУ8/10, 2-HCP
	double		Refer;				//опора для расчета дБ
	wchar_t		Afch[MAX_PATH];		//файл поправок АЧХ
	double		DC;					//смещение постоянной составляющей
	wchar_t		Comment[400];		//название канала
	GUID		IdChanTab;			//идентификатор канала
	uint16_t	hpf;				//ФВЧ 0.1 Гц
} TABSIG_INFO, *pTABSIG_INFO;
////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _CHAN_DAC_INFO
{	// структура по номерам каналов ЦАП от 0 до количества каналов ЦАП-1
	int			EnableChanDac;			// разрешение работы по заданному каналу ЦАП
	int			OldEnableChanDac;		// старое состояние оно необходимо в процедуре DACSTOP
	int			Otkl;					// был отключен
	int			ModDac;					// модуль ЦАП на котором расположен данный канал
	int			OldModDac;				// модуль ЦАП на котором был данный модуль
	int			NumChanDacGl;			// номер физического канала модуля ЦАП, начинается после последнего номера канала АЦП
	int			NumChanDacLoc;			// номер виртуального канала ЦАП, начинается с 0 (редко, если только нет ЦАП) до кол-ва каналов в системе. Этот номер один и тот же для всех программ
	int			NumChanDacReal;			// номер в таблице устройств(не hidden)
	int			ChannelWithFreedback;	// канал с обратной связью(0 - обычный канал | 1 - канал с ОС)
	int			OldBuiltinGenEnabled;	// состояние внутреннего генератора до включения обычного генератора
	GUID		IdChanDac;				// идентификатор канала
	float		AttenuationVal;			// значение аттенюатора(учитывается при выдаче текущего уровня)
	HWND		UnitHwnd;				// окно программы, которая использует этот генератор
} CHAN_DAC_INFO, *pCHAN_DAC_INFO;
///////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _CHAN_VIRT_INFO
{	// информация по виртуальному каналу
	HWND		UnitHwnd;			// окно класса unit в приложении
	int			NumVirtLoc;			// номер виртуального канала в контексте приложения, создавшего этот виртуальный канал (всегда начинается с 0)
	int			NumChanGl;			// номер канала в перечне всех каналов
	wchar_t		Convert[200];		// единица измерения
	wchar_t		Comment[400];		// комментарий
	float		Frequency;			// частота дискретизации
	float		Refer;				// опора для расчета дБ
	float		MaxLevel;			// Максимальный уровень
	float		MinLevel;			// минимальный уровень
	GUID		IdChanVirt;			// GUID канала
	wchar_t		GroupNm[200];		// групповое имя канала
	int			TypeChannel;		// тип виртуального канала
	int			NumSourse;			// номер приложения, создавшего канал среди подобных
}CHAN_VIRT_INFO, *pCHAN_VIRT_INFO;
///////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _SIGNAL_ANP	
{	//данные из файла SIGXXX.ANP и .ANA
	int			Enable;				// разрешение чтения файла
	double		IntervalSec;		// длина файла в секундах
	FILE		*Input;				// хэндлер файла
	float		Gain;				// коэффициент усиления
	float		AbsVolt;			// абсолютные вольты
	float		Frequency;			// частота дискретизации
	float		Sense;				// чувствительность
	float		Ampl;				// коэффициент усиления
	float		Refer;				// опора для расчета дБ
	float		DC;					// смещение постоянной составляющей
	float		MaxUr;				// максимальный уровень сигнала
	wchar_t		Conversion[200];	// единица измерения	
	char		Afch[200];			// файл поправок АЧХ
	int			SigWav;				// формат файла 0-ana, 1-WAV
	int			Format;				// формат двоичных данных 0 - 16 разрядные слова, 1 - 32-разрядные, 2 - float 32 - бит
	struct __timeb64	StartTime;	//стартовое дата-время
	long		TypeADC;
	long		Serial;
	wchar_t		GroupNm[200];		// групповое имя канала
}SIGNAL_ANP, *pSIGNAL_ANP;
//////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _MEM_INFO
{	// информация по распределению памяти всех существующих каналов
	int				NullPointMem[MAXDECADE];		//указатель начала данных в файл-мапе
	int				CurPointMem[MAXDECADE];			//текущий указатель накопления в файл-мапе
	int				SizeBufMem[MAXDECADE];			//размер для каждого буфера в файл-мапе для каждого канала и декады
	long long		CountSample;					//количество накопленных точек в файл-мапе от запуска АЦП
	long long		NullCountSample;				//количество накопленных точек при запуске ЦАП
	long long		OldPointer;						//старый указатель записи данных в виртуальных каналах, в промежутках должны писаться не числа (not a number NAN), сейчас пока 0
	long long		DeltaStartTime;					//смещение на запуск для ЦАП
	int				OldPointMem;					//старый указатель на файлы для АЦП и ЦАП, и виртуальных каналов
	long			nak[MAXDECADE];
	long			tek[MAXDECADE];
	int				PortionDataVirt;				// порция данных в виртуальный канал
	int				PrisnakDataVirt;				// наличие данных в виртуальном канале
	CHANNEL_EVENTS  Events;							// события изменения состояния по цифровому каналу
	GUID			IdChanMem;						// идентификатор канала
	double			DeltaInTime;					// разница между текущим временем и реальным временем(для режима DAC MUX время бежит быстрее)
} MEM_INFO, *pMEM_INFO;
//////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _WATERMARKFLOAT_INFO{	//информация о Watermark
	unsigned long lTimeLabel;			//значение временной метки
	unsigned char ucQuality;			//качество данных
	long lPart;							//сколько бит Watermark уже записано в данные
	long lPoint[MAXCHANMOD];			//массив с номерами отсчетов с которого нужно ставить Watermark для каждого канала
} WATERMARKFLOAT_INFO, *pWATERMARKFLOAT_INFO;
//////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _MODULE_INFO {			// полная информация по модулю (сигнальному процессору)
	ADC_INFO_EXT DSPInfo;				//информация о модулях АЦП
	int			TypeDSP;				//тип драйвера АЦП
	int			NumberDSP;				//номер сигнального процессора
	long		SerialNumber;			//заводской номер модуля
	char		DeviceName[200];		//имя устройства
	int			CurModify;				//старая модификация драйвера
	short		*AdressUserBuffer;		//адрес пользовательской области памяти драйвера
	long		ulWatermarkPointer;	
	bool		bNeedToFindTimeLabel;
	WATERMARKFLOAT_INFO wmInfo;
	bool		bCriticalError;
	int			FirstChannelIndex;		// индекс первого канала устройства
	// еще добавить по векторам и Device.cfg

} MODULE_INFO, *pMODULE_INFO;
////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _CORRECT_ADC_INFO {				//данные из файла ADC.DAT
	double		AmplCorr;						//амплитудная коррекция канала АЦП
	double		DcCorr;							//смещение постоянной составляющей АЦП
} CORRECT_ADC_INFO, *pCORRECT_ADC_INFO;
///////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _CORRECT_PRUS_INFO {				//ДАННЫЕ ИЗ ФАЙЛА PRUS.DAT
	double		PrusCorr;						//поправка к усилителю заряда
} CORRECT_PRUS_INFO, *pCORRECT_PRUS_INFO;
/////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _GPS {
	float			shift;					//сдвиг системного времени и UTC
	char			buffer[GPSSIZE];			//буфер NMEA потока
	int				repointer;					//указатель на чтение из буфера NMEA
	int				wrpointer;					//указатель на запись в буфер NMEA
	struct _timeb	starttime;					//когда стартовали АЦП
	struct _timeb	promstart;					//промежуточный старт АЦП
	FILE			*file;						//GPS LOG файл
	long			filepos;					//позиция по файлу
	long			filelen;					//длина GPS лог файла
	int				satinfo;					//информация о количестве спутников
	int				advance;					//опережение при запуске по GPS
	int				enaGPSync;					//разрешение синхронизации по GPS
	ULONGLONG	UTCstartime;				// запуск по внешнему времени UTC в секундах от 1970 г
} GPS, *pGPS;
/////////////////////////////////////////////////////////////////////
typedef struct _ALL_INFO {				// информация обо всем
	int				IsServer;					// наличие сервера - 1, нет - 0
	int				Demo;						// демо режим 1, реальное время - 0
	int				ReadFile;					// режим чтения файла
	int				StartStop;					// старт стоп чтения файлов
	int				Pause;						// пауза в чтении файлов
	int				RealTime;					// 0-чтение быстро - 1 - медленно
	int				peretasovka;				// 0 - обычная работа, 1 - запоминает удаленные каналы
	wchar_t			PathSignal[400];			// полный путь к сигналам
	double			StartPosition;				// начальная позиция
	double			CurrentPosition;			// текущая позиция
	int				BlockDataAccess;			// блокировка доступа данных во время чтения
	HWND			hWndSRV;					// хэндлер окна класса сервера

	int				QuanModuleAdc;				// количество модулей АЦП
	MODULE_INFO		ModuleAdc[MAXMODULE];		// полное описание модуля АЦП
	int				QuanModuleDac;				// количество модулей ЦАП
	MODULE_INFO		ModuleDac[MAXMODULE];		// полное описание модуля ЦАП

	int				QuanChan;					//количество включенных каналов всех АЦП-ЦАП-виртуальных
	int				QuanChanAdc;				//количество включенных каналов АЦП
	int				QuanChanDac;				//количество включенных каналов ЦАП
	int				QuanChanVrt;				//количество включенных каналов виртуальных
	int				QuanChanFromFile;			//количество каналов из файла

	TABSIG_INFO			TabSig[MAXCHAN+MAXCHAN+MAXCHAN];	//данные из файла TABSIG.CFG
	CORRECT_ADC_INFO	CorrAdc[MAXCHAN+MAXCHAN];			//данные из файла ADC.DAT
	CORRECT_PRUS_INFO	CorrPrus[MAXCHAN];					//ДАННЫЕ ИЗ ФАЙЛА PRUS.DAT

	ALL_CHAN_INFO		AllChan[MAXCHAN+MAXCHAN];		//информация логических каналов
	MEM_INFO			MemChan[MAXCHAN+MAXCHAN];		//информация о распределении памяти в МАП-файле
	int					DacEnable;						//наличие ЦАП
	int					DacModify;						//изменение параметров ЦАП
	int					MaxQuanChanDac;					//максимальное количество каналов ЦАП в системе
	CHAN_DAC_INFO		DacChan[MAXDACS];

	INTER_PROC		unit[MAXUNITS];				//межпроцессорный обмен
	int				FileUsed[MAXCHAN];			//используемые файлы в процессе чтения из файлов 1- надо читать
	int				FileFirstInUse[MAXCHAN];	//Файл читается впервые, необходимо обнулить указатели 0 - читается
	SIGNAL_ANP		SigAnp[MAXCHAN];			//описатели файлов SIGxxx.ANP
	double			Resource;					// используемый сервером ресурс (загрузка процессора)
	CHAN_VIRT_INFO	VrtChn[MAXCHAN];
	int				changevirt;					// признак изменения количества виртуальных каналов, 1 - выставляется признак, 2 - отрабатывается

	long			EnaPWM[MAXMODULE];			//Наличие PWM аппаратная ШИМ-модуляция в SigmaUsb
	long			RatePWM[MAXMODULE];			//Коэффициент деления частоты 96 МГц
	long			PeriodPWM[MAXMODULE];		//Период ШИМ модуляции
	long			OnDutyPWM0[MAXMODULE];		//скважность 0-го канала
	long			OnDutyPWM1[MAXMODULE];
	long			OnDutyPWM2[MAXMODULE];
	long			ShiftPWM1[MAXMODULE];		//сдвиг по фазе 1-го отностительно 0-го
	long			ShiftPWM2[MAXMODULE];
	long			StartPWM0[MAXMODULE];		//Работает модулятор = 1, нет = 0.
	long			StartPWM1[MAXMODULE];
	long			StartPWM2[MAXMODULE];
	GPS				GPSData;
	long			GetUnit;					//блокировка взятия адреса unit во время другого доступа
	long			Stop_ADC_DAC;				//старт-стоп АЦП 0-идет процесс, 1-останавливается.
} ALL_INFO, *pALL_INFO;

typedef struct _DELTA_INFO 						
{
	char		conv[16];						// единицы измерения
	float		sense;							// чувствительность датчика В/ед.изм.
	float		refer;							// опорное значение для вычисления значения в дБ
} DELTA_INFO, *pDELTA_INFO;

typedef struct _PULSE_INFO{
	double timestart;
	long addrstart;
	long size;
} PULSE_INFO, *pPULSE_INFO;

// перечисление возможных режимов работы фреймов
typedef enum _DAC_MUX_MODE
{
	DAC_MUX_MODE_SUV = 0,
	DAC_MUX_MODE_LAST
};
// массив с режимами работы при работе с фреймами
static unsigned char g_DAC_MUX_MODES[] = { 0x35 };

// фрейм, передающийся в канал генератора в режиме DAC MUX
typedef struct _DAC_MUX_FRAME
{
	unsigned char sof;					// режим работы
	unsigned char fcs;					// сумма всех байт с 2го по 15й
	short code_out[2];					// предискаженный синус 2 отсчета
	short code_sin;						// синус для Герцеля
	short code_cos;						// косинус для Герцеля
	unsigned char reserved;				// 
	unsigned char param_switch;			// выбор параметра
	union {								// значение параметра
		long u32;
// 		short u16[2];
// 		unsigned char u8[4];
	} param;
	_DAC_MUX_FRAME()
	{
		ZeroMemory(this, sizeof(_DAC_MUX_FRAME));
		CalcFcs();
	}
	_DAC_MUX_FRAME(_In_ unsigned char Mode)
	{
		ZeroMemory(this, sizeof(_DAC_MUX_FRAME));
		sof = Mode;
		CalcFcs();
	}
	_DAC_MUX_FRAME(const _DAC_MUX_FRAME &Param)
	{
		*this = Param;
	}
	_DAC_MUX_FRAME & operator = (const _DAC_MUX_FRAME &Param)
	{
		memcpy_s(this, sizeof(_DAC_MUX_FRAME), &Param, sizeof(_DAC_MUX_FRAME));
		return *this;
	}
	// рассчет контрольной суммы фрейма
	void CalcFcs()
	{
		long Summ(0);
		this->fcs = 0;
		unsigned char *b = (unsigned char *)this;
		for (int i = 0; i < sizeof(_DAC_MUX_FRAME); i++)
			Summ += b[i];

		this->fcs = (unsigned char)-Summ;
	}
	// проверка валидности структуры
	bool Isvalid()
	{
		unsigned char *Element = (unsigned char *)this;
		// проверяем что режим работы один из возможных 
		bool bValid(false);
		for (auto &Mode : g_DAC_MUX_MODES)
		{
			if (*Element == Mode)
			{
				bValid = true;
				break;
			}
		}

		if (bValid)			
		{
			unsigned char Summ(0);
			// если режим выбран допустимый проверяем что сумма всех байт равна 0
			for (int i = 0; i < sizeof(_DAC_MUX_FRAME); i++)
				Summ += Element[i];

			bValid = Summ == 0;
		}

		return bValid;
	}
} _DAC_MUX_FRAME;

//------------------------------------------------------------------------------
#define SIZE_CHANNEL_NAME		200
#define SIZE_CHANNEL_STRUCT		880
//------------------------------------------------------------------------------
// Чтобы всё работало правильно в свойствах проекта для Debug и для Release
// параметр "Свойства конфигурации->С/С++->Создание кода->
// Выравнивание членов структур" должен быть задан "По умолчанию"
//
typedef struct _CHANNEL_STRUCT
{
	const uint32_t size;	// размер структуры
	long status;			// статус канала
	GUID id;				// GUID (идентификатор) канала
	wchar_t name[SIZE_CHANNEL_NAME];	// название канала
	wchar_t measure[SIZE_CHANNEL_NAME];	// единица измерения
	double frequency;		// частота дискретизации
	double sense;			// чувствительность
	double referense;		// опорное значение
	double dc;				// смещение постоянной составляющей
	double maxLevel;		// максимальный уровень
	double minLevel;		// минимальный уровень
	double subLevel;		// порог чувствительности

	_CHANNEL_STRUCT()
		: size(sizeof(_CHANNEL_STRUCT))
		, status(-1)
		, frequency(0.)
		, sense(0.)
		, referense(0.)
		, dc(0.)
		, maxLevel(0.)
		, minLevel(0.)
		, subLevel(0.)
		, id(GUID_NULL)
	{
		ZeroMemory(name, sizeof(name));
		ZeroMemory(measure, sizeof(measure));
	}

	~_CHANNEL_STRUCT() {}

	_CHANNEL_STRUCT(const _CHANNEL_STRUCT & cs)
		: size(sizeof(_CHANNEL_STRUCT))
	{
		memcpy_s(this, size, &cs, size);
	}

	_CHANNEL_STRUCT & operator = (const _CHANNEL_STRUCT & cs)
	{
		memcpy_s(this, size, &cs, size);
		return *this;
	}

	bool operator == (const _CHANNEL_STRUCT & cs)
	{
		return (size == cs.size) && (memcmp(this, &cs, size) == 0);
	}

	bool operator != (const _CHANNEL_STRUCT & cs)
	{
		return !(*this == cs);
	}

	long GetChannelID()
	{
		return *(long*)(id.Data4 + 4);
	}

	// проверяем является ли устройство по каналу интеллектуальным датчиком
	bool Is7xxxChannel()
	{
		return status == 6 || status == 7;
	}

	// получаем серийный номер устройства
	long long GetChannelSerial()
	{
		long long Serial;

		if (Is7xxxChannel())
		{
			LARGE_INTEGER lt;
			lt.HighPart = id.Data1;
			lt.LowPart = MAKELONG(id.Data2, id.Data3);

			Serial = lt.QuadPart;
		}
		else
			Serial = (GetChannelID() >> 16) & 0xffffffff;
		
		return Serial;
	}

} CHANNEL_STRUCT, *LPCHANNEL_STRUCT;
//------------------------------------------------------------------------------

#endif
