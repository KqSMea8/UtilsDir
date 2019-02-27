#ifndef _WATERMARK_H_ 
#define _WATERMARK_H_ 

/*----------------------------------------------------------------------------*/
/* Defines */

// маски с привязкой к отсчёту
#define SYNCH_MASK			0x80
#define PPS_MASK			0x40
#define CRITICAL_ERROR_MASK	0x20

// уровни качества синхронизации
#define SYNCH_BAD		0x01
#define SYNCH_GOOD		0x02
#define SYNCH_FINE		0x03

// уровни качества питания
#define POWER_LOW		0x01
#define POWER_HIGH		0x02
#define POWER_NORM		0x03

// уровни качества получения данных
#define DATA_BAD		0x01
#define DATA_GOOD		0x02
#define DATA_FINE		0x03

#define PARAM_NOT_SUPPORT    0x00

// типы сообщений (ID содержательных сообщений)
#define TIME_LABLE_ID		0x01	// временная метка
#define DEVICE_DESCRIPT_ID	0x03	// описатель устройства

// размер данные в сообщении, байт
#define DATA_SZ			8
// размер сообщения, байт
#define MESS_SZ			(DATA_SZ + 1 + 1)

// начальное значение CRC
#define CRC_START_VAL	(char)0xE3


/*----------------------------------------------------------------------------*/
/* Data Types */

typedef struct _STRUCT_MESSAGE
{
	char mesID; // ID сообщения
	char *data; // указатель, т.е. никаких промежуточных буферов и значения всегда актуальные
	char CRC8; // контрольная сумма по полю данных сообщения
	_STRUCT_MESSAGE()
		: mesID(0)
		, data(nullptr)
		, CRC8(0)
	{
	}
} STRUCT_MESSAGE;


/*----------------------------------------------------------------------------*/
/* Macros */

// альтернатива функции WM_MarkSample(), где требуется скорость
#define WM_MARK_SAMPLE(sample, mask)	\
	*(char *)(sample) = (mask);


/*----------------------------------------------------------------------------*/
/* Variables */

/*----------------------------------------------------------------------------*/
/* Functions */

// Coder
void WMI_InitCoder(void);
void WMI_MarkSample(long *sample, unsigned char mask);
void WMI_PutTimeLabel(long rtc_time, long diff_pps, 
					unsigned char synch_lev, unsigned char pow_lev, unsigned char dat_lev);
void WMI_AddMessageList(char *data, unsigned char ID, unsigned char struct_vers, unsigned short size);
void WMI_ProcessCoder(long *data, unsigned int size);

// Decoder
void WMI_InitDecoder(void);
bool WMI_GetSampleSynch(long sample);
bool WMI_GetSamplePPS(long sample);
bool WMI_GetSampleCriticalError(long sample);
bool WMI_GetLastSynch(long *data, unsigned int size, unsigned int *index);
bool WMI_GetFirstPPS(long *data, unsigned int size, unsigned int *index);
bool WMI_GetFirstCriticalError(long *data, unsigned int size, unsigned int *index);
bool WMI_GetLastPPS(long *data, unsigned int size, unsigned int *index);
int WMI_GetFirstTimeLabel(long *data, unsigned int size, long *rtc_time, long *diff_pps, unsigned char *synch_lev, unsigned char *pow_lev, unsigned char *dat_lev, unsigned char *reserv_lev);
int WMI_GetLastTimeLabel(long *data, unsigned int size, long *rtc_time, long *diff_pps, unsigned char *synch_lev, unsigned char *pow_lev, unsigned char *dat_lev, unsigned char *reserv_lev);
bool WMI_GetStartMessage(long *data, unsigned int size, unsigned int *index, unsigned char *ID, 
						unsigned char *struct_vers, unsigned short *struct_sz);
bool WMI_GetMessageData(long *data, unsigned int size, void *mess);
unsigned long WMI_GetMessageDataSize(unsigned short struct_sz, unsigned long freq, unsigned short channel_num);


/*----------------------------------------------------------------------------*/

#endif /* #ifndef _WATERMARK_H_ */
