// adcinfo.h
#ifndef __adcinfo_h__
#define __adcinfo_h__

// sizeof(ADC_INFO) = 1176 ����
typedef struct _ADC_INFO {
	// ���������� ������������ DSP
	unsigned short	numberDSP;				//���������� ����� DSP �� �����
	unsigned short	typeADC;				//����� ������ DSP: 0 - ADC, 1 - DAC
	unsigned short	EnableADC;				//������� ���
	unsigned short	EnableDAC;				//������� ���
	unsigned short	EnableExtDevice;		//0 - ������ ���, 0x01 - PRUS, 0x02 - HCP, 0x04 - Atten,
											//0x08 - SD, 0x10 - Ethernet, 0x20 - Bluetooth, 0x40 - PWM, 0x80 - DigAnaliz,
											//0x100 - 1-Wire, 0x200 - ���������� �� GPS, 0x400 - ������������� �� PTP
	// ������ DSP
	short			Start;					//��������� ����� ������ 1- ����

	// ������������ �������
	unsigned long	Channel;				//������� ���������� �������� �������
	unsigned long	HCPChannel;				//������� ���������� ���������� ������� HCP
	unsigned long	OverloadChannel;		//������� ���������� ������������� �������
	unsigned short	WorkChADC;				//���������� ���������� ������� ���
	unsigned short	WorkChDAC;				//���������� ���������� ������� ���

	// ������������ ����. ��������
	unsigned short	KodAmplify[32];			//���� �������� �� ���
	unsigned short	TypeConnection;			//��� ���������� (1 - PCI, 2 - USB, 3 - Ethernet, 4 - Bluetouch) (��� �������������!!!)
	double			Amplitude[32];			//����������� �������� ��������� ���
	unsigned short	PRUS[32];				//���� �������� ��������� ������ / ���. (����.) HCP
	double			PreAmplitude[32];		//����������� �������� �������������
	unsigned short	Atten[4];				//���� ����������� ���
	double			Reduction[4];			//����������� �������� ����������� ���

	// ������������ ������
	unsigned short	EnaExtStart;			//���������� �������� ������ ��� 0-�����. 1-����
	unsigned short	EnaExtFreq;				//���������� ������� ������� ��� 0-�����. 1-����
	unsigned short	EnaCycle;				//���������� ������������ ���������� 0-����������, 1-����������
	unsigned short	ExtCycle;				//MasterSynchr: 1 - �������� �������������, 0 - ������� ���������� � �������������
	unsigned short	ModaADC;				//���� ������ ��� 0-1, 1-2, 2-4, 3-5, 4-8, 5-10 ������������

	unsigned short	EnaExtStartDAC;			//���������� �������� ������ ��� 0-�����. 1-���� (��� �������������!!!)
	unsigned short	EnaExtFreqDAC;			//���������� ������� ������� ��� 0-�����. 1-���� (��� �������������!!!)
unsigned short	Reserv1;				//


	double			Freq;					//������� ������� �������������� (Freq = OporFreq/Rate)
	double			OporFreq;				//������� �������
	double			ExtOporFreq;			//������� ������� �������
	unsigned long	Rate;					//����������� ������� ������� �������

	// ������������ �������
	unsigned long	sizeInterrupt;			//������ ��� ��������� ������ ��� ������ ����������
											//(sizeInterrupt = QuantityPacketsADC * SizePacketADC)
	short			*AddressBuffer;			//����� ������ � ��������� �������
	short			*AddressBufferDAC;		//����� ������ � ��������� �������
	short			*AddressUserBuffer;		//������ ������ � ���������������� �������
	short			*AddressUserBufferDAC;	//����� ������ � ���������������� �������
	unsigned long	SizeBuffer;				//������ ������ ����� �� ����� 1 000 000 ��������
	unsigned long	SizeBufferDAC;			//������ ������ ����� �� ����� 1 000 000 ��������
	unsigned long	Pointer;				//������ �������� ���������� � ������ ���
	unsigned long	PointerDAC;				//������ �������� ���������� � ������ ���
	unsigned long	PointerDSP;				//������ ���������� ���������� � DSP

	// ���������� ��������� ��� (���)
	unsigned short	Bits;					//���������� ��� � ��� ���������������
	unsigned short	BitsDAC;				//���������� ��� � ���
	unsigned short	Words;					//���������� 16-��������� ���� � ����� �������
	unsigned short	WordsDAC;				//���������� 16-��������� ���� � ����� �������
	unsigned short	QuantityChannelDigPort;	// ���������� ����� ��������� ����� (��� �������������!!!)
	double			DigitalResolution;		//��� �������� ������� ���
	double			DigitalResolutionDAC;	//��� �������� ������� ���
	double			MaxFreq;				//������������ ������� ��������������
	unsigned short	QuantityChannelADC;		//������������ ���������� ������� � ������ ���
	unsigned short	QuantityChannelDAC;		//������������ ���������� ������� � ������ ���
	unsigned short	QuantityChannelVirtual;	//������������ ���������� ����������� �������

unsigned short	Reserv3;				//

	// ���������� ������������� ��������� ���������
	ULONGLONG		Flag;					//���������� ����������
	ULONGLONG		CurrentCounter;			//�������� �������� � ������ ������
	ULONGLONG		IrqCounter;				//�������� �������� � ������� ����������
	ULONGLONG		IrqPeriod;				//������� ������ ����������
	ULONGLONG		FreqCounter;			//������� ��������

	// ������ ����������
	unsigned long	MyError;				//������ ����� ��� ������� ����������
	unsigned short	modify;					//����� ���������, ������ ������������ ��������� ���������� ����������
											//������ ��������� ����� ���������
	unsigned long	AdrDSP;					//Adress in DSP
	void			*AdrMem;				//Adress of Memory for DSP
	unsigned long	count;					//���������� ���� ��� ��������
	unsigned long	numberCHNL;				//����� ������ (��������� ����������)
	unsigned short	TestCode[6];			//�������� ��������
	char			verDSP[32];				//������ DSP
	char			DeviceName[16];			//��� �������
char			Reserv4[16];			//���������������
	char			verDriver[64];			//������ ��������
	
	unsigned long	sizeInterruptDAC;		//������ ��� ��������� ������ ��� ������ ����������
											//(sizeInterruptDAC = QuantityPacketsDAC * SizePacketDAC)
	long			StartDAC;				//��������� ����� ������ 1- ����
	unsigned long	MaxSizeInterrupt;		//����������� ��������� ������ ��� ��������� ������ ��� ������ ����������
	unsigned long	MaxSizeInterruptDAC;	//����������� ��������� ������ ��� ��������� ������ ��� ������ ����������
	unsigned long	ChannelDAC;				//������� ���������� ��������� �������
	unsigned long	EnaCycleDAC;			//���������� ������������ ���������� 0-����������, 1-����������
	unsigned long	ExtCycleDAC;			//0 - ��������� ������� �� ���������� ������ DSP, ��� �������� �� ������� ������
											//1 - ��������� ������� �� ������� ������ � ��������� �� �����������
	double			FreqDAC;				//������� ������� �������������� (FreqDAC = OporFreqDAC/RateDAC)
	double			OporFreqDAC;			//������� �������
	double			MaxFreqDAC;				//������������ ������� ��������������
	unsigned long	RateDAC;				//����������� ������� ������� �������
	unsigned long	DigitalInput;			//������ ��������� �����
	unsigned long	DigitalOutput;			//������ ��������� �����
	unsigned long	StopDevice;				//������� ����������
	unsigned long	NumVersionDriver;		//�������� ������ ��������
	unsigned long	ChannelDiff;			//������� ���������� ���������������� �������
	unsigned long	DigitalOutEnable;		//������� ���������� ������� ��������� �����
	unsigned short	SizePacketADC;			//���-�� ���� ������ � ������ ���
	unsigned short	SizePacketDAC;			//���-�� ���� ������ � ������ ���
	unsigned short	MaxSizePacketADC;		//����. ���-�� ���� ������ � ������ ���
	unsigned short	MaxSizePacketDAC;		//����. ���-�� ���� ������ � ������ ���
	unsigned short	QuantityPacketsADC;		//���-�� ������� ������������ �� ���� ���������� ���
	unsigned short	QuantityPacketsDAC;		//���-�� ������� ������������ �� ���� ���������� ���
	unsigned short	MaxQuantityPacketsADC;	//����. ���-�� ������� ������������ �� ���� ���������� ���
	unsigned short	MaxQuantityPacketsDAC;	//����. ���-�� ������� ������������ �� ���� ���������� ���

	unsigned short	StartPWM;				//������ ��� (0x01 - ����� 1 ������, 0x02 - ����� 2 ������, 0x04 - ����� 3 ������;
											//			  0x100 - ���� 1 ������, 0x200 - ���� 2 ������, 0x400 - ���� 3 ������)
	unsigned short	RatePWM;				//����. ������� ������� ������� ��� ���
	unsigned short	PeriodPWM;				//������ ���	
	unsigned short	OnDutyPWM0;				//���������� ��� (����� 0)
	unsigned short	OnDutyPWM1;				//���������� ��� (����� 1)
	unsigned short	OnDutyPWM2;				//���������� ��� (����� 2)
	unsigned short	ShiftPWM1;				//����� ��� (����� 0)
	unsigned short	ShiftPWM2;				//����� ��� (����� 1)

	unsigned short	Revision;				//������� ����������

	unsigned short	Reserved[9];			//���������������
} ADC_INFO, *PADC_INFO;



// sizeof(ADC_INFO_EXT) = 2048 ����
typedef struct _ADC_INFO_EXT{
	// ���������� ������������ DSP
	unsigned short	numberDSP;				//���������� ����� DSP �� �����
	unsigned short	typeADC;				//����� ������ DSP: 0 - ADC, 1 - DAC
	unsigned short	EnableADC;				//������� ���
	unsigned short	EnableDAC;				//������� ���
	unsigned short	EnableExtDevice;		//0 - ������ ���, 0x01 - PRUS, 0x02 - HCP, 0x04 - Atten,
											//0x08 - SD, 0x10 - Ethernet, 0x20 - Bluetooth, 0x40 - PWM, 0x80 - DigAnaliz(����������� �����),
											//0x100 - 1-Wire, 0x200 - ���������� �� GPS, 0x400 - ������������� PTP, 0x800 - ������ PTP
	// ������ DSP
	short	Start;							//��������� ����� ������ 1- ����

	// ������������ �������
	unsigned long	Channel;				//������� ���������� �������� �������
	unsigned long	HCPChannel;				//������� ���������� ���������� ������� HCP
	unsigned long	OverloadChannel;		//������� ���������� ������������� �������
	unsigned short	WorkChADC;				//���������� ���������� ������� ���
	unsigned short	WorkChDAC;				//���������� ���������� ������� ���

	// ������������ ����. ��������
	unsigned short	KodAmplify[32];			//���� �������� �� ���
	unsigned short	TypeConnection;			//��� ���������� (1 - PCI, 2 - USB, 3 - Ethernet, 4 - Bluetouch) (��� �������������!!!)
	double			Amplitude[32];			//����������� �������� ��������� ���
	unsigned short	PRUS[32];				//���� �������� ��������� ������ / ���. (����.) HCP
	double			PreAmplitude[32];		//����������� �������� �������������
	unsigned short	Atten[4];				//���� ����������� ���
	double			Reduction[4];			//����������� �������� ����������� ���

	// ������������ ������
	unsigned short	EnaExtStart;			//���������� �������� ������ ��� 0-�����. 1-����
	unsigned short	EnaExtFreq;				//���������� ������� ������� ��� 0-�����. 1-����
	unsigned short	EnaCycle;				//���������� ������������ ���������� 0-����������, 1-����������
//	unsigned short	ExtCycle;				//0 - ��������� ������� �� ���������� ������ DSP, ��� �������� �� ������� ������
	unsigned short	MasterSynchr;			//1 - �������� �������������, 0 - ������� ���������� � �������������
	unsigned short	ModaADC;				//���� ������ ��� 0-1, 1-2, 2-4, 3-5, 4-8, 5-10 ������������

	unsigned short	EnaExtStartDAC;			//���������� �������� ������ ��� 0-�����. 1-���� (��� �������������!!!)
	unsigned short	EnaExtFreqDAC;			//���������� ������� ������� ��� 0-�����. 1-���� (��� �������������!!!)
unsigned short	Reserv1;				//

	double			Freq;					//������� ������� �������������� (Freq = OporFreq/Rate)
	double			OporFreq;				//������� �������
	double			ExtOporFreq;			//������� ������� �������
	unsigned long	Rate;					//����������� ������� ������� �������

	// ������������ �������
	unsigned long	sizeInterrupt;			//������ ��� ��������� ������ ��� ������ ����������
											//(sizeInterrupt = QuantityPacketsADC * SizePacketADC)
	short			*AddressBuffer;			//����� ������ � ��������� �������
	short			*AddressBufferDAC;		//����� ������ � ��������� �������
	short			*AddressUserBuffer;		//������ ������ � ���������������� �������
	short			*AddressUserBufferDAC;	//����� ������ � ���������������� �������
	unsigned long	SizeBuffer;				//������ ������ ����� �� ����� 1 000 000 ��������
	unsigned long	SizeBufferDAC;			//������ ������ ����� �� ����� 1 000 000 ��������
	unsigned long	Pointer;				//������ �������� ���������� � ������ ���
	unsigned long	PointerDAC;				//������ �������� ���������� � ������ ���
	unsigned long	PointerDSP;				//������ ���������� ���������� � DSP

	// ���������� ��������� ��� (���)
	unsigned short	Bits;					//���������� ��� � ��� ���������������
	unsigned short	BitsDAC;				//���������� ��� � ���
	unsigned short	Words;					//���������� 16-��������� ���� � ����� �������
	unsigned short	WordsDAC;				//���������� 16-��������� ���� � ����� �������
	unsigned short	QuantityChannelDigPort;	// ���������� ����� ��������� ����� (��� �������������!!!)
	double			DigitalResolution;		//��� �������� ������� ���
	double			DigitalResolutionDAC;	//��� �������� ������� ���
	double			MaxFreq;				//������������ ������� ��������������
	unsigned short	QuantityChannelADC;		//������������ ���������� ������� � ������ ���
	unsigned short	QuantityChannelDAC;		//������������ ���������� ������� � ������ ���
	unsigned short	QuantityChannelVirtual;	//������������ ���������� ����������� �������

unsigned short	Reserv3;				//

	// ���������� ������������� ��������� ���������
	ULONGLONG		Flag;					//���������� ����������
	ULONGLONG		CurrentCounter;			//�������� �������� � ������ ������
	ULONGLONG		IrqCounter;				//�������� �������� � ������� ����������
	ULONGLONG		IrqPeriod;				//������� ������ ����������
	ULONGLONG		FreqCounter;			//������� ��������

	// ������ ����������
	unsigned long	MyError;				//������ ����� ��� ������� ����������
	unsigned short	modify;					//����� ���������, ������ ������������ ��������� ���������� ����������
											//������ ��������� ����� ���������
	unsigned long	AdrDSP;					//Adress in DSP
	void			*AdrMem;				//Adress of Memory for DSP
	unsigned long	count;					//���������� ���� ��� ��������
	unsigned long	numberCHNL;				//����� ������ (��������� ����������)
	unsigned short	TestCode[6];			//�������� ��������
	char			verDSP[32];				//������ DSP
	char			DeviceName[16];			//��� �������
char			Reserv4[16];			//���������������
	char			verDriver[64];			//������ ��������
	
	unsigned long	sizeInterruptDAC;		//������ ��� ��������� ������ ��� ������ ����������
											//(sizeInterruptDAC = QuantityPacketsDAC * SizePacketDAC)
	long			StartDAC;				//��������� ����� ������ 1- ����
	unsigned long	MaxSizeInterrupt;		//����������� ��������� ������ ��� ��������� ������ ��� ������ ����������
	unsigned long	MaxSizeInterruptDAC;	//����������� ��������� ������ ��� ��������� ������ ��� ������ ����������
	unsigned long	ChannelDAC;				//������� ���������� ��������� �������
	unsigned long	EnaCycleDAC;			//���������� ������������ ���������� 0-����������, 1-����������
	unsigned long	ExtCycleDAC;			//0 - ��������� ������� �� ���������� ������ DSP, ��� �������� �� ������� ������
											//1 - ��������� ������� �� ������� ������ � ��������� �� �����������
	double			FreqDAC;				//������� ������� �������������� (FreqDAC = OporFreqDAC/RateDAC)
	double			OporFreqDAC;			//������� �������
	double			MaxFreqDAC;				//������������ ������� ��������������
	unsigned long	RateDAC;				//����������� ������� ������� �������
	unsigned long	DigitalInput;			//������ ��������� �����
	unsigned long	DigitalOutput;			//������ ��������� �����
	unsigned long	StopDevice;				//������� ����������
	unsigned long	NumVersionDriver;		//�������� ������ ��������
	unsigned long	ChannelDiff;			//������� ���������� ���������������� �������
	unsigned long	DigitalOutEnable;		//������� ���������� ������� ��������� �����
	unsigned short	SizePacketADC;			//���-�� ���� ������ � ������ ���
	unsigned short	SizePacketDAC;			//���-�� ���� ������ � ������ ���
	unsigned short	MaxSizePacketADC;		//����. ���-�� ���� ������ � ������ ���
	unsigned short	MaxSizePacketDAC;		//����. ���-�� ���� ������ � ������ ���
	unsigned short	QuantityPacketsADC;		//���-�� ������� ������������ �� ���� ���������� ���
	unsigned short	QuantityPacketsDAC;		//���-�� ������� ������������ �� ���� ���������� ���
	unsigned short	MaxQuantityPacketsADC;	//����. ���-�� ������� ������������ �� ���� ���������� ���
	unsigned short	MaxQuantityPacketsDAC;	//����. ���-�� ������� ������������ �� ���� ���������� ���

	// ���
	signed short	StartPWM;				//������ ��� (1 - �����, -1 - ����)
	unsigned short	RatePWM;				//����. ������� ������� ������� ��� ���
	unsigned short	PeriodPWM;				//������ ���	
	unsigned short	OnDutyPWM0;				//���������� ��� (����� 0)
	unsigned short	OnDutyPWM1;				//���������� ��� (����� 1)
	unsigned short	OnDutyPWM2;				//���������� ��� (����� 2)
	unsigned short	ShiftPWM1;				//����� ��� (����� 0)
	unsigned short	ShiftPWM2;				//����� ��� (����� 1)

	unsigned short	Revision;				//������� ����������
	unsigned short	Reserved[7];			//��������������� (�� ��������� � ���������� ADC_INFO !!!!!!!!!!!!!!!!!!!!!!!!!)

	float			DigitalResolChanADC[32];//��������������� ��� �������� ������� ���
	float			DigitalResolChanDAC[4];	//��������������� ��� �������� ������� ���

	unsigned long	SizeBufferDSP_DAC;		// ������ ������ SDRAM ��� � DSP
	unsigned long	MaxSizeBufferDSP_DAC;	// ����. ������ ������ SDRAM ��� � DSP

	double			ExtOporFreqDAC;			//������� ������� �������

	long			OffsetChanADC[32];		//��������������� �������� ���
	long			OffsetChanDAC[4];		//��������������� �������� ���

	ULONGLONG		StartTime;				//���� � ����� ����������� �������
	ULONGLONG		GPSTime;				//���� � ����� ��������� GPS
	LONGLONG		Drift;					//������� �������� ���-�� �������� ����-�� (������� 4 �����)
	LONGLONG		SyncDrift;				//������� �������� � ������ ��������� �������������
	unsigned char	SyncCount;				//������� �������������
	unsigned char	SyncFlag;				//���� �������������
	unsigned char	SatsInUse;				//���-�� ������������� ���������, ��������� ��� �������������
	unsigned char	StatePTP;				//��������� ����� PTP (0 - PTP ����������, 3 - Disabled, 6 - Master, 9 - Slave)
	unsigned char	BatteryCharge;			//������� ������ ������������ (1..100, ���� 0 - ���������� ����������)
	char			BoardTemperature;		//����������� ����� � �������� �������
	unsigned short	BatteryVoltage;			//������� ���������� ������������ � �� (0 ���� ����������)
	unsigned short	BatteryStatus;			//�� ������� (��������, ��������� ������������)
	unsigned short	BatteryStatus2;			//�� ������� (��������, ��� ������)

	long			SDTotalSectors;			//����� ���������� �������� �� SD ����� (0 - ��� ����������, < 0 - ������)
	long			SDFreeSectors;			//���������� ��������� �������� �� SD ����� (0 - ��� ����������, < 0 - ������)

	unsigned short	BuiltinGenSupport;		//����� ��������� ����������� ���������� (����: 0 - ���������, 1 - ���������� ������)
	unsigned short	BuiltinGenControl;		//����� ���������� ���������� ����������� (����: 0 - ���������, 1 - ���������� ������)
	long			BuiltinGenSineFreq;		//������� ������, ��
	long			BuiltinGenSineAmpl;		//��������� ������ (�� ���), ��� ���
	long			BuiltinGenSineBias;		//�������� ������, ��� ���

	unsigned short	DACMUX_Support;			// ����� �������������� ������� DACMUX
											// 0x0001 - ��� 0x35 (���)
	unsigned short	DACMUX_Active;			// ����� ��������� ������� DACMUX
											// 0x0001 - ��� 0x35 (���)
	unsigned short	DACMUX_WaitCode;		// ���� ������� ��� �� 0, �� ����� ��������, ������� 15 ��� - ��� ������� ����� � �����
	unsigned short	DACMUX_WaitReset;		// ���� �� 0, ����� �� ������ ��������
	unsigned long	DACMUX_Diagnost;		// ��������������� ����������, ���� ��������� ���������� �������� ���������� � ������ ���
	unsigned long	DACMUX_DriverAhead;		// ����������, ����������� ���������
	unsigned short	DACMUX_WaitSet;			// ��� ���������� ��������� (���� �� 0)
	unsigned short	DACMUX_Reserved;
	unsigned long	DACMUX_FrameBoost;		// �� ������� ������������� �������� ������ ������

	unsigned long	Stats_CountStarts;		// ���������� ������� ��� � ������� ����������� ��������
	ULONGLONG		Stats_CountADC;			// ���������� �������� ���� ��� � ������� ���������� ������
	ULONGLONG		Stats_CountDAC;			// ���������� ������������ ���� ��� � ������� ���������� ������
	ULONGLONG		Stats_TimeADC;			// ����� ������ ��� � ������� ���������� ������, � ��
	ULONGLONG		Stats_TimeDAC;			// ����� ������ ��� � ������� ���������� ������, � ��

	unsigned char	SyncAdjustment;			//������� �������� ���������� � ��������� (1..100, ���� 0 - ��� ����������)
	unsigned char	ReservedChars[3];		//���������������

	unsigned long	DeviceUptime;			//����� ������ ���������� � ������� ���������, �
	unsigned long	DriverUptime;			//����� ������ �������� � ������ �����������, �
	unsigned short	ReservedExt[212];		//���������������
} ADC_INFO_EXT, *PADC_INFO_EXT;


// sizeof(SYNHR_QUARTZ_INFO) = 64 ����
typedef struct _SYNHR_QUARTZ_INFO
{
	ULONGLONG		FreqCounterPC;
	ULONGLONG		StartCounterPC;
	ULONGLONG		EndCounterPC;
	ULONGLONG		DurationSynhrPC;
	unsigned long	TimerDSP;
	unsigned long	NumberSecondDSP;
	unsigned long	FreqQuartzDSP;
	unsigned long	Reserved1;
	unsigned long	Reserved2;
	unsigned long	Reserved3;
	unsigned long	Reserved4;
	unsigned long	Reserved5;
} SYNHR_QUARTZ_INFO, *PSYNHR_QUARTZ_INFO;

#ifndef _COMMAND_INFO_USB_h
#define _COMMAND_INFO_USB_h
typedef struct _COMMAND_INFO_USB {
	// ������� DSP
	unsigned short	Command;			//�������
	unsigned short	Error;				//�������������� ��� ��� ������
	unsigned long	Size;				//������ � ������
	unsigned short	Data[252];			//������
} COMMAND_INFO_USB, *PCOMMAND_INFO_USB;
#endif



#endif
