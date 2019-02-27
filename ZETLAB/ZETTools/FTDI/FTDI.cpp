#include "FTDI.h"
#include "FTChipID.h"

#include <tchar.h>
#include <Dbt.h>

#pragma comment(lib, "ftd2xx.lib")
#pragma comment(lib, "FTChipID.lib")

using namespace std;

void GetFTDIDevsInfo(list<FT_DEVICE_LIST_INFO_NODE>& info_list)
{
	unsigned int devs_count(0);
	FT_DEVICE_LIST_INFO_NODE *pInfo;
	info_list.clear();

	if(FT_CreateDeviceInfoList((LPDWORD) &devs_count) != FT_OK)
		throw FTDIException(50);

	if(devs_count)
	{
		pInfo = (FT_DEVICE_LIST_INFO_NODE*) malloc(sizeof(FT_DEVICE_LIST_INFO_NODE)*devs_count);
	
		if(FT_GetDeviceInfoList(pInfo, (LPDWORD) &devs_count) != FT_OK)
		{
			free(pInfo);
			throw FTDIException(65);
		}
	
		while(devs_count--)
		{
			info_list.push_front(pInfo[devs_count]);
		}
		
		free(pInfo);
	}
}

void RegisterFTDINotification(HANDLE hWnd)
{
	DEV_BROADCAST_DEVICEINTERFACE dbch;
	dbch.dbcc_size = sizeof(dbch);
	dbch.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	
	GUID guid = {0x219d0508, 0x57a8, 0x4ff5, {0x97, 0xa1, 0xbd, 0x86, 0x58, 0x7c, 0x6c, 0x7e } };
	memcpy_s(&dbch.dbcc_classguid, sizeof(GUID), &guid, sizeof(GUID));
	
	dbch.dbcc_name[0] = '\0';
	RegisterDeviceNotification(hWnd, &dbch, DEVICE_NOTIFY_WINDOW_HANDLE);
}

CFTDIdev::CFTDIdev(unsigned int dev_idx) : m_Handle(0), m_Idx(0)
{
	if(FT_Open(dev_idx, &m_Handle) != FT_OK  || !m_Handle)
		throw FTDIException(51);
		
	ReadEEPROM();

	FT_Purge(m_Handle, FT_PURGE_RX | FT_PURGE_TX);

	if(FTID_GetChipIDFromHandle(m_Handle, &m_ChipID) != FT_OK)
	{
		m_ChipID = 0;
		//throw FTDIException(65);
	}
		
	
	FT_SetLatencyTimer(m_Handle, 1);
	FT_SetUSBParameters(m_Handle, 5, 5);
}

CFTDIdev::~CFTDIdev()
{
	try
	{
		if(m_Handle)
		{
			FT_ResetDevice(m_Handle);
			FT_Close(m_Handle);
		}
	}
	catch(...)
	{

	}
}

void CFTDIdev::SetSerial(CString& serial)
{
	int len(serial.GetLength());
	
	if(len > SERIAL_LEN - 1)
	{
		m_Serial.Right(SERIAL_LEN - 1);
	}
	else
	{
		int idx(SERIAL_LEN - len - 1);
		m_Serial = serial;
		while(idx--)
			m_Serial.Insert(0, _T("0"));
	}
};												

void CFTDIdev::SaveParams(void)
{
	WriteEEPROM();
}

bool CFTDIdev::IsAvailable(void)
{
	DWORD chip_id;
	if(FTID_GetChipIDFromHandle(m_Handle, &chip_id) != FT_OK)
		return false;
	return true;
}

void CFTDIdev::ReadUserData(void *pBuff, unsigned int start_byte, unsigned int num_bytes)
{
	if(start_byte + num_bytes > UA_SIZE)
		throw FTDIException(11);
	
	memcpy_s(pBuff, num_bytes, &m_UserData[start_byte], num_bytes);
}

void CFTDIdev::WriteUserData(void *pBuff, unsigned int start_byte, unsigned int num_bytes)
{
	if(start_byte + num_bytes > UA_SIZE)
		throw FTDIException(12);
	memcpy_s(&m_UserData[start_byte], num_bytes, pBuff, num_bytes);
}

void CFTDIdev::Recv(BYTE* pBuff, unsigned int num_bytes)
{
	DWORD read_bytes;
	/*
	while(num_bytes--)
	{
		if(FT_Read(m_Handle, pBuff++, 1, &read_bytes) != FT_OK || !read_bytes)
			throw FTDIException(57);
	}
	*/
	FT_STATUS status = FT_Read(m_Handle, pBuff, num_bytes, &read_bytes);
	if(status != FT_OK || num_bytes != read_bytes)
		throw FTDIException(57);
}

void CFTDIdev::Send(BYTE* pBuff, unsigned int num_bytes)
{
	DWORD write_bytes;
	/*
	while(num_bytes--)
		if(FT_Write(m_Handle, pBuff++, 1, &write_bytes) != FT_OK || !write_bytes)
			throw FTDIException(58);
	*/
	FT_STATUS status = FT_Write(m_Handle, pBuff, num_bytes, &write_bytes);
	if(status != FT_OK || num_bytes != write_bytes)
		throw FTDIException(58);
}

void CFTDIdev::SetBaudrate(BAUD_RATE br)
{
	switch(br)
	{
		case 0: return;
		case BR1200:
		case BR2400:
		case BR4800:
		case BR9600:
		case BR14400:
		case BR19200:
		case BR38400:
		case BR57600:
		case BR115200:
		case BR230400:
		case BR460800:
		case BR921600:
		case BR1500000:
		case BR3000000:
		{
			if(FT_SetBaudRate(m_Handle, br) != FT_OK)
				throw FTDIException(53);

			if(FT_SetFlowControl(m_Handle, DTR_CONTROL_DISABLE, (char) 0x00, (char) 0xFF) != FT_OK)
				throw FTDIException(55);

			DWORD timeout(br < 19200 ? 3600000/br : 100);
			if(FT_SetTimeouts(m_Handle, timeout, timeout) != FT_OK)
				throw FTDIException(56);
			break;
		}
		default: return; //throw FTDIException(13); //это вернуть

	}
	FT_Purge(m_Handle, FT_PURGE_RX | FT_PURGE_TX);
	FT_ResetDevice(m_Handle);
}

void CFTDIdev::SetParity(bool parity_flag)
{
	if(FT_SetDataCharacteristics(m_Handle, 8, ONESTOPBIT, parity_flag) != FT_OK)
		throw FTDIException(54);
	FT_Purge(m_Handle, FT_PURGE_RX | FT_PURGE_TX);
	FT_ResetDevice(m_Handle);
}

void CFTDIdev::ReadEEPROM()
{
	//FT_PROGRAM_DATA data;
	wchar_t w_str[64];

	char Manufacturer[64];
	char ManufacturerId[64];
	char Description[64];
	char SerialNumber[64];

	FT_DEVICE ftDevice;
	DWORD dwId;
	char cSerialNumber[16];
	char cDescription[64];

	FT_GetDeviceInfo(m_Handle, &ftDevice, &dwId, cSerialNumber, cDescription, NULL);

	if (ftDevice == FT_DEVICE_232H)
	{
		FT_EEPROM_HEADER ft_eeprom_header;
		ft_eeprom_header.deviceType = FT_DEVICE_232H;
		FT_EEPROM_232H ft_eeprom_232h;
		ft_eeprom_232h.common = ft_eeprom_header;
		ft_eeprom_232h.common.deviceType = FT_DEVICE_232H;

		if (FT_EEPROM_Read(m_Handle, &ft_eeprom_232h, sizeof(ft_eeprom_232h), Manufacturer, ManufacturerId, Description, SerialNumber) != FT_OK)
			throw FTDIException(59);

		m_VID = ft_eeprom_232h.common.VendorId;
		m_PID = ft_eeprom_232h.common.ProductId;
	}
	if (ftDevice == FT_DEVICE_232R)
	{
		FT_EEPROM_HEADER ft_eeprom_header;
		ft_eeprom_header.deviceType = FT_DEVICE_232R;
		FT_EEPROM_232R ft_eeprom_232r;
		ft_eeprom_232r.common = ft_eeprom_header;
		ft_eeprom_232r.common.deviceType = FT_DEVICE_232R;

		if (FT_EEPROM_Read(m_Handle, &ft_eeprom_232r, sizeof(ft_eeprom_232r), Manufacturer, ManufacturerId, Description, SerialNumber) != FT_OK)
			throw FTDIException(59);

		m_VID = ft_eeprom_232r.common.VendorId;
		m_PID = ft_eeprom_232r.common.ProductId;
	}

	//data.Manufacturer = Manufacturer;
	//data.Description = Description;
	//data.SerialNumber = SerialNumber;
	//data.ManufacturerId = ManufacturerId;

	//if(FT_EE_Read(m_Handle, &data) != FT_OK)
	//	throw FTDIException(59);

	//m_VID = data.VendorId;
	//m_PID = data.ProductId;

	Manufacturer[MANUFACTERER_LEN - 1] = 0x00;
	Description[DESCRIPTION_LEN - 1] = 0x00;
	SerialNumber[SERIAL_LEN - 1] = 0x00;
	ManufacturerId[0] = 0x00;					//Не используется

	MultiByteToWideChar(CP_ACP, 0, Manufacturer, MANUFACTERER_LEN, w_str, MANUFACTERER_LEN);
	m_Vendor = w_str;
	MultiByteToWideChar(CP_ACP, 0, Description, DESCRIPTION_LEN, w_str, DESCRIPTION_LEN);
	m_DevName = w_str;
	MultiByteToWideChar(CP_ACP, 0, SerialNumber, SERIAL_LEN, w_str, SERIAL_LEN);
	SetSerial(CString(w_str));

	DWORD size;
	DWORD read_bytes;
	
	if(FT_EE_UASize(m_Handle, &size) != FT_OK)
		throw FTDIException(63);
	
	if(size > UA_SIZE)
		size = UA_SIZE;
	memset(m_UserData, 0x00, UA_SIZE);
	
	if(size)
		if(FT_EE_UARead(m_Handle, m_UserData, size, &read_bytes) != FT_OK || read_bytes != size)
			throw FTDIException(61);
}

void CFTDIdev::WriteEEPROM()
{
// 	FT_PROGRAM_DATA data;
	FT_DEVICE ftDevice;
	DWORD dwId;
	char cSerialNumber[16];
	char cDescription[64];

	FT_GetDeviceInfo(m_Handle, &ftDevice, &dwId, cSerialNumber, cDescription, NULL);

	char Manufacturer[64];
	char ManufacturerId[64];
	char Description[64];
	char SerialNumber[64];
	
	if (ftDevice == FT_DEVICE_232H)
	{
		FT_EEPROM_HEADER ft_eeprom_header;
		ft_eeprom_header.deviceType = FT_DEVICE_232H;
		FT_EEPROM_232H ft_eeprom_232h;
		ft_eeprom_232h.common = ft_eeprom_header;
		ft_eeprom_232h.common.deviceType = FT_DEVICE_232H;

		if (FT_EEPROM_Read(m_Handle, &ft_eeprom_232h, sizeof(ft_eeprom_232h), Manufacturer, ManufacturerId, Description, SerialNumber) != FT_OK)
			throw FTDIException(59);

		ft_eeprom_232h.common.VendorId = m_VID;
		ft_eeprom_232h.common.ProductId = m_PID;
		ManufacturerId[0] = 0x00;					//Не используется
		WideCharToMultiByte(CP_ACP, 0, m_Vendor.GetBuffer(), MANUFACTERER_LEN, Manufacturer, MANUFACTERER_LEN, NULL, NULL);
		WideCharToMultiByte(CP_ACP, 0, m_DevName.GetBuffer(), DESCRIPTION_LEN, Description, DESCRIPTION_LEN, NULL, NULL);
		WideCharToMultiByte(CP_ACP, 0, m_Serial.GetBuffer(), SERIAL_LEN, SerialNumber, SERIAL_LEN, NULL, NULL);
		
		if(FT_EEPROM_Program(m_Handle, &ft_eeprom_232h, sizeof(ft_eeprom_232h), Manufacturer, ManufacturerId, Description, SerialNumber) != FT_OK)
			throw FTDIException(60);
	}

	if (ftDevice == FT_DEVICE_232R)
	{
		FT_EEPROM_HEADER ft_eeprom_header;
		ft_eeprom_header.deviceType = FT_DEVICE_232R;
		FT_EEPROM_232R ft_eeprom_232r;
		ft_eeprom_232r.common = ft_eeprom_header;
		ft_eeprom_232r.common.deviceType = FT_DEVICE_232R;

		if (FT_EEPROM_Read(m_Handle, &ft_eeprom_232r, sizeof(ft_eeprom_232r), Manufacturer, ManufacturerId, Description, SerialNumber) != FT_OK)
			throw FTDIException(59);

		ft_eeprom_232r.common.VendorId = m_VID;
		ft_eeprom_232r.common.ProductId = m_PID;
		ManufacturerId[0] = 0x00;					//Не используется
		WideCharToMultiByte(CP_ACP, 0, m_Vendor.GetBuffer(), MANUFACTERER_LEN, Manufacturer, MANUFACTERER_LEN, NULL, NULL);
		WideCharToMultiByte(CP_ACP, 0, m_DevName.GetBuffer(), DESCRIPTION_LEN, Description, DESCRIPTION_LEN, NULL, NULL);
		WideCharToMultiByte(CP_ACP, 0, m_Serial.GetBuffer(), SERIAL_LEN, SerialNumber, SERIAL_LEN, NULL, NULL);

		if(FT_EEPROM_Program(m_Handle, &ft_eeprom_232r, sizeof(ft_eeprom_232r), Manufacturer, ManufacturerId, Description, SerialNumber) != FT_OK)
			throw FTDIException(60);
	}

	//data.Manufacturer = Manufacturer;
	//data.Description = Description;
	//data.SerialNumber = SerialNumber;
	//data.ManufacturerId = ManufacturerId;

	//if(FT_EE_Read(m_Handle, &data) != FT_OK)
	//	throw FTDIException(59);

	//ManufacturerId[0] = 0x00;					//Не используется

	//data.Signature1 =	0x00000000;
	//data.Signature2 =	0xffffffff;
	//data.Version =		0x00000002;
	
	//data.VendorId = m_VID;
	//data.ProductId = m_PID;

	//WideCharToMultiByte(CP_ACP, 0, m_Vendor.GetBuffer(), MANUFACTERER_LEN, Manufacturer, MANUFACTERER_LEN, NULL, NULL);
	//WideCharToMultiByte(CP_ACP, 0, m_DevName.GetBuffer(), DESCRIPTION_LEN, Description, DESCRIPTION_LEN, NULL, NULL);
	//WideCharToMultiByte(CP_ACP, 0, m_Serial.GetBuffer(), SERIAL_LEN, SerialNumber, SERIAL_LEN, NULL, NULL);
	
	
	//if(FT_EE_Program(m_Handle, &data) != FT_OK)
	//	throw FTDIException(60);

	DWORD size;
	
	if(FT_EE_UASize(m_Handle, &size) != FT_OK)
		throw FTDIException(63);
	
	if(size < UA_SIZE)
		throw  FTDIException(62);

	if(FT_EE_UAWrite(m_Handle, m_UserData, UA_SIZE) != FT_OK)
		throw FTDIException(62);
}