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
#define SETINFOSERVER 0x2000		//все исходные данные переданы в сервер
#define LIVEPARAM 0xff

//номера функций при MODBUS
#define Read_Coil_Status 1				//чтение значений из нескольких регистров флагов 
#define Read_Discrete_Inputs 2			//чтение значений из нескольких дискретных регистров 
#define Read_Holding_Registers 3		//чтение значений из нескольких регистров хранения 
#define Read_Input_Registers 4			//чтение значений из нескольких регистров ввода
#define Force_Single_Coil 5				//запись значения одного флага 
#define Preset_Single_Register 6		//запись значения в один регистр хранения 
#define Force_Multiple_Coils 15			//запись значений в несколько регистров флагов 
#define Preset_Multiple_Registers 16	//запись значений в несколько регистров хранения 


/*
struct DevParInter{						// эта структура для Блэкфина и ЦП одинаковы
// эти параметры задаются или опрашиваются в программе настройки и опрашиваются в рабочем режиме
	float frequency;	// частота опроса - дискретизации по каналу
	float minlevel;		// минимальный уровень
	float maxlevel;		// максимальный уровень
	float reference;	// опора для расчета дБ
	float sense;		// чувствительность датчика В/е.и. (Вольт на единицу измерения)
	char measure[8];	// единица измерения - текст
	char nameinfo[32];	// наименование канала - текст
//	long kbod;			// скорость передачи по каналу
//	long newadress;		// установка нового адреса
// эти параметры задаются или опрашиваются в программе настройки и в рабочем режиме
//	long numberofsensor;// задает-получает номер канала - структуры DevPar для каждого датчика в многоканальной системе,
						// или параметров измерения в параметрической системе, например, частота, амплитуда, уровень и пр.
// эти параметры только опрашиваются всегда
	char status;		// статус канала
//	char typesensor;	// тип датчика
//	short sernum;		// заводской номер
//	char subtypesensor;	// подтип датчика
};
*/
struct NodeInter : public CHANNEL_PAR {
//
//			место для структур специфических для тензо и термо и пр. датчиков
//
	long livestat;		// уровень жизни, устанавливаются биты в зависимости от принятых данных, это необходимо при асинхронном сборе информации по каналу
	long livetick;		// тики жизни, при каждой команде от устройства устанавливается 30 тиков, при отсутствии сбрасывается уровень жизни в 0
	long adressnode;	// адрес устройства - ноды
	float *bufer;		// буфер для принятых данных, с размером SIZEBUF
	double *time;		// буфер времени прибытия данных
	long posreader;		// указатель буфера для чтения
	long poswriter;		// указатель буфера для записи
	long numvirtchan;	// номер виртуального канала, связанного с измерительным каналом
	double curtime;		// current time
};

struct Paket{
	short node;					// адрес устройства в сети
	short enable;				// разрешение обработки пакета 0- инфы нет, 1 инфа полная, -1 ошибка
	short command;				// номер функции - чтение 3 (0x03) — чтение значений из нескольких регистров хранения (Read Holding Registers)
								// или запись 16 (0x10) — запись значений в несколько регистров хранения (Preset Multiple Registers)
	short adtable;				// адрес таблицы для передачи данных
	short size;					// количество данных в текущем пакете в байтах
	short quantity;				// количество данных для передачи, в запросе (в словах?)
	short data[128];			// данные
};

struct InterMod{
	HANDLE hCom;	
	long pointReadQueSend;		// указатель чтения из очереди передачи 
	long pointWriteQueSend;		// указатель записи из очереди передачи
	long pointReadQueRec;		// указатель чтения из очереди приема
	long pointWriteQueRec;		// указатель записи из очереди приема
	long SendInd;
	long Master;
	long BlockSend;				//если равен 0, то можно передавать следующий пакет
	long baud;					//скорость передачи для расчета задержек
	Paket QueSendPaket[NUMPACKETS];	// Количество пакетов в циклическом буфере
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
	long SendPaket(Paket *spack);					// поставить в очередь на передачу пакет
	long ReceivePaket(Paket *rpack);				// взять из очереди пакет
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
