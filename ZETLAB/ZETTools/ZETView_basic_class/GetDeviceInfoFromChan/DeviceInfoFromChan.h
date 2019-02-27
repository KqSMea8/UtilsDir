#pragma once
#include <ZETView_basic_class/zetbasic.h>

// ����� ��� SCADA �����������, ����������� ������ ���������� � ���������� �� ��������� chanInfoS
class CDeviceInfoFromChan
{
public:
	CDeviceInfoFromChan();
	CDeviceInfoFromChan(_In_ chanInfoS &channel);
	~CDeviceInfoFromChan();
public://******************************************************************************************
	void SetChannelData(_In_ chanInfoS &channel);
	CString sGetName();			// �������� ��� ����������
	CString sGetsSerial();		// �������� �������� ����� ����������
	uint64_t ullGetSerial();	// �������� �������� ����� ����������
	long lGetType();			// �������� ����� ���� ����������
	//*********************************************************************************************
	long GetADCDACDeviceSerial	(_In_ long lID);
	long GetADCDACDeviceType	(_In_ long lID);
	void Get7xxxDeviceInfo		(_In_ long lID, _In_ CString Name, 
								 _Out_opt_ long		*Type	= nullptr, 
								 _Out_opt_ uint64_t *Serial = nullptr);
private://*****************************************************************************************
	CString m_sTypeName;			// ��� ����������
	long m_lDeviceType;				// ����� ���� ����������
	//*********************************************************************************************
	CString m_sDeviceSerial;		// �������� ����� ����������
	uint64_t m_ullDeviceSerial;		// �������� ����� ����������
	//*********************************************************************************************
	chanInfoS m_ChannelData;		// ����������� ���������
};	//*********************************************************************************************

