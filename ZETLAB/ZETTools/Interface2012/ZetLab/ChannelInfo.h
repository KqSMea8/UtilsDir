#pragma once
#include "C:\ZETTools\ZETView_basic_class\CDSRV.h"
#include <map>

#define MAX_CHAN_LEVELS	10

#define STARTBIT		1
#define STOPBIT			1
#define SIZEOFQUALITY	2
#define SIZEOFTIMELABEL	32
#define SIZEOFCRC		8
//размер посылки:		| стартовый бит |  метка времени   |       качество данных        |  контрольная сумма  | стоповый бит  
#define SIZEOFWMF		(STARTBIT		+ SIZEOFTIMELABEL +		4 * SIZEOFQUALITY  +			SIZEOFCRC		 + STOPBIT)
#define CRC_START_VAL	(char)0xE3

#define IDS_MY_COMPUTER                 1070 
#define IDS_STRING104                   1071
#define IDS_NO_NAME                     1072
#define IDS_CHANNELS                    1073
#define IDS_CHANNELS_TOOLTIP            1074
#define IDS_TIME                        1075
#define IDS_STRING108                   1076
#define IDS_TIME_TOOLTIP                1077

class CChannelInfo
{
public:
	CChannelInfo(CDSRV* pSRV);
	~CChannelInfo(void);

	//заполнить информацию о каналах
	void FillChannelInfo();
	//запросить максимальный уровень иерархии в системе
	long GetMaxLevels();

	CDSRV* m_pServer;

	//структура для хранения информации о каналах
	struct ChannelInfoStruct
	{
		ChannelInfoStruct()
		{
			sName = L"";
			sGroupName = L"";
			for (int i = 0; i < MAX_CHAN_LEVELS; ++i)
				pLevels[i] = L"";
			lLevels = 0;
			iStatus = 0;
			dTime = 0.;
			dWatermarkPeriod = 0.;
			dFrequency = 0.;
		}

		CString sName;
		CString sGroupName;
		CString pLevels[MAX_CHAN_LEVELS];
		long lLevels;
		CString sConversion;
		int iStatus;
		double dTime;
		double dWatermarkPeriod;
		double dFrequency;
		long idChan;
	};	

	std::map <int, ChannelInfoStruct> m_ChannelInfoMap;

protected:
	//заполнить информацию по измерительному каналу
	void FillChannel(long lNumber);
	//заполнить иерархическую цепочку канала
	long FillChanLevels(CString* pLevels, CString sGroupName);
	//заполнить уровни иерархии
	void FillLevels(long lNumber);
};

