#pragma once
#include "afxwin.h"
#include <process.h> 
#include <time.h>
#include <stdio.h>
#include <sys/timeb.h>
#include "devpar.h"
#define MODBUSRTU  1
#define MODBUSASCII 2
#define MASTER 1
#define SLAVE 2
#define SIZEBUF 100
#define SIZEQUE 100
#define MAXLENGTHRECBUF 4000
#define NUMNODES 5//12
#define NUMPACKETS 100

#define LIVESTATUS 0x1000
#define SETINFOSERVER 0x2000		//��� �������� ������ �������� � ������
#define LIVEPARAM 0xff

//������ ������� ��� MODBUS
#define Read_Coil_Status 1				//������ �������� �� ���������� ��������� ������ 
#define Read_Discrete_Inputs 2			//������ �������� �� ���������� ���������� ��������� 
#define Read_Holding_Registers 3		//������ �������� �� ���������� ��������� �������� 
#define Read_Input_Registers 4			//������ �������� �� ���������� ��������� �����
#define Force_Single_Coil 5				//������ �������� ������ ����� 
#define Preset_Single_Register 6		//������ �������� � ���� ������� �������� 
#define Force_Multiple_Coils 15			//������ �������� � ��������� ��������� ������ 
#define Preset_Multiple_Registers 16	//������ �������� � ��������� ��������� �������� 


/*
struct DevParInter{						// ��� ��������� ��� �������� � �� ���������
// ��� ��������� �������� ��� ������������ � ��������� ��������� � ������������ � ������� ������
	float frequency;	// ������� ������ - ������������� �� ������
	float minlevel;		// ����������� �������
	float maxlevel;		// ������������ �������
	float reference;	// ����� ��� ������� ��
	float sense;		// ���������������� ������� �/�.�. (����� �� ������� ���������)
	char measure[8];	// ������� ��������� - �����
	char nameinfo[32];	// ������������ ������ - �����
//	long kbod;			// �������� �������� �� ������
//	long newadress;		// ��������� ������ ������
// ��� ��������� �������� ��� ������������ � ��������� ��������� � � ������� ������
//	long numberofsensor;// ������-�������� ����� ������ - ��������� DevPar ��� ������� ������� � �������������� �������,
						// ��� ���������� ��������� � ��������������� �������, ��������, �������, ���������, ������� � ��.
// ��� ��������� ������ ������������ ������
	char status;		// ������ ������
//	char typesensor;	// ��� �������
//	short sernum;		// ��������� �����
//	char subtypesensor;	// ������ �������
};
*/
struct NodeInter : public CHANNEL_PAR {
//
//			����� ��� �������� ������������� ��� ����� � ����� � ��. ��������
//
	long livestat;		// ������� �����, ��������������� ���� � ����������� �� �������� ������, ��� ���������� ��� ����������� ����� ���������� �� ������
	long livetick;		// ���� �����, ��� ������ ������� �� ���������� ��������������� 30 �����, ��� ���������� ������������ ������� ����� � 0
	long adressnode;	// ����� ���������� - ����
	float *bufer;		// ����� ��� �������� ������, � �������� SIZEBUF
	double *time;		// ����� ������� �������� ������
	long posreader;		// ��������� ������ ��� ������
	long poswriter;		// ��������� ������ ��� ������
	long numvirtchan;	// ����� ������������ ������, ���������� � ������������� �������
	double curtime;		// current time
};

struct Paket{
	short node;					// ����� ���������� � ����
	short enable;				// ���������� ��������� ������ 0- ���� ���, 1 ���� ������, -1 ������
	short command;				// ����� ������� - ������ 3 (0x03) � ������ �������� �� ���������� ��������� �������� (Read Holding Registers)
								// ��� ������ 16 (0x10) � ������ �������� � ��������� ��������� �������� (Preset Multiple Registers)
	short adtable;				// ����� ������� ��� �������� ������
	short size;					// ���������� ������ � ������� ������ � ������
	short quantity;				// ���������� ������ ��� ��������, � ������� (� ������?)
	short data[128];			// ������
};

struct InterMod{
	HANDLE hCom;	
	long pointReadQueSend;		// ��������� ������ �� ������� �������� 
	long pointWriteQueSend;		// ��������� ������ �� ������� ��������
	long pointReadQueRec;		// ��������� ������ �� ������� ������
	long pointWriteQueRec;		// ��������� ������ �� ������� ������
	long SendInd;
	long Master;
	long BlockSend;				//���� ����� 0, �� ����� ���������� ��������� �����
	long baud;					//�������� �������� ��� ������� ��������
	Paket QueSendPaket[NUMPACKETS];	// ���������� ������� � ����������� ������
	Paket QueRecPaket[NUMPACKETS];
};

class MODBUS
{
	InterMod intmod;
public:
	CWinThread *hThreadR = nullptr;
//	CWinThread *hThreadS;
public:
	MODBUS(void);
public:
	~MODBUS(void);
public:
	long InitCom(wchar_t * PortName, long baudrate);
	long SendPaket(Paket *spack);					// ��������� � ������� �� �������� �����
	long ReceivePaket(Paket *rpack);				// ����� �� ������� �����
//	long Convert2CharBuf(Paket * pack, char * SendBuf, long maxlen);
//	unsigned long Crc16( char *pcBlock, unsigned long RS_CLC);
//	long Convert2ByteBuf(Paket * Paket, char * SendBuf, long maxlen);
	long PaketIntMod(Paket * Reply, NodeInter *Nodes);

	long CloseCom();

};

//UINT ThreadProcReceiveM(LPVOID pParam);
UINT ThreadProcSendM(LPVOID pParam);
unsigned short Crc16( char *pcBlock, unsigned short RS_crc);
long Convert2ByteBuf(Paket * Paket, char * SendBuf, long maxlen);
