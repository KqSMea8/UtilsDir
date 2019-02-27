//!---------------------------------------------------------------------
//! ����� ������������ ��� �������������� DAT-������ ������� ���������
//! � AMA/ANP-����� � ������� �������.
//! ������� �����. ��� "����" 2016 �.
//!---------------------------------------------------------------------

#pragma once

//#include "OtherFormat.h"

#include "DeviceInfoParcer.h"
#include "WMInfo.h"
#include "WMAligner.h"
//#include <Dialog_ZET/CDZetPath.h>
#include <TrendWriter/SingleTrendWriter.h>

#include <map>

struct datHeaderStruct
{
	ADC_INFO_USB adcInfo;
	CALIBR calibr;
};

struct datPpsErrorsStruct
{
	time_t first_rtc;						//	������ ���������� � �������������� �����
	time_t last_rtc;						//	��������� ���������� � �����
	long bad_pps;							//	���������� ��������� ����������
	long bad_writes;						//	���������� ������ ������
	long conv_size;							//	���������� ����������������� ����
	std::map<long, long> time_interval;		//	���������� ���������� ����� ������������

	datPpsErrorsStruct()
	{
		Clear();
	}

	void Clear()
	{
		first_rtc = 0;
		last_rtc = 0;
		bad_pps = 0;
		bad_writes = 0;
		conv_size = 0;

		if (!time_interval.empty())
			time_interval.clear();
	}
};

struct datQualityStruct
{
	unsigned char sync_qual;	//	�������� �������������
	unsigned char powr_qual;	//	�������� �������
	unsigned char data_qual;	//	�������� ������
	time_t rtc_time;				//	����� �������

	datQualityStruct()
	{
		Clear();
	}

	void Clear()
	{
		sync_qual = 0;
		powr_qual = 0;
		data_qual = 0;
		rtc_time = 0;
	}
};




class CDatConverter
{
public:
	void SetSignalDir(CString signal_dir) { m_sSignalDir = signal_dir.Left(signal_dir.GetLength() - 15); };
	void SetCompresDir(CString compres_dir) { m_sCompresDir = compres_dir; };

	time_t GetStartTime() { return m_tStartTime; };
	time_t GetFinishTime() { return m_tEndTime; };
	CString GetConvertedChannels() { return m_channels; };

    long GetDataSizePerSec();

private:
	CString m_sSignalDir;				//!	�������� ���������� ��� ������ �������� ������
	CString m_sCompresDir;				//!	�������� ���������� ��� ������ ������ ������ ������

	std::vector<WMInfo> m_watermarks;
	std::vector<CSingleTrendWriter> m_writer;
	std::vector<datQualityStruct> m_qual;

	deviceparcer::deviceparcer m_parcer;
	datPpsErrorsStruct m_stat;

	CDriveChecker m_checker;
	CDescriptorFileHelper m_helper;

	std::vector<CALIBR> m_Calibr;
	ADC_INFO_USB m_adcInfo;

	time_t m_tStartTime;
	time_t m_tEndTime;
	CString m_channels;

public:

	void PrepareWriter();
	void PrepareLogWriter();
	void ClearWriter();
	void ClearStatistic();
	void PrintStatistic();
	long GetConvertedSize() { return m_stat.conv_size; };

	void AddConvSize(long delta) { m_stat.conv_size += delta; };
	long ConvertDatBufferToTrend(char *dataBuf, unsigned long rbSize, bool isComing, time_t &fileTime);
	long ConvertLogBufferToTrend(char *dataBuf, unsigned long ldSize, bool bLocal);

	void ReadPPS(long *lngBuf, long sizeBuf, const unsigned int &countCh, const long &frq, const long &corrFactor);
	void MakePPS(long sizeBuf, time_t rtc, unsigned int countCh, long frq, long corrFactor);
	int CalcStartAcpOffset(const unsigned int &countCh);

	CString RtcToString(time_t time);

	bool TestDataQuality(time_t lrtc, unsigned char quality);
	CString GetSynchroQualityMessage(unsigned char quality);
	CString GetPowerQualityMessage(unsigned char quality);
	CString GetDataQualityMessage(unsigned char quality);

	void SetCalibr(char* pBuffer, long size);
	void SetDevices(char* fileName);   // ����������� ������� � SetCalibr, �� ���������� � DEVICES.CFG
	void SetAdcInfo(char* pBuffer, long size);

	bool CheckStandartName(CString chanName);

public:
	CDatConverter();
	~CDatConverter();
};

