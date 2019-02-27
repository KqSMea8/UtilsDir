#ifndef _WATERMARKFLOAT 
#define _WATERMARKFLOAT

#define STARTBIT		1
#define STOPBIT			1
#define SIZEOFQUALITY	2
#define SIZEOFTIMELABEL	32
#define SIZEOFCRC		8
//размер посылки:		| стартовый бит |  метка времени   |       качество данных        |  контрольная сумма  | стоповый бит  
#define SIZEOFWMF		(STARTBIT		+ SIZEOFTIMELABEL +		4 * SIZEOFQUALITY  +			SIZEOFCRC		 + STOPBIT)
#define CRC_START_VAL	(char)0xE3

// Функция WMF_PutWatermark(...) помещает данные WATERMARK в младшие биты данных.
// Параметры:
//		pBuffer -		адрес буфера данных;
//		lSize -			размер буфера данных;
//		lRTC -			региональное время в секундах от 1970 года (проверяется на достоверность) максимальная дата до 2038 года
//		ucQuality -		качество:
//								0, 1 биты - качество синхронизации, задается от 0 до 3: 
//										0 - резерв,
//										1 - плохо,
//										2 - хорошо,
//										3 - отлично;
//								2, 3 биты - качество питания, задается от 0 до 3: 
//										0 - резерв,
//										1 - ниже нормы, 
//										2 - выше нормы,
//										3 - в диапазоне;
//								4, 5 биты - качество получения данных, задается от 0 до 3:
//										0 - резерв,
//										1 - плохо,
//										2 - хорошо,
//										3 - отлично;
//								6, 7 биты - резерв;
//		lStartWatermark - бит, начиная с которого продолжает ставиться Watermark
// Коды ошибок:
//		 0 - функция выполнена успешно;
//		-1 - размер буфера меньше размера пакета (данные по WATERMARK не заполняется);
//		-2 - метка времения имеет отрицательное значение (предупреждение, данные по WATERMARK заполняются).
long WMF_PutWatermark(float* pBuffer, long lSize, long lRTC, unsigned char ucQuality, long lStartWatermark);
// Функция WMF_ClearBit(...) обнуляет младший бит данных
// Параметры:
//		buffer -		адрес буфера данных;
//		size -			размер буфера данных.
// Коды ошибок:
//		 0 - функция выполнена успешно;
//		-1 - размер буфера меньше или равен 0;
long WMF_ClearBit(float* pBuffer, long lSize);

long WMF_FindPPS(float* pBuffer, long lSize, long* lAdress, long* lRTC, unsigned char* ucQuality);
long WMF_FindFirstPPS(float* pBuffer, long lSize, long* lAdress, long* lRTC, unsigned char* ucQuality);
long WMF_FindLastPPS(float* pBuffer, long lSize, long* lAdress, long* lRTC, unsigned char* ucQuality);

#endif