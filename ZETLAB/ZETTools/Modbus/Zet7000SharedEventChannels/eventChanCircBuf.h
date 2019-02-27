#ifndef EVENT_CHAN_CIRC_BUF_HEADER
#define EVENT_CHAN_CIRC_BUF_HEADER
#include <Buffer\SmartArray.h>

	#define MAX_SIZE_CIRC_BUF_FOR_EVENT_CHANNEL 20000   // ������������ ������ ������ ������ ��� ���������� �����
	#define MAX_SIZE_EVENT_DATA                 100	   // ������������ ������ ������ ������� (� ������)
	#define MAX_NUM_EVENT_CHANNELS              20     // ������������ ���-�� ���������� ������� �� ���� ������
	#define EV_CH_FAIL_RESULT                   -1     // ���������� ��������� ������ �������
	#define EV_CH_ERROR_STATE_CIRC_BUF          1      // ������ ������

	#define MAX_SIZE_EVENT_CHANNEL_NAME 		100		// ������������ ������ ����� ����������� ������
	#define MAX_SIZE_EVENT_MEASURE_NAME			50		// ������������ ������ ������� ��������� ����������� ������

	struct EVENT_CHANNEL_CIRC_BUF
	{
		long      ftdindex;		     // ����� MODBUS �����
		long      node;			     // ����� ���������� �� MODBUS �����
		long      quanEventChannels; // ���������� ���������� �������
		 
		long      posWrite;			 // ��������� ����� ������ ��� ����� ������
		long long nextEventId;		 // ������������� ��� ���������� ������

		bool      activeChannels[MAX_NUM_EVENT_CHANNELS];	// ������ � ���������� ���������� �������
		char      namesEventChannels[MAX_NUM_EVENT_CHANNELS][MAX_SIZE_EVENT_CHANNEL_NAME];   // ������ � ������ ���������� �������
		char      measureEventChannels[MAX_NUM_EVENT_CHANNELS][MAX_SIZE_EVENT_MEASURE_NAME];   // ������ � �������� ��������� ���������� �������

		char      buffEvent[MAX_SIZE_CIRC_BUF_FOR_EVENT_CHANNEL][MAX_SIZE_EVENT_DATA];	// ������
		long long eventId  [MAX_SIZE_CIRC_BUF_FOR_EVENT_CHANNEL];                      // ID �������
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

	// ������������� ������ ����� ��������������� ������
	long EvCh_Init(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, unsigned long num_, long ftdindex_, long node_, long quanEventChannels_);
	// ����� ������� ������ �� ������ ����� � ��������� MODBUS-ZETLAB & ������ ����������
	long EvCh_FindEventChannel(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_);
	// ����� �� �������� ������
	long EvCh_FindEmptyEventChannel(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_);
	// ����� ������ ����� ��������������� ������
	long EvCh_Reset(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_);
	long EvCh_ResetBuf(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long numBuf_);
	// ��������� ������� ����������� ������
	void EvCh_SetActiveChannel(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_, long num_channel_, bool status_);
	bool EvCh_GetActiveChannel(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_, long num_channel_);
	// ��������� ����� ����������� ������
	long EvCh_SetNameChannel(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_, long num_channel_, char* name_, long size_name_);
	long EvCh_GetNameChannel(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_, long num_channel_, char* name_, long size_name_);
	// ��������� ������� ��������� ����������� ������
	long EvCh_SetMeasureChannel(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_, long num_channel_, char* measure_, long size_measure_);
	long EvCh_GetMeasureChannel(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_, long num_channel_, char* measure_, long size_measure_);
	// ������� ���������� �������� ���������� �������
	long EvCh_GetNumActiveChannel(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_);
	// ���������� ������� � ����� �����
	long EvCh_PutEvent(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_, void* buffer_, long size_);
	// ������� ������� �� ������ ����� �������
	long long EvCh_GetEvent(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_, long long eventId_, void* buffer_, long* size_);
	// ����� ID ������� ������������� ����� ������������ �������
	long long EvCh_GetIDTime(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_, long eventTimeSec_, double eventTimeMkSec_);
	// ����� ID ������ ������� ������� � ������ �����
	long long EvCh_GetFirstEventID(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_);
	// ����� ID ������ ������ ������
	long long EvCh_GetLastEventID(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_);
	// ������� ������ ����������� ������
	long EvCh_GetError(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, unsigned long num_);
	// ����������� �������������� ������
	long EvCh_GetNumFTDIndex(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, unsigned long num_);
	long EvCh_GetNode(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, unsigned long num_);
	long EvCh_GetQuanEventChannels(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, unsigned long num_);

	#ifdef __cplusplus
	} /* extern "C" */
	#endif

#endif
