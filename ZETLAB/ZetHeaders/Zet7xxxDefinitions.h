#ifndef ZET7XXX_DEFINITIONS_HEADER
#define ZET7XXX_DEFINITIONS_HEADER

	#include "Zet7xxxDeviceTypes.h"

	#define MAX_STRUCT_SIZE		256			//Максимальный размер структуры в составе общей структуры устройства

	typedef enum _STRUCT_TYPE				//Множество, определяющее типы структур (которые перечислены ниже)
	{							
		STRUCT_TYPE_MASK = 0x3FF,//Значения не должны превышать 0x3FF (1023), т.к. на данное поле выделено 10 битов
		COMMON_STRUCT = 0x000,	//зарезервировано под всю структуру устройства
		CHANNEL_PAR_STRUCT = 0x0D0,	//структура _CHANNEL_PAR
		UART_PAR_STRUCT = 0x07A,	//структура _UART_PAR
		CAN_PAR_STRUCT = 0x02D,	//структура _CAN_PAR
		DETECTOR_PAR_STRUCT = 0x137,	//структура _DETECTOR_PAR
		ADC_PAR_STRUCT = 0x0BA,	//структура _ADC_PAR
		DAC_PAR_STRUCT = 0x327,	//структура _DAC_PAR
		DEV_PAR_STRUCT = 0x18C,	//структура _DEVICE_PAR
		DATA_BUFFER_STRUCT = 0x0D7,	//структура _DATA_BUFFER
		COMMAND_STRUCT = 0x17F,	//структура _DEV_COMMAND
		SENSOR_TR_STRUCT = 0x0AE,	//структура _SENSOR_TR
		SENSOR_TC_STRUCT = 0x13D,	//структура _SENSOR_TC
		SETTINGS_TENZO_STATIC_STRUCT_CAN = 0x19C,	//структура _SENSOR_BRIDGE
		SETTINGS_TENZO_DINAMIC_STRUCT = 0x12C,	//структура _SENSOR_BRIDGE
		SENSOR_PRESSURE_STRUCT_CAN = 0x13C,	//структура _SENSOR_BRIDGE
		SENSOR_PIEZO_7050_STRUCT = 0x27D,	//структура _SENSOR_PIEZO
		SENSOR_ADXL_7052_STRUCT = 0x313,	//структура _SENSOR_ADXL
		SENSOR_PORT_STRUCT_CAN = 0x3E4,	//структура SENSOR_PORT
		SENSOR_PORT_STRUCT_RS = 0x3E5,	//структура SENSOR_PORT
		SENSOR_MOTOR_STRUCT = 0x2B6,	//структура SENSOR_MOTOR
		SENSOR_GENERATOR_STRUCT = 0x169,	//структура SENSOR_GENERATOR 
		SENSOR_ANALOG_GENERATOR_STRUCT = 0x249,	//структура SENSOR_ANALOG_GENERATOR
		SETTINGS_MOTOR_STRUCT = 0x221,	//структура SETTINGS_MOTOR
		SETTINGS_ENCODER_STRUCT = 0x351,	//структура SETTINGS_ENCODER
		SETTINGS_GENERATOR_STRUCT = 0x209,	//структура SETTINGS_GENERATOR
		SETTINGS_ANALOG_GENERATOR_STRUCT_0_5 = 0x0C3,	//структура SETTINGS_ANALOG_GENERATOR
		SETTINGS_ANALOG_GENERATOR_STRUCT_0_10 = 0x0C4,	//структура SETTINGS_ANALOG_GENERATOR
		PID_REGULATOR_STRUCT = 0x367,	//структура ПИД регулятора
		SETTINGS_X_BEE_STRUCT = 0x227,	//структура SETTINGS_X_BEE
		SETTINGS_ETHERNET_STRUCT = 0x011,	//структура SETTINGS_ETHERNET
		SETTINGS_PTP_STRUCT = 0x012,	//структура SETTINGS_PTP
		SENSOR_PTP_STRUCT = 0x212,	//структура SETTINGS_PTP
		SETTINGS_GSM_STRUCT = 0x013,	//структура SETTINGS_GSM
		SENSOR_GSM_STRUCT = 0x213,	//структура SENSOR_GSM
		EDDYCUR_SENSOR_STRUCT = 0x170,	//структура EDDYCUR_SENSOR
		AEMISSION_SENSOR_STRUCT = 0x171,	//структура AEMISSION_SENSOR
		RP_COUNTER_STRUCT = 0x172,	//структура RP_COUNTER
		ZET7140_SENSOR_STRUCT = 0x173,	//структура ZET7140_SENSO
		METROLOGY_STRUCT = 0x138,	//структура _METROLOGY
		DIAGNOST_STRUCT = 0x148,	//структура _DIAGNOST
		MODBUS_PAR_STRUCT = 0x371,	//структура _MODBUS
		X_BEE_PAR_STRUCT = 0x250,	//структура _X_BEE
		FT232H_PAR_STRUCT = 0x010,	//структура _FT232H_PAR
		SD_CARD_PAR_STRUCT = 0x023,	//структура _SD_CARD_PAR
		ERROR_PAR_STRUCT = 0x215,	//структура _ERROR
		CHAIN_PAR_STRUCT = 0x242,	//структура _CHAIN
		TIME_PAR_STRUCT = 0x354,	//структура _TIME
		LCD9832_PAR_STRUCT = 0x063,	//структура _LCD9832_PAR
		CAN_MASTER_SETTINGS_STRUCT = 0x325,	//структура _CAN_MASTER_SETTINGS_STRUCT
		SENSOR_CALIBRATE_STRUCT = 0x111,	//структура _SENSOR_CALIBRATE
		SENSOR_FOR_7x80i_STRUCT = 0x19A,
		SENSOR_FOR_7x80v_STRUCT = 0x20A,
		SETTINGS_GPS_STRUCT = 0x214,	//структура SETTINGS_GPS
		SENSOR_GPS_STRUCT = 0x225,	//структура SENSOR_GPS
		PARAM_OLD_PAR_STRUCT = 0x015,	//структура PARAM_OLD_PAR
		PARAM_PAR_STRUCT = 0x016,	//структура PARAM_PAR
		SENSOR_RELAY_SWITCH_STRUCT = 0x21A,
		SETTINGS_RELAY_SWITCH_STRUCT = 0x22A,
		CALIBR_MODE_STRUCT = 0x25A,
		TESTING_STRUCT = 0x26A,
		SENSOR_LDC1000_STRUCT = 0x314,	//структура _SENSOR_LDC1000
		DEBUG_PIEZO_STRUCT = 0x315,
		AUTO_DIGITAL_PORT_STRUCT = 0x27A,
		REPEATER_STRUCT = 0x368,	//структура повторителя на ЦАПе
		HF_GENERATOR_STRUCT = 0x128,	//структура высокочастотного ЦАПа
		SERVICE_PAR_STRUCT = 0x373,	//структура сервисной обработки
		PID_SET_STRUCT = 0x129, 	//структура быстрых настроек ПИД
		LCD9832_SET_STRUCT = 0x12A,	//структура _LCD9832_SET
		SENSOR_FAST_PORT_STRUCT = 0x28A,	//структура SENSOR_FAST_PORT
		SENSOR_ADIS_STRUCT = 0x316,	//структура _SENSOR_ADIS
		ANGULAR_ENCODER_STRUCT = 0x29A,	//структура ANGULAR_ENCODER
		LINEAR_ENCODER_STRUCT = 0x30A,	//структура LINEAR_ENCODER
		FREQUENCY_ENCODER_STRUCT = 0x31A,	//структура FREQUENCY_ENCODER
		ENCODER_STRUCT = 0x33A,	//структура ENCODER_STRUCT
		TENZO_SENSOR_STRUCT = 0x34A,	//структура TENZO_SENSOR_STRUCT
		TENZO_RESISTOR_STRUCT = 0x36A,	//структура 
		ZERO_OFFSET_STRUCT = 0x37A,	//структура ZERO_OFFSET_STRUCT
		CAN_DIAGNOST_STRUCT = 0x38A,	//структура CAN_DIAGNOST_STRUCT
		SETTINGS_TENZO_STATIC_STRUCT_RS = 0x39A,
		SENSOR_PRESSURE_STRUCT_RS = 0x39B,	//структура _SENSOR_BRIDGE
		SETTINGS_LVDT_STRUCT = 0x39C,	//структура _SENSOR_BRIDGE
		SENSOR_PIEZO_7150_STRUCT = 0x005,	//структура SENSOR_PIEZO_7150_STRUCT
		SENSOR_PIEZO_7051_STRUCT = 0x006,	//структура SENSOR_PIEZO_7051_STRUCT
		SENSOR_PIEZO_7151_STRUCT = 0x007,	//структура SENSOR_PIEZO_7151_STRUCT
		SENSOR_PIEZO_314_STRUCT = 0x008,	//структура SENSOR_PIEZO_314_STRUCT
		SENSOR_ADXL_7152_STRUCT = 0x317,	//структура SENSOR_ADXL_7152_STRUCT
		SENSOR_ADXL_7152H_STRUCT = 0x318,	//структура SENSOR_ADXL_7152H_STRUCT
		SENSOR_ADXL_7152ST_STRUCT = 0x319,	//структура SENSOR_ADXL_7152ST_STRUCT
		AEMISSION_SENSOR_H_STRUCT = 0x176,	//структура AEMISSION_SENSOR_H_STRUCT
		DAC_R_PAR_STRUCT = 0x328,	//структура DAC_R_PAR_STRUCT
		CAN_MASTER_GSM_SETTINGS_STRUCT = 0x326,	//структура CAN_MASTER_GSM_SETTINGS_STRUCT
		ADC_PAR2_STRUCT = 0x0BB,	//структура _ADC_PAR2
		ADC_PAR3_STRUCT = 0x0BC,	//структура _ADC_PAR3
		MESSAGE_PAR_STRUCT = 0x00C,
	}STRUCT_TYPE, *PSTRUCT_TYPE;

#pragma pack(push, 1)

	typedef enum _type_of_arg
	{
		t_none = 0,
		t_char,
		t_short,
		t_long,
		t_unshort,
		t_unlong,
		t_float,
		t_list_float,
		t_list_long,
		t_list_char,
		t_list_char_index,
		t_time,
		t_longhex,
		t_longlong,
		t_longlonghex,
		t_ip,
		t_mac,
		t_type_sensor,
		t_calibration,

		ZET7XXX_TYPE_OF_ARG_COUNT
	} type_of_arg;

	enum access_type {
		access_unknown = -1,
		access_operator = 0,
		access_dispatcher,
		access_metrology
	};

	typedef struct _STRUCT_HEAD
	{
		unsigned int	size		: 12;	//Размер текущей структуры (в байтах)
		unsigned int	struct_type : 10;	//Тип текущей структуры
		unsigned int	status		: 10;	//Статус канала (ошибка)
		unsigned int	write_enable: 16;	//Разрешение записи в структуру
		unsigned int	usCRC16     : 16;
	} STRUCT_HEAD, *pSTRUCT_HEAD;

	typedef struct _DEF_STRUCT
	{
		STRUCT_HEAD head;
		char        name[12];
		char        desc[48];

	} DEF_STRUCT, *pDEF_STRUCT;

	typedef struct 							//Структура, определяющай параметры канала
	{
		STRUCT_HEAD		head;
		float			value;				//Текущее значение датчика
		float			frequency;			//Частота опроса - дискретизации по каналу
		char			measure[8];			//Единица измерения
		char			channel_name[32];	//Наименование канала - текст
		float			min_level;			//Минимальный уровень (в ед. изм.)
		float			max_level;			//Максимальный уровень (в ед. изм.)
		float			reference;			//Опора для расчета дБ (в ед. изм.)
		float			sense;				//Чувствительность датчика В/е.и. (Вольт на единицу измерения)
		float			resolution;			//разрешающающая способность датчика
	} CHANNEL_PAR, *pCHANNEL_PAR;

	typedef struct
	{
		STRUCT_HEAD  head;
		long loctime;       // âðåìÿ â ñåêóíäàõ îò 1970 ãîäà
		long    split_sec;    // äîëÿ ñåêóíäû
		long  canstamp;       // âðåìÿ ïî ñ÷åò÷èêó CAN â ìîìåíò ïðè¸ìà ïàêåòà ïî CAN
		long  mbID_sync_can_packet; // íîìåð MB, ïî êîòîðîìó ïðèø¸ë ïàêåò ñèíõðîíèçàöèè (ìýéëîêñîâ âñåãî 32, ïîýòîó charà õâàòèò)
		long    pps_edge;    // êàæäûé áèò îòâå÷àåò çà ñîîòâåòñòâóþùèé åìó êàíàë (0 - 0 êàíàë, 1 - 1 êàíàë, è ò.ä.)
	} TIME_PAR;

	typedef struct _PARAM_OLD_PAR				//Ñòðóêòóðà, îïðåäåëÿþùàÿ ïàðàìåòðû êàíàëà
	{
		STRUCT_HEAD    head;
		char           param_name[32];    	//Íàèìåíîâàíèå ïàðàìåòðà - òåêñò
		char           measure[8];    		//Åäèíèöà èçìåðåíèÿ
		long 		   activity; 			//Àêòèâíîñòü
	} PARAM_OLD_PAR, *pPARAM_OLD_PAR;

	typedef struct _PARAM_PAR
	{
		STRUCT_HEAD  head;
		char  param_name[32];
		char  measure[8];
	} PARAM_PAR, *pPARAM_PAR;

	typedef enum							//Множество, определяющее канал подключения устройства
	{
		RS_485 = 0							//0 - Тип канала связи - RS-485

	} CONNECTION_TYPE;

	typedef struct 							//Структура, определяющая параметры связи
	{
		unsigned int	size;				//Размер текущей структуры (в словах)
		unsigned int	dev_addr;			//Адрес устройства
		CONNECTION_TYPE	type;				//Тип канала связи
		//	BAUD_RATE		baud_rate;			//Скорость передачи данных
		unsigned int	write_enable;		//Разрешение записи в структуру
		unsigned int	status;				//Статус канала связи (ошибка)
		STRUCT_TYPE		struct_type;		//Тип текущей структуры
	} CONNECT_PAR;

	typedef enum							//Множество, определяющее 
	{
		AVERAGE = 0							//0 - Тип усреднения - среднее значение

	} AVERAGE_TYPE;

	typedef struct 							//Структура, определяющая параметры преобразования исходного сигнала в результирующий
	{
		unsigned int	size;				//Размер текущей структуры (в словах)
		AVERAGE_TYPE	type;				//Тип детектора (тип преобразования)
		float			average_time;		//Время усреднения
		float			filter_freq;		//Частота среза для фильтра
		unsigned int	filter_type;		//Тип фильтрации (ФВЧ/ФНЧ)
		unsigned int	write_enable;		//Разрешение записи в структуру
		unsigned int	status;				//Статус модуля преобразования (ошибка)
		STRUCT_TYPE		struct_type;		//Тип текущей структуры
	} DETECTOR_PAR;

	typedef struct 							//Структура, определяющая параметры АЦП
	{
		STRUCT_HEAD		head;
		float			freq;				//Частота дискретизации
		float			gain;				//Коэффициент усиления
		float			abs_volt;			//Вес младшего разряда номинальный
		float			calibr;				//Калибровочная поправка коэффициента передачи
		float			offset;				//Смещение
	} ADC_PAR, *pADC_PAR;

	typedef struct							//Структура, определяющая дополнительные параметры устройства
	{
		long   type;    //Тип устройства
		unsigned long long serial_num;   //Серийный номер
		long    compile_time;  //Время компиляции программного обеспечения (time_t)
		long    edition_time;  //Время выпуска аппаратной части (time_t)
		unsigned long dev_addr;//Адрес устройства 1-255 для всех, 0 - для контроллера, 7174 - USB-CAN
	} DEVICE_PAR;

	typedef struct							//Структура, определяющая дополнительные параметры устройства
	{
		STRUCT_HEAD head;
		DEVICE_PAR device_par;
	} DEV_PAR, *PDEV_PAR;

	typedef struct _CAN_PAR					//Структура, определяющая параметры связи (CAN)
	{
		STRUCT_HEAD		head;				//Заголовок структуры
		long			baud_rate;			//Скорость передачи данных (младший бит - флаг контроля четности)
		long			current_time;		//Текущее время устройства
		long long		sync_offset;		//Смещение в сек
		long			sync_state;			//Состояние синхронизации
	} CAN_PAR, *pCAN_PAR;
	//-----------------------------------------------------------------------------------------
	typedef enum _BAUD_RATE
	{
		BR1200		= 1200,
		BR2400		= 2400,
		BR4800		= 4800,
		BR9600		= 9600,
		BR14400		= 14400,
		BR19200		= 19200,
		BR38400		= 38400,
		BR57600		= 57600,
		BR115200	= 115200,
		BR230400	= 230400,
		BR250000    = 250000,
		BR460800	= 460800,
		BR921600	= 921600,
		BR1500000	= 1500000,
		BR3000000	= 3000000,
	} BAUD_RATE, *PBAUD_RATE;
	//--------------------------------------------------------------------------------------------
	typedef struct _STRUCT_LIST_LONG2
	{
		long size;
		long list[2];
	} STRUCT_LIST_LONG2, *pSTRUCT_LIST_LONG2;
	//---------------------------------------------------------------------------
	typedef struct _STRUCT_LIST_LONG9
	{
		long size;
		long list[9];
	} STRUCT_LIST_LONG9, *pSTRUCT_LIST_LONG9;
	//-------------------------------------------------
	typedef struct _STRUCT_ARGV				//структура аргумента
	{
		unsigned short type : 5;
		unsigned short size : 10;
		unsigned short editable : 1;
		char           name[46];
	} STRUCT_ARGV, *PSTRUCT_ARGV;

	//-----------------------------------------------------------------------------------------------------------------------
	typedef struct _UART_PAR			//Ñòðóêòóðà, îïðåäåëÿþùàÿ ïàðàìåòðû ñâÿçè (UART)
	{
		STRUCT_HEAD		head;			//Çàãîëîâîê ñòðóêòóðû
		long		baud_rate;			//Ñêîðîñòü ïåðåäà÷è äàííûõ ôëàã êîíòðîëÿ ÷åòíîñòè)
		long		parity_enable;		//ôëàã êîíòðîëÿ ÷åòíîñòè
		long		current_time;		//Текущее время устройства
	} UART_PAR, *pUART_PAR;
	//------------------------------------------------------------------------------------------------------------
	typedef struct _DEF_UART_PAR			//Ñòðóêòóðà, îïðåäåëÿþùàÿ ïàðàìåòðû ñâÿçè (UART)
	{
		STRUCT_HEAD		head;				//Çàãîëîâîê ñòðóêòóðû
		char	subtitle[12];				//Ïîäçàãîëîâîê ñòðóêòóðû
		char	title[48];					//Íàçâàíèå ñòðóêòóðû
		STRUCT_ARGV		arg1;				//Ñêîðîñòü ïåðåäà÷è äàííûõ
		STRUCT_LIST_LONG9 list1;
		STRUCT_ARGV		arg2;				//ôëàã êîíòðîëÿ ÷åòíîñòè
		STRUCT_LIST_LONG2 list2;			
		STRUCT_ARGV		arg3;				//ôëàã êîíòðîëÿ ÷åòíîñòè
	} DEF_UART_PAR, *pDEF_UART_PAR;

	typedef struct _CHANNEL_INFO
	{
		float				fFrequency;			//Частота опроса - дискретизации по каналу
		char				cChannelName[32];	//Наименование канала - текст
		char				cMeasure[8];		//Единица измерения
		unsigned long long	ullSerial;			//Серийный номер устройства
		float				fMaxLevel;			//Максимальный уровень (в ед. изм.)
		float				fMinLevel;			//Минимальный уровень (в ед. изм.)
		float				fResolution;		//разрешающающая способность датчика
		float				fReference;			//Опора для расчета дБ (в ед. изм.)
		unsigned long		ulDeviceType;		//Тип устройства
	} CHANNEL_INFO, *pCHANNEL_INFO;

	typedef struct _CHANNEL_INFO_OLD
	{
		float				fFrequency;			//Частота опроса - дискретизации по каналу
		char				cChannelName[32];	//Наименование канала - текст
		char				cMeasure[8];		//Единица измерения
		unsigned long long	ullSerial;			//Серийный номер устройства
		float				fMaxLevel;			//Максимальный уровень (в ед. изм.)
		float				fMinLevel;			//Минимальный уровень (в ед. изм.)
		float				fResolution;		//разрешающающая способность датчика
		float				fReference;			//Опора для расчета дБ (в ед. изм.)
	} CHANNEL_INFO_OLD, *pCHANNEL_INFO_OLD;

	typedef struct _FILE_HEAD
	{
		unsigned char ucAddress;
		unsigned char ucReserve;
		unsigned long ulRTC;
		unsigned char ucDataType;
		unsigned char ucStatus;
		unsigned short usQuantity;
	} FILE_HEAD, pFILE_HEAD;

	typedef struct _MESSAGE_PAR    //Структура, определяющая параметры канала
	{
		STRUCT_HEAD    head;
		unsigned long format_id;  // Идентификатор формата сообщений
		unsigned long source_addr; // Адрес истоничка (нода относительно ноды устройства, или адрес регистра)
		unsigned long flags;   // Флаги выдачи сообщений
	} MESSAGE_PAR, *pMESSAGE_PAR;

#pragma pack(pop)

#endif /* ZET7xxxDEFINITIONS_HEADER */
