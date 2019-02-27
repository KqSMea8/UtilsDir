Attribute VB_Name = "ZadcModule"
Option Explicit

' �������������� ���� ���������
Global Const KD1610 = 0         ' ADC 16/200
Global Const KD216 = 1          ' APC 216
Global Const KD500 = 2          ' ADC 16/500
Global Const KD500P = 3         ' ADC 16/500P
Global Const KD816 = 4          ' ADC 816
Global Const KD1002 = 5         ' ADC 1002
Global Const KDU216 = 6         ' APC 216 USB
Global Const KD24 = 7           ' ADC 24
Global Const KD1432 = 8         ' ADC 1432
Global Const KDUACPB = 9        ' ACPB USB
Global Const KDU1616 = 10       ' ZET210
Global Const KDUPD14 = 11       ' PD14 USB
Global Const KDUVN = 12         ' ZET110
Global Const KDUOSC = 13        ' ZET302
Global Const KDU8500 = 14       ' A17U8
Global Const KDU2500 = 15       ' A17U2
Global Const KDU1624 = 16       ' ZET220
Global Const KDU0424 = 17       ' ZET230
Global Const KDU0414 = 18       ' ZET240


' ������� ��������
Public Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)
' ������� ������ ������
Public Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" _
 (lpDest As Any, lpSrc As Any, ByVal Length As Long)


'��� ������� ���������� ��� ������. 0 - ��� ������, ����� - ��� ������


' ------------- ������� ������������� ������ �1 -------------
' ������������ � �������� � ������������ ������ �1
Public Declare Function ZExclusive1Open Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long) As Long
' ����������� �� �������� � ������������ ������ �1
Public Declare Function ZExclusive1Close Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long) As Long
' ������� ������� DSP
Public Declare Function ZSendCommand Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, buffer As Any, ByRef size As Long) As Long
' ������� ������� DSP
Public Declare Function ZReceiveCommand Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, buffer As Any, ByRef size As Long) As Long


' ------------- ������� ��������� TCP/IP -------------
' ��������� ��������� Ethernet ���������� ��� TCP/IP �� ���
Public Declare Function ZReadSettingsEthernet Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef sourceIPAdr As Byte, ByRef sourcePort0 As Long, _
 ByRef subnetMask As Byte, ByRef gatewayIPAdr As Byte, ByRef sourceMACAdr As Byte, _
 ByRef duplex As Byte, ByRef speed100Mb As Byte, ByRef TimeoutWDOG As Long, ByRef Reserved As Integer) As Long
' ��������� ��������� Ethernet ���������� ��� TCP/IP � ���
Public Declare Function ZSaveSettingsEthernet Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef sourceIPAdr As Byte, ByRef sourcePort0 As Long, _
 ByRef subnetMask As Byte, ByRef gatewayIPAdr As Byte, ByRef sourceMACAdr As Byte, _
 ByRef duplex As Byte, ByRef speed100Mb As Byte, ByVal TimeoutWDOG As Long, ByVal Reserved As Integer) As Long

' ------------- ������� ��������� PTP -------------
' ��������� ��������� PTP �� ���
Public Declare Function ZReadSettingsPTP Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, _
 ByRef enableMaster As Byte, ByRef enableSlave As Byte, ByRef domain As Byte, _
 ByRef absolutePriority As Byte, ByRef relativePriority As Byte) As Long
' ��������� ��������� PTP � ���
Public Declare Function ZSaveSettingsPTP Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, _
 ByVal enableMaster As Byte, ByVal enableSlave As Byte, ByVal domain As Byte, _
 ByVal absolutePriority As Byte, ByVal relativePriority As Byte) As Long

' ------------- ������� ���������� ���/��� -------------
' ������� ��� ���������� ���/���
' DigitalResolChanADC/DAC:  ��� ����. �������� == 1.0 - ��� �������� �������
'                           ��� ����. �������� > 1.0 - ����. ��������
Public Declare Function ZGetCalibrDataADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal numberChannel As Long, _
 ByRef digitalResolChan As Double, ByRef offsetChan As Double) As Long
Public Declare Function ZGetCalibrDataDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal numberChannel As Long, _
 ByRef digitalResolChan As Double, ByRef offsetChan As Double) As Long
 
Public Declare Function ZSetCalibrDataADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal numberChannel As Long, _
 ByVal digitalResolChan As Double, ByVal offsetChan As Double) As Long
Public Declare Function ZSetCalibrDataDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal numberChannel As Long, _
 ByVal digitalResolChan As Double, ByVal offsetChan As Double) As Long
 ' ��������� �� ��� ������� � �������
Public Declare Function ZReadCalibrData Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long) As Long
' ���������� ���������� �� ���������
Public Declare Function ZSetDefaultCalibrData Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long) As Long
' �������� �� �������� � ���
Public Declare Function ZSaveCalibrData Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long) As Long
 
' ���������� �������!!!
' ���������� ��� �������� ������� ���/���
Public Declare Function ZSetDigitalResolChanADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal numberChannel As Long, _
 ByVal digitalResolChan As Double) As Long
Public Declare Function ZSetDigitalResolChanDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal numberChannel As Long, _
 ByVal digitalResolChan As Double) As Long
' ��������� �� ��� ��� �������� ������� ��� � ���
Public Declare Function ZReadDigitalResolChan Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long) As Long
' ��������� � ��� ��� �������� ������� ��� � ���
Public Declare Function ZSaveDigitalResolChan Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long) As Long



' ------------- ����� ������� -------------
' ������������ � ��������
Public Declare Function ZOpen Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long) As Long
' ����������� �� ��������
Public Declare Function ZClose Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long) As Long

' ����� � ������� ���������� ����������� (������ �� ��� DSP ������ ����������)
Public Declare Function ZResetDSP Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long) As Long
' ������������������� ���������� ���������
Public Declare Function ZInitDSP Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal filename As String) As Long
' �������� �������� ����� DSP
Public Declare Function ZGetSerialNumberDSP Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef serialNumber As Long) As Long
' �������� ����� ������� �������������� DSP
Public Declare Function ZGetRevisionDSP Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef revisionNumber As Long) As Long
' �������� ��� ���������� (������������ ������ �� ����� 16 �������� ������ � ����������� 0, maxSizeStr - ������ ������ strName[])
Public Declare Function ZGetNameDevice Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal verDSP As String, ByVal maxSizeStr As Long) As Long
' �������� ������ ��
Public Declare Function ZGetVersion Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal verDSP As String, ByVal verDRV As String, ByVal verLIB As String) As Long
' �������� ��� ���������� (1 - PCI, 2 - USB, 3 - Ethernet)
Public Declare Function ZGetTypeConnection Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef typeConnection As Long) As Long
' �������� ���������� ����������� ����-��
'long WINAPI ZGetDeviceCapabilities(long typeDevice, long numberDSP, long *capabilities);
Public Declare Function ZGetDeviceCapabilities Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef capabilities As Long) As Long

' ��������� ��� ������
Public Declare Function ZGetError Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef error As Long) As Long
' ���������� ���-�� ��������� ���������� � ������� ��������
Public Declare Function ZGetModify Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef modify As Long) As Long

' ��������� ��� ���������� � ������ ���, ��� �� ��������
Public Declare Function ZGetData Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef data As ADC_INFO) As Long
' ��������� ��� ���������� � ������ ���, ��� �� DSP
Public Declare Function ZGetInfo Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef data As ADC_INFO) As Long
' �������� ���������� � DSP (����� ������� ���������� � ������)
Public Declare Function ZPutInfo Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef data As ADC_INFO) As Long

' ��������� ���� - ���������� ���������� � ������� ZStartADC()
Public Declare Function ZGetFlag Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef Flag As Long) As Long

Public Declare Function ZTestCode Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef code0 As Long, ByRef code1 As Long, ByRef code2 As Long) As Long

' ��������� ���-�� ����� ��������� �����
Public Declare Function ZGetQuantityChannelDigPort Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef quantityChannel As Long) As Long
' ��������� ����� ������� �����
Public Declare Function ZGetDigOutEnable Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef digitalOutEnableMask As Long) As Long
' ���������� ����� ������� �����
Public Declare Function ZSetDigOutEnable Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal digitalOutEnableMask As Long) As Long
' ���������� ����� ��������� ����� �� ����� (��� ��������� ��������� ������ �����)
Public Declare Function ZSetBitDigOutEnable Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal numberOfBit As Long) As Long
' ���������� ����� ����� ��������� ����� �� ����� (��� ��������� ��������� ������ �����)
Public Declare Function ZSetBitMaskDigOutEnable Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal maskOfBits As Long) As Long
' ���������� ����� ��������� ����� �� ���� (��� ��������� ��������� ������ �����)
Public Declare Function ZClrBitDigOutEnable Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal numberOfBit As Long) As Long
' ���������� ����� ����� ��������� ����� �� ���� (��� ��������� ��������� ������ �����)
Public Declare Function ZClrBitMaskDigOutEnable Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal maskOfBits As Long) As Long
' ��������� ������ �� ������ �����
Public Declare Function ZGetDigInput Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef digitalInput As Long) As Long
' ��������� ������, ���������� �� ������ �����
Public Declare Function ZGetDigOutput Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef digitalOutput As Long) As Long
' �������� ������ � ����
Public Declare Function ZSetDigOutput Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal digitalOutput As Long) As Long
' ���������� ����� ��������� ����� � "1" (��� ��������� ��������� ������ �������)
Public Declare Function ZSetBitDigOutput Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal numberOfBit As Long) As Long
' ���������� ����� ������� ��������� ����� � "1" (��� ��������� ��������� ������ �������)
Public Declare Function ZSetBitMaskDigOutput Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal maskOfBits As Long) As Long
' ���������� ����� ��������� ����� � "0" (��� ��������� ��������� ������ �������)
Public Declare Function ZClrBitDigOutput Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal numberOfBit As Long) As Long
' ���������� ����� ������� ��������� ����� � "0" (��� ��������� ��������� ������ �������)
Public Declare Function ZClrBitMaskDigOutput Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal maskOfBits As Long) As Long
' �������� ����� ���������� �����-������ (enable=0 - ���������� ����, enable=1 - ��������� ����)
Public Declare Function ZGetDigitalMode Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef enable As Long) As Long
' ����������� ����� ���������� �����-������ (enable=0 - ���������� ����, enable=1 - ��������� ����)
Public Declare Function ZSetDigitalMode Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal enable As Long) As Long
' ����� ���������� ��������� �������� ������������� �� �������� ����
Public Declare Function ZGetMasterSynchr Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef enable As Long) As Long
' ��������� ��������� �������� ������������� �� �������� ����
Public Declare Function ZSetMasterSynchr Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal enable As Long) As Long

' ��������� �������������� �� ������ ���
Public Declare Function ZFindPWM Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef present As Long) As Long
' ������ ������� ���
Public Declare Function ZStartPWM Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal Start0 As Long, ByVal Start1 As Long, ByVal Start2 As Long) As Long
' ������� ������� ���
Public Declare Function ZStopPWM Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal Stop0 As Long, ByVal Stop1 As Long, ByVal Stop2 As Long) As Long
' ������ ������� ��� ����� ����. ������� ������� ������� � ������
Public Declare Function ZSetFreqPWM Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal Rate As Long, ByVal Period As Long) As Long
' ������ ���������� � ����� ������� ���
Public Declare Function ZSetOnDutyPWM Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal OnDutyPWM0 As Long, ByVal OnDutyPWM1 As Long, _
 ByVal OnDutyPWM2 As Long, ByVal ShiftPWM1 As Long, ByVal ShiftPWM2 As Long) As Long
' ����� �������� ������������� ���������� � DSP
Public Declare Function ZRegulatorPWM Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef data As Long, ByRef size As Long) As Long


' ------------- ������ ������� ��� ������ � ��� ��� ��� -------------
' ��������� ��� � ������� ������ DSP
Public Declare Function ZGetEnableADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef enable As Long) As Long
' ��������� ��� � ������� ������ DSP
Public Declare Function ZGetEnableDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef enable As Long) As Long
' ���������� ���������� ��������� � ����� ���/���
Public Declare Function ZSetTypeADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long) As Long
Public Declare Function ZSetTypeDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long) As Long
' ������ � ����� ��������� ��������� ���/���
Public Declare Function ZGetStartADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef status As Long) As Long
Public Declare Function ZGetStartDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef status As Long) As Long
' ������ ��������� �����/������ �������
Public Declare Function ZStartADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long) As Long
Public Declare Function ZStartDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long) As Long
' ������� ��������� �����/������
Public Declare Function ZStopADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long) As Long
Public Declare Function ZStopDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long) As Long

' ��������� ���-�� ������� ���/���
Public Declare Function ZGetQuantityChannelADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef quantityChannel As Long) As Long
Public Declare Function ZGetQuantityChannelDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef quantityChannel As Long) As Long

' ��������� ��� �������� ������� ���/��� (���������� �������)
Public Declare Function ZGetDigitalResolutionADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef digitalResolution As Double) As Long
Public Declare Function ZGetDigitalResolutionDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef digitalResolution As Double) As Long
' ��������� ��������������� ���������� ��� �������� ������� ���/���
Public Declare Function ZGetDigitalResolChanADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal numberChannel As Long, _
 ByRef digitalResolution As Double) As Long
Public Declare Function ZGetDigitalResolChanDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal numberChannel As Long, _
 ByRef digitalResolution As Double) As Long

' ��������� ���-�� ��� � ����� ������� ���/���
Public Declare Function ZGetBitsADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef numberBits As Long) As Long
Public Declare Function ZGetBitsDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef numberBits As Long) As Long
' ��������� ���-�� ���� � ����� ������� ���/���
Public Declare Function ZGetWordsADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef numberWords As Long) As Long
Public Declare Function ZGetWordsDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef numberWords As Long) As Long

' ��������� ������� �������������
Public Declare Function ZGetFreqADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef freq As Double) As Long
Public Declare Function ZGetFreqDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef freq As Double) As Long
' ��������� ������� �� ������ ��������� ������ �������������
Public Declare Function ZGetListFreqADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal nextFreq As Long, ByRef freq As Double) As Long
Public Declare Function ZGetListFreqDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal nextFreq As Long, ByRef freq As Double) As Long
' ���������� ��������� �� ������ ������� �������������
Public Declare Function ZSetNextFreqADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal nextFreq As Long, ByRef freq As Double) As Long
Public Declare Function ZSetNextFreqDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal nextFreq As Long, ByRef freq As Double) As Long
' ���������� ������� �������������
Public Declare Function ZSetFreqADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal FreqIn As Double, ByRef FreqOut As Double) As Long
Public Declare Function ZSetFreqDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal FreqIn As Double, ByRef FreqOut As Double) As Long

' ��������� �������� ������� ������� �������
Public Declare Function ZGetExtFreqADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef OporFreq As Double) As Long
Public Declare Function ZGetExtFreqDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef OporFreq As Double) As Long
' ���������� �������� ������� ������� �������
Public Declare Function ZSetExtFreqADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal extFreq As Double) As Long
Public Declare Function ZSetExtFreqDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal extFreq As Double) As Long
' ��������� ������ ������������� �� ������� �������
Public Declare Function ZGetEnableExtFreqADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef enable As Long) As Long
Public Declare Function ZGetEnableExtFreqDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef enable As Long) As Long
' ���./����. ������������� �� ������� �������
Public Declare Function ZSetEnableExtFreqADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal enable As Long) As Long
Public Declare Function ZSetEnableExtFreqDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal enable As Long) As Long
' ��������� ������ �������� �������
Public Declare Function ZGetEnableExtStartADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef enable As Long) As Long
Public Declare Function ZGetEnableExtStartDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef enable As Long) As Long
' ���./����. �������� �������
Public Declare Function ZSetEnableExtStartADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal enable As Long) As Long
Public Declare Function ZSetEnableExtStartDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal enable As Long) As Long

' ���������� ������������� ������ ������ ��� ����������
Public Declare Function ZGetInterruptADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef size As Long) As Long
Public Declare Function ZGetInterruptDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef size As Long) As Long
' ���������� ����. ��������� ������ ������ ��� ����������
Public Declare Function ZGetMaxInterruptADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef size As Long) As Long
Public Declare Function ZGetMaxInterruptDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef size As Long) As Long
' ���������� ������ ������ ��� ��������� ��� ����������
Public Declare Function ZSetInterruptADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal size As Long) As Long
Public Declare Function ZSetInterruptDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal size As Long) As Long

' ���������� ������������� ������ ������ ������ DSP
Public Declare Function ZGetSizePacketADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef size As Long) As Long
Public Declare Function ZGetSizePacketDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef size As Long) As Long
' ���������� ����. ��������� ������ ������ ������ DSP
Public Declare Function ZGetMaxSizePacketADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef size As Long) As Long
Public Declare Function ZGetMaxSizePacketDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef size As Long) As Long
' ���������� ������ ������ ������ DSP
Public Declare Function ZSetSizePacketADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal size As Long) As Long
Public Declare Function ZSetSizePacketDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal size As Long) As Long

' ���������� ������������� ���-�� ������� �� ���� ����������
Public Declare Function ZGetQuantityPacketsADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef size As Long) As Long
Public Declare Function ZGetQuantityPacketsDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef size As Long) As Long
' ���������� ����. ��������� ���-�� ������� �� ���� ����������
Public Declare Function ZGetMaxQuantityPacketsADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef size As Long) As Long
Public Declare Function ZGetMaxQuantityPacketsDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef size As Long) As Long
' ���������� ���-�� ������� �� ���� ����������
Public Declare Function ZSetQuantityPacketsADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal size As Long) As Long
Public Declare Function ZSetQuantityPacketsDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal size As Long) As Long

' ���������� ����������� ��� ����������� ���������� � ����� ��� ��
Public Declare Function ZSetCycleSampleADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal enable As Long) As Long
Public Declare Function ZSetCycleSampleDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal enable As Long) As Long

' ������ ������ ������ ��� � ��� ��
Public Declare Function ZSetBufferSizeADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal size As Long) As Long
Public Declare Function ZSetBufferSizeDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal size As Long) As Long
' ��������� ����� � ��� ��
Public Declare Function ZGetBufferADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, buffer As Any, ByRef size As Long) As Long
Public Declare Function ZGetBufferDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, buffer As Any, ByRef size As Long) As Long
' ���������� ����� � ��� ��
Public Declare Function ZRemBufferADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, buffer As Any) As Long
Public Declare Function ZRemBufferDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, buffer As Any) As Long
' ������ ��������� ������ � ��� ��
Public Declare Function ZGetPointerADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef pointer As Long) As Long
Public Declare Function ZGetPointerDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef pointer As Long) As Long

' ����� ���������� ���������� �������
Public Declare Function ZGetNumberInputADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef workChannel As Long) As Long
Public Declare Function ZGetNumberOutputDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef workChannel As Long) As Long
' ����� ������� �� �������� �����
Public Declare Function ZGetInputADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal numberChannel As Long, ByRef enable As Long) As Long
Public Declare Function ZGetOutputDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal numberChannel As Long, ByRef enable As Long) As Long
' ��������/��������� �������� �����
Public Declare Function ZSetInputADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal numberChannel As Long, ByVal enable As Long) As Long
Public Declare Function ZSetOutputDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal numberChannel As Long, ByVal enable As Long) As Long

' ------------- ������� ��� ������ ������ � ��� -------------
' �������� ���������� ������ ���
Public Declare Function ZGetLastDataADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal numberChannel As Long, ByRef buffer As Any, ByVal size As Long) As Long

' ��������� ����� ������ ����������
Public Declare Function ZGetModaADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef moda As Long) As Long
' ���������� ����� ������ ���������� (�������., ������������ �� �������������)
Public Declare Function ZSetModaADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal moda As Long) As Long

' ��������� ����. ��������
Public Declare Function ZGetAmplifyADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal numberChannel As Long, ByRef amplify As Double) As Long
' ��������� ����. �������� �� ������ ��������� ����. ��������
Public Declare Function ZGetListAmplifyADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal nextAmpl As Long, ByRef amplify As Double) As Long
' ���������� ��������� ����. ��������
Public Declare Function ZSetNextAmplifyADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal numberChannel As Long, ByVal nextAmpl As Long, ByRef amplify As Double) As Long
' ���������� ����. ��������
Public Declare Function ZSetAmplifyADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal numberChannel As Long, ByVal amplifyIn As Double, ByRef amplifyOut As Double) As Long

' ��������� ����. �������� ��8/10
Public Declare Function ZGetPreAmplifyADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal numberChannel As Long, ByRef amplify As Double) As Long
' ��������� ����. �������� �� ������ ��������� ����. �������� ��8/10
Public Declare Function ZGetListPreAmplifyADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal nextPreAmpl As Long, ByRef amplify As Double) As Long
' ���������� ��������� ����. �������� ��8/10
Public Declare Function ZSetNextPreAmplifyADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal numberChannel As Long, ByVal nextAmpl As Long, ByRef amplify As Double) As Long
' ���������� ����. �������� ��8/10
Public Declare Function ZSetPreAmplifyADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal numberChannel As Long, ByVal amplifyIn As Double, ByRef amplifyOut As Double) As Long

' ��������� �������������� � ��������� �� ������ HCP
Public Declare Function ZFindHCPADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef present As Long) As Long
' ��������� � ��������� ��������� ������ ������ HCP
Public Declare Function ZGetHCPADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal numberChannel As Long, ByRef enable As Long) As Long
' ���������� ��������� ��������� ������ ������ HCP
Public Declare Function ZSetHCPADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal numberChannel As Long, ByVal enable As Long) As Long

' ����� ����������������� ������ ��������� ������ ��� �����
Public Declare Function ZGetInputDiffADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal numberChannel As Long, ByRef enable As Long) As Long
' ����������-�������� �������� ����� ��� ����� � ���������������� �����
Public Declare Function ZSetInputDiffADC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal numberChannel As Long, ByVal enable As Long) As Long

' ------------- ������� ��� ������ ������ � ��� -------------
' ���������� ������� (�� ������ ��) ��� ���������� (������ �� ������ DSP) �������� � ���
Public Declare Function ZSetExtCycleDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal enable As Long) As Long

' ��������� ���. ��������� �����������
Public Declare Function ZGetAttenDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal numberChannel As Long, ByRef reduction As Double) As Long
' ���������� ���. ��������� �����������
Public Declare Function ZSetAttenDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal numberChannel As Long, ByVal reductionIn As Double, ByRef reductionOut As Double) As Long

' ����� ������������� ������� ������ ��� � DSP
Public Declare Function ZGetMaxSizeBufferDSPDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef size As Long) As Long
'long WINAPI ZGetSizeBufferDSPDAC(long numberDSP, long *size);
' ���������� ������ ������ ��� � DSP
Public Declare Function ZGetSizeBufferDSPDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByRef size As Long) As Long
' ���������� ���. ��������� �����������
Public Declare Function ZSetSizeBufferDSPDAC Lib "Zadc.dll" _
(ByVal typeDevice As Long, ByVal numberDSP As Long, ByVal size As Long) As Long


' ������� ��������� ����� ������ �� ������������ ���� (������ C-�����)
Public Function Trim0(sName As String) As String
   ' Right trim string at first null.
   Dim x As Integer
   x = InStr(sName, vbNullChar)
   If x > 0 Then Trim0 = Left$(sName, x - 1) Else Trim0 = sName
End Function


