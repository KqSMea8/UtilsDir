// �������������� ���� ���������
#define KD1610	0	// ADC 16/200
#define KD216	1	// APC 216
#define KD500	2	// ADC 16/500
#define KD500P	3	// ADC 16/500P
#define KD816	4	// ADC 816
#define KD1002	5	// ADC 1002
#define KDU216	6	// APC 216 USB
#define KD24	7	// ADC 24
#define KD1432	8	// ADC 1432
#define KDUACPB	9	// ACPB USB
#define KDU1616	10	// ZET210
#define KDUPD14	11	// PD14 USB
#define KDUVN	12	// ZET110
#define KDUOSC	13	// ZET302
#define KDU8500	14	// A17U8
#define KDU2500	15	// A17U2
#define KDU1624	16	// ZET220
#define KDU0424	17	// ZET230
#define KDU0414	18	// ZET048
#define KDU0824	19	// ZET240

#define ZET_DEVICE_TYPE_COUNT 20
#define ZET_DEFINE_DEVICE_NAMES(name) \
static const TCHAR *name[ZET_DEVICE_TYPE_COUNT] = { \
	_T("ADC 16/200"), \
	_T("APC 216"), \
	_T("ADC 16/500"), \
	_T("ADC 16/500P"), \
	_T("ADC 816"), \
	_T("ADC 1002"), \
	_T("APC 216 USB"), \
	_T("ADC 24"), \
	_T("ADC 1432"), \
	_T("ACPB USB"), \
	_T("ZET210"), \
	_T("PD14 USB"), \
	_T("ZET110"), \
	_T("ZET302"), \
	_T("A17U8"), \
	_T("A17U2"), \
	_T("ZET220"), \
	_T("ET230"), \
	_T("ZET048"), \
	_T("ZET240"), \
}

//��� ������� ���������� ��� ������. 0 - ��� ������, ����� - ��� ������

// ------------- ������� ��� ������ ����� LAN -------------
// ������������ IP-����� �� LAN
long WINAPI ZnetPing(char *strIP, long *Timeout);

// ����������� � ����������� �� LAN
long WINAPI ZnetOpen(long typeDevice, long numberDSP, long OpenTimeout, long CommandTimeout, char *AddressIP, long PortIP);
// ��������� ���������� � ����������� ����� LAN
long WINAPI ZnetClose(long typeDevice, long numberDSP, long Timeout);
// ��������� ����������� �� ���������� � ����������� ����� LAN
long WINAPI ZnetGetReady(long typeDevice, long numberDSP);
// ��������� ����������� �� ���������� � ����������� ����� LAN � ������� IP-������ (AddressIP[16])
long WINAPI ZnetGetReadyExt(long typeDevice, long numberDSP, char *AddressIP, long *PortIP);
// ZnetGetReady ����������:
//	0 - ��� ������
//	1 - �� ������� � �� �����������
//	2 - �����������, �� ����� ��� �� �����������
//	3 - �����������, ����� � ��������� ������� �����������
//	4,5,6 ����� ��������
// 0x100 - ������������ numberDSP

// ��������� ����� ������������ ��������� � ����.
// ������� func ����� ���������� �� ���������� ������.
// ��� ��������� ������������ � �������� ������ ����� ������� ������� � ���������� func = NULL.
// �������� status:
// -1 - ���������� ������� (��������� ��������� ������)
//  0 - ���������� �������� ��� �����������
//  1 - ���������� ��� ����� ����������� (� ���� ������ peer_ip ����� ��������� ����� ����, ��� � ���� �����������)
long WINAPI ZnetScan(long typeDevice,
					 void (WINAPI *func)(long serial, long status, const WCHAR *label, const char *ip, long port, const char *peer_ip, void *arg),
					 void *arg);

// ------------- ������� ������������ ������� -------------
// ������� � ������������ ����� �1
//(������������ ������ ������ � ����������, �������� � ���������� ������� �������� - ��������)
long WINAPI ZExclusive1Open(long typeDevice, long numberDSP);
// ����� �� ������������� ������ �1
long WINAPI ZExclusive1Close(long typeDevice, long numberDSP);
// ������� � ������������ ����� �2
//(������������ � � ���������� � � ��������, �.�. ���������� ���������� �������)
long WINAPI ZExclusive2Open(long typeDevice, long numberDSP);
// ����� �� ������������� ������ �2
long WINAPI ZExclusive2Close(long typeDevice, long numberDSP);
// ������� ������� DSP (size - ������ 512)
long WINAPI ZSendCommand(long typeDevice, long numberDSP, void *buffer, long *size);
// ������� ������� DSP (size - ������ 512)
long WINAPI ZReceiveCommand(long typeDevice, long numberDSP, void *buffer, long *size);
// ������� ������ ��� (size - ������ 512)	������� ������ ��� USB !!!
long WINAPI ZSendDataDAC(long typeDevice, long numberDSP, void *buffer, long *size);
// ������� ������ ��� (size - ������ 512)	������� ������ ��� USB !!!
long WINAPI ZReceiveDataADC(long typeDevice, long numberDSP, void *buffer, long *size);

// ------------- ������� ��������� TCP/IP -------------
// ��������� ��������� Ethernet ���������� ��� TCP/IP �� ���
long WINAPI ZReadSettingsEthernet(long typeDevice, long numberDSP, unsigned char *sourceIPAdr,
								  unsigned short *sourcePort0, unsigned char *subnetMask,
								  unsigned char *gatewayIPAdr, unsigned char *sourceMACAdr,
								  unsigned char *duplex, unsigned char *speed100Mb,
								  unsigned short *TimeoutWDOG, unsigned short *Reserved);

// ��������� ��������� Ethernet ���������� ��� TCP/IP � ���
long WINAPI ZSaveSettingsEthernet(long typeDevice, long numberDSP, unsigned char *sourceIPAdr,
								  unsigned short *sourcePort0, unsigned char *subnetMask,
								  unsigned char *gatewayIPAdr, unsigned char *sourceMACAdr,
								  unsigned char *duplex, unsigned char *speed100Mb,
								  unsigned short TimeoutWDOG, unsigned short Reserved);

// ------------- ������� ��������� PTP -------------
// ��������� ��������� PTP �� ���
long WINAPI ZReadSettingsPTP(long typeDevice, long numberDSP,
							 unsigned char *enableMaster, unsigned char *enableSlave, unsigned char *domain,
							 unsigned char *absolutePriority, unsigned char *relativePriority);

// ��������� ��������� PTP � ���
long WINAPI ZSaveSettingsPTP(long typeDevice, long numberDSP,
							 unsigned char enableMaster, unsigned char enableSlave, unsigned char domain,
							 unsigned char absolutePriority, unsigned char relativePriority);

// ------------- ������� TEDS -------------
// ��������� �������������� �� ������ TEDS
long WINAPI ZFindTEDS(long typeDevice, long numberDSP, long* present);
// ����������� �������� ����� � ����� 1Wire (TEDS) � ������� EEPROM, � ����� �������� ����� EEPROM
// ZOpenTEDS ����������:
//	0 - ��� ������
//	2 - ��� ������ �� ����������� 1-Wire
//	3 - ������ ������ � ������������ 1-Wire
//	4 - 1-Wire ����������
//	5 - alarming presence pulse (������������ ������ ��� ���������� ���������� ������� ����������)
//	6 - no presence pulse (��� ������ �� TEDS)
//	7-9 - ��� ������ �� ����������� 1-Wire
// 0x100 - ������������ numberDSP
// 0x200 - ������������ ����� ������
// 0x300 - ������ TEDS �� ��������������
long WINAPI ZOpenTEDS(long typeDevice, long numberDSP, long numberChannel, void *pBuffer);

//  ��������� ����� 1Wire (TEDS)
long WINAPI ZCloseTEDS(long typeDevice, long numberDSP);
// ��������� Size = 1-8 ����, ������� � ������ Address
// ZReadTEDS ����������:
//	0 - ��� ������
//	2 - ��� ������ �� ����������� 1-Wire
//	3 - ������ ������ � ������������ 1-Wire
//	4 - 1-Wire ����������
//	5 - alarming presence pulse
//	6 - no presence pulse (��� ������ �� TEDS)
//	7-12 - ��� ������ �� ����������� 1-Wire
// 0x100 - ������������ numberDSP
// 0x200 - ������������ ����� ������
// 0x300 - ������ TEDS �� ��������������
long WINAPI ZReadTEDS(long typeDevice, long numberDSP, long Address, void *pBuffer, long *Size);



// ------------- ������� ���������� ���/��� -------------
// ������� ��� ���������� ���/���
// DigitalResolChan:	 ��� ����. �������� == 1.0 - ��� �������� �������, �)
//						 ��� ����. �������� > 1.0 - ����. ��������
// offsetChan - ������� ��������, � 
long WINAPI ZGetCalibrDataADC(long typeDevice, long numberDSP, long numberChannel, double *digitalResolChan, double *offsetChan);
long WINAPI ZGetCalibrDataDAC(long typeDevice, long numberDSP, long numberChannel, double *digitalResolChan, double *offsetChan);
long WINAPI ZSetCalibrDataADC(long typeDevice, long numberDSP, long numberChannel, double digitalResolChan, double offsetChan);
long WINAPI ZSetCalibrDataDAC(long typeDevice, long numberDSP, long numberChannel, double digitalResolChan, double offsetChan);
long WINAPI ZReadCalibrData(long typeDevice, long numberDSP);		// ��������� �� ��� ������� � �������
long WINAPI ZSetDefaultCalibrData(long typeDevice, long numberDSP);	// ���������� ���������� �� ���������
long WINAPI ZSaveCalibrData(long typeDevice, long numberDSP);		// �������� �� �������� � ���
// ������� ���������� ���������� ������ ����������������� ������ ���
// ������ (�����������������) ����� ����������� � ������� ZSetCalibrDataDAC
// numberChannelDiff - ����������� ����� ������ ��� ����� ��, ��� � ZSetCalibrDataDAC
// nonidentityMul, nonidentityAdd - ����������������� � ���������� �������������� ���������� ������
long WINAPI ZGetCalibrDataDiffDAC(long typeDevice, long numberDSP, long numberChannelDiff, double *nonidentityMul, double *nonidentityAdd);
long WINAPI ZSetCalibrDataDiffDAC(long typeDevice, long numberDSP, long numberChannelDiff, double nonidentityMul, double nonidentityAdd);

// ------------- ����� ������� -------------
// ������������ � ��������
long WINAPI ZOpen(long typeDevice, long numberDSP);
// ����������� �� ��������
long WINAPI ZClose(long typeDevice, long numberDSP);

// ����� � ������� ���������� ����������� (������ �� ��� DSP ������ ����������)
long WINAPI ZResetDSP(long typeDevice, long numberDSP);
// ������������������� ���������� ���������
long WINAPI ZInitDSP(long typeDevice, long numberDSP, const char *fileName);
// �������� �������� ����� DSP
long WINAPI ZGetSerialNumberDSP(long typeDevice, long numberDSP, long *serialNumber);
// �������� ����� ������� �������������� DSP
long WINAPI ZGetRevisionDSP(long typeDevice, long numberDSP, long *revisionNumber);
// �������� ��� ���������� (������������ ������ �� ����� 16 �������� ������ � ����������� 0, maxSizeStr - ������ ������ strName[])
long WINAPI ZGetNameDevice(long typeDevice, long numberDSP, char *strName, long maxSizeStr);
// �������� ������ �� (������������ ������ �� ����� 64 �������� ������ � ����������� 0)
long WINAPI ZGetVersion(long typeDevice, long numberDSP, char *verDSP, char *verDRV, char *verLIB);
// �������� ��� ���������� (1 - PCI, 2 - USB, 3 - Ethernet)
long WINAPI ZGetTypeConnection(long typeDevice, long numberDSP, long *type);
// �������� ���������� ����������� ����-��
long WINAPI ZGetDeviceCapabilities(long typeDevice, long numberDSP, long *capabilities);
// ��������� ��� ������
long WINAPI ZGetError(long typeDevice, long numberDSP, long *error);
// ���������� ���-�� ��������� ���������� � ������� ��������
long WINAPI ZGetModify(long typeDevice, long numberDSP, long *modify);

// ��������� ��� ���������� � ������ ���, ��� �� ��������
long WINAPI ZGetData(long typeDevice, long numberDSP, ADC_INFO *Data);
long WINAPI ZGetDataExt(long typeDevice, long numberDSP, ADC_INFO_EXT *Data);
// ��������� ��� ���������� � ������ ���, ��� �� DSP
long WINAPI ZGetInfo(long typeDevice, long numberDSP, ADC_INFO *Data);
// �������� ���������� � DSP (����� ������� ���������� � ������)
long WINAPI ZPutInfo(long typeDevice, long numberDSP, ADC_INFO *Data);
// ��������� ����������� ���������� �� DSP
long WINAPI ZGetInfoExt(long typeDevice, long numberDSP, ADC_INFO_EXT *Data);

// ��������� ���� - ���������� ���������� DSP (�������) � ������� ZStartADC()
long WINAPI ZGetFlag(long typeDevice, long numberDSP, unsigned long *flag);

// ��������� ������ ������������� �� ������� ������� (���������� �������, �� ��������������� ������� ��� ��� � ���)
long WINAPI ZGetEnableExtFreq(long typeDevice, long numberDSP, long *enable);
// ���./����. ������������� �� ������� ������� (���������� �������, �� ��������������� ������� ��� ��� � ���)
long WINAPI ZSetEnableExtFreq(long typeDevice, long numberDSP, long enable);
// ��������� ������ �������� ������� (���������� �������, �� ��������������� ������� ��� ��� � ���)
long WINAPI ZGetEnableExtStart(long typeDevice, long numberDSP, long *enable);
// ���./����. �������� ������� (���������� �������, �� ��������������� ������� ��� ��� � ���)
long WINAPI ZSetEnableExtStart(long typeDevice, long numberDSP, long enable);
// ��������� ���� ������ ��� ���� PnP
long WINAPI ZTestCode(long typeDevice, long numberDSP, long *code0, long *code1, long *code2);


// ��������� ���-�� ����� ��������� �����
long WINAPI ZGetQuantityChannelDigPort(long typeDevice, long numberDSP, long *quantityChannel);
// ��������� ����� ������� �����
long WINAPI ZGetDigOutEnable(long typeDevice, long numberDSP, unsigned long *digitalOutEnableMask);
// �������� ����� ������� �����
long WINAPI ZSetDigOutEnable(long typeDevice, long numberDSP, unsigned long digitalOutEnableMask);
// ���������� ����� ��������� ����� �� ����� (��� ��������� ��������� ������ �����, numberOfBit = 0..quantityChannel-1)
long WINAPI ZSetBitDigOutEnable(long typeDevice, long numberDSP, long numberOfBit);
// ���������� ����� ����� ��������� ����� �� ����� (��� ��������� ��������� ������ �����)
long WINAPI ZSetBitMaskDigOutEnable(long typeDevice, long numberDSP, unsigned long maskOfBits);
// ���������� ����� ��������� ����� �� ���� (��� ��������� ��������� ������ �����, numberOfBit = 0..quantityChannel-1)
long WINAPI ZClrBitDigOutEnable(long typeDevice, long numberDSP, long numberOfBit);
// ���������� ����� ����� ��������� ����� �� ���� (��� ��������� ��������� ������ �����)
long WINAPI ZClrBitMaskDigOutEnable(long typeDevice, long numberDSP, unsigned long maskOfBits);
// ��������� ������ �� ������ �����
long WINAPI ZGetDigInput(long typeDevice, long numberDSP, unsigned long *digitalInput);
// ��������� ������, ���������� �� ������ �����
long WINAPI ZGetDigOutput(long typeDevice, long numberDSP, unsigned long *digitalOutput);
// �������� ������ � ����
long WINAPI ZSetDigOutput(long typeDevice, long numberDSP, unsigned long digitalOutput);
// ���������� ����� ��������� ����� � "1" (��� ��������� ��������� ������ �������, numberOfBit = 0..quantityChannel-1)
long WINAPI ZSetBitDigOutput(long typeDevice, long numberDSP, long numberOfBit);
// ���������� ����� ������� ��������� ����� � "1" (��� ��������� ��������� ������ �������)
long WINAPI ZSetBitMaskDigOutput(long typeDevice, long numberDSP, unsigned long maskOfBits);
// ���������� ����� ��������� ����� � "0" (��� ��������� ��������� ������ �������, numberOfBit = 0..quantityChannel-1)
long WINAPI ZClrBitDigOutput(long typeDevice, long numberDSP, long numberOfBit);
// ���������� ����� ������� ��������� ����� � "0" (��� ��������� ��������� ������ �������)
long WINAPI ZClrBitMaskDigOutput(long typeDevice, long numberDSP, unsigned long maskOfBits);
// �������� ����� ���������� �����-������ (mode=0 - ���������� ����, mode=1 - ��������� ����, mode=2 - UART)
										// +0x04 - ��������������� UART
										// +0x08 - 9 ��� UART
										// +0x10 - 2 ����-���� UART (�� ���.)
										// +0x20 - �������� �������� UART (�� ���.)
										// +0x40 - IrDA UART (�� ���.)
long WINAPI ZGetDigitalMode(long typeDevice, long numberDSP, long *mode);
// ����������� ����� ���������� �����-������ (mode=0 - ���������� ����, mode=1 - ��������� ����, mode=2 - UART)
long WINAPI ZSetDigitalMode(long typeDevice, long numberDSP, long mode);
// ����� ���������� ��������� �������� ������������� �� �������� ����  
long WINAPI ZGetMasterSynchr(long typeDevice, long numberDSP, long *enable);
// ��������� ��������� �������� ������������� �� �������� ����  
long WINAPI ZSetMasterSynchr(long typeDevice, long numberDSP, long enable);

// ��������� ������������� �������� ���������� ����������
long WINAPI ZStartSynhrQuartz(long typeDevice, long numberDSP);
// ���������� ������������� �������� ���������� ����������
long WINAPI ZStopSynhrQuartz(long typeDevice, long numberDSP);
// �������� ��������� ��������� ������������
long WINAPI ZGetSynhrTimeQuartz(long typeDevice, long numberDSP, SYNHR_QUARTZ_INFO *synhrElement);
// ���������� ������� �������� ���������� ����������
long WINAPI ZTrimmingQuartz(long typeDevice, long numberDSP, long size);
// �������� ���� � ����� GPS
long WINAPI ZGetGPSTime(long typeDevice, long numberDSP, ULONGLONG *time);
// �������� ���� � ����� ������
long WINAPI ZGetStartTime(long typeDevice, long numberDSP, ULONGLONG *time);
// ���������� ���� � ����� ������ (time=0 - ��������� ������ �� �������)
long WINAPI ZSetStartTime(long typeDevice, long numberDSP, ULONGLONG time);

// ��������� �������������� �� ������ GPS
long WINAPI ZFindUART(long typeDevice, long numberDSP, long *present);
// ���������������� UART (stopBits = 1, 2; ConfigParity: 0 ��� - Parity, 1 ��� - Even, 2 ��� - StickParity)
long WINAPI ZSetConfigUART(long typeDevice, long numberDSP, long baudRate, short stopBits, short ConfigParity);
// ������� ������ �������� � UART
long WINAPI ZReadUART(long typeDevice, long numberDSP, void *buffer, long *size);
// ������� ������ �������� � UART
long WINAPI ZWriteUART(long typeDevice, long numberDSP, void *buffer, long *size);
// ��������� ����� ��� ����������������� ������ UART 
long WINAPI ZStartBufReadingUART(long typeDevice, long numberDSP);
// ��������� ����� ��� ���������������� ������ UART
long WINAPI ZStartBufWritingUART(long typeDevice, long numberDSP);
// ���������� ����� ��� ����������������� ������ UART
long WINAPI ZStopBufReadingUART(long typeDevice, long numberDSP);
// ���������� ����� ��� ���������������� ������ UART
long WINAPI ZStopBufWritingUART(long typeDevice, long numberDSP);
// ��������� ������ UART ����� �����
long WINAPI ZReadBufUART(long typeDevice, long numberDSP, void *buffer, long *size);
// �������� ������ UART ����� �����
long WINAPI ZWriteBufUART(long typeDevice, long numberDSP, void *buffer, long *size);
// ������������ ����� UART ����� GPS � ������������ ������� (switchRead: 0 - ������ ������ GPS, 1 - ������ ������ ������; switchWrite: 1 - ����� � UART �� ���� GPIOB, 0 - ������ GPIOB �� ���������)
long WINAPI ZSwitchUART(long typeDevice, long numberDSP, long switchRead, long switchWrite);
// ��������� ��������� ���������� ���� �������� ��������������� � UART
long WINAPI ZReadFromUART(long typeDevice, long numberDSP, void *buffer, long *size);
// ������� � ����� �����������
long WINAPI ZSetRegistratorMode(long typeDevice, long numberDSP);
// ������������ ������������� ������
long WINAPI ZResetAM(long typeDevice, long numberDSP);
// ���������� ������ ���������� �� ������ ��������������� ��� � �����������
long WINAPI ZReadUT(long typeDevice, long numberDSP, float *U1, float *U2, float *U3, float *U4, float *T);
// ����������� �������
long WINAPI ZAsynchrGate(long typeDevice, long numberDSP, long gate);
// ��������� �������������� �� ������ ���
long WINAPI ZFindPWM(long typeDevice, long numberDSP, long* present);
// ������ ������� ���
long WINAPI ZStartPWM(long typeDevice, long numberDSP,long Start0, long Start1, long Start2);
// ������� ������� ���
long WINAPI ZStopPWM(long typeDevice, long numberDSP, long Stop0, long Stop1, long Stop2);
// ������ ������� ��� ����� ����. ������� ������� ������� � ������
long WINAPI ZSetFreqPWM(long typeDevice, long numberDSP, long Rate, long Period);
// ������ ���������� � ����� ������� ���
long WINAPI ZSetOnDutyPWM(long typeDevice, long numberDSP, long OnDutyPWM0, long OnDutyPWM1, long OnDutyPWM2, long ShiftPWM1, long ShiftPWM2);
// ����� �������� ������������� ���������� � DSP
long WINAPI ZRegulatorPWM(long typeDevice, long numberDSP, void *data, long *size);


// ------------- ������ ������� ��� ������ � ��� ��� ��� -------------
// ��������� ��� � ������� ������ DSP
long WINAPI ZGetEnableADC(long typeDevice, long numberDSP, long *enable);
// ��������� ��� � ������� ������ DSP
long WINAPI ZGetEnableDAC(long typeDevice, long numberDSP, long *enable);
// ���������� ���������� ��������� � ����� ���/���
long WINAPI ZSetTypeADC(long typeDevice, long numberDSP);
long WINAPI ZSetTypeDAC(long typeDevice, long numberDSP);
// ������ � ����� ��������� ��������� ���/���
long WINAPI ZGetStartADC(long typeDevice, long numberDSP, long *status);
long WINAPI ZGetStartDAC(long typeDevice, long numberDSP, long *status);
// ������ ��������� �����/������ �������
long WINAPI ZStartADC(long typeDevice, long numberDSP);
long WINAPI ZStartDAC(long typeDevice, long numberDSP);
// ������� ��������� �����/������
long WINAPI ZStopADC(long typeDevice, long numberDSP);
long WINAPI ZStopDAC(long typeDevice, long numberDSP);

// ��������� ���-�� ������� ���/���
long WINAPI ZGetQuantityChannelADC(long typeDevice, long numberDSP, long *quantityChannel);
long WINAPI ZGetQuantityChannelDAC(long typeDevice, long numberDSP, long *quantityChannel);
// ��������� ��� �������� ������� ���/��� (���������� �������)
long WINAPI ZGetDigitalResolutionADC(long typeDevice, long numberDSP, double *digitalResolution);
long WINAPI ZGetDigitalResolutionDAC(long typeDevice, long numberDSP, double *digitalResolution);
// ��������� ��������������� ���������� ��� �������� ������� ���/��� (� �������) 
long WINAPI ZGetDigitalResolChanADC(long typeDevice, long numberDSP, long numberChannel, double *digitalResolChan);
long WINAPI ZGetDigitalResolChanDAC(long typeDevice, long numberDSP, long numberChannel, double *digitalResolChan);
// ��������� ���-�� ��� � ����� ������� ���/���
long WINAPI ZGetBitsADC(long typeDevice, long numberDSP, long *numberBits);
long WINAPI ZGetBitsDAC(long typeDevice, long numberDSP, long *numberBits);
// ��������� ���-�� ���� � ����� ������� ���/���
long WINAPI ZGetWordsADC(long typeDevice, long numberDSP, long *numberWords);
long WINAPI ZGetWordsDAC(long typeDevice, long numberDSP, long *numberWords);

// ��������� ������� ������������� 
long WINAPI ZGetFreqADC(long typeDevice, long numberDSP, double *freq);
long WINAPI ZGetFreqDAC(long typeDevice, long numberDSP, double *freq);
// ��������� ������� �� ������ ��������� ������ ������������� 
long WINAPI ZGetListFreqADC(long typeDevice, long numberDSP, long next, double *freq);
long WINAPI ZGetListFreqDAC(long typeDevice, long numberDSP, long next, double *freq);
// ���������� ��������� �� ������ ������� ������������� 
long WINAPI ZSetNextFreqADC(long typeDevice, long numberDSP, long next, double *freq);
long WINAPI ZSetNextFreqDAC(long typeDevice, long numberDSP, long next, double *freq);
// ���������� ������� �������������
long WINAPI ZSetFreqADC(long typeDevice, long numberDSP, double freqIn, double *freqOut);
long WINAPI ZSetFreqDAC(long typeDevice, long numberDSP, double freqIn, double *freqOut);
// ���������� ������������� ����������� ������� (�������., ������������ �� �������������)
long WINAPI ZGetRateADC(long typeDevice, long numberDSP, long *rate);
long WINAPI ZGetRateDAC(long typeDevice, long numberDSP, long *rate);
// Rate - ����������� ������� ������� ������� ������������ � AdcGetOporFreq()
// ���������� ������� �������������� ��� (�������., ������������ �� �������������)
long WINAPI ZSetRateADC(long typeDevice, long numberDSP, long rate);
long WINAPI ZSetRateDAC(long typeDevice, long numberDSP, long rate);

// ��������� �������� ������� ������� �������
long WINAPI ZGetExtFreqADC(long typeDevice, long numberDSP, double *oporFreq);
long WINAPI ZGetExtFreqDAC(long typeDevice, long numberDSP, double *oporFreq);
// ���������� �������� ������� ������� �������
long WINAPI ZSetExtFreqADC(long typeDevice, long numberDSP, double extFreq);
long WINAPI ZSetExtFreqDAC(long typeDevice, long numberDSP, double extFreq);
// ��������� ������ ������������� �� ������� ������� 
long WINAPI ZGetEnableExtFreqADC(long typeDevice, long numberDSP, long *enable);
long WINAPI ZGetEnableExtFreqDAC(long typeDevice, long numberDSP, long *enable);
// ���./����. ������������� �� ������� �������
long WINAPI ZSetEnableExtFreqADC(long typeDevice, long numberDSP, long enable);
long WINAPI ZSetEnableExtFreqDAC(long typeDevice, long numberDSP, long enable);
// ��������� ������ �������� �������
long WINAPI ZGetEnableExtStartADC(long typeDevice, long numberDSP, long *enable);
long WINAPI ZGetEnableExtStartDAC(long typeDevice, long numberDSP, long *enable);
// ���./����. �������� �������
long WINAPI ZSetEnableExtStartADC(long typeDevice, long numberDSP, long enable);
long WINAPI ZSetEnableExtStartDAC(long typeDevice, long numberDSP, long enable);

// ���������� ������������� ������ ������ ��� ����������
long WINAPI ZGetInterruptADC(long typeDevice, long numberDSP, long *size);
long WINAPI ZGetInterruptDAC(long typeDevice, long numberDSP, long *size);
// ���������� ����. ��������� ������ ������ ��� ����������
long WINAPI ZGetMaxInterruptADC(long typeDevice, long numberDSP, long *size);
long WINAPI ZGetMaxInterruptDAC(long typeDevice, long numberDSP, long *size);
// ���������� ������ ������ ��� ��������� ��� ����������
long WINAPI ZSetInterruptADC(long typeDevice, long numberDSP, long size);
long WINAPI ZSetInterruptDAC(long typeDevice, long numberDSP, long size);

// ���������� ������������� ������ ������ ������ DSP
long WINAPI ZGetSizePacketADC(long typeDevice, long numberDSP, long *size);
long WINAPI ZGetSizePacketDAC(long typeDevice, long numberDSP, long *size);
// ���������� ����. ��������� ������ ������ ������ DSP
long WINAPI ZGetMaxSizePacketADC(long typeDevice, long numberDSP, long *size);
long WINAPI ZGetMaxSizePacketDAC(long typeDevice, long numberDSP, long *size);
// ���������� ������ ������ ������ DSP
long WINAPI ZSetSizePacketADC(long typeDevice, long numberDSP, long size);
long WINAPI ZSetSizePacketDAC(long typeDevice, long numberDSP, long size);

// ���������� ������������� ���-�� ������� �� ���� ����������
long WINAPI ZGetQuantityPacketsADC(long typeDevice, long numberDSP, long *size);
long WINAPI ZGetQuantityPacketsDAC(long typeDevice, long numberDSP, long *size);
// ���������� ����. ��������� ���-�� ������� �� ���� ����������
long WINAPI ZGetMaxQuantityPacketsADC(long typeDevice, long numberDSP, long *size);
long WINAPI ZGetMaxQuantityPacketsDAC(long typeDevice, long numberDSP, long *size);
// ���������� ���-�� ������� �� ���� ����������
long WINAPI ZSetQuantityPacketsADC(long typeDevice, long numberDSP, long size);
long WINAPI ZSetQuantityPacketsDAC(long typeDevice, long numberDSP, long size);

// ���������� ����������� ��� ����������� ���������� � ����� ��� �� 
long WINAPI ZSetCycleSampleADC(long typeDevice, long numberDSP, long enable);
long WINAPI ZSetCycleSampleDAC(long typeDevice, long numberDSP, long enable);

// ������ ������ ������ ��� � ��� ��
long WINAPI ZSetBufferSizeADC(long typeDevice, long numberDSP, long size);
long WINAPI ZSetBufferSizeDAC(long typeDevice, long numberDSP, long size);
// ��������� ����� � ��� ��
long WINAPI ZGetBufferADC(long typeDevice, long numberDSP, void **buffer, long *size);
long WINAPI ZGetBufferDAC(long typeDevice, long numberDSP, void **buffer, long *size);
// ���������� ����� � ��� ��
long WINAPI ZRemBufferADC(long typeDevice, long numberDSP, void **buffer);
long WINAPI ZRemBufferDAC(long typeDevice, long numberDSP, void **buffer);
// ������ ��������� ������ � ��� ��
long WINAPI ZGetPointerADC(long typeDevice, long numberDSP, long *pointer);
long WINAPI ZGetPointerDAC(long typeDevice, long numberDSP, long *pointer);

// ����� ���������� ���������� �������
long WINAPI ZGetNumberInputADC(long typeDevice, long numberDSP, long *workChannels);
long WINAPI ZGetNumberOutputDAC(long typeDevice, long numberDSP, long *workChannels);
// ����� ������� �� �������� �����
long WINAPI ZGetInputADC(long typeDevice, long numberDSP, long numberChannel, long *enable);
long WINAPI ZGetOutputDAC(long typeDevice, long numberDSP, long numberChannel, long *enable);
// ��������/��������� �������� �����
long WINAPI ZSetInputADC(long typeDevice, long numberDSP, long numberChannel, long enable);
long WINAPI ZSetOutputDAC(long typeDevice, long numberDSP, long numberChannel, long enable);
// ��������/��������� ������ � ������� ������� ����� (�������., ������������ �� �������������)
long WINAPI ZSetChannelADC(long typeDevice, long numberDSP, unsigned long channelMask);
long WINAPI ZSetChannelDAC(long typeDevice, long numberDSP, unsigned long channelMask);


// ------------- ������� ��� ������ ������ � ��� -------------
// �������� ���������� ������ ��� (������ �� ��������������!)
long WINAPI ZGetLastDataADC(long typeDevice, long numberDSP, long numberChannel, void *buffer, long size);

// ��������� ����� ������ ����������
long WINAPI ZGetModaADC(long typeDevice, long numberDSP, long *moda);
// ���������� ����� ������ ���������� (�������., ������������ �� �������������)
long WINAPI ZSetModaADC(long typeDevice, long numberDSP, long moda);

// ��������� ����. ��������
long WINAPI ZGetAmplifyADC(long typeDevice, long numberDSP, long numberChannel, double *amplify);
// ��������� ����. �������� �� ������ ��������� ����. ��������
long WINAPI ZGetListAmplifyADC(long typeDevice, long numberDSP, long next, double *amplify);
// ���������� ��������� ����. ��������
long WINAPI ZSetNextAmplifyADC(long typeDevice, long numberDSP, long numberChannel, long next, double *amplify);
// ���������� ����. ��������
long WINAPI ZSetAmplifyADC(long typeDevice, long numberDSP, long numberChannel, double amplifyIn, double *amplifyOut);

// ��������� ����. �������� ��8/10
long WINAPI ZGetPreAmplifyADC(long typeDevice, long numberDSP, long numberChannel, double *amplify);
// ��������� ����. �������� �� ������ ��������� ����. �������� ��8/10
long WINAPI ZGetListPreAmplifyADC(long typeDevice, long numberDSP, long next, double *amplify);
// ���������� ��������� ����. �������� ��8/10
long WINAPI ZSetNextPreAmplifyADC(long typeDevice, long numberDSP, long numberChannel, long next, double *amplify);
// ���������� ����. �������� ��8/10
long WINAPI ZSetPreAmplifyADC(long typeDevice, long numberDSP, long numberChannel, double amplifyIn, double *amplifyOut);

// ��������� �������������� � ��������� �� ������ HCP
long WINAPI ZFindHCPADC(long typeDevice, long numberDSP, long *present);
// ��������� � ��������� ��������� ������ ������ HCP
long WINAPI ZGetHCPADC(long typeDevice, long numberDSP, long numberChannel, long *enable);
// ���������� ��������� ��������� ������ ������ HCP
long WINAPI ZSetHCPADC(long typeDevice, long numberDSP, long numberChannel, long enable);

// ����� ����������������� ������ ��������� ������ ��� ����� 
long WINAPI ZGetInputDiffADC(long typeDevice, long numberDSP, long numberChannel, long *enable);
// ����������-�������� �������� ����� ��� ����� � ���������������� �����
long WINAPI ZSetInputDiffADC(long typeDevice, long numberDSP, long numberChannel, long enable);


// ------------- ������� ��� ������ ������ � ��� -------------
// ���������� ������� (�� ������ ��) ��� ���������� (������ �� ������ DSP) �������� � ���
long WINAPI ZSetExtCycleDAC(long typeDevice, long numberDSP, long enable);

// �������� �������������� �� ����������� ����������
long WINAPI ZFindSoftAtten(long typeDevice, long numberDSP, long *present);
// ��������� ���. ��������� �����������
long WINAPI ZGetAttenDAC(long typeDevice, long numberDSP, long numberChannel, double *reduction);
// ���������� ���. ��������� �����������
long WINAPI ZSetAttenDAC(long typeDevice, long numberDSP, long numberChannel, double reductionIn, double *reductionOut);

// ����� ������������� ������� ������ ��� � DSP
long WINAPI ZGetMaxSizeBufferDSPDAC(long typeDevice, long numberDSP, long *size);
// ����� ������� ������ ��� � DSP
long WINAPI ZGetSizeBufferDSPDAC(long typeDevice, long numberDSP, long *size);
// ���������� ������ ������ ��� � DSP
long WINAPI ZSetSizeBufferDSPDAC(long typeDevice, long numberDSP, long size);


// ------------- ������� ��� ������ � ������ ���������� -------------
// ���������� ����� ����������
long WINAPI ZSetDeviceLabel(long typeDevice, long numberDSP, const WCHAR *label);
// ��������� ����� ����������
long WINAPI ZGetDeviceLabel(long typeDevice, long numberDSP, WCHAR *label, long maxWchars);

// ------------- ������� ��� ������ � ��� -------------
// ���������� � ��������� ��������� ���
long WINAPI ZSetVibrationControlParams(long typeDevice, long numberDSP, long numberChannel, double value, long numberSamples, double coef, const void *ext, long extSize);
long WINAPI ZGetVibrationControlDiagnostics(long typeDevice, long numberDSP, void *data, long *size);

// ------------- ������� ��� ������ �� ���������� ����������� -------------
// ��������/��������� ���������� ���������
long WINAPI ZGetEnableBuiltinGenerator(long typeDevice, long numberDSP, long *enable);
// ��������� ��������� ����������� ����������
long WINAPI ZSetEnableBuiltinGenerator(long typeDevice, long numberDSP, long enable);
// ���������� ��������� �������������� ����� ����������� ����������
long WINAPI ZSetSineBuiltinGenerator(long typeDevice, long numberDSP, long active, double freq, double level, double offset);
// ��������� ��������� �������������� ����� ����������� ����������
long WINAPI ZGetSineBuiltinGenerator(long typeDevice, long numberDSP, long *active, double *freq, double *level, double *offset);
// ���������� ��������� ����� 1PPS ����������� ����������
long WINAPI ZSet1PPSBuiltinGenerator(long typeDevice, long numberDSP, long active);
// ��������� ��������� ����� 1PPS ����������� ����������
long WINAPI ZGet1PPSBuiltinGenerator(long typeDevice, long numberDSP, long *active);


// ------------- ������� ��� ������ � DAC_MUX -------------
// ��������� ��������� ������ DAC_MUX
long WINAPI ZCheckModeDACMUX(long typeDevice, long numberDSP, long mode);
// ���������� ����� (0 - ����� �����)
long WINAPI ZSetModeDACMUX(long typeDevice, long numberDSP, long mode);
// ��������� ������� �����
long WINAPI ZGetModeDACMUX(long typeDevice, long numberDSP, long *mode);
// ��������� ��� ��������
long WINAPI ZGetWaitDACMUX(long typeDevice, long numberDSP, long *waitCode);
// ���������� ��� �������� (���������� ���������)
long WINAPI ZSetWaitDACMUX(long typeDevice, long numberDSP, long waitCode);
// �������� ��� ��������
long WINAPI ZResetWaitDACMUX(long typeDevice, long numberDSP, long waitCode);
// �������� ����� DAC_MUX_FB_STRUCT
long WINAPI ZGetDataDACMUX(long typeDevice, long numberDSP, void *data, long *dataSize);


// ���������� �������� ������� (��. ����) !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//��� ������� ���������� ��� ������. 0 - ��� ������, ����� ��� ������
// ������� �������������� �� ��� DSP:
int WINAPI Adc_StopDSP(int typeDevice, int numberDSP);	// ����� � ������� ���������� ����������� (������ �� ��� DSP ������ ����������)

// ------------- ������� ������������� ������ �1 -------------

// ������������ � �������� � ������������ ������ �1
int WINAPI Adc_Exclusive1Open(int typeDevice, int numberDSP);
// ����������� �� �������� � ������������ ������ �1
int WINAPI Adc_Exclusive1Close(int typeDevice, int numberDSP);
// ������� ������� DSP
int WINAPI Adc_SendCommand(int typeDevice, int numberDSP, void *buffer, long *size);
// ������� ������� DSP
int WINAPI Adc_ReceiveCommand(int typeDevice, int numberDSP, void *buffer, long *size);
// ������� ������ ��� (size - ������ 512)
int WINAPI Adc_SendDataDAC(int typeDevice, int numberDSP, void *buffer, long *size);
// ������� ������ ��� (size - ������ 512)
int WINAPI Adc_ReceiveDataADC(int typeDevice, int numberDSP, void *buffer, long *size);


// ------------- ������� ��������� TCP/IP -------------
// ��������� ��������� LAN ���������� ��� TCP/IP �� ���
int WINAPI Adc_ReadSettingsLAN(int typeDevice, int numberDSP, unsigned char *sourceIPAdr,
							   unsigned short *sourcePort0, unsigned char *subnetMask,
							   unsigned char *gatewayIPAdr, unsigned char *sourceMACAdr);
// ��������� ��������� LAN ���������� ��� TCP/IP � ���
int WINAPI Adc_SaveSettingsLAN(int typeDevice, int numberDSP, unsigned char *sourceIPAdr,
							   unsigned short *sourcePort0, unsigned char *subnetMask,
							   unsigned char *gatewayIPAdr, unsigned char *sourceMACAdr);


// ------------- ����� ������� -------------
int WINAPI AdcOpen(int typeDevice, int numberDSP);		// ������������ � ��������
int WINAPI AdcClose(int typeDevice, int numberDSP);		// ����������� �� ��������
int WINAPI Adc_Load_DSP(int typeDevice, int numberDSP, ULONG *mass, ULONG size, ULONG adrdsp);	// ��������� ������ � ������ ����������� ����������
int WINAPI Adc_UnLoad_DSP(int typeDevice, int numberDSP, ULONG *mass, ULONG size, ULONG adrdsp);	// ��������� ������ �� ������ ����������� ����������
int WINAPI Adc_Zagr_DSP(int typeDevice, int numberDSP, char *filename);	// ��������� ��������� � ���������� ���������
int WINAPI AdcGetVersion(int typeDevice, int numberDSP, char *verDSP, char *verDRV, char *verLIB); // �������� ������ ������
int WINAPI AdcGetData(int typeDevice, int numberDSP, ADC_INFO *Data);	// ��������� ��� ���������� � ������ ���, ��� �� ��������
int WINAPI AdcGetDataExt(int typeDevice, int numberDSP, ADC_INFO_EXT *Data);
int WINAPI AdcGetInfo(int typeDevice, int numberDSP, ADC_INFO *Data);	// ��������� ��� ���������� � ������ ���, ��� �� DSP
int WINAPI AdcPutInfo(int typeDevice, int numberDSP, ADC_INFO *Data);	// �������� ���������� � DSP (����� ������� ���������� � ������)
int WINAPI AdcError(int typeDevice, int numberDSP, int *error);		// ��������� ��� ������
int WINAPI AdcFlag(int typeDevice, int numberDSP, UINT *flag);		// ������ ����� - ���������� ���������� �� ������� AdcStart()
int WINAPI AdcGetModify(int typeDevice, int numberDSP, int *modify);	// ���������� ���-�� ��������� ���������� � ������� ��������
int WINAPI AdcGetTypeAdc(int typeDevice, int numberDSP, int *typeDeviceADC);// ���������� � ����� ������ ��������� ���������� ���������
int WINAPI AdcTestCode(int typeDevice, int numberDSP, int *TestCode0, int *TestCode1, int *TestCode2);	// ��������� ���� ������ ��� ���� PnP

int WINAPI AdcGetDigOutEnable(int typeDevice, int numberDSP, int *OutEnableMask);	// ��������� ����� ������� �����
int WINAPI AdcSetDigOutEnable(int typeDevice, int numberDSP, int OutEnableMask);	// ���������� ����� ������� �����
int WINAPI AdcGetDigInput(int typeDevice, int numberDSP, int *Input);		// ��������� ������ �� ������ �����
int WINAPI AdcGetDigOutput(int typeDevice, int numberDSP, int *Output);	// ��������� ������, ���������� �� ������ �����
int WINAPI AdcSetDigOutput(int typeDevice, int numberDSP, int Output);	// �������� ������ � ����

// ������� ��� ������ � ��� ��� ���:
int WINAPI AdcSetTypeDac(int typeDevice, int numberDSP);					// ���������� ���������� ��������� � ����� ���
int WINAPI AdcSetTypeAdc(int typeDevice, int numberDSP);					// ���������� ���������� ��������� � ������ ���
int WINAPI AdcGetEnableAdc(int typeDevice, int numberDSP, int *EnableADC);	// ��������� ��� � ������� ������ DSP
int WINAPI AdcGetEnableDac(int typeDevice, int numberDSP, int *EnableDAC);	// ��������� ��� � ������� ������ DSP
int WINAPI AdcStart(int typeDevice, int numberDSP);							// ������ ��������� ����� ������� ���
int WINAPI AdcStartDac(int typeDevice, int numberDSP);						// ������ ��������� ������ ������� ���
int WINAPI AdcGetStart(int typeDevice, int numberDSP, UINT *status);			// ������ � ����� ��������� ��������� ���
int WINAPI AdcGetStartDac(int typeDevice, int numberDSP, UINT *status);		// ������ � ����� ��������� ��������� ���
int WINAPI AdcStop(int typeDevice, int numberDSP);							// ������� ��������� ����� ���
int WINAPI AdcStopDac(int typeDevice, int numberDSP);							// ������� ��������� ������ ���

int WINAPI AdcGetQuantityChannelAdc(int typeDevice, int numberDSP, int *QuantityChannelADC);	// ���-�� ������� ���
int WINAPI AdcGetQuantityChannelDac(int typeDevice, int numberDSP, int *QuantityChannelDAC);	// ���-�� ������� ���
int WINAPI AdcGetDigitalResolutionAdc(int typeDevice, int numberDSP, double *DigitalResolutionADC);// ��� �������� ������� ���
int WINAPI AdcGetDigitalResolutionDac(int typeDevice, int numberDSP, double *DigitalResolutionDAC);// ��� �������� ������� ���
int WINAPI AdcGetDigitalResolChanAdc(int typeDevice, int numberDSP, int numberChannel, double *digitalResolChan);
int WINAPI AdcGetDigitalResolChanDac(int typeDevice, int numberDSP, int numberChannel, double *digitalResolChan);

int WINAPI AdcGetAdcBits(int typeDevice, int numberDSP, int *numberBITS);		// ����������� ���
int WINAPI AdcGetDacBits(int typeDevice, int numberDSP, int *numberBITS);		// ����������� ���
int WINAPI AdcGetAdcWords(int typeDevice, int numberDSP, int *numberWORDS);	// ���-�� ���� � ����� ������� ���
int WINAPI AdcGetDacWords(int typeDevice, int numberDSP, int *numberWORDS);	// ���-�� ���� � ����� ������� ���

int WINAPI AdcRate(int typeDevice, int numberDSP, int Rate);			// ���������� ������� �������������� ��� (�������.)
int WINAPI AdcRateDac(int typeDevice, int numberDSP, int Rate);		// ���������� ������� �������������� ��� (�������.)
								// Rate - ����������� ������� ������� ������� ������������ � AdcGetOporFreq()
int WINAPI AdcGetRate(int typeDevice, int numberDSP, int *Rate);		// ���������� ������������� ����������� ������� (�������.)
int WINAPI AdcGetRateDac(int typeDevice, int numberDSP, int *Rate);	// ���������� ������������� ����������� ������� (�������.)
int WINAPI AdcGetFreq(int typeDevice, int numberDSP, double *freq);		// ���������� ������� ��� 
int WINAPI AdcGetFreqDac(int typeDevice, int numberDSP, double *freq);	// ���������� ������� ���
int WINAPI AdcGetListFreq(int typeDevice, int numberDSP, int next, double *freq);		// �������� ������ ������ ���
int WINAPI AdcGetListFreqDac(int typeDevice, int numberDSP, int next, double *freq);	// �������� ������ ������ ���
int WINAPI AdcSetNextFreq(int typeDevice, int numberDSP, int next, double *freq);		// ���������� ��������� �� ������ ������� ��� 
int WINAPI AdcSetNextFreqDac(int typeDevice, int numberDSP, int next, double *freq);	// ���������� ��������� �� ������ ������� ���
int WINAPI AdcSetFreq(int typeDevice, int numberDSP, double freqin, double *freqout);		// ���������� ������� ���
int WINAPI AdcSetFreqDac(int typeDevice, int numberDSP, double freqin, double *freqout);	// ���������� ������� ���
int WINAPI AdcSetExtFreq(int typeDevice, int numberDSP, double extFreq);		// ���������� �������� ������� ������� ������� ���
int WINAPI AdcSetExtFreqDac(int typeDevice, int numberDSP, double extFreq);	// ���������� �������� ������� ������� ������� ���
int WINAPI AdcGetExtFreq(int typeDevice, int numberDSP, double *oporfreq);	// �������� �������� ������� ������� ���
int WINAPI AdcGetExtFreqDac(int typeDevice, int numberDSP, double *oporfreq);	// �������� �������� ������� ������� ���
int WINAPI AdcEnaExtFreq(int typeDevice, int numberDSP, short ext);		// ���./����. ������������� �� ������� �������
int WINAPI AdcEnaExtFreqDac(int typeDevice, int numberDSP, short ext);		// ���./����. ������������� �� ������� �������
int WINAPI AdcGetEnaExtFreq(int typeDevice, int numberDSP, short *ext);	// ��������� ������ ������������� �� ������� �������
int WINAPI AdcGetEnaExtFreqDac(int typeDevice, int numberDSP, short *ext);	// ��������� ������ ������������� �� ������� �������
int WINAPI AdcEnaExtStart(int typeDevice, int numberDSP, short ext);		// ���./����. �������� �������
int WINAPI AdcEnaExtStartDac(int typeDevice, int numberDSP, short ext);		// ���./����. �������� �������
int WINAPI AdcGetEnaExtStart(int typeDevice, int numberDSP, short *ext);	// ��������� ������ �������� �������
int WINAPI AdcGetEnaExtStartDac(int typeDevice, int numberDSP, short *ext);	// ��������� ������ �������� �������


int WINAPI AdcInterrupt(int typeDevice, int numberDSP, int size);		// ���������� ������ ������ ��� ��������� ��� ���������� ���
int WINAPI AdcInterruptDac(int typeDevice, int numberDSP, int size);	// ���������� ������ ������ ��� ��������� ��� ���������� ���
int WINAPI AdcGetInterrupt(int typeDevice, int numberDSP, int *size);		// ���������� ������������� ������ ������ ��� ���������� ���
int WINAPI AdcGetInterruptDac(int typeDevice, int numberDSP, int *size);	// ���������� ������������� ������ ������ ��� ���������� ���
int WINAPI AdcGetMaxInterrupt(int typeDevice, int numberDSP, int *size);		// ���������� ����. ��������� ������ ������ ��� ���������� ���
int WINAPI AdcGetMaxInterruptDac(int typeDevice, int numberDSP, int *size);	// ���������� ����. ��������� ������ ������ ��� ���������� ���

int WINAPI AdcSetSizePacketAdc(int typeDevice, int numberDSP, int size);	// ���������� ������ ������ ������ DSP
int WINAPI AdcSetSizePacketDac(int typeDevice, int numberDSP, int size);	
int WINAPI AdcGetSizePacketAdc(int typeDevice, int numberDSP, int *size);	// ���������� ������������� ������ ������ ������ DSP
int WINAPI AdcGetSizePacketDac(int typeDevice, int numberDSP, int *size);
int WINAPI AdcGetMaxSizePacketAdc(int typeDevice, int numberDSP, int *size);	// ���������� ����. ��������� ������ ������ ������ DSP
int WINAPI AdcGetMaxSizePacketDac(int typeDevice, int numberDSP, int *size);	

int WINAPI AdcSetQuantityPacketsAdc(int typeDevice, int numberDSP, int size);	// ���������� ���-�� ������� �� ���� ����������
int WINAPI AdcSetQuantityPacketsDac(int typeDevice, int numberDSP, int size);
int WINAPI AdcGetQuantityPacketsAdc(int typeDevice, int numberDSP, int *size);	// ���������� ������������� ���-�� ������� �� ���� ����������
int WINAPI AdcGetQuantityPacketsDac(int typeDevice, int numberDSP, int *size);
int WINAPI AdcGetMaxQuantityPacketsAdc(int typeDevice, int numberDSP, int *size);	// ���������� ����. ��������� ���-�� ������� �� ���� ����������
int WINAPI AdcGetMaxQuantityPacketsDac(int typeDevice, int numberDSP, int *size);

int WINAPI AdcEnaCycleSample(int typeDevice, int numberDSP, short ext);		// ����������� ��� ����������� ���������� � ����� ��� 
int WINAPI AdcEnaCycleSampleDac(int typeDevice, int numberDSP, short ext);	// ����������� ��� ����������� ���������� � ����� ���
int WINAPI AdcSetBufferSize(int typeDevice, int numberDSP, int size);		// ������ ������ ������ ��� � ���
int WINAPI AdcSetBufferSizeDac(int typeDevice, int numberDSP, int size);	// ������ ������ ������ ��� � ���
int WINAPI AdcGetBuffer(int typeDevice, int numberDSP, short **bufer, int *size);	// ��������� ����� ���
int WINAPI AdcGetBufferDac(int typeDevice, int numberDSP,short **bufer, int *size);	// ��������� ����� ���
int WINAPI AdcRemBuffer(int typeDevice, int numberDSP, short **buffer);		// ���������� ����� ���
int WINAPI AdcRemBufferDac(int typeDevice, int numberDSP, short **buffer);	// ���������� ����� ���
int WINAPI AdcGetPointer(int typeDevice, int numberDSP, ULONG *pointer);		// ������ ��������� ���
int WINAPI AdcGetPointerDac(int typeDevice, int numberDSP, ULONG *pointer);	// ������ ��������� ���
int WINAPI AdcChannel(int typeDevice, int numberDSP, long channelbit);		// ������� ����� ���������� ������� ���
int WINAPI AdcChannelOutDac(int typeDevice, int numberDSP, long channelbit);	// ������� ����� ���������� ������� ���
int WINAPI AdcSetInput(int typeDevice, int numberDSP, int channel, int enable);	// ���./����. �������� ����� ���
int WINAPI AdcGetInput(int typeDevice, int numberDSP, int channel, int *enable);	// ����� ��������� ��������� ������ ���
int WINAPI AdcSetOutDac(int typeDevice, int numberDSP, int channel, int enable);	// ���./����. �������� ����� ���
int WINAPI AdcGetOutDac(int typeDevice, int numberDSP, int channel, int *enable);	// ����� ��������� ��������� ������ ���	
int WINAPI AdcGetNumberInput(int typeDevice, int numberDSP, int *numberchannel);	// ����� ���������� ���������� ������� ���
int WINAPI AdcGetNumberOutDac(int typeDevice, int numberDSP, int *numberchannel);	// ����� ���������� ���������� ������� ���


// ������� ��� ������ ������ � ���:
int WINAPI AdcGetLastDataInt(int typeDevice, int numberDSP, int channel, short *buffer, unsigned long size); // �������� ���������� ������ ���
int WINAPI AdcModa(int typeDevice, int numberDSP, int moda);		// ���������� ����� ������ ���������� ��������� ������� ���
int WINAPI AdcGetModa(int typeDevice, int numberDSP, int *moda);	// ��������� ����� ������ ���������� ��������� ������� ���
int WINAPI AdcFindHCP(int typeDevice, int numberDSP, int *present);	// ��������� �������������� � ��������� �� ������ HCP
int WINAPI AdcGetHCP(int typeDevice, int numberDSP, int numberChannel, int *state);	// ��������� � ��������� ��������� ������ ������ HCP
int WINAPI AdcSetHCP(int typeDevice, int numberDSP, int numberChannel, int state);	// ���������� ��������� ��������� ������ ������ HCP

int WINAPI AdcGetListAmplify(int typeDevice, int numberDSP, int next, double *Amplify); //�������� ������ ��������� ����. ��������
int WINAPI AdcSetNextAmplify(int typeDevice, int numberDSP, int numberchannel, int next, double *Amplify);//���������� ��������� ����. ��������
int WINAPI AdcGetAmplify(int typeDevice, int numberDSP, int numberchannel, double *Amplify); // ��������� ����. ��������
int WINAPI AdcSetAmplify(int typeDevice, int numberDSP, int numberchannel, double InAmplify, double *OutAmplify);//���������� ����. ��������
int WINAPI AdcAmplify(int typeDevice, int numberDSP, int kanal, int ampl);	// ��������� ����. �������� (�������.)

int WINAPI AdcGetListPreAmplify(int typeDevice, int numberDSP, int next, double *Amplify); //�������� ������ ��������� ����. ������������
int WINAPI AdcSetNextPreAmplify(int typeDevice, int numberDSP, int numberchannel, int next, double *Amplify);	// ���������� ��������� ����. ������������
int WINAPI AdcSetPreAmplify(int typeDevice, int numberDSP, int numberchannel, double InAmplify, double *OutAmplify);	// ��������� ����. ������������
int WINAPI AdcGetPreAmplify(int typeDevice, int numberDSP, int numberchannel, double * OutAmplify);//���������� ����. ��������
int WINAPI AdcPreAmpl(int typeDevice, int numberDSP, int kanal, int ampl);		// ��������� ����. ������������ (�������.)

int WINAPI AdcSetInputDiff(int typeDevice, int numberDSP, int channel, int enable);//����������-�������� �������� ����� ��� ����� � ���������������� �����
int WINAPI AdcGetInputDiff(int typeDevice, int numberDSP, int channel, int *enable);//����� ���������������� ������ ��������� ������ ��� ����� 


// ������� ��� ������ ������ � ���:
int WINAPI AdcSetExtCycle(int typeDevice, int numberDSP, int trig);		// ���������� ������� ��� ���������� �������� � ���
int WINAPI AdcSetAtt(int typeDevice, int numberDSP, int channel, double att, double *setatt); // ���������� ���. ��������� �����������
int WINAPI AdcGetAtt(int typeDevice, int numberDSP, int channel, double *setatt); // ��������� ���. ��������� �����������
int WINAPI AdcAtten(int typeDevice, int numberDSP, int kanal, int ampl);		// ��������� ������������ ��������� ����������� (�������.)
