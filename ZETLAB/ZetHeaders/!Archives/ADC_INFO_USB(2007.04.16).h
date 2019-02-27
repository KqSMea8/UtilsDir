// adc_info_usb.h
#ifndef __adc_info_usb_h__
#define __adc_info_usb_h__

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
	unsigned short	SizeInterrupt;			//������ ��� ��������� ������ ��� ������ ����������
	unsigned short	SizeInterruptDAC;			//������ ��� ��������� ������ ��� ������ ����������
	unsigned short	PointerDSP;				//������ ���������� ���������� � DSP

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


typedef struct _COMMAND_INFO_USB {
	// ������� DSP
	unsigned short	Command;			//�������
	unsigned short	Error;				//�������������� ��� ��� ������
	unsigned long	Size;				//������ � ������
	unsigned short	Data[252];			//������
} COMMAND_INFO_USB, *PCOMMAND_INFO_USB;

#endif
