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
//тип данных и команд по передаче
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

////коды команд при запросе и ответе
#define INQUIRY 0x2000		// Запрос прибавляется к названию параметра и означает запрос
//#define FREQUENCY 1			// частота опроса
//#define MINLEVEL 2			// уровень миммального значения, разоешающая способность датчика
//#define MAXLEVEL 3			// уровень максимального значения датчика
//#define REFERENCE 4			// опорное значение для расчета в дБ
//#define SENSE 5				// чувствительность по каналу
//#define MEASURE 6			// единица измерения
//#define NAME1 7				// название канала
//#define NAME2 8
//#define NAME3 9
//#define NAME4 10
//#define STATUS 11			// состояние канала
//#define KBOD 12
//#define NEWADRESS 13		// установка нового адреса

//Коды команд и смещения регистров хранения в контроллере

struct DevPar{						// эта структура для Блэкфина и ЦП одинаковы
// эти параметры задаются или опрашиваются в программе настройки и опрашиваются в рабочем режиме
	float frequency;	// частота опроса - дискретизации по каналу
	float minlevel;		// минимальный уровень
	float maxlevel;		// максимальный уровень
	float reference;	// опора для расчета дБ
	float sense;		// чувствительность датчика В/е.и. (Вольт на единицу измерения)
	char measure[8];	// единица измерения - текст
	char nameinfo[32];	// наименование канала - текст
	long kbod;			// скорость передачи по каналу
	long newadress;		// установка нового адреса
// эти параметры задаются или опрашиваются в программе настройки и в рабочем режиме
	long numberofsensor;// задает-получает номер канала - структуры DevPar для каждого датчика в многоканальной системе,
						// или параметров измерения в параметрической системе, например, частота, амплитуда, уровень и пр.
// эти параметры только опрашиваются всегда
	char status;		// статус канала
	char typesensor;	// тип датчика
	short sernum;		// заводской номер
	char subtypesensor;	// подтип датчика
};

#define FREQUENCY 0								// частота опроса
#define MINLEVEL (sizeof(float))				// уровень миммального значения, разоешающая способность датчика
#define MAXLEVEL (MINLEVEL+sizeof(float))	// уровень максимального значения датчика
#define REFERENCE (MAXLEVEL+sizeof(float))	// опорное значение для расчета в дБ
#define SENSE (REFERENCE+sizeof(float))		// чувствительность по каналу
#define MEASURE (SENSE+sizeof(float))		// единица измерения 8 byte
#define NAME1 (MEASURE + sizeof(char)*8)		// название канала
#define NAME2 (NAME1 + sizeof(char)*6)
#define NAME3 (NAME2 + sizeof(char)*6)
#define NAME4 (NAME3 + sizeof(char)*6)
#define NAME5 (NAME4 + sizeof(char)*6)
#define KBOD (NAME5 + sizeof(char)*8)			// скорость передачи по каналу
#define NEWADRESS (KBOD+sizeof(long))			// установка нового адреса
#define NUMBEROFSENSOR (NEWADRESS+sizeof(long))
#define STATUS (NUMBEROFSENSOR+sizeof(long))		// состояние канала
#define TYPESENSOR (SERNUM+sizeof(char))		// тип датчика
#define SERNUM (STATUS+sizeof(char))			// заводской номер
#define SUBTYPESENSOR (SERNUM+sizeof(short))	// тип датчика

						 //типы датчиков//
#define UNIVERSAL 0				// универсальный вход
#define THERMOPARA 1			// 
#define THERMOSOPR 2			// 
#define TENZOSLOW 3				// 
#define SENSOR4_20 4			// 
#define TENZOFAST 5				// 
#define VIBRO 6					// 
#define SEISMO 7				// 
#define ACUSEMISS 8				// датчик акустической эмиссии

#define LIVEDATA 0x1				// идут данные
#define LIVEFREQUENCY 0x2			// пришла информация о частоте
#define LIVEMINLEVEL 0x4			// пришла информация о минимальном уровне
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
#define SETINFOSERVER 0x2000		//все исходные данные переданы в сервер
#define LIVEPARAM (LIVEFREQUENCY|LIVEMINLEVEL|LIVEMAXLEVEL|LIVEREFERENCE|LIVESENSE|LIVEMEASURE|LIVENAME1|LIVENAME2|LIVENAME3|LIVENAME4|LIVENAME5)

union DATATYPE    // Declare union type
{
   char				ch[246];
   unsigned short	s[123];
   unsigned long	l[62];
   float			f[62];
   double			d[31];
};    

//	Пакеты с приоритетом и адресом ххх
//	0	ххх – от ведущего к ведомому командный запрос
//	1	ххх – от ведомого ответ на командный запрос
//	2	ххх – поток данных от ведущего к ведомому на ЦАП
//	3	ххх – поток данных от ведомого  - эти данные идут всегда при подключении устройства к сети.

struct CAN_Protokol{
	int adress;					// адрес устройства, ноды от 1 до 511, 0 - системный адрес
	int pri;					// приоритет меняется от 0 до 3
	int dlc;					// длина посылки
	int command;				// код команды при запросе и ответе INQUIRY
	int type;					// type = 0 ничего не пересылать, например, TypeDatShort1
	unsigned short smera[4];	// type 1 2 3 4
	unsigned long lmera[2];		// type 5 6
	float fmera[2];				// type 7 8
	double dmera[2];			// type 9
	char tmera[8];				// type 11 12 ... 18
};

struct RS_CAN_Protokol{
	int adress;					// адрес устройства 1 - 247
	int function;				// функция: 3 чтение регистра хранения		| prior 0 + INQUIRY -- pri 1 - ответ
								//			4 - чтение регистра ввода		| prior 2 + INQUIRY		-"-
								//			16 - запись в регистр хранения	| prior 0					-"-
								//			65 - запись в регистр вывода	| prior 2 в CAN			-"-
	int fadr;					// адрес функции - код команды в CAN (адрес в структуре DevParam)
	DATATYPE mera;				// данные для передачи
	long type;					// тип передаваемых данных - целые, плавающие, текст и пр.
	long size;					// количество данных в DATATYPE для передачи - строго байт
	RS_CAN_Protokol* pointer;	// для организации цепочной структуры
};

struct node : public DevPar{
//
//			место для структур специфических для тензо и термо и пр. датчиков
//
	long livestat;		// уровень жизни, устанавливаются биты в зависимости от принятых данных, это необходимо при асинхронном сборе информации по каналу
	long livetick;		// тики жизни, при каждой команде от устройства устанавливается 30 тиков, при отсутствии сбрасывается уровень жизни в 0
	long adressnode;	// адрес устройства - ноды
	float *bufer;		// буфер для принятых данных, с размером SIZEBUF
	long posreader;		// указатель буфера для чтения
	long poswriter;		// указатель буфера для записи

	long numvirtchan;	// номер виртуального канала, связанного с измерительным каналом
	double curtime;		// current time
};

//CString Convert2String(CAN_Protokol Number);
int Convert2Struct(char * RecBuf, RS_CAN_Protokol * Paket);
long Convert2CharBuf(RS_CAN_Protokol * Paket, char * SendBuf, long maxlen);
int PaketInterp(RS_CAN_Protokol Reply, node *NODES);

long InitCom(wchar_t * PortName);
long CloseCom();
int SendPaket(RS_CAN_Protokol *Paket);				// поставить в очередь на передачу пакет
int ReceivePaket(RS_CAN_Protokol *Paket);			// взять из очереди пакет
int SinhroFromNode(long adres, DevPar* NodeDev);	// прочитать из устройствапо адресу adress и заполнить DevPar struct
int	WriteToNode(DevPar NodeDev, long adress);		// Записать информацию из DevPar struct в устройство по адресу adress
long TableFadr(long index);
UINT ThreadProcReceive(LPVOID pParam);
UINT ThreadProcSend(LPVOID pParam);
long SetMaster(long param);
