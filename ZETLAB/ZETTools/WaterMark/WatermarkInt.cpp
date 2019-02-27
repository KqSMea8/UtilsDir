/*=============================================================================
	Project: 		Watermark
	Author:			��������� �.
	Version: 		1.0
	Description:	������ ������ � Watermark - �������
	Documetation:	https://docs.google.com/document/d/1XWoFwWzcsTCq6BR-DrH720ivCJkCCg8CMAcBXhPnInA/edit
======= Update History =========================================================
   Date			Author		Rev		Reason Description 
07.06.2012		���������	1.0		��������
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
// ������� ������������� ��������
void WMI_InitDecoder(void)
{

}

//---------------------------------------------------------------------------
// ������� ���������� ������� (true) ��� ���������� (false) ������������� �� ������� �������
// sample - ������
bool WMI_GetSampleSynch(long sample)
{
	if (sample & SYNCH_MASK)
		return true;
	else
		return false;
}

//---------------------------------------------------------------------------
// ������� ���������� ������� (true) ��� ���������� (false) ������� ������� (PPS) �� ������� �������
// sample - ������
bool WMI_GetSamplePPS(long sample)
{
	if (sample & PPS_MASK)
		return true;
	else
		return false;
}

//---------------------------------------------------------------------------
// ������� ���������� ������� (true) ��� ���������� (false) ����������� ������ �� ������� �������
// sample - ������
bool WMI_GetSampleCriticalError(long sample)
{
	if (sample & CRITICAL_ERROR_MASK)
		return true;
	else
		return false;
}

//---------------------------------------------------------------------------
// ������� ���� � ������ ��������� ������ � �������� �������������
// ���������� ������� (true) ��� ���������� (false) ������������� � ������
// *data - ��������� �� ����� ��������
// size - ���������� �������� � ������
// *index - ��������� �� ����������, � ������� ������� ����� ����� ���������� ������� � ������ 
//			� ������� ������ ������� ������� �������������
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
// ������� ���� � ������ ������ ������ � �������� ������� �������
// ���������� ������� (true) ��� ���������� (false) ������� ������� (PPS) � ������
// *data - ��������� �� ����� ��������
// size - ���������� �������� � ������
// *index - ��������� �� ����������, � ������� ������� ����� ����� ������� ������� 
//			� ������ � ������� ������ ������� ������� �������
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
// ������� ���� � ������ ������ ������ � �������� ����������� ������
// ���������� ������� (true) ��� ���������� (false) ����������� ������ � ������
// *data - ��������� �� ����� ��������
// size - ���������� �������� � ������
// *index - ��������� �� ����������, � ������� ������� ����� ����� ������� ������� 
//			� ������ � ������� ������ ������� ����������� ������

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
// ������� ���� � ������ ��������� ������ � �������� ������� �������
// ���������� ������� (true) ��� ���������� (false) ������� ������� (PPS) � ������
// *data - ��������� �� ����� ��������
// size - ���������� �������� � ������
// *index - ��������� �� ����������, � ������� ������� ����� ����� ���������� ������� 
//			� ������ � ������� ������ ������� ������� �������
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
// ������� ���� � ������ ������ ��������� ����� (��������� �������)
// ���������� ����� �������, ������� � �������� ������� ��������� �����, � ��������� ������ ���������� -1.
// *data - ��������� �� ����� ��������
// size - ���������� �������� � ������
// *rtc_time - ��������� �� ����� UTC ��� ������������ �����
// *diff_pps - ��������� �� �������� ����� ���������� � ������� PPS, �������� 24 ���� � ��������� � 100��.
// *synch_lev - ��������� �� �������� �������������
// *pow_lev - ��������� �� �������� �������
// *dat_lev - ��������� �� �������� ��������� ������
int WMI_GetFirstTimeLabel( long *data, unsigned int size, long *rtc_time, long *diff_pps, unsigned char *synch_lev, unsigned char *pow_lev, unsigned char *dat_lev, unsigned char *reserv_lev )
{
	char buf[MESS_SZ] = { 0 };

	// ������� ����� ��������� � ����������� CRC8
	for (unsigned int i = 0; i < size; ++i)
	{
		// �� ����� ���������
		for (unsigned char j = 0; (j < MESS_SZ * 2) && ((i + j + 1) < size); j += 2)
		{
			buf[j / 2] = (char)(*(data + i + j) & 0x0000000F | ((*(data + i + j + 1) & 0x0000000F) << 4));
		}
		// ���������, ��� ID == TIME_LABEL, ���� ������� PPS � ��������� CRC8
		if (buf[0] == TIME_LABLE_ID && (data[i] & PPS_MASK) &&
			GetCrc8(buf, MESS_SZ - 1, CRC_START_VAL) == buf[MESS_SZ - 1])
		{
			memcpy_s(rtc_time, 4, & buf[1], 4);
			// ��������� ��������� ��� ������ � ����������� �� ����� ����� � ������
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
// ������� ���� � ������ ��������� ��������� ����� (��������� �������)
// ���������� ����� �������, ������� � �������� ������� ��������� �����, � ��������� ������ ���������� -1.
// *data - ��������� �� ����� ��������
// size - ���������� �������� � ������
// *rtc_time - ��������� �� ����� UTC ��� ������������ �����
// *diff_pps - ��������� �� �������� ����� ���������� � ������� PPS, �������� 24 ���� � ��������� � 100��.
// *synch_lev - ��������� �� �������� �������������
// *pow_lev - ��������� �� �������� �������
// *dat_lev - ��������� �� �������� ��������� ������
int WMI_GetLastTimeLabel( long *data, unsigned int size, long *rtc_time, long *diff_pps, unsigned char *synch_lev, unsigned char *pow_lev, unsigned char *dat_lev, unsigned char *reserv_lev )
{
	char buf[MESS_SZ] = { 0 };
	int iRetIndex(-1);

	// ������� ����� ��������� � ����������� CRC8
	for (unsigned int i = 0; i < size; ++i)
	{
		// �� ����� ���������
		for (unsigned char j = 0; (j < MESS_SZ * 2) && ((i + j + 1) < size); j += 2)
		{
			buf[j / 2] = (char)(*(data + i + j) & 0x0000000F | ((*(data + i + j + 1) & 0x0000000F) << 4));
		}
		// ���������, ��� ID == TIME_LABEL, ���� ������� PPS � ��������� CRC8
		if (buf[0] == TIME_LABLE_ID && (data[i] & PPS_MASK) &&
			GetCrc8(buf, MESS_SZ - 1, CRC_START_VAL) == buf[MESS_SZ - 1])
		{
			memcpy_s(rtc_time, 4, & buf[1], 4);
			// ��������� ��������� ��� ������ � ����������� �� ����� ����� � ������
			*diff_pps = (buf[7] & 0x80) ? -1 : 0;
			memcpy_s(diff_pps, 3, & buf[5], 3);
			*synch_lev = buf[8] & 0x03;
			*pow_lev = (buf[8] >> 2) & 0x03;
			*dat_lev = (buf[8] >> 4) & 0x03;
			*reserv_lev = (buf[8] >> 6) & 0x03;
			iRetIndex = i;
			// ���������� ��������� ���������
			i += (MESS_SZ * 2 - 1);
		}
	}
	
	return iRetIndex;
}

//---------------------------------------------------------------------------
// ������� ���� � ������ ������ ��������� ��������� (��������� �������)
// ���������� true, ���� ��� �������, � ��������� ������ ���������� false
// *data - ��������� �� ����� ��������
// size - ���������� �������� � ������
// *index - ��������� �� ����������, � ������� ������� ���������� ����� ������� � ������, 
//			�� ������� �������� ������ ���������� ���������
// *ID - ��������� �� ����������, � ������� ������� ���������� ID ���������� ���������
// *struct_vers - ��������� �� ����������, � ������� ������� ���������� ������ ��������� ������
// *struct_sz - ��������� �� ����������, � ������� ������� ���������� ������ ��������� ������
bool WMI_GetStartMessage(long *data, unsigned int size, unsigned int *index, unsigned char *ID, 
						unsigned char *struct_vers, unsigned short *struct_sz)
{
	char buf[MESS_SZ];
	char tempID;

	// ������� ����� ��������� � ����������� CRC8
	for (unsigned int i = 0; i < size; i++)
	{
		// �� ����� ���������
		for (unsigned char j = 0; (j < MESS_SZ * 2) && ((i + j + 1) < size); j += 2)
		{
			buf[j / 2] = (char)((*(data + i + j) & 0x0000000F) | ((*(data + i + j + 1) & 0x0000000F) << 4));
		}
		if (GetCrc8(buf, MESS_SZ - 1, CRC_START_VAL) == buf[MESS_SZ - 1])
		{
			tempID = buf[0];
			// ���� ��� ID ���������� ���������
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
// ������� ��������� ����������� � ���������� ID ���������
// ���������� true, ���� ����������� ����� ���������, ����� - false
// *data - ��������� �� ����� ��������, ������ �������� ������ ���� �� ������ ���������
// *ID - ��������� �� ����������, � ������� ������� ���������� ID ���������
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
// ������� ���������� ������ ��������� � ��������� �����
// ���������� true � ������ �������� ������ ���������, false - � ������ ������
// *data - ��������� �� ����� ��������, ������ �������� ������ ���� �� ������� � ������� index, 
//			������������ �������� WM_GetStartMessage()
// size - ���������� �������� � ������, ������ ���� �� ����� SAMPLE_QNT (��. ����������), 
//			����� ������ ��� ������ ��������� (���������, �������� � ��������������)
// *mess - ��������� �� ������ ������ ���������
bool WMI_GetMessageData(long *data, unsigned int size, void *mess)
{
	char *ptr = (char *)mess;
	char buf[DATA_SZ];
	unsigned int Idx_val;
	unsigned char ID_val, temp_ID_val;
	unsigned char Struct_vers_val;
	unsigned short Struct_size_val;
	unsigned int i = 0, j = 0;

	// ���� ��������� ��������� ������
	if (WMI_GetStartMessage(data, MESS_SZ * 2, &Idx_val, &ID_val, &Struct_vers_val, &Struct_size_val))
	{
		// ���� ��������� �������������� ���������
		for (i = MESS_SZ * 2; i < size; i += MESS_SZ * 2)
		{
			if (WMI_CheckMessage((data + i), &temp_ID_val, buf))
			{
				// ���� �������������� ��������� ����������� ���������� ���������� ���������
				if (temp_ID_val == (ID_val + 1))
				{
					// ���� ��������� �������������� ���������
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

	// ���� ��������� ��������� ��������
	if (WMI_CheckMessage((data + i + MESS_SZ * 2), &temp_ID_val, buf))
	{
		if (temp_ID_val == (ID_val + 2))
			return true;
	}

	return false;
}

//---------------------------------------------------------------------------
// ������� ������������� � ���������� SAMPLE_QNT (��. �������� WM_GetMessageData())
// struct_sz - ������ ��������� ������ �� WM_GetStartMessage()
// freq - ������� ������������� �������
// channel_num - ���������� �������
unsigned long WMI_GetMessageDataSize(unsigned short struct_sz, unsigned long freq, unsigned short channel_num)
{
	unsigned long clear_sz;

	// ������ ��� ����� ��������� �����
	if (struct_sz % DATA_SZ)
		clear_sz = MESS_SZ * 2 * (struct_sz / DATA_SZ + 1 + 2);
	else
		clear_sz = MESS_SZ * 2 * (struct_sz / DATA_SZ + 2);

	// � ������ ��������� �����, ������� ����� ���� �������� �� ���� ���������
	return clear_sz + MESS_SZ * 2 * (clear_sz / (freq * channel_num) + 1);
}

//---------------------------------------------------------------------------
