#include "stdafx.h"
#include "EventChanCircBuf.h"
#include <algorithm>

long EvCh_Init(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, unsigned long num_, long ftdindex_, long node_, long quanEventChannels_)
{
	long err = 0;
	EVENT_CHANNEL_CIRC_BUF *currEventChannel = &allEventChannels_->eventChannelCircBuf[num_];

	currEventChannel->err = 0;
	currEventChannel->ftdindex = ftdindex_;
	currEventChannel->node = node_;
	currEventChannel->quanEventChannels = quanEventChannels_;
	currEventChannel->posWrite = 0;
	currEventChannel->nextEventId = 1;
	
	//исправлена из-за исправления ошибки найденной 2014.09.04 pvs studio
//	memset(currEventChannel->buffEvent,      0x00, MAX_SIZE_CIRC_BUF_FOR_EVENT_CHANNEL*MAX_SIZE_EVENT_DATA);
	memset(currEventChannel->buffEvent,      0x00, MAX_SIZE_CIRC_BUF_FOR_EVENT_CHANNEL*MAX_SIZE_EVENT_DATA);
	memset(currEventChannel->eventId,		 0x00, sizeof(currEventChannel->eventId));
//	memset(currEventChannel->eventId,		 0x00, MAX_SIZE_CIRC_BUF_FOR_EVENT_CHANNEL);
	memset(currEventChannel->eventTimeSec,   0x00, MAX_SIZE_CIRC_BUF_FOR_EVENT_CHANNEL*sizeof(long));
	memset(currEventChannel->eventTimeMkSec, 0x00, MAX_SIZE_CIRC_BUF_FOR_EVENT_CHANNEL*sizeof(double));

	memset(currEventChannel->activeChannels, 0x00, MAX_NUM_EVENT_CHANNELS*sizeof(bool));
	memset(currEventChannel->namesEventChannels, 0x00, MAX_NUM_EVENT_CHANNELS*MAX_SIZE_EVENT_CHANNEL_NAME*sizeof(char));
	memset(currEventChannel->measureEventChannels, 0x00, MAX_NUM_EVENT_CHANNELS*MAX_SIZE_EVENT_MEASURE_NAME*sizeof(char));
	
	return err;
}

long EvCh_FindEventChannel(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_)
{
	long numEventChannel = EV_CH_FAIL_RESULT;
	EVENT_CHANNEL_CIRC_BUF *currEventChannel;

	for (unsigned int currEventChan = 0; currEventChan < MAX_NUM_EVENT_CHANNELS; currEventChan++)
	{
		currEventChannel = &(allEventChannels_->eventChannelCircBuf[currEventChan]);

		if ( ((*currEventChannel).ftdindex == ftdindex_) && ((*currEventChannel).node == node_) && (node_ > 0) )
		{
			numEventChannel = currEventChan;
			break;
		}			
	}
	return numEventChannel;
}

long EvCh_FindEmptyEventChannel(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_)
{
	long numEventChannel = EV_CH_FAIL_RESULT;
	EVENT_CHANNEL_CIRC_BUF *currEventChannel;

	for (unsigned int currEventChan = 0; currEventChan < MAX_NUM_EVENT_CHANNELS; currEventChan ++)
	{
		currEventChannel = &allEventChannels_->eventChannelCircBuf[currEventChan];
		if ( (*currEventChannel).node == 0 )
		{
			numEventChannel = currEventChan;
			break;
		}
	}
	return numEventChannel;
}

long EvCh_Reset(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_)
{
	long numChan = EvCh_FindEventChannel(allEventChannels_, ftdindex_,  node_);

	if (numChan != EV_CH_FAIL_RESULT)
	{
		EVENT_CHANNEL_CIRC_BUF *currEventChannel = &allEventChannels_->eventChannelCircBuf[numChan];

		(*currEventChannel).err               = 0;
		(*currEventChannel).ftdindex          = 0;
		(*currEventChannel).node              = 0;
		(*currEventChannel).quanEventChannels = 0;
		(*currEventChannel).posWrite          = 0;
		(*currEventChannel).nextEventId       = 1;

		memset((*currEventChannel).buffEvent,      0x00, MAX_SIZE_CIRC_BUF_FOR_EVENT_CHANNEL*MAX_SIZE_EVENT_DATA);
		memset((*currEventChannel).eventId,        0x00, MAX_SIZE_CIRC_BUF_FOR_EVENT_CHANNEL*sizeof(long long));
		memset((*currEventChannel).eventTimeSec,   0x00, MAX_SIZE_CIRC_BUF_FOR_EVENT_CHANNEL*sizeof(long));
		memset((*currEventChannel).eventTimeMkSec, 0x00, MAX_SIZE_CIRC_BUF_FOR_EVENT_CHANNEL*sizeof(double));

		memset(currEventChannel->activeChannels, 0x00, MAX_NUM_EVENT_CHANNELS*sizeof(bool));
		memset(currEventChannel->namesEventChannels, 0x00, MAX_NUM_EVENT_CHANNELS*MAX_SIZE_EVENT_CHANNEL_NAME*sizeof(char));
		memset(currEventChannel->measureEventChannels, 0x00, MAX_NUM_EVENT_CHANNELS*MAX_SIZE_EVENT_MEASURE_NAME*sizeof(char));
	}

	return numChan;
}

long EvCh_ResetBuf(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long numBuf_)
{
	EVENT_CHANNEL_CIRC_BUF *currEventChannel = &allEventChannels_->eventChannelCircBuf[numBuf_];

	(*currEventChannel).err               = 0;
	(*currEventChannel).ftdindex          = 0;
	(*currEventChannel).node              = 0;
	(*currEventChannel).quanEventChannels = 0;
	(*currEventChannel).posWrite          = 0;
	(*currEventChannel).nextEventId       = 1;

	memset((*currEventChannel).buffEvent,      0x00, MAX_SIZE_CIRC_BUF_FOR_EVENT_CHANNEL*MAX_SIZE_EVENT_DATA);
	memset((*currEventChannel).eventId,        0x00, MAX_SIZE_CIRC_BUF_FOR_EVENT_CHANNEL*sizeof(long long));
	memset((*currEventChannel).eventTimeSec,   0x00, MAX_SIZE_CIRC_BUF_FOR_EVENT_CHANNEL*sizeof(long));
	memset((*currEventChannel).eventTimeMkSec, 0x00, MAX_SIZE_CIRC_BUF_FOR_EVENT_CHANNEL*sizeof(double));

	memset(currEventChannel->activeChannels, 0x00, MAX_NUM_EVENT_CHANNELS*sizeof(bool));
	memset(currEventChannel->namesEventChannels, 0x00, MAX_NUM_EVENT_CHANNELS*MAX_SIZE_EVENT_CHANNEL_NAME*sizeof(char));
	memset(currEventChannel->measureEventChannels, 0x00, MAX_NUM_EVENT_CHANNELS*MAX_SIZE_EVENT_MEASURE_NAME*sizeof(char));

	return numBuf_;
}

void EvCh_SetActiveChannel(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_, long num_channel_, bool status_)
{
	long numChan = EvCh_FindEventChannel(allEventChannels_, ftdindex_,  node_);
	if (numChan != EV_CH_FAIL_RESULT)
	{
		if ( num_channel_ < MAX_NUM_EVENT_CHANNELS )
		{
			EVENT_CHANNEL_CIRC_BUF *currEventChannel = &allEventChannels_->eventChannelCircBuf[numChan];
			currEventChannel->activeChannels[num_channel_] = status_;
		}
	}
}

bool EvCh_GetActiveChannel(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_, long num_channel_)
{
	bool returnVal = false;

	long numChan = EvCh_FindEventChannel(allEventChannels_, ftdindex_,  node_);
	if (numChan != EV_CH_FAIL_RESULT)
	{
		if ( num_channel_ < MAX_NUM_EVENT_CHANNELS )
		{
			EVENT_CHANNEL_CIRC_BUF *currEventChannel = &allEventChannels_->eventChannelCircBuf[numChan];
			returnVal = currEventChannel->activeChannels[num_channel_];
		}
	}

	return returnVal;
}

long EvCh_SetNameChannel(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_, long num_channel_, char* name_, long size_name_)
{
	long retVal = -1;
	long numChan = EvCh_FindEventChannel(allEventChannels_, ftdindex_, node_);
	if (numChan != EV_CH_FAIL_RESULT)
	{
		if (num_channel_ < MAX_NUM_EVENT_CHANNELS)
		{
			EVENT_CHANNEL_CIRC_BUF *currEventChannel = &allEventChannels_->eventChannelCircBuf[numChan];
			if (size_name_ > 0 && size_name_ <= MAX_SIZE_EVENT_CHANNEL_NAME)
			{
				memcpy_s(currEventChannel->namesEventChannels[num_channel_], MAX_SIZE_EVENT_CHANNEL_NAME, name_, size_name_);
				retVal = 0;
			}
		}
	}
	return retVal;
}

long EvCh_GetNameChannel(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_, long num_channel_, char* name_, long size_name_)
{
	long retVal = -1;

	long numChan = EvCh_FindEventChannel(allEventChannels_, ftdindex_, node_);
	if (numChan != EV_CH_FAIL_RESULT)
	{
		if (num_channel_ < MAX_NUM_EVENT_CHANNELS)
		{
			EVENT_CHANNEL_CIRC_BUF *currEventChannel = &allEventChannels_->eventChannelCircBuf[numChan];
			if (size_name_ > 0 && size_name_ <= MAX_SIZE_EVENT_CHANNEL_NAME)
			{
				memcpy_s(name_, size_name_, currEventChannel->namesEventChannels[num_channel_], MAX_SIZE_EVENT_CHANNEL_NAME);
				retVal = 0;
			}
		}
	}

	return retVal;
}

long EvCh_SetMeasureChannel(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_, long num_channel_, char* measure_, long size_measure_)
{
	long retVal = -1;
	long numChan = EvCh_FindEventChannel(allEventChannels_, ftdindex_, node_);
	if (numChan != EV_CH_FAIL_RESULT)
	{
		if (num_channel_ < MAX_NUM_EVENT_CHANNELS)
		{
			EVENT_CHANNEL_CIRC_BUF *currEventChannel = &allEventChannels_->eventChannelCircBuf[numChan];
			if (size_measure_ > 0 && size_measure_ <= MAX_SIZE_EVENT_MEASURE_NAME)
			{
				memcpy_s(currEventChannel->measureEventChannels[num_channel_], MAX_SIZE_EVENT_MEASURE_NAME, measure_, size_measure_);
				retVal = 0;
			}
		}
	}
	return retVal;
}

long EvCh_GetMeasureChannel(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_, long num_channel_, char* measure_, long size_measure_)
{
	long retVal = -1;

	long numChan = EvCh_FindEventChannel(allEventChannels_, ftdindex_, node_);
	if (numChan != EV_CH_FAIL_RESULT)
	{
		if (num_channel_ < MAX_NUM_EVENT_CHANNELS)
		{
			EVENT_CHANNEL_CIRC_BUF *currEventChannel = &allEventChannels_->eventChannelCircBuf[numChan];
			if (size_measure_ > 0 && size_measure_ <= MAX_SIZE_EVENT_MEASURE_NAME)
			{
				memcpy_s(measure_, size_measure_, currEventChannel->measureEventChannels[num_channel_], MAX_SIZE_EVENT_MEASURE_NAME);
				retVal = 0;
			}
		}
	}

	return retVal;
}

long EvCh_GetNumActiveChannel(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_)
{
	long returnVal = 0;

	long numChan = EvCh_FindEventChannel(allEventChannels_, ftdindex_,  node_);
	if (numChan != EV_CH_FAIL_RESULT)
	{
		EVENT_CHANNEL_CIRC_BUF *currEventChannel = &allEventChannels_->eventChannelCircBuf[numChan];
		for (long i = 0; i < currEventChannel->quanEventChannels; ++i)
			if ( currEventChannel->activeChannels[i] == true )
				returnVal ++;
	}

	return returnVal;
}

long EvCh_PutEvent(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_, void* buffer_, long size_)
{
	long err = -1;

	long numChan = EvCh_FindEventChannel(allEventChannels_, ftdindex_,  node_);
	if (numChan != EV_CH_FAIL_RESULT)
	{
		err = numChan;
		if ( (size_ >= long(sizeof(long long) + EvCh_GetQuanEventChannels(allEventChannels_, numChan)*sizeof(float) + sizeof(short))) && (size_ <= MAX_SIZE_EVENT_DATA) ) 
		{
			EVENT_CHANNEL_CIRC_BUF *currEventChannel = &allEventChannels_->eventChannelCircBuf[numChan];

			memcpy_s((*currEventChannel).buffEvent[ (*currEventChannel).posWrite ], size_, buffer_, size_);
			long long currEventTime = *(long long*)(buffer_);
			(*currEventChannel).eventTimeSec  [ (*currEventChannel).posWrite ] = currEventTime & 0x00000000ffffffff;
			(*currEventChannel).eventTimeMkSec[ (*currEventChannel).posWrite ] = (float)(currEventTime >> 32) / (float)1000000;

			(*currEventChannel).posWrite ++;
			if ( (*currEventChannel).posWrite >= MAX_SIZE_CIRC_BUF_FOR_EVENT_CHANNEL )
				(*currEventChannel).posWrite = 0;

			(*currEventChannel).eventId[ (*currEventChannel).posWrite ] = (*currEventChannel).nextEventId;
			(*currEventChannel).nextEventId ++;
		}
	}

	return err;
}

long long EvCh_GetEvent(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_, long long eventId_, void* buffer_, long* size_)
{
	long long err = -1;

	long numChan = EvCh_FindEventChannel(allEventChannels_, ftdindex_,  node_);
	if (numChan != EV_CH_FAIL_RESULT)
	{
		EVENT_CHANNEL_CIRC_BUF *currEventChannel = &allEventChannels_->eventChannelCircBuf[numChan];

		*size_ = sizeof(long long) + EvCh_GetQuanEventChannels(allEventChannels_, numChan)*sizeof(float) + sizeof(short);

		long long startID  = (*currEventChannel).eventId[0];
		long long finishID = (*currEventChannel).eventId[MAX_SIZE_CIRC_BUF_FOR_EVENT_CHANNEL - 1];
		long      offsetID = 0;

		if ( (*currEventChannel).posWrite == 0 )
		{
			if ( (eventId_ <= finishID) && (eventId_ >= startID) )
			{
				offsetID = long(eventId_ - startID);
				memcpy_s(buffer_, *size_, (*currEventChannel).buffEvent[offsetID], *size_);
				err = (*currEventChannel).eventId[offsetID];
			}
		}
		else
		{
			long long writePosID_left  = (*currEventChannel).eventId[ (*currEventChannel).posWrite - 1 ];
			long long writePosID_right = (*currEventChannel).eventId[ (*currEventChannel).posWrite ];

			if ( (eventId_ >= writePosID_right) && (eventId_ <= finishID) )
			{
				offsetID = long(eventId_ - writePosID_right);
				memcpy_s(buffer_, *size_, (*currEventChannel).buffEvent[ (*currEventChannel).posWrite + offsetID - 1 ], *size_);
				err = (*currEventChannel).eventId[ (*currEventChannel).posWrite + offsetID - 1 ];
			}

			if ( (eventId_ >= startID) && (eventId_ <= writePosID_left) )
			{
				offsetID = long(eventId_ - startID);
				memcpy_s(buffer_, *size_, (*currEventChannel).buffEvent[ offsetID ], *size_);
				err = (*currEventChannel).eventId[ offsetID ];
			}
		}

	}

	return err;
}

SmartArray<double> g_pDiff;

long long EvCh_GetIDTime(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_, long eventTimeSec_, double eventTimeMkSec_)
{
	long long err = -1;

	long numChan = EvCh_FindEventChannel(allEventChannels_, ftdindex_,  node_);
	if (numChan != EV_CH_FAIL_RESULT)
	{
		EVENT_CHANNEL_CIRC_BUF *currEventChannel = &allEventChannels_->eventChannelCircBuf[numChan];
		if (g_pDiff.ReSize(MAX_SIZE_CIRC_BUF_FOR_EVENT_CHANNEL))
		{
			for (unsigned int currEvent = 0; currEvent < MAX_SIZE_CIRC_BUF_FOR_EVENT_CHANNEL; currEvent++)
				g_pDiff.pData[currEvent] = abs((*currEventChannel).eventTimeSec[currEvent] - eventTimeSec_) + abs((*currEventChannel).eventTimeMkSec[currEvent] - eventTimeMkSec_);

			double *minDiffEventTime = std::min_element(g_pDiff.pData, g_pDiff.pData + MAX_SIZE_CIRC_BUF_FOR_EVENT_CHANNEL);
			err = (*currEventChannel).eventId[minDiffEventTime - g_pDiff.pData];
		}

		//double diff[MAX_SIZE_CIRC_BUF_FOR_EVENT_CHANNEL];
		//for (unsigned int currEvent = 0; currEvent < MAX_SIZE_CIRC_BUF_FOR_EVENT_CHANNEL; currEvent ++)
		//	diff[currEvent] = abs((*currEventChannel).eventTimeSec[currEvent] - eventTimeSec_) + abs((*currEventChannel).eventTimeMkSec[currEvent] - eventTimeMkSec_);

		//double *minDiffEventTime = std::min_element(diff, diff + MAX_SIZE_CIRC_BUF_FOR_EVENT_CHANNEL);
		//err = (*currEventChannel).eventId[minDiffEventTime - diff];
	}

	return err;
}

long long EvCh_GetFirstEventID(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_)
{
	long long err = -1;

	long numChan = EvCh_FindEventChannel(allEventChannels_, ftdindex_,  node_);
	if (numChan != EV_CH_FAIL_RESULT)
	{
		EVENT_CHANNEL_CIRC_BUF *currEventChannel = &allEventChannels_->eventChannelCircBuf[numChan];
		
		if ((*currEventChannel).eventId[ (*currEventChannel).posWrite ] == 0)
			err = (*currEventChannel).eventId[0];
		else
			err = (*currEventChannel).eventId[ (*currEventChannel).posWrite ];
	}

	return err;
}

long long EvCh_GetLastEventID(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, long ftdindex_, long node_)
{
	long long err = -1;

	long numChan = EvCh_FindEventChannel(allEventChannels_, ftdindex_,  node_);
	if (numChan != EV_CH_FAIL_RESULT)
	{
		EVENT_CHANNEL_CIRC_BUF *currEventChannel = &allEventChannels_->eventChannelCircBuf[numChan];
		err = (*currEventChannel).nextEventId - 1;
	}

	return err;
}

long EvCh_GetError(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, unsigned long num_)
{
	EVENT_CHANNEL_CIRC_BUF *currEventChannel = &allEventChannels_->eventChannelCircBuf[num_];

	return (*currEventChannel).err;
}

long EvCh_GetNumFTDIndex(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, unsigned long num_)
{
	EVENT_CHANNEL_CIRC_BUF *currEventChannel = &allEventChannels_->eventChannelCircBuf[num_];

	return (*currEventChannel).ftdindex;
}

long EvCh_GetNode(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, unsigned long num_)
{
	EVENT_CHANNEL_CIRC_BUF *currEventChannel = &allEventChannels_->eventChannelCircBuf[num_];

	return (*currEventChannel).node;
}

long EvCh_GetQuanEventChannels(ALL_EVENT_CHANNELS_CIRC_BUF* allEventChannels_, unsigned long num_)
{
	EVENT_CHANNEL_CIRC_BUF *currEventChannel = &allEventChannels_->eventChannelCircBuf[num_];

	return (*currEventChannel).quanEventChannels;
}