/*=============================================================================
	Project: 		Watermark
	Author:			Василевич М.
	Version: 		1.0
	Description:	Модуль работы с Watermark - декодер
	Documetation:	https://docs.google.com/document/d/1XWoFwWzcsTCq6BR-DrH720ivCJkCCg8CMAcBXhPnInA/edit
======= Update History =========================================================
   Date			Author		Rev		Reason Description 
07.06.2012		Василевич	1.0		Создание
==============================================================================*/

#include "string.h"
#include "CRC.h"
#include "WatermarkInt.h"
#include <vector>

/*-------------------------------------------------------------------------*/
/* Defines */

/*-------------------------------------------------------------------------*/
/* Constant */

/*-------------------------------------------------------------------------*/
/* Variables */

/*-------------------------------------------------------------------------*/
/* Functions */

bool WMI_CheckMessage(long *data, unsigned char *ID, char *mess_data);


//---------------------------------------------------------------------------
// Функция инициализации декодера
void WMI_InitDecoder(void)
{

}

//---------------------------------------------------------------------------
// Функция возвращает наличие (true) или отсутствие (false) синхронизации на текущем отсчёте
// sample - отсчёт
bool WMI_GetSampleSynch(long sample)
{
	if (sample & SYNCH_MASK)
		return true;
	else
		return false;
}

//---------------------------------------------------------------------------
// Функция возвращает наличие (true) или отсутствие (false) границы секунды (PPS) на текущем отсчёте
// sample - отсчёт
bool WMI_GetSamplePPS(long sample)
{
	if (sample & PPS_MASK)
		return true;
	else
		return false;
}

//---------------------------------------------------------------------------
// Функция возвращает наличие (true) или отсутствие (false) критической ошибки на текущем отсчёте
// sample - отсчёт
bool WMI_GetSampleCriticalError(long sample)
{
	if (sample & CRITICAL_ERROR_MASK)
		return true;
	else
		return false;
}

//---------------------------------------------------------------------------
// Функция ищет в потоке последний отсчёт с наличием синхронизации
// Возвращает наличие (true) или отсутствие (false) синхронизации в потоке
// *data - указатель на буфер отсчётов
// size - количество отсчётов в буфере
// *index - указатель на переменную, в которую функция пишет номер последнего отсчёта в буфере 
//			в котором найден признак наличия синхронизации
bool WMI_GetLastSynch(long *data, unsigned int size, unsigned int *index)
{
	bool ret(false);

	for (unsigned int i = 0; i < size; ++i)
	{
		if (WMI_GetSampleSynch(*(data + i)))
		{
			*index = i;
			ret = true;
		}
	}

	return ret;
}

//---------------------------------------------------------------------------
// Функция ищет в потоке первый отсчёт с наличием границы секунды
// Возвращает наличие (true) или отсутствие (false) границы секунды (PPS) в потоке
// *data - указатель на буфер отсчётов
// size - количество отсчётов в буфере
// *index - указатель на переменную, в которую функция пишет номер первого отсчёта 
//			в буфере в котором найден признак границы секунды
bool WMI_GetFirstPPS(long *data, unsigned int size, unsigned int *index)
{
	for (unsigned int i = 0; i < size; ++i)
	{
		if (WMI_GetSamplePPS(*(data + i)))
		{
			*index = i;
			return true;
		}
	}

	return false;
}

//---------------------------------------------------------------------------
// Функция ищет в потоке первый отсчёт с наличием критической ошибки
// Возвращает наличие (true) или отсутствие (false) критической ошибки в потоке
// *data - указатель на буфер отсчётов
// size - количество отсчётов в буфере
// *index - указатель на переменную, в которую функция пишет номер первого отсчёта 
//			в буфере в котором найден признак критической ошибки

bool WMI_GetFirstCriticalError(long *data, unsigned int size, unsigned int *index)
{
	for (unsigned int i = 0; i < size; ++i)
	{
		if (WMI_GetSampleCriticalError(*(data + i)))
		{
			*index = i;
			return true;
		}
	}

	return false;
}

//---------------------------------------------------------------------------
// Функция ищет в потоке последний отсчёт с наличием границы секунды
// Возвращает наличие (true) или отсутствие (false) границы секунды (PPS) в потоке
// *data - указатель на буфер отсчётов
// size - количество отсчётов в буфере
// *index - указатель на переменную, в которую функция пишет номер последнего отсчёта 
//			в буфере в котором найден признак границы секунды
bool WMI_GetLastPPS(long *data, unsigned int size, unsigned int *index)
{
	bool ret(false);

	for (unsigned int i = 0; i < size; ++i)
	{
		if (WMI_GetSamplePPS(*(data + i)))
		{
			*index = i;
			ret = true;
		}
	}

	return ret;
}

//---------------------------------------------------------------------------
// Функция ищет в потоке первую временную метку (сообщение целиком)
// Возвращает номер отсчета, начиная с которого найдена временная метка, в противном случае возвращает -1.
// *data - указатель на буфер отсчётов
// size - количество отсчётов в буфере
// *rtc_time - указатель на время UTC или региональное время
// *diff_pps - указатель на разность между внутренним и внешним PPS, знаковые 24 бита с дискретом в 100нс.
// *synch_lev - указатель на качество синхронизации
// *pow_lev - указатель на качество питания
// *dat_lev - указатель на качество получения данных
int WMI_GetFirstTimeLabel( long *data, unsigned int size, long *rtc_time, long *diff_pps, unsigned char *synch_lev, unsigned char *pow_lev, unsigned char *dat_lev, unsigned char *reserv_lev )
{
	char buf[MESS_SZ] = { 0 };

	// пробуем найти сообщение с совпадающей CRC8
	for (unsigned int i = 0; i < size; ++i)
	{
		// по длине сообщения
		for (unsigned char j = 0; (j < MESS_SZ * 2) && ((i + j + 1) < size); j += 2)
		{
			buf[j / 2] = (char)(*(data + i + j) & 0x0000000F | ((*(data + i + j + 1) & 0x0000000F) << 4));
		}
		// Проверяем, что ID == TIME_LABEL, есть признак PPS и совпадает CRC8
		if (buf[0] == TIME_LABLE_ID && (data[i] & PPS_MASK) &&
			GetCrc8(buf, MESS_SZ - 1, CRC_START_VAL) == buf[MESS_SZ - 1])
		{
			memcpy_s(rtc_time, 4, & buf[1], 4);
			// заполняем единицами или нулями в зависимости от знака числа в буфере
			*diff_pps = (buf[7] & 0x80) ? -1 : 0;
			memcpy_s(diff_pps, 3, & buf[5], 3);
			*synch_lev = buf[8] & 0x03;
			*pow_lev = (buf[8] >> 2) & 0x03;
			*dat_lev = (buf[8] >> 4) & 0x03;
			*reserv_lev = (buf[8] >> 6) & 0x03;
			return i;
		}
	}
	
	return -1;
}

//---------------------------------------------------------------------------
// Функция ищет в потоке последнюю временную метку (сообщение целиком)
// Возвращает номер отсчета, начиная с которого найдена временная метка, в противном случае возвращает -1.
// *data - указатель на буфер отсчётов
// size - количество отсчётов в буфере
// *rtc_time - указатель на время UTC или региональное время
// *diff_pps - указатель на разность между внутренним и внешним PPS, знаковые 24 бита с дискретом в 100нс.
// *synch_lev - указатель на качество синхронизации
// *pow_lev - указатель на качество питания
// *dat_lev - указатель на качество получения данных
int WMI_GetLastTimeLabel( long *data, unsigned int size, long *rtc_time, long *diff_pps, unsigned char *synch_lev, unsigned char *pow_lev, unsigned char *dat_lev, unsigned char *reserv_lev )
{
	char buf[MESS_SZ] = { 0 };
	int iRetIndex(-1);

	// пробуем найти сообщение с совпадающей CRC8
	for (unsigned int i = 0; i < size; ++i)
	{
		// по длине сообщения
		for (unsigned char j = 0; (j < MESS_SZ * 2) && ((i + j + 1) < size); j += 2)
		{
			buf[j / 2] = (char)(*(data + i + j) & 0x0000000F | ((*(data + i + j + 1) & 0x0000000F) << 4));
		}
		// Проверяем, что ID == TIME_LABEL, есть признак PPS и совпадает CRC8
		if (buf[0] == TIME_LABLE_ID && (data[i] & PPS_MASK) &&
			GetCrc8(buf, MESS_SZ - 1, CRC_START_VAL) == buf[MESS_SZ - 1])
		{
			memcpy_s(rtc_time, 4, & buf[1], 4);
			// заполняем единицами или нулями в зависимости от знака числа в буфере
			*diff_pps = (buf[7] & 0x80) ? -1 : 0;
			memcpy_s(diff_pps, 3, & buf[5], 3);
			*synch_lev = buf[8] & 0x03;
			*pow_lev = (buf[8] >> 2) & 0x03;
			*dat_lev = (buf[8] >> 4) & 0x03;
			*reserv_lev = (buf[8] >> 6) & 0x03;
			iRetIndex = i;
			// пропускаем найденное сообщение
			i += (MESS_SZ * 2 - 1);
		}
	}
	
	return iRetIndex;
}

//---------------------------------------------------------------------------
// Функция ищет в потоке первое стартовое сообщение (сообщение целиком)
// Возвращает true, если оно найдено, в противном случае возвращает false
// *data - указатель на буфер отсчётов
// size - количество отсчётов в буфере
// *index - указатель на переменную, в которую функция возвращает номер отсчёта в буфере, 
//			на который выпадает начало стартового сообщения
// *ID - указатель на переменную, в которую функция записывает ID стартового сообщения
// *struct_vers - указатель на переменную, в которую функция записывает версию структуры данных
// *struct_sz - указатель на переменную, в которую функция записывает размер структуры данных
bool WMI_GetStartMessage(long *data, unsigned int size, unsigned int *index, unsigned char *ID, 
						unsigned char *struct_vers, unsigned short *struct_sz)
{
	char buf[MESS_SZ];
	char tempID;

	// пробуем найти сообщение с совпадающей CRC8
	for (unsigned int i = 0; i < size; i++)
	{
		// по длине сообщения
		for (unsigned char j = 0; (j < MESS_SZ * 2) && ((i + j + 1) < size); j += 2)
		{
			buf[j / 2] = (char)((*(data + i + j) & 0x0000000F) | ((*(data + i + j + 1) & 0x0000000F) << 4));
		}
		if (GetCrc8(buf, MESS_SZ - 1, CRC_START_VAL) == buf[MESS_SZ - 1])
		{
			tempID = buf[0];
			// если это ID стартового сообщения
			if ((tempID == 0x02) || (tempID == 0x05) || (tempID == 0x08) || (tempID == 0x0B))
			{
				*ID = tempID;
				*struct_vers = buf[1];
				*struct_sz = buf[2] + (((unsigned short)buf[3]) << 8);
				*index = i;

				return true;
			}
		}
	}

	return false;
}

//---------------------------------------------------------------------------
// Функция проверяет целостность и возвращает ID сообщения
// Возвращает true, если контрольная сумма совпадает, иначе - false
// *data - указатель на буфер отсчётов, начало которого должно быть на начале сообщения
// *ID - указатель на переменную, в которую функция записывает ID сообщения
bool WMI_CheckMessage(long *data, unsigned char *ID, char *mess_data)
{
	bool ret(false);
	char buf[MESS_SZ] = {""};

	for (unsigned int i = 0; i < MESS_SZ * 2; i += 2)
	{
		buf[i / 2] = (*(data + i) & 0x0000000F) | ((*(data + i + 1) & 0x0000000F) << 4); 
	}
	if (GetCrc8(buf, MESS_SZ - 1, CRC_START_VAL) == buf[MESS_SZ - 1])
	{
		*ID = buf[0] & 0x0F;
		memcpy_s(mess_data, DATA_SZ, &buf[1], DATA_SZ);
		ret = true;
	}

	return ret;
}

//---------------------------------------------------------------------------
// Функция записывает данные сообщения в указанный буфер
// Возвращает true в случае успешной свёртки сообщения, false - в случае ошибки
// *data - указатель на буфер отсчётов, начало которого должно быть на отсчёте с номером index, 
//			возвращаемом функцией WM_GetStartMessage()
// size - количество отсчётов в буфере, должен быть не менее SAMPLE_QNT (см. примечание), 
//			чтобы влезла вся триада сообщений (стартовое, стоповое и содержательные)
// *mess - указатель на начало буфера сообщений
bool WMI_GetMessageData(long *data, unsigned int size, void *mess)
{
	char *ptr = (char *)mess;
	char buf[DATA_SZ];
	unsigned int Idx_val;
	unsigned char ID_val, temp_ID_val;
	unsigned char Struct_vers_val;
	unsigned short Struct_size_val;
	unsigned int i = 0, j = 0;

	// если стартовое сообщение верное
	if (WMI_GetStartMessage(data, MESS_SZ * 2, &Idx_val, &ID_val, &Struct_vers_val, &Struct_size_val))
	{
		// ищем следующее содержательное сообщение
		for (i = MESS_SZ * 2; i < size; i += MESS_SZ * 2)
		{
			if (WMI_CheckMessage((data + i), &temp_ID_val, buf))
			{
				// если содержательное сообщение принадлежит указанному стартовому сообщению
				if (temp_ID_val == (ID_val + 1))
				{
					// если последнее содержательное сообщение
					if (Struct_size_val < DATA_SZ)
					{
						memcpy_s((ptr + j), Struct_size_val, buf, Struct_size_val);
						break;
					}
					else
					{
						memcpy_s((ptr + j), DATA_SZ, buf, DATA_SZ);
						Struct_size_val -= DATA_SZ;
						j += DATA_SZ;
						if (!Struct_size_val)
							break;
					}
				}
			}
		}
	}

	// если следующее сообщение стоповое
	if (WMI_CheckMessage((data + i + MESS_SZ * 2), &temp_ID_val, buf))
	{
		if (temp_ID_val == (ID_val + 2))
			return true;
	}

	return false;
}

//---------------------------------------------------------------------------
// Функция пересчитывает и возвращает SAMPLE_QNT (см. описание WM_GetMessageData())
// struct_sz - размер структуры данных из WM_GetStartMessage()
// freq - частота дискретизации сигнала
// channel_num - количество каналов
unsigned long WMI_GetMessageDataSize(unsigned short struct_sz, unsigned long freq, unsigned short channel_num)
{
	unsigned long clear_sz;

	// размер без учёта временной метки
	if (struct_sz % DATA_SZ)
		clear_sz = MESS_SZ * 2 * (struct_sz / DATA_SZ + 1 + 2);
	else
		clear_sz = MESS_SZ * 2 * (struct_sz / DATA_SZ + 2);

	// с учётом временной метки, которая могла быть записана на этом интервале
	return clear_sz + MESS_SZ * 2 * (clear_sz / (freq * channel_num) + 1);
}

//---------------------------------------------------------------------------
