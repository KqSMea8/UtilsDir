Attribute VB_Name = "ADCINFOModule"
Option Explicit

Public Type ADC_INFO
    ' ���������� ������������ DSP
    x_numberDSP               As Integer  '���������� ����� DSP �� �����
    x_typeADC                 As Integer  '����� ������ DSP: 0 - ADC, 1 - DAC
    x_EnableADC               As Integer  '������� ���
    x_EnableDAC               As Integer  '������� ���
    x_EnableExtDevice         As Integer  '0 - ������ ���, 1 - PRUS, 2 - HCP
    
    ' ������ DSP
    x_Start                   As Integer  '��������� ����� ������ 1- ����

    ' ������������ �������
    x_Channel                 As Long     '������� ���������� �������� �������
    x_HCPChannel              As Long     '������� ���������� ���������� ������� HCP
    x_OverloadChannel         As Long     '������� ���������� ������������� �������
    x_WorkChADC               As Integer  '���������� ���������� ������� ���
    x_WorkChDAC               As Integer  '���������� ���������� ������� ���

    ' ������������ ����. ��������
    x_KodAmplify(31)          As Integer  '���� �������� �� ���
    x_TypeConnection          As Integer  '��� ���������� (1 - PCI, 2 - USB, 3 - Ethernet) (��� �������������!!!)
    x_Amplitude(31)           As Double   '����������� �������� ��������� ���
    x_PRUS(31)                As Integer  '���� �������� ��������� ������ / ���. (����.) HCP
    x_PreAmplitude(31)        As Double   '����������� �������� �������������
    x_Atten(3)                As Integer  '����������� ���
    x_Reduction(3)            As Double   '����������� �������� ����������� ���

    ' ������������ ������
    x_EnaExtStart             As Integer  '���������� �������� ������ 0-�����. 1-����
    x_EnaExtFreq              As Integer  '���������� ������� ������� 0-�����. 1-����
    x_EnaCycle                As Integer  '���������� ������������ ���������� 0-����������, 1-����������
    x_ExtCycle                As Integer  'MasterSynchr: 1 - �������� �������������, 0 - ������� ���������� � �������������
    x_ModaADC                 As Integer  '���� ������ ��� 0-1, 1-2, 2-3, 3-4, 4-5, 5-8, 6-10 ������������
    
    x_EnaExtStartDAC          As Integer  '���������� �������� ������ ��� 0-�����. 1-���� (��� �������������!!!)
    x_EnaExtFreqDAC           As Integer  '���������� ������� ������� ��� 0-�����. 1-���� (��� �������������!!!)
x_Reserv3       As Integer

    x_Freq                    As Double   '������� ������� ��������������
    x_OporFreq                As Double   '������� �������
    x_ExtOporFreq             As Double   '������� ������� �������
    x_Rate                    As Long     '����������� ������� ������� ������� Fqwartc/Rate

    ' ������������ �������
    x_sizeInterrupt           As Long     '������ ��� ��������� ������ ��� ������ ���������� ���
    x_AddressBuffer           As Long     '����� ������ � ��������� �������
    x_AddressBufferDAC        As Long     '����� ������ � ��������� �������
    x_AddressUserBuffer       As Long     '������ ������ � ���������������� �������
    x_AddressUserBufferDAC    As Long '����� ������ � ���������������� �������
    x_SizeBuffer              As Long     '������ ������ ����� �� ����� 1 000 000 ��������
    x_SizeBufferDAC           As Long     '������ ������ ����� �� ����� 1 000 000 ��������
    x_Pointer                 As Long     '������ �������� ���������� � ������ ���
    x_PointerDAC              As Long     '������ �������� ���������� � ������ ���
    x_PointerDSP              As Long     '������ ���������� ���������� � DSP

    ' ���������� ��������� ��� (���)
    x_Bits                    As Integer  '���������� ��� � ��� ���������������
    x_BitsDAC                 As Integer  '���������� ��� � ���
    x_Words                   As Integer  '���������� 16-��������� ���� � ����� �������
    x_WordsDAC                As Integer  '���������� 16-��������� ���� � ����� �������
    x_QuantityDigitalChannel  As Integer  '���������� ����� ��������� �����
    x_DigitalResolution       As Double   '��� �������� ������� ���
    x_DigitalResolutionDAC    As Double   '��� �������� ������� ���
    x_MaxFreq                 As Double   '������������ ������� ��������������
    x_QuantityChannelADC      As Integer  '������������ ���������� ������� � ������ ���
    x_QuantityChannelDAC      As Integer  '������������ ���������� ������� � ������ ���

    ' ���������� ������������� ��������� ���������
x_Reserv4   As Integer
x_Reserv5   As Integer
    x_Flag_Low                As Long     '���������� ����������
    x_Flag_High               As Long
    x_CurentCounter_Low       As Long     '�������� �������� � ������ ������
    x_CurentCounter_High      As Long
    x_IrqCounter_Low          As Long     '�������� �������� � ������� ����������
    x_IrqCounter_High         As Long
    x_PeriodIrq_Low           As Long     '������� ������ ����������
    x_PeriodIrq_High          As Long
    x_FreqIrqCounter_Low      As Long     '������� ��������
    x_FreqIrqCounter_High     As Long
    
    ' ������ ����������
    x_MyError                 As Long     '������ ����� ��� ������� ����������
    x_modify                  As Integer  '����� ���������, ������ ������������ ��������� ���������� ����������
                                        '������ ��������� ����� ���������
    x_AdrDSP                  As Long     'Adress in DSP
    x_AdrMem                  As Long     'Adress of Memory for DSP
    x_count                   As Long     '���������� ���� ��� ��������
    x_numberCHNL              As Long     '����� ������ (��������� ����������)
    x_TestCode(5)             As Integer  '�������� ��������
    x_verDSP(63)              As Byte     '������ DSP
    x_verDriver(63)           As Byte     '������ ��������
    x_sizeInterruptDAC        As Long     '������ ��� ��������� ������ ��� ������ ���������� ���
    x_StartDAC                As Long     '��������� ����� ������ 1- ����
    �_MaxSizeInterrupt        As Long     '����������� ��������� ������ ��� ��������� ������ ��� ������ ����������
    x_MaxSizeInterruptDAC     As Long     '����������� ��������� ������ ��� ��������� ������ ��� ������ ����������
    x_ChannelDAC              As Long     '������� ���������� ��������� �������


    x_EnaCycleDAC             As Long     '���������� ������������ ���������� 0-����������, 1-����������
    x_ExtCycleDAC             As Long     '0 - ��������� ������� �� ���������� ������ DSP, ��� �������� �� ������� ������
                                            '1 - ��������� ������� �� ������� ������ � �������� �� �����������
    x_FreqDAC                 As Double   '������� ������� �������������� (FreqDAC = OporFreqDAC/RateDAC)
    x_OporFreqDAC             As Double   '������� �������
    x_MaxFreqDAC              As Double   '������������ ������� ��������������
    x_RateDAC                 As Long     '����������� ������� ������� �������
    x_DigitalInput            As Long     '������ ��������� �����
    x_DigitalOutput           As Long     '������ ��������� �����
    
    x_StopDevice              As Long     '������� ����������
    x_NumVersionDriver        As Long     '�������� ������ ��������
    x_ChannelDiff             As Long     '������� ���������� ���������������� �������
    x_DigitalOutEnable        As Long     '������� ���������� ������� ��������� �����

    x_SizePacketADC           As Integer  '���-�� ���� ������ � ������ ���
    x_SizePacketDAC           As Integer  '���-�� ���� ������ � ������ ���
    x_MaxSizePacketADC        As Integer  '����. ���-�� ���� ������ � ������ ���
    x_MaxSizePacketDAC        As Integer  '����. ���-�� ���� ������ � ������ ���
    x_QuantityPacketsADC      As Integer  '���-�� ������� ������������ �� ���� ���������� ���
    x_QuantityPacketsDAC      As Integer  '���-�� ������� ������������ �� ���� ���������� ���
    x_MaxQuantityPacketsADC   As Integer  '����. ���-�� ������� ������������ �� ���� ���������� ���
    x_MaxQuantityPacketsDAC   As Integer  '����. ���-�� ������� ������������ �� ���� ���������� ���

    x_StartPWM                As Integer  '������ ��� (0x01 - ����� 1 ������, 0x02 - ����� 2 ������, 0x04 - ����� 3 ������;
                                          '            0x100 - ���� 1 ������, 0x200 - ���� 2 ������, 0x400 - ���� 3 ������)
    x_RatePWM                 As Integer  '����. ������� ������� ������� ��� ���
    x_PeriodPWM               As Integer  '������ ���
    x_OnDutyPWM0              As Integer  '���������� ��� (����� 0)
    x_OnDutyPWM1              As Integer  '���������� ��� (����� 1)
    x_OnDutyPWM2              As Integer  '���������� ��� (����� 2)
    x_ShiftPWM1               As Integer  '����� ��� (����� 0)
    x_ShiftPWM2               As Integer  '����� ��� (����� 1)

    x_Reserved(9)             As Integer  '���������������
End Type


Public Type COMMAND_INFO_USB
    x_Command       As Integer  '�������
    x_Error         As Integer  '�������������� ��� ��� ������
    x_Size          As Long     '������ � ������
    x_Data(251)     As Integer  '������
End Type
