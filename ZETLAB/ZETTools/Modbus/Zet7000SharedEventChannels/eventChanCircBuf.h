#ifndef EVENT_CHAN_CIRC_BUF_HEADER
#define EVENT_CHAN_CIRC_BUF_HEADER
#include <Buffer\SmartArray.h>

	#define MAX_SIZE_CIRC_BUF_FOR_EVENT_CHANNEL 20000   // Максимальный размер буфера данных под событийный канал
	#define MAX_SIZE_EVENT_DATA                 100	   // Максимальный размер данных события (в байтах)
	#define MAX_NUM_EVENT_CHANNELS              20     // Максимальное кол-во событийных каналов на один датчик
	#define EV_CH_FAIL_RESULT                   -1     // Провальный результат работы функции
	#define EV_CH_ERROR_STATE_CIRC_BUF          1      // Ошибка буфера

	#define MAX_SIZE_EVENT_CHANNEL_NAME 		100		// Максимальный размер имени событийного канала
	#define MAX_SIZE_EVENT_MEASURE_NAME			50		// Максимальный размер единицы измерения событийного канала

	struct EVENT_CHANNEL_CIRC_BUF
	{
		long      ftdindex;		     // Номер MODBUS линии
		long      node;			     // Адрес устройства на MODBUS линии
		long      quanEventChannels; // Количество событийных каналов
		 
		long      posWrite;			 // Указатель места записи для новых данных
		long long nextEventId;		 // Идентификатор для следующего пакета

		bool      activeChannels[MAX_NUM_EVENT_CHANNELS];	// Данные о активности событийных каналов
		char      namesEventChannels[MAX_NUM_EVENT_CHANNELS][MAX_SIZE_EVENT_CHANNEL_NAME];   // Данные о именах событийных каналов
		char      measureEventChannels[MAX_NUM_EVENT_CHANNELS][MAX_SIZE_EVENT_MEASURE_NAME];   // Данные о единицах измерения событийных каналов

		char      buffEvent[MAX_SIZE_CIRC_BUF_FOR_EVENT_CHANNEL][MAX_SIZE_EVENT_DATA];	// Данные
		long long eventId  [MAX_SIZE_CIRC_BUF_FOR_EVENT_CHANNEL];                      // ID события
		long      eventTimeSec[MAX_SIZE_CIRC_BUF_FOR_EVENT_CHANNEL];
		double    eventTimeMkSec[MAX_SIZE_CIRC_BUF_FOR_EVENT_CHANNEL];
		long err;
	};

	struct ALL_EVENT_CHANNELS_CIRC_BUF
	{
		EVENT_CHANNEL_CIRC_BUF eventChannelCircBuf[MAX_NUM_EVENT_CHANNELS];
	};

	#ifdef __cplusplus
	extern "C" {
	#endif

	// Инициализация буфера приёма параметрических данных
	long EvCh_Init(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, unsigned long num_, long ftdindex_, long node_, long quanEventChannels_);
	// Поиск нужного буфера по номеру линии в программе MODBUS-ZETLAB & адресу устройства
	long EvCh_FindEventChannel(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_);
	// Поиск не занятого буфера
	long EvCh_FindEmptyEventChannel(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_);
	// Сброс буфера приёма параметрических данных
	long EvCh_Reset(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_);
	long EvCh_ResetBuf(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long numBuf_);
	// Установка статуса событийного канала
	void EvCh_SetActiveChannel(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_, long num_channel_, bool status_);
	bool EvCh_GetActiveChannel(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_, long num_channel_);
	// Установка имени событийного канала
	long EvCh_SetNameChannel(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_, long num_channel_, char* name_, long size_name_);
	long EvCh_GetNameChannel(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_, long num_channel_, char* name_, long size_name_);
	// Установка единицы измерения событийного канала
	long EvCh_SetMeasureChannel(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_, long num_channel_, char* measure_, long size_measure_);
	long EvCh_GetMeasureChannel(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_, long num_channel_, char* measure_, long size_measure_);
	// Подсчёт количества активных событийных каналов
	long EvCh_GetNumActiveChannel(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_);
	// Добавление события в буфер приёма
	long EvCh_PutEvent(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_, void* buffer_, long size_);
	// Достать событие из буфера приёма событий
	long long EvCh_GetEvent(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_, long long eventId_, void* buffer_, long* size_);
	// Поиск ID события произошедшего возле определённого времени
	long long EvCh_GetIDTime(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_, long eventTimeSec_, double eventTimeMkSec_);
	// Поиск ID самого давнего события в буфере приёма
	long long EvCh_GetFirstEventID(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_);
	// Поиск ID самого нового пакета
	long long EvCh_GetLastEventID(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_);
	// Считать ошибку конкретного буфера
	long EvCh_GetError(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, unsigned long num_);
	// Определение принадлежности буфера
	long EvCh_GetNumFTDIndex(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, unsigned long num_);
	long EvCh_GetNode(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, unsigned long num_);
	long EvCh_GetQuanEventChannels(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, unsigned long num_);

	#ifdef __cplusplus
	} /* extern "C" */
	#endif

#endif
