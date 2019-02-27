#pragma once

//#include "DataTypes/DataTypes.h"

//������������ ������� ������
bool DevParReadWord(void *pStructAddr, unsigned short offset, unsigned short* pValue);		//������ ����� �� ��������� ����������
bool DevParWriteWord(void *pStructAddr, unsigned short offset, unsigned short* pValue);		//������ ����� � ��������� ����������

//����� ��������, ������������ ��������� ��� ������/������ ��������� ���������

typedef enum							//���������, ������������ ���� �������� (������� ����������� ����)
{
	CHANNEL_PAR_STRUCT =	0x0D0,		//��������� _CHANNEL_PAR
	CONNECT_PAR_STRUCT =	0x07A,		//��������� _CONNECT_PAR
	DETECTOR_PAR_STRUCT = 	0x337,		//��������� _DETECTOR_PAR
	ADC_PAR_STRUCT		=	0x2BA		//��������� _ADC_PAR
} STRUCT_TYPE;
//
//typedef struct HEADER
//{
//unsigned int size:12;
//unsigned int	status:10;
//STRUCT_TYPE		struct_type:10;
//} *pHeader;

typedef struct 							//���������, ������������ ��������� ������
{
	unsigned int	size:12;				//������ ������� ��������� (� ������)
	STRUCT_TYPE		struct_type:10;		//��� ������� ���������
	unsigned int	status:10;				//������ ������ (������)
	unsigned int	write_enable;		//���������� ������ � ���������
	float			value;				//������� �������� �������
	float			frequency;			//������� ������ - ������������� �� ������
	char			measure[8];			//������� ���������
	char			channel_name[32];	//������������ ������ - �����
	float			min_level;			//����������� ������� (� ��. ���.)
	float			max_level;			//������������ ������� (� ��. ���.)
	float			reference;			//����� ��� ������� �� (� ��. ���.)
	float			sense;				//���������������� ������� �/�.�. (����� �� ������� ���������)
	float			resolution;			//�������������� ����������� �������
} CHANNEL_PAR;

typedef enum							//���������, ������������ ����� ����������� ����������
{
	RS_485 = 0							//0 - ��� ������ ����� - RS-485

} CONNECTION_TYPE;

typedef struct 							//���������, ������������ ��������� �����
{
	unsigned int	size;				//������ ������� ��������� (� ������)
	unsigned int	dev_addr;			//����� ����������
	CONNECTION_TYPE	type;				//��� ������ �����
//	BAUD_RATE		baud_rate;			//�������� �������� ������
	unsigned int	write_enable;		//���������� ������ � ���������
	unsigned int	status;				//������ ������ ����� (������)
	STRUCT_TYPE		struct_type;		//��� ������� ���������
} CONNECT_PAR;

typedef enum							//���������, ������������ 
{
	AVERAGE = 0							//0 - ��� ���������� - ������� ��������

} AVERAGE_TYPE;

typedef struct 							//���������, ������������ ��������� �������������� ��������� ������� � ��������������
{
	unsigned int	size;				//������ ������� ��������� (� ������)
	AVERAGE_TYPE	type;				//��� ��������� (��� ��������������)
	float			average_time;		//����� ����������
	float			filter_freq;		//������� ����� ��� �������
	unsigned int	filter_type;		//��� ���������� (���/���)
	unsigned int	write_enable;		//���������� ������ � ���������
	unsigned int	status;				//������ ������ �������������� (������)
	STRUCT_TYPE		struct_type;		//��� ������� ���������
} DETECTOR_PAR;

typedef struct 							//���������, ������������ ��������� ���
{
	unsigned int	size;				//������ ������� ��������� (� ������)
	float			freq;				//������� �������������
	float			gain;				//����������� ��������
	float			abs_volt;			//��� �������� ������� �����������
	float			calibr;				//������������� �������� ������������ ��������
	float			offset;				//��������
	unsigned int	write_enable;		//���������� ������ � ���������
	unsigned int	status;				//������ ��� (������)
	STRUCT_TYPE		struct_type;		//��� ������� ���������
} ADC_PAR;

typedef enum							//���������, ������������ ����� ����������� ����������
{
	ZET7010 = 0							//0 - ��� ���������� - ZET7010

} DEVICE_TYPE;

typedef struct 							//���������, ������������ �������������� ��������� ����������
{
	unsigned int	size;				//������ ������� ��������� (� ������)
	unsigned short	hard_ver;			//������ ���������� �����
	unsigned short	soft_ver;			//������ ������������ �����������
	DEVICE_TYPE		type;				//��� ����������
	unsigned int	serial_num;			//�������� �����
	unsigned int	edition_date;		//���� ������� (time_t)
	char			generant[16];		//�������������
	unsigned int	write_enable;		//���������� ������ � ���������
	unsigned int	status;				//������ (����������������)
	STRUCT_TYPE		struct_type;		//��� ������� ���������
} DEVICE_PAR;


