#pragma once

//#include "DataTypes/DataTypes.h"

//Интерфейсные функции модуля
bool DevParReadWord(void *pStructAddr, unsigned short offset, unsigned short* pValue);		//Чтение слова из структуры устройства
bool DevParWriteWord(void *pStructAddr, unsigned short offset, unsigned short* pValue);		//Запись слова в структуру устройства

//Набор структур, определяющий доступные для чтения/записи параметры устройств

typedef enum							//Множество, определяющее типы структур (которые перечислены ниже)
{
	CHANNEL_PAR_STRUCT =	0x0D0,		//структура _CHANNEL_PAR
	CONNECT_PAR_STRUCT =	0x07A,		//структура _CONNECT_PAR
	DETECTOR_PAR_STRUCT = 	0x337,		//структура _DETECTOR_PAR
	ADC_PAR_STRUCT		=	0x2BA		//структура _ADC_PAR
} STRUCT_TYPE;
//
//typedef struct HEADER
//{
//unsigned int size:12;
//unsigned int	status:10;
//STRUCT_TYPE		struct_type:10;
//} *pHeader;

typedef struct 							//Структура, определяющай параметры канала
{
	unsigned int	size:12;				//Размер текущей структуры (в словах)
	STRUCT_TYPE		struct_type:10;		//Тип текущей структуры
	unsigned int	status:10;				//Статус канала (ошибка)
	unsigned int	write_enable;		//Разрешение записи в структуру
	float			value;				//Текущее значение датчика
	float			frequency;			//Частота опроса - дискретизации по каналу
	char			measure[8];			//Единица измерения
	char			channel_name[32];	//Наименование канала - текст
	float			min_level;			//Минимальный уровень (в ед. изм.)
	float			max_level;			//Максимальный уровень (в ед. изм.)
	float			reference;			//Опора для расчета дБ (в ед. изм.)
	float			sense;				//Чувствительность датчика В/е.и. (Вольт на единицу измерения)
	float			resolution;			//разрешающающая способность датчика
} CHANNEL_PAR;

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
	unsigned int	size;				//Размер текущей структуры (в словах)
	float			freq;				//Частота дискретизации
	float			gain;				//Коэффициент усиления
	float			abs_volt;			//Вес младшего разряда номинальный
	float			calibr;				//Калибровочная поправка коэффициента передачи
	float			offset;				//Смещение
	unsigned int	write_enable;		//Разрешение записи в структуру
	unsigned int	status;				//Статус АЦП (ошибка)
	STRUCT_TYPE		struct_type;		//Тип текущей структуры
} ADC_PAR;

typedef enum							//Множество, определяющее канал подключения устройства
{
	ZET7010 = 0							//0 - Тип устройства - ZET7010

} DEVICE_TYPE;

typedef struct 							//Структура, определяющая дополнительные параметры устройства
{
	unsigned int	size;				//Размер текущей структуры (в словах)
	unsigned short	hard_ver;			//Версия аппаратной части
	unsigned short	soft_ver;			//Версия программного обеспечения
	DEVICE_TYPE		type;				//Тип устройства
	unsigned int	serial_num;			//Серийный номер
	unsigned int	edition_date;		//Дата выпуска (time_t)
	char			generant[16];		//Производитель
	unsigned int	write_enable;		//Разрешение записи в структуру
	unsigned int	status;				//Статус (зарезервированно)
	STRUCT_TYPE		struct_type;		//Тип текущей структуры
} DEVICE_PAR;


