#ifndef MODBUS_DEFINITIONS_HEADER
#define MODBUS_DEFINITIONS_HEADER

#include <windows.h>
#include "CommCtrl.h"
#include <Zet7xxxDefinitions.h>

#define SIZE_OF_STRUCT 16384
#define NUMLINES 27
#define NUMCELLS 1000
#define MAX_FAST_COMMAND_PER_LINE 20


/*typedef struct _DEF_STRUCT			//
{
	STRUCT_HEAD		head;				
	char	subtitle[12];				
	char	title[48];				
} DEF_STRUCT, *pDEF_STRUCT;*/

typedef struct _LINE_INFO
{
	char		name[64];
	union {
		long long SerialNumber64;
		struct {
			long		SerialNumber;
			long		SerialNumberHigh;
		};
	};
	long		rs_baud_rate;
	long		rs_parity;
	short		EthEnabled;
	short		EthConnected;
} LINE_INFO, *pLINE_INFO;

struct DeviceInfo : public DEVICE_PAR
{
	long ftindex;
	long node;
};

typedef struct _FAST_COMMAND
{
	long status;	// ��������� ������� (1 - � ������� �� ����������, 0 - ���������)
	long node;		// ���� ����������
	long adr;		// ����� ��������� � ������ �������
	long size;		// ������ ������������ ������
	long serialNumberInterface;	// �������� ����� ������������ �����
	char data[200];	// ������������ ������
} FAST_COMMAND, *pFAST_COMMAND;

typedef struct _STRUCT7000EX
{
	HANDLE			hMODBUSZETLAB;
	long			virtchan;			// IN ����� ������������ ������ � ��������� MODBUSZETLAB, ���� -1, �� ������������ ���������� �� ����� FTDI � ���� node
	long			stepcomm;			// IN ������� ������� 1 - ��������� ������� ������ ���������� �� ��������� ������������ ������, -1 ������ ���������� ����������
	// ������� 1 ��������� � 2 - ���� ����������-������ ������ ���� ������, 3- ������ ������ ���� ������
	long			GUID;				// OUT GUID �������������� ������
	long			IsReady;			// OUT ����� � ���������� ��������� 0- �� �����, 1- �����
	long			ftdindex;			// OUT ����� ����� � ������� ��������� ������,
	// IN - ����� ����� �� ������� ���� �������� ����������, ���� � ��� �� ��������� ������,
	// ����� ����� virtchan = -1
	long			node;				// OUT ����� ���� ������� ������� ������������� �����
	long			size_dev_struct;	// OUT ������ ��������� �������� ������
	long			size_define;		// OUT ������ ��������� �������� ����������
	char			dev_struct   [SIZE_OF_STRUCT + 1000];	//OUT ��������� ������
	char			define_struct[SIZE_OF_STRUCT + 1000];	//OUT ��������� ����������
	long			numftdindex;		// ���������� ����� FTDI �� 0 �� NUMLINES
	LINE_INFO		linfo  [NUMLINES];				//������ � FTDI ������������
	DeviceInfo		devinfo[NUMCELLS];				//������ �� �����������
	double			lastsend;		// ����� ���������� �������, ���� ���������, �� ����� ��������
	long			adtable;
	long			quantity;
	long			command;
	long			numelem;			// ���������� ��������� ������������ �� ������, ��� ���������� ��������
	long			numerr;		// ���������� ������������ ����� 10 ���� �������������
	HTREEITEM		pos;			// ������� � ������, �� ������� ����� ������
	FAST_COMMAND	fastCommands[NUMLINES][MAX_FAST_COMMAND_PER_LINE];	// 20 ��������� ������� ������ �� NUMLINES ������
} STRUCT7000EX, *pSTRUCT7000EX;

#endif   /* MODBUS_DEFINITIONS_HEADER */
