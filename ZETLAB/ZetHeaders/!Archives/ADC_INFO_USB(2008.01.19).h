// adc_info_usb.h
#ifndef __adc_info_usb_h__
#define __adc_info_usb_h__

// ���� ������ (ADC_INFO_USB.Command):
//	0x0000 - GetInfo
// 	0x0001 - Reading Digital port
//	0x0002 - Writing to Digital port
//	0x0003 - Digital output enable
//	0x0004 - PWM, Open 1Wire EEPROM
//	0x0005 - Read 1Wire EEPROM
//	0x0006 - Write 1Wire EEPROM
//	0x0007 - Close 1Wire EEPROM
//	0x0012 - PutInfo
//	0x0013 - read correct settings
//	0x0014 - save correct settings
//	0x0015 - read LAN settings
//	0x0016 - save LAN settings
//	0x0105 - read_dir
//	0x0108 - read time from RTC
//	0x0109 - set time to RTC
//	0x0110 - read from SD
//	0x0114 - del File SD
//	0x0116 - write to SD
//	0x0120 - save InfoInt in file
//	0x0122 - Format SD

// 	���� EnaExtStartFreq;		//���������� �������� ������, ������� ������� 0-�����. 1-����
#define ENA_EXT_START_ADC	0x0001	// 0 ��� - ������� ����� ���
#define ENA_EXT_START_DAC	0x0002	// 1 ��� - ������� ����� ���
#define ENA_EXT_FREQ_ADC	0x0004	// 2 ��� - ������� ������� ������� ���
#define ENA_EXT_FREQ_DAC	0x0008	// 3 ��� - ������� ������� ������� ���
#define ENA_INT_CYCLE_DAC	0x0010	// 4 ��� - ������ ��� ��� �������� ������ �����

typedef struct _ADC_INFO_USB {
	// ������� DSP
	unsigned short	Command;				//�������
	unsigned short	TypeADC;				//����� ������ DSP: 0 - ADC, 1 - DAC
	short			Start;					//��������� ����� ������ 1- ����
	short			StartDAC;				//��������� ����� ������ 1- ����

	// ���������� ������������ DSP
	unsigned short	EnableADC;				//������� ���
	unsigned short	EnableDAC;				//������� ���
	unsigned short	EnableExtDevice;		//0 - ������ ���, 0x01 - PRUS, 0x02 - HCP, 0x04 - Atten,
											//0x08 - SD, 0x10 - Ethernet, 0x20 - 1-Wire, 0x40 - PWM, 0x80 - DigAnaliz
	unsigned short	QuantityChannelADC;		//������������ ���������� ������� � ������ ���
	unsigned short	QuantityChannelDAC;		//������������ ���������� ������� � ������ ���

	// ������������ �������
	unsigned long	Channel;				//������� ���������� �������� �������
	unsigned long	ChannelDAC;				//������� ���������� ��������� �������
	unsigned long	HCPChannel;				//������� ���������� ���������� ������� HCP
	unsigned long	OverloadChannel;		//������� ���������� ������������� �������
	unsigned short	WorkChADC;				//���������� ���������� ������� ���
	unsigned short	WorkChDAC;				//���������� ���������� ������� ���

	// ������������ ����. ��������
	unsigned short	CodAmplify[32];			//���� �������� �� ���
	unsigned short	PRUS[32];				//���� �������� ��������� ������ / ���. (����.) HCP
	unsigned short	Atten[4];				//���� ����������� ���

	// ������������ ������
	unsigned short	EnaExtStart;			//���������� �������� ������ 0-�����. 1-����
	unsigned short	EnaExtFreq;				//���������� ������� ������� 0-�����. 1-����
	unsigned short	EnaCycle;				//���������� ������������ ���������� 0-����������, 1-����������
	unsigned short	EnaCycleDAC;			//���������� ������������ ���������� 0-����������, 1-����������
	unsigned short	ExtCycleDAC;			//0 - ��������� ������� �� ���������� ������ DSP
											//1 - ��������� ������� �� ������� ������ � ��������� �� �����������
	unsigned short	ModaADC;				//���� ������ ��� 0-1, 1-2, 2-4, 3-5, 4-8, 5-10 ������������
	unsigned short	Rate;					//����������� ������� ������� �������
	unsigned short	RateDAC;				//����������� ������� ������� �������

	// ������������ �������
	unsigned short	SizeInterrupt;			//������ ��� ��������� ������ ��� ��� ������ ����������
	unsigned short	SizeInterruptDAC;		//������ ��� ��������� ������ ��� ��� ������ ����������
	unsigned long	SizeBufferDAC;			//������ ������ ��� SDRAM � ������

	// ���������� ������������� ��������� ���������
	ULONGLONG		Flag;					//���������� ����������

	// ������ ����������
	unsigned short	AdrDSP;					//Adress in DSP
	unsigned short	Count;					//���������� ���� ��� ��������
	unsigned short	NumberChannel;			//����� ������ (��������� ����������)
	unsigned long	DigitalInput;			//������ ��������� �����
	unsigned long	DigitalOutput;			//������ ��������� �����
	unsigned short	TestCode[6];			//�������� ��������
	char			VersionDSP[64];			//������ DSP
	unsigned long	SerialNumber;			//�������� ����� DSP
	unsigned long	ModelNumber;			//��� ����� � DSP
	unsigned long	Maska;					//��������������� �����
	unsigned long	ChannelDiff;			//������� ���������� ���������������� �������
	unsigned long	DigitalOutEnable;		//������� ���������� ������� ��������� �����

	unsigned long	DigitalResolutionLowADC;	//��� �������� ������� ��� (������� �����)
	unsigned long	DigitalResolutionHighADC;	//��� �������� ������� ��� (������� �����)
	unsigned long	DigitalResolutionLowDAC;	//��� �������� ������� ��� (������� �����)
	unsigned long	DigitalResolutionHighDAC;	//��� �������� ������� ��� (������� �����)


	unsigned long	DigitalResolChanADC[32];	//��������������� ��� �������� ������� ���
	unsigned long	DigitalResolChanDAC[4];		//��������������� ��� �������� ������� ���
	ULONGLONG		SystemTime;					//��������� ����� ������� UTC (��������� FILETIME)

	// ���
	unsigned short	StartPWM;				//������ ��� (0x01 - ����� 1 ������, 0x02 - ����� 2 ������, 0x04 - ����� 3 ������;
											//			  0x100 - ���� 1 ������, 0x200 - ���� 2 ������, 0x400 - ���� 3 ������)
	unsigned short	RatePWM;				//����. ������� ������� ������� ��� ���
	unsigned short	PeriodPWM;				//������ ���	
	unsigned short	OnDutyPWM0;				//���������� ��� (����� 0)
	unsigned short	OnDutyPWM1;				//���������� ��� (����� 1)
	unsigned short	OnDutyPWM2;				//���������� ��� (����� 2)
	unsigned short	ShiftPWM1;				//����� ��� (����� 0)
	unsigned short	ShiftPWM2;				//����� ��� (����� 1)
	
	unsigned short	Reserved[4];			//���������������
} ADC_INFO_USB, *PADC_INFO_USB;


typedef struct _ADC_INFO_USB2 {
	// ������� DSP
	unsigned short	Command;				//�������
	unsigned short	TypeADC;				//����� ������ DSP: 0 - ADC, 1 - DAC
	signed short	Start;					//��������� ����� ������ 1- ����
	signed short	StartDAC;				//��������� ����� ������ 1- ����

	// ���������� ������������ DSP
	unsigned short	EnableADC;				//������� ���
	unsigned short	EnableDAC;				//������� ���
	unsigned short	EnableExtDevice;		//0 - ������ ���, 0x01 - PRUS, 0x02 - HCP, 0x04 - Atten,
											//0x08 - SD, 0x10 - Ethernet, 0x20 - 1-Wire, 0x40 - PWM, 0x80 - DigAnaliz
	unsigned short	QuantityChannelADC;		//������������ ���������� ������� � ������ ���
	unsigned short	QuantityChannelDAC;		//������������ ���������� ������� � ������ ���
	unsigned char	TypeDataADC;			//��� ������ ��� 0-16���, 1-32���, 2-float 
	unsigned char	TypeDataDAC;			//��� ������ ��� 0-16���, 1-32���, 2-float 
//unsigned short Reserved0;	// ������������
	
	// ������������ �������
	unsigned long	Channel;				//������� ���������� �������� �������
	unsigned long	ChannelDAC;				//������� ���������� ��������� �������
	unsigned long	HCPChannel;				//������� ���������� ���������� ������� HCP
	unsigned long	OverloadChannel;		//������� ���������� ������������� �������
	unsigned short	WorkChADC;				//���������� ���������� ������� ���
	unsigned short	WorkChDAC;				//���������� ���������� ������� ���

	// ������������ ����. ��������
	unsigned short	CodAmplify[16];			//���� �������� �� ���
unsigned short Reserved1[16];	
	unsigned short	PRUS[16];				//���� �������� ��������� ������ / ���. (����.) HCP
unsigned short Reserved2[16];	
	unsigned short	Atten[4];				//���� ����������� ���

	// ������������ ������
//	unsigned short	EnaExtStart;			//���������� �������� ������ 0-�����. 1-����
//	unsigned short	EnaExtFreq;				//���������� ������� ������� 0-�����. 1-����
//	unsigned short	EnaCycle;				//���������� ������������ ���������� 0-����������, 1-����������
//	unsigned short	EnaCycleDAC;			//���������� ������������ ���������� 0-����������, 1-����������
//	unsigned short	ExtCycleDAC;			//0 - ��������� ������� �� ���������� ������ DSP
											//1 - ��������� ������� �� ������� ������ � ��������� �� �����������

unsigned long	ProgramMask;	// �������� ������������ ���� (LAN)
unsigned long	ProgramMaskExt;	//
	// ������������ ������
	unsigned short	EnaExtStartFreq;		//���������� �������� ������, ������� ������� 0-�����. 1-����
											// 0 ��� - ������� ����� ���
											// 1 ��� - ������� ����� ���
											// 2 ��� - ������� ������� ������� ���
											// 3 ��� - ������� ������� ������� ���
											// 4 ��� - ������ ��� ��� �������� ������ �����
	unsigned short	ModaADC;				//���� ������ ��� 0-1, 1-2, 2-4, 3-5, 4-8, 5-10 ������������
	unsigned short	Rate;					//����������� ������� ������� �������
	unsigned short	RateDAC;				//����������� ������� ������� �������

	// ������������ �������
	unsigned short	SizeInterrupt;			//������ ��� ��������� ������ ��� ������ ����������
	unsigned short	SizeInterruptDAC;		//������ ��� ��������� ������ ��� ������ ����������
//	unsigned short	PointerDSP;				//������ ���������� ���������� � DSP
	unsigned long	SizeBufferDAC;			//������ ������ ��� SDRAM � ������

	// ���������� ������������� ��������� ���������
	ULONGLONG		Flag;					//���������� ����������

	// ������ ����������
//	unsigned short	AdrDSP;					//Adress in DSP
//	unsigned short	Count;					//���������� ���� ��� ��������
//	unsigned short	NumberChannel;			//����� ������ (��������� ����������)
unsigned short Reserved3[4];	

	unsigned long	DigitalInput;			//������ ��������� �����
	unsigned long	DigitalOutput;			//������ ��������� �����
	unsigned short	TestCode[6];			//�������� ��������
	char			VersionDSP[32];			//������ DSP
unsigned short Reserved4[16];	
	unsigned long	SerialNumber;			//�������� ����� DSP
	unsigned long	ModelNumber;			//��� ����� � DSP
	unsigned long	Maska;					//��������������� �����
	unsigned long	ChannelDiff;			//������� ���������� ���������������� �������
	unsigned long	DigitalOutEnable;		//������� ���������� ������� ��������� �����

	unsigned long	DigitalResolutionADC;		//��� �������� ������� ��� (������� �����)
	unsigned long	DigitalResolutionADCExt;	//��� �������� ������� ��� (������� �����)
	unsigned long	DigitalResolutionDAC;		//��� �������� ������� ��� (������� �����)
	unsigned long	DigitalResolutionDACExt;	//��� �������� ������� ��� (������� �����)


	unsigned long	DigitalResolChanADC[16];	//��������������� ��� �������� ������� ���
	unsigned long	Amplify[16];				//��������������� ����. ��������
	unsigned long	DigitalResolChanDAC[4];		//��������������� ��� �������� ������� ���
	ULONGLONG		SystemTime;					//��������� ����� ������� UTC (��������� FILETIME)

	// ���
	unsigned short	StartPWM;				//������ ��� (0x01 - ����� 1 ������, 0x02 - ����� 2 ������, 0x04 - ����� 3 ������;
											//			  0x100 - ���� 1 ������, 0x200 - ���� 2 ������, 0x400 - ���� 3 ������)
	unsigned short	RatePWM;				//����. ������� ������� ������� ��� ���
	unsigned short	PeriodPWM;				//������ ���	
	unsigned short	OnDutyPWM0;				//���������� ��� (����� 0)
	unsigned short	OnDutyPWM1;				//���������� ��� (����� 1)
	unsigned short	OnDutyPWM2;				//���������� ��� (����� 2)
	unsigned short	ShiftPWM1;				//����� ��� (����� 0)
	unsigned short	ShiftPWM2;				//����� ��� (����� 1)
	
	unsigned short	Reserved[4];			//���������������
} ADC_INFO_USB2, *PADC_INFO_USB2;



typedef struct _COMMAND_INFO_USB {
	// ������� DSP
	unsigned short	Command;			//�������
	unsigned short	Error;				//�������������� ��� ��� ������
	unsigned long	Size;				//������ � ������
	unsigned short	Data[252];			//������
} COMMAND_INFO_USB, *PCOMMAND_INFO_USB;

#endif
