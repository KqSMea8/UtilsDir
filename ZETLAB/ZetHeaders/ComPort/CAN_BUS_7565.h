#pragma once
#include "afxwin.h"
#define NUMNODES 5//12
#define SIZEBUF 100
#define SIZEQUE 100
#define MAXLENGTHRECBUF 4000

#define CANBUS 0
#define MODBUSRTU  1
#define MODBUSASCII 2
#define MASTER 1
#define SLAVE 2
//��� ������ � ������ �� ��������
#define TypeDatNothing 0
#define TypeDatShort1 1
#define TypeDatShort2 2
#define TypeDatShort3 3
#define TypeDatShort4 4
#define TypeDatLong1 5
#define TypeDatLong2 6
#define TypeDatFloat1 7
#define TypeDatFloat2 8
#define TypeDatDouble1 9
#define TypeDatChar1 11
#define TypeDatChar2 12
#define TypeDatChar3 13
#define TypeDatChar4 14
#define TypeDatChar5 15
#define TypeDatChar6 16
#define TypeDatChar7 17
#define TypeDatChar8 18
#define TypeComNothing 20
#define TypeComFloat 21
#define TypeComChar 22
#define TypeComLong 23

////���� ������ ��� ������� � ������
#define INQUIRY 0x2000		// ������ ������������ � �������� ��������� � �������� ������
//#define FREQUENCY 1			// ������� ������
//#define MINLEVEL 2			// ������� ����������� ��������, ����������� ����������� �������
//#define MAXLEVEL 3			// ������� ������������� �������� �������
//#define REFERENCE 4			// ������� �������� ��� ������� � ��
//#define SENSE 5				// ���������������� �� ������
//#define MEASURE 6			// ������� ���������
//#define NAME1 7				// �������� ������
//#define NAME2 8
//#define NAME3 9
//#define NAME4 10
//#define STATUS 11			// ��������� ������
//#define KBOD 12
//#define NEWADRESS 13		// ��������� ������ ������

//���� ������ � �������� ��������� �������� � �����������

struct DevPar{						// ��� ��������� ��� �������� � �� ���������
// ��� ��������� �������� ��� ������������ � ��������� ��������� � ������������ � ������� ������
	float frequency;	// ������� ������ - ������������� �� ������
	float minlevel;		// ����������� �������
	float maxlevel;		// ������������ �������
	float reference;	// ����� ��� ������� ��
	float sense;		// ���������������� ������� �/�.�. (����� �� ������� ���������)
	char measure[8];	// ������� ��������� - �����
	char nameinfo[32];	// ������������ ������ - �����
	long kbod;			// �������� �������� �� ������
	long newadress;		// ��������� ������ ������
// ��� ��������� �������� ��� ������������ � ��������� ��������� � � ������� ������
	long numberofsensor;// ������-�������� ����� ������ - ��������� DevPar ��� ������� ������� � �������������� �������,
						// ��� ���������� ��������� � ��������������� �������, ��������, �������, ���������, ������� � ��.
// ��� ��������� ������ ������������ ������
	char status;		// ������ ������
	char typesensor;	// ��� �������
	short sernum;		// ��������� �����
	char subtypesensor;	// ������ �������
};

#define FREQUENCY 0								// ������� ������
#define MINLEVEL (sizeof(float))				// ������� ����������� ��������, ����������� ����������� �������
#define MAXLEVEL (MINLEVEL+sizeof(float))	// ������� ������������� �������� �������
#define REFERENCE (MAXLEVEL+sizeof(float))	// ������� �������� ��� ������� � ��
#define SENSE (REFERENCE+sizeof(float))		// ���������������� �� ������
#define MEASURE (SENSE+sizeof(float))		// ������� ��������� 8 byte
#define NAME1 (MEASURE + sizeof(char)*8)		// �������� ������
#define NAME2 (NAME1 + sizeof(char)*6)
#define NAME3 (NAME2 + sizeof(char)*6)
#define NAME4 (NAME3 + sizeof(char)*6)
#define NAME5 (NAME4 + sizeof(char)*6)
#define KBOD (NAME5 + sizeof(char)*8)			// �������� �������� �� ������
#define NEWADRESS (KBOD+sizeof(long))			// ��������� ������ ������
#define NUMBEROFSENSOR (NEWADRESS+sizeof(long))
#define STATUS (NUMBEROFSENSOR+sizeof(long))		// ��������� ������
#define TYPESENSOR (SERNUM+sizeof(char))		// ��� �������
#define SERNUM (STATUS+sizeof(char))			// ��������� �����
#define SUBTYPESENSOR (SERNUM+sizeof(short))	// ��� �������

						 //���� ��������//
#define UNIVERSAL 0				// ������������� ����
#define THERMOPARA 1			// 
#define THERMOSOPR 2			// 
#define TENZOSLOW 3				// 
#define SENSOR4_20 4			// 
#define TENZOFAST 5				// 
#define VIBRO 6					// 
#define SEISMO 7				// 
#define ACUSEMISS 8				// ������ ������������ �������

#define LIVEDATA 0x1				// ���� ������
#define LIVEFREQUENCY 0x2			// ������ ���������� � �������
#define LIVEMINLEVEL 0x4			// ������ ���������� � ����������� ������
#define LIVEMAXLEVEL 0x8
#define LIVEREFERENCE 0x10
#define LIVESENSE 0x20
#define LIVEMEASURE 0x40
#define LIVENAME1 0x80
#define LIVENAME2 0x100
#define LIVENAME3 0x200
#define LIVENAME4 0x400
#define LIVENAME5 0x800
#define LIVESTATUS 0x1000
#define SETINFOSERVER 0x2000		//��� �������� ������ �������� � ������
#define LIVEPARAM (LIVEFREQUENCY|LIVEMINLEVEL|LIVEMAXLEVEL|LIVEREFERENCE|LIVESENSE|LIVEMEASURE|LIVENAME1|LIVENAME2|LIVENAME3|LIVENAME4|LIVENAME5)

union DATATYPE    // Declare union type
{
   char				ch[246];
   unsigned short	s[123];
   unsigned long	l[62];
   float			f[62];
   double			d[31];
};    

//	������ � ����������� � ������� ���
//	0	��� � �� �������� � �������� ��������� ������
//	1	��� � �� �������� ����� �� ��������� ������
//	2	��� � ����� ������ �� �������� � �������� �� ���
//	3	��� � ����� ������ �� ��������  - ��� ������ ���� ������ ��� ����������� ���������� � ����.

struct CAN_Protokol{
	int adress;					// ����� ����������, ���� �� 1 �� 511, 0 - ��������� �����
	int pri;					// ��������� �������� �� 0 �� 3
	int dlc;					// ����� �������
	int command;				// ��� ������� ��� ������� � ������ INQUIRY
	int type;					// type = 0 ������ �� ����������, ��������, TypeDatShort1
	unsigned short smera[4];	// type 1 2 3 4
	unsigned long lmera[2];		// type 5 6
	float fmera[2];				// type 7 8
	double dmera[2];			// type 9
	char tmera[8];				// type 11 12 ... 18
};

struct RS_CAN_Protokol{
	int adress;					// ����� ���������� 1 - 247
	int function;				// �������: 3 ������ �������� ��������		| prior 0 + INQUIRY -- pri 1 - �����
								//			4 - ������ �������� �����		| prior 2 + INQUIRY		-"-
								//			16 - ������ � ������� ��������	| prior 0					-"-
								//			65 - ������ � ������� ������	| prior 2 � CAN			-"-
	int fadr;					// ����� ������� - ��� ������� � CAN (����� � ��������� DevParam)
	DATATYPE mera;				// ������ ��� ��������
	long type;					// ��� ������������ ������ - �����, ���������, ����� � ��.
	long size;					// ���������� ������ � DATATYPE ��� �������� - ������ ����
	RS_CAN_Protokol* pointer;	// ��� ����������� �������� ���������
};

struct node : public DevPar{
//
//			����� ��� �������� ������������� ��� ����� � ����� � ��. ��������
//
	long livestat;		// ������� �����, ��������������� ���� � ����������� �� �������� ������, ��� ���������� ��� ����������� ����� ���������� �� ������
	long livetick;		// ���� �����, ��� ������ ������� �� ���������� ��������������� 30 �����, ��� ���������� ������������ ������� ����� � 0
	long adressnode;	// ����� ���������� - ����
	float *bufer;		// ����� ��� �������� ������, � �������� SIZEBUF
	long posreader;		// ��������� ������ ��� ������
	long poswriter;		// ��������� ������ ��� ������

	long numvirtchan;	// ����� ������������ ������, ���������� � ������������� �������
	double curtime;		// current time
};

//CString Convert2String(CAN_Protokol Number);
int Convert2Struct(char * RecBuf, RS_CAN_Protokol * Paket);
long Convert2CharBuf(RS_CAN_Protokol * Paket, char * SendBuf, long maxlen);
int PaketInterp(RS_CAN_Protokol Reply, node *NODES);

long InitCom(wchar_t * PortName);
long CloseCom();
int SendPaket(RS_CAN_Protokol *Paket);				// ��������� � ������� �� �������� �����
int ReceivePaket(RS_CAN_Protokol *Paket);			// ����� �� ������� �����
int SinhroFromNode(long adres, DevPar* NodeDev);	// ��������� �� ������������ ������ adress � ��������� DevPar struct
int	WriteToNode(DevPar NodeDev, long adress);		// �������� ���������� �� DevPar struct � ���������� �� ������ adress
long TableFadr(long index);
UINT ThreadProcReceive(LPVOID pParam);
UINT ThreadProcSend(LPVOID pParam);
long SetMaster(long param);
