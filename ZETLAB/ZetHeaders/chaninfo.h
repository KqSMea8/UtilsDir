// chaninfo.h
//
#ifndef __chaninfo_h__
#define __chaninfo_h__

#include "ADCINFO.h"
#include <sys/timeb.h>
#include <stdint.h>
#include "ZetTools\include\ZetTools.h" 

#define MAXUNITS			200
#define U_INVITE			-1
#define U_USED				1
#define U_VRTCHN			2
#define MAXDSP				137
#define MAXMODULE			50
#define MAXCHAN				400
#define MAXDECADE			5
#define MAXDACS				100
#define MODIFY				1
#define FILEMODE			2
#define STARTFILE			3
#define SETTIME				4
#define GPSSIZE				400
#define MAXCHANMOD			32
#define MODULEADCDAC		500
#define DIGEVENTSQUELEN     10

typedef enum _CHANNEL_STATUS
{
	csADC = 0,
	csDAC,
	csVirtual,
	csDigital,
	csLost,
	csLostDevice,
	csSmartSensor,
	csLostSmartSensor,
	csFastSmartSensor,
	csInactiveVirtual,
	csHiddenADC,
	csHiddenDAC
} CHANNEL_STATUS;

typedef enum _COMMAND_ID
{
	cidSetDigOutEnable = 0,
	cidGetDigOutEnable,
	cidGetDigInput,
	cidGetDigOutput,
	cidSetDigOutput,
	cidGetDigBits,
	cidSetBitDigOutput,
	cidClrBitDigOutput,
	cidGetFreqADC,
	cidGetFreqDAC,
	cidSetFreqADC,
	cidSetFreqDAC,
	cidGetNextFreqADC,
	cidGetNextFreqDAC,
	cidOnOffChannel,
	cidSinDiffChannel,
	cidEnableBuiltinGenerator,
	cid1PPSBuiltinGenerator,
#pragma region DAC MUX
	cidDACMUXMode,
	cidDACMUXSetWait,
	cidDACMUXResetWait,
#pragma endregion DAC MUX
} COMMAND_ID;

typedef struct _SRV_COMMAND_INFO
{
	union
	{
		struct
		{
			long lParametr1;
			long lParametr2;
		} Parameters_long_long;

		struct
		{
			long lParametr1;
			float fParametr2;
		} Parameters_long_float;

		struct
		{
			float fParametr1;
			float fParametr2;
		} Parameters_float_float;

		long lParameter_long;
		float fParameter_float;
	};

	union
	{
		unsigned long ulResult;
		long lResult;
		float fResult;
	};

	long lError;
	long lFlag;
	long lUse;
	long lCommandID;
	long lTypeDsp;
	long lNumberDsp;
	long lNeedRewriteDeviceCfg;		// ���� ������������� ������������ device.cfg
	long lNeedRestart;				// ���� ������������ ������������� ���
} SRV_COMMAND_INFO, *pSRV_COMMAND_INFO;
////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _INTER_PROC
{	// ��������������� �����
	int			Used;					// ������������ ������
	int			Dac;					// 1 - ��� �����������, 0 - ��� ��������� (������������ ��� ��������� ������)
	HWND		hUnitWin;				// ������� ���� ������ unit � ����������
	int			EndFile;				// ������ ����� ��������� - 1
	wchar_t		UnitFileName[200];		// ��� �������� � ".exe"
	int			FileNameWriten;			// ��������� ������������� � �������, 1 - ������ �� ����������, 0 - �����
	short*		DacBuffer[MAXDACS];		// ������ ������� ��� � ������ ��������
	int			TypeSrvDac[MAXDACS];	// ��� ���������� ��������, ������������ ���� ���������
	int			NumSrvDac[MAXDACS];		// � ����� ����������� ����������
	long		Guid[MAXDACS];			// ������������� ������ ���
	int			TalkToBorland;			// ������� �� ������� ������
	int			countlive;				// ������� ��������� � ������� ��� ����������� �������
	int			prevcountlive;
	HANDLE		hMFile;					// ��������
	LPVOID		lpBaseAddress;			// � ��������� �� ������� ������, ��� ��������� ������� ������
	int			NumSourse;
	SRV_COMMAND_INFO srvCommand;
 
 } INTER_PROC, *pINTER_PROC;
////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _CHANNEL_EVENT
{
	GUID guid;          // ���������� ������������� ������, �� �������� ��������� ������� ��������� ���������
	long sec;           // ��������� ������������ �������
	long nanosec;       // ������������� ������������ �������
	long type;          // ��� ������� (6 - ��������, 7 - ������� ������, 8 - ������� �����)
	char reserve[200];  // ����� ��� �������� �������������� ���������� �� �������, � ����������� �� ���� �������
} CHANNEL_EVENT, *pCHANNEL_EVENT;
////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _CHANNEL_EVENTS
{
	long currPos;   // ������ ��� ������ ������ ������� � �������
	CHANNEL_EVENT events[DIGEVENTSQUELEN];   // ������� ������� ��������� ��������� ��������� ������
} CHANNEL_EVENTS, *pCHANNEL_EVENTS;
////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _ALL_CHAN_INFO {				// ���������� ���������� �������
	int			IndexTable;						// ������ (������) ������� TABSIG.CFG
	int			NumModule;						// ����� ������
	int			NumContact;						// ����� �������� � ������
	int			TypeChannel;					// 0 - ���, 1 - ���, 2 - ����������� �����, 3-�������� �����, 4-�������� �����, 5 - ������� ����������, 6 - ���������������� (�� MODBUSZETLAB), 7 - �������� ����������� � MODBUSZETLAB 8-������� �����
	double		FreqVirt;						// ������� ������������ ������
	int			UpDownInt;						// ����������� ������������ ���-��� ����� (0-���� ���������)
	double		UpDownFlt;						// ����������� ������������ ���-��� ���������
	double		PhasaDac;						// ���� ����� �������� ��� �������������
	int			PhasaDownDac;					// ���� ����� �������� ��� �������������
	double		CurLevel;						// ������� �������
	double		MaxLevel;						// ������������ ������
	float		Subtraction;					// �� ��� ���� �������
	float		Multiplier;						// �� �� ��� ���� ��������
	GUID		IdChannel;						// ������������� ������
	int			WatermarkFlag;					//
	double		hpf[6];
	double		hpfHistory[4];
} ALL_CHAN_INFO, *pALL_CHAN_INFO;
//////////////////////////////////////////////////////////////////////////////////////////////
//num    sense   convert ampl  prus  refer   afch    dc  comment
typedef struct _TABSIG_INFO {					//������ �� ����� DEVICES.CFG
	double		Sense;				//����������������
	wchar_t		Convert[200];		//������� ���������
	double		PreAmpl;			//����������� ���������������� ��������
	int			PrUs;				//������� ��������� 0-���, 1-��8/10, 2-HCP
	double		Refer;				//����� ��� ������� ��
	wchar_t		Afch[MAX_PATH];		//���� �������� ���
	double		DC;					//�������� ���������� ������������
	wchar_t		Comment[400];		//�������� ������
	GUID		IdChanTab;			//������������� ������
	uint16_t	hpf;				//��� 0.1 ��
} TABSIG_INFO, *pTABSIG_INFO;
////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _CHAN_DAC_INFO
{	// ��������� �� ������� ������� ��� �� 0 �� ���������� ������� ���-1
	int			EnableChanDac;			// ���������� ������ �� ��������� ������ ���
	int			OldEnableChanDac;		// ������ ��������� ��� ���������� � ��������� DACSTOP
	int			Otkl;					// ��� ��������
	int			ModDac;					// ������ ��� �� ������� ���������� ������ �����
	int			OldModDac;				// ������ ��� �� ������� ��� ������ ������
	int			NumChanDacGl;			// ����� ����������� ������ ������ ���, ���������� ����� ���������� ������ ������ ���
	int			NumChanDacLoc;			// ����� ������������ ������ ���, ���������� � 0 (�����, ���� ������ ��� ���) �� ���-�� ������� � �������. ���� ����� ���� � ��� �� ��� ���� ��������
	int			NumChanDacReal;			// ����� � ������� ���������(�� hidden)
	int			ChannelWithFreedback;	// ����� � �������� ������(0 - ������� ����� | 1 - ����� � ��)
	int			OldBuiltinGenEnabled;	// ��������� ����������� ���������� �� ��������� �������� ����������
	GUID		IdChanDac;				// ������������� ������
	float		AttenuationVal;			// �������� �����������(����������� ��� ������ �������� ������)
	HWND		UnitHwnd;				// ���� ���������, ������� ���������� ���� ���������
} CHAN_DAC_INFO, *pCHAN_DAC_INFO;
///////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _CHAN_VIRT_INFO
{	// ���������� �� ������������ ������
	HWND		UnitHwnd;			// ���� ������ unit � ����������
	int			NumVirtLoc;			// ����� ������������ ������ � ��������� ����������, ���������� ���� ����������� ����� (������ ���������� � 0)
	int			NumChanGl;			// ����� ������ � ������� ���� �������
	wchar_t		Convert[200];		// ������� ���������
	wchar_t		Comment[400];		// �����������
	float		Frequency;			// ������� �������������
	float		Refer;				// ����� ��� ������� ��
	float		MaxLevel;			// ������������ �������
	float		MinLevel;			// ����������� �������
	GUID		IdChanVirt;			// GUID ������
	wchar_t		GroupNm[200];		// ��������� ��� ������
	int			TypeChannel;		// ��� ������������ ������
	int			NumSourse;			// ����� ����������, ���������� ����� ����� ��������
}CHAN_VIRT_INFO, *pCHAN_VIRT_INFO;
///////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _SIGNAL_ANP	
{	//������ �� ����� SIGXXX.ANP � .ANA
	int			Enable;				// ���������� ������ �����
	double		IntervalSec;		// ����� ����� � ��������
	FILE		*Input;				// ������� �����
	float		Gain;				// ����������� ��������
	float		AbsVolt;			// ���������� ������
	float		Frequency;			// ������� �������������
	float		Sense;				// ����������������
	float		Ampl;				// ����������� ��������
	float		Refer;				// ����� ��� ������� ��
	float		DC;					// �������� ���������� ������������
	float		MaxUr;				// ������������ ������� �������
	wchar_t		Conversion[200];	// ������� ���������	
	char		Afch[200];			// ���� �������� ���
	int			SigWav;				// ������ ����� 0-ana, 1-WAV
	int			Format;				// ������ �������� ������ 0 - 16 ��������� �����, 1 - 32-���������, 2 - float 32 - ���
	struct __timeb64	StartTime;	//��������� ����-�����
	long		TypeADC;
	long		Serial;
	wchar_t		GroupNm[200];		// ��������� ��� ������
}SIGNAL_ANP, *pSIGNAL_ANP;
//////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _MEM_INFO
{	// ���������� �� ������������� ������ ���� ������������ �������
	int				NullPointMem[MAXDECADE];		//��������� ������ ������ � ����-����
	int				CurPointMem[MAXDECADE];			//������� ��������� ���������� � ����-����
	int				SizeBufMem[MAXDECADE];			//������ ��� ������� ������ � ����-���� ��� ������� ������ � ������
	long long		CountSample;					//���������� ����������� ����� � ����-���� �� ������� ���
	long long		NullCountSample;				//���������� ����������� ����� ��� ������� ���
	long long		OldPointer;						//������ ��������� ������ ������ � ����������� �������, � ����������� ������ �������� �� ����� (not a number NAN), ������ ���� 0
	long long		DeltaStartTime;					//�������� �� ������ ��� ���
	int				OldPointMem;					//������ ��������� �� ����� ��� ��� � ���, � ����������� �������
	long			nak[MAXDECADE];
	long			tek[MAXDECADE];
	int				PortionDataVirt;				// ������ ������ � ����������� �����
	int				PrisnakDataVirt;				// ������� ������ � ����������� ������
	CHANNEL_EVENTS  Events;							// ������� ��������� ��������� �� ��������� ������
	GUID			IdChanMem;						// ������������� ������
	double			DeltaInTime;					// ������� ����� ������� �������� � �������� ��������(��� ������ DAC MUX ����� ����� �������)
} MEM_INFO, *pMEM_INFO;
//////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _WATERMARKFLOAT_INFO{	//���������� � Watermark
	unsigned long lTimeLabel;			//�������� ��������� �����
	unsigned char ucQuality;			//�������� ������
	long lPart;							//������� ��� Watermark ��� �������� � ������
	long lPoint[MAXCHANMOD];			//������ � �������� �������� � �������� ����� ������� Watermark ��� ������� ������
} WATERMARKFLOAT_INFO, *pWATERMARKFLOAT_INFO;
//////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _MODULE_INFO {			// ������ ���������� �� ������ (����������� ����������)
	ADC_INFO_EXT DSPInfo;				//���������� � ������� ���
	int			TypeDSP;				//��� �������� ���
	int			NumberDSP;				//����� ����������� ����������
	long		SerialNumber;			//��������� ����� ������
	char		DeviceName[200];		//��� ����������
	int			CurModify;				//������ ����������� ��������
	short		*AdressUserBuffer;		//����� ���������������� ������� ������ ��������
	long		ulWatermarkPointer;	
	bool		bNeedToFindTimeLabel;
	WATERMARKFLOAT_INFO wmInfo;
	bool		bCriticalError;
	int			FirstChannelIndex;		// ������ ������� ������ ����������
	// ��� �������� �� �������� � Device.cfg

} MODULE_INFO, *pMODULE_INFO;
////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _CORRECT_ADC_INFO {				//������ �� ����� ADC.DAT
	double		AmplCorr;						//����������� ��������� ������ ���
	double		DcCorr;							//�������� ���������� ������������ ���
} CORRECT_ADC_INFO, *pCORRECT_ADC_INFO;
///////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _CORRECT_PRUS_INFO {				//������ �� ����� PRUS.DAT
	double		PrusCorr;						//�������� � ��������� ������
} CORRECT_PRUS_INFO, *pCORRECT_PRUS_INFO;
/////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _GPS {
	float			shift;					//����� ���������� ������� � UTC
	char			buffer[GPSSIZE];			//����� NMEA ������
	int				repointer;					//��������� �� ������ �� ������ NMEA
	int				wrpointer;					//��������� �� ������ � ����� NMEA
	struct _timeb	starttime;					//����� ���������� ���
	struct _timeb	promstart;					//������������� ����� ���
	FILE			*file;						//GPS LOG ����
	long			filepos;					//������� �� �����
	long			filelen;					//����� GPS ��� �����
	int				satinfo;					//���������� � ���������� ���������
	int				advance;					//���������� ��� ������� �� GPS
	int				enaGPSync;					//���������� ������������� �� GPS
	ULONGLONG	UTCstartime;				// ������ �� �������� ������� UTC � �������� �� 1970 �
} GPS, *pGPS;
/////////////////////////////////////////////////////////////////////
typedef struct _ALL_INFO {				// ���������� ��� ����
	int				IsServer;					// ������� ������� - 1, ��� - 0
	int				Demo;						// ���� ����� 1, �������� ����� - 0
	int				ReadFile;					// ����� ������ �����
	int				StartStop;					// ����� ���� ������ ������
	int				Pause;						// ����� � ������ ������
	int				RealTime;					// 0-������ ������ - 1 - ��������
	int				peretasovka;				// 0 - ������� ������, 1 - ���������� ��������� ������
	wchar_t			PathSignal[400];			// ������ ���� � ��������
	double			StartPosition;				// ��������� �������
	double			CurrentPosition;			// ������� �������
	int				BlockDataAccess;			// ���������� ������� ������ �� ����� ������
	HWND			hWndSRV;					// ������� ���� ������ �������

	int				QuanModuleAdc;				// ���������� ������� ���
	MODULE_INFO		ModuleAdc[MAXMODULE];		// ������ �������� ������ ���
	int				QuanModuleDac;				// ���������� ������� ���
	MODULE_INFO		ModuleDac[MAXMODULE];		// ������ �������� ������ ���

	int				QuanChan;					//���������� ���������� ������� ���� ���-���-�����������
	int				QuanChanAdc;				//���������� ���������� ������� ���
	int				QuanChanDac;				//���������� ���������� ������� ���
	int				QuanChanVrt;				//���������� ���������� ������� �����������
	int				QuanChanFromFile;			//���������� ������� �� �����

	TABSIG_INFO			TabSig[MAXCHAN+MAXCHAN+MAXCHAN];	//������ �� ����� TABSIG.CFG
	CORRECT_ADC_INFO	CorrAdc[MAXCHAN+MAXCHAN];			//������ �� ����� ADC.DAT
	CORRECT_PRUS_INFO	CorrPrus[MAXCHAN];					//������ �� ����� PRUS.DAT

	ALL_CHAN_INFO		AllChan[MAXCHAN+MAXCHAN];		//���������� ���������� �������
	MEM_INFO			MemChan[MAXCHAN+MAXCHAN];		//���������� � ������������� ������ � ���-�����
	int					DacEnable;						//������� ���
	int					DacModify;						//��������� ���������� ���
	int					MaxQuanChanDac;					//������������ ���������� ������� ��� � �������
	CHAN_DAC_INFO		DacChan[MAXDACS];

	INTER_PROC		unit[MAXUNITS];				//��������������� �����
	int				FileUsed[MAXCHAN];			//������������ ����� � �������� ������ �� ������ 1- ���� ������
	int				FileFirstInUse[MAXCHAN];	//���� �������� �������, ���������� �������� ��������� 0 - ��������
	SIGNAL_ANP		SigAnp[MAXCHAN];			//��������� ������ SIGxxx.ANP
	double			Resource;					// ������������ �������� ������ (�������� ����������)
	CHAN_VIRT_INFO	VrtChn[MAXCHAN];
	int				changevirt;					// ������� ��������� ���������� ����������� �������, 1 - ������������ �������, 2 - ��������������

	long			EnaPWM[MAXMODULE];			//������� PWM ���������� ���-��������� � SigmaUsb
	long			RatePWM[MAXMODULE];			//����������� ������� ������� 96 ���
	long			PeriodPWM[MAXMODULE];		//������ ��� ���������
	long			OnDutyPWM0[MAXMODULE];		//���������� 0-�� ������
	long			OnDutyPWM1[MAXMODULE];
	long			OnDutyPWM2[MAXMODULE];
	long			ShiftPWM1[MAXMODULE];		//����� �� ���� 1-�� ������������� 0-��
	long			ShiftPWM2[MAXMODULE];
	long			StartPWM0[MAXMODULE];		//�������� ��������� = 1, ��� = 0.
	long			StartPWM1[MAXMODULE];
	long			StartPWM2[MAXMODULE];
	GPS				GPSData;
	long			GetUnit;					//���������� ������ ������ unit �� ����� ������� �������
	long			Stop_ADC_DAC;				//�����-���� ��� 0-���� �������, 1-���������������.
} ALL_INFO, *pALL_INFO;

typedef struct _DELTA_INFO 						
{
	char		conv[16];						// ������� ���������
	float		sense;							// ���������������� ������� �/��.���.
	float		refer;							// ������� �������� ��� ���������� �������� � ��
} DELTA_INFO, *pDELTA_INFO;

typedef struct _PULSE_INFO{
	double timestart;
	long addrstart;
	long size;
} PULSE_INFO, *pPULSE_INFO;

// ������������ ��������� ������� ������ �������
typedef enum _DAC_MUX_MODE
{
	DAC_MUX_MODE_SUV = 0,
	DAC_MUX_MODE_LAST
};
// ������ � �������� ������ ��� ������ � ��������
static unsigned char g_DAC_MUX_MODES[] = { 0x35 };

// �����, ������������ � ����� ���������� � ������ DAC MUX
typedef struct _DAC_MUX_FRAME
{
	unsigned char sof;					// ����� ������
	unsigned char fcs;					// ����� ���� ���� � 2�� �� 15�
	short code_out[2];					// �������������� ����� 2 �������
	short code_sin;						// ����� ��� �������
	short code_cos;						// ������� ��� �������
	unsigned char reserved;				// 
	unsigned char param_switch;			// ����� ���������
	union {								// �������� ���������
		long u32;
// 		short u16[2];
// 		unsigned char u8[4];
	} param;
	_DAC_MUX_FRAME()
	{
		ZeroMemory(this, sizeof(_DAC_MUX_FRAME));
		CalcFcs();
	}
	_DAC_MUX_FRAME(_In_ unsigned char Mode)
	{
		ZeroMemory(this, sizeof(_DAC_MUX_FRAME));
		sof = Mode;
		CalcFcs();
	}
	_DAC_MUX_FRAME(const _DAC_MUX_FRAME &Param)
	{
		*this = Param;
	}
	_DAC_MUX_FRAME & operator = (const _DAC_MUX_FRAME &Param)
	{
		memcpy_s(this, sizeof(_DAC_MUX_FRAME), &Param, sizeof(_DAC_MUX_FRAME));
		return *this;
	}
	// ������� ����������� ����� ������
	void CalcFcs()
	{
		long Summ(0);
		this->fcs = 0;
		unsigned char *b = (unsigned char *)this;
		for (int i = 0; i < sizeof(_DAC_MUX_FRAME); i++)
			Summ += b[i];

		this->fcs = (unsigned char)-Summ;
	}
	// �������� ���������� ���������
	bool Isvalid()
	{
		unsigned char *Element = (unsigned char *)this;
		// ��������� ��� ����� ������ ���� �� ��������� 
		bool bValid(false);
		for (auto &Mode : g_DAC_MUX_MODES)
		{
			if (*Element == Mode)
			{
				bValid = true;
				break;
			}
		}

		if (bValid)			
		{
			unsigned char Summ(0);
			// ���� ����� ������ ���������� ��������� ��� ����� ���� ���� ����� 0
			for (int i = 0; i < sizeof(_DAC_MUX_FRAME); i++)
				Summ += Element[i];

			bValid = Summ == 0;
		}

		return bValid;
	}
} _DAC_MUX_FRAME;

//------------------------------------------------------------------------------
#define SIZE_CHANNEL_NAME		200
#define SIZE_CHANNEL_STRUCT		880
//------------------------------------------------------------------------------
// ����� �� �������� ��������� � ��������� ������� ��� Debug � ��� Release
// �������� "�������� ������������->�/�++->�������� ����->
// ������������ ������ ��������" ������ ���� ����� "�� ���������"
//
typedef struct _CHANNEL_STRUCT
{
	const uint32_t size;	// ������ ���������
	long status;			// ������ ������
	GUID id;				// GUID (�������������) ������
	wchar_t name[SIZE_CHANNEL_NAME];	// �������� ������
	wchar_t measure[SIZE_CHANNEL_NAME];	// ������� ���������
	double frequency;		// ������� �������������
	double sense;			// ����������������
	double referense;		// ������� ��������
	double dc;				// �������� ���������� ������������
	double maxLevel;		// ������������ �������
	double minLevel;		// ����������� �������
	double subLevel;		// ����� ����������������

	_CHANNEL_STRUCT()
		: size(sizeof(_CHANNEL_STRUCT))
		, status(-1)
		, frequency(0.)
		, sense(0.)
		, referense(0.)
		, dc(0.)
		, maxLevel(0.)
		, minLevel(0.)
		, subLevel(0.)
		, id(GUID_NULL)
	{
		ZeroMemory(name, sizeof(name));
		ZeroMemory(measure, sizeof(measure));
	}

	~_CHANNEL_STRUCT() {}

	_CHANNEL_STRUCT(const _CHANNEL_STRUCT & cs)
		: size(sizeof(_CHANNEL_STRUCT))
	{
		memcpy_s(this, size, &cs, size);
	}

	_CHANNEL_STRUCT & operator = (const _CHANNEL_STRUCT & cs)
	{
		memcpy_s(this, size, &cs, size);
		return *this;
	}

	bool operator == (const _CHANNEL_STRUCT & cs)
	{
		return (size == cs.size) && (memcmp(this, &cs, size) == 0);
	}

	bool operator != (const _CHANNEL_STRUCT & cs)
	{
		return !(*this == cs);
	}

	long GetChannelID()
	{
		return *(long*)(id.Data4 + 4);
	}

	// ��������� �������� �� ���������� �� ������ ���������������� ��������
	bool Is7xxxChannel()
	{
		return status == 6 || status == 7;
	}

	// �������� �������� ����� ����������
	long long GetChannelSerial()
	{
		long long Serial;

		if (Is7xxxChannel())
		{
			LARGE_INTEGER lt;
			lt.HighPart = id.Data1;
			lt.LowPart = MAKELONG(id.Data2, id.Data3);

			Serial = lt.QuadPart;
		}
		else
			Serial = (GetChannelID() >> 16) & 0xffffffff;
		
		return Serial;
	}

} CHANNEL_STRUCT, *LPCHANNEL_STRUCT;
//------------------------------------------------------------------------------

#endif
