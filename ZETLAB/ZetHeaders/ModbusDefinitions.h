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
	long status;	// Состояние команды (1 - в очереди на исполнение, 0 - исполнена)
	long node;		// Нода устройства
	long adr;		// Адрес написания в памяти датчика
	long size;		// Размер записываемых данных
	long serialNumberInterface;	// Серийный номер интерфейсной платы
	char data[200];	// Записываемые данные
} FAST_COMMAND, *pFAST_COMMAND;

typedef struct _STRUCT7000EX
{
	HANDLE			hMODBUSZETLAB;
	long			virtchan;			// IN номер виртуального канала в программе MODBUSZETLAB, если -1, то зачитывается информация из линии FTDI и ноды node
	long			stepcomm;			// IN команда запуска 1 - запустить процесс чтения информации по заданному виртуальному каналу, -1 запись измененной информации
	// команда 1 переходит в 2 - этап выполнения-чтение первой зоны памяти, 3- чтение второй зоны памяти
	long			GUID;				// OUT GUID измерительного канала
	long			IsReady;			// OUT ответ о готовности программы 0- не готов, 1- готов
	long			ftdindex;			// OUT номер линии к которой подключен модуль,
	// IN - номер линии по которой надо получить информацию, если к ней не подключен модуль,
	// тогда номер virtchan = -1
	long			node;				// OUT номер ноды которая создает измерительный канал
	long			size_dev_struct;	// OUT размер структуры структур данных
	long			size_define;		// OUT размер структуры структур описателей
	char			dev_struct   [SIZE_OF_STRUCT + 1000];	//OUT структуры данных
	char			define_struct[SIZE_OF_STRUCT + 1000];	//OUT структуры описателей
	long			numftdindex;		// количество линий FTDI от 0 до NUMLINES
	LINE_INFO		linfo  [NUMLINES];				//данные о FTDI контроллерах
	DeviceInfo		devinfo[NUMCELLS];				//данные об устройствах
	double			lastsend;		// время последнего запроса, если потерялся, то давай повторим
	long			adtable;
	long			quantity;
	long			command;
	long			numelem;			// количество элементов отображаемых на дереве, при обновлении занулять
	long			numerr;		// количество перезапусков после 10 надо останавливать
	HTREEITEM		pos;			// позиция в дереве, на котором стоит курсор
	FAST_COMMAND	fastCommands[NUMLINES][MAX_FAST_COMMAND_PER_LINE];	// 20 возможных быстрых команд по NUMLINES линиям
} STRUCT7000EX, *pSTRUCT7000EX;

#endif   /* MODBUS_DEFINITIONS_HEADER */
