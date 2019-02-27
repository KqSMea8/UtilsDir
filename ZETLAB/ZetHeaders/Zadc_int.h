// �������������� ���� ���������
#define KD1610		0	// ADC 16/200
#define KD216		1	// APC 216
#define KD500		2	// ADC 16/500
#define KD500P		3	// ADC 16/500P
#define KD816		4	// ADC 816
#define KD1002		5	// ADC 1002
#define KDU216		6	// APC 216 USB
#define KD24		7	// ADC 24
#define KD1432		8	// ADC 1432
#define KDUACPB		9	// ACPB USB
#define KDU1616		10	// ZET210
#define KDUPD14		11	// PD14 USB
#define KDUVN		12	// ZET110
#define KDUOSC		13	// ZET302
#define KDU8500		14	// A17U8
#define KDU2500		15	// A17U2
#define KDU1624		16	// ZET220
#define KDU0424		17	// ZET230
#define KDU0414		18	// ZET
#define KDU0824		19	// ZET240


//��� ������� ���������� ��� ������. 0 - ��� ������, ����� - ��� ������

// ------------- ����� ������� -------------
// ������������ � ��������
__declspec(dllimport) long WINAPI ZOpen(long typeDevice, long numberDSP);
// ����������� �� ��������
__declspec(dllimport) long WINAPI ZClose(long typeDevice, long numberDSP);

// ����� � ������� ���������� ����������� (������ �� ��� DSP ������ ����������)
__declspec(dllimport) long WINAPI ZResetDSP(long typeDevice, long numberDSP);
// ������������������� ���������� ���������
__declspec(dllimport) long WINAPI ZInitDSP(long typeDevice, long numberDSP, const char *fileName);
// �������� �������� ����� DSP
__declspec(dllimport) long WINAPI ZGetSerialNumberDSP(long typeDevice, long numberDSP, long *serialNumber);
// �������� ��� ���������� (������������ ������ �� ����� 16 �������� ������ � ����������� 0, maxSizeStr - ������ ������ strName[])
__declspec(dllimport) long WINAPI ZGetNameDevice(long typeDevice, long numberDSP, char *strName, long maxSizeStr);
// �������� ������ �� (������������ ������ �� ����� 64 �������� ������ � ����������� 0)
__declspec(dllimport) long WINAPI ZGetVersion(long typeDevice, long numberDSP, char *verDSP, char *verDRV, char *verLIB);
// �������� ��� ���������� (1 - PCI, 2 - USB, 3 - Ethernet)
__declspec(dllimport) long WINAPI ZGetTypeConnection(long typeDevice, long numberDSP, long *type);
// ��������� ��� ������
__declspec(dllimport) long WINAPI ZGetError(long typeDevice, long numberDSP, long *error);
// ���������� ���-�� ��������� ���������� � ������� ��������
__declspec(dllimport) long WINAPI ZGetModify(long typeDevice, long numberDSP, long *modify);

// ��������� ���� - ���������� ���������� DSP (�������) � ������� ZStartADC()
__declspec(dllimport) long WINAPI ZGetFlag(long typeDevice, long numberDSP, unsigned long *flag);

// ��������� ������ ������������� �� ������� ������� (���������� �������, �� ��������������� ������� ��� ��� � ���)
__declspec(dllimport) long WINAPI ZGetEnableExtFreq(long typeDevice, long numberDSP, long *enable);
// ���./����. ������������� �� ������� ������� (���������� �������, �� ��������������� ������� ��� ��� � ���)
__declspec(dllimport) long WINAPI ZSetEnableExtFreq(long typeDevice, long numberDSP, long enable);
// ��������� ������ �������� ������� (���������� �������, �� ��������������� ������� ��� ��� � ���)
__declspec(dllimport) long WINAPI ZGetEnableExtStart(long typeDevice, long numberDSP, long *enable);
// ���./����. �������� ������� (���������� �������, �� ��������������� ������� ��� ��� � ���)
__declspec(dllimport) long WINAPI ZSetEnableExtStart(long typeDevice, long numberDSP, long enable);

// ��������� ���� ������ ��� ���� PnP
__declspec(dllimport) long WINAPI ZTestCode(long typeDevice, long numberDSP, long *code0, long *code1, long *code2);


// ��������� ���-�� ����� ��������� �����
__declspec(dllimport) long WINAPI ZGetQuantityChannelDigPort(long typeDevice, long numberDSP, long *quantityChannel);
// ��������� ����� ������� �����
__declspec(dllimport) long WINAPI ZGetDigOutEnable(long typeDevice, long numberDSP, unsigned long *digitalOutEnableMask);
// ���������� ����� ������� �����
__declspec(dllimport) long WINAPI ZSetDigOutEnable(long typeDevice, long numberDSP, unsigned long digitalOutEnableMask);
// ���������� ����� ��������� ����� �� ����� (��� ��������� ��������� ������ �����, numberOfBit = 0..quantityChannel-1)
__declspec(dllimport) long WINAPI ZSetBitDigOutEnable(long typeDevice, long numberDSP, long numberOfBit);
// ���������� ����� ����� ��������� ����� ����� (��� ��������� ��������� ������ �����)
__declspec(dllimport) long WINAPI ZSetBitMaskDigOutEnable(long typeDevice, long numberDSP, unsigned long maskOfBits);
// ���������� ����� ��������� ����� �� ���� (��� ��������� ��������� ������ �����, numberOfBit = 0..quantityChannel-1)
__declspec(dllimport) long WINAPI ZClrBitDigOutEnable(long typeDevice, long numberDSP, long numberOfBit);
// ���������� ����� ����� ��������� ����� ���� (��� ��������� ��������� ������ �����)
__declspec(dllimport) long WINAPI ZClrBitMaskDigOutEnable(long typeDevice, long numberDSP, unsigned long maskOfBits);
// ��������� ������ �� ������ �����
__declspec(dllimport) long WINAPI ZGetDigInput(long typeDevice, long numberDSP, unsigned long *digitalInput);
// ��������� ������, ���������� �� ������ �����
__declspec(dllimport) long WINAPI ZGetDigOutput(long typeDevice, long numberDSP, unsigned long *digitalOutput);
// �������� ������ � ����
__declspec(dllimport) long WINAPI ZSetDigOutput(long typeDevice, long numberDSP, unsigned long digitalOutput);
// ���������� ����� ��������� ����� � "1" (��� ��������� ��������� ������ �������, numberOfBit = 0..quantityChannel-1)
__declspec(dllimport) long WINAPI ZSetBitDigOutput(long typeDevice, long numberDSP, long numberOfBit);
// ���������� ����� ������� ��������� ����� � "1" (��� ��������� ��������� ������ �������)
__declspec(dllimport) long WINAPI ZSetBitMaskDigOutput(long typeDevice, long numberDSP, unsigned long maskOfBits);
// ���������� ����� ��������� ����� � "0" (��� ��������� ��������� ������ �������, numberOfBit = 0..quantityChannel-1)
__declspec(dllimport) long WINAPI ZClrBitDigOutput(long typeDevice, long numberDSP, long numberOfBit);
// ���������� ����� ������� ��������� ����� � "0" (��� ��������� ��������� ������ �������)
__declspec(dllimport) long WINAPI ZClrBitMaskDigOutput(long typeDevice, long numberDSP, unsigned long maskOfBits);
// �������� ����� ���������� �����-������ (mode=0 - ���������� ����, mode=1 - ��������� ����, mode=2 - UART)
										// +0x04 - ��������������� UART
										// +0x08 - 9 ��� UART
										// +0x10 - 2 ����-���� UART (�� ���.)
										// +0x20 - �������� �������� UART (�� ���.)
										// +0x40 - IrDA UART (�� ���.)
__declspec(dllimport) long WINAPI ZGetDigitalMode(long typeDevice, long numberDSP, long *mode);
// ����������� ����� ���������� �����-������ (mode=0 - ���������� ����, mode=1 - ��������� ����, mode=2 - UART)
__declspec(dllimport) long WINAPI ZSetDigitalMode(long typeDevice, long numberDSP, long mode);
// ����� ���������� ��������� �������� ������������� �� �������� ����  
__declspec(dllimport) long WINAPI ZGetMasterSynchr(long typeDevice, long numberDSP, long *enable);
// ��������� ��������� �������� ������������� �� �������� ����  
__declspec(dllimport) long WINAPI ZSetMasterSynchr(long typeDevice, long numberDSP, long enable);


// ��������� �������������� �� ������ ���
__declspec(dllimport) long WINAPI ZFindPWM(long typeDevice, long numberDSP, long* present);
// ������ ������� ���
__declspec(dllimport) long WINAPI ZStartPWM(long typeDevice, long numberDSP, long Start0, long Start1, long Start2);
// ������� ������� ���
__declspec(dllimport) long WINAPI ZStopPWM(long typeDevice, long numberDSP, long Stop0, long Stop1, long Stop2);
// ������ ������� ��� ����� ����. ������� ������� ������� � ������
__declspec(dllimport) long WINAPI ZSetFreqPWM(long typeDevice, long numberDSP, long Rate, long Period);
// ������ ���������� � ����� ������� ���
__declspec(dllimport) long WINAPI ZSetOnDutyPWM(long typeDevice, long numberDSP, long OnDutyPWM0, long OnDutyPWM1, long OnDutyPWM2, long ShiftPWM1, long ShiftPWM2);
// ����� �������� ������������� ���������� � DSP
__declspec(dllimport)long WINAPI ZRegulatorPWM(long typeDevice, long numberDSP, void *data, long *size);


// ------------- ������ ������� ��� ������ � ��� ��� ��� -------------
// ��������� ��� � ������� ������ DSP
__declspec(dllimport) long WINAPI ZGetEnableADC(long typeDevice, long numberDSP, long *enable);
// ��������� ��� � ������� ������ DSP
__declspec(dllimport) long WINAPI ZGetEnableDAC(long typeDevice, long numberDSP, long *enable);
// ���������� ���������� ��������� � ����� ���/���
__declspec(dllimport) long WINAPI ZSetTypeADC(long typeDevice, long numberDSP);
__declspec(dllimport) long WINAPI ZSetTypeDAC(long typeDevice, long numberDSP);
// ������ � ����� ��������� ��������� ���/���
__declspec(dllimport) long WINAPI ZGetStartADC(long typeDevice, long numberDSP, long *status);
__declspec(dllimport) long WINAPI ZGetStartDAC(long typeDevice, long numberDSP, long *status);
// ������ ��������� �����/������ �������
__declspec(dllimport) long WINAPI ZStartADC(long typeDevice, long numberDSP);
__declspec(dllimport) long WINAPI ZStartDAC(long typeDevice, long numberDSP);
// ������� ��������� �����/������
__declspec(dllimport) long WINAPI ZStopADC(long typeDevice, long numberDSP);
__declspec(dllimport) long WINAPI ZStopDAC(long typeDevice, long numberDSP);

// ��������� ���-�� ������� ���/���
__declspec(dllimport) long WINAPI ZGetQuantityChannelADC(long typeDevice, long numberDSP, long *quantityChannel);
__declspec(dllimport) long WINAPI ZGetQuantityChannelDAC(long typeDevice, long numberDSP, long *quantityChannel);
// ��������� ��� �������� ������� ���/��� (���������� �������)
__declspec(dllimport) long WINAPI ZGetDigitalResolutionADC(long typeDevice, long numberDSP, double *digitalResolution);
__declspec(dllimport) long WINAPI ZGetDigitalResolutionDAC(long typeDevice, long numberDSP, double *digitalResolution);
// ��������� ��������������� ���������� ��� �������� ������� ���/���
__declspec(dllimport) long WINAPI ZGetDigitalResolChanADC(long typeDevice, long numberDSP, long numberChannel, double *digitalResolChan);
__declspec(dllimport) long WINAPI ZGetDigitalResolChanDAC(long typeDevice, long numberDSP, long numberChannel, double *digitalResolChan);
// ��������� ���-�� ��� � ����� ������� ���/���
__declspec(dllimport) long WINAPI ZGetBitsADC(long typeDevice, long numberDSP, long *numberBits);
__declspec(dllimport) long WINAPI ZGetBitsDAC(long typeDevice, long numberDSP, long *numberBits);
// ��������� ���-�� ���� � ����� ������� ���/���
__declspec(dllimport) long WINAPI ZGetWordsADC(long typeDevice, long numberDSP, long *numberWords);
__declspec(dllimport) long WINAPI ZGetWordsDAC(long typeDevice, long numberDSP, long *numberWords);

// ��������� ������� ������������� 
__declspec(dllimport) long WINAPI ZGetFreqADC(long typeDevice, long numberDSP, double *freq);
__declspec(dllimport) long WINAPI ZGetFreqDAC(long typeDevice, long numberDSP, double *freq);
// ��������� ������� �� ������ ��������� ������ ������������� 
__declspec(dllimport) long WINAPI ZGetListFreqADC(long typeDevice, long numberDSP, long next, double *freq);
__declspec(dllimport) long WINAPI ZGetListFreqDAC(long typeDevice, long numberDSP, long next, double *freq);
// ���������� ��������� �� ������ ������� ������������� 
__declspec(dllimport) long WINAPI ZSetNextFreqADC(long typeDevice, long numberDSP, long next, double *freq);
__declspec(dllimport) long WINAPI ZSetNextFreqDAC(long typeDevice, long numberDSP, long next, double *freq);
// ���������� ������� �������������
__declspec(dllimport) long WINAPI ZSetFreqADC(long typeDevice, long numberDSP, double freqIn, double *freqOut);
__declspec(dllimport) long WINAPI ZSetFreqDAC(long typeDevice, long numberDSP, double freqIn, double *freqOut);

// ��������� �������� ������� ������� �������
__declspec(dllimport) long WINAPI ZGetExtFreqADC(long typeDevice, long numberDSP, double *oporFreq);
__declspec(dllimport) long WINAPI ZGetExtFreqDAC(long typeDevice, long numberDSP, double *oporFreq);
// ���������� �������� ������� ������� �������
__declspec(dllimport) long WINAPI ZSetExtFreqADC(long typeDevice, long numberDSP, double extFreq);
__declspec(dllimport) long WINAPI ZSetExtFreqDAC(long typeDevice, long numberDSP, double extFreq);
// ��������� ������ ������������� �� ������� ������� 
__declspec(dllimport) long WINAPI ZGetEnableExtFreqADC(long typeDevice, long numberDSP, long *enable);
__declspec(dllimport) long WINAPI ZGetEnableExtFreqDAC(long typeDevice, long numberDSP, long *enable);
// ���./����. ������������� �� ������� �������
__declspec(dllimport) long WINAPI ZSetEnableExtFreqADC(long typeDevice, long numberDSP, long enable);
__declspec(dllimport) long WINAPI ZSetEnableExtFreqDAC(long typeDevice, long numberDSP, long enable);
// ��������� ������ �������� �������
__declspec(dllimport) long WINAPI ZGetEnableExtStartADC(long typeDevice, long numberDSP, long *enable);
__declspec(dllimport) long WINAPI ZGetEnableExtStartDAC(long typeDevice, long numberDSP, long *enable);
// ���./����. �������� �������
__declspec(dllimport) long WINAPI ZSetEnableExtStartADC(long typeDevice, long numberDSP, long enable);
__declspec(dllimport) long WINAPI ZSetEnableExtStartDAC(long typeDevice, long numberDSP, long enable);

// ���������� ������������� ������ ������ ��� ����������
__declspec(dllimport) long WINAPI ZGetInterruptADC(long typeDevice, long numberDSP, long *size);
__declspec(dllimport) long WINAPI ZGetInterruptDAC(long typeDevice, long numberDSP, long *size);
// ���������� ����. ��������� ������ ������ ��� ����������
__declspec(dllimport) long WINAPI ZGetMaxInterruptADC(long typeDevice, long numberDSP, long *size);
__declspec(dllimport) long WINAPI ZGetMaxInterruptDAC(long typeDevice, long numberDSP, long *size);
// ���������� ������ ������ ��� ��������� ��� ����������
__declspec(dllimport) long WINAPI ZSetInterruptADC(long typeDevice, long numberDSP, long size);
__declspec(dllimport) long WINAPI ZSetInterruptDAC(long typeDevice, long numberDSP, long size);

// ���������� ������������� ������ ������ ������ DSP
__declspec(dllimport) long WINAPI ZGetSizePacketADC(long typeDevice, long numberDSP, long *size);
__declspec(dllimport) long WINAPI ZGetSizePacketDAC(long typeDevice, long numberDSP, long *size);
// ���������� ����. ��������� ������ ������ ������ DSP
__declspec(dllimport) long WINAPI ZGetMaxSizePacketADC(long typeDevice, long numberDSP, long *size);
__declspec(dllimport) long WINAPI ZGetMaxSizePacketDAC(long typeDevice, long numberDSP, long *size);
// ���������� ������ ������ ������ DSP
__declspec(dllimport) long WINAPI ZSetSizePacketADC(long typeDevice, long numberDSP, long size);
__declspec(dllimport) long WINAPI ZSetSizePacketDAC(long typeDevice, long numberDSP, long size);

// ���������� ������������� ���-�� ������� �� ���� ����������
__declspec(dllimport) long WINAPI ZGetQuantityPacketsADC(long typeDevice, long numberDSP, long *size);
__declspec(dllimport) long WINAPI ZGetQuantityPacketsDAC(long typeDevice, long numberDSP, long *size);
// ���������� ����. ��������� ���-�� ������� �� ���� ����������
__declspec(dllimport) long WINAPI ZGetMaxQuantityPacketsADC(long typeDevice, long numberDSP, long *size);
__declspec(dllimport) long WINAPI ZGetMaxQuantityPacketsDAC(long typeDevice, long numberDSP, long *size);
// ���������� ���-�� ������� �� ���� ����������
__declspec(dllimport) long WINAPI ZSetQuantityPacketsADC(long typeDevice, long numberDSP, long size);
__declspec(dllimport) long WINAPI ZSetQuantityPacketsDAC(long typeDevice, long numberDSP, long size);

// ���������� ����������� ��� ����������� ���������� � ����� ��� �� 
__declspec(dllimport) long WINAPI ZSetCycleSampleADC(long typeDevice, long numberDSP, long enable);
__declspec(dllimport) long WINAPI ZSetCycleSampleDAC(long typeDevice, long numberDSP, long enable);

// ������ ������ ������ ��� � ��� ��
__declspec(dllimport) long WINAPI ZSetBufferSizeADC(long typeDevice, long numberDSP, long size);
__declspec(dllimport) long WINAPI ZSetBufferSizeDAC(long typeDevice, long numberDSP, long size);
// ��������� ����� � ��� ��
__declspec(dllimport) long WINAPI ZGetBufferADC(long typeDevice, long numberDSP, void **buffer, long *size);
__declspec(dllimport) long WINAPI ZGetBufferDAC(long typeDevice, long numberDSP, void **buffer, long *size);
// ���������� ����� � ��� ��
__declspec(dllimport) long WINAPI ZRemBufferADC(long typeDevice, long numberDSP, void **buffer);
__declspec(dllimport) long WINAPI ZRemBufferDAC(long typeDevice, long numberDSP, void **buffer);
// ������ ��������� ������ � ��� ��
__declspec(dllimport) long WINAPI ZGetPointerADC(long typeDevice, long numberDSP, long *pointer);
__declspec(dllimport) long WINAPI ZGetPointerDAC(long typeDevice, long numberDSP, long *pointer);

// ����� ���������� ���������� �������
__declspec(dllimport) long WINAPI ZGetNumberInputADC(long typeDevice, long numberDSP, long *workChannels);
__declspec(dllimport) long WINAPI ZGetNumberOutputDAC(long typeDevice, long numberDSP, long *workChannels);
// ����� ������� �� �������� �����
__declspec(dllimport) long WINAPI ZGetInputADC(long typeDevice, long numberDSP, long numberChannel, long *enable);
__declspec(dllimport) long WINAPI ZGetOutputDAC(long typeDevice, long numberDSP, long numberChannel, long *enable);
// ��������/��������� �������� �����
__declspec(dllimport) long WINAPI ZSetInputADC(long typeDevice, long numberDSP, long numberChannel, long enable);
__declspec(dllimport) long WINAPI ZSetOutputDAC(long typeDevice, long numberDSP, long numberChannel, long enable);


// ------------- ������� ��� ������ ������ � ��� -------------
// �������� ���������� ������ ���
__declspec(dllimport) long WINAPI ZGetLastDataADC(long typeDevice, long numberDSP, long numberChannel, void *buffer, long size);

// ��������� ����. ��������
__declspec(dllimport) long WINAPI ZGetAmplifyADC(long typeDevice, long numberDSP, long numberChannel, double *amplify);
// ��������� ����. �������� �� ������ ��������� ����. ��������
__declspec(dllimport) long WINAPI ZGetListAmplifyADC(long typeDevice, long numberDSP, long next, double *amplify);
// ���������� ��������� ����. ��������
__declspec(dllimport) long WINAPI ZSetNextAmplifyADC(long typeDevice, long numberDSP, long numberChannel, long next, double *amplify);
// ���������� ����. ��������
__declspec(dllimport) long WINAPI ZSetAmplifyADC(long typeDevice, long numberDSP, long numberChannel, double amplifyIn, double *amplifyOut);

// ��������� ����. �������� ��8/10
__declspec(dllimport) long WINAPI ZGetPreAmplifyADC(long typeDevice, long numberDSP, long numberChannel, double *amplify);
// ��������� ����. �������� �� ������ ��������� ����. �������� ��8/10
__declspec(dllimport) long WINAPI ZGetListPreAmplifyADC(long typeDevice, long numberDSP, long next, double *amplify);
// ���������� ��������� ����. �������� ��8/10
__declspec(dllimport) long WINAPI ZSetNextPreAmplifyADC(long typeDevice, long numberDSP, long numberChannel, long next, double *amplify);
// ���������� ����. �������� ��8/10
__declspec(dllimport) long WINAPI ZSetPreAmplifyADC(long typeDevice, long numberDSP, long numberChannel, double amplifyIn, double *amplifyOut);

// ��������� �������������� � ��������� �� ������ HCP
__declspec(dllimport) long WINAPI ZFindHCPADC(long typeDevice, long numberDSP, long *present);
// ��������� � ��������� ��������� ������ ������ HCP
__declspec(dllimport) long WINAPI ZGetHCPADC(long typeDevice, long numberDSP, long numberChannel, long *enable);
// ���������� ��������� ��������� ������ ������ HCP
__declspec(dllimport) long WINAPI ZSetHCPADC(long typeDevice, long numberDSP, long numberChannel, long enable);

// ����� ����������������� ������ ��������� ������ ��� ����� 
__declspec(dllimport) long WINAPI ZGetInputDiffADC(long typeDevice, long numberDSP, long numberChannel, long *enable);
// ����������-�������� �������� ����� ��� ����� � ���������������� �����
__declspec(dllimport) long WINAPI ZSetInputDiffADC(long typeDevice, long numberDSP, long numberChannel, long enable);


// ------------- ������� ��� ������ ������ � ��� -------------
// ���������� ������� (�� ������ ��) ��� ���������� (������ �� ������ DSP) �������� � ���
__declspec(dllimport) long WINAPI ZSetExtCycleDAC(long typeDevice, long numberDSP, long enable);

// �������� �������������� �� ����������� ����������
__declspec(dllimport) long WINAPI ZFindSoftAtten(long typeDevice, long numberDSP, long *present);
// ��������� ���. ��������� �����������
__declspec(dllimport) long WINAPI ZGetAttenDAC(long typeDevice, long numberDSP, long numberChannel, double *reduction);
// ���������� ���. ��������� �����������
__declspec(dllimport) long WINAPI ZSetAttenDAC(long typeDevice, long numberDSP, long numberChannel, double reductionIn, double *reductionOut);

// ����� ������������� ������� ������ ��� � DSP
__declspec(dllimport) long WINAPI ZGetMaxSizeBufferDSPDAC(long typeDevice, long numberDSP, long *size);
// ����� ������� ������ ��� � DSP
__declspec(dllimport) long WINAPI ZGetSizeBufferDSPDAC(long typeDevice, long numberDSP, long *size);
// ���������� ������ ������ ��� � DSP
__declspec(dllimport) long WINAPI ZSetSizeBufferDSPDAC(long typeDevice, long numberDSP, long size);




