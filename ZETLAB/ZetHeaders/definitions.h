#pragma once
#ifndef __ZET_7XXX_DEFINTIONS__
#define __ZET_7XXX_DEFINTIONS__

typedef enum							//Множество, определяющее типы структур (которые перечислены ниже)
{
	COMMON_STRUCT			=	0x000,	//зарезервировано под всю структуру устройства
	CHANNEL_PAR_STRUCT 		=	0x0D0,	//структура _CHANNEL_PAR
	UART_PAR_STRUCT 		=	0x07A,	//структура _UART_PAR
	CAN_PAR_STRUCT			=	0x02D,	//структура _CAN_PAR
	DETECTOR_PAR_STRUCT 	= 	0x137,	//структура _DETECTOR_PAR
	ADC_PAR_STRUCT			=	0x0BA,	//структура _ADC_PAR
	DEV_PAR_STRUCT			= 	0x18C,	//структура _DEVICE_PAR
	DATA_BUFFER_STRUCT		=	0x0D7,	//структура _DATA_BUFFER
	COMMAND_STRUCT			=	0x17F,	//структура _DEV_COMMAND
	SENSOR_TR_STRUCT		= 	0x0AE,	//структура _SENSOR_TR
	SENSOR_TC_STRUCT		= 	0x13D,	//структура _SENSOR_TC
	SENSOR_BRIDGE_STRUCT	=	0x19C,	//структура _SENSOR_BRIDGE
	SENSOR_PIEZO_STRUCT		=	0x27D,	//структура _SENSOR_PIEZO
	SENSOR_ADXL_STRUCT		=	0x313,	//структура _SENSOR_ADXL
	SENSOR_PORT_STRUCT      =   0x3E4,  //структура SENSOR_PORT
	SENSOR_MOTOR_STRUCT     =   0x2B6,  //структура SENSOR_MOTOR
	SENSOR_GENERATOR_STRUCT =   0x169,  //структура SENSOR_GENERATOR 
	SENSOR_ANALOG_GENERATOR_STRUCT 	=	0x249,
	SETTINGS_MOTOR_STRUCT   =   0x221,  //структура SETTINGS_MOTOR
	SETTINGS_GENERATOR_STRUCT = 0x209,  //структура SETTINGS_GENERATOR 
	SETTINGS_ANALOG_GENERATOR_STRUCT= 	0x0C3,
	EDDY_CURRENT_STRUCT		=	0x468,	//структура	EDDY_CURRENT 
	METROLOGY_STRUCT		= 	0x138,	//структура _METROLOGY
	//	FTDI_PAR_STRUCT			= 	0x00C,	//структура _FTDI_PAR
	MODBUS_PAR_STRUCT		=	0x371,	//структура _MODBUS
	ERROR_PAR_STRUCT		=	0x215,	//структура _ERROR
	CHAIN_PAR_STRUCT		=	0x242,	//структура _CHAIN
	TIME_PAR_STRUCT			=	0x354,	//структура _TIME
} STRUCT_TYPE;

typedef struct _STRUCT_HEAD				
{
	unsigned int	size		: 12;	//Размер текущей структуры (в словах)
	unsigned int	struct_type : 10;	//Тип текущей структуры
	unsigned int	status		: 10;	//Статус канала (ошибка)
	unsigned int	write_enable;		//Разрешение записи в структуру
} STRUCT_HEAD, *pSTRUCT_HEAD;

typedef struct _STRUCT_ARGV				//структура аргумента
{
	short arg1 :5;
	short size1:10;
	short change1 :1;
	char  def1[46];
} STRUCT_ARGV, *PSTRUCT_ARGV;
//-----------------------------------------------------------------------------------------
typedef struct _STRUCT_LIST_LONG2
{
	long size;
	long list[2];
} STRUCT_LIST_LONG2, *pSTRUCT_LIST_LONG2;
//-----------------------------------------------------------------------
typedef struct _STRUCT_LIST_LONG14
{
	long size;
	long list[14];
} STRUCT_LIST_LONG14, *pSTRUCT_LIST_LONG14;
//-----------------------------------------------------------------------
typedef struct _STRUCT_LIST_FLOAT10
{
	long size;
	float list[10];
} STRUCT_LIST_FLOAT10, *pSTRUCT_LIST_FLOAT10;
//------------------------------------------------------------------------------
typedef struct _STRUCT_LIST_CHAR5
{
	long size;
	char list1[16];
	char list2[16];
	char list3[16];
	char list4[16];
	char list5[16];
} STRUCT_LIST_CHAR5, *pSTRUCT_LIST_CHAR5;
//-------------------------------------------------------------------------------------------------------
typedef struct _STRUCT_LIST_CHAR12
{
	long size;
	char list1[16];
	char list2[16];
	char list3[16];
	char list4[16];
	char list5[16];
	char list6[16];
	char list7[16];
	char list8[16];
	char list9[16];
	char list10[16];
	char list11[16];
	char list12[16];
} STRUCT_LIST_CHAR12, *pSTRUCT_LIST_CHAR12;
//-------------------------------------------------------------------------------------------------------
typedef struct _STRUCT_LIST_CHAR17
{
	long size;
	char list1[16];
	char list2[16];
	char list3[16];
	char list4[16];
	char list5[16];
	char list6[16];
	char list7[16];
	char list8[16];
	char list9[16];
	char list10[16];
	char list11[16];
	char list12[16];
	char list13[16];
	char list14[16];
	char list15[16];
	char list16[16];
	char list17[16];
} STRUCT_LIST_CHAR17, *pSTRUCT_LIST_CHAR17;
//-------------------------------------------------------------------------------------------------------
//type of arg
typedef enum _type_of_arg{
	t_char = 1,
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
	t_longhex 
}type_of_arg;
//------------------------------------------------------------------------------------------------------
//type of change
typedef enum _type_of_change{
	unchange = 0,
	change
} type_of_change;

typedef struct _DEF_STRUCT			//
{
	STRUCT_HEAD		head;				
	char	subtitle[12];				
	char	title[48];				
} DEF_STRUCT, *pDEF_STRUCT;


typedef struct _LINE_INFO
{
	char	name[64];
	long	SerialNumber;
	long	rs_baud_rate;
	long	rs_parity;
} LINE_INFO, *pLINE_INFO;

#define NUMLINES 27
#define SIZE_OF_STRUCT 16384

typedef struct _STRUCT7000EX
{
	HANDLE hMODBUSZETLAB;
	long virtchan;			// IN номер виртуального канала в программе MODBUSZETLAB, если -1, то зачитывается информация из линии FTDI и ноды node
	long stepcomm;			// IN команда запуска 1 - запустить процесс чтения информации по заданному виртуальному каналу
	// команда 1 переходит в 2 - этап выполнения-чтение первой зоны памяти, 3- чтение второй зоны памяти
	long GUID;				// OUT GUID измерительного канала
	long IsReady;			// OUT ответ о готовности программы 0- не готов, 1- готов
	long ftdindex;			// OUT номер линии к которой подключен модуль, IN - номер линии по которой надо получить информацию, если к ней не подключен модуль,
	// тогда номер virtchan = -1
	long node;				// OUT номер ноды которая создает измерительный канал
	long size_dev_struct;	// OUT размер структуры структур данных
	long size_define;		// OUT размер структуры структур описателей
	char dev_struct[SIZE_OF_STRUCT+1000];		//OUT структуры данных
	char define_struct[SIZE_OF_STRUCT+1000];	//OUT структуры описателей
	long numftdindex;		// количество линий FTDI  от 0 до NUMLINES
	LINE_INFO	linfo[NUMLINES];				//данные о FTDI контроллерах
	double		lastsend;		// время последнего запроса, если потерялся, то давай повторим
	long		adtable;
	long		quantity;
	long		command;
} STRUCT7000EX, *pSTRUCT7000EX;

#endif /*__ZET_7XXX_DEFINTIONS__*/
