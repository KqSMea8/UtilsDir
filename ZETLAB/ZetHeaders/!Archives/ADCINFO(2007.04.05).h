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
											//0x08 - SD, 0x10 - Ethernet, 0x20 - 1-Wire, 0x40 - PWM, 0x80 - DigAnaliz

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
	unsigned short	Reserv0;				//
	double			Amplitude[32];			//����������� �������� ��������� ���
	unsigned short	PRUS[32];				//���� �������� ��������� ������ / ���. (����.) HCP
	double			PreAmplitude[32];		//����������� �������� �������������
	unsigned short	Atten[4];				//���� ����������� ���
	double			Reduction[4];			//����������� �������� ����������� ���

	// ������������ ������
	unsigned short	EnaExtStart;			//���������� �������� ������ 0-�����. 1-����
	unsigned short	EnaExtFreq;				//���������� ������� ������� 0-�����. 1-����
	unsigned short	EnaCycle;				//���������� ������������ ���������� 0-����������, 1-����������
	unsigned short	ExtCycle;				//0 - ��������� ������� �� ���������� ������ DSP, ��� �������� �� ������� ������
											//1 - ��������� ������� �� ������� ������ � ��������� �� �����������
	unsigned short	ModaADC;				//���� ������ ��� 0-1, 1-2, 2-4, 3-5, 4-8, 5-10 ������������

unsigned short	Reserv1;				//
unsigned short	Reserv2;				//
unsigned short	Reserv3;				//


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

unsigned short	Reserv4;				//
unsigned short	Reserv5;				//

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
	char			verDSP[64];				//������ DSP
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

	unsigned short	Reserved[10];			//���������������
} ADC_INFO, *PADC_INFO;



// sizeof(ADC_INFO_EXT) = 2048 ����
typedef struct _ADC_INFO_EXT{
	// ���������� ������������ DSP
	unsigned short	numberDSP;				//���������� ����� DSP �� �����
	unsigned short	typeADC;				//����� ������ DSP: 0 - ADC, 1 - DAC
	unsigned short	EnableADC;				//������� ���
	unsigned short	EnableDAC;				//������� ���
	unsigned short	EnableExtDevice;		//0 - ������ ���, 1 - PRUS, 2 - HCP

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
unsigned short	Reserv0;				//
	double			Amplitude[32];			//����������� �������� ��������� ���
	unsigned short	PRUS[32];				//���� �������� ��������� ������ / ���. (����.) HCP
	double			PreAmplitude[32];		//����������� �������� �������������
	unsigned short	Atten[4];				//���� ����������� ���
	double			Reduction[4];			//����������� �������� ����������� ���

	// ������������ ������
	unsigned short	EnaExtStart;			//���������� �������� ������ 0-�����. 1-����
	unsigned short	EnaExtFreq;				//���������� ������� ������� 0-�����. 1-����
	unsigned short	EnaCycle;				//���������� ������������ ���������� 0-����������, 1-����������
	unsigned short	ExtCycle;				//0 - ��������� ������� �� ���������� ������ DSP, ��� �������� �� ������� ������
											//1 - ��������� ������� �� ������� ������ � ��������� �� �����������
	unsigned short	ModaADC;				//���� ������ ��� 0-1, 1-2, 2-4, 3-5, 4-8, 5-10 ������������

unsigned short	Reserv1;				//
unsigned short	Reserv2;				//
unsigned short	Reserv3;				//

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
unsigned short	QuantityDigitalChannel;				//
	double			DigitalResolution;		//��� �������� ������� ���
	double			DigitalResolutionDAC;	//��� �������� ������� ���
	double			MaxFreq;				//������������ ������� ��������������
	unsigned short	QuantityChannelADC;		//������������ ���������� ������� � ������ ���
	unsigned short	QuantityChannelDAC;		//������������ ���������� ������� � ������ ���

unsigned short	Reserv4;				//
unsigned short	Reserv5;				//

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
	char			verDSP[64];				//������ DSP
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

	unsigned short	Reserved[8];			//��������������� (�� ��������� � ���������� ADC_INFO !!!!!!!!!!!!!!!!!!!!!!!!!)

	float			DigitalResolChanADC[32];//��������������� ��� �������� ������� ���
	float			DigitalResolChanDAC[4];	//��������������� ��� �������� ������� ���

	unsigned short	ReservedExt[366];			//���������������
} ADC_INFO_EXT, *PADC_INFO_EXT;


#endif
