#pragma once

//Определение дополнительных кодов команд для режима работы с командой _COMMAND_INFO_USB.Command = 0x0113
#define NUM_SIGNALS				0x00		//Получения количества непрерывных сигналов в приборе (сигналом считается непрерывная по времени запись одного канала (может находится в разных файлах))


typedef struct _DEV_INFO					//Структура, определяющая параметры записи прибора
{
	short	num_ch_adc;						//Количество каналов прибора
	time_t	start_time;						//Время старта записи
	time_t	duration;						//Продолжительность записи, сек
	short	data_type;						//Тип данных (0 - 16 бит, 1 - 32 бита)
	float	frecuency;						//Частота дискретизации одного канала
	
	float	abs_volt;
} DEV_INFO;

#define CANAL_NAME_LEN			128			//Максимальная длина строки имени канала
#define CONV_NAME_LEN			16			//Максимальная длина строки для единицы измерения

typedef struct _CHAN_INFO					//Структура, определяющая параметры отдельного
{
	char name[CANAL_NAME_LEN];				//Название сигнала
	char conv[CONV_NAME_LEN];				//Единица измерения
	float sense;							//Чувствительность датчика В/ед.изм.
	float DC;								//Смещение нуля
	float refer;							//Опорное значение для вычисления значения в дБ
} CHAN_INFO;

#define COMMENT_LEN				128			//Максимальная длина строки комментария
#define FORMAT_LEN				4			//Максимальная длина строки формата числа
#define MAX_STR_LEN_ANP_INF		256			//Максимальная длина строки в структуре _CHAN_INFO_ANP


typedef struct _CHAN_INFO_ANP				//Структура, определяющая параметры отдельного канала для отображения сигнала (данная структура записывается в файл ANP)
{
	char name[CANAL_NAME_LEN];				//Название канала ввода сигнала
	char comment[COMMENT_LEN];				//Комментарий
	float abs_volt;							//Вес младшего разряда АЦП в Вольтах
	unsigned int frq;						//Частота дискретизации сигнала в Гц
	int tmi;								//(???) сек? int? Начальный временной сдвиг сигнала при вводе
	float frl;								//Частота среза фильтра нижних частот
	float frh;								//Частота среза фильтра верхних частот
	char format[FORMAT_LEN];				//Формат представленных данных (целое/дробное, 1слово/2слова)
	struct tm start;						//Время и дата старта сигнала
	short ch_idx;							//Индекс канала ввода
	float sense;							//Чувствительность первичного преобразователя физической величины в напряжение, измеряемого в Вольт/CONVERT
	char conv[CONV_NAME_LEN];				//Название единицы измерения физической величины
	float ampl;								//Коэффициент усиления предварительного или промежуточного усилителя	
	float refer;							//Пороговая величина для расчета дБ. Для электричества эта величина составляет 1 мкВ, для гидроакустики – 20 мкПа, для вибрации – 300 мкм/с^2
	int afch;								//Файл поправок АЧХ измерительного тракта
	float dc;								//Смещение постоянной составляющей в сигнале
	float gain;								//Усиление

#ifdef WIN32
	_CHAN_INFO_ANP();	 						//Конструктор по умолчанию		
	_CHAN_INFO_ANP(const _CHAN_INFO_ANP& inf);	//Конструктор копирования
#endif

} CHAN_INFO_ANP;

#ifdef WIN32
	bool ReadAnpFile(const CString& fname, CHAN_INFO_ANP* pChInfAnp);		//Читаем структуру CHAN_INFO_ANP из файла
	bool WriteAnpFile(CHAN_INFO_ANP *pChInfAnp, const char *fname);			//Пишем структуру CHAN_INFO_ANP в файл
#endif
