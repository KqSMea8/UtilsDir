#pragma once
#ifndef __ZET_7XXX_DEFINTIONS__
#define __ZET_7XXX_DEFINTIONS__

typedef enum							//���������, ������������ ���� �������� (������� ����������� ����)
{
	COMMON_STRUCT			=	0x000,	//��������������� ��� ��� ��������� ����������
	CHANNEL_PAR_STRUCT 		=	0x0D0,	//��������� _CHANNEL_PAR
	UART_PAR_STRUCT 		=	0x07A,	//��������� _UART_PAR
	CAN_PAR_STRUCT			=	0x02D,	//��������� _CAN_PAR
	DETECTOR_PAR_STRUCT 	= 	0x137,	//��������� _DETECTOR_PAR
	ADC_PAR_STRUCT			=	0x0BA,	//��������� _ADC_PAR
	DEV_PAR_STRUCT			= 	0x18C,	//��������� _DEVICE_PAR
	DATA_BUFFER_STRUCT		=	0x0D7,	//��������� _DATA_BUFFER
	COMMAND_STRUCT			=	0x17F,	//��������� _DEV_COMMAND
	SENSOR_TR_STRUCT		= 	0x0AE,	//��������� _SENSOR_TR
	SENSOR_TC_STRUCT		= 	0x13D,	//��������� _SENSOR_TC
	SENSOR_BRIDGE_STRUCT	=	0x19C,	//��������� _SENSOR_BRIDGE
	SENSOR_PIEZO_STRUCT		=	0x27D,	//��������� _SENSOR_PIEZO
	SENSOR_ADXL_STRUCT		=	0x313,	//��������� _SENSOR_ADXL
	SENSOR_PORT_STRUCT      =   0x3E4,  //��������� SENSOR_PORT
	SENSOR_MOTOR_STRUCT     =   0x2B6,  //��������� SENSOR_MOTOR
	SENSOR_GENERATOR_STRUCT =   0x169,  //��������� SENSOR_GENERATOR 
	SENSOR_ANALOG_GENERATOR_STRUCT 	=	0x249,
	SETTINGS_MOTOR_STRUCT   =   0x221,  //��������� SETTINGS_MOTOR
	SETTINGS_GENERATOR_STRUCT = 0x209,  //��������� SETTINGS_GENERATOR 
	SETTINGS_ANALOG_GENERATOR_STRUCT= 	0x0C3,
	EDDY_CURRENT_STRUCT		=	0x468,	//���������	EDDY_CURRENT 
	METROLOGY_STRUCT		= 	0x138,	//��������� _METROLOGY
	//	FTDI_PAR_STRUCT			= 	0x00C,	//��������� _FTDI_PAR
	MODBUS_PAR_STRUCT		=	0x371,	//��������� _MODBUS
	ERROR_PAR_STRUCT		=	0x215,	//��������� _ERROR
	CHAIN_PAR_STRUCT		=	0x242,	//��������� _CHAIN
	TIME_PAR_STRUCT			=	0x354,	//��������� _TIME
} STRUCT_TYPE;

typedef struct _STRUCT_HEAD				
{
	unsigned int	size		: 12;	//������ ������� ��������� (� ������)
	unsigned int	struct_type : 10;	//��� ������� ���������
	unsigned int	status		: 10;	//������ ������ (������)
	unsigned int	write_enable;		//���������� ������ � ���������
} STRUCT_HEAD, *pSTRUCT_HEAD;

typedef struct _STRUCT_ARGV				//��������� ���������
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
	long virtchan;			// IN ����� ������������ ������ � ��������� MODBUSZETLAB, ���� -1, �� ������������ ���������� �� ����� FTDI � ���� node
	long stepcomm;			// IN ������� ������� 1 - ��������� ������� ������ ���������� �� ��������� ������������ ������
	// ������� 1 ��������� � 2 - ���� ����������-������ ������ ���� ������, 3- ������ ������ ���� ������
	long GUID;				// OUT GUID �������������� ������
	long IsReady;			// OUT ����� � ���������� ��������� 0- �� �����, 1- �����
	long ftdindex;			// OUT ����� ����� � ������� ��������� ������, IN - ����� ����� �� ������� ���� �������� ����������, ���� � ��� �� ��������� ������,
	// ����� ����� virtchan = -1
	long node;				// OUT ����� ���� ������� ������� ������������� �����
	long size_dev_struct;	// OUT ������ ��������� �������� ������
	long size_define;		// OUT ������ ��������� �������� ����������
	char dev_struct[SIZE_OF_STRUCT+1000];		//OUT ��������� ������
	char define_struct[SIZE_OF_STRUCT+1000];	//OUT ��������� ����������
	long numftdindex;		// ���������� ����� FTDI  �� 0 �� NUMLINES
	LINE_INFO	linfo[NUMLINES];				//������ � FTDI ������������
	double		lastsend;		// ����� ���������� �������, ���� ���������, �� ����� ��������
	long		adtable;
	long		quantity;
	long		command;
} STRUCT7000EX, *pSTRUCT7000EX;

#endif /*__ZET_7XXX_DEFINTIONS__*/
