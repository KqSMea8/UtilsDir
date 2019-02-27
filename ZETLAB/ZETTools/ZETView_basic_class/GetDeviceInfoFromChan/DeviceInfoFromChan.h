#pragma once
#include <ZETView_basic_class/zetbasic.h>

// класс для SCADA компонентов, позволяющий узнать информацию о устройстве из структуры chanInfoS
class CDeviceInfoFromChan
{
public:
	CDeviceInfoFromChan();
	CDeviceInfoFromChan(_In_ chanInfoS &channel);
	~CDeviceInfoFromChan();
public://******************************************************************************************
	void SetChannelData(_In_ chanInfoS &channel);
	CString sGetName();			// получаем имя устройства
	CString sGetsSerial();		// получаем серийный номер устройства
	uint64_t ullGetSerial();	// получаем серийный номер устройства
	long lGetType();			// получаем номер типа устройства
	//*********************************************************************************************
	long GetADCDACDeviceSerial	(_In_ long lID);
	long GetADCDACDeviceType	(_In_ long lID);
	void Get7xxxDeviceInfo		(_In_ long lID, _In_ CString Name, 
								 _Out_opt_ long		*Type	= nullptr, 
								 _Out_opt_ uint64_t *Serial = nullptr);
private://*****************************************************************************************
	CString m_sTypeName;			// тип устройства
	long m_lDeviceType;				// номер типа устройства
	//*********************************************************************************************
	CString m_sDeviceSerial;		// серийный номер устройства
	uint64_t m_ullDeviceSerial;		// серийный номер устройства
	//*********************************************************************************************
	chanInfoS m_ChannelData;		// загруженная структура
};	//*********************************************************************************************

